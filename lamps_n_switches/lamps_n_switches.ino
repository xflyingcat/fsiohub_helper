
#define DEVICE_ID  2

/*--------------------------------------------------------
  "THE BEER-WARE LICENSE" (Revision 42):
  Alex Kostyuk wrote this code. As long as you retain this 
  notice you can do whatever you want with this stuff. 
  If we meet some day, and you think this stuff is worth it, 
  you can buy me a beer in return.
----------------------------------------------------------*/

#define STX   '{'
#define ETX   '}'

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
#if SERIAL_TX_BUFFER_SIZE != 512
 //#error SERIAL_TX_BUFFER_SIZE still is not 512 bytes
#endif

#if SERIAL_RX_BUFFER_SIZE != 1024
 //#error SERIAL_RX_BUFFER_SIZE still is not 1024 bytes
#endif

const char hex_table[] = "0123456789ABCDEF";

static unsigned char nibble(int hex)
{
 if(hex>='0' && hex<='9') return (hex - '0');
 if(hex>='A' && hex<='F') return (hex - 'A' + 10);
 if(hex>='a' && hex<='f') return (hex - 'a' + 10);
return 0;
}

static int hex2int(char *hh)
{
int nib1 = nibble(hh[0]&0xFF);
int nib2 = nibble(hh[1]&0xFF);
return  (nib1 << 4) + nib2;
}

unsigned long hex2ulong(const char *in)
{
unsigned long ul  = 0;
char *wp = (char*)in;
int i;

   for(i=0;i<4;i++)
   {
    ul <<= 8;
    ul += hex2int(wp);
    wp += 2;
   }

  return ul;
}


void discrete_task( void);
void discrete_task_init(void);

const char leds[] = {3,4,5};

void leds_setup(void)
{
   int i;
   for(i=0;i<3;i++)
   {
     pinMode(leds[i],OUTPUT);
     digitalWrite(leds[i],HIGH);
   }  
  
}

void setup()
{
    Serial.begin(115200);
    leds_setup();
    discrete_task_init();
}


void serial_read(void) 
{
    static char buf[8];
    static int ndx = 0;
    int itmp,i;
    if(Serial.available()>0) 
    {
        char ch = (char)Serial.read();
        switch(ch)
        {
            case STX:
                ndx = 0;
            break;

            case ETX:
                
                if(buf[0] == 'D')
                {
                   Serial.write('!');
                   Serial.write(STX);
                   Serial.write('F');
                   itmp = DEVICE_ID;
                   Serial.write(hex_table[itmp >> 4]);
                   Serial.write(hex_table[itmp & 0x0F]);
                   Serial.write(ETX);
                }
                if(buf[0] == 'L')
                {
                  for(i=0;i<3;i++)
                  {
                   if(buf[3+i] & 1)
                        digitalWrite(leds[i],LOW);
                   else
                        digitalWrite(leds[i],HIGH);
                  }      
                   Serial.write('!');
                }
                else
                    Serial.write('!');
            break;
            default:
                if(ndx<8)
                    buf[ndx++] = ch;

        }
    }
    
}  



void loop(void)
{
    static unsigned long ms_reftime = 0;
    int cur[4];
    unsigned long cur_time = millis();
    static int timer = 100;
    serial_read(); 
    if((cur_time - ms_reftime) >= 10) 
    {
       ms_reftime = cur_time;
       
       discrete_task();  

              if(timer)
              {
                  if(--timer == 0)
                  {
                    Serial.write(".");
                    timer = 100;
                  } 
              }

    }
}



//-------------------------------------------
//               Discrete task
//-------------------------------------------
#define BITS_MAXIMUM   3 /*ITEMS_MAX*/
int shift = 0;

#define FLAG_INVERTED   (1<<0)
#define FLAG_ENABLED    (1<<1)

enum {
  PUSH_BUTTON = 0,
  TOGGLE_SWITCH,
  ROTARY_SWITCH_POLE,
  ENCODER_A,
  ENCODER_B
};

typedef struct {
int counter;
char signal;
int  max;
//char evcode;
char flag;
char mode;
char pin;
unsigned char emitted_code_rising;
unsigned char emitted_code_falling;
int pulse_timer;
int pulse_timer_value;
} BITS_DESCRIPTION;

BITS_DESCRIPTION  bits[BITS_MAXIMUM];
 
#define input_init(x,y,z,m,r,f, t)   do {\
bits[x].counter = 0;\
bits[x].signal = 0;\
bits[x].max = y;\
bits[x].flag = (FLAG_ENABLED | FLAG_INVERTED);\
bits[x].pin = z;\
pinMode(z,INPUT_PULLUP);\
bits[x].mode = m;\
bits[x].emitted_code_rising = r;\
bits[x].emitted_code_falling = f;\
bits[x].pulse_timer_value = t;\
} while(0)

void discrete_task_init(void)
{
  int i;
  memset(bits,0,sizeof(bits));
// buttons  
#define DEBOUNCE_DELAY   10

input_init(0, DEBOUNCE_DELAY, 6, PUSH_BUTTON,   0, -1, 0);
input_init(1, DEBOUNCE_DELAY, 7, PUSH_BUTTON,   1, -1, 0);
input_init(2,DEBOUNCE_DELAY, 8, TOGGLE_SWITCH,  2,  3, 0);

}

void discrete_task( void)
{
    int bit,flag;
    int bit_state;
    int i,itmp;

    for(i=0;i<BITS_MAXIMUM;i++)
    {
        if(bits[i].pulse_timer)
        {
            if(--bits[i].pulse_timer == 0)
            {
                switch(bits[i].mode)
                {
                    case PUSH_BUTTON:
                    
                    break;

                    case ENCODER_A:
                        if(bits[i+1].signal)
                        {
                            
                        } else
                        {
                            
                        }
                    break;

                    case ENCODER_B:
                    
                    break;

                    case TOGGLE_SWITCH:

                    break;

                    case ROTARY_SWITCH_POLE:

                    break;
                    

                }
            }
        }

        if(bits[i].flag & FLAG_ENABLED)
        {
            if(bits[i].pin) // regular signal
                bit_state = digitalRead(bits[i].pin);
            else
                bit_state = 0;
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
                            //front __0__/--1-- 
                            switch(bits[i].mode)
                            {
                                case PUSH_BUTTON:
                                    Serial.write(STX);
                                    Serial.write('8');
                                    itmp = bits[i].emitted_code_rising;
                                    Serial.write(hex_table[itmp >> 4]);
                                    Serial.write(hex_table[itmp & 0x0F]);
                                    Serial.write(ETX);
                                break;

                                case ROTARY_SWITCH_POLE:
                                    Serial.write(STX);
                                    Serial.write('1');
                                    itmp = bits[i].emitted_code_rising;
                                    Serial.write(hex_table[itmp >> 4]);
                                    Serial.write(hex_table[itmp & 0x0F]);
                                    Serial.write(ETX);
                                break;

                                case ENCODER_A:
                                    if(bits[i+1].signal)
                                    {

                                    } else
                                    {

                                    }
                                break;

                                case ENCODER_B:
                                break;

                                case TOGGLE_SWITCH:
                                    Serial.write(STX);
                                    Serial.write('8');
                                    itmp = bits[i].emitted_code_rising;
                                    Serial.write(hex_table[itmp >> 4]);
                                    Serial.write(hex_table[itmp & 0x0F]);
                                    Serial.write(ETX);
                                break;


                            }
                            bits[i].signal = 1;
                        }

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
                            switch(bits[i].mode)
                            {
                                case PUSH_BUTTON:
                                    Serial.write(STX);
                                    Serial.write('4');
                                    itmp = bits[i].emitted_code_rising;
                                    Serial.write(hex_table[itmp >> 4]);
                                    Serial.write(hex_table[itmp & 0x0F]);
                                    Serial.write(ETX);
                                break;

                                case ENCODER_A:
;
                                break;

                                case ENCODER_B:
                                
                                break;

                                case TOGGLE_SWITCH:
                                    Serial.write(STX);
                                    Serial.write('4');
                                    itmp = bits[i].emitted_code_rising;
                                    Serial.write(hex_table[itmp >> 4]);
                                    Serial.write(hex_table[itmp & 0x0F]);
                                    Serial.write(ETX);
                                break;

                            }
                            bits[i].signal = 0;
                        }
                    }
                }
            }
        }
    }
}




