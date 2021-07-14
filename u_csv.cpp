#include "u_csv.hpp"
#include <sstream>
#include <vector>

u_csv::u_csv():u_matrix(1,1)
{

}

u_csv::u_csv(std::string _path):u_matrix(1,1),path(_path)
{
    std::ifstream ifs(path);
    data=new std::string;
    std::string tmp,col;
    std::vector<std::vector<std::string> > vec;
    std::vector<std::string> vecCol;
    std::stringstream ss;

    std::size_t max=0;
    std::size_t lines=0;
    while (!ifs.eof())    
    {
        lines++;
        //   ifs >> tmp ;
         std::getline(ifs,tmp);
         ss.str(tmp);
         while(std::getline(ss,col,';'))
         {
             vecCol.push_back(col);
         }
         max= max>vecCol.size()?max:vecCol.size();
         vec.push_back(vecCol);
         vecCol.clear();
    }

    for (size_t i = 0; i < max; i++)
    {
        push_column();
    }
    for (size_t i = 0; i < lines; i++)
    {
        push_line();
    }
    size_t i=0;
    size_t j=0;
    for(auto& col : vec)
    {
        for(auto& var : col)
        {
            insert(i,j,var);
            j++;
        }
        j=0;
        i++;
    }
    std::cout << lines << " " << max;
    print();
    // *data=ss.str();
    // std::cout << *data << std::endl;     

}

u_csv::~u_csv()
{
    delete data;
}







int main(int argc, char const *argv[])
{
    
    u_csv fl("./t.cpp");
    
    return 0;
}















