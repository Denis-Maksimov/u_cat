#include <u_network/u_server.h>
#include <u_types/u_graph_v2.h>


typedef struct
{
    u_graph_v2* topology_graph;
    size_t my_id;
    uint64_t* recv_data;
    struct sockaddr from_addr;
} topology_data;
 



enum{
    ADD_SELF_NODE=1,
    ADD_SELF_EDGE,
    ADD_NODE,
    ADD_EDGE
};

struct topology_add_edge
{
    uint64_t placeholder;
    struct sockaddr from;
    struct sockaddr to;
};








//=====================================================================
static void 
topology_proto_handler(fsm* f,u_server* srv)
{
   topology_data* proto=srv->proto_data;
   if(proto->recv_data[0]==ADD_SELF_NODE)
   {
       void* data=malloc(sizeof(struct sockaddr));
       memcpy(data,&proto->from_addr,sizeof(proto->from_addr));
       graph_add_vertex(proto->topology_graph, data, free);
   }
   if(proto->recv_data[0]==ADD_EDGE)
   {
       struct topology_add_edge data;
       //TODO: find addreses, if no math then add
       graph_add_edge(proto->topology_graph, 1, 2, 1.);
   }

}




static void 
topology_close_handler(fsm* f,u_server* srv)
{
    shutdown(srv->io.out,SHUT_RDWR);
    u_close_sock(srv->io.out);
    puts("SHUTDOWN");
    srv->io.out=0;
    // fsm_end_state(f); 
    fsm_free(f);
    srv->fsm_wr=0;
}


static void 
topology_wr_handler(fsm* f,u_server* srv)
{
    // if(srv->state!=3) return;
    char msg[]="Hello client\r\n";
    ssize_t i=send( srv->io.out, msg, 14, 0);
    printf("sendok:%d\n",i);
    fflush(stdout);
    fsm_change_state(f,topology_close_handler,srv);

    char* data_from_client=u_vector_pop_back(srv->buffer);
    // printf("Received data:\n%.*s\n",1024, data_from_client);
    free(data_from_client);

    // u_close_sock(srv->io.out);
    // fsm_end_state(f); 
}


static void 
topology_rd_handler(fsm* f,u_server* srv)
{
    char data_from_client[1024]={0};
    int n = recv( srv->io.out, data_from_client, 1024, 0 );
    topology_data* proto=srv->proto_data;

    
    recvfrom( srv->io.out, data_from_client, 1024, 0 ,&proto->from_addr,sizeof(proto->from_addr));
        

        // fflush(stdout);
        //отправка
        if(n>0){
                        
            char* buff=calloc(n,sizeof(char));
            memcpy(buff,data_from_client,n);
            // printf("Received data:\n%.*s\n", n,data_from_client);        
            if(proto->recv_data!=NULL){
                free(proto->recv_data);
            }
            proto->recv_data=buff;


            if(srv->fsm_wr)
            fsm_change_state(srv->fsm_wr,topology_proto_handler,srv);
            else
            srv->fsm_wr=fsm_new(topology_proto_handler,srv);
            // return;
        }
    // fsm_end_state(f);
}

//=====================================================================


void
u_server_set_topology_proto(u_server* srv)
{

    srv->rd=topology_rd_handler;
    srv->wr=topology_wr_handler;

    srv->fsm_rd=0;
    srv->fsm_wr=0;

    srv->proto_data = malloc(sizeof(topology_data));
    topology_data* proto = srv->proto_data;

    proto->topology_graph= new_graph();
    proto->my_id=0;
    graph_add_vertex(proto->topology_graph, (void*)proto->my_id, NULL);
    
   
}



void
u_server_free_topology_proto(u_server* srv)
{

    srv->rd=0;
    srv->wr=0;

    srv->fsm_rd=0;
    srv->fsm_wr=0;
    topology_data* proto = srv->proto_data;
    graph_free(proto->topology_graph);



}

