byte noFix[8]= // Definizione del carattere NF, composto da 8 righe.
{B11001,      // I caratteri del display sono 5x7, ma sotto c'è un'altra riga per virgola o altro.
 B10101,
 B10011,
 B11110,
 B10000,
 B11100,
 B10000,
 B10000};

byte cerchietto[8]=
{B00100,     
 B01010,
 B00100};

byte spina[8]=
{B01010,
 B01010,
 B11111,
 B11111,
 B11111,
 B01110,
 B00100,
 B00100};

byte Batt_0[8]=
{10,17, 0,17, 0,17, 0,21};
byte Batt_1[8]=
{14,17,17,17,17,17,17,31};
byte Batt_2[8]=
{14,17,17,17,17,17,31,31};
byte Batt_3[8]=
{14,17,17,17,17,31,31,31};
byte Batt_4[8]=
{14,17,17,17,31,31,31,31};
byte Batt_5[8]=
{14,17,17,31,31,31,31,31};
byte Batt_6[8]=
{14,17,31,31,31,31,31,31};
byte Batt_7[8]=
{14,31,31,31,31,31,31,31};

// Frecce Nord-Sud-Ovest-Est grandi:
byte Nord[8]=
{0,4,14,31,4,4};
byte NordEst[8]=
{0,7,3,5,8,16};
byte Est[8]=
{0,4,6,31,6,4};
byte SudEst[8]=
{0,16,8,5,3,7};
byte Sud[8]=
{0,4,4,31,14,4};
byte SudOvest[8]=
{0,1,2,20,24,28};
byte Ovest[8]=
{0,4,12,31,12,4};
byte NordOvest[8]=
{0,28,24,20,2,1};

// Frecce Nord-Sud-Ovest-est piccole:
/*
byte Nord[8]=
{0,4,14,4,4,4};
byte NordEst[8]=
{0,7,3,5,8,16};
byte Est[8]=
{0,0,2,31,2,0};
byte SudEst[8]=
{0,16,8,5,3,7};
byte Sud[8]=
{0,4,4,4,14,4};
byte SudOvest[8]=
{0,1,2,20,24,28};
byte Ovest[8]=
{0,0,8,31,8,0};
byte NordOvest[8]=
{0,28,24,20,2,1};
*/
