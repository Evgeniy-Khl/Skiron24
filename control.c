#define ON              1
#define OFF             0
#define UNCHANGED       2

// ch = 0 -> (����.) [0]Tday;  [1]Tnight;  [2]dTalarm;  [3]hysteresis;  [4]mode=1 -> ������ / mode=0 -> ����������; [5]����� � 0 ��� 4
// ch = 1 -> (����.) [0]RHday; [1]RHnight; [2]dRHalarm; [3]hysteresis;  [4]mode=1 -> ������ / mode=0 -> ��������;   [5]����� � 1 ��� 5
// ch = 2 -> (�����) [0]Tday;  [1]Tnight;  [2]dTalarm;  [3]hysteresis;  [4]mode=1 -> ������ / mode=0 -> ��������;   [5]����� � 4 ��� 0
// ch = 3 -> (�����) [0]RHday; [1]RHnight; [2]dTalarm;  [3]hysteresis;  [4]mode=1 -> ������ / mode=0 -> ��������;   [5]����� � 5 ��� 1
void RelaySensor(signed int val, unsigned char ch){
 char x=UNCHANGED, byte;                           
 signed int error;
    byte = set[ch][2];                       // ������� ������
    if(set[ch][4]) error = set[ch][Night]-val;  // mode=1(������/����������)   ������ �������������
    else error = val - set[ch][Night];         // mode=0(����������/��������) ������ �������������
    
    if(abs(error)<(byte/2)) ok |= (1<<ch);   // �������� ��������� ��������
    if(abs(error)>byte) byte |= (1<<ch); else byte = 0;// ������
    if(ok&byte) alarm[ch] = 1; else if(ok&(1<<ch)) alarm[ch] = 0; else alarm[ch] = 2;// ������ ��� ����� ��� ��� �� ����� �� �������� ��������

    if(error-set[ch][3] > 0) x = ON;     // ��������
    if(error < 0) x = OFF;               // ���������

    if(x<UNCHANGED) {
        byte = 1 << set[ch][6];              // � ����
        if(x) {portOut |= byte; relOut[ch] = 1;}
        else {portOut &= ~byte; relOut[ch] = 0;}
    }
}

unsigned char UpdatePI(signed int val, char i){// i-> ������ iPart[i]; time-> ������ ����� �������� ������ �������������
  signed int error;
  float pPart, Kp, Ki, Ud;
    Kp = (float) limit[i][2]/4;               // ����������������    limit[i][1]=20/4=5
    Ki = (float) limit[i][3]*10;             // ������������        limit[i][2]=200*10=2000
    if(set[i][4]) error = set[i][Night]-val;  // mode=1(������/����������) ������ �������������
    else error = val - set[i][Night];         // mode=0(����������/��������) ������ �������������
    pPart = (float) Kp * error;               // ������ ���������������� �����
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

