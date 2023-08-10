unsigned char pointY, txt[20];

void fraction(signed int t){
  if (t<0) {t = -t; signchar = '-';} else signchar = ' ';
  frcval = t%10; intval = t/10;
}

//-------------------------------- ����� ��������� ����������. ---------------------------------------
void displ_0(void){
 unsigned char i, keystop=0;
 unsigned int fillWindow = BLUE1, bordWindow = BLACK, temp;
//    cnt++;
    if(newSetButt){newSetButt=0; keystop=1; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);}// ������� ������
//-------------------------- ��������� ����� --------------------------
    pointY = 3;
    if(Clock_Ok){
//        if(cnt&1) temp=WHITE; else temp=fillWindow;
//        ILI9341_FillRectangle(15,pointY+2,40,15,temp);
        read_TWI(DS_SRTC,0,clock_buffer,7);// ������ ������ ������� ����������
        sprintf(buff,"%02x:%02x   %02x.%02x.20%02x",clock_buffer[2],clock_buffer[1],clock_buffer[4],clock_buffer[5],clock_buffer[6]);//���:��� ����.���.��� 
        ILI9341_WriteString(70,pointY,buff,Font_11x18,BLACK,fillWindow,1);
    }
    else {
        sprintf(buff,"������� �����!"); 
        ILI9341_WriteString(70,pointY,buff,Font_11x18,YELLOW,RED,1); ILI9341_WriteString(70,pointY,buff,Font_11x18,RED,YELLOW,1);// "������� ������"
    }
    if(keynum&&!keystop){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************
    pointY += 22;
//--- ��������� t ������� -----
    temp = spT[0]; fraction(temp);     // �������� ����� �����������
    sprintf(buff,"[%2u.%u]",intval,frcval);   // ������� T ���������� � ���������� ������
    ILI9341_WriteString(5,pointY+5,buff,Font_11x18,bordWindow,fillWindow,1);
    ILI9341_WriteString(5,pointY+25,"�����������",Font_11x18,bordWindow,fillWindow,1);
    if (pvT>1250) sprintf(buff,"**.*");
    else {
        temp = pvT; fraction(temp);     // �������� ����� �����������
        sprintf(buff,"%2u.%u",intval,frcval); // T �������� ���������� � ���������� ������ 
    }
    if(errors&0xC0) bordWindow = RED;   // ����������� ������� / ������
    ILI9341_WriteString(140,pointY,buff,Font_11x18,bordWindow,fillWindow,3);
    bordWindow = BLACK;
    if(keynum&&!keystop){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************
    pointY += 52;
//--- 
    temp = t[2]; fraction(temp);     // �������� ����� ����������� 
    ILI9341_WriteString(5,pointY, "��������",Font_11x18,bordWindow,fillWindow,1);
    ILI9341_WriteString(5,pointY+16,"�����������",Font_11x18,bordWindow,fillWindow,1);    
    if(temp<1250) sprintf(buff,"%2u.%u",intval,frcval); else sprintf(buff,"**.*");
    ILI9341_WriteString(142,pointY,buff,Font_11x18,bordWindow,fillWindow,2); 
    if(keynum&&!keystop){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************
// --- ��������� ������� -------
    pointY += 43;
    ILI9341_WriteString(70,pointY,"���� ����������",Font_11x18,bordWindow,fillWindow,1);
    ILI9341_FillScreen(0, max_X,135, max_Y, fillWindow);
    if(errors){
        BeepT=100;
        if(errors&0x03){pointY += 18;      ILI9341_WriteString(5,pointY,"������� ���������� ��������",Font_11x18,RED,fillWindow,1);}
        else if(errors&0x04){pointY += 18; ILI9341_WriteString(5,pointY,"������� ���������� �������",Font_11x18,RED,fillWindow,1);}
        else if(errors&0xC0){pointY += 18; ILI9341_WriteString(5,pointY,"������ ��������� ����������",Font_11x18,RED,fillWindow,1);}           
        if(noAutoRel){
            pointY += 18;
            sprintf(buff,"R-");
            sprintf(txt,"������: %u � ������� ������",noAutoRel);
            strcat(buff,txt);
            ILI9341_WriteString(5,pointY,buff,Font_11x18,RED,fillWindow,1);
        }
        if(noAutoAna){
            pointY += 18;
            sprintf(buff,"U-");
            sprintf(txt,"������: %u � ������� ������",noAutoAna);
            strcat(buff,txt);
            ILI9341_WriteString(5,pointY,buff,Font_11x18,RED,fillWindow,1);
        }
                         
    }
    else {pointY += 25; ILI9341_WriteString(80,pointY,"������� �����",Font_11x18,bordWindow,fillWindow,1);}               
}

//-------------------------------- �������� ���� ��������. -------------------------------------------
void displ_1(void){
 unsigned char i, num = 1;
 signed int temp;
 unsigned int fillWindow = YELLOW2, bordWindow = BLACK;
    pointY = 7; //keynum=0;
    if(newSetButt){
        newSetButt=0; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
        ILI9341_WriteString(100,pointY,"���� ��������",Font_11x18,bordWindow,fillWindow,1);
    }
//--- ��������� t ������ ---
    pointY += 20;
    if(ds18b20){
        for (i=0;i<ds18b20;i++){
            sprintf(buff,"���� %u",i+1);
            ILI9341_WriteString(5,pointY+12,buff,Font_11x18,bordWindow,fillWindow,1);
            temp = t[i];
            if(temp>1250) sprintf(buff,"**.*");
            else {
                fraction(temp);     // �������� ����� �����������
                sprintf(buff,"%2u.%u",intval,frcval); // T �������� ���������� � ���������� ������ 
            }
            ILI9341_WriteString(90,pointY,buff,Font_11x18,bordWindow,fillWindow,2);
            pointY += 35;
        };
    } 
    if(keynum){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************
}

//-------------------------------- ��������� ������� -------------------------------------------------
void displ_2(void){
 signed char i, x, keystop=0;
 unsigned int fillWindow = GRAY1, bordWindow = BLACK, color_box; 
    pointY=7; //keynum=0;
    if(newSetButt){
        newSetButt=0; keystop=1; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
        ILI9341_WriteString(90,pointY,"���� ����������",Font_11x18,bordWindow,fillWindow,1);
    }
//---- �������� ������ ----
    pointY += 20;
    for (i=0;i<4;i++){
        sprintf(buff,"����  N%u: ",i+1);
        if(relaySet[i]<0) strcat(buff,"���"); else strcat(buff,"���");
        if(i == numSet){bordWindow = WHITE; fillWindow = BLACK;} else {bordWindow = BLACK; fillWindow = GRAY1;}
        ILI9341_WriteString(5,pointY,buff,Font_11x18,bordWindow,fillWindow,1);        
        if(relOut[i]) color_box=YELLOW; else color_box=BLACK;
        ILI9341_FillRectangle(160,pointY,40,18,color_box);
        pointY = pointY+22;
    }
    if(keynum&&!keystop){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************
//---- ���������� ������ ----
    for (i=0;i<4;i++){
        sprintf(buff,"����� N%u: ",i+1);
        if(analogSet[i]<0) strcat(buff,"���"); else {strcat(buff,"���"); analogOut[i]=analogSet[i];}
        sprintf(txt," %3u%% ",analogOut[i]);
//        sprintf(txt,"%7.4f %3u%%",iPart[i],analogOut[i]);
        strcat(buff,txt);
        if(i+4 == numSet){bordWindow = WHITE; fillWindow = BLACK;} else {bordWindow = BLACK; fillWindow = GRAY1;}
        ILI9341_WriteString(5,pointY,buff,Font_11x18,bordWindow,fillWindow,1);
        pointY = pointY+22;    
    }
    if(keynum&&!keystop){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************
}

//-------------------------------- ��������� ������ ���� ---------------------------------------------
void displ_3(void){
 char item;
 unsigned int fillWindow = GREEN1, bordWindow = BLACK, color_txt = BLACK, color_fon = GREEN1; 
  pointY=7; //keynum=0;
  if(newSetButt){
    newSetButt=0; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
    ILI9341_WriteString(50,pointY,"������������ �������",Font_11x18,bordWindow,fillWindow,1);
  }
  pointY += 25;
  //---
  for (item = 0; item < MENU; item++){
    sprintf(buff,"%s",setMenu[item]);   //------ "������i","����i","�����i�","�������i" -------- (numMenu)
    if(item == numMenu){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
    ILI9341_WriteString(20,pointY,buff,Font_11x18,color_txt,color_fon,1);    
    pointY += 25;
  }
  if(keynum){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************
}

//-------------------------------- ��� ������� -------------------------------------------------------
void displ_4(void){
 char item, keystop=0, tmpv0, tmpv1;
 unsigned int fillWindow = GREEN1, bordWindow = BLACK, color_txt = BLACK, color_fon = GREEN1, temp;
  pointY=7; //keynum=0;
  if(newSetButt){
    newSetButt=0; keystop=1; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
    sprintf(buff,"%s",setMenu[numMenu]);
    ILI9341_WriteString(80,pointY,buff,Font_11x18,bordWindow,fillWindow,1);
  }
  pointY += 25;
  //---
  switch (numMenu){
    //--------------------- ������� "������i" -------------------------------------
    case 0: 
        for (item = 0; item < LIST0; item++){
            temp = spT[item]; tmpv0 = temp%10; tmpv1 = temp/10;
            sprintf(buff,"%11s = %2u.%u", setName0[item],tmpv1,tmpv0); //------ "�����������","������ ����","����� ����" -------- (subMenu)   
            if(item == numSet){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
            ILI9341_WriteString(20,pointY,buff,Font_11x18,color_txt,color_fon,1);    
            pointY += 25;
        }
    break;
    //--------------------- ������� "����" ---------------------------------------
    case 1: 
        for (item = 0; item < LIST1; item++){
            sprintf(buff,"%s",setName1[item]);   //------ "��i�����", "�i��������" -------- (subMenu)
            if(item == subMenu){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
            ILI9341_WriteString(20,pointY,buff,Font_11x18,color_txt,color_fon,1);    
            pointY += 25;
        }
    break;
    //--------------------- ������� "�������" -------------------------------------
    case 2: 
        for (item = 0; item < LIST2; item++){
            sprintf(buff,"%s",setName2[item]);   //------ "��i�����","���.1","���.2","MIN","MAX" -------- (subMenu)
            if(item == subMenu){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
            ILI9341_WriteString(20,pointY,buff,Font_11x18,color_txt,color_fon,1);    
            pointY += 25;
        }
    break;
    //--------------------- ������� "��� ����" ------------------------------------
    case 3: 
        for (item = 0; item < LIST3;item++){
            if (item<2) tmpv0 = clock_buffer[item+1];
            else tmpv0 = clock_buffer[item+2];   //------ "�������","������","����","�i���","�i�" -------- (subMenu)
            sprintf(buff,"%8s = %x", setName3[item],tmpv0);
            if (item == numSet){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
            ILI9341_WriteString(5,pointY,buff,Font_11x18,color_txt,color_fon,1);            
            pointY += 25;
        }
    break;
    //--------------------- ������� "�������" ------------------------------------
    case 4: 
        for (item = 0; item < LIST4;item++){
            sprintf(buff,"%9s = %2u", setName4[item],spT[item+3]);
            if (item == numSet){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
            ILI9341_WriteString(5,pointY,buff,Font_11x18,color_txt,color_fon,1);
            pointY += 25;
        }
    break;    
  };
  if(keynum&&!keystop){checkkey(keynum); return;}//***************************** �������� ������� ������ ***************************************   
}

//-------------------------------- �������������� ----------------------------------------------------
void displ_5(void){
 char tmpv0, tmpv1;
 unsigned int fillWindow = GREEN1, bordWindow = BLACK, temp;
    pointY=7;
    if(newSetButt){
        ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
//        pauseEdit = 3;
        sprintf(buff,"����������� %s", setMenu[numMenu]);
        ILI9341_WriteString(20,pointY,buff,Font_11x18,bordWindow,fillWindow,1); 
    }
    pointY = pointY+50;
    switch (numMenu){
        case 0: //-------- "�����������" ---------
            if(newSetButt){
                sprintf(buff,"%12s",setName0[numSet]);  // "�����������","������ ����","����� ����"
                ILI9341_WriteString(5,pointY,buff,Font_11x18,bordWindow,fillWindow,2);
            }
            temp = newval[numSet]; tmpv0 = temp%10; tmpv1 = temp/10;
            sprintf(buff,"%3u.%u",tmpv1,tmpv0);     // T � ���������� ������
            ILI9341_WriteString(80,pointY+40,buff,Font_11x18,bordWindow,fillWindow,2);    
        break;
        case 3: //-------- "��� ����" ------------
            sprintf(buff,"%7s:%02u",setName3[numSet],newval[numSet]);  // "�������","������","����","�i���","�i�"
            ILI9341_WriteString(5,pointY,buff,Font_11x18,bordWindow,fillWindow,2);
        break;
        case 4: //-------- "�������" -------------- 
            sprintf(buff,"%9s:%u",setName4[numSet],newval[numSet]);  // "Bluetooth","I���"
            ILI9341_WriteString(5,pointY,buff,Font_11x18,bordWindow,fillWindow,2);
        break;
    }
   newSetButt=0;
//   if(--pauseEdit==0){pauseEdit=1; if(keynum) checkkey(keynum);}//***************************** �������� ������� ������ ***************************************
   if(keynum) checkkey(keynum);//***************************** �������� ������� ������ ***************************************
}

//-------------------------------- ��������� �������� ** numMenu=1; subMenu=0->"��i�����", subMenu=1->"�i��������"--
void displ_6(void){
 char item, tmpv0, tmpv1;
 unsigned int fillWindow = GREEN1, bordWindow = BLACK, color_txt = BLACK, color_fon = GREEN1, temp; 
    pointY=7;
    if (newSetButt){
        newSetButt=0; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
        ILI9341_WriteString(20,pointY,"������������ ������",Font_11x18,bordWindow,fillWindow,1);
    }
    pointY += 35;
    for (item = 0; item < 4; item++){        
        temp = digital[item][subMenu]; tmpv0 = temp%10; tmpv1 = temp/10;
        sprintf(buff,"%8s R%u = %2u.%u",setName1[subMenu],item+1,tmpv1,tmpv0);
        if(item == numSet){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
        ILI9341_WriteString(20,pointY,buff,Font_11x18,color_txt,color_fon,1);    
        pointY += 25;
    }
  if(keynum) checkkey(keynum);//***************************** �������� ������� ������ ***************************************
}

//-------------------------------- ��������� �������  ** numMenu=1; subMenu=0->"��i�����", subMenu=1->"���.1"...  --
void displ_7(void){
 char item, tmpv0, tmpv1;
 unsigned int fillWindow = GREEN1, bordWindow = BLACK, color_txt = BLACK, color_fon = GREEN1, temp;
    pointY=7;
    if(newSetButt){
        newSetButt=0; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
        ILI9341_WriteString(20,pointY,"������������ �������",Font_11x18,bordWindow,fillWindow,1);
    }
    pointY += 25;
    for (item = 0; item < 4; item++){        
        if(subMenu){
            temp = analog[item][subMenu];
            sprintf(buff,"%8s U%u = %3u ",setName2[subMenu],item+1,temp);
        }
        else {
            temp = analog[item][subMenu]; tmpv0 = temp%10; tmpv1 = temp/10;
            sprintf(buff,"%8s U%u = %2u.%u",setName1[subMenu],item+1,tmpv1,tmpv0);
        }
        if(item == numSet){color_txt = WHITE; color_fon = BLACK;} else {color_txt = BLACK; color_fon = GREEN1;}
        ILI9341_WriteString(20,pointY,buff,Font_11x18,color_txt,color_fon,1);    
        pointY += 25;
    }
   if(keynum) checkkey(keynum);//***************************** �������� ������� ������ ***************************************   
}

//-------------------- �������������� ��������� �������� ** numMenu=1; subMenu=0->"��i�����", subMenu=1->"�i��������"--
void displ_8(void){
 char tmpv0, tmpv1;
 unsigned int fillWindow = GREEN1, bordWindow = BLACK, temp;
  pointY=7;
  if (newSetButt){
    newSetButt=0; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
//    pauseEdit = 3;
    sprintf(buff,"����������� %s T%u",setName1[subMenu],numSet+1);
    ILI9341_WriteString(10,pointY,buff,Font_11x18,bordWindow,fillWindow,1); 
  }
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    pointY = pointY+50;
    temp = newval[numSet]; tmpv0 = temp%10; tmpv1 = temp/10;
    sprintf(buff,"%s:%2u.%u",setName1[subMenu],tmpv1,tmpv0);
    ILI9341_WriteString(5,pointY,buff,Font_11x18,bordWindow,fillWindow,2);
//    if(--pauseEdit==0){pauseEdit=1; if(keynum) checkkey(keynum);}//***************************** �������� ������� ������ ***************************************
   if(keynum) checkkey(keynum);//***************************** �������� ������� ������ ***************************************
}

//-------------------- �������������� ��������� �������  ** numMenu=1; subMenu=0->"��i�����", subMenu=1->"���.1"...  --
void displ_9(void){
 char item, tmpv0, tmpv1;
 unsigned int fillWindow = GREEN1, bordWindow = BLACK, color_txt = BLACK, color_fon = GREEN1, temp; 
    pointY=7;
    if (newSetButt){
        newSetButt=0; ILI9341_FillScreen(0, max_X, 0, max_Y, fillWindow);
        sprintf(buff,"����������� %s T%u",setName2[subMenu],numSet+1);
        ILI9341_WriteString(10,pointY,buff,Font_11x18,bordWindow,fillWindow,1);
    }
    pointY = pointY+50;
    if(subMenu){
        temp = newval[numSet];
        sprintf(buff,"%s:%3u",setName2[subMenu],temp);
    }
    else {
        temp = newval[numSet]; tmpv0 = temp%10; tmpv1 = temp/10;
        sprintf(buff,"%s:%2u.%u",setName1[subMenu],tmpv1,tmpv0);
    }
    ILI9341_WriteString(5,pointY,buff,Font_11x18,bordWindow,fillWindow,2);
    if(keynum) checkkey(keynum);//***************************** �������� ������� ������ ***************************************
}

void display(void){
 switch (displ_num){
    case 0: displ_0(); break;  // ����� ��������� ����������.
    case 1: displ_1(); break;  // �������� ���� ��������.
    case 2: displ_2(); break;  // ��������� �������
    case 3: displ_3(); break;  // ��������� ������ ����
    case 4: displ_4(); break;  // ��� �������
    case 5: displ_5(); break;  // ��������������
    case 6: displ_6(); break;  // ��������� ��������
    case 7: displ_7(); break;  // ��������� �������
    case 8: displ_8(); break;  // �������������� ��������� ��������
    case 9: displ_9(); break;  // �������������� ��������� �������
    default: displ_0(); break; // ����� ��������� ����������.
  }
}

