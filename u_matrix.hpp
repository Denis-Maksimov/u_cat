// #ifndef __U_MATRIX_H__
// #define __U_MATRIX_H__
#pragma once

#include <stddef.h>
#include <vector>
#include <iostream>




template <class Type>
class u_matrix
{
private:
    size_t lenght;
    size_t height;
    size_t n;
    
    std::vector<Type>* data;
public:
    
    explicit u_matrix(size_t i,size_t j);
     ~u_matrix();
    Type at(size_t i,size_t j);
    void insert(size_t i,size_t j, Type& el);
    void push_line();
    void push_column();
    u_matrix<Type>* get_submatrix(size_t i1,size_t j1, size_t i2,size_t j2);
    void print();
    void transpose();
    auto begin();
    auto end();

};











// #endif // 
