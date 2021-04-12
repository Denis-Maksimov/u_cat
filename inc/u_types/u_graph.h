#ifndef U_GRAPH_H
#define U_GRAPH_H

#include "u_vector.h"


/** узел графа */
typedef struct 
{
    
    void* data;                //указатель на данные
    //------------
    //Для Дейкстры вес и ветвь
    
    size_t wire;
    u32 weight;
    
    //------------
    
    int _nodes;                //число связей
    char vertex_name[8];
    u8 ok;
}u_graph_V;


/** дуга графа */
typedef struct
{
    char cable_name[25];       // имя
    size_t V1;    // узел1 (источник)
    size_t V2;    // узел2 (получатель)
    u8 dir;                // направленность
    //------------
    //Для Дейкстры вес
    u32 weight;
    //------------

}u_graph_E;

typedef struct
{
    u_vector V;
    u_vector E;
}u_graph;


void u_graph_init(u_graph* graph);
void u_graph_deinit(u_graph* graph);
void u_graph_add_v(u_graph* graph, char* vertex_name);
void u_graph_bind_vertex(u_graph* graph, char* wire_name, char* a, char* b, u32 Bec);
size_t u_graph_name_to_vertex(u_graph* graph, char* vert_name);
size_t u_graph_name_to_wire(u_graph* graph, char*  wire_name);
void u_graph_get_neighbors(u_graph* graph, size_t ind, u_vector* indexes);
void u_graph_get_connected_wires(u_graph* graph, size_t ind, u_vector* indexes);
u32 u_graph_find_min(u_graph* graph);
void u_graph_Dijkstra(u_graph* graph, char* node);
void u_graph_BFS_by_name(u_graph* graph, char* vert_name, u_vector* neighbors);
void u_graph_BFS_by_num(u_graph* graph, size_t num_vertex, u_vector* neighbors);
void u_graph_bfs_wires(u_graph* graph, size_t num_wire,  u_vector* wires);


//======== Зависит от printf, puts ====================
// #include <stdio.h>
#ifdef _STDIO_H
void u_graph_print_wires(u_graph* graph);
void u_graph_print_short_way(u_graph* graph, char* A, char* B);
#endif // _STDIO_H

#endif //!U_GRAPH_H
