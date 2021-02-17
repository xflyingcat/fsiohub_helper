#ifndef __LOGGER_H__
#define __LOGGER_H__


#ifdef __cplusplus
extern "C" {

int log_write(const char* str, ... );
extern char logfile_path[MAX_PATH];
void log_path_init(void);
void log_cleaner(void);
FILE *log_open(void);
void log_generator(void);

}
#endif




#endif

