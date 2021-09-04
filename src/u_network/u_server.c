#include <u_network/u_server.h>
#include <stdlib.h>

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
    fsm_change_state(f,(fsm_func)std_close_handler,srv);

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
            fsm_change_state(srv->fsm_wr,(fsm_func)std_wr_handler,srv);
            else
            srv->fsm_wr=fsm_new((fsm_func)std_wr_handler,srv);
            // return;
        }
    fsm_end_state(f);
}

//=====================================================================

static void 
init_server(uint16_t port, io_sockets* io)
{
    /********************************************
    1. создание серверного сокета*/

    
    io->in=socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(io->in<=0){
        printf("1)   ERROR! Socket is'n created\n");
    }else{ 
        printf("1)   Socket created is succes.\n");
    }
    // int yes=1;
    // setsockopt(io->in,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    
    /********************************************
    2. Привязка к локальным именам*/

    u_sockaddr_in_t addr;//создаем прототип(addr), структуры sockaddr_in
    addr.sin_family=AF_INET;//при создании Интернет-приложений


    addr.sin_port=htons(port);//порт
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
 
    int i_error=0;
    i_error = bind( io->in, (u_sockaddr_t*)&addr,  sizeof(addr)  );

    if(i_error < 0){ 
        
        printf("2)   ERROR binding!\n");
    }else{ 
        printf("2)   Binding success!\n");
    }
    
    /********************************************
    3. Инициализация процесса «прослушивания» порта*/
    
    
    listen( io->in,3 );
    {//говорим в stdout какой у нас порт
        int tmp=sizeof(u_sockaddr_t);
        u_getsockname(io->in, (u_sockaddr_t*)&addr, &tmp);
        printf("your port = %d\n",ntohs(addr.sin_port));
    }
    io->out=0;
    
}

static void 
deinit_server(io_sockets* io)
{
    
    u_close_sock(io->in);
    if(io->out) u_close_sock(io->out);
}


u_server* 
u_server_new(uint16_t port)
{
    u_server* rv = malloc(sizeof(u_server));
    rv->io.in=0;
    rv->io.out=0;
    rv->rd=std_rd_handler;
    rv->wr=std_wr_handler;

    init_server(port, &rv->io);
    rv->timeout.tv_sec=3;
    rv->timeout.tv_usec=0;
    rv->fsm_rd=0;
    rv->fsm_wr=0;

    rv->buffer = u_vector_new();
    // rv->handler=standart_handler;
   
    return rv;
}

void
u_server_free(u_server* srv)
{
    deinit_server(&srv->io);
    u_vector_free(srv->buffer,free);
    free(srv);
    
}

int 
u_server_update(u_server* srv)
{

    struct timeval tv;
    fd_set fd_in,fd_out;
    u_socket_t largest_sock = srv->io.in;
    
    while(1){  
        FD_ZERO( &fd_in );
        FD_ZERO( &fd_out );
        FD_SET( srv->io.in, &fd_in );//добавляем в сет
        if(srv->io.out){
            FD_SET( srv->io.out, &fd_in );//добавляем в сет
            FD_SET( srv->io.out, &fd_out );//добавляем в сет
            
        }
        tv.tv_sec = srv->timeout.tv_sec;
        tv.tv_usec = srv->timeout.tv_usec;
  
        largest_sock=(srv->io.in < srv->io.out)?(srv->io.out):(srv->io.in);
        
        int ret = select( largest_sock + 1, &fd_in, &fd_out, NULL, &tv );
        // проверяем успешность вызова
        if ( ret == -1 )
        {
            printf("error\n");
            return -1;  

        }else if( ret == 0 )
        {
            //!TODO
            // таймаут, событий не произошло
            
            return 0;

        }else{
            // обнаружили событие
            if ( FD_ISSET( srv->io.in, &fd_in ) && srv->io.out==0 )
            {
                //CONNECTING
                // srv->state=1;
                srv->io.out=u_accept( srv->io.in, 0, 0);
                if(srv->io.out==-1){printf(" err accept\n");srv->io.out=0;}
                printf("new client\n");
            }
             if ( FD_ISSET(srv->io.out, &fd_in )&& srv->io.out)
            {
                //HANDLE DATA
                // srv->state=2;
                printf("read data\n");
                if(!srv->fsm_rd)
                    srv->fsm_rd = fsm_new((fsm_func)srv->rd,srv);
                
                if (!fsm_update(srv->fsm_rd))
                {
                        srv->fsm_rd = 0;
                }
            }
             if ( FD_ISSET( srv->io.out, &fd_out )&& srv->io.out)
            {
                //WRITE MESSAGE AVAILABLE
                printf("writing avail\n");
                if (!fsm_update(srv->fsm_wr))
                {
                        srv->fsm_wr = 0;
                }

            } 
            //end events
        }   
        //end cycle
        printf("end cycle\n");

    }
    //finalize

}


//!CHECKME
void u_server_send_to(u_server* srv,const void *buf, size_t len, uint32_t ip,uint16_t port)
{
    
    u_sockaddr_in_t addr;//создаем прототип(addr), структуры sockaddr_in
    addr.sin_family=AF_INET;//при создании Интернет-приложений
    addr.sin_port=htons(port);//порт
    addr.sin_addr.s_addr=(ip);

    struct sockaddr* dest_addr=(struct sockaddr*)(&addr);
    sendto(srv->io.in, buf, len, 0, dest_addr, sizeof(struct sockaddr));
}

//.....................................................
//..======..||==../==\...======........................
//....||....||==..\==\.....||..........................
//....||....||==../==/.....||..........................
//.....................................................

int howto_test_v2 (void)
{

    
    /********************************************
    0. настройка библиотеки Ws2_32.dll*/
    if (start_NW()) return -1;

    u_server* srv=u_server_new(3000);

    for (size_t i = 0; i < 15; i++)
    {
        u_server_update(srv);
    }
    
    u_server_free(srv);
    stop_NW();
    return 0;
}
