 void LCDmask()
{
lcd.setCursor(0,0);
if(Ult==0) lcd.print(' ');
else {lcd.print('U');}

BattIco();

lcd.setCursor(5,0); lcd.print(F("VEL="));
lcd.setCursor(5,1); lcd.print(F("LIM=")); if(LIM<100)lcd.print(' '); lcd.print( LIM );

if(Bussola==1) {lcd.setCursor(13,1); lcd.print('B');}  // Se l'indicatore di direzione è in modalità Bussola, scrive B.

if(PN>0) {lcd.setCursor(2,0); lcd.print(F("Pr")); lcd.setCursor(3,1); lcd.print(PN);}

if(Vcar>870) {lcd.setCursor(1,1); lcd.write(byte(2));} // Se è collegata, visualizza la spina
}
