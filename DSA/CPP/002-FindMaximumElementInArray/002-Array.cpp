// =================== HEADER FILES ======================
#include "002-Array.hpp"

// =================== MAIN FUNCTION (ENTRY POINT FUNCTION ======================
int main()
{
    // ==================== VARIBLE DECLARTIONS ==================
    // Size of the Array
    int arraySize = 20;

    // Origianl Array for Operations
    int arr[arraySize] = { 1, 9, 2, 5, 0, 13, 2, 5, 0, 8, 5, 3, 6, 8, 54, 12, 45, 78, 23, 67};


    // ================ CODE =================
    // Function for Show Array elements
    showArray(arr, arraySize);

    // --------- Largest Element -------------
    // Function call for find largest element in Array
    // findLargestNumebrInArray(arr, arraySize);

    // --------- Reverse Array ----------------
    // reverseArray(arr, arraySize);

    // --------- Array is Shorted? ------------
    // checkArrayIsSorted(arr, arraySize);

    // Find the second largest element
    // findTheSecondLargestNumber(arr, arraySize);

    // Move all the zeros to the end 
    // moveAllZerosToTheEnd(arr, arraySize);

    // Move Array by value k
    rotateArrayByK(arr, arraySize, 3);



    // Exit With Success
    return(EXIT_SUCCESS);
}

