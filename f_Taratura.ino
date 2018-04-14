void taratura()
{
if(B>1) Bip();

// -------------------------------- Impostazione della quantità di Preset --------------------------------
lcd.clear();
lcd.setCursor(0,0);
lcd.print( F("Quanti Preset?") );
lcd.setCursor(4,1);
lcd.print(qp);
lcd.print( F("    ") );

while(digitalRead(10)==LOW); // Attende che venga lasciato il pulsante
while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  // encoder();  (ora è chiamata dagli interrupt)
  if(E!=0){qp+=E;E=0;delay(20);}
  if(qp<1)qp=4;
  if(qp>4)qp=1;

  lcd.setCursor(4,1);
  lcd.print( qp );
  lcd.print( F("    ") );
  };
if(B>1) Bip(); // Ho premuto il pulsante, perciò prosegue 
if (qp!=EEPROM.read(10))
  {
  EEPROM.update(10,qp);
  lcd.setCursor(9,1);
  lcd.print(F("SET"));
  delay(1200);
  }
delay(200);
lcd.clear();                    // Se è cambiata, memorizza l'impostazione in EEPROM 9.

// -------------------------------- Impostazione della capacità della batteria --------------------------------
lcd.setCursor(0,0); lcd.print( F("Vel. scar. batt.") );
while(digitalRead(10)==LOW); // Attende che venga lasciato il pulsante
while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  // encoder();  (ora è chiamata dagli interrupt)
  if(E!=0){VSB+=E;E=0;delay(20);}
  if(VSB<20)VSB=20;
  if(VSB>50)VSB=50;
  lcd.setCursor(0,1);
  lcd.print(VSB);
  lcd.print( F("mV/h AutMx:") );
  lcd.print( (int)(1000/VSB) );
  lcd.print( 'h' );
  };
if(B>1) Bip(); // Ho premuto il pulsante, perciò prosegue 
if (VSB!=EEPROM.read(14))
  {
  EEPROM.update(14,VSB);
  lcd.setCursor(8,1);
  lcd.print( F("SET     ") );
  delay(1200);
  }
delay(200); lcd.clear();                    // Se è cambiata, memorizza l'impostazione in EEPROM 9.

// -------------------------------- Impostazione dell'indicatore di direzione --------------------------------
lcd.setCursor(0,0); lcd.print( F("Indic. di direz.") );
while(digitalRead(10)==LOW); // Attende che venga lasciato il pulsante
while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  // encoder();  (ora è chiamata dagli interrupt)
  if(E!=0){Bussola+=E;E=0;delay(20);}
  if(Bussola<0) Bussola=1;
  if(Bussola>1) Bussola=0; // Per Bussola lascio l'alternanza
  lcd.setCursor(0,1);
  if(Bussola==0) lcd.print( F("Nord in alto") );
  else           lcd.print( F("Bussola     ") );
  };
if(B>1) Bip(); // Ho premuto il pulsante, perciò prosegue 
if (Bussola!=EEPROM.read(15))
  {
  EEPROM.update(15,Bussola);
  lcd.setCursor(13,1);
  lcd.print( F("SET") );
  delay(1200);
  }
delay(200); lcd.clear();                    // Se è cambiata, memorizza l'impostazione in EEPROM 15.
  
} // END Taratura
