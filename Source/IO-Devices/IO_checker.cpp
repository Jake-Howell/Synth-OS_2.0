#include "IO-Devices.h"

IO_Checker::IO_Checker(){
    IO_count = 0;  //initalise IO_count to 0
    sleepTime = 50ms;
}
void IO_Checker::add_to_IO_check_list(IO * device){
    const char * device_name = device->getName();
    if(IO_count < MAX_IO){
        IO_checkList[IO_count] = device;   //add function to IO check list
        IO_count++;
        PrintQueue.call(printf,"Added %s to IO check list\r\n", device_name);
    }else{
        PrintQueue.call(printf,"No space left on IO check list for %s\r\n", device_name);
    }
}
void IO_Checker::setUpdateRate(milliseconds ms){
    this->sleepTime = ms;
}
milliseconds IO_Checker::getSleepTime(){
    return sleepTime;
}
int IO_Checker::run_IOchecks(){
    int err = 0;
    for (int i = 0; i < IO_count; i++){
        err = IO_checkList[i]->update_IO();
        if (err != 0){
            PrintQueue.call(printf, "IO Check Error: %d\r\n", err); //report error to user
        }
    }
    //PrintQueue.call(printf, "Checked %d Devices\r\n", IO_count);
    ThisThread::sleep_for(sleepTime);
    return 0;
}
