#ifndef rpl_visitor_hpp
#define rpl_visitor_hpp

#include "rtree.hpp"

///////////////////////////////////////////////////////////////////////////////
//
//  The skeleton visitor abstract class
//
///////////////////////////////////////////////////////////////////////////////
struct skel_visitor
{
    virtual void visit(seq_node& n)     = 0;
    virtual void visit(comp_node& n)    = 0;
    virtual void visit(pipe_node& n)    = 0;
    virtual void visit(farm_node& n)    = 0;
    virtual void visit(map_node& n)     = 0;
    virtual void visit(reduce_node& n)  = 0;
    virtual void visit(id_node& n)      = 0;

    virtual void operator()(skel_node& sk){}
};

///////////////////////////////////////////////////////////////////////////////
//
//  The visitor abstract class
//
///////////////////////////////////////////////////////////////////////////////
struct visitor : public virtual skel_visitor
{
    using skel_visitor::visit;
    virtual void visit(assign_node& n) = 0;
    virtual void visit(show_node& n)   = 0;
    virtual void visit(set_node& n)    = 0;
    virtual void visit(ann_node& n)    = 0;
    virtual void visit(rwr_node& n)    = 0;
    virtual void visit(opt_node& n)    = 0;
};

struct servicetime: public skel_visitor
{
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);

    virtual void operator()(skel_node& sk){
            res = 0;
            sk.accept(*this);
    }

    double get() const {
        return res;
    }

protected:
    double res;
};

struct printer : public skel_visitor
{
    virtual void visit(seq_node& n);
    virtual void visit(comp_node& n);
    virtual void visit(pipe_node& n);
    virtual void visit(farm_node& n);
    virtual void visit(map_node& n);
    virtual void visit(reduce_node& n);
    virtual void visit(id_node& n);

    virtual void operator()(skel_node& sk){
            sk.accept(*this);
    }
private:
    template<typename Iterator>
    void print(const std::string& name, Iterator begin, Iterator end);
};

#endif
