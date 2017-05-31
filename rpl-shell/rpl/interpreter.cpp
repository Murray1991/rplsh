#include "interpreter.hpp"
#include "rewriting/rewriter.hpp"
#include "utils/mytuple.hpp"
#include "utils/rank.hpp"
#include "utils/utils.hpp"
#include <memory>
#include <iostream>

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
    success(true)
{}

void interpreter::visit(assign_node& n) {
    n.rvalue->accept(*this);                // recurse for semantic check in skel tree
    if ( success ) {
        unranktorank2(*n.rvalue, snc);
        env.put(n.id, n.rvalue);
    }
    else
        delete n.rvalue;
}

/* dirty implementation */
void interpreter::visit(show_node& n) {
    try {

        if ( n.id == "" && n.parameters.size() == 0) {
            cout << utils::to_string( gdispatch[n.prop]() ) << endl;
            return;
        }

        printer p;
        auto range = env.range( n.id );
        vector<mytuple> tuples;
        for ( auto it = range.first; it != range.second; it++ )
        {
            tuples.push_back(mytuple());
            auto& last  = tuples.back();
            auto& skptr = *it;
            for (const string& par : n.parameters)
                if ( par != "show_default" ) {
                    printable& shower = *vdispatch[ par ];
                    last.add( unique_ptr<wrapper>(new double_wrapper( shower.print(*skptr))) );
                } else
                    last.add( unique_ptr<wrapper>(new string_wrapper( p.print(*skptr))) );
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
    skel_node& sk = *env.get(n.id);
    bool response = (*adispatch[ n.prop ])( sk, n.value );
    cout << "response: " << (response? "annotated!" : "not annotated!") << endl;
}

void interpreter::visit(rwr_node& n) {
    try {

        string id = n.id;
        string rule = n.prop;
        auto p = env.range(id);
        node_set _set;
        rewriter _rewriter;
        _set = ( rule == "allrules" ) ?
            _rewriter.apply_allrules( p.first, p.second, rdispatch) :
            _rewriter.apply_rule( p.first, p.second, *rdispatch[ rule ]);

        env.clear( id );
        for ( auto& p : _set )
            env.add( id, p.second );

    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

void interpreter::visit(opt_node& n) {
    try {

        auto p = env.range( n.id );
        optrule& optrule = *odispatch[ n.prop ];
        for (auto it = p.first; it != p.second; it++) {
            auto& skptr = *it;
            optrule( *skptr );
        }

    } catch (out_of_range& e) {
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}

void interpreter::visit(seq_node& n) {
    if (n.get(0) != nullptr)
        n.get(0)->accept(*this);
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
    auto ptr = env.get(n.id);                 // check if it exists
    if (ptr == nullptr) {
        success  = false;
        err_repo.add( make_shared<error_not_exist>(n.id) );
    }
}
