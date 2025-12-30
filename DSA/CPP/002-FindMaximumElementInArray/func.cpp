// ================== FUNCTION DEFINATIONS ======================

// Rverse Array
void reverseArray(int* arr, int size)
{
    int 
}


// Find largest element in array 
void findLargestNumebrInArray(int *arr, int size)
{
    // Variable declartions
    int largestnumber = arr[0];
    
    // Code
    std::cout << std::endl << "Largest Number in Array:";
    for(int i = 1; i < size; i++)
    {
        if(largestnumber < arr[i])
        {
            largestnumber = arr[i];
        }
    }

    std::cout << largestnumber << std::endl; 
}

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
