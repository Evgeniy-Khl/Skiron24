#define KEY_1          1
#define KEY_2          2
#define KEY_3          4
#define KEY_4          8
#define KEY_5          16
#define KEY_6          32
#define KEY_7          64
#define KEY_7_4        72
#define KEY_7_6        96
#define KEY_8          128
#define KEY_7_8        192

void checkkey(unsigned char key){
 char byte, max = 5;
  if(key){
     switch (displ_num){
       //----------------------------- Состояние ВЫХОДОВ -------------------------------
       case 2:
         max = 8;
         switch (key){
            case KEY_1: if(--displ_num<0) displ_num=3; newSetButt=1; break;//  (белая 1)
            case KEY_6: displ_num = 0; newSetButt = 1; break;
            case KEY_2: if(++displ_num>3) displ_num=0; newSetButt=1; break;//  (белая 2)
            case KEY_3: if(--numSet<0) numSet = 0;     break;
            case KEY_7: if(++numSet>max) numSet = max; break;
            case KEY_4: 
                if(numSet<4){if(++relaySet[numSet]>1) relaySet[numSet]=-1;}
                else {
                    byte = numSet-4;
                    analogSet[byte]+=10;
                    if(analogSet[byte]==9) analogSet[byte]=0;
                    if(analogSet[byte]>100) analogSet[byte]=100;
                } 
            break;
            case KEY_8: 
                if(numSet<4){if(--relaySet[numSet]<-1) relaySet[numSet]=1;}
                else {
                    byte = numSet-4;
                    analogSet[byte]-=10; 
                    if(analogSet[byte]<0) analogSet[byte]=-1;}
            break;
         }
       break;
       //----------------------------- Начальное меню ----------------------------------
       case 3:  // displ_num
         switch (key){
            case KEY_1: if(--displ_num<0) displ_num=3; newSetButt=1; break;//  (белая 1)
            case KEY_6: displ_num = 0; newSetButt = 1; break;
            case KEY_3: if(--numMenu<0) numMenu = MENU-1; break;    // "Головнi","Щаблi","Функцiя","Системнi"
            case KEY_7: if(++numMenu>MENU-1) numMenu = 0; break;
            case KEY_5: newSetButt = 1; displ_num=4; subMenu=0; numSet=0; break;// (зеленая)
         }
       break;
       //----------------------------- Все подменю -------------------------------------
       case 4:  // displ_num
         switch (numMenu){
           case 0: max = LIST0; break;
           case 1: max = LIST1; break;
           case 2: max = LIST2; break;
           case 3: max = LIST3; break;
           case 4: max = LIST4; break;            
           default: max = 2;
         };
         switch (key){
            case KEY_1: displ_num = 3; newSetButt = 1; break;
            case KEY_6: displ_num = 0; newSetButt = 1; break;
            case KEY_3: 
                    if(numMenu==0||numMenu>2){if(--numSet<0) numSet = max-1;}
                    else if(--subMenu<0) subMenu = 3; 
            break;
            case KEY_7: 
                    if(numMenu==0||numMenu>2){if(++numSet>max-1) numSet = 0;}
                    else if(++subMenu>3) subMenu = 0; 
            break;
            case KEY_5: displ_num = 5; newSetButt = 1; // (зеленая)
                switch (numMenu) {
                    case 0: for (byte=0;byte<LIST0;byte++) newval[byte] = spT[byte]; break;    // "Температура","Верхня межа","Нижня межа" 
                    case 3:                                                                    // "Час Дата"
                        for (byte=0;byte<2;byte++) newval[byte] = rtcTodec(clock_buffer[byte+1]); 
                        for (byte=2;byte<5;byte++) newval[byte] = rtcTodec(clock_buffer[byte+2]);
                    break;     
                    case 4: for (byte=0;byte<LIST4;byte++) newval[byte] = spT[byte+3]; break;  // "Bluetooth","Iнше"
                    case 1: displ_num=6; subMenu=0; break;  // "Щаблi"
                    case 2: displ_num=7; subMenu=0; break;  // "Функцiя"
                } 
            break;
         }
       break;
       //----------------------------- РЕДАКТИРОВАНИЕ ----------------------------------
       case 5:  // displ_num
          switch (key){
            case KEY_1: displ_num = 4; newSetButt = 1; break;
            case KEY_6: displ_num = 0; newSetButt = 1; break;
            case KEY_4: ++newval[numSet];    
                 switch (numMenu) {
                    case 0:  // Головнi 
                        switch (numSet) {
                            case 0: if(newval[numSet]>spT[1]) newval[numSet]=spT[1]; break; // Температура
                            case 1: if(newval[numSet]>1200) newval[numSet]=1200; break; // Верхня межа
                            case 2: if(newval[numSet]>500) newval[numSet]=500; break;   // Нижня межа
                        }; 
                    break;
                    case 3:  // Час Дата 
                        switch (numSet) {
                            case 0: if(newval[numSet]>59) newval[numSet]=0;  break;   // минуты
                            case 1: if(newval[numSet]>23) newval[numSet]=0;  break;   // часы
                            case 2: if(newval[numSet]>31) newval[numSet]=1;  break;   // день
                            case 3: if(newval[numSet]>12) newval[numSet]=1;  break;   // месяц
                            case 4: if(newval[numSet]>59) newval[numSet]=22; break;   // год
                        };
                    break;
                    case 4:  // Системнi 
                        switch (numSet) {
                            case 0: if(newval[numSet]>1) newval[numSet]=1; break;   // Bluetooth
                            case 1: if(newval[numSet]>1) newval[numSet]=1; break;   // Iнше                        
                        }; 
                    break;
                  }; 
            break;
            case KEY_8: --newval[numSet];
                 switch (numMenu) {
                    case 0:  // Температура 
                        switch (numSet) {
                            case 0: if(newval[numSet]<spT[2]) newval[numSet]=spT[2]; break;     // Температура
                            case 1: if(newval[numSet]<0) newval[numSet]=0; break;               // Верхня межа
                            case 2: if(newval[numSet]<-500) newval[numSet]=-500; break;         // Нижня межа                          
                        }; 
                    break;
                    case 3:  // Час Дата 
                        switch (numSet) {
                            case 0: if(newval[numSet]<0)  newval[numSet]=59; break;   // минуты
                            case 1: if(newval[numSet]<0)  newval[numSet]=23; break;   // часы
                            case 2: if(newval[numSet]<1)  newval[numSet]=31; break;   // день
                            case 3: if(newval[numSet]<1)  newval[numSet]=12; break;   // месяц
                            case 4: if(newval[numSet]<22) newval[numSet]=59; break;   // год
                        };
                    break;
                    case 4:  // Системнi 
                        switch (numSet) {
                            case 0: if(newval[numSet]<0) newval[numSet]=0; break;   // Bluetooth
                            case 1: if(newval[numSet]<0) newval[numSet]=0; break;   // Iнше                        
                        }; 
                    break;
                  };
            break;
            case KEY_5: ILI9341_FillScreen(0, max_X, 0, max_Y, fillScreen);
                  ILI9341_WriteString(10,100,"ВИКОНУЮ ЗАПИС",Font_11x18,GREEN,fillScreen,2);
                  switch (numMenu){
                    case 0: spT[numSet] = newval[numSet]; break;                                          //"Температура","Верхня межа","Нижня межа"                    
                    case 3: for (byte=0;byte<2;byte++) clock_buffer[byte+1] = ByteToBcd2(newval[byte]);   // "Час Дата"
                            for (byte=2;byte<5;byte++) clock_buffer[byte+2] = ByteToBcd2(newval[byte]);
                            clock_buffer[0]=0;  
                            Clock_Ok=write_TWI(DS_SRTC,0,clock_buffer,7);  
                    break;
                    case 4: spT[numSet+3] = newval[numSet]; break;                                        // "Bluetooth","Iнше" 
                  }
                  delay_ms(500);
                  displ_num = 4; newSetButt = 1; 
            break;
          }
       break;
       //----------------------------- Список установок ступеней -----------------------
       case 6:  // displ_num
          switch (key){
            case KEY_6: displ_num=4; newSetButt=1; subMenu=0; break;
            case KEY_1: if (--subMenu<0){displ_num=4; newSetButt=1; subMenu = 0;} break;
            case KEY_2: if (++subMenu>LIST1-1) subMenu = LIST1-1;  break;            
            case KEY_3: if (--numSet<0) numSet = 0;  break;
            case KEY_7: if (++numSet>3) numSet = 3;  break;
            case KEY_5: 
                displ_num = 8; newSetButt = 1; 
                newval[numSet] = digital[numSet][subMenu];
            break;
          }
       break;
       //---------------------------- Список установо функций --------------------------
       case 7:  // displ_num
          switch (key){
            case KEY_6: displ_num=4; newSetButt=1; subMenu=0; break;
            case KEY_1: if (--subMenu<0){displ_num=4; newSetButt=1; subMenu = 0;} break;
            case KEY_2: if (++subMenu>LIST2-1) subMenu = LIST2-1;  break;            
            case KEY_3: if (--numSet<0) numSet = 0;  break;
            case KEY_7: if (++numSet>3) numSet = 3;  break;
            case KEY_5: 
                displ_num = 9; newSetButt = 1; 
                newval[numSet] = analog[numSet][subMenu];
            break;
          }
       break;
       //- РЕДАКТИРОВАНИЕ ступеней -
       case 8:
          switch (key){
            case KEY_1: displ_num = 6; newSetButt = 1; break;
            case KEY_6: displ_num = 3; newSetButt = 1; break;
            case KEY_4: ++newval[numSet]; if(newval[numSet]>250) newval[numSet]=250; break; 
            case KEY_8: --newval[numSet]; if(newval[numSet]<0) newval[numSet]=0; break; 
            case KEY_5: ILI9341_FillScreen(0, max_X, 0, max_Y, fillScreen);
                    ILI9341_WriteString(10,100,"ВИКОНУЮ ЗАПИС",Font_11x18,GREEN,fillScreen,2);
                    digital[numSet][subMenu] = newval[numSet];
                    displ_num = 6; newSetButt = 1; delay_ms(500);                     
            break;
          }
       break;
       //-- Общий список модулей #1 - #4 --
       case 9:
          switch (key){
            case KEY_1: displ_num = 7; newSetButt = 1; break;
            case KEY_6: displ_num = 3; newSetButt = 1; break;
            case KEY_4: ++newval[numSet]; if(newval[numSet]>250) newval[numSet]=250; break; 
            case KEY_8: --newval[numSet]; if(newval[numSet]<0) newval[numSet]=0; break; 
            case KEY_5: ILI9341_FillScreen(0, max_X, 0, max_Y, fillScreen);
                    ILI9341_WriteString(10,100,"ВИКОНУЮ ЗАПИС",Font_11x18,GREEN,fillScreen,2);
                    analog[numSet][subMenu] = newval[numSet];
                    displ_num = 7; newSetButt = 1; delay_ms(500);                     
            break;
          }
       break; 
       default:
         switch (key){
            case KEY_1: if(--displ_num<0) displ_num=3; newSetButt=1; break;//  (белая 1)
            case KEY_2: if(++displ_num>3) displ_num=0; newSetButt=1; break;//  (белая 2)
            case KEY_3:  break;//  (белая 3)
            case KEY_4:  break;//  (желтая)            
            case KEY_5:  break;//  (зеленая)       
            case KEY_6:  break;//  (черная)           
            case KEY_7:  break;//  (красная)             
            case KEY_8:  break;//  (синяя)
            case KEY_7_4:  break;// (красная + зеленая)
            case KEY_7_8:  break;// (красная + синяя)
            case KEY_7_6:  break;// (красная + черная)
         };
     };     
  }
  keynum=0;
}