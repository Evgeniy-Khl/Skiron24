unsigned char UpdatePI(signed int error, char i)// i-> ������ iPart[i]; time-> ������ ����� �������� ������ �������������
{
float pPart, Kp, Ki, Ud;//
  Kp = (float) analog[i][1]/4;   // ����������������    analog[i][1]=20/4=5
  Ki = (float) analog[i][2]*100; // ������������        analog[i][2]=100*100=10000
  pPart = (float) Kp * error;    // ������ ���������������� �����
  //---- ������� ����������� pPart -----------------------------
  if(pPart <0){pPart = 0; iPart[i] = 0;} else if(pPart > 100) pPart = 100; // ������� ����������� ????? if(pPart <=0)
//--------------------------------------------------------------
  iPart[i] += (float) Kp / Ki  * error; // ���������� ������������ �����  
  Ud = pPart + iPart[i];                // ����� ���������� �� �����������
//------ ������� ����������� -----------------------------------
  if(Ud < 0) Ud = 0; else if(Ud > 100) Ud = 100;
  iPart[i] = Ud - pPart;                // "��������������" ��������
  error = Ud;                           // �������������� ������� �� float � signed int
  return error;
};

signed int LowPassF2(signed int t,unsigned char i,unsigned char n)
{
float val;
  val = A1[n]*Told1[i]-A2[n]*Told2[i]+A3[n]*t;
  Told2[i] = Told1[i];
  Told1[i] = val;
  return val;
}

void calcDj(signed int actualT){      // ��������� ������� �������� ������� � 1,2,3,4,5,6
 signed int err;
 unsigned char i;
    for (i=0;i<4;i++){
     err = (spT[0] + digital[i][0]) - actualT;              // digital[x][0]-> d� �������� �����������;
     if (err > digital[i][1]){relOut[i]=OFF; relayOut &= ~(1<<(i+4));}    // ��������� digital[x][1]-> ����������
     if (err < 0) {relOut[i]=ON; relayOut |= (1<<(i+4));}                 // ��������    timerDj[i]
    };
}

void calcAn(signed int actualT)      // ��������� ������� ���������� ������� � 1,2,3,4
{
 unsigned char i, xx;
 signed int err; 
    for (i=0;i<4;i++){     
     if(i==3){                                          // ��������� ������� ������� ����. 
        err = (spT[2] + analog[i][0]) - actualT;        // spT[2] -> ����� ����
     }
     else err = actualT - (spT[0] + analog[i][0]);      // analog[i][0]-> d� �������� �����������;
     xx = (1<<i);
     if(!(flags&xx) && err>0){flags |= xx; iPart[i]=0; err = UpdatePI(err,0);}
     else if(flags&xx) {flags &= ~xx; err=0;}           // ������� ��������
     if(xx>analog[i][4]) err = analog[i][4];
     if(xx<analog[i][3]) err = analog[i][3];
     dacU[i] = err;
    };
}

void control(signed int actualT){                           
    calcAn(actualT);
    calcDj(actualT);
    if((errors&0x0F)==0){
        if(spT[1] - actualT < 0) errors |= 0x80;   // ����������� �������
        if(actualT - spT[2] < 0) errors |= 0x40;   // ����������� ������
    }
}