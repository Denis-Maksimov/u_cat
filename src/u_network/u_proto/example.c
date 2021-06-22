#include <u_network/u_server.h>

//=====================================================================

static void 
std_close_handler(fsm* f,u_server* srv)
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
std_wr_handler(fsm* f,u_server* srv)
{
    // if(srv->state!=3) return;
    char msg[]="Hello client\r\n";
    ssize_t i=send( srv->io.out, msg, 14, 0);
    printf("sendok:%d\n",i);
    fflush(stdout);
    fsm_change_state(f,std_close_handler,srv);

    char* data_from_client=u_vector_pop_back(srv->buffer);
    // printf("Received data:\n%.*s\n",1024, data_from_client);
    free(data_from_client);

    // u_close_sock(srv->io.out);
    // fsm_end_state(f); 
}


static void 
std_rd_handler(fsm* f,u_server* srv)
{
    char data_from_client[1024]={0};
        int n = recv( srv->io.out, data_from_client, 1024, 0 );

        
        

        // fflush(stdout);
        //отправка
        if(n>0){
            
            char* buff=calloc(n,sizeof(char));
            memcpy(buff,data_from_client,n);
            // printf("Received data:\n%.*s\n", n,data_from_client);        
            u_vector_push_back(srv->buffer, buff);

            if(srv->fsm_wr)
            fsm_change_state(srv->fsm_wr,std_wr_handler,srv);
            else
            srv->fsm_wr=fsm_new(std_wr_handler,srv);
            // return;
        }
    fsm_end_state(f);
}

//=====================================================================


void
u_server_set_std_proto(u_server* srv)
{

    srv->rd=std_rd_handler;
    srv->wr=std_wr_handler;

    srv->fsm_rd=0;
    srv->fsm_wr=0;

    srv->buffer = u_vector_new();

   
}




