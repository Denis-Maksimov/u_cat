
#include <u_network/u_server.hpp>


u_server::u_server(uint16_t port)
{
    try
    {
        /********************************************
        1. создание серверного сокета*/

        
        in=socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(in<=0){
            
            throw -1;
        }
        printf("1)   Socket created is succes.\n");

        // int yes=1;
        // setsockopt(in,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
        
        /********************************************
        2. Привязка к локальным именам*/

        // u_sockaddr_in_t addr;//создаем прототип(addr), структуры sockaddr_in
        addr_in.sin_family=AF_INET;//при создании Интернет-приложений

        addr_in.sin_port=htons(port);//порт
        addr_in.sin_addr.s_addr=htonl(INADDR_ANY);
    
        int i_error=0;
        i_error = bind( in,  &addr,  sizeof(addr)  );

        if(i_error < 0){ 
            throw -2;
        }
        printf("2)   Binding success!\n");

        
        /********************************************
        3. Инициализация процесса «прослушивания» порта*/
        
        
        listen( in,3 );
        {//говорим в stdout какой у нас порт
            int tmp=sizeof(u_sockaddr_t);
            u_getsockname(in, (u_sockaddr_t*)&addr, &tmp);
            printf("your port = %d\n",ntohs(addr_in.sin_port));
        }
        out=0;

    }catch(int e){

        switch (e)
        {
        case -1:
            fprintf(stderr,"1)   ERROR! Socket is'n created\n");
            break;
            
        case -2:
            fprintf(stderr, "2)   ERROR binding!\n");
            break;
        default:
            break;
        }
        
    }
}

u_server::~u_server()
{
    u_close_sock(in);
    if(out) u_close_sock(out);
}






void 
u_server::update()
{
    try{
        struct timeval tv;

        fd_set fd_in,fd_out;
        u_socket_t largest_sock = in;
        
        while(1){  
            FD_ZERO( &fd_in );
            FD_ZERO( &fd_out );

            FD_SET(  in, &fd_in );//добавляем в сет
            FD_SET(  in, &fd_out );//добавляем в сет
            if(out){
                FD_SET( out, &fd_in );//добавляем в сет
                FD_SET( out, &fd_out );//добавляем в сет
            }
            tv.tv_sec = timeout.tv_sec;
            tv.tv_usec = timeout.tv_usec;
    
            largest_sock=(in < out)?(out):(in);
            
            int ret = select( largest_sock + 1, &fd_in, &fd_out, NULL, &tv );
            // проверяем успешность вызова
            if ( ret == -1 )
            {
                
                throw -1;  

            }else if( ret == 0 )
            {
                //!TODO
                // таймаут, событий не произошло
                
                return;

            }else{
                // обнаружили событие
                if ( FD_ISSET( in, &fd_in ) && out==0 )
                {
                    //CONNECTING                    
                    printf("new client\n");
                    accept_handle();
                    
                }
                if ( FD_ISSET(out, &fd_in )&& out)
                {
                    //HANDLE DATA

                    printf("read client\n");
                    read_handle();
                    //TODO 
                }




                if ( FD_ISSET( in, &fd_out ) )
                {
                    //CONNECTING

                    printf("srv write\n");
                }
                if ( FD_ISSET( out, &fd_out ))
                {
                    //WRITE MESSAGE AVAILABLE
                    printf("client write\n");
                    write_handle();
                } 
                //end events
            }   
            //end cycle
            printf("end cycle\n");

        }
        //finalize
    }catch(int e){
        fprintf(stderr,"error\n");
    }
}

//==================================================

simple_proto::simple_proto(/* args */)
:u_server()
{


}

simple_proto::~simple_proto()
{
}

void 
simple_proto::read_handle()
{
    char buff[1024];
    
    ssize_t n=recv(in,buff,1024,0);
    buff[n]=0;
    printf(buff);
    fflush(stdout);
    // sleep(3);
}

void 
simple_proto::write_handle()
{
     char buff[1024];
    ssize_t n=recv(out,buff,1024,0);
    buff[n]=0;
    printf(buff);
    fflush(stdout);

    send(out,"hello",5,0);
    u_close_sock(out);
    out=0;
    
}

static void 
print_client_addr(u_sockaddr_t* client_addr)
{

    u_sockaddr_in_t* cli=(u_sockaddr_in_t*)(client_addr);
       union
    {
        u_sockaddr_t client_addr;
        u_sockaddr_in_t cli;

    }cl;
    union{
        struct 
        {
            uint8_t b1;
            uint8_t b2;
            uint8_t b3;
            uint8_t b4;
        };
        uint32_t add;
    } _ad;

    _ad.add=cli->sin_addr.s_addr;
    
    printf("%d.%d.%d.%d:%d\n",_ad.b1,_ad.b2,_ad.b3,_ad.b4, ntohs(cli->sin_port));

}


void 
simple_proto::accept_handle()
{
    u_sockaddr_t client_addr;
    int size_client_addr;
    
    out=u_accept( in, &client_addr, &size_client_addr);

    if(out==-1){printf(" err accept\n"); out=0;}
    print_client_addr(&client_addr);

}


void test_server( )
{
    simple_proto srv;

    for (size_t i = 0; i < 15; i++)
    {
        srv.update();
    }
    
    
    return;
}




