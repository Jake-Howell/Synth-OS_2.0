/*
ELEC-351 Group E
Author: Jacob Howell
December-January 2022
*/

#ifndef C_BUFF
#define C_BUFF
#include "stdlib.h"


//using namespace std;

template<class T>
class Circular_Buff{

    //public attributes and member functions can be seen and used by anywhere in the software
    public:
        
        //constructor
        Circular_Buff();                //default constructor (default size of 128 samples)
        Circular_Buff (unsigned int size);    //initalise member attribute to size
        Circular_Buff(unsigned int size, char * buffName);
        
        //destructor
        ~Circular_Buff();               //destructor (deletes dynamicly allocated memory)

        int     put(T sample);  //add a new sample to the buffer
        T peek(unsigned int index);     //look inside buffer without changing it
        T get();                  //get the oldest sample from the buffer 
        void    reset();                //clear the buffer
        bool    isEmpty() const;        //check if empty
        bool    isFull() const;         //check if full
        unsigned int  getCapacity() const;    //returns max capacity 
        unsigned int  getSize() const;        //returns current number of samples in buffer
				char * getName();
        bool    hasName();

    //private attributes and member functions that only this class can see and use
    private:

        char * bufferName;
        bool named = false;
        const int max_size;          //specify size of buffer
        volatile int head = 0;                   //store the location of the first element in buffer
        volatile int tail = 0;                   //store location of last element in buffer              //needs to be locked and volatile
        volatile int curr_size = 0;
        
        T* buffer;                  //store location of buffer when created by constructor
};

#include "..\..\Common\src\cbuff.tpp"

#endif
