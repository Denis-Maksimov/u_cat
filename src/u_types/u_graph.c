#include <u_types/u_graph.h>



#define V_AT(graph,_i) ((u_graph_V*)u_vector_at(&(graph->V),_i)) 
#define E_AT(graph,_i) ((u_graph_E*)u_vector_at(&(graph->E),_i)) 

void u_graph_init(u_graph* graph)
{

    u_vector_init( &graph->E);
    u_vector_init( &graph->V);

}

void u_graph_deinit(u_graph* graph)
{
    u_vector_deinit( &graph->E,0);
    u_vector_deinit( &graph->V,0);
}






void u_graph_add_v(u_graph* graph, char* vertex_name)
{

    u_graph_V c={
        // .vertex_name=0,
        ._nodes=0
    };
    u_memcpy(c.vertex_name,vertex_name,u_strlen(vertex_name));
    // c.vertex_name
    u_vector_push_back(&graph->V, &c);

}


/**
 * @brief  Протягивает провод между двумя узлами
 * @note   
 * @param  wire_name: имя проводника
 * @param  a: имя узла А
 * @param  b: имя узла B
 * @retval None
 */
void u_graph_bind_vertex(u_graph* graph, char* wire_name, char* a, char* b, u32 Bec)
{
    size_t ia=0;
    u8 flag_a=0;
    u8 flag_b=0;
    size_t ib=0;
    
    //-- тупой поиск наших узлов FIXME
    for(size_t i=0; i < graph->V.n_elem; i++){
        
        if(!u_strncmp(a, V_AT(graph,i)->vertex_name, 25 )){
            ia=i;
            flag_a=1;
        }
        if(!u_strncmp(b, V_AT(graph,i)->vertex_name, 25 )){
            ib=i;
            flag_b=1;
        }
        if(flag_a & flag_b)break;

    }
    if(flag_b & flag_a){

        V_AT(graph,ia)->_nodes++; 
        V_AT(graph,ib)->_nodes++;
        u_graph_E wire={
            .cable_name={0},
            .V1=ia,
            .V2=ib,
            .dir=0,   //по умолчанию ненаправленный
            .weight=Bec
        };
        // wire.cable_name
        u_memcpy(wire.cable_name,wire_name,u_strlen(wire_name));
        
        u_vector_push_back(&graph->E,&wire);

    }else{
        // std::cout <<"Check names or codepage\n";
    }   
}

size_t u_graph_name_to_vertex(u_graph* graph, char* vert_name)
{

    size_t num_vertex=0;
    size_t limit=graph->V.n_elem;
        //-- тупой поиск узла FIXME
    for(size_t i=0; i<limit; i++){

        
        if(!u_strncmp(vert_name,V_AT(graph,i)->vertex_name,u_strlen(vert_name))){
            num_vertex=i;
            break;
        }        
        
        // if(i==limit-1) std::cout<<"error!!!!\n";
    }
    return num_vertex;
}

size_t u_graph_name_to_wire(u_graph* graph, char*  wire_name)
{

    size_t num_vertex=0;
    size_t limit=graph->E.n_elem;
        //-- тупой поиск узла FIXME
    for(size_t i=0; i<limit; i++){

        if(wire_name==E_AT(graph,i)->cable_name){
            num_vertex=i;
            break;
        }        
        
        // if(i==limit-1)std::cout<<"error!!!!\n";
    }
    return num_vertex;
}



void u_graph_get_neighbors(u_graph* graph, size_t ind, u_vector* indexes)
{

    for(size_t i=0; i<graph->E.n_elem; i++)
    {
        if(E_AT(graph,i)->V1==ind ){
            
            if(!u_vector_find(indexes, &E_AT(graph,i)->V2))
            {
                u_vector_push_back(indexes, &E_AT(graph,i)->V2);
                // return 1;
            }
        }
        if(E_AT(graph,i)->V2==ind ){
            if(!u_vector_find(indexes, &E_AT(graph,i)->V1))
            {
                u_vector_push_back(indexes, &E_AT(graph,i)->V1);
                // return 1;
            }
        }
    }

}

//id присоединёных ветвей к узлу
void u_graph_get_connected_wires(u_graph* graph, size_t ind, u_vector* indexes)
{
    // printf(V_AT(graph,ind)->vertex_name);
    for(size_t i=0; i < graph->E.n_elem; i++)
    {
        if( E_AT(graph,i)->V1 == ind || E_AT(graph,i)->V2 ==ind){
           if(!u_vector_find(indexes, &i))
           {
                u_vector_push_back(indexes, &i);
                // return 1;
            }
        }
    }

}

u32 u_graph_find_min(u_graph* graph)
{
    u32 rv=0-1;
    for (u32 i = 0; i < graph->V.n_elem; i++)
    {
        if(V_AT(graph,i)->weight < rv &&  !V_AT(graph,i)->ok){
            rv=i;
        }
    }
    return rv;
}

void u_graph_Dijkstra(u_graph* graph, char* node)
{
    size_t v = u_graph_name_to_vertex(graph,node);
    
    u_vector wires;
    u_vector_init(&wires);

    for (size_t i = 0; i < graph->V.n_elem; i++)
    {
       V_AT(graph,i)->weight=(0-1); //дохуахуа
       V_AT(graph,i)->ok=0;
    }
    V_AT(graph,v)->weight=0;
    
    
    size_t vert,v_2;
    for (size_t x = 0; x < graph->V.n_elem; x++)
    {
        u_vector_deinit(&wires,0);
        vert=u_graph_find_min(graph);
        V_AT(graph,vert)->ok=1;
        u_graph_get_connected_wires(graph, vert, &wires);
        //для каждой ветви
        for (size_t i = 0; i < wires.n_elem; i++)
        {
            (vert==E_AT(graph, ((size_t*)(wires.mem))[i])->V2)?
            (v_2=E_AT(graph, ((size_t*)(wires.mem))[i])->V1):
            (v_2=E_AT(graph, ((size_t*)(wires.mem))[i])->V2);

            if( V_AT(graph,v_2)->weight 
                    > 
                E_AT(graph, ((size_t*)(wires.mem))[i])->weight + V_AT(graph,vert)->weight
              )
            {
                V_AT(graph,v_2)->weight = E_AT(graph, ((size_t*)(wires.mem))[i])->weight+V_AT(graph,vert)->weight;
                V_AT(graph,v_2)->wire= ((size_t*)(wires.mem))[i];
                
            }
            
        }


    }
    
}




void u_graph_BFS_by_name(u_graph* graph, char* vert_name, u_vector* neighbors)
{

    size_t num_vertex=u_graph_name_to_vertex(graph,vert_name);

    u_graph_get_neighbors(graph,num_vertex, neighbors);

    for (size_t i = 0; i < neighbors->n_elem; i++)
    {
        num_vertex=((size_t*)neighbors->mem)[i];
        u_graph_get_neighbors(graph,num_vertex, neighbors);
    }
            
}

void u_graph_BFS_by_num(u_graph* graph, size_t num_vertex, u_vector* neighbors)
{

    u_graph_get_neighbors(graph,num_vertex, neighbors);

    for (size_t i = 0; i < neighbors->n_elem; i++)
    {
        num_vertex=((size_t*)neighbors->mem)[i];
        u_graph_get_neighbors(graph,num_vertex, neighbors);
    }
            
}

//поиск ветвей в ширину
void u_graph_bfs_wires(u_graph* graph, size_t num_wire,  u_vector* wires)
{

    // u_vector_deinit(wires);
    // u_vector_init(wires,sizeof(size_t));

    u_graph_get_connected_wires(graph,E_AT(graph,num_wire)->V1, wires);
    u_graph_get_connected_wires(graph,E_AT(graph,num_wire)->V2, wires);
    for (size_t i = 0; i < wires->n_elem; i++)
    {
        num_wire=((size_t*)wires->mem)[i];
        u_graph_get_connected_wires(graph,E_AT(graph,num_wire)->V1, wires);
        u_graph_get_connected_wires(graph,E_AT(graph,num_wire)->V2, wires);
    }
            
}

//======== Зависит от printf, puts ====================
#ifdef _STDIO_H


void u_graph_print_short_way(u_graph* graph, char* A, char* B)
{
    u_graph_Dijkstra(graph, B);
    size_t dest=u_graph_name_to_vertex(graph,B);
    size_t vert=u_graph_name_to_vertex(graph,A);
    size_t tmp=0;
    while(vert!=dest)
    {
        tmp=V_AT(graph,vert)->wire;
        puts(E_AT(graph, tmp)->cable_name);
        // std::cout<<E[V[vert].wire].cable_name<<std::endl;
        tmp=V_AT(graph,vert)-> wire;
        
        (E_AT(graph, tmp)->V1==vert)?
        (vert=E_AT(graph, tmp)->V2):
        (vert=E_AT(graph, tmp)->V1);
    }
}

void u_graph_print_wires(u_graph* graph)
{
    for(size_t i=0; i < graph->E.n_elem; i++)
    {
        printf(
            "%s: %s + %s = %u\n",
            ((u_graph_E*)graph->E.mem)[i].cable_name,
            ((u_graph_V*)graph->V.mem)[((u_graph_E*)graph->E.mem)[i].V1].vertex_name,
            ((u_graph_V*)graph->V.mem)[((u_graph_E*)graph->E.mem)[i].V2].vertex_name,
            ((u_graph_E*)graph->E.mem)[i].weight
        );
    }
}

#endif // _STDIO_H
