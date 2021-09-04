#ifndef U_SERVERMM
#define U_SERVERMM
// extern "C"{
#include <u_network/u_network.h>
// };

class u_server
{
protected:
    u_socket_t in=0;  //for reading
    u_socket_t out=0; //for writing

    struct timeval timeout={.tv_sec=3,.tv_usec=0};

    union
    {
        u_sockaddr_in_t addr_in;
        u_sockaddr_t addr;
    };

    virtual void read_handle()   = 0;
    virtual void write_handle()  = 0;
    virtual void accept_handle() = 0;
    
public:
    u_server(uint16_t port=8088);
    ~u_server();

    void update();

    
};

//==========================================================

class simple_proto : public u_server
{

private:
     void read_handle() override;
     void write_handle() override;
     void accept_handle() override;
public:
    simple_proto(/* args */);
    ~simple_proto();
};

void test_server();
#endif // !U_SERVER
