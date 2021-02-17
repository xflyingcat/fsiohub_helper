
/*-------------------------------------------------------------
W A R N I N G !
Put into the file
Arduino/hardware/arduino/avr/cores/arduino/HardwareSerial.h
after lines:

#ifndef HardwareSerial_h
#define HardwareSerial_h

these lines to increase the serial buffer size
     #define SERIAL_TX_BUFFER_SIZE 512
     #define SERIAL_RX_BUFFER_SIZE 1024
---------------------------------------------------------------*/

#define DEVICE_ID  2
#define HALF_CYCLE_ENCODER
#if SERIAL_TX_BUFFER_SIZE != 512
// #error SERIAL_TX_BUFFER_SIZE still is not 512 bytes
#endif

#if SERIAL_RX_BUFFER_SIZE != 1024
// #error SERIAL_RX_BUFFER_SIZE still is not 1024 bytes
#endif

#define BAUD_RATE 115200
#define TIMING_TEST   0

void serial_read();
/****************** System ******************/
void setup(void);
void loop(void);
void serialEvent();

int stepper_driver(int new_pos);

/************ Indication buffer ***************/
char seven_segments_leds[16][8];
  /* leds state from simulator (all of 64)*/
char single_leds[64];
/****************** LEDs ******************/
#ifdef USING LAPMS
void leds_init(void);
void led_control(int led, char state);
#define LEDS_MAX   0
//const char leds[] = 
//{
//  14
// //   53,51,49,47,45,43,41,39,37
//};
#endif


/******** Main discrete input task ********/
#define BITS_MAXIMUM   3
void bits_init(void);
void discrete_task( void);

#ifdef USING_KEY_MATRIX
/******** Keys matrix ********/
void keys_matrix_init(void);
int  keys_scanner(void);
const char scan_table_x[] = {1, 2, 3, 4, 5, 6, 7, 8};
const char scan_table_y[] = {0, 9, 10, 16, A0, A1,A2, A3};
#endif

/****************** Utilities ******************/
const char hex_table[] = "0123456789ABCDEF";
char *form_put_event(char *buf, unsigned char id, unsigned short val)
{
    buf[0] = '{';
    
    buf[1] = 'A';
    
    buf[2] = hex_table[id >> 4];
    buf[3] = hex_table[id & 0x0F];
    
    buf[7] = hex_table[val & 0x0F];
    val >>= 4;
    buf[6] = hex_table[val & 0x0F];
    val >>= 4;
    buf[5] = hex_table[val & 0x0F];
    val >>= 4;
    buf[4] = hex_table[val & 0x0F];
    
    buf[8] = '}';
    buf[9] = 0;
    return buf;
}

/******** Serial communication ********/
#define STX           '{'
#define ETX           '}'
#define RBUF_LEN       162 
char rbuf[RBUF_LEN];
int  rcnt;

char line[41];
char status_line[41];
void setup() 
{
    //pinMode(14, OUTPUT); 
    //pinMode(15, OUTPUT); 
    //digitalWrite(14, LOW);
    //digitalWrite(15, LOW);
    memset(seven_segments_leds,' ',sizeof(seven_segments_leds));
    memset(single_leds,'0',sizeof(single_leds));
    //leds_init();  
    //keys_matrix_init();
    bits_init();
    Serial.begin(BAUD_RATE);
}  

#define AVERAGE_FACTOR  1000
long filter = 0;
int filter_cnt = 0;
int prev_value = 0;
char drefput_txbuf[12];

void loop() 
{
  int cur_value;
#ifdef LEONARDO  
  serial_read();
#endif  
  discrete_task();

  filter += analogRead(A1);
  if(++filter_cnt == AVERAGE_FACTOR)
  {
     filter_cnt = 0;
  } else
     return;
       
    cur_value = filter / AVERAGE_FACTOR;
    filter = 0;
    if(cur_value != prev_value)
    {
      prev_value = cur_value;
      form_put_event(drefput_txbuf, 0x80, cur_value);
      Serial.println(drefput_txbuf);  
    }
}


#ifdef LEONARDO
void serial_read() 
#else
void serialEvent()  
#endif
{
    int ndx,i,itmp;  
    while (Serial.available() > 0) 
    {
        char ch = (char)Serial.read();
        switch(ch)
        {
            case STX:
                rcnt = 0;
            break;

            case ETX:
                rbuf[rcnt] = 0x00;
        
                if(rbuf[0] == '7')
                {
                    memcpy(seven_segments_leds,rbuf+3,128);
                }    
                if(rbuf[0] == 'L')   
                    memcpy(single_leds,rbuf+3,64);

                if(rbuf[0] == 'D')
                {
                   Serial.write('!');
                   Serial.write(STX);
                   Serial.write('F');
                   itmp = DEVICE_ID;
                   Serial.write(hex_table[itmp >> 4]);
                   Serial.write(hex_table[itmp & 0x0F]);
                   Serial.write(ETX);
                } 
                else
                    Serial.write('!');

            break;

            default:
                if(rcnt<RBUF_LEN)
                    rbuf[rcnt++] = ch;
        }
    }
} 




/*****************************************
           Discrete inputs
*****************************************/
#define FLAG_INVERTED         (1<<0)
#define FLAG_ENABLED          (1<<1)
#define FLAG_HALF_CYCLE_EC    (1<<2)

enum {
   TOGGLE_SWITCH_0 = 0,
   TOGGLE_SWITCH_1,
   ITEMS_MAX
};

enum {
  PUSH_BUTTON = 0,
  PUSH_BUTTON_X,
  TOGGLE_SWITCH,
  ROTARY_SWITCH,
  ROTARY_SWITCH_X,
  ENCODER_A,
  ENCODER_B,
  SHIFT
};


typedef struct {
char counter;
char signal;
char  max;
short critical_time;
short critical_timer;
char evcode;
char flag;
char mode;
char pin;
char id;
} BITS_DESCRIPTION;

BITS_DESCRIPTION  bits[BITS_MAXIMUM];

#ifdef USING_KEY_MATRIX
/*********************************************
          Keys matrix routines
**********************************************/

/* keys_scanner() populates this table */
/* discrete_task() uses this table as a source of signals 
when bits[x].pin == 0 (zero means keys matrix node) */
unsigned char keys_state[64]; 

void keys_matrix_init(void)
{
int i;
    for(i=0;i<8;i++)
    {
       pinMode(scan_table_x[i], OUTPUT); 
       digitalWrite(scan_table_x[i], HIGH);
       pinMode(scan_table_y[i],INPUT_PULLUP);
    }  
    memset(keys_state,0,64);
    
}
static char x = 0;
int keys_scanner(void)
{
char i;        
   for(i=0;i<8;i++)
   {
     keys_state[x*8 + i] = digitalRead(scan_table_y[i]);
   }
   digitalWrite(scan_table_x[x], HIGH);
   x++;
   if(x>7)
      x = 0;
   digitalWrite(scan_table_x[x], LOW);
   if(x==0)
        return 1; /* scanning cycle completed */
return 0;        
}
#endif

/*********************************************
          Main discrete task
**********************************************/
/* takes 448 uS  */
char prev_pos = 0x19;
void discrete_task( void)
{
    int bit,flag;
    int bit_state;
    int i,itmp;
    #if TIMING_TEST
        digitalWrite(A0, HIGH);  
    #endif  
    #ifdef USING_MATRIX
    keys_scanner();
    #endif
    for(i=0;i<BITS_MAXIMUM;i++)
    {
       if(bits[i].flag & FLAG_ENABLED)
       {
           if(bits[i].critical_timer)
           {
               if(--bits[i].critical_timer == 0)
               {
                   Serial.write(STX);
                   Serial.write('1');
                   itmp = bits[i].id + 128;
                   Serial.write(hex_table[itmp >> 4]);
                   Serial.write(hex_table[itmp & 0x0F]);
                   Serial.write(ETX);
               }
           }
         #ifdef USING_MATRIX  
           if(bits[i].pin == 0)
               bit_state = keys_state[i];
           else
         #endif  
               bit_state = digitalRead(bits[i].pin);
               
           flag = bits[i].flag;
           bit = (flag & FLAG_INVERTED) ^ bit_state;
           if(bit)
           {
               if(bits[i].counter < bits[i].max)
               {
                   if(++(bits[i].counter) >= bits[i].max)
                   {
                       if(bits[i].signal == 0)
                       {
                           #ifdef NEW_BOARD
                              if(bits[i].id == 1)
                              {
                                 if(++crs_shift >= 3)
                                 {  
                                    crs_shift = 0; 
                                 }
                              }   

                           #endif
                           
                           //front __0__/--1--
                           
                           if(bits[i].critical_time)
                              bits[i].critical_timer = bits[i].critical_time;  
                           
                           if(bits[i].mode == ENCODER_A)
                           {
                               Serial.write(STX);
                               Serial.write('2');
                               itmp = bits[i].id + bits[i+1].signal;
                               Serial.write(hex_table[itmp >> 4]);
                               Serial.write(hex_table[itmp & 0x0F]);
                               Serial.write(ETX);
                            }
                            else if(bits[i].mode == ROTARY_SWITCH)
                            {
                                Serial.write(STX);
                                Serial.write('1');
                                itmp = bits[i].id;
                                Serial.write(hex_table[itmp >> 4]);
                                Serial.write(hex_table[itmp & 0x0F]);
                                Serial.write(ETX);
                            }
                            else if(bits[i].mode == ROTARY_SWITCH_X)
                            {
                                Serial.write(STX);
                                Serial.write('1');
                                itmp = bits[i].id;
                                Serial.write(hex_table[itmp >> 4]);
                                Serial.write(hex_table[itmp & 0x0F]);
                                Serial.write(ETX);

                                Serial.write(STX);
                                Serial.write('2');
                                if(prev_pos > bits[i].id)
                                    itmp = 0xF0;
                                else
                                    itmp = 0xF1; 
                                Serial.write(hex_table[itmp >> 4]);
                                Serial.write(hex_table[itmp & 0x0F]);
                                Serial.write(ETX);
                                prev_pos = bits[i].id;
                                
                            }  
                            else if(bits[i].mode != ENCODER_B && bits[i].mode != PUSH_BUTTON_X)
                            {
                                Serial.write(STX);
                                Serial.write('8');
                                itmp = bits[i].id;
                                Serial.write(hex_table[itmp >> 4]);
                                Serial.write(hex_table[itmp & 0x0F]);
                                Serial.write(ETX);
                            }
                        }
                        bits[i].signal = 1;
                    }
                }
            } 
            else
            {
                if(bits[i].counter > 0)
                {
                    if(--(bits[i].counter) <= 0)
                    {
                        if(bits[i].signal)
                        {
                            // edge  --1--\___0___

                           if(bits[i].flag & FLAG_HALF_CYCLE_EC)
                           {
                               if(bits[i].mode == ENCODER_B)
                               {
                                   Serial.write(STX);
                                   Serial.write('2');
                                   itmp = bits[i].id + bits[i-1].signal;
                                   Serial.write(hex_table[itmp >> 4]);
                                   Serial.write(hex_table[itmp & 0x0F]);
                                   Serial.write(ETX);
                               }
                           } 
                           if( bits[i].mode == TOGGLE_SWITCH || 
                                bits[i].mode == PUSH_BUTTON )
                            {
                                Serial.write(STX);
                                Serial.write('4');
                                itmp = bits[i].id;
                                Serial.write(hex_table[itmp >> 4]);
                                Serial.write(hex_table[itmp & 0x0F]);
                                Serial.write(ETX);
                            } 
                            else if(bits[i].mode == PUSH_BUTTON_X && bits[i].critical_timer)
                            {
                                Serial.write(STX);
                                Serial.write('1');
                                itmp = bits[i].id;
                                Serial.write(hex_table[itmp >> 4]);
                                Serial.write(hex_table[itmp & 0x0F]);
                                Serial.write(ETX);
                            }
                            bits[i].critical_timer = 0;
                        }   
                        bits[i].signal = 0;
                    }
                }
            }
        }
    }
    #if TIMING_TEST    
        digitalWrite(A0, LOW);  
    #endif
}



/*********************************************
      Regular inputs setup macro definitions
**********************************************/
    
#define input_init(x,y,m,i)   do {\
bits[x].counter = 0;\
bits[x].signal = 0;\
bits[x].max = 50;\
bits[x].flag = (FLAG_ENABLED | FLAG_INVERTED);\
bits[x].pin = y;\
pinMode(y,INPUT_PULLUP);\
bits[x].mode = m;\
bits[x].id = i;\
} while(0)


#define input_init_rotary_switch(x,y,m,i)   do {\
bits[x].counter = 0;\
bits[x].signal = 0;\
bits[x].max = 30;\
bits[x].flag = (FLAG_ENABLED | FLAG_INVERTED);\
bits[x].pin = y;\
pinMode(y,INPUT_PULLUP);\
bits[x].mode = m;\
bits[x].id = i;\
} while(0)

#define input_init_fast(x,y,m,i)   do {\
bits[x].counter = 0;\
bits[x].signal = 0;\
bits[x].max = 2;\
bits[x].flag = (FLAG_ENABLED | FLAG_INVERTED );\
bits[x].pin = y;\
pinMode(y,INPUT_PULLUP);\
bits[x].mode = m;\
bits[x].id = i;\
} while(0)

#define input_init_fast_half_cycle_ec(x,y,m,i)   do {\
bits[x].counter = 0;\
bits[x].signal = 0;\
bits[x].max = 2;\
bits[x].flag = (FLAG_ENABLED | FLAG_INVERTED | FLAG_HALF_CYCLE_EC);\
bits[x].pin = y;\
pinMode(y,INPUT_PULLUP);\
bits[x].mode = m;\
bits[x].id = i;\
} while(0)

   
/*****************************************
           Inputs setup
*****************************************/
void bits_init(void)
{  
    int j = 0;
    memset(bits,0,sizeof(bits));
    input_init(0,5,PUSH_BUTTON,j++);
    input_init(1,6,PUSH_BUTTON,j++);
    input_init(2,7,PUSH_BUTTON,j++);
    
} 


#ifdef USING LAPMS
/*****************************************
           Discrete outputs (LEDs)
*****************************************/
void leds_init(void)
{
int i;
    for(i=0;i<LEDS_MAX;i++)
    {
      pinMode(leds[i], OUTPUT); 
      digitalWrite(leds[i], LOW);
    }

}
#endif
