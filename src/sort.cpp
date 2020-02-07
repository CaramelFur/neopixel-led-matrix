#include "sort.h"

bool builtinLargerThan(char *first, char *second)
{
  return strcmp(first, second) > 0;
}

void insertionSort(char *array[], size_t sizeOfArray, bool (*largerThan)(char *, char *))
{
  for (size_t i = 1; i < sizeOfArray; i++)
  {
    for (size_t j = i; j > 0 && largerThan(array[j - 1], array[j]); j--)
    {
      char *tmp = array[j - 1];
      array[j - 1] = array[j];
      array[j] = tmp;
    }
  }
}

void sortArray(char *array[], size_t sizeOfArray)
{
  insertionSort(array, sizeOfArray, builtinLargerThan);
}