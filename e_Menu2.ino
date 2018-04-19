void menu2()
{
Voff(); GRoff(); // Spegne i 3 LED, poiché ora non viene letta la velocità
lcd.clear();
lcd.print( F("     Preset") );
if(B>1) {Bip(); delay(100); Bip();}

bool esce_da_menu2 = false;

t1=millis();
while(digitalRead(10)==LOW) // Attende che venga lasciato il pulsante; se nel frattempo millis() arriva a 4000, regola il contrasto,
                            // altrimenti va a Ora e data
  {if(millis()-t1>1000) // ------------------ Regolazione del contrasto ------------------
    {
    lcd.setCursor(0,0);
    lcd.print( F("Contrasto    ") );
    if(B>0) {Bip(); delay(100); Bip(); delay(100); Bip();}
    Contr=EEPROM.read(13);
    while(digitalRead(10)==LOW); // Attende che venga lasciato il pulsante
    delay(300);
    while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo 
      {
      // encoder();  (ora è chiamata dagli interrupt)
      if(E!=0)
        {
        if(E==1 && Contr<10) Contr+=E;
        else if(E==-1 && Contr<11) Contr+=E;
        else Contr+=E*5;
        E=0;delay(20);
        
        if(Contr>100) Contr=100;
        else if(Contr<0) Contr=0;
        analogWrite(11,Contr);
        }
      lcd.setCursor(13,0);
      if(Contr<10)
        lcd.print(' ');
      if(Contr<100)
        lcd.print(' ');
      lcd.print(Contr);
      };
      
    t1=millis();
    while(digitalRead(10)==LOW) 
      {
      if(millis()-t1>1000) {lcd.clear(); if(B>2) Bip(); return;}
      }
        
    if (Contr!=EEPROM.read(13))
      {
      EEPROM.update(13,Contr);
      lcd.setCursor(13,1);
      lcd.print( F("SET") );
      if(B>1) Bip();
      delay(800);
      }
    delay(200);                 // Se è cambiata, memorizza l'impostazione in EEPROM 13.
    
    esce_da_menu2 = true;
    break;
    }
  }
  
if (!esce_da_menu2)
  {
  // ------------------ Preset n? ------------------   
  delay(200);
  lcd.clear();
  for(n=1; n<=qp; n++)
    {
    if(B>2) Bip(); 
    lcd.setCursor(0,0);
    lcd.print( F("Preset ") );
    lcd.print( n );
    lcd.print( '?' );
    Y=EEPROM.read(n); // Legge l'indice del limite attualmente impostato nel Preset n.
   
    while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo.
      {
      // encoder();  (ora è chiamata dagli interrupt)
      if(E!=0){Y+=E;E=0;delay(20);}
      if(Y>9) Y=0;
      if(Y<0) Y=9;
    
      lcd.setCursor(10,0);
      lcd.print( L[Y] );
      lcd.print( ' ' );
      };
    
    t1=millis();
    while(digitalRead(10)==LOW) 
      {
      if(millis()-t1>1000) {lcd.clear(); if(B>2) Bip();return;}
      }
      
    if (Y!=EEPROM.read(n)) {EEPROM.update(n,Y); lcd.setCursor(10,1); lcd.print( F("SET") ); if(B>1) Bip(); delay(800);}
    delay(200); lcd.clear();       // Se è cambiato, memorizza l'indice della velocità in EEPROM n.
    }
    
  lcd.setCursor(0,0); //    ------- Bip? -------
  if(B>2)Bip();
  lcd.print( F("Bip? ") );
  B=EEPROM.read(7);
  Bt=B;

  while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo
    {
    // encoder();  (ora è chiamata dagli interrupt)
    if(E!=0){Bt+=E;E=0;delay(20);}
    if(Bt>3) Bt=0;
    if(Bt<0) Bt=3;

    lcd.setCursor(5,0);
    if(Bt==0){lcd.print(F("Mai        ")); lcd.setCursor(10,1);    lcd.print(F("      "));}
    if(Bt==1){lcd.print(F("Solo Preset")); lcd.setCursor(6,1); lcd.print(F("    e alim"));}
    if(Bt==2){lcd.print(F("Solo Preset")); lcd.setCursor(6,1); lcd.print(F("alim e SET"));}
    if(Bt==3){lcd.print(F("Sempre     ")); lcd.setCursor(6,1); lcd.print(F("          "));}
    };

  t1=millis();
  while(digitalRead(10)==LOW) 
    {
    if(millis()-t1>1000) {lcd.clear(); if(B>2) Bip(); return;}
    }
    
  // ho lasciato il pulsante, perciò prosegue:
  B=Bt;
  if (B!=EEPROM.read(7)) {EEPROM.update(7,B); lcd.setCursor(2,1); lcd.print( F("SET") ); if(B>1) Bip(); delay(800);} 
  delay(200); lcd.clear();                // Se è cambiata, memorizza l'impostazione in EEPROM 7.

  lcd.setCursor(0,0); //    ------- Bip-Bip VMAX? -------
  if(B>2)Bip();
  lcd.print( F("Bip-Bip VMAX? ") );
  BVM=EEPROM.read(8);

  while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo
    {
    // encoder();  (ora è chiamata dagli interrupt)
    if(E!=0){BVM+=E;E=0;delay(20);}
    if(BVM>1) BVM=1;
    if(BVM<0) BVM=0;

    lcd.setCursor(14,0);
    if(BVM==0){lcd.print( F("No") );}
    else{lcd.print( F("Si") );}
    };

  t1=millis();
  while(digitalRead(10)==LOW) 
    {
    if(millis()-t1>1000) {lcd.clear(); if(B>2) Bip();return;}
    }
    
  if (BVM!=EEPROM.read(8)) {EEPROM.update(8,BVM); lcd.setCursor(13,1); lcd.print( F("SET") ); if(B>1) Bip(); delay(800);}
  delay(200); lcd.clear();                    // Se è cambiata, memorizza l'impostazione in EEPROM 8.

  lcd.setCursor(0,0); //    ------- Bip a Giallo e Rosso? -------
  if(B>2)Bip();
  lcd.print( F("Bip Gi/Rosso? ") );
  BGR=EEPROM.read(12);

  while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo
    {
    // encoder();  (ora è chiamata dagli interrupt)
    if(E!=0){BGR+=E;E=0;delay(20);}
    if(BGR>1) BGR=1;
    if(BGR<0) BGR=0;

    lcd.setCursor(14,0);
    if(BGR==0){lcd.print( F("No") );}
    else{lcd.print( F("Si") );}
    };

  t1=millis();
  while(digitalRead(10)==LOW) 
    {
    if(millis()-t1>1000) {lcd.clear(); if(B>2) Bip();return;}
    }
    
  if (BGR!=EEPROM.read(12)) {EEPROM.update(12,BGR); lcd.setCursor(13,1); lcd.print( F("SET") ); if(B>1) Bip(); delay(800);}
  delay(200); lcd.clear();                    // Se è cambiata, memorizza l'impostazione in EEPROM 12.

  // --------------------------------- Lim. iniz.? ----------------------------
  lcd.setCursor(0,0);
  if(B>2)Bip();
  lcd.print( F("Lim. iniz.?") );

  if(Ult==0) Y=EEPROM.read(0); // Legge l'indice del limite iniziale attualmente impostato.
  else Y=10; // Se Ult è attivo, scorre i limiti a partire da "Ult." 
  if(EEPROM.read(11)==1) {lcd.setCursor(12,1); lcd.print(L[Z]);} // Se è attivo Ult, visualizza sotto il limite attuale.
  while(digitalRead(10)==HIGH) // Continua a leggere l'encoder finché non premo.
    {
    // encoder();  (ora è chiamata dagli interrupt)
    if(E!=0){Y+=E;E=0;delay(20);}
    if(Y>10) Y=0;
    if(Y<0) Y=10;
    lcd.setCursor(12,0);
    if(Y<10) {Vi=L[Y]; Ult=0; lcd.print(Vi); lcd.print( F("  ") );}
    else {Ult=1; lcd.print( F("Ult.") );}
    }
    
  t1=millis();
  while(digitalRead(10)==LOW) 
    {
    if(millis()-t1>1000) {lcd.clear(); if(B>2) Bip();return;}
    }

  if(Ult==0 && Y!=10 && Y!=EEPROM.read(0)) {EEPROM.update(0,Y); lcd.setCursor(12,1); lcd.print( F("SET ") ); if(B>1) Bip(); delay(800);} // Se è cambiato,
                                                     // memorizza l'indice del limite iniziale in EEPROM(0) e scrive "SET".
  if(Ult!=EEPROM.read(11)) {EEPROM.update(11,Ult); lcd.setCursor(12,1); lcd.print( F("SET ") ); if(B>1) Bip(); delay(800);} // Se Ult è cambiato
                                                     // rispetto al valore memorizzato in EEPROM(11), lo memorizza e scrive "SET".
  delay(200); lcd.clear();
  }

// ------------------------- Esce dal menu -------------------------
lcd.clear();
LCDmask();
}
