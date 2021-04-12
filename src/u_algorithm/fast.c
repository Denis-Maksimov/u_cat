

// Функция быстрой сортировки
void quickSort(int *numbers, int left, int right)
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
    quickSort(numbers, left, barrier - 1);
  if (right > barrier)
    quickSort(numbers, barrier + 1, right);
}


