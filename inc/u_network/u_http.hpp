#ifndef U_HTTP
#define U_HTTP
#include "u_server.hpp"
#include <sstream>
#include <regex>
#include <unordered_map>







class u_http:
public u_server
{
private:

    typedef std::unordered_map<std::string,std::string> Dict;

    void read_handle()   override;
    void write_handle()  override;
    void accept_handle() override;

    void parse_HTTPrequest();
    std::stringstream HTTPbuffer;

    std::regex re_splitter;
    std::regex re_head;
    std::regex re_meta;
    std::smatch m;
    Dict params;
protected:
    std::stringstream HTTPcontent;
    std::stringstream content;
public:
    u_http(/* args */);
    ~u_http();
};


void test_http();















#endif
