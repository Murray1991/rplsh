#include "ffcode.hpp"
#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <cassert>

using namespace std;

map<string, int> names;
map<string, bool> business_headers;

string new_name(const string& name) {
    int num = 0;
    string apnd;

    try {
        num = ++names.at(name);
    } catch (out_of_range& e) {
        names[name] = 0;
    }

    apnd = !num ? "" : to_string(num);
    return name + apnd;
}

// return the ff_node calling the function
// typein and typeout MUST implement copy
// assignment operator and default constructor
string stage_declaration( const seq_node& n ) {
    stringstream ss;
    ss << "class " << n.name << "_stage : public ff_node {\n";
    ss << "protected:\n\t" << n.name << " wrapper; \n";
    ss << "public:\n";
    ss << "\tvoid * svc(void *t) {\n";
    ss << "\t\t" << n.typein << " _in  = *((" << n.typein << "*) t);\n";
    ss << "\t\t" << n.typeout << "* _out  = new " << n.typeout << "();\n";
    ss << "\t\t" << "*_out = wrapper.compute(_in);\n";
    //TODO memory leak of previous node
    ss << "\t\t" << "return (void*) _out;\n";
    ss << "\t}\n";
    ss << "};\n\n";
    return ss.str();
}

string source_declaration( const source_node& n ) {
    stringstream ss;
    ss << "class " << n.name << "_stage : public ff_node {\n";
    ss << "protected:\n\tstd::unique_ptr<" << n.name << "> src; \n\n";
    ss << "public:\n";
    ss << "\t" << n.name << "_stage() : src(new " << n.name << "()) {}\n";
    ss << "\tvoid * svc(void *t) {\n";
    ss << "\t\twhile( src->has_next() )\n";
    ss << "\t\t\tff_send_out((void*) src->next() );\n";
    ss << "\t\treturn (NULL);\n";
    ss << "\t}\n";
    ss << "};\n\n";
    return ss.str();
}

string drain_declaration( const drain_node& n ) {
    stringstream ss;
    ss << "class " << n.name << "_stage : public ff_node {\n";
    ss << "protected:\n\tstd::unique_ptr<" << n.name << "> drn; \n\n";
    ss << "public:\n";
    ss << "\t" << n.name << "_stage() : drn(new " << n.name << "()) {}\n";
    ss << "\tvoid * svc(void *t) {\n";
    ss << "\t\tdrn->process((" << n.typein << "*) t);\n";
    ss << "\t\treturn (GO_ON);\n";
    ss << "\t}\n";
    ss << "};\n\n";
    return ss.str();
}

string map_declaration( map_node& n, rpl_environment& env ) {
    // two-tier model: inside map nodes only seq or compseq are allowed:
    // if stream/datap inside, ignore it when compile and show a warning

    pardegree nw(env);         // setup the pardegree visitor
    get_seq_wrappers gsw(env); // setup the get_seq_wrappers visitor

    gsw(n); // find the sequential wrappers
    auto src_nodes = gsw.get_source_nodes();
    auto drn_nodes = gsw.get_drain_nodes();
    auto seq_nodes = gsw.get_seq_nodes();
    if (!src_nodes.empty() || !drn_nodes.empty() || ((int) nw(n)) != n.pardegree)
        cout << "warning: two tier modeling application for generation of code" << endl;

    stringstream ss;
    string typein  = seq_nodes.front()->typein;
    string typeout = seq_nodes.back()->typeout;
    string ffMap   = "ff_Map<"+typein+","+typeout+">";
    ss << "class map" << n.getid() << "_stage : public " << ffMap << " {\n";
    ss << "protected:\n";
    for (size_t i = 0; i < seq_nodes.size(); i++)
        ss << "\t" << seq_nodes[i]->name << " wrapper" << i << ";\n";

    ss << "public:\n";
    ss << "\t" << typeout << "* svc("<< typein << " *t) {\n";
    ss << "\t\t" << typein << "& _task = *t;\n";
    ss << "\t\t" << typeout << "* out = new " << typeout << "();\n";
    ss << "\t\tout->resize(_task.size());\n"; //TODO really necessary?

    // start parallel for
    size_t i;
    string par;
    ss << "\t\t" << ffMap << "::parallel_for(0, _task.size(),";
    // begin lambda
    ss << "[this, &_task, &out](const long i) {\n";
    for (i = 0; i < seq_nodes.size()-1; i++) {
        par = !i ? "_task[i]" : ("res" + to_string(i-1));
        ss << "\t\t\tauto res" << i << " = wrapper" << i << ".op(" << par << ");\n";
    }
    par = !i ? "_task[i]" : ("res" + to_string(i-1));
    ss << "\t\t\t(*out)[i] = wrapper" << i << ".op(" << par << ");\n";
    ss << "\t\t}";
    // end lambda
    ss << "," << to_string(n.pardegree) << ");\n";
    // end parallel for

    //TODO memory leak of previous node
    ss << "\t\t" << "return out;\n";
    ss << "\t}\n";
    ss << "};\n\n";

    return ss.str();
}

string red_declaration( reduce_node& n, rpl_environment& env ) {
    // two-tier model: inside reduce nodes only seq or compseq are allowed:
    // if stream/datap inside, ignore it when compile and show a warning
    pardegree nw(env);         // setup the pardegree visitor
    get_seq_wrappers gsw(env); // setup the get_seq_wrappers visitor

    gsw(n); // find the sequential wrappers
    auto src_nodes = gsw.get_source_nodes();
    auto drn_nodes = gsw.get_drain_nodes();
    auto seq_nodes = gsw.get_seq_nodes();
    if (((int) nw(n)) != n.pardegree)
        cout << "warning: two tier modeling will be applied for generation of code" << endl;
    if (!src_nodes.empty() || !drn_nodes.empty())
        cout << "warning: source/drain nodes inside reduce" << endl;

    // in case of multiple sequential nodes:
    // reduce(comp(a,b,c)) == pipe(map(comp(a,b)),c)
    // where c is the only node implementing a binary operation f: v1 x v2 -> res
    stringstream ss;
    string typein  = seq_nodes.front()->typein;
    string typeout = seq_nodes.back()->typeout;
    string ffMap   = "ff_Map<"+typein+","+typeout+","+typeout+">";

    ss << "class reduce" << n.getid() << "_stage : public " << ffMap << " {\n";
    ss << "protected:\n";
    for (size_t i = 0; i < seq_nodes.size(); i++)
        ss << "\t" << seq_nodes[i]->name << " wrapper" << i << ";\n";
    ss << "public:\n";
    ss << "\t" << typeout << "* svc("<< typein <<"* t) {\n";
    string task = "_task";
    ss << "\t\t" << typein << "& _task = *t;\n";

    if ( seq_nodes.size() > 1) {
        cout << "warning: reduce(comp(s1, s2, ..., sk, sn)) -> comp(map(s1,...,sk), reduce(sn))" << endl;
        string mapout  = seq_nodes[seq_nodes.size()-2]->typeout;
        ss << "\t\t" << mapout << "* mapout = new " << mapout << "();\n";
        ss << "\t\tmapout->resize(_task.size());\n"; //TODO really necessary?
        task = "mapout";

        size_t i;
        string par;
        // start parallel for
        ss << "\t\t"<<ffMap<<"::parallel_for(0, _task.size(),";
        // begin lambda
        ss << "[this, &_task, &mapout](const long i) {\n";
        for (i = 0; i < seq_nodes.size()-2; i++) {
            par = !i ? "_task[i]" : ("res" + to_string(i-1));
            ss << "\t\t\tauto res" << i << " = wrapper" << i << ".op(" << par << ");\n";
        }
        par = !i ? "_task[i]" : ("res" + to_string(i-1));
        ss << "\t\t\t(*mapout)[i] = wrapper" << i << ".op(" << par << ");\n";
        ss << "\t\t}\n";
        // end lambda
        ss << "," << to_string(n.pardegree) << ");\n";
        // end parallel for

    }

    size_t idx = seq_nodes.size()-1;
    ss << "\t\t" << typeout << "* out  = new " << typeout << "();\n";
    ss << "\t\tauto reduceF = [this]("<<typeout<<"& sum, "<<typeout<<" elem) {sum = wrapper"<<idx<<".op(sum, elem);};\n";
    ss << "\t\tauto bodyF = [this,&_task](const long i, "<<typeout<<"& sum) {sum = wrapper"<<idx<<".op(sum, _task[i]);};\n";
    ss << "\t\t" << ffMap <<"::parallel_reduce(*out, wrapper"<<idx<<".identity,0,"<<task<<".size(),bodyF,reduceF,"<<to_string(n.pardegree)<<");\n";

    //TODO memory leak of previous node
    ss << "\t\t" << "return out;\n";
    ss << "\t}\n";
    ss << "};\n\n";

    return ss.str();
}

// header includes and namespaces
string includes() {
    stringstream ss;
    ss << "#include <iostream>\n";
    ss << "#include <vector>\n\n";
    ss << "// specify include directory for fastflow\n";
    ss << "#include <ff/farm.hpp>\n";
    ss << "#include <ff/map.hpp>\n";
    ss << "#include <ff/pipeline.hpp>\n";
    ss << "#include <ff/parallel_for.hpp>\n\n";
    ss << "// specify include directory for RPL-Shell\n";
    ss << "#include <aux/types.hpp>\n";
    ss << "#include <aux/wrappers.hpp>\n";
    ss << "#include <aux/ff_comp.hpp>\n\n";
    ss << "// business code headers\n";
    for (auto& kv : business_headers) {
        ss << "#include <" << kv.first << ">\n";
    }
    ss << "\n\n";
    return ss.str();
}

string main_wrapper( const string& code ) {
    regex nline("\n");
    stringstream ss;
    ss << "int main( int argc, char* argv[] ) {\n";
    ss << "\t" << regex_replace(code, nline, "\n\t" );
    ss << "\n}\n";
    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////

ffcode::ffcode( rpl_environment& env ) :
    env(env),
    gsw(env),
    tds(env)
{}

void ffcode::visit( seq_node& n ) {
    seq_wraps(n.name);
}

void ffcode::visit( source_node& n ) {
    seq_wraps(n.name);
}

void ffcode::visit( drain_node& n ) {
    seq_wraps(n.name);
}

void ffcode::visit( comp_node& n ) {
    comp_pipe("ff_comp", "comp", n);
}

void ffcode::visit( pipe_node& n ) {
    comp_pipe("ff_pipeline", "pipe", n);
}

void ffcode::visit( farm_node& n ) {

    stringstream ss;
    string wvar = new_name("workers");
    string fvar = new_name("farm");

    ss << "\n// vector of workers of " << fvar << "\n";
    ss << "std::vector<ff_node*> " << wvar << ";\n";

    for (int i = 0; i < n.pardegree; i++) {
        ss << "\n// " << fvar << "'s worker " << (i+1) << "\n";

        n.get(0)->accept(*this);
        auto& p = code_lines.front();
        ss << p.second;
        ss << wvar << ".push_back(&" << p.first << ");\n";
        code_lines.pop();
    }

    ss << "\n// add " << wvar << " to " << fvar << "\n";
    ss << "ff_farm<> " << fvar << ";\n";
    ss << fvar << ".add_workers(" << wvar << ");\n";
    ss << fvar << ".add_collector(NULL);\n\n";

    assert(code_lines.empty());
    code_lines.push({fvar, ss.str()});

}

void ffcode::visit( map_node& n ) {
    stringstream ss;
    string var = new_name("_map"+to_string(n.getid())+"_");
    ss << "map" << n.getid() << "_stage " << var << ";\n";
    code_lines.push({var, ss.str()});
}

void ffcode::visit( reduce_node& n ) {
    stringstream ss;
    string var = new_name("_red"+to_string(n.getid())+"_");
    ss << "reduce" << n.getid() << "_stage " << var << ";\n";
    code_lines.push({var, ss.str()});
}

void ffcode::visit( id_node& n ) {
    auto ptr = env.get(n.id, n.index);
    if (ptr != nullptr)
        ptr->accept(*this);
    else
        cout << n.id << " whaaaat?" << endl;
}

string ffcode::operator()(skel_node& n) {

    // clear global environment:
    // names, business_headers, queue
    names.clear();
    business_headers.clear();
    queue<pair<string,string>> empty;
    swap( code_lines, empty );

    gsw(n);     // start visit for getting seq wrappers
    auto src_nodes = gsw.get_source_nodes();
    auto drn_nodes = gsw.get_drain_nodes();
    auto seq_nodes = gsw.get_seq_nodes();

    tds(n);     // start visit for getting datap wrappers
    auto map_nodes = tds.get_map_nodes();
    auto red_nodes = tds.get_reduce_nodes();

    size_t idx;
    string code  = "";
    string decls = "";

    for (auto src : src_nodes) {
        business_headers[src->file] = true;
        decls += source_declaration(*src);
    }

    for (auto drn : drn_nodes) {
        business_headers[drn->file] = true;
        decls += drain_declaration(*drn);
    }

    for (auto seq : seq_nodes) {
        business_headers[seq->file] = true;
        decls += stage_declaration(*seq);
    }

    idx = 0;
    for (auto mapn : map_nodes) {
        mapn->setid(idx++);
        decls += map_declaration(*mapn, env);
    }

    idx = 0;
    for (auto redn : red_nodes) {
        redn->setid(idx++);
        decls += red_declaration(*redn, env);
    }

    n.accept(*this);
    stringstream ss;
    auto p = code_lines.front();
    ss << p.second << "\n";
    ss << p.first << ".run_and_wait_end();\n";
    ss << "std::cout << \"Spent:\" << ";
    ss <<  p.first << ".ffTime() << \"msecs\" << std::endl;\n\n";
    ss << "return 0;\n";

    code_lines.pop();
    assert(code_lines.empty());

    // main code
    code = includes() + decls + main_wrapper( ss.str() );

    return code;
}

void ffcode::seq_wraps(const string& name) {
    stringstream ss;
    string var = new_name("_"+name);
    ss << name << "_stage " << var << ";\n";
    code_lines.push({var, ss.str()});
}

void ffcode::comp_pipe(const string& type, const string& name, skel_node& n) {
    stringstream ss;
    string var  = new_name(name);

    // recursion over the children
    // and pick from code_lines
    std::vector<pair<string,string>> vec;
    for (size_t i = 0; i < n.size(); i++) {
        n.get(i)->accept(*this);
        vec.push_back(code_lines.front());
        ss << vec.back().second;
        code_lines.pop();
    }

    ss << type << " " << var << ";\n";
    for (auto p : vec)
        ss << var << ".add_stage(&" << p.first << ");\n";
    ss << "\n";

    assert(code_lines.empty());
    code_lines.push({var, ss.str()});
}
