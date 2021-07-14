#include "u_matrix.hpp"

// u_matrix<std::string> mata(0,0);

template class u_matrix<std::string>;

template <class Type>
u_matrix<Type>::u_matrix(size_t i,size_t j):
lenght(j),height(i),n(i*j)
{
    this->data=new std::vector<Type>(n);
}

template <class Type>
u_matrix<Type>:: ~u_matrix()
{
    delete this->data;
};

//-----------------------------------------


template <class Type>
Type u_matrix<Type>::at(size_t i,size_t j)
{
    if(i>=height||j>=lenght) return 0;
    return this->data->at(i*(this->lenght)+j);
}



template <class Type>
void u_matrix<Type>::push_line()
{

}

template <class Type>
void u_matrix<Type>::push_column()
{

}


template <class Type>
u_matrix<Type>* u_matrix<Type>::
get_submatrix(size_t i1,size_t j1, size_t i2,size_t j2)
{

    u_matrix<Type>* rv = new u_matrix<Type>(i2-i1, j2-j1);
    Type tmp;
    for (size_t i = i1; i < i2; i++)
    {
        for (size_t j = j1; j < j2; j++)
        {
            tmp = at(i,j);
            rv->insert(i-i1, j-j1, tmp);
        }
        
    }
    return rv;

}


template <class Type>
void u_matrix<Type>::
insert(size_t i,size_t j, Type& el)
{
    if(i>=height||j>=lenght) return ;
    this->data->at(i*lenght+j)=el;
    // this->data->insert(i*lenght+j,el);
//    this->data[()]=el;
}

template <class Type>
void u_matrix<Type>::
print()
{

   for (size_t i = 0; i < this->height; i++)
    {
        for (size_t j = 0; j < this->lenght; j++)
        {
            std::cout << this->at(i,j) << "\t";

        }
        std::cout << std::endl;
    }
    
}



template <class Type>
auto u_matrix<Type>::
begin(){
    return data->begin();
}

template <class Type>
auto
u_matrix<Type>::
end(){
    return data->end();
}
//---------------------------



template <class Type>
void u_matrix<Type>::
transpose(){

    Type tmp;
    u_matrix<Type> rv1(lenght,height);
   for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < lenght; j++)
        {
            tmp=at(i,j);
            rv1.insert(j,i,tmp);
        }
        
    }
   
   data->swap(*(rv1.data));
   lenght=rv1.lenght;
   height=rv1.height;  

}




static int 
test(int argc, char const *argv[])
{
    u_matrix<double> mat(3,8);

    size_t i=0;
    for(auto& var : mat)
    {
        var=++i/10.;
    }
    mat.print();
    mat.transpose();
    mat.print();

    return 0;
}
