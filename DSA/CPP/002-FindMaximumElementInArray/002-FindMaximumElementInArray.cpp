#include "002-FindMaximumElementInArray.hpp"
int main()
{
    // ==================== VARIBLE DECLARTIONS ==================
    // Size of the Array
    int arraySize = 10;

    // Origianl Array for Operations
    int arr[arraySize] = { 9, 5, 3, 2, 5, 8, 5, 3, 6, 8};


    // ================ CODE =================
    // Function for Show Array elements
    showArray(arr, arraySize);

    // --------- Largest Element -------------
    // Function call for find largest element in Array
    findLargestNumebrInArray(arr, arraySize);


    return(EXIT_SUCCESS);
}

