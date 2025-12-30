// ================== FUNCTION DEFINATIONS ======================
#include "002-FindMaximumElementInArray.hpp"
// Show All Elements of array
void showArray(int *arr, int size)
{
    // Code
    std::cout << std::endl;
    std::cout << "All Array Elementes: ";
    for(int i = 0; i < size; i++)
    {
        std::cout << " " << arr[i];
    }
    std::cout << std::endl;
}


// Rverse Array
void reverseArray(int* arr, int size)
{
    // Variable declarations
    int temp;

    // Code
    for(int i = 0; i < size / 2; i++)
    {
        temp = arr[i];
        arr[i] = arr[size -1 - i];
        arr[size -1 - i] = temp;
    }

    // Show Array Elements
    showArray(arr, size);
}


// Find largest element in array 
void findLargestNumebrInArray(int *arr, int size)
{
    // Variable declartions
    int largestnumber = arr[0];
    
    // Code
    std::cout << std::endl << "Largest Number in Array: ";
    for(int i = 1; i < size; i++)
    {
        if(largestnumber < arr[i])
        {
            largestnumber = arr[i];
        }
    }

    std::cout << largestnumber << std::endl; 
}

// Array is Sorted
void checkArrayIsSorted(int* arr, int size)
{
    bool flagshort = false;
    int count = 0;
    for(int i = 0; i < size - 1 ; i++)
    {
        if(arr[i] > arr[i + 1])
        {
            flagshort = true;
            count++;
            
            if(count == 1)
            {
                std::cout << "Array is not shorted" << std::endl;
            }
        }
    }
}