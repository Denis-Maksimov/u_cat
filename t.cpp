#include <vector>
#include <iostream>
 
 template <typename T>
class Foo
{
private:
   typedef std::vector<T> TVec;
private:
   TVec mVector;
public:
   Foo() : mVector({1,2,3,4,5})
   {
   }
   
   std::vector<int>::iterator begin()
   {
      return mVector.begin();
   }
   
   std::vector<int>::iterator end()
   {
      return mVector.end();
   }
};
 
int main()
{
   Foo<int> f;
   
   for (auto& i : f)
      std::cout << i++;
   std::cout << std::endl;
   for (auto i : f)
      std::cout << i;
}