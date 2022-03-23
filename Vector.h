#ifndef _VECTOR_H_
#define _VECTOR_H_
using namespace std;

// Version
// 02 - Odilia 2021-10-19
//      Creation of my own Vector class
template <typename T>
class Vector{
    T* arr; // Array to store values
    int totalCapacity; // Total storage capacity
    int currentCapacity; // Current length of vector

public:
    Vector(){
        arr = new T[1];
        totalCapacity = 1;
        currentCapacity = 0;
    }

    // Adds a new value at the end of the arr
    void pushBack(T data){
        if (currentCapacity == totalCapacity){ // The array is full, need to increase storage capacity
            T* temp = new T[totalCapacity * 2]; // Increase the capacity by 2x

            // copying old array elements to new arr
            for (int i = 0; i < totalCapacity; i++){
                temp[i] = arr[i];
            }

            // delete the previous arr
            delete[] arr;
            totalCapacity++; // Increase the indicated total capacity by 1
            arr = temp;
        }

        // Insert data
        arr[currentCapacity] = data;
        currentCapacity++; // Add the current capacity by 1
    }

    // Adds a new value at a specific index
    void push(T data, int index){
        int tempCapacity = totalCapacity;

        if (currentCapacity == totalCapacity){ // The array is full, need to increase storage capacity
            tempCapacity = totalCapacity * 2; // Increase array size by 2x
        }
        T* temp = new T[tempCapacity];

        // copying old array elements to new arr
        for (int i = 0; i < tempCapacity; i++){
            if (i == index){ // Insert data at the specified index
                temp[i] = data;
            }
            else if (i > index) { // Push the rest of the data behind by 1

                temp[i] = arr[i-1];
            }
            else {
                temp[i] = arr[i];
            }
        }

        // delete the previous arr
        delete[] arr;
        currentCapacity++;
        totalCapacity++; // Increase the indicated total capacity by 1
        arr = temp;
    }

    // Delete last element of vector
    void popBack(){
        currentCapacity--; // Ignore the last element in arr, will be replaced if push new data
    }

    // Delete specific element of vector
    void pop(int index){
        T* temp = new T[totalCapacity-1]; // New array has the size of 1 less
        for (int i=0;i<currentCapacity;i++){
            // Copy old elements to new array, as long as it is not the element that is being removed
            if(i != index){
                if (i > index){
                    temp[i-1] = arr[i];
                }
                else{
                   temp[i] = arr[i];
                }
            }
        }
        delete[] arr; // Delete current vector array and replace with new one
        arr = temp;
        currentCapacity--; // Decrease the current capacity by 1
        totalCapacity--; // Decrease the total capacity by 1
    }

    // Return the current length of the array
    int length(){
        return this->currentCapacity;
    }

    // Return the value of specific index
    T get(int index){
        return arr[index];
    }

    // Return the index of a given value
    int getIndex(T value){
        for (int i=0; i<currentCapacity; i++){
            if (arr[i] == value){
                return i; // return value of i if value found
            }
        }
        return -1;
    }

    // Display all array elements
    void display(){
        for (int i=0; i<currentCapacity; i++){
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};
#endif // _VECTOR_H_
