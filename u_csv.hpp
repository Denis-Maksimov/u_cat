#pragma once
#include "u_matrix.hpp"
#include <string>
#include <fstream>


class u_csv: public u_matrix<std::string>
{
public:
    u_csv();
    u_csv(std::string path);
    ~u_csv();
    int read_csv();
private:
    std::string* data;
    std::string path;
    
};

