#ifndef IO_INTERFACE_H_
#define IO_INTERFACE_H_
class IO{
    public:
    virtual int update_IO() = 0;
    virtual const char * getName() = 0;
};

#endif