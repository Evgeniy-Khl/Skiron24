unsigned char UpdatePI(signed int error, char i)// i-> индекс iPart[i]; time-> период между замерами ошибки регулирования
{
float pPart, Kp;//
  Kp = (float) analog[i][1]/4;   // Пропорциональный    analog[i][1]=20/4=5
  pPart = (float) Kp * error;    // расчет пропорциональной части
  //---- функция ограничения pPart -----------------------------
  if(pPart <0) pPart = 0; else if(pPart > 100) pPart = 100; // функция ограничения ????? if(pPart <=0)
  error = pPart;                           // преобразование формата из float к signed int
  return error;
};

signed int LowPassF2(signed int pV,unsigned char i)
{
float val;
  val = A1*Told1[i]-A2*Told2[i]+A3*pV;
  Told2[i] = Told1[i];
  Told1[i] = val;
  return val;
}

void calcDj(signed int actualT){      // Расчетное задание ЦИФРОВЫХ выходов № 1,2,3,4,5,6
 signed int err;
 unsigned char i;
    for (i=0;i<4;i++){
     err = (spT[0] + digital[i][0]) - actualT;              // digital[x][0]-> dТ смещение температуры;
     if (err > digital[i][1]){relOut[i]=OFF; relayOut &= ~(1<<(i+4));}    // отключить digital[x][1]-> ГИСТЕРЕЗИС
     if (err < 0) {relOut[i]=ON; relayOut |= (1<<(i+4));}                 // включить    timerDj[i]
    };
}

void calcAn(signed int actualT)      // Расчетное задание АНАЛОГОВЫХ выходов № 1,2,3,4
{
 unsigned char i;
 signed int err; 
    for (i=0;i<4;i++){     
     if(i==3){                                          // Положение клапана горячей воды. 
        err = (spT[2] + analog[i][0]) - actualT;        // spT[2] -> Нижня межа
     }
     else err = actualT - (spT[0] + analog[i][0]);      // analog[i][0]-> dТ смещение температуры;;
     if(err>0){  
         err = UpdatePI(err,i);                         // расчет управляющего напряжения
         if(err>analog[i][3]) err = analog[i][3];
         if(err<analog[i][2]) err = analog[i][2];
     }
     else err=0;
     analogOut[i] =  LowPassF2(err, i);
    };
}

void control(signed int actualT){                           
    calcAn(actualT);
    calcDj(actualT);
    if((errors&0x0F)==0){
        if(spT[1] - actualT < 0) errors |= 0x80;   // Температура ВЫСОКАЯ
        if(actualT - spT[2] < 0) errors |= 0x40;   // Температура НИЗКАЯ
    }
}