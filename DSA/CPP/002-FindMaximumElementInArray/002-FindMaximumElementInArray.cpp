#include "002-FindMaximumElementInArray.hpp"
int main()
{
    int arraySize = 10;
    int arr[arraySize] = { 9, 5, 3, 2, 5, 8, 5, 3, 6, 8};

    showArray(arr, arraySize);

    int largenumber = arr[0];

    for(int i = 1; i < 10; i++)
    {
        if(largenumber < arr[i])
        {
            largenumber = arr[i];
        }
    }

    std::cout << "Larges elemnet in Array: " << largenumber;


    std::cout << "\n\n";


    return(EXIT_SUCCESS);
}

