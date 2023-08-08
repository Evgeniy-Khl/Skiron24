/*******************************************************
Project        : Skiron
Version        : 0.0
Date           : 30.07.2023
Chip type      : ATmega32
Clock frequency: 16,000000 MHz
Program size   : 10823 words (21646 bytes), 66,1% of FLASH [xxxx] EEPROM [xxxx] 31.07.2023
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

#define ON              1
#define OFF             0
#define MAX_DEVICES     3
#define MAX_MENU        7
#define MENU            5
#define LIST0           3
#define LIST1           2
#define LIST2           5
#define LIST3           5
#define LIST4           2
#define MENU2           5
#define MENU3           3
#define MAX_5           5
#define MAX_6           6
#define MISTAKE         3
#define ZERO	        50

#define DATAREAD        0xA1      // Read Scratchpad
#define SPI_MOUD_DSP    0xD2      // Mode 0; Clock Rate: 250,000 kHz; SPI Interrupt Enable.
#define SPI_MOUD_FL	    0x51	  // SPI Type: Master, Clock Rate: 1000,000 kHz, Clock Phase: Cycle Half, Clock Polarity: Low, Data Order: MSB First
#define TWI_CLK_RATE    100000    // TWI clock rate [bps]
#define ADC_VREF_TYPE   0x40
#define F_XTAL			16000L               //quartz crystal frequency [KHz]
#define INIT_TIMER0     TCNT0 = 0x100L-F_XTAL/1024L*5L    // 5 ms

#define DS_SRTC         0xD0    // Serial Real Time Clock (Часовая микросхема)
#define CLOCK_BUFFER	8
#define SQWE_32768Hz    0x13
#define SQWE_1Hz        0x10

// Declare your global variables here
flash float A1[2]={1.8,1.2}, A2[2]={0.81,0.36}, A3[2]={0.01,0.16};  // a[0]=0.9 и a[1]=0.6
unsigned char BeepT, flags, keynum, keycount, keywait, ok, relayOut, newSetButt, ds18b20, pointY, DHTexist, signchar, intval, frcval, errors, noAutoRel, noAutoAna;
signed char numMenu, subMenu, numSet, displ_num, pauseEdit;
unsigned char relOut[4]={0}, analogOut[4]={0}, dacU[4]={ZERO}, buff[40], familycode[MAX_DEVICES][9], clock_buffer[CLOCK_BUFFER], alarm[4]={2,2,2,2};
unsigned int  max_X, max_Y, timerOn, timerOff, fillScreen = BLACK;
signed int pvT=1990, offsetT, pvRH=1990, offsetRH, newval[7], t[MAX_DEVICES], hum[MAX_DEVICES];
unsigned char *ptr_char;
const char* setMenu[MENU]={"Головны","Щаблы","Функцыя","Час Дата","Системны"};     // Главное меню
const char* setName0[LIST0]={"Температура","Верхня межа","Нижня межа"}; // подменю "Головнi"
const char* setName1[LIST1]={"Змыщення", "Гыстер."};                    // подменю "Щаблі"
const char* setName2[LIST2]={"Змыщення","КОФ.1","КОФ.2","MIN","MAX"};   // подменю "Функція"
const char* setName3[LIST3]={"Хвилини","Години","День","Мысяц","Рык"};  // подменю "Час Дата"
const char* setName4[LIST4]={"Bluetooth","Ынше"};            // подменю "Системні"
//--------------- union declaration -----------------------------------------------
union {unsigned char buffer[8]; unsigned int pvT;} ds;          // буффер микросхемы DS18B20
//---------------------------------------------------------------------------------
float Told1[8], Told2[8], iPart[4];
//-------------------------
eeprom signed char relaySet[4] ={-1,-1,-1,-1};
eeprom signed char analogSet[4]={-1,-1,-1,-1};
// Основные уставки -      Т;  max. min.
eeprom signed int spT[5]={270, 300, 200, 0, 0};
// Уставка относительной температуры ЦИФРОВЫХ выходов
eeprom unsigned char digital[4][2]={    // [0]-> dТ смещение температуры; [1]-> ГИСТЕРЕЗИС
                    { 5, 1},       // 0 - Ступень I  
                    {10, 2},       // 1 - Ступень II
                    {15, 3},       // 2 - Ступень III
                    {20, 4}};      // 3 - Ступень IV
// Уставка относительной температуры АНАЛОГОВЫХ выходов
eeprom unsigned char analog[4][5]={     // [0]-> dТ смещение температуры; [1]-> Проп. коэфф.; [2]-> Интегр. коэфф.; [3]-> MIN; [4]-> MAX
                    {20, 20, 100, 0, 100},  // 0 - Аналоговый ВЫХОД I   Тунельная вентиляция
                    {10, 20, 100, 0, 100},  // 1 - Аналоговый ВЫХОД II  Положение заслонок вытяжной вентиляции
                    {15, 20, 100, 0, 100},  // 2 - Аналоговый ВЫХОД III Положение заслонок приточной вентиляции
                    { 5, 20, 100, 0, 100}}; // 3 - Аналоговый ВЫХОД IV  Положение клапана горячей воды.

bit Sec;          // новая секунда
bit Display;      // обновить дисплай
bit Clock_Ok;

//#define FLG0    flags.0
//#define FLG1    flags.1
//#define FLG2    flags.2
//#define FLG3    flags.3


//- prototypes ------
void display(void);

#include "fontsN.c"
#include "ili9341.c"
#include "twi.c"
#include "proc.c"
#include "calculate.c"
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
  if(keywait) --keywait;
  else {SCK = 1; SCK = 0; SPCR=SPI_MOUD_DSP; RCK = 0; SPDR=0xFF;}  // паралельная загрузка значения клавиатуры + запуск передачи по SPI
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
  if(keycount>2){keynum=data; BeepT=25; keywait=255; Display=1;}  
}

void main(void){
// Declare your local variables here
signed char x, byte;
#include "init.c"

 while (1){
 //--------------------------- функция 1 секунда ---------------------------------------------
    if(Sec){                     
        Sec=0; noAutoRel=0; noAutoAna=0; errors=0;
        for (byte=0; byte<4; byte++) if(relaySet[byte]>=0)  {noAutoRel++; errors|=0x10;} // проверка - все ли находится в автоматическом режиме
        for (byte=0; byte<4; byte++) if(analogSet[byte]>=0) {noAutoAna++; errors|=0x10;} // проверка - все ли находится в автоматическом режиме
    //------ считывание значений температуры ---------
        if(ds18b20) temperature_check();          
        pvT = mean(ds18b20-1); 
    //------ Аналитика ----------------------------------------------------------------------------
        for (byte=0;byte<4;byte++){dacU[byte]=analog[byte][2];};// минимальное задание каналов 1,2,3,4
        control(pvT);                         // Среднее значение датчиков воздуха (2 шт.) 
        if(errors&0x08){                      // выключаем всё если ошибка среднего значения
            for (byte=0;byte<4;byte++){relOut[byte]=OFF; relayOut &= ~(1<<(byte+4));}// все Off
            for (byte=0;byte<4;byte++){dacU[byte] = 0;} // все 0%               
        }
        for (byte=0;byte<4;byte++) if(analogSet[byte]>=0) dacU[byte]=adapt(analogSet[byte]); else dacU[byte] = adapt(dacU[byte]);         
    //------ Проверка на ручное управление --------------------------------------------------------        
        for (byte=0;byte<4;byte++){
            if(relaySet[byte]==1) {relOut[byte]=ON;  relayOut |= (1<<(byte+4));} // ручной On
            if(relaySet[byte]==0) {relOut[byte]=OFF; relayOut &= ~(1<<(byte+4));}// ручной Off
        }
        
        byte = PORTA & 0x0F;
        relayOut = relayOut & 0xF0;
        relayOut |= byte;
        PORTA = relayOut;  // PORA.4->Ст. I -- PORA.7->Ст. IV
        LOCK=1; delay_us(1); LOCK=0;
//        setDAC();                           // подать напряжение на аналоговые выходы
    }
 //------------------------- КОНЕЦ функция 1 секунда -----------------------------------------
    if(Display){
      Display=0; display();
      //----------
//      sprintf(buff,"rOut 0x%02x",relayOut&0xF0);
//      ILI9341_WriteString(5,200,buff,Font_11x18,WHITE,BLACK,1);
      sprintf(buff,"Dn%2u nM%2u sM%2u nS%2u FL0x%x",displ_num,numMenu,subMenu,numSet,flags);
      ILI9341_WriteString(5,220,buff,Font_11x18,WHITE,BLACK,1);
      //----------
    }
 }
}
