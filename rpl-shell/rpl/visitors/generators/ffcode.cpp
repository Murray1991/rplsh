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

// header includes and namespaces
string includes() {
    stringstream ss;
    ss << "#include <iostream>\n";
    ss << "#include <vector>\n\n";
    ss << "// specify include directory for fastflow\n";
    ss << "#include <ff/farm.hpp>\n";
    ss << "#include <ff/pipeline.hpp>\n\n";
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
    gsw(env)
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
}

void ffcode::visit( reduce_node& n ) {
}

void ffcode::visit( id_node& n ) {
    auto ptr = env.get(n.id);
    ptr->accept(*this);
}

string ffcode::operator()(skel_node& n) {

    // clear names and code_lines global environments
    names.clear();
    business_headers.clear();
    queue<pair<string,string>> empty;
    swap( code_lines, empty );

    gsw(n); // start visit for getting wrappers

    auto src_nodes = gsw.get_source_nodes();
    auto drn_nodes = gsw.get_drain_nodes();
    auto seq_nodes = gsw.get_seq_nodes();

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
    // and pick from code_line
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
