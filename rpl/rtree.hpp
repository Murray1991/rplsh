#ifndef rpl_rtree_hpp
#define rpl_rtree_hpp

#include <iterator>
#include <string>
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
//
//  Forward declarations of the visitors
//
///////////////////////////////////////////////////////////////////////////////
struct skel_visitor;
struct visitor;

///////////////////////////////////////////////////////////////////////////////
//
//  The base node interface of the ast
//
///////////////////////////////////////////////////////////////////////////////
struct rpl_node
{
    virtual void accept(visitor& v) = 0;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The assignable node interface
//
///////////////////////////////////////////////////////////////////////////////
struct rvalue_node : public rpl_node {
};

///////////////////////////////////////////////////////////////////////////////
//
//  The verb_node node interface
//
///////////////////////////////////////////////////////////////////////////////
struct verb_node : public rpl_node {
    verb_node(const std::string& id, const std::string& prop);
    std::string id;
    std::string prop;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The skeleton node interface
//
///////////////////////////////////////////////////////////////////////////////
struct skel_node : public rvalue_node {
    virtual void accept(skel_visitor& v) = 0;
};

///////////////////////////////////////////////////////////////////////////////
//
//  The assign node class
//
///////////////////////////////////////////////////////////////////////////////
struct assign_node : public rpl_node
{
    assign_node(const std::string& id, const std::shared_ptr<skel_node>& rvalue);
    virtual void accept(visitor & v);

    std::string id;
    std::shared_ptr<skel_node> rvalue;
};

///////////////////////////////////////////////////////////////////////////////
//
//                          Shell verb nodes
//
///////////////////////////////////////////////////////////////////////////////
struct show_node : public verb_node
{
    show_node(const std::string& id, const std::string& prop);
    virtual void accept(visitor & v);
};

struct set_node : public verb_node
{
    set_node(const std::string& id, const std::string& prop, int value);
    virtual void accept(visitor & v);
    int value;
};

struct ann_node : public verb_node
{
    ann_node(const std::string& id, const std::string& prop, double value);
    virtual void accept(visitor & v);
    double value;
};

struct rwr_node : public verb_node
{
    rwr_node(const std::string& id, const std::string& rule);
    virtual void accept(visitor & v);
};

struct opt_node : public verb_node
{
    opt_node(const std::string& id, const std::string& opt);
    virtual void accept(visitor & v);
};

///////////////////////////////////////////////////////////////////////////////
//
//                          Skeleton nodes
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//  STL-like interface common to all the subclasses of a skeleton node
//  with the exception of the id_node, in order to give access to the
//  child nodes
//
///////////////////////////////////////////////////////////////////////////////
typedef std::shared_ptr<skel_node> SkelUptr;
typedef std::vector<SkelUptr> vSkelUptr;

struct access_node : public skel_node {

    access_node();
    access_node(std::shared_ptr<skel_node> sk);
    void push(std::shared_ptr<skel_node> sk);
    std::shared_ptr<skel_node> get(int idx) const;
    int size() const;

private:
    vSkelUptr children;
};

struct seq_node : public access_node
{
    seq_node(std::shared_ptr<skel_node> pattexp, double servicetime = 1.0);
    virtual void accept(visitor& v);
    virtual void accept(skel_visitor& v);
    double servicetime;
};

struct comp_node : public access_node
{
    virtual void accept(visitor& v);
    virtual void accept(skel_visitor& v);
};

struct pipe_node : public access_node
{
    virtual void accept(visitor& v);
    virtual void accept(skel_visitor& v);
};

struct farm_node : public access_node
{
    farm_node(std::shared_ptr<skel_node> pattexp, int pardegree = 1);
    virtual void accept(visitor& v);
    virtual void accept(skel_visitor& v);
    int pardegree;
};

struct map_node : public access_node
{
    map_node(std::shared_ptr<skel_node> pattexp, int pardegree = 1);
    virtual void accept(visitor& v);
    virtual void accept(skel_visitor& v);
    int pardegree;
};

struct reduce_node : public access_node
{
    reduce_node(std::shared_ptr<skel_node> pattexp);
    virtual void accept(visitor& v);
    virtual void accept(skel_visitor& v);
};

struct id_node : public skel_node
{
    id_node(std::string& identifier);
    virtual void accept(visitor& v);
    virtual void accept(skel_visitor& v);
    std::string identifier;
};

#endif
