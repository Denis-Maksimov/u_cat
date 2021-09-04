#ifndef U_GRAPH
#define U_GRAPH

#include <u_types/u_vector.h>
#include <u_types/u_matrix.h>

C_START
/**********************************
 * Стрктура данных "Граф"
 * хранение структуры реализовано в виде двух списков: вершин и рёбер
 * предусмотрена репрезентация структуры в виде: 
 * 
 * 1) Списка рёбер 
 * 2) Матрицы смежности
 * 3) Списка смежности
 * 
 * -------------------------------
 *  рассмотрим граф:
 * 
 *  (1)---(2)---(3)
 *   |           |
 *  (4)---(5)---(6)
 * 
 * =============[1]================
 * 
 * {
 *  [1,2],[2,3],[3,6],
 *  [6,5],[5,4],[4,1],
 * }
 * 
 * =============[2]================
 *   
 *   _1_2_3_4_5_6_
 * 1| 0 1 0 1 0 0 |
 * 2| 1 0 1 0 0 0 |
 * 3| 0 1 0 0 0 1 |
 * 4| 1 0 0 0 1 0 |
 * 5| 0 0 0 1 0 1 |
 * 6| 0 0 1 0 1 0 |
 *   -------------
 *      
 * =============[3]================
 * 
 * 1| [2,4]->
 * 2| [1,3]->
 * 3| [2,6]->
 * 4| [1,5]->
 * 5| [4,6]->
 * 6| [3,5]->
 *  V
**********************************/


typedef struct 
{
    size_t id;
    void* data; //данные в вершине
    void (*free_func)(void *);

}vertex_t;


typedef struct 
{
    vertex_t* v1;
    vertex_t* v2;
    double weight;
}edge_t;



typedef 
enum 
{
    GRAPH_NOP=0,
    GRAPH_EXEC_DESTRUCTOR,

}graph_free_old_flag;


typedef u_vector list_edges;
typedef u_matrix matrix_indentifity;


/**************************************
* вектор векторов i-й вектор содержит 
* вектор граней присоединённых к вершине
***************************************/
typedef u_vector list_indentifity; 






typedef struct u_graph_v2_t
{
    size_t* n_vertex;
    size_t* n_edges;
    
    u_vector* V;
    union{
      u_vector* E;
      list_edges* _LE;  ///список граней
    };
    matrix_indentifity* _MI; //в узлах матрицы указатель на грань
    list_indentifity* _LI;
    size_t state;
    
    // void(*add_edge)(struct u_graph_v2_t* self, vertex_t* v1, vertex_t* v2);



}u_graph_v2;




u_graph_v2* new_graph();
void graph_free(u_graph_v2* Graph);
matrix_indentifity* graph_get_matrix_indetify(u_graph_v2* Graph);
list_indentifity* graph_get_list_indetify(u_graph_v2* Graph);

size_t graph_add_vertex(u_graph_v2* Graph,void* data,void (*free_func)(void *));
void graph_add_edge(u_graph_v2* Graph,size_t v1, size_t v2,double weight);

void print_graph(u_graph_v2* Graph);

void graph_vertex_set_data(u_graph_v2* Graph,size_t vertex_id,void* data,void (*free_func)(void *), graph_free_old_flag flag);
void* graph_vertex_get_data(u_graph_v2* Graph,size_t vertex_id);

u_graph_v2* new_graph_from_graph_matrix(matrix_indentifity* mat);
u_graph_v2* new_graph_from_pdouble_matrix(u_matrix* mat);

void save_graph_struct_to_csv(u_graph_v2* Graph, char* filename);
u_graph_v2* load_graph_struct_from_csv(char* filename);

void graph_test();




C_END


#endif