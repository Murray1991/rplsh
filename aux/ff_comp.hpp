#ifndef ff_comp_hpp
#define ff_comp_hpp

#include <ff/node.hpp>
#include <vector>
#include <functional>
#include <initializer_list>

using namespace ff;


class ff_comp : public ff_node {
protected:
  std::vector<ff_node *>stages;
  int stageno;
public:

  ff_comp(ff_node *s1, ff_node *s2) {
    stages.push_back(s1);
    stages.push_back(s2);
    stageno = 2;
  }

  ff_comp(std::initializer_list<ff_node*> pats) {
    stageno = 0;
    for(auto i = pats.begin(); i != pats.end(); i++) {
      stages.push_back(*i);
      stageno++;
    }
  }


  // for cloning

  ff_comp() {
    stageno=0;
    return;
  }

  void add_stage(ff_node * stage) {
    stages.push_back(stage);
    stageno++;
    return;
  }

  ff_comp * clone() {
    ff_comp * p = new ff_comp();
    for(int i=0; i<stageno; i++)
      p->add_stage(stages[i]);
    return(p);
  }
  // end of bad idea to clone

  void * svc (void * t) {
    auto x = stages[0]->svc(t);
    for(int i=1; i<stageno; i++)
      x = stages[i]->svc(x);
    return(x);
  }
};

#endif
