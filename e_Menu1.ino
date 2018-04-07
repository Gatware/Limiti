void menu1()
{
Voff(); GRoff(); // Spegne i 3 LED, poiché ora non viene letta la velocità
if(B>1) Bip(); //    -------- OraeData --------
lcd.clear();
lcd.print("   Ora e data   ");
t1=millis();
delay(500);
while(digitalRead(10)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>1000) {menu2(); goto esce_da_menu1;}} // Quando t1 arriva a 2,5 secondi, salta a menu2(), poi ritorna qui
                                                      // e salta alla sequenza di uscita da questo menu.
while(digitalRead(10)==LOW); // Attende che venga lasciato il pulsante
lcd.clear();
while(digitalRead(10)==HIGH) // Continua a pilotare i LED ed eseguire OraeData finché non premo
{LEDFixNoFix(); OraeData();}

if(B>2) Bip(); //    ------- Coordinate -------
lcd.clear();
lcd.print("  Coordinate");
while(digitalRead(10)==LOW)
{delay(500);}
lcd.clear();
lcd.setCursor(12,0);
if(R==1)lcd.print("a"); // Distanza in linea d'aria
else lcd.print("r"); // Distanza reale percorsa
t1=millis();
while(digitalRead(10)==HIGH) // Continua a pilotare i LED ed eseguire Coordinate finché non premo
{LEDFixNoFix(); Coordinate();}

if(B>2) Bip(); //    ------- Tensione della batteria -------
lcd.clear();
lcd.print("Batteria:");
delay(500);
while(digitalRead(10)==LOW);
BattVolt();
while(digitalRead(10)==HIGH) // Continua a pilotare i LED e a visualizzare la tensione finché non premo
     {
     if(millis()-t>2000) {t=millis(); if(GPS.newNMEAreceived()) {if(GPS.parse(GPS.lastNMEA())){} }; LEDFixNoFix(); BattVolt();}
     }                                                                  // Parse the data; Pilota i LED; Legge la batteria.

esce_da_menu1:
// ------------------------- Esce dal menu -------------------------
lcd.clear();
if(B>2) Bip();
lcd.setCursor(9,1);
lcd.print(StrLim); 
}
