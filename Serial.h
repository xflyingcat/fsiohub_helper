#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <windows.h>

#define DEFAULT_RX_BUFF_SIZE 1000

//#define PARANOIC_TEST

typedef int (*RX_CALLBACK)(char *buf, int len);

class Serial
{
    HANDLE hcom;
    int flag;
    char *rx_buff;
    int rx_buff_size;
    HANDLE rx_handle;
    RX_CALLBACK rx_call_back;
    long curr_baudrate;
    DWORD WINAPI ReadingThread(LPVOID lpParameter);

public:
    Serial();
    Serial(int rx_buf_size);
    Serial(int rx_buf_sz, void *cb);
    virtual ~Serial();
    int Open(const char *dev_name, long baud_rate);
    void Close(void);
    void SetCallback(void *ptr);
    int SerialWrite(unsigned char *data, size_t size);
    int SerialRead(unsigned char *buf, size_t size);
    void SerialFlush(void);
    void SetTimeout(int ms);
    void DTR_on(void);
    void DTR_off(void);
protected:

private:
};

#endif // __SERIAL_H__
