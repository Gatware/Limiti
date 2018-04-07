// ------- Sceglie e disegna l'icona della batteria ------- 
void BattIco() 
{
Vb=int(analogRead(A3)*XVref/1000L);

// (Il display può caricare in memoria un massimo di 8 simboli: carico solo quello che mi serve se lo stato è diverso da precedente)
     if(Vb>838) {bat=7;} // 4,1V  Batt_7 100%
else if(Vb>808) {bat=6;} // 3,95V Batt_6  83% 
else if(Vb>777) {bat=5;} // 3,8V  Batt_5  67% 
else if(Vb>745) {bat=4;} // 3,65V Batt_4  50% 
else if(Vb>716) {bat=3;} // 3,5V  Batt_3  33%
else if(Vb>695) {bat=2;} // 3,4V  Batt_2  22%
else if(Vb>675) {bat=1;} // 3,3V  Batt_1  11%
else            {bat=0;} // 3,2V  Batt_0   0%

switch (bat)
  {
  case 0: lcd.createChar(4, Batt_0); break;
  case 1: lcd.createChar(4, Batt_1); break;
  case 2: lcd.createChar(4, Batt_2); break;
  case 3: lcd.createChar(4, Batt_3); break;
  case 4: lcd.createChar(4, Batt_4); break;
  case 5: lcd.createChar(4, Batt_5); break;
  case 6: lcd.createChar(4, Batt_6); break;
  case 7: lcd.createChar(4, Batt_7); break;
  }
lcd.setCursor(0,1); lcd.write(byte(4));
}

void BattVolt() // -------- Misura e visualizza la tensione della batteria al litio e l'autonomia residua --------
{
Vb=analogRead(A3)*XVref/1000L; // Pur essendo Vb unsigned long, è necessario specificare che 1000 è Long,
                               // se no va in overflow!
lcd.setCursor(10,0); 
if(((500*Vb/1023)%100)>9)
  {lcd.print(String(int(500*Vb/102300)) + "." + String((500*Vb/1023)%100));}
else
  {lcd.print(String(int(500*Vb/102300)) + ".0" + String((500*Vb/1023)%100));}
lcd.setCursor(15,0); lcd.print("V");

lcd.setCursor(0,1);
// Poiché Vb è unsigned long (quindi intero), per sottrarre 3,2V devo moltiplicare tutto per 10 e poi dividere per 10:
// N.B.: nella v2.9a, fatta per usare il convertitore a 5V, aggiungo la correzione *Vb/859 considerando che, usando un alimentatore
// switching, al calare della tensione aumenta un po' la corrente assorbita.
if(((Vb*10000/205 - 32000) /VSB*Vb/859/10)<10) lcd.print("Autonomia:   " + String(int(Vb*10000/205 - 32000) /VSB*Vb/859/10) + " h");
else lcd.print("Autonomia:  " + String(int(Vb*10000/205 - 32000) /VSB*Vb/859/10) + " h");
}

/*
[V]  Vb   Vb  [V]
4.2 859   850 4.15
4.1 838   840 4.11
4   818   830 4.06
3.9 797   820 4.01
3.8 777   810 3.96
3.7 757   800 3.91
3.6 736   790 3.86
3.5 716   780 3.81
3.4 695   770 3.76
3.3 675   760 3.71
3.2 654   750 3.67
3.1 634   740 3.62
3   613   730 3.57
2.9 593   720 3.52
2.8 572   710 3.47
2.7 552   700 3.42
2.6 531   690 3.37
2.5 511   680 3.32
2.4 491   670 3.27
          660 3.23
          650 3.18
          640 3.13
          630 3.08
          620 3.03
          610 2.98
          600 2.93
*/
