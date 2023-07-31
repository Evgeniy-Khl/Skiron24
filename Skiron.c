/*******************************************************
Project        : Skiron
Version        : 0.0
Date           : 30.07.2023
Chip type      : ATmega32
Clock frequency: 16,000000 MHz
Program size   : 12079 words (24158 bytes), 73,7% of FLASH [xxxx] EEPROM [xxxx] 30.07.2023
*******************************************************/

#include <mega32.h>
#include <stdio.h>
#include <twi.h>
#include <delay.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <1wire.h>
#include "fontsN.h"
#include "tft.h"
#include "9341.h"
//*********************************
#define RCK		        PORTB.0
#define LOCK	        PORTB.1
#define BEEP		    PORTB.2
#define SCK		        PORTB.7
#define CSDAC1		    PORTD.4
#define CSDAC2		    PORTD.5
#define CSDAC3		    PORTD.6
#define CSDAC4	        PORTD.7
#define CONECT          PINB.3    // если bluetooth подключен то здесь +3,3В

#define MAX_DEVICES     3
#define MAX_MENU        7
#define MENU0           4
#define MENU1           3
#define MAX_4           4
#define MAX_5           5
#define MAX_6           6
//#define MAX_7           7
#define MISTAKE         3
#define ZERO	        50

#define DATAREAD        0xA1      // Read Scratchpad
#define SPI_MOUD_DSP    0xD2      // Mode 0; Clock Rate: 250,000 kHz; SPI Interrupt Enable.
#define SPI_MOUD_FL	    0x51	  // SPI Type: Master, Clock Rate: 1000,000 kHz, Clock Phase: Cycle Half, Clock Polarity: Low, Data Order: MSB First
#define TWI_CLK_RATE    100000    // TWI clock rate [bps]
#define EEPROM_TWI_BUS_ADDRESS (0xA0 >> 1)// 7 bit TWI bus slave address of the AT24C16B 2kbyte EEPROM
#define ADC_VREF_TYPE   0x40
#define F_XTAL			16000L               //quartz crystal frequency [KHz]
#define INIT_TIMER0     TCNT0 =   0x100L-F_XTAL/1024L*5L    // 5 ms

#define DS_SRTC         0xD0    // Serial Real Time Clock (Часовая микросхема)
#define CLOCK_BUFFER	8
#define SQWE_32768Hz    0x13
#define SQWE_1Hz        0x10

// Declare your global variables here
//flash float A1=1.8, A2=0.81, A3=0.01;  // порядок a=0.9 (A1=2a; A2=a^2; A3=(1-a)^2)
//flash float B1=1.6, B2=0.64, B3=0.04;  // порядок a=0.8 (A1=2a; A2=a^2; A3=(1-a)^2)
flash float A1=1.2, A2=0.36, A3=0.16;  // порядок a=0.6 (A1=2a; A2=a^2; A3=(1-a)^2)
unsigned char BeepT, keynum, keycount, ok, portOut, newSetButt, ds18b20, soilModule, pointY, DHTexist, signchar, intval, frcval, errors, noAutoRel, noAutoAna;
signed char numMenu, numSet, displ_num, pauseEdit, moduleEdit/*, displCO2, timerCO2*/;
unsigned char relOut[4]={0}, analogOut[4]={0}, dacU[4]={ZERO}, buff[40], familycode[MAX_DEVICES][9], clock_buffer[CLOCK_BUFFER], alarm[4]={2,2,2,2};
unsigned int  max_X, max_Y, timerOn, timerOff, fillScreen = BLACK, val1,val2;
signed int pvT=1990, offsetT, pvRH=1990, offsetRH, pvCO2, pvPH, newval[7], t[MAX_DEVICES], hum[MAX_DEVICES];
unsigned char *ptr_char;
const char* setMenu[MENU0]={"Головны","Ступынь","Функцыя","Системны"};
//const char* setMenu[MAX_MENU]={"Температура","Вологысть","Таймер","День Ныч","Час Дата","Ынше","Модуль"};
const char* setName0[MENU1]={"Температура","Выдхилення","Аварыйне"};
const char* setName1[MAX_6]={"Увымкнено","Розмір.","Вимкнено","Розмір.","Крок","ЗМЫЩЕННЯ"};
const char* setName2[MAX_6]={"День почат.","Ныч почат.","Увымкнено Р","Вимкнено Р","Увымкнено В","Вимкнено В"};
const char* setName3[MAX_4]={"MIN","MAX","КОФ.1","Коф.2"};
const char* setName7[MAX_5]={"Хвилини","Години","День","Мысяц","Рык"};
//--------------- union declaration -----------------------------------------------
union {unsigned char buffer[8]; unsigned int pvT;} ds;          // буффер микросхемы DS18B20
union {unsigned char buffer[4]; unsigned int val[2];} in;
union {unsigned char buffer[4]; unsigned int val[2];} out;
//---------------------------------------------------------------------------------
float Told1[8], Told2[8], iPart[4];

//-------------------------
eeprom signed char relaySet[4] ={-1,1,1,-1};
eeprom signed char analogSet[4]={-1,20,50,-1};

// Уставка относительной температуры АНАЛОГОВЫХ выходов
eeprom struct {
                 signed int biasT; // Т сдвиг
              unsigned char bandT, // Т диапазон
                           minVal, // минимальное значение
                           maxVal; // максимальное значение
              } analog[4]={
//biasT bandT  min  max 
// 1,0   2,0   Val  Val
{0xFFF0, 0x40,  0,  100},  // 0 - Аналоговый ВЫХОД I
{0x0010, 0x40,  0,  100},  // 1 - Аналоговый ВЫХОД II
{0x0020, 0x40,  0,  100},  // 2 - Аналоговый ВЫХОД III
{0x0030, 0x40,  0,  100}}; // 3 - Аналоговый ВЫХОД IV 

eeprom signed int set[6][7]={
{ 230, 200,  50,  10,   1,  -1,   0},  // (ВОЗД.) Tday;  Tnight;  dTalarm;  hysteresis;  mode=1(нагрев)/mode=0(охлаждение); резерв;    выход № РЕЛЕ1
{  60,  60,  10,   5,   0,   0,   1},  // (ВОЗД.) RHday; RHnight; dRHalarm; hysteresis;  mode=1(увлажнение)/mode=0(осушение); DHT22=0; выход № РЕЛЕ2
{ 200, 180,  50,  10,   1,  -1,   6},  // (ГРУНТ) Tday;  Tnight;  dTalarm;  hysteresis;  mode=1(нагрев)/mode=0(охлаждение); резерв;    выход №
{ 400, 350, 100,  50,   1,  -1,   7},  // (ГРУНТ) RHday; RHnight;  dTalarm;  hysteresis; mode=1(увлажнение)/mode=0(осушение); резерв;  выход №   
{  10,  0,   10,   1,   0,0x06,   2},  // tmOn; dimOn=0(сек.)/dim=1(мин.); tmOff; dimOff; HourStart; Programm;                         выход № РЕЛЕ3
{0x07,0x20,0x05,0x09,0x18,0x23,   3}}; // DayBeg; DayEnd; Light0Beg; Light0End; Light1Beg; Light1End;                                  выход № РЕЛЕ4

eeprom unsigned char limit[5][4]={
                    // min max  kP   kI 
                      {  0,100, 20, 100}, // 4
                      {  0,100, 21, 100}, // 5
                      {  0,100, 22, 100}, // 6
                      {  0,100, 23, 100}, // 7
                      {  0, 30, 66, 138}}; // Грунт температура  t=0 -> V=1.32 -> ADC=270; t=25 -> V=2.51 -> ADC=514

eeprom unsigned int module[4][2]={
                      { 10,600}, // Грунт влажность 100%: 0% OutMIN; OutMAX; модуль №1
                      { 10,600}, // Грунт влажность 100%: 0% OutMIN; OutMAX; модуль №2
                      { 10,600}, // Грунт влажность 100%: 0% OutMIN; OutMAX; модуль №3
                      { 10,600}};// Грунт влажность 100%: 0% OutMIN; OutMAX; модуль №4
bit Night;
bit Sec;          // новая секунда
bit Display;      // обновить дисплай
bit Dht;
bit Clock_Ok;
bit ;    // несколько реле в ручном режиме
bit ;    // несколько аналог. в ручном режиме

//- prototypes ------
void display(void);

#include "fontsN.c"
#include "ili9341.c"
#include "twi.c"
#include "dht.c"
#include "proc.c"
#include "control.c"
#include "keypad.c"
#include "displ.c"

// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void){
 static unsigned int count1;
 count1++;
}

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)  // интервал 5 mS отсчитывает текущее время
{
  RCK = 1;
  INIT_TIMER0;
  if(BeepT){--BeepT; BEEP = ON;} else BEEP = OFF;		    // включить серену  1; отключить серену 0      конец  1,9-2,44 us
  SCK = 1; SCK = 0; SPCR=SPI_MOUD_DSP; RCK = 0; SPDR=0xFF;  // паралельная загрузка значения клавиатуры + запуск передачи по SPI
}

// 0.25 Hz timer interrupt generated by Timer1 overflow;  Timer Period: 1,0486 s
interrupt [TIM1_OVF] void timer_comp_isr(void){
 Sec=1; Display=1;
} 

// SPI interrupt service routine
interrupt [SPI_STC] void spi_isr(void){
 unsigned char data;
  SPCR=0;           // SPI disabled
  data = ~SPDR;     // данные клавиатуры
  if(data) keycount++; else keycount=0;
  if(keycount>2){keynum=data; BeepT=25;}  
}

void main(void){
// Declare your local variables here
signed char x, byte;
#include "init.c"

 while (1){
//--------------------------- функция 1 секунда ---------------------------------------------
    if(Sec){                     
        Sec=0; noAutoRel=0; noAutoAna=0;
        for (byte=0; byte<4; byte++) if(relaySet[byte]>=0) noAutoRel++; // проверка - все ли находится в автоматическом режиме
        for (byte=0; byte<4; byte++) if(analogSet[byte]>=0) noAutoAna++;// проверка - все ли находится в автоматическом режиме
    // -- считывание значений температуры ---------
        if(ds18b20) temperature_check();          
        if(Dht){                                         // присутствует датчик влажности
            if(readDHT()) DHTexist = 3; 
            else if(DHTexist) DHTexist--;                // датчик влажности работает? 
            else {pvT = 1900; pvRH = 190;}
        }
        else if(ds18b20) pvT = mean(ds18b20); 
//------ Аналитика ----------------------------------------------------------------------------
        for (byte=0;byte<4;byte++){dacU[byte]=analog[byte].minVal;};     // Исходное задание каналов № 1,2,3,4
        for (byte=0;byte<4;byte++) if(analogSet[byte]>=0) dacU[byte]=adapt(analogSet[byte]); else dacU[byte] = adapt(dacU[byte]);
//------- Проверка на ручное управление --------------------------------------------------------
        for (byte=0;byte<4;byte++){
            if(relaySet[byte]==1) relOut[byte]=ON;  // ручной On
            if(relaySet[byte]==0) relOut[byte]=OFF; // ручной Off
        }
    }
//------------------------- КОНЕЦ функция 1 секунда -----------------------------------------
    if(Display){
      Display=0; display();
      //----------
      sprintf(buff,"kn=%3u D=%2u M=%2u S=%2u",keynum,displ_num,numMenu,numSet);
      ILI9341_WriteString(5,220,buff,Font_11x18,WHITE,BLACK,1);
      //----------
    }
 }
}
