// =================== HEADER FILES ======================
#include "002-FindMaximumElementInArray.hpp"

// =================== MAIN FUNCTION (ENTRY POINT FUNCTION ======================
int main()
{
    // ==================== VARIBLE DECLARTIONS ==================
    // Size of the Array
    int arraySize = 20;

    // Origianl Array for Operations
    int arr[arraySize] = { 9, 5, 13, 2, 5, 8, 5, 3, 6, 8};


    // ================ CODE =================
    // Function for Show Array elements
    showArray(arr, arraySize);

    // --------- Largest Element -------------
    // Function call for find largest element in Array
    findLargestNumebrInArray(arr, arraySize);

    // --------- Reverse Array ----------------
    // reverseArray(arr, arraySize);

    // --------- Array is Shorted? ------------
    checkArrayIsSorted(arr, arraySize);

    // Find the second largest element
    findTheSecondLargestNumber(arr, arraySize);

    // Move all the zeros to the end 



    // Exit With Success
    return(EXIT_SUCCESS);
}

