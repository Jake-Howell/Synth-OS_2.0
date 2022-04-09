/*
ELEC-351 Group E
Author: Jacob Howell
December-January 2022
*/

#ifndef C_BUFF
#define C_BUFF
#include "stdlib.h"


template<class T>
class Circular_Buff{

    //public attributes and member functions can be seen and used by anywhere in the software
    public:
        
        //constructor
        Circular_Buff();                //default constructor (default size of 128 samples)
        Circular_Buff (unsigned int size);    //initalise member attribute to size
        Circular_Buff(unsigned int size, char * buffName);
        
        //destructor
        //~Circular_Buff();               //destructor (deletes dynamicly allocated memory)

        void     put(T sample);  				//add a new sample to the buffer
        T peek(unsigned int index);     //look inside buffer without changing it
        T get();                  			//get the oldest sample from the buffer 
        void    reset();                //clear the buffer
        bool    isEmpty() const;        //check if empty
        bool    isFull() const;         //check if full
        unsigned int  getCapacity() const;    //returns max capacity 
        unsigned int  getSize() const;        //returns current number of samples in buffer
				char * getName();
        bool    hasName();

    //private attributes and member functions that only this class can see and use
    private:
			const int max_size = 128;          //specify size of buffer
			T buffer[128];                  //store location of buffer when created by constructor
			volatile int curr_size;
			char * bufferName;
			bool named = false;
			
			volatile int head;                   //store the location of the first element in buffer
			volatile int tail;                   //store location of last element in buffer 
        
        
        
};



template<class T>
Circular_Buff<T>::Circular_Buff (unsigned int size, char * buffName) : max_size(size), named(true), bufferName(buffName){    //initalise member attribute to size
  //buffer = new T [size];                               //allocate memory for buffer
	reset();
}
template<class T>
Circular_Buff<T>::Circular_Buff (unsigned int size) : max_size(size){    //initalise member attribute to size
    //buffer = new T [size];                               //allocate memory for buffer
	reset();
}
template<class T>
Circular_Buff<T>::Circular_Buff():max_size(128){                   //overloaded constructor gives default size of 128 samples
    //buffer = new T [128];
	reset();
}
//destructor
//template<class T>
//Circular_Buff<T>::~Circular_Buff(){                                //destructor deletes new dynamicly allocated memory
//    delete buffer;                                              //delete array from heap to avoid memory leaks
//}
template<class T>
void Circular_Buff<T>::put(T sample){                       //add new elements to buffer
    if(isFull()){                                               //if the head is in the same position as the tail, the buffer is full
        //TODO Call error handler "Buffer overrun"
        if(hasName()){
        }else{}
    }
    buffer[head] = sample;                                      //write sample to buffer array
    head = (head + 1) % max_size;                               //increment head by 1, and wrap around if greater than buffer size
    curr_size++;
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
	head = 0;
	tail = 0;
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
#endif
