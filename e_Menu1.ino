extern void Coordinate();
extern void BattVolt();

void menu1()
{
Voff(); GRoff(); // Spegne i 3 LED, poiché ora non viene letta la velocità
if(B>1) Bip(); //    -------- OraeData --------
lcd.clear();
lcd.print(F("   Ora e data   "));
t1=millis();

bool esce_da_menu1 = false;
while((millis()-t1<500) || (digitalRead(10)==LOW)) // Attende che venga lasciato il pulsante.
  {
  checkGPS();
  if(millis()-t1>1000)
    {
    menu2();
    esce_da_menu1 = true;
    break;
    }
  } // Quando t1 arriva a 2,5 secondi, salta a menu2(), poi ritorna qui
    // e salta alla sequenza di uscita da questo menu.

if(!esce_da_menu1)
  {
  while(digitalRead(10)==LOW); // Attende che venga lasciato il pulsante
  lcd.clear();
  while(digitalRead(10)==HIGH) // Continua a pilotare i LED ed eseguire OraeData finché non premo
  {LEDFixNoFix(); OraeData();}

  if(B>2) Bip(); //    ------- Coordinate -------
  lcd.clear();
  lcd.print(F("  Coordinate"));
  t1=millis();
  while ((millis() - t1 < 500) || (digitalRead(10)==LOW))
    {
    if (GPS.available(gpsSerial)) fix=GPS.read();
    }
  lcd.clear();



  t1=millis();
  while(digitalRead(10)==HIGH) // Continua a pilotare i LED ed eseguire Coordinate finché non premo
  {LEDFixNoFix(); Coordinate();}

  if(B>2) Bip(); //    ------- Tensione della batteria -------
  lcd.clear();
  lcd.print( F("Batteria:") );
  t1=millis();
  while ((millis()-t1<500) || (digitalRead(10)==LOW))
    {
    checkGPS();
    }
  BattVolt();

  while(digitalRead(10)==HIGH) // Continua a pilotare i LED e a visualizzare la tensione finché non premo
    {
    checkGPS();
    if (millis()-t>2000)
      {
      t=millis();
      LEDFixNoFix();
      BattVolt();
      }
    }            // Parse the data; Pilota i LED; Legge la batteria.
  }

// ------------------------- Esce dal menu -------------------------
lcd.clear();
if(B>2) Bip();
DisplayLimiti(); 
}
