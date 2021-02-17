#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
//#include <setupapi.h>
#include "time.h"

FILE *log_open(void);


typedef struct CONFIG_ {

  char mode_log[MAX_PATH];
  char mode_console_log[MAX_PATH];
  char log_path[MAX_PATH];
  char logs_keeping_period[MAX_PATH];
} CONFIG;

CONFIG config;

int log_write(const char* str, ... )
{

struct tm *tm_tmp;
time_t lt;
va_list arp;
FILE* fp;
char datetime[40];
  lt = time(NULL);
  tm_tmp = localtime(&lt);
  strftime(datetime, 31,"%H:%M:%S ",tm_tmp);

if(0==strcmp(config.mode_log,"on") || 0==strcmp(config.mode_log,"ON"))
{
  if((fp = log_open()))
  {
   fprintf(fp,datetime);
   va_start(arp, str);
     vfprintf(fp,str,arp);
   va_end(arp);
   fprintf(fp,"\n");
   //fp = log_rotation(fp);
   fclose(fp);
  }
}

if(0==strcmp(config.mode_console_log,"on") || 0==strcmp(config.mode_console_log,"ON"))
{
    printf(datetime);
    va_start(arp, str);
      vprintf(str,arp);
    va_end(arp);
    printf("\n");
}
return 0;
}


char *strip_quotes(char *quoted_line)
{
char *stripped_line = strchr(quoted_line, '\"');

if(stripped_line)
{
char *tmp = strchr(++stripped_line, '\"');
     if(tmp)
        *tmp = 0x00;
return stripped_line;
}

return quoted_line;
}

static const char logfile_path[] = "";

void log_path_init(void)
{

  strcpy(config.mode_log,"ON");
  strcpy(config.mode_console_log,"ON");
  strcpy(config.log_path,"");
  strcpy(config.logs_keeping_period,"365");

int len = strlen(config.log_path);
if(len>2)
{
char *stmp = strip_quotes(config.log_path);

  sprintf(config.log_path,"%s",stmp);
} else
   strcpy(config.log_path, logfile_path);

 printf("LOG PATH: %s\n",config.log_path);


 //log_generator();
 //exit(0);

}

int delete_if_file_existing(char *fname)
{
char cmd[MAX_PATH+1];
FILE *fp = fopen(fname,"rb");
  if(fp)
  {
    fclose(fp);
    sprintf(cmd,"del \"%s\"",fname);
    system(cmd);
    return 1;

  }
return 0;
}


void log_cleaner(void)
{
struct tm *tm_tmp;
time_t oldest_time,tmp;
int lkp = atoi(config.logs_keeping_period);

if(lkp <  1)
     lkp = 181;
if(lkp > 730)
     lkp = 181;

time_t logs_keeping_period = 86400*lkp;
oldest_time = time(NULL) - logs_keeping_period;
char name[MAX_PATH+1];
char datetime[40];
int i,j;
for(i=0,j=0;i<730  ;i++)
{
  tmp = oldest_time - i * 86400;
  tm_tmp = localtime(&tmp);
  strftime(datetime, 31,"%Y-%m-%d",tm_tmp);
  sprintf(name,"%slog%s.txt",config.log_path,datetime);
  if(delete_if_file_existing(name))
  {
    j++;
  }
}
printf("%d files deleted\n",j);
}


FILE *log_open(void)
{
struct tm *tm_tmp;
time_t tmp;
char name[MAX_PATH+1];
char datetime[40];
FILE *fp;
  tmp = time(NULL);
  tm_tmp = localtime(&tmp);
  strftime(datetime, 31,"%Y-%m-%d",tm_tmp);
  sprintf(name,"%slog%s.txt",config.log_path,datetime);
  if((fp = fopen(name,"r")))
  {
     fclose(fp);
     return fopen(name,"a");
  }

log_cleaner();
return fopen(name,"a");
}


void log_generator(void)
{
struct tm *tm_tmp;
time_t oldest_time,tmp;

time_t logs_keeping_period = 86400;
oldest_time = time(NULL) - logs_keeping_period;
char name[MAX_PATH+1];
char datetime[40];
int i,j;
for(i=0,j=0;i<1460  ;i++)
{
  tmp = oldest_time - i * 86400;
  tm_tmp = localtime(&tmp);
  strftime(datetime, 31,"%Y-%m-%d",tm_tmp);
  sprintf(name,"%slog%s.txt",config.log_path,datetime);
  printf("%s\n",name);
  FILE *fp = fopen(name,"w");
  fprintf(fp,"Qu-qu, Grynia!\n");
  fclose(fp);
  {
    j++;
  }
}
printf("%d files created\n",j);
}


