#ifndef FUNC_HPP
#define FUNC_HPP

// ================ HEADER FILES =======================
#include<iostream> // For basic input output 
#include<climits>

// -------------- Function Declarations ---------------
// Largest Element
void findLargestNumebrInArray(int *arr, int size);
// Show Array elements
void showArray(int *arr, int size);
// Reverse Array
void reverseArray(int* arr, int size); 
// Array is sorted?
bool checkArrayIsSorted(int* arr, int size);
// Find second largest element
void findTheSecondLargestNumber(int* arr,int size);
// Move all zeros to the end
void moveAllZerosToTheEnd(int* arr, int size);
// Rotate Array by K value
void rotateArrayByK(int* arr, int size, int k);

#endif