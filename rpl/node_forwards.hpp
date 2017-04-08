///////////////////////////////////////////////////////////////////////////////
//
//  This file contains the forward declarations of the rpl nodes (definited elsewhere)
//  This is useful for declare/define classes that "encapsulate" these declarations
//  but that are not effectively used/known (because maybe passed by reference)
//
//  This header is needed in the file containing the definitions of visitors and
//  annotations
//
///////////////////////////////////////////////////////////////////////////////
#ifndef rpl_node_forwards_hpp
#define rpl_node_forwards_hpp

struct skel_node;
struct assign_node;
struct show_node;
struct set_node;
struct ann_node;
struct rwr_node;
struct opt_node;
struct seq_node;
struct comp_node;
struct pipe_node;
struct farm_node;
struct map_node;
struct reduce_node;
struct id_node;

#endif
