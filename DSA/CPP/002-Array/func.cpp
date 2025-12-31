// ================== FUNCTION DEFINATIONS ======================
#include "002-Array.hpp"
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
bool checkArrayIsSorted(int* arr, int size)
{
    // Code
    for(int i = 0; i < size - 1 ; i++)
    {
        if(arr[i] > arr[i + 1])
        {
            std::cout << "Array is not shorted" << std::endl;
            return false;
        }
    }

    std::cout << "Array is sorted!!!" << std::endl;
    return true;
}

void findTheSecondLargestNumber(int* arr,int size)
{
    // Varibel declarations
    int secondLargestElement = 0;
    secondLargestElement = INT_MIN;
    int firstLargestElement = INT_MIN;

    // Code
    // Check the array is empty
    if(size < 0)
    {
        std::cout << "Second largest element dows not existes!!" << std::endl;
        return;
    }
    // Second largest elemnt in array
    for(int i = 0; i < size; i++)
    {
        if(arr[i] > firstLargestElement)
        {
            secondLargestElement = firstLargestElement;
            firstLargestElement = arr[i];
        }
        else if(arr[i] < firstLargestElement && arr[i] > secondLargestElement)
        {
            secondLargestElement = arr[i];
        }
    }

    // Show Output
    if(secondLargestElement == INT_MIN)
    {
        std::cout << "Second largest element does not exists!!!" << std::endl;
    }
    else
    {
        std::cout << "Second Largest Element: " << secondLargestElement << std::endl;
    }
}

void moveAllZerosToTheEnd(int* arr, int size)
{
    // Variable declartions
    // Code
    for(int i = 0, j = 0; i < size; i++)
    {
        if(arr[i] != 0)
        {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            j++;
        }
    }
    showArray(arr, size);
}

void rotateArrayByK(int* arr, int size, int k)
{

    while(k > 0)
    {
        int firstElement = arr[0];
    
        for(int i = 0; i < size; i++)
        {
            arr[i] = arr[i + 1];
        }
        arr[size - 1] = firstElement;
        k--;
    }

    showArray(arr, size);
}