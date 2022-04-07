/*
ELEC-351 Group E
Author: Jacob Howell
December-January 2022
*/

//constructor

//#include "..\..\Common\inc\cbuff.hpp"

template<class T>
Circular_Buff<T>::Circular_Buff (unsigned int size, char * buffName) : max_size(size), named(true), bufferName(buffName){    //initalise member attribute to size
    buffer = new T [size];                               //allocate memory for buffer
}
template<class T>
Circular_Buff<T>::Circular_Buff (unsigned int size) : max_size(size){    //initalise member attribute to size
    buffer = new T [size];                               //allocate memory for buffer
}
template<class T>
Circular_Buff<T>::Circular_Buff():max_size(128){                   //overloaded constructor gives default size of 128 samples
    buffer = new T [128];
    this->full++;
}
//destructor
template<class T>
Circular_Buff<T>::~Circular_Buff(){                                //destructor deletes new dynamicly allocated memory
    delete buffer;                                              //delete array from heap to avoid memory leaks
}
template<class T>
int Circular_Buff<T>::put(T sample){                       //add new elements to buffer
    if(isFull()){                                               //if the head is in the same position as the tail, the buffer is full
        //TODO Call error handler "Buffer overrun"
        if(hasName()){
        }else{}
        return -1;  //error buffer full
    }
    buffer[head] = sample;                                      //write sample to buffer array
    head = (head + 1) % max_size;                               //increment head by 1, and wrap around if greater than buffer size
    curr_size++;
    return 0;
}
template<class T>
T Circular_Buff<T>::peek(unsigned int index){               //allow external function to look inside buffer without changing it
    T sample;
    int i = (head - index);
    if (i < 0){
        i+=max_size;
    }
    sample = buffer[i];                          //return value of sample at specified index
    return sample;
}
template<class T>
T Circular_Buff<T>::get(){                                  //get last sample and remove it from buffer (move the tail)
    //mutex.lock();                                               //lock mutex in critical section
    while(isEmpty()){                                              //check if the buffer is empty before getting sample from buffer
        //TODO Call error handeler "Buffer underrun"
        //return <T>();                                      //return empty sample
    }
    T sample = buffer[tail];                             //if buffer is not empty, return sample from buffer's tail
    curr_size--;                                                //subtract 1 from current size
    tail = (tail + 1) % max_size;                               //increment tail (includes wrapping so pointer stays within arrays limits)
    return sample;                                              //return sample
}
template<class T>
void Circular_Buff<T>::reset(){                                    //move head and tail to same position and reset current size. (emptying buffer)
    head = tail;                                                //
    curr_size = 0;                                              //  
}
template<class T>
bool Circular_Buff<T>::isEmpty() const{                            //check if buffer is empty (for use external to the class)
    return (curr_size == 0);                                    //check size 
}
template<class T>
bool Circular_Buff<T>::isFull() const{                             //check if buffer is full (for use external to the class)
    return (curr_size >= max_size);     //check if full         //check current size to max size
}
template<class T>
unsigned int Circular_Buff<T>::getCapacity() const{                      //return capacity of buffer (for external use to the class) 
    return max_size;                                            //return size of buffer 
}
template<class T>
unsigned int Circular_Buff<T>::getSize() const{                          //return the current size of the buffer (for external use to the class)
    return curr_size;                               
}
template<class T>
char * Circular_Buff<T>::getName(){
    return bufferName;
}
template<class T>
bool Circular_Buff<T>::hasName(){
    return named;
}
