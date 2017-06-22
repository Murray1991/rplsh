#include "interpreter.hpp"
#include "cpp_parser/cpp_parser.hpp"
#include "rewriting/rewriter.hpp"
#include "utils/mytuple.hpp"
#include "utils/rank.hpp"
#include "utils/utils.hpp"
#include <memory>
#include <iostream>
#include <fstream>

using namespace std;

single_node_cloner snc;

///////////////////////////////////////////////////////////////////////////////

interpreter::interpreter(rpl_environment& env, error_container& err_repo) :
    env(env),
    err_repo(err_repo),
    adispatch(env),
    sdispatch(env),
    gdispatch(env),
    odispatch(env),
    vdispatch(env),
    normform(env),
    ff(env),
    success(true)
{}

void interpreter::visit(assign_node& n) {
    // recurse for semantic check in skel tree
    id_node* idnode = dynamic_cast<id_node*>(n.rvalue);
    n.rvalue->accept(*this);

    if ( success && !idnode) {
        unranktorank2(*n.rvalue, snc);
        env.put(n.id, n.rvalue);
    } else if ( success ) {
        skel_node& skel = *env.get(idnode->id, idnode->index);
        env.put(n.id, skel.clone());
        delete n.rvalue;
    } else
        delete n.rvalue;

}

void interpreter::visit(show_node& n) {
    try {

        if ( n.id == "" && n.parameters.size() == 1) {
            cout << utils::to_string( gdispatch[n.parameters[0]]() ) << endl;
            return;
        }

        ann_printer p;
        vector<mytuple> tuples;
        auto range = env.range( n.id );
        auto index = n.index < 0 ? 0 : n.index;
        auto begin = range.first + ( n.index < 0 ? 0 : n.index );
        auto end   = n.index < 0 ? range.second : range.first + n.index + 1;

        for ( auto it = begin; it != end; it++ ) {

            tuples.push_back(mytuple());
            auto& last  = tuples.back();
            auto& skptr = *it;
            assignres(*skptr, env.get_inputsize());

            for (const string& par : n.parameters)
                if ( par != "show_default" ) {
                    printable& shower = *vdispatch[ par ];
                    last.add( unique_ptr<wrapper>(new double_wrapper( shower.print(*skptr))) );
                } else {
                    string idx  = utils::get_idx(it - begin + index, end - begin + index);
                    last.add( unique_ptr<wrapper>(new string_wrapper( idx + " : " + p.print(*skptr))) );
                }
        }

        // sort the tuples by calling stable_sort multiple times
        // complexity: O(N·log^2(N)) * #parameters
        size_t i = n.parameters.size()-1;
        while ( i-- > 0 )
            std::stable_sort(tuples.begin(), tuples.end(), [&i](const mytuple& t1, const mytuple& t2) {
                return t1.compare(t2, i);
            });

        if (n.lines_to_print >= 0) {
            int max = n.lines_to_print;
            for (auto it = tuples.begin(); it != tuples.end() && max-- > 0; it++)
                cout << it->tostring() << endl;
        } else {
            size_t max = -n.lines_to_print;
            size_t start = tuples.size() < max ? 0 : tuples.size() - max;
            for (auto it = tuples.begin() + start; it != tuples.end(); it++)
                cout << it->tostring() << endl;
        }

    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

void interpreter::visit(set_node& n) {
    sdispatch[ n.prop ]( n.value );
}

void interpreter::visit(ann_node& n) {
    std::size_t i = n.index < 0 ? 0 : n.index;
    skel_node& sk = *env.get(n.id, i);
    bool response = (*adispatch[ n.prop ])( sk, n.value );
    cout << "response: " << (response? "annotated!" : "not annotated!") << endl;
}

void interpreter::visit(rwr_node& n) {
    try {
        string id = n.id;


        for (const string& rule : n.parameters ) {
            node_set _set;
            rewriter _rewriter;
            auto range = env.range( n.id );
            auto begin = range.first + ( n.index < 0 ? 0 : n.index );
            auto end   = n.index < 0 ? range.second : range.first + n.index + 1;

            _set = ( rule == "allrules" ) ?
                _rewriter.apply_allrules( begin, end, rdispatch) :
                _rewriter.apply_rule( begin, end, *rdispatch[ rule ]);

            env.clear( id );
            for ( auto& p : _set )
                env.add( id, p.second );
        }

    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

void interpreter::visit(opt_node& n) {
    try {
        auto range = env.range( n.id );
        auto begin = range.first + ( n.index < 0 ? 0 : n.index );
        auto end   = n.index < 0 ? range.second : range.first + n.index + 1;

        for (const string& opt : n.parameters ) {
            if (opt == "normalform") {
                skel_node* newsk = normform( **begin );
                unranktorank2(*newsk, snc);
                env.clear( n.id );
                env.add( n.id, newsk );
            } else {
                optrule& optrule = *odispatch[ opt ];
                for (auto it = begin; it != end; it++) {
                    auto& skptr = *it;
                    assignres( *skptr, env.get_inputsize() );
                    optrule( *skptr );
                }
            }
        }
    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

void interpreter::visit(history_node& n) {
    if (n.id == "")
        phistory.print();
    else {
        history h(n.id, phistory);
        h.print(true);
    }
}

void interpreter::visit(import_node& n) {
    try {

        string path = utils::get_real_path(n.id);
        cout << "importing from: " << path << endl;

        cpp_parser cp(path);
        auto p = cp.parse();

        for (auto it =  p.first; it != p.second; it++) {

            cout << "importing " << it->name << endl;

            skel_node* sk;
            string name = it->name;
            string tin  = it->typein;
            string tout = it->typeout;

            if (it->wtype == wrapper_info::seq)
                sk = new seq_node(name, tin, tout, path);
            else if (it->wtype == wrapper_info::source)
                sk = new source_node(name,tout, path);
            else if (it->wtype == wrapper_info::drain)
                sk = new drain_node(name,tin, path);
            else
                cout << "error: no type recognized" << endl;

            assign_node a ( name, sk );
            visit(a);

        }
    } catch (std::logic_error) {
        cout << "impossible import code from " << n.id << endl;
    }
}

void interpreter::visit(gencode_node& n) {
    int i = 0;
    string code;
    string fname;
    printer print;
    ranker rank(env);
    unranker unrank(env);

    auto ptr = env.get(n.id, n.index);

    if (ptr != nullptr) {
        // unrank and generate code
        unrank(*ptr);
        code = ff(*ptr);

        // find name and store
        while ( utils::file_exists("ff"+to_string(++i)+".cpp") );
        fname = "ff" + to_string(i) + ".cpp";
        cout << "-- " << fname << endl;
        std::ofstream out(fname);
        out << code;

        // rerank
        rank(*ptr);
    }
}

void interpreter::visit(seq_node& n) {
    if (n.get(0) != nullptr)
        n.get(0)->accept(*this);
}

void interpreter::visit(source_node& n) {
}

void interpreter::visit(drain_node& n) {
}

void interpreter::visit(comp_node& n) {
    for (size_t i = 0;  i < n.size(); i++)
        n.get(i)->accept(*this);
}

void interpreter::visit(pipe_node& n) {
    for (size_t i = 0;  i < n.size(); i++)
        n.get(i)->accept(*this);
}

void interpreter::visit(farm_node& n) {
    n.get(0)->accept(*this);
}

void interpreter::visit(map_node& n) {
    n.get(0)->accept(*this);
}

void interpreter::visit(reduce_node& n) {
    n.get(0)->accept(*this);
}

void interpreter::visit(id_node& n) {
    auto ptr = env.get(n.id, n.index);                 // check if it exists
    if (ptr == nullptr) {
        success  = false;
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

history& interpreter::get_history() {
    return phistory;
}
