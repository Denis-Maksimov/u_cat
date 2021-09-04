#include <u_network/u_http.hpp>
#include <iostream>
#include <u_application/slre.h>
#include <u_application/u_strtokens.h>

// локальная функция парсинга
static char* foo(const char *regexp, char* fbuf,size_t size,struct slre_cap* caps)
{

    char* dest;
    if ((slre_match(regexp, fbuf, size, caps, 2, 0)) > 0) 
        {
            dest=(char*)malloc(caps[1].len+1);
			sprintf(dest, "%.*s", caps[1].len, caps[1].ptr);
			dest[caps[1].len]=0;//для уверенности
        } else{ 

            return NULL;
        }
}

u_http::u_http(/* args */)
:u_server()
{
    re_splitter.assign(R"([\s]{2,})");
    re_head.assign(R"(^(GET|HEAD|PUT|POST|DELETE)\s+\/(.+\b)\s+HTTP\/1.\d)");
    re_meta.assign(R"(^([\w\S]+\b):\s(.+)$)");

}

u_http::~u_http()
{

}



void 
u_http::read_handle()
{
    std::cout << "read_handle is working???\n";
    char buff[1024];
    ssize_t n=1;

    if ((n=recv(out,buff,1024,0))>0)
    {
        buff[n]=0;
        HTTPbuffer.str(buff);
        // return;
    }else{
        std::cout<<"read"<<n;
    }
}

#include <string>
#include <cstring>
#define _send(out,msg) (send(out,msg,sizeof(msg)-1,0))


void
u_http::parse_HTTPrequest()
{
    try{


        std::string buf;
        // std::regex_search(buf, m, re_splitter);
        std::cout<<HTTPbuffer.str();


        std::regex_search(buf, m, re_head);

        for (auto i : m)
        {
            std::cout << m.str()<<"\n";
        }
        
        
        if(m.size() == 3)
        {
        params["Method"]=m[1];
        params["URI"]=m[2];
        }else{
            std::cout<<"header";
            throw 400;
        }
        

        std::getline(HTTPcontent,buf);
        int it=0;
        while(buf.npos!=buf.find(':'))
        {
            std::regex_search(buf, m, re_meta);
            if(m.size() == 3)
            {
                params[m[1].str()]=m[2].str();
                std::getline(HTTPcontent,buf);
            }else {
                std::cout<<"to many";
                throw 400;
            }
        }

        content.str("");
        content << HTTPcontent.str().substr(HTTPcontent.tellg());
        std::cout<<content.str();

        if(params["Method"].compare("GET"))
        {
            _send(out,"HTTP/1.1 200 OK\r\n");
        }else{
            throw 405;
        }


    }catch(int err)
    {
        switch (err)
        {
        case 400:
        std::cout <<"Ставь чайник, зажигай плиту..\n";
            _send(out,"HTTP/1.1 418 I’m a teapot\n\r");
            break;
        case 405:
            _send(out,"HTTP 400 Method Not Allowed\r\n");
            break;
        case 418:
        case 505:
            _send(out,"HTTP 418 I’m a teapot\r\n");
            std::cout <<"Ставь чайник, зажигай плиту..\n";
            break;
        
        default:
            _send(out,"HTTP 418 I’m a teapot\r\n");
            break;
        }

    }

}





void 
u_http::write_handle()
{
    std::cout << "http\n";
    char buff[1024];
    ssize_t n=1;

    // if ((n=recv(out,buff,1024,0))>0)
    // {
    //     buff[n]=0;
    //     HTTPbuffer << buff;
    //     // return;
    // }

    parse_HTTPrequest();

    // fflush(stdout);

    
    if(out)
        send(out,"hello",5,0);
        u_close_sock(out);
    out=0;
    content.str("");
    HTTPcontent.str("");
    HTTPbuffer.str("");
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
u_http::accept_handle()
{
    u_sockaddr_t client_addr;
    int size_client_addr;
    
    out=u_accept( in, &client_addr, &size_client_addr);

    if(out==-1){printf(" err accept\n"); out=0;}
    print_client_addr(&client_addr);

}



void test_http()
{
    u_http Serv;
    for (size_t i = 0; i < 15; i++)
    {
        Serv.update();
    }
    
}



