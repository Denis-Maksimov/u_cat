/******************************************
 * Структура данных бинарного дерева поиска,
 * добавляет, попутно сортируя значения.
 * слева - меньше; справа - больше
 * ***************************************/
#include <stdio.h>
#include <stdlib.h>
#define c_new(t)     ((t*)malloc(sizeof(t)))

// Структура - узел дерева
typedef struct tnode_tag
{
  int key;           // поле данных
  struct tnode_tag *left;  // левый потомок
  struct tnode_tag *right; // правый потомок
  void* data;             //данные
} tnode;

// typedef struct tnode_tag tnode;



// Вывод узлов дерева (обход в инфиксной форме)
void treeprint(tnode *tree)
{
  static int i=0;
  i++;
  if (tree != NULL) {      //Пока не встретится пустой узел
    treeprint(tree->left);  //Рекурсивная функция вывода левого поддерева
    printf("lvl:%d - %d\n",i, tree->key);//Отображаем корень дерева
    treeprint(tree->right); //Рекурсивная функция вывода правого поддерева
  }
  i--;
}


// Добавление узлов в дерево
tnode* addnode(int x, tnode *tree) {
  if (!tree)     // Если дерева нет, то формируем корень
  {
    tree = c_new(tnode); //память под узел
    tree->key = x;   //поле данных
    tree->left = 0;
    tree->right = 0; //ветви инициализируем пустотой
    tree->data = 0;
  }
  else{     // иначе
  
    if (x < tree->key){   //Если элемент x меньше корневого, уходим влево
      tree->left = addnode(x, tree->left); //Рекурсивно добавляем элемент
    }else{  //иначе уходим вправо
      tree->right = addnode(x, tree->right); //Рекурсивно добавляем элемент
    }
  }
  return(tree);
}

//Освобождение памяти дерева
void freemem(tnode *tree)
{
  if (tree != NULL)    // если дерево не пустое
  {
    freemem(tree->left);   // рекурсивно удаляем левую ветку
    freemem(tree->right);  // рекурсивно удаляем правую ветку
    free(tree);           // удаляем корень
  }
}

tnode* get_elem_by_key(tnode* tree,int key)
{
  if(!tree) return 0;
  if(key==tree->key){
    return tree;
  }
  if(key > tree->key)
  {
    return get_elem_by_key(tree->right,key);
  }
  if(key < tree->key)
  {
    return get_elem_by_key(tree->left,key);
  }
}




int nosort[]={5,12,33,7,569,7,578,1};

// Тестирование работы
int main()
{
  tnode *root = 0;    // Объявляем структуру дерева
  // В цикле вводим 8 узлов дерева
  for (int i = 0; i< 8; i++) 
  {
    root = addnode(nosort[i], root); // размещаем введенный узел на дереве
  }
  treeprint(root);    // выводим элементы дерева, получаем отсортированный массив
  freemem(root);      // удаляем выделенную память
  return 0;
}
