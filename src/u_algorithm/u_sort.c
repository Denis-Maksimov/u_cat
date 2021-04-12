/************************************
 *      Квадратичные сортировки     *
************************************/


///------Сортировка вставками-------
// каждого следующего сверяем с отсортированным предыдущим и протаскиваем его на место
void insert_sort_const(int* dest, const int* src, int n)
{

    int tmp;
    for (int i=0;i<n;i++){
        dest[i]=src[i];
        for(int j=i;j;j--){
            if(dest[j]<dest[j-1]) {
                tmp=dest[j-1];
                dest[j-1]=dest[j];
                dest[j]=tmp;
            }
        }
    }
    
    return;
}

void insert_sort(int* src, int n)
{

    int tmp;
    for (int i=1;i<n;i++)
    {
        for(int j=i;j;j--)
        {
            if(src[j]<src[j-1]) 
            {   
                //---swap---
                tmp=src[j-1];
                src[j-1]=src[j];
                src[j]=tmp;
            }
        }
    }
    
    return;
}
///------Сортировка выбором-------
//~~ ищем минимальный и меняем с первым, снова ищем минимальный и меняем со вторым и т.д. 
int find_min(int* src, int n)
{
    int min=src[0];
    int index=0;
    for (int i=1;i<n;i++)
    {
        if(src[i]<min)
        {
            min=src[i];
            index=i;
        }
    }
    return index;
}

void choise_sort(int* src, int n)
{
    int tmp;
    int index;
    for (int i=0;i<n-1;i++)
    {
        index=find_min(&src[i],n-i)+i;
        if(src[index]<src[i])
        {
            tmp=src[index];
            src[index]=src[i];
            src[i]=tmp;
        }
    }

}

///------Сортировка пузырьком----------
// попарно меняем последовательно, т.о. наибольший элемент как пузырёк "всплывает" вверх
void bubble_sort(int* src, int n)
{
    int tmp;
    int flag=0;//если заа итерацию не поменяли, то массив отсортирован
    while (n)
    {
        for(int i=1;i<n;i++)
        {
            if(src[i-1]>src[i])
            {
                tmp=src[i-1];
                src[i-1]=src[i];
                src[i]=tmp;
                flag=1;
            }
        }
        if(!flag) return;
        flag=0;
        n--;
    }
    

}
/************************************
 *      Рекурентные сортировки     *
************************************/
//------ Сортировка Тони Хоара (быстрая) ----------
// берём базис и раскидываем на больше меньше и равно
// и так рекурсивно


static void _quickSort(int *numbers, int left, int right)
{
  int l_hold = left; //левая граница
  int r_hold = right; // правая граница
  int barrier = numbers[left]; // барьерный элемент
  
  while (left < right) // пока границы не сомкнутся
  {
    while ((numbers[right] >= barrier) && (left < right))
      right--; // сдвигаем правую границу пока элемент [right] больше [barrier]
    if (left != right) // если границы не сомкнулись
    {
      numbers[left] = numbers[right]; // перемещаем элемент [right] на место разрешающего
      left++; // сдвигаем левую границу вправо
    }
    while ((numbers[left] <= barrier) && (left < right))
      left++; // сдвигаем левую границу пока элемент [left] меньше [barrier]
    if (left != right) // если границы не сомкнулись
    {
      numbers[right] = numbers[left]; // перемещаем элемент [left] на место [right]
      right--; // сдвигаем правую границу вправо
    }
  }
  numbers[left] = barrier; // ставим барьерный элемент на место
  barrier = left;
  left = l_hold;
  right = r_hold;
  if (left < barrier) // Рекурсивно вызываем сортировку для левой и правой части массива
    _quickSort(numbers, left, barrier - 1);
  if (right > barrier)
    _quickSort(numbers, barrier + 1, right);
}

void quick_sort(int *src, int n){
    _quickSort(src,0,n-1);
}
//------ Сортировка слиянием ----------
//делим пополам, сортируем каждую половину, мержим




/************************************
 *      Прочее                      *
************************************/
//------ Гномья сортировка ------------
void Gnome_sort( int *mas,int n)
{
    int i=1; int j=2;
    while (i < n)
    {
        if (mas[i - 1] <= mas[i]){
            i = j;
            j++;
        }else{
            int t = mas[i];
            mas[i] = mas[i - 1];
            mas[i - 1] = t;
            i--;
            if (!i){
                i = j;
                j++;
            }

        }
    }//while
}
//------ Сортировка подсчётом------------
// при малом разбросе значений
// хреначим отсортированный массив из уникальных элементов, считаем сколько раз каждый встретился, размножаем


