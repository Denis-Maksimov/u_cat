#include <u_types/u_graph_v2.h>


u_graph_v2* 
new_graph()
{
    u_graph_v2* Graph;
    Graph=(u_graph_v2*)malloc(sizeof(u_graph_v2));
    Graph->V=u_vector_new();
    Graph->n_vertex=(&Graph->V->n_elem);

    Graph->E=u_vector_new();
    Graph->n_edges=(&Graph->E->n_elem);

    Graph->state=0;
    Graph->_MI=NULL;
    Graph->_LI=u_vector_new();

   return Graph;
} 

matrix_indentifity* 
graph_get_matrix_indetify(u_graph_v2* Graph)
{
    if(Graph->state&1) {return Graph->_MI;}
    Graph->state = 1;
    if(Graph->_MI)
    {
        u_matrix_free(Graph->_MI);
    }
    Graph->_MI=u_matrix_new(*Graph->n_vertex,*Graph->n_vertex);
    
    for (size_t i = 0; i < *Graph->n_edges; i++)
    {
        
        u_matrix_insert(Graph->_MI,
            ((edge_t*)(u_vector_at(Graph->_LE,i)))->v1->id,
            ((edge_t*)(u_vector_at(Graph->_LE,i)))->v2->id,
            u_vector_at(Graph->_LE,i)
        );
        u_matrix_insert(Graph->_MI,
            ((edge_t*)(u_vector_at(Graph->_LE,i)))->v2->id,
            ((edge_t*)(u_vector_at(Graph->_LE,i)))->v1->id,
            u_vector_at(Graph->_LE,i)
        );
    }
    return Graph->_MI;
    
}


list_indentifity*
graph_get_list_indetify(u_graph_v2* Graph)
{
    if(Graph->state&2) {return Graph->_LI;}
    

    graph_get_matrix_indetify(Graph);
    
    

    u_vector_free(Graph->_LI,free);
    Graph->_LI=u_vector_new();
    u_vector* el;
    for (size_t i = 0; i < *Graph->n_vertex; i++)
    {
        el=u_vector_new();
        u_vector_push_back(Graph->_LI,el);
        
        for (size_t j = 0; j < *Graph->n_vertex; j++)
        {
            edge_t* tmp = u_matrix_at(Graph->_MI,i,j);
            if(tmp){
              u_vector_push_back(el,tmp);  
            }
        }
        
    }
    Graph->state |= 2 ;
    return Graph->_LI;
}



size_t
graph_add_vertex(u_graph_v2* Graph,void* data,void (*free_func)(void *))
{
    Graph->state = 0;
    vertex_t* vrtx=malloc(sizeof(vertex_t));
    vrtx->id=(*Graph->n_vertex);
    vrtx->data=data;
    vrtx->free_func=free_func;
    u_vector_push_back(Graph->V,vrtx);
    return (*Graph->n_vertex)-1;
}

void
graph_add_edge(u_graph_v2* Graph,size_t v1, size_t v2,double weight)
{
    //TODO: проверка на переполнение
    Graph->state = 0;


    edge_t* edge=malloc(sizeof(edge_t));
    edge->v1=u_vector_at(Graph->V,v1);
    edge->v2=u_vector_at(Graph->V,v2);
    edge->weight=weight;
    u_vector_push_back(Graph->E,edge);
}


static
void free_priv(void* el)
{
    u_vector_free(el,NULL);
}

static
void free_priv1(void* el)
{
    if(((vertex_t*)el)->free_func){   
        ((vertex_t*)el)->free_func
        (
            ((vertex_t*)el)->data
        );
    }
    free(el);
}

void 
graph_free(u_graph_v2* Graph)
{

    u_vector_free(Graph->V, free_priv1);
    u_vector_free(Graph->E,free);

    if(Graph->_MI)
    {
        u_matrix_free(Graph->_MI);
    }
   

    u_vector_free(Graph->_LI,free_priv);
    free(Graph);
}

#include <stdio.h>

void
print_graph(u_graph_v2* Graph)
{
    graph_get_list_indetify(Graph);
    u_vector* el;
    edge_t* edge;
    for (size_t i = 0; i < *Graph->n_vertex; i++)
    {
        printf("Vertex №%d:\n",i);
        u_vector* el=u_vector_at(Graph->_LI, i);
        for (size_t j = 0; j < el->n_elem; j++)
        {
            edge=u_vector_at(el, j);
            printf("-edge %d-%d ",edge->v1->id, edge->v2->id);
            printf("weight %f\n", edge->weight);
        }
        printf("\n");
    }
    u_matrix_fprint(stdout,Graph->_MI);
}


void 
graph_vertex_set_data(
        u_graph_v2* Graph,      //Handler
        size_t vertex_id,       //id узла (вершины)
        void* data,             //данные
        void (*free_func)(void *), //деструктор или NULL для новых данных
        graph_free_old_flag flag    //вызвать деструктор имеющихся данных?
        )
{
    vertex_t* vrtx=u_vector_at(Graph->V,vertex_id);

    if((flag==GRAPH_EXEC_DESTRUCTOR) && vrtx->free_func)
    {
        vrtx->free_func(data);
    }
    vrtx->data=data;
    vrtx->free_func=free_func;
    u_vector_replace(Graph->V,vertex_id,vrtx);
}


void* 
graph_vertex_get_data(u_graph_v2* Graph,size_t vertex_id)
{
    vertex_t* vrtx=u_vector_at(Graph->V,vertex_id);
    return vrtx->data;
}





u_graph_v2* 
new_graph_from_graph_matrix(matrix_indentifity* mat)
{
    if(!mat) return NULL;
    if(mat->height != mat->lenght) return NULL;

    u_graph_v2* Graph = new_graph();
    edge_t* ed;
    for (size_t i = 0; i < mat->lenght; i++)
    {
        graph_add_vertex(Graph, NULL, NULL);
    }

    for (size_t i = 1; i < mat->lenght; i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            if((ed = u_matrix_at(mat,i,j))!=NULL)
            {
                graph_add_edge(Graph,i, j,ed->weight);
            }
        }
        
    }
    return Graph;
}
u_graph_v2* 
new_graph_from_pdouble_matrix(u_matrix* mat)
{
    if(!mat) return NULL;
    if(mat->height != mat->lenght) return NULL;

    u_graph_v2* Graph = new_graph();
    double* weight;
    for (size_t i = 0; i < mat->lenght; i++)
    {
        graph_add_vertex(Graph, NULL, NULL);
    }

    for (size_t i = 1; i < mat->lenght; i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            if((weight = u_matrix_at(mat,i,j))!=NULL)
            {
                graph_add_edge(Graph,i, j, *weight);
            }
        }
        
    }
    return Graph;
}

void save_graph_struct_to_csv(u_graph_v2* Graph, char* filename)
{

       matrix_indentifity* mat=graph_get_matrix_indetify(Graph);

       edge_t* ed;
       FILE* fs=fopen(filename,"w");
        for (size_t i = 0; i < mat->lenght; i++)
        {
            for (size_t j = 0; j < mat->lenght; j++)
            {
                if((ed = u_matrix_at(mat,i,j))!=NULL)
                {
                    fprintf(fs,"%f;",ed->weight);
                }else{
                    fprintf(fs,"nan;");
                }
            }
            fprintf(fs,"\n");            
        }
        fclose(fs);
}

u_graph_v2* load_graph_struct_from_csv(char* filename)
{
    FILE* fs=fopen(filename,"r");
    u_graph_v2* Graph=new_graph();
    char line[1024]={0};
    char* ptr;
    char* eptr;
    size_t i=0;
    size_t j=0;
    size_t len;
    size_t n_vertex=0;

    fgets(line,1024,fs);
    len = strlen(line);
    ptr=line; 
    eptr=line;
   
   puts(ptr);
    while (ptr<(line+len-1))
    {
        graph_add_vertex(Graph,NULL,NULL);

        eptr=strchr(ptr,';');
        if(eptr==0)break;
        eptr[0]=0;
        puts(ptr);
        if (strstr(ptr,"nan")){
            
        }else{
            graph_add_edge(Graph,i,j,atof(ptr));
        }
        ptr=eptr+1;
        j++;
    }
    n_vertex=j;
    
    for (size_t _i = 1; _i < n_vertex; _i++)
    {
        fgets(line,1024,fs);
        len = strlen(line);
        ptr=line; 
        eptr=line;
        for (size_t _j = 0; _j < n_vertex; _j++)
        {
            eptr=strchr(ptr,';');
            eptr[0]=0;
            if (strstr(ptr,"nan")){
                
            }else{
                graph_add_edge(Graph,_i,_j,atof(ptr));
            }
            ptr=eptr+1;
        }
        
    }

    fclose(fs);
    return Graph;
    


}



static void
print_graph_text(u_graph_v2* Graph)
{
    graph_get_list_indetify(Graph);
    u_vector* el;
    edge_t* edge;
    for (size_t i = 0; i < *Graph->n_vertex; i++)
    {
        printf("Vertex №%d: ",i);
        printf("~%s~\n", ((vertex_t*)u_vector_at(Graph->V, i))->data);
        u_vector* el=u_vector_at(Graph->_LI, i);
        for (size_t j = 0; j < el->n_elem; j++)
        {
            edge=u_vector_at(el, j);
            printf("-edge %d-%d: ",edge->v1->id, edge->v2->id);
            printf("%s-%s\n",((vertex_t*)u_vector_at(Graph->V, edge->v1->id))->data, ((vertex_t*)u_vector_at(Graph->V, edge->v2->id))->data);
        }
        printf("\n");
    }
    u_matrix_fprint(stdout,Graph->_MI);
}

void graph_test()
{
    u_graph_v2* Graph = new_graph();
    graph_add_vertex(Graph, "Чингисхан", NULL);
    graph_add_vertex(Graph, "Хан Батый", NULL);
    graph_add_vertex(Graph, "Киану Ривз", NULL);
    graph_add_vertex(Graph, "Саакашвили", NULL);
    graph_add_vertex(Graph, "Ассонанс", NULL);

    graph_add_edge(Graph,0 , 1, 12.);
    graph_add_edge(Graph,1 , 2,100.);
    graph_add_edge(Graph,2 , 3, 23.);
    graph_add_edge(Graph,2 , 4, 67.);
    graph_add_edge(Graph,3 , 4, 69.);
    graph_add_edge(Graph,4 , 0,666.);

    print_graph_text(Graph);

    u_graph_v2* Graph2=new_graph_from_graph_matrix(graph_get_matrix_indetify(Graph));
    print_graph(Graph2);
    save_graph_struct_to_csv(Graph2, "test.csv");
    graph_free(Graph);
    graph_free(Graph2);


    u_graph_v2* Graph3 = load_graph_struct_from_csv("test.csv");
    save_graph_struct_to_csv(Graph3, "test2.csv");
    graph_free(Graph3);





}

