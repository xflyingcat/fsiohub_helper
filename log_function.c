
/********************************************
       LOGGER DATA TYPES AND VARIABLES
*********************************************/
typedef struct {
    int limit_sw1;
    int limit_sw2;
    int limit_sw3;
    int hours;
    int min;
    int sec;
} MASTER_DATA;

typedef struct {
    int limit_sw1;
    int limit_sw2;
    int limit_sw3;
    float acc_x;
    float acc_y;
    float acc_z;
    float batt_voltage;
} CASSETTE_DATA;

#define CASSETTES_MAX  4

#ifdef WIN32
// WINDOWS
#define send_log_data(x,y) do {serial_write(buf, strlen(buf)); printf(buf);} while(0)
#else
// ARDUINO
#define send_log_data(x,y)  Serial.print(x)
#endif

CASSETTE_DATA cd[CASSETTES_MAX];
MASTER_DATA md[1];

/********************************************
       LOGGER MAIN FUNCTION
void log_send(MASTER_DATA *md, int cass_ndx, CASSETTE_DATA *cd);
Usage example:
          int cas_ndx;

          // these structures are defined as the global variables
          // CASSETTE_DATA cd[CASSETTES_MAX];
          // MASTER_DATA md[1];

          // filling master data
          md[0].hours = 7;
          md[0].min = 40;
          md[0].sec = 0;
          md[0].limit_sw1 = 1;
          md[0].limit_sw2 = 0;
          md[0].limit_sw3 = 1;

          // setting cassette index
          cas_ndx = 0;

          // filling cassette data indexed as cas_ndx
          cd[cas_ndx].limit_sw1 = 0;
          cd[cas_ndx].limit_sw2 = 1;
          cd[cas_ndx].limit_sw3 = 0;

          cd[cas_ndx].acc_x = 1.23;
          cd[cas_ndx].acc_y = 2.34;
          cd[cas_ndx].acc_z = 3.45;
          cd[cas_ndx].batt_voltage = 3.45;

          log_send(md, cas_ndx, cd);

*********************************************/

void log_send(MASTER_DATA *md, int cass_ndx, CASSETTE_DATA *cd)
{
    char buf[80];
    sprintf(buf,"MASTER LSW:%d%d%d, TIME:%02d:%02d:%02d\n",
            md->limit_sw1,
            md->limit_sw1,
            md->limit_sw2,
            md->hours,
            md->min,
            md->sec
            );

    send_log_data(buf, strlen(buf));

    sprintf(buf,"CASS:%d LSW:%d%d%d\n",
            cass_ndx,
            cd[cass_ndx].limit_sw1,
            cd[cass_ndx].limit_sw2,
            cd[cass_ndx].limit_sw3
            );
    send_log_data(buf, strlen(buf));

    sprintf(buf,"ACCX=%5.2f,ACCY=%5.2f,ACCZ=%5.2f, BATT:%4.2fV,\n",
            cd[cass_ndx].acc_x,
            cd[cass_ndx].acc_y,
            cd[cass_ndx].acc_z,
            cd[cass_ndx].batt_voltage);

    send_log_data(buf, strlen(buf));

}
