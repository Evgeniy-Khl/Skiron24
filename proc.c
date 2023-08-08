#define TUNING    170

void temperature_check(void){
 int val;
 unsigned char item, byte, crc, try=0;
    for (item=0; item < ds18b20;){
        w1_init();                 // 1 Wire Bus initialization
        w1_write(0x55);            // Load MATCH ROM [55H] comand
        ptr_char = &familycode[item][0];
        for (byte=0; byte < 8; byte++) w1_write(*ptr_char++);
        ptr_char = ds.buffer;
        w1_write(0xBE);            // Read Scratchpad command [BE]
        for (byte=0; byte < 8; byte++) *ptr_char++ = w1_read(); // Read cont. byte
        crc = w1_read();           // Read CRC byte
        ptr_char = ds.buffer;
        if (w1_dow_crc8(ptr_char, 8)==crc){
            try = 0; val = ds.pvT;
            if (val<0) {val=-val; val = val*10/16; val=-val;} // перевод в десятичное значение !!
            else val = val*10/16; // перевод в десятичное значение !!
            //----- Коректировка датчика DS18B20 ----------
            if(ds.buffer[2]==TUNING) val +=(signed char)ds.buffer[3]; // корекция показаний датчика
        }
        else if (++try > 2) {val = 1990; try = 0; errors |= (1<<item);}// (199.0) если ошибка более X раз то больше не опрашиваем     
        t[item] = val; 
        if (try==0) item++;
    }
    w1_init();                    // 1 Wire Bus initialization
    w1_write(0xCC);               // Load Skip ROM [CCH] command
    w1_write(0x44);               // Load Convert T [44H] command
}

signed int mean(char item){
unsigned char i, x=0;
signed int tt=0;
 for (i=0; i<item; i++){if(t[i] < 850) {tt += t[i]; x++;}};
 if(x) tt /= x; else {tt = 1900; errors |= 0x08;}  // 0x08 - ошибка датчиков среднего значения внутр. воздуха
 return tt;
}

void setDAC(void){ // V = Vref x (255/256)
  unsigned char i, val;
    for (i=0; i<4; i++){
        switch (i){
            case 0: CSDAC1 = 0; break;
            case 1: CSDAC2 = 0; break;
            case 2: CSDAC3 = 0; break;
            case 3: CSDAC4 = 0; break;
        }
        SPCR = SPI_MOUD_FL;// SPI port enabled
        val = dacU[i]>>4;
        SPDR = val & 0x0F; // Load Register to DAC
        while (!(SPSR & (1<<SPIF)));     // ожидаем конца передачи по SPI Opcode
        val = dacU[i]<<4;
        SPDR = val & 0xF0;
        while (!(SPSR & (1<<SPIF)));     // ожидаем конца передачи по SPI Opcode
        PORTD|=0xF0;       // INSTRUCTION EXECUTED
    };
    SPCR = 0;             //disable SPI
}

unsigned char adapt(unsigned char val){
    val <<= 1; val += ZERO;
    return val;
}

// - Converts from 2 digit BCD to Binary. -----------
unsigned char rtcTodec(unsigned char rtc){
  unsigned char res;
    res = (rtc>>4)*10;
    res += (rtc&0x0F);
    return res;
}

//- Converts a 2 digit decimal to BCD format. ---------
char ByteToBcd2(char Value){
 char bcdhigh = 0;
  while (Value >= 10){
    bcdhigh++;
    Value -= 10;
  }
  return ((bcdhigh << 4U) | Value);
}

unsigned char calcRtc(unsigned char rtc, signed char val){
  unsigned char res;
    res = rtcTodec(rtc);
    res += val;
    res = ByteToBcd2(res);
    return res;
}
