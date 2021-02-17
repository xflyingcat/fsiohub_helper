#include "Serial.h"
#include <stdio.h>

Serial::Serial()
{
    //ctor

    rx_call_back = NULL;
    rx_buff_size = DEFAULT_RX_BUFF_SIZE;
    rx_buff = (char*)malloc(rx_buff_size);
}

Serial::Serial(int rx_buf_sz)
{
    //ctor
    rx_call_back = NULL;
    rx_buff_size = rx_buf_sz;
    rx_buff = (char*)malloc(rx_buff_size);
}


Serial::Serial(int rx_buf_sz, void *cb)
{
    //ctor
    rx_call_back = (RX_CALLBACK)cb;
    rx_buff_size = rx_buf_sz;
    rx_buff = (char*)malloc(rx_buff_size);
}

Serial::~Serial()
{
    //dtor
    free(rx_buff);
}

static wchar_t *get_wc(char *c)
{
    const size_t sz = strlen(c)+1;
    wchar_t* wc = new wchar_t[sz];
    mbstowcs (wc, c, sz);
    return wc;
}


int Serial::Open(const char *dev_name, long baud_rate)
{

    DWORD symto;
    DCB dcb;
    COMMTIMEOUTS commtimeouts;
    char portname[32];
    sprintf(portname,"\\\\.\\%s",dev_name);
    //wchar_t *s = get_wc(portname);
    curr_baudrate = baud_rate;
    hcom = CreateFile( portname,
                       GENERIC_READ|GENERIC_WRITE,
                       FILE_SHARE_READ|FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,/*FILE_FLAG_OVERLAPPED,*/
                       NULL);



    if (hcom == INVALID_HANDLE_VALUE)
    {
        printf("Serial::Open(): %s could not be opened.\n", portname);
        return FALSE;
    }
    else
        printf("Serial::Open(): %s opened sexyfully handle=%d.\n", portname,hcom);


    dcb.DCBlength = sizeof(dcb);

    symto = 10 * 115200 / baud_rate;
    if(symto == 0)
        symto = 1;

    //printf("serial_open(): 0   symto = %ld\n",symto);

    commtimeouts.ReadIntervalTimeout = 200;
    commtimeouts.ReadTotalTimeoutMultiplier = 0;
    commtimeouts.ReadTotalTimeoutConstant = 100;

    commtimeouts.WriteTotalTimeoutMultiplier = 100;
    commtimeouts.WriteTotalTimeoutConstant = 200;
    if (GetCommState(hcom, &dcb))
    {
        //printf("serial_open(): 1\n");
        dcb.BaudRate = baud_rate;
        dcb.ByteSize = 8;
        dcb.StopBits = TWOSTOPBITS;
        dcb.Parity = NOPARITY;
        dcb.fParity = FALSE;
        dcb.fOutxCtsFlow = FALSE;
        dcb.fOutxDsrFlow = FALSE;
        dcb.fDtrControl = FALSE;
        dcb.fRtsControl = FALSE;
        dcb.fDsrSensitivity = FALSE;

        if (SetCommState(hcom, &dcb))
        {
            //printf("serial_open(): 2\n");
            SetCommState(hcom, &dcb);

            if (SetCommTimeouts(hcom, &commtimeouts))
            {
                flag = 1;
                rx_handle = CreateThread(NULL,
                                         2000,
                                         (LPTHREAD_START_ROUTINE)&ReadingThread,
                                         this,
                                         0,
                                         NULL);

                if(rx_handle != INVALID_HANDLE_VALUE)
                    printf("Serial::Open(): RX thread has been created (%d)\n",rx_handle);
                return TRUE;
            }
        }

    }
    return FALSE;
}

void Serial::Close(void)
{
    flag = 0;
    CloseHandle(hcom);
}

BOOL ReadFile_(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
)
{
    return 0;
}


DWORD WINAPI Serial::ReadingThread(LPVOID lpParameter)
{
    int res;
    DWORD len;
    printf("Serial::ReadingThread(): Started\n");
    DTR_off();
     Sleep(500);
    DTR_on();
     Sleep(500);
    while(this->flag)
    {
      #ifdef PARANOIC_TEST
        printf("R");
      #endif
        if(this->hcom != INVALID_HANDLE_VALUE)
            res = ReadFile(this->hcom,
                           this->rx_buff,
                           this->rx_buff_size,
                           &len,
                           NULL);
        if(!res)
        {
            this->flag = 0;
            break;
        }


        if(len)
        {
            if(this->rx_call_back)
                this->rx_call_back(this->rx_buff,len);
        }
        else
        {
            Sleep(10);
        }
      #ifdef PARANOIC_TEST
        printf("r");
      #endif

    }
    printf("Serial::ReadingThread(): Finished\n");
    return 0;
}

void Serial::SetCallback(void *ptr)
{
    rx_call_back = (RX_CALLBACK)ptr;
}



int Serial::SerialWrite(unsigned char *data, size_t size)
{
    DWORD n,res;
    unsigned char *p = data;

    while(size)
    {
        res = WriteFile(this->hcom, p, size, &n, NULL);
        size -= n;
        p += n;
        if(!res)
            return -1;
    }


    return n;
}


int Serial::SerialRead(unsigned char *buf, size_t size)
{
    DWORD cnt;
    if(ReadFile(this->hcom, buf, size, &cnt, NULL))
    {
        return cnt;
    }
    return -1;
}



void Serial::SerialFlush(void)
{
    unsigned char buf[10];
    DWORD cnt;
    COMMTIMEOUTS commtimeouts,prev_commtimeouts;
    GetCommTimeouts(hcom, &commtimeouts);
    GetCommTimeouts(hcom, &prev_commtimeouts);

    commtimeouts.ReadIntervalTimeout = 10;
    commtimeouts.ReadTotalTimeoutMultiplier = 1;
    commtimeouts.ReadTotalTimeoutConstant = 10;

    SetCommTimeouts(this->hcom, &commtimeouts);
    do
    {
        ReadFile(this->hcom, buf, 1, &cnt, NULL);

    }
    while(cnt>0);


    SetCommTimeouts(this->hcom, &prev_commtimeouts);


}


void Serial::SetTimeout(int ms)
{
    COMMTIMEOUTS commtimeouts;
    int symto = 50 * 115200 / curr_baudrate;
    if(symto == 0)
        symto = 1;
    GetCommTimeouts(hcom, &commtimeouts);
    commtimeouts.ReadIntervalTimeout = symto;
    commtimeouts.ReadTotalTimeoutMultiplier = 1;
    commtimeouts.ReadTotalTimeoutConstant = ms;
    SetCommTimeouts(hcom, &commtimeouts);
}



void Serial::DTR_on(void)
{

   int result = EscapeCommFunction(hcom,SETDTR);
   printf("DTR on result: %d\n",result);

}

void Serial::DTR_off(void)
{

    int result = EscapeCommFunction(hcom,CLRDTR);
    printf("DTR off result: %d\n",result);
}
