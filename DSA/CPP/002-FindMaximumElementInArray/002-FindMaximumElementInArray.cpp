#include "002-FindMaximumElementInArray.hpp"
int main()
{
    int arraySize = 10;
    int arr[arraySize] = { 9, 5, 3, 2, 5, 8, 5, 3, 6, 8};

    showArray(arr, arraySize);

    findLargestNumebrInArray(arr, arraySize);


    return(EXIT_SUCCESS);
}

