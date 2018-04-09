extern void BattIco();
extern void DisplayLimiti();
extern void Zero();
extern void Direzione();
extern void LEDFixNoFix();

void loop() // ------------------------------------ loop () ------------------------------------------------------
{

if(millis()-t6>30000) // --------------- Ogni 30 secondi legge la tensione dell'elemento al litio e sceglie e disegna l'icona: ---------------
  { // La forma millis()-t6>intervallo non soffre dei problemi dovuti all'overflow di millis() dopo 49,7 giorni.
  t6=millis();
  BattIco(); // in m_Batteria
  }

if(millis()-t7>500) // --------------- Ogni 500mS controlla se è stato spento e se è stata collegata l'alimentazione ---------------
  {
  t7=millis(); 
  
    // Se è stato spento (Gate del MOSFET a 1), memorizzo e scrivo "Vi SET!"; tra poco si spegnerà.
    if((PINC&B00000001)==1 && Ult==1 && Z!=Zo) // PINC & B00000001: = digitalRead(A0)
      {
      Zo=Z;
      EEPROM.update(0,Z);
      lcd.setCursor(0,0);
      lcd.print( F(" Vi ") );
      lcd.setCursor(0,1);
      lcd.print( F("SET!") );
      if(B>1) Bippino();
      }
    
  lcd.setCursor(1,1);
  analogRead(A4); Vcar=analogRead(A4)*XVref/1000;
  
  if(Vcar>860)
    {
    if(Car==0)
      {lcd.write(byte(2)); Car=1; if(B>0) Bip();} // Se è stata collegata adesso, visualizza la spina
    }
  if(Vcar<=860)
    {
    if(Car==1)
      {lcd.print( ' ' ); Car=0; if(B>0) {Bip(); delay(100); Bip();};} // Se è stata staccata adesso, cancella la spina      
    }
  }

P=digitalRead(10); // Legge il pulsante dell'encoder (0=Premuto)
if(Po==1 && P==0) {t1=millis(); Po=0;} // Quando viene premuto il pulsante legge il tempo
if(Po==0 && P==0) // Se era ed è premuto
  {
  if(millis()-t1>1200) {Po=1; menu1(); LCDmask(); while(digitalRead(10)==LOW); delay(500);} //... se sono trascorsi 1,2 secondi salta a menu(), poi ritorna,
  }                                             // cancella, aspetta che venga lasciato il pulsante e va avanti. Poiché a volte si attiva un Preset, aggiungo il delay.
if(Po==0 && P==1) // se lo lascio prima, richiama i Preset 1...3
  {
  PN+=1; if(PN>qp)PN=1;
  if(B>0) for(n=0;n<PN;n++) {Bip(); delay(60);}
    
  Z=EEPROM.read(PN);
  lcd.setCursor(2,0); lcd.print( F("Pr") );
  lcd.setCursor(3,1); lcd.print(PN);
  CalcoloLimiti();
  DisplayLimiti(); 
  }
if(Po==0 && P==1) Po=1;
// encoder(); // Legge l'encoder (ora è chiamata dagli interrupt)

if(E!=0)
  {
  lcd.setCursor(2,0); lcd.print( F("  ") ); PN=0; // Poiché ho ruotato l'encoder, cancella l'eventuale scritta Pr1...3
  lcd.setCursor(3,1); lcd.print( F(" ") );
  Z+=E; E=0;
  if(Z>9) Z=0;
  if(Z<0) Z=9;
  CalcoloLimiti();
  DisplayLimiti();
  }

// Per il GPS: (Sembra che non serva... Senza, funziona lo stesso!)
if (GPS.available( gpsSerial ))
  {
  // Once per second, new GPS information is ready.
  fix = GPS.read();

  lcd.setCursor(14,1);
  if (fix.valid.satellites)
    {
    if (fix.satellites<10) lcd.print(' ');
    lcd.print( fix.satellites );
    }

  lcd.setCursor(15,0);
  if(fix.valid.status && (fix.status >= gps_fix::STATUS_STD) && fix.valid.speed)
    {
    // Good fix
    lcd.print('F');
    if (PrimoFix) { Zero(); PrimoFix=0;}

    int vd = fix.speed_metersph()/10; // km/h*100 (intero)
    int vm;
    // vdx[cm]=vd;
    // vm=(vdx[1]+vdx[2]+vdx[3])/3; // fa la media delle ultime 3 letture
    // cm++; if(cm==4) cm=1;
    vm = vd;
    V = (int) vm/100;
    if (V<5) V=0;

    lcd.setCursor(9,0);
    if (V<100) lcd.print(' ');
    if (V<10)  lcd.print(' ');
    lcd.print(V);

    // Esegue Direzione in Coordinate e disegna la freccia in (13,0):
    if (vm>100)
      {
      // Se la velocità è almeno 1km/h*, aggiorna lo stato della freccia.
      Fermo=0;
      xLCD=13;
      Direzione();
      }
    else if (Fermo==0)
      {
      // *: Serve per evitare che da fermo la freccia giri casualmente.
      // Se si è appena fermato, cancella la freccia, se lo ricorda e non lo fa più.
      lcd.setCursor(13,0);
      lcd.print(' ');
      Fermo=1;
      }

    }
   else if (fix.valid.time)
    {
    // Legge solo ora e data
    lcd.print('t');
    lcd.setCursor(9,0);
    lcd.print(F(" --  "));
    V = 0;
    }
    
  else
    {
    // not a good fix
    lcd.write(byte(0));
    lcd.setCursor(9,0);
    lcd.print(F(" --  "));
    V = 0;
    }
  }

if(digitalRead(0)==0) // Se il pulsante ACQ è premuto
  {
  if(V>39) // e la velocità è almeno 40km/h
    {
    if(B>1) Bip();
    if(V%10>3) {LIM=(V+10)/10*10;} // Se V è 4...9km/h oltre la decina intera (es.64 o 67km/h) imposta 70
    else {LIM=V/10*10;} // Se V è 0...3km/h oltre la decina intera (es.70 o 73km/h) imposta 70
    Z=(LIM-40)/10; // Calcola l'indice Z dal limite impostato a partire dalla velocità corrente.
    CalcoloLimiti();
    DisplayLimiti();
    t1=millis();
    while(millis()-t1<150)
      if(GPS.available(gpsSerial)) fix=GPS.read();
    }
  else
    {
    if(B>1) Bippino();
    lcd.setCursor(0,0); lcd.print(F("     ")); delay(100);
    if(B>1) Bippino();
    lcd.setCursor(0,0); lcd.print(F("V<40!")); delay(100);
    if(B>1) Bippino();
    while (millis()-t1<700)
      if (GPS.available(gpsSerial))
        fix = GPS.read();
    lcd.setCursor(0,0); lcd.print(F("     "));
    while (millis() - t1 < 100)
    while (millis() - t1 < 100)
      if (GPS.available( gpsSerial ))
        fix = GPS.read();
    LCDmask();
    }
    
  while(digitalRead(0)==0); // Attende che venga lasciato il pulsante ACQ
  }

LEDFixNoFix();

PORTC^=B00000100; // Monitoria 1/2 f loop su A2 (ad ogni passaggio inverte lo stato)
} // END loop
