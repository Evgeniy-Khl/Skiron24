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
     Display=1;
     switch (displ_num){
       //---- ������ ----
       case 2:
         max = 8;
         switch (key){
            case KEY_1: if(--displ_num<0) displ_num=3; newSetButt=1; break;//  (����� 1)
            case KEY_2: if(++displ_num>3) displ_num=0; newSetButt=1; break;//  (����� 2)
            case KEY_3: if(--numSet<0) numSet = 0;     break;
            case KEY_7: if(++numSet>max) numSet = max; break;
            case KEY_4: 
                if(numSet<4){relaySet[numSet]++; if(relaySet[numSet]>2) relaySet[numSet]=0;}
                else {analogSet[numSet]+=10; if(analogSet[numSet]>100) analogSet[numSet]=100;} 
            break;
            case KEY_8: 
                if(numSet<4){relaySet[numSet]--; if(relaySet[numSet]<0) relaySet[numSet]=2;}
                else {analogSet[numSet]-=10; if(analogSet[numSet]<0) analogSet[numSet]=-1;}
            break;
         }
       break;
       //- ����� ������ ��������� -
       case 3:
         switch (key){
            case KEY_1: displ_num = 0; newSetButt = 1; break;
            case KEY_3: if(--numMenu<0) numMenu = MENU0-1;  break;
            case KEY_7: if(++numMenu>MENU0-1) numMenu = 0; break;
            case KEY_5: newSetButt = 1; // (�������)
                    switch (numMenu){
                        case 5:  displ_num=6; numMenu = 0; break;
                        case 6:  displ_num=9; numMenu = 0; break;   
                        default: displ_num=4;
                    };  
            break;
         }
       break;
       //- ��������� ������� -
        case 4:
        if(numMenu<2) max=6; else if(numMenu==4) max=4;
          switch (key){
            case KEY_1: displ_num = 3; newSetButt = 1;  break;
            case KEY_3: if(--numSet<0) numSet = 0;     break;
            case KEY_7: if(++numSet>max) numSet = max; break;
            case KEY_5:  // (�������) 
              switch (numMenu){
                case 0: for (byte=0;byte<MENU1;byte++) newval[byte] = set[0][byte]; break; // "�����������"
                case 1: for (byte=0;byte<MENU1;byte++) newval[byte] = set[1][byte]; break; // "���������"
                case 2: for (byte=0;byte<MAX_6;byte++) newval[byte] = set[4][byte]; break; // "������" 
                case 3: for (byte=0;byte<MAX_6;byte++) newval[byte] = rtcTodec(set[5][byte]); break; // "���� ����"
                case 4: for (byte=0;byte<2;byte++) newval[byte] = rtcTodec(clock_buffer[byte+1]); 
                        for (byte=2;byte<5;byte++) newval[byte] = rtcTodec(clock_buffer[byte+2]);
                    break;     // "����� � ����"
              };
              displ_num = 5; newSetButt = 1; 
            break;
          }
        break;
       //- �������������� -
        case 5:
          switch (key){
            case KEY_1: displ_num = 4; newSetButt = 1; break;
            case KEY_6: displ_num = 4; newSetButt = 1; break;
            case KEY_3: ++newval[numSet];    
                 switch (numMenu) {
                    case 0:  // ����������� 
                        switch (numSet) {
                            case 0: if(newval[numSet]>1200) newval[numSet]=1200; break; // ����
                            case 1: if(newval[numSet]>1200) newval[numSet]=1200; break; // ����
                            case 2: if(newval[numSet]>500) newval[numSet]=500; break;   // ����������
                            case 3: if(newval[numSet]>100) newval[numSet]=100; break;   // ����������
                            case 4: if(newval[numSet]>1)  newval[numSet]=1;    break;   // �����
                            case 5: if(newval[numSet]>0) newval[numSet]=0;     break;   // ������
                            case 6: if(newval[numSet]>0)  newval[numSet]=0;    break;   // �����
                        }; 
                    break;
                    case 1:  // ��������� 
                        switch (numSet) {
                            case 0: if(newval[numSet]>100) newval[numSet]=100; break; // ����
                            case 1: if(newval[numSet]>100) newval[numSet]=100; break; // ����
                            case 2: if(newval[numSet]>90) newval[numSet]=90; break;   // ����������
                            case 3: if(newval[numSet]>50) newval[numSet]=50; break;   // ����������
                            case 4: if(newval[numSet]>1)  newval[numSet]=1;  break;   // �����
                            case 5: if(newval[numSet]>1) newval[numSet]=1;   break;   // ������ DHT11/22
                            case 6: if(newval[numSet]>1)  newval[numSet]=1;  break;   // �����
                        };
                    break;
                    case 2:  // ������ 
                        switch (numSet) {
                            case 0: if(newval[numSet]>1200) newval[numSet]=1200; break;  // �������
                            case 1: if(newval[numSet]>1) newval[numSet]=1; break;        // �����������
                            case 2: if(newval[numSet]>1200) newval[numSet]=1200; break;  // ��������
                            case 3: if(newval[numSet]>1) newval[numSet]=1; break;        // �����������
                            case 4: if(newval[numSet]>14) newval[numSet]=14; break;      // ���
                            case 5: if(newval[numSet]>14) newval[numSet]=14; break;      // ��������
                        };
                    break;
                    case 3:  // ���� ���� 
                        switch (numSet) {
                            case 0: if(newval[numSet]>12) newval[numSet]=12; break;   // DayBeg
                            case 1: if(newval[numSet]>23) newval[numSet]=23; break;   // DayEnd
                            case 2: if(newval[numSet]>12) newval[numSet]=12; break;   // Light0Beg
                            case 3: if(newval[numSet]>12) newval[numSet]=12; break;   // Light0End
                            case 4: if(newval[numSet]>23) newval[numSet]=23; break;   // Light1Beg
                            case 5: if(newval[numSet]>23) newval[numSet]=23; break;   // Light1End
                        };
                    break;
                    case 4:  // ����� � ���� 
                        switch (numSet) {
                            case 0: if(newval[numSet]>59) newval[numSet]=0;  break;   // ������
                            case 1: if(newval[numSet]>23) newval[numSet]=0;  break;   // ����
                            case 2: if(newval[numSet]>31) newval[numSet]=1;  break;   // ����
                            case 3: if(newval[numSet]>12) newval[numSet]=1;  break;   // �����
                            case 4: if(newval[numSet]>59) newval[numSet]=22; break;   // ���
                        };
                    break;
                  }; 
            break;
            case KEY_7: --newval[numSet];
                 switch (numMenu) {
                    case 0:  // ����������� 
                        switch (numSet) {
                            case 0: if(newval[numSet]<-500) newval[numSet]=-500; break;   // ����
                            case 1: if(newval[numSet]<-500) newval[numSet]=-500; break;   // ����
                            case 2: if(newval[numSet]<1) newval[numSet]=1;       break;   // ����������
                            case 3: if(newval[numSet]<1) newval[numSet]=1;       break;   // ����������
                            case 4: if(newval[numSet]<0) newval[numSet]=0;       break;   // �����
                            case 5: if(newval[numSet]<-1) newval[numSet]=-1;     break;   // ������
                            case 6: if(newval[numSet]<0)  newval[numSet]=0;      break;   // �����                            
                        }; 
                    break;
                    case 1:  // ��������� 
                        switch (numSet) {
                            case 0: if(newval[numSet]<10) newval[numSet]=10; break;   // ����
                            case 1: if(newval[numSet]<10) newval[numSet]=10; break;   // ����
                            case 2: if(newval[numSet]<1) newval[numSet]=1;   break;   // ����������
                            case 3: if(newval[numSet]<1) newval[numSet]=1;   break;   // ����������
                            case 4: if(newval[numSet]<0) newval[numSet]=0;   break;   // �����
                            case 5: if(newval[numSet]<0) newval[numSet]=0;   break;   // ������ DHT11/22
                            case 6: if(newval[numSet]<1)  newval[numSet]=1;  break;   // �����     
                        };
                    break;
                    case 2:  // ������ 
                        switch (numSet) {
                            case 0: if(newval[numSet]<1) newval[numSet]=1; break;   // �������
                            case 1: if(newval[numSet]<0) newval[numSet]=1; break;   // �����������
                            case 2: if(newval[numSet]<1) newval[numSet]=1; break;   // ��������
                            case 3: if(newval[numSet]<0) newval[numSet]=1; break;   // �����������
                            case 4: if(newval[numSet]<0) newval[numSet]=0; break;   // ���
                            case 5: if(newval[numSet]<0) newval[numSet]=0; break;   // ��������
                        };
                    break;
                    case 3:  // ���� ���� 
                        switch (numSet) {
                            case 0: if(newval[numSet]<0) newval[numSet]=12; break;   // DayBeg
                            case 1: if(newval[numSet]<0) newval[numSet]=23; break;   // DayEnd
                            case 2: if(newval[numSet]<0) newval[numSet]=12; break;   // Light0Beg
                            case 3: if(newval[numSet]<0) newval[numSet]=12; break;   // Light0End
                            case 4: if(newval[numSet]<0) newval[numSet]=23; break;   // Light1Beg
                            case 5: if(newval[numSet]<0) newval[numSet]=23; break;   // Light1End
                        };
                    break;
                    case 4:  // ����� � ���� 
                        switch (numSet) {
                            case 0: if(newval[numSet]<0)  newval[numSet]=59; break;   // ������
                            case 1: if(newval[numSet]<0)  newval[numSet]=23; break;   // ����
                            case 2: if(newval[numSet]<1)  newval[numSet]=31; break;   // ����
                            case 3: if(newval[numSet]<1)  newval[numSet]=12; break;   // �����
                            case 4: if(newval[numSet]<22) newval[numSet]=59; break;   // ���
                        };
                    break;
                  };
            break;
            case KEY_5: ILI9341_FillScreen(0, max_X, 0, max_Y, fillScreen);
                  ILI9341_WriteString(10,100,"������� �����",Font_11x18,GREEN,fillScreen,2);
                  switch (numMenu){
                    case 0: set[0][numSet] = newval[numSet]; break; // "�����������"
                    case 1: set[1][numSet] = newval[numSet]; break; // "���������"
                    case 2: set[4][numSet] = newval[numSet]; break; // "������"
                    case 3: set[5][numSet] = ByteToBcd2(newval[numSet]); break; // "���� ����"
                    case 4: for (byte=0;byte<2;byte++) clock_buffer[byte+1] = ByteToBcd2(newval[byte]);
                            for (byte=2;byte<5;byte++) clock_buffer[byte+2] = ByteToBcd2(newval[byte]);
                            clock_buffer[0]=0;  
                            Clock_Ok=write_TWI(DS_SRTC,0,clock_buffer,7);  break; // "����� � ����"
                  }
                  delay_ms(500);
                  displ_num = 4; newSetButt = 1; break;
          }
        break;
       //-- ����� ������ ������������� #1 - #5 --
       case 6:
          switch (key){
            case KEY_1: displ_num = 3; newSetButt = 1; break;
            case KEY_3: if (--numMenu<0) numMenu = 0;  break;
            case KEY_7: if (++numMenu>4) numMenu = 4;  break;
            case KEY_5: displ_num = 7; newSetButt = 1; moduleEdit = 0; numSet = 0; break;
          }
        break;
       //- ��������� ��������� �������� ������������� -
        case 7:
          switch (key){
            case KEY_1: if(moduleEdit) displ_num = 9; else displ_num = 6; newSetButt = 1; break;  // ������ ��� ������������
            case KEY_3: if (--numSet<0) numSet = 3;    break;
            case KEY_7: if (++numSet>3) numSet = 0;    break;
            case KEY_5: for (byte=0;byte<2;byte++){
                        if(moduleEdit) newval[byte] = module[numMenu][byte];
                        else newval[byte] = limit[numMenu][byte];
                    }
                    displ_num = 8; newSetButt = 1; 
            break;
          }
        break;
       //- �������������� ������������� -
        case 8:
          switch (key){
            case KEY_1: displ_num = 7; newSetButt = 1; break;
            case KEY_6: displ_num = 7; newSetButt = 1; break;
            case KEY_3: ++newval[numSet];    
                switch (numSet) {
                    case 0: if(newval[numSet]>1000) newval[numSet]=1000; break; // MIN
                    case 1: if(newval[numSet]>1000) newval[numSet]=1000; break; // MAX
                    case 2: if(newval[numSet]>1000) newval[numSet]=1000; break; // ���.1
                    case 3: if(newval[numSet]>1000) newval[numSet]=1000; break; // ���.2
                };
            break; 
            case KEY_7: --newval[numSet];
                switch (numSet) {
                    case 0: if(newval[numSet]<0) newval[numSet]=0; break;   // MIN
                    case 1: if(newval[numSet]<0) newval[numSet]=0; break;   // MAX
                    case 2: if(newval[numSet]<1) newval[numSet]=1; break;   // ���.1
                    case 3: if(newval[numSet]<10) newval[numSet]=10; break; // ���.2                            
                };
            break; 
            case KEY_5: ILI9341_FillScreen(0, max_X, 0, max_Y, fillScreen);
                    ILI9341_WriteString(10,100,"������� �����",Font_11x18,GREEN,fillScreen,2);
                    if(moduleEdit){
                        module[numMenu][numSet] = newval[numSet];
                        displ_num = 9;
                    }
                    else {
                        limit[numMenu][numSet] = newval[numSet];
                        displ_num = 7;
                    }
                    newSetButt = 1; delay_ms(500);                     
            break;
          }
        break;
       //-- ����� ������ ������� #1 - #4 --
        case 9:
          switch (key){
            case KEY_1: displ_num = 3; newSetButt = 1; break;
            case KEY_3: if (--numMenu<0) numMenu = 0;  break;
            case KEY_7: if (++numMenu>3) numMenu = 3;  break;
            case KEY_5: displ_num = 7; newSetButt = 1; moduleEdit = 1; numSet = 0; break;
          }
        break; 
       default:
         switch (key){
            case KEY_1: if(--displ_num<0) displ_num=3; newSetButt=1; break;//  (����� 1)
            case KEY_2: if(++displ_num>3) displ_num=0; newSetButt=1; break;//  (����� 2)
            case KEY_3:  break;//  (����� 3)
            case KEY_4:  break;//  (������)            
            case KEY_5:  break;//  (�������)       
            case KEY_6:  break;//  (������)           
            case KEY_7:  break;//  (�������)             
            case KEY_8:  break;//  (�����)
            case KEY_7_4:  break;// (������� + �������)
            case KEY_7_8:  break;// (������� + �����)
            case KEY_7_6:  break;// (������� + ������)
         };
     };     
  }
  keynum=0;
}