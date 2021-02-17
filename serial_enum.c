//---------------------------------------------------------------------------
/*
This enumerator is 99.9% based on this material:
http://www.codeproject.com/KB/system/serial_portsenum_fifo.aspx
By Vladimir Afanasyev

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <setupapi.h>



#define MAX_NAME_PORTS 7
#define RegDisposition_OpenExisting (0x00000001)   // open key only if exists
#define CM_REGISTRY_HARDWARE        (0x00000000)


static int main_result = 0;
static int scan_flag = 0;
static int web_flag = 0;
static int detect_flag = 0;
static char file_name[MAX_PATH];
static FILE *output;
HANDLE serial_handle;

typedef DWORD WINAPI
(* CM_Open_DevNode_Key)(DWORD, DWORD, DWORD, DWORD, PHKEY, DWORD);



HKEY GetDeviceKey(LPTSTR portName)
{
    static HINSTANCE CfgMan;
    static CM_Open_DevNode_Key OpenDevNodeKey=NULL;
    int numDev=0;
    int res1;
    BOOL guidTest=FALSE;
    DWORD RequiredSize=0;
    char* buf;
    HDEVINFO DeviceInfoSet;
    SP_DEVINFO_DATA DeviceInfoData= {0};
    unsigned char DevName[MAX_NAME_PORTS]= {0};

    guidTest=SetupDiClassGuidsFromNameA(
                 "Ports",0,0,&RequiredSize);
    if(RequiredSize < 1)
        return (HKEY)-1;

    buf=(char *)malloc(RequiredSize*sizeof(GUID));

    guidTest=SetupDiClassGuidsFromNameA(
                 "Ports",(GUID *)buf,RequiredSize*sizeof(GUID),&RequiredSize);

    if(!guidTest)return (HKEY)-1;


    DeviceInfoSet=SetupDiGetClassDevs(
                      (GUID *)buf,NULL,NULL,DIGCF_PRESENT);
    if(DeviceInfoSet == INVALID_HANDLE_VALUE)
        return (HKEY)-1;

    free(buf);

    if(!OpenDevNodeKey)
    {
        CfgMan=LoadLibrary("cfgmgr32");
        if(!CfgMan)return (HKEY)-1;
        OpenDevNodeKey=(CM_Open_DevNode_Key)GetProcAddress(CfgMan,
                       "CM_Open_DevNode_Key");
        if(!OpenDevNodeKey)
        {
            FreeLibrary(CfgMan);
            return (HKEY)-1;
        }
    }

    DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);

    while(TRUE)
    {
        HKEY KeyDevice;
        DWORD len,len2;
        res1=SetupDiEnumDeviceInfo(
                 DeviceInfoSet,numDev,&DeviceInfoData);

        if(!res1)
        {
            SetupDiDestroyDeviceInfoList(DeviceInfoSet);
            FreeLibrary(CfgMan);
            OpenDevNodeKey=NULL;
            return (HKEY)-1;
        }

        res1=OpenDevNodeKey(DeviceInfoData.DevInst,KEY_QUERY_VALUE | KEY_WRITE,0,
                            RegDisposition_OpenExisting,&KeyDevice,CM_REGISTRY_HARDWARE);
        if(res1 != ERROR_SUCCESS)
        {
            SetupDiDestroyDeviceInfoList(DeviceInfoSet);
            FreeLibrary(CfgMan);
            OpenDevNodeKey=NULL;
            return (HKEY)-1;
        }
        len=MAX_NAME_PORTS;

        res1=RegQueryValueEx(
                 KeyDevice,	// handle of key to query
                 "portname",	// address of name of value to query
                 NULL,	// reserved
                 NULL,	// address of buffer for value type
                 DevName,	// address of data buffer
                 &len 	// address of data buffer size
             );
        if(res1 != ERROR_SUCCESS)
        {
            FreeLibrary(CfgMan);
            OpenDevNodeKey=NULL;
            return (HKEY)-1;
        }
        if(!strcasecmp((char*)DevName,(char*)portName))
        {
            FreeLibrary(CfgMan);
            OpenDevNodeKey=NULL;
            return KeyDevice;
        }
        RegCloseKey(KeyDevice);
        numDev++;
    }

}


HANDLE  BeginEnumeratePorts(VOID)
{
    BOOL guidTest=FALSE;
    DWORD RequiredSize=0;
    int j;
    HDEVINFO DeviceInfoSet;
    char* buf;

    guidTest=SetupDiClassGuidsFromNameA(
                 "Ports",0,0,&RequiredSize);
    if(RequiredSize < 1)return (HANDLE)(-1);

    buf=(char *)malloc(RequiredSize*sizeof(GUID));

    guidTest=SetupDiClassGuidsFromNameA(
                 "Ports",(GUID *)buf,RequiredSize*sizeof(GUID),&RequiredSize);

    if(!guidTest)return (HANDLE)(-1);


    DeviceInfoSet=SetupDiGetClassDevs(
                      (GUID *)buf,NULL,NULL,DIGCF_PRESENT);
    if(DeviceInfoSet == INVALID_HANDLE_VALUE)return (HANDLE)-1;

    free(buf);

    return DeviceInfoSet;
}

char FriendlyName[MAX_PATH]= {0};

BOOL EnumeratePortsNext(HANDLE DeviceInfoSet, LPTSTR lpBuffer)
{
    static CM_Open_DevNode_Key OpenDevNodeKey=NULL;
    static HINSTANCE CfgMan;

    int res1,res2;
    unsigned char DevName[MAX_NAME_PORTS]= {0};

    static int numDev=0;
    int numport;

    if(DeviceInfoSet == NULL && lpBuffer == NULL)
    {
        numDev = 0;
        return FALSE;
    }

    SP_DEVINFO_DATA DeviceInfoData= {0};
    DeviceInfoData.cbSize=sizeof(SP_DEVINFO_DATA);


    if(!DeviceInfoSet || !lpBuffer)return -1;
    if(!OpenDevNodeKey)
    {
        CfgMan=LoadLibrary("cfgmgr32");
        if(!CfgMan)return FALSE;
        OpenDevNodeKey=(CM_Open_DevNode_Key)GetProcAddress(CfgMan,"CM_Open_DevNode_Key");
        if(!OpenDevNodeKey)
        {
            FreeLibrary(CfgMan);
            return FALSE;
        }
    }

    while(TRUE)
    {

        HKEY KeyDevice;
        DWORD len,len2;
        res1=SetupDiEnumDeviceInfo(
                 DeviceInfoSet,numDev,&DeviceInfoData);

        if(!res1)
        {
            SetupDiDestroyDeviceInfoList(DeviceInfoSet);
            FreeLibrary(CfgMan);
            OpenDevNodeKey=NULL;
            return FALSE;
        }


        res1=OpenDevNodeKey(DeviceInfoData.DevInst,KEY_QUERY_VALUE,0,
                            RegDisposition_OpenExisting,&KeyDevice,CM_REGISTRY_HARDWARE);
        if(res1 != ERROR_SUCCESS)
            return NULL;
        len=MAX_NAME_PORTS;

        res1=RegQueryValueEx(
                 KeyDevice,	// handle of key to query
                 "portname",	// address of name of value to query
                 NULL,	// reserved
                 NULL,	// address of buffer for value type
                 DevName,	// address of data buffer
                 &len 	// address of data buffer size
             );


        RegCloseKey(KeyDevice);
        if(res1 != ERROR_SUCCESS)return NULL;
        numDev++;
        if(memicmp(DevName, "com", 3))continue;
        numport=atoi(DevName+3);
        if(numport > 0)
        {
            strcpy(lpBuffer,DevName);
            return TRUE;
        }

        FreeLibrary(CfgMan);
        OpenDevNodeKey=NULL;
        return FALSE;
    }
}

//---------------------------------------------------------------------------

BOOL  EndEnumeratePorts(HANDLE DeviceInfoSet)
{
    if(SetupDiDestroyDeviceInfoList(DeviceInfoSet))
        return TRUE;
    printf("Error:%d\n",GetLastError());
    return FALSE;
}



char *filename_extract(char *full_name)
{
    int len = strlen(full_name);
    int i;
    for(i=len-1; i>=0; i--)
    {
        if(full_name[i] == '/' || full_name[i] == '\\')
            return &full_name[i+1];
    }

    return full_name;
}


int get_port_list(char *ports_pull)
{
    char port[20] = "",*p = ports_pull;
    HANDLE res;
    int cnt = 0;
    EnumeratePortsNext(NULL, NULL);
    res=BeginEnumeratePorts();
    while(TRUE)
    {
        if(!EnumeratePortsNext(res,port))
            break;
        cnt++;
        p += sprintf(p,"%s\n",port);
    }


    // EndEnumeratePorts(res);



    return cnt;
}





