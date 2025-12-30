void findLargestNumebrInArray(int *arr, int size)
{
    int largestnumber = arr[0];
    
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

void showArray(int *arr, int size)
{
    std::cout << std::endl;
    std::cout << "All Array Elementes:";
    for(int i = 0; i < size; i++)
    {
        std::cout << " " << arr[i];
    }
    std::cout << std::endl;
}