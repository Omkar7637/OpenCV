#include<iostream>

int main()
{
    int arr[10] = { 9, 5, 3, 2, 5, 8, 5, 3, 6, 8};

    std::cout << "\n\n";

    std::cout << "Array:";
    for(int i = 0; i < 10; i++)
    {
        std::cout << " " << arr[i];
    }

    std::cout << "\n";

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

