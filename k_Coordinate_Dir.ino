void Coordinate()
{
if (PrimoFix) { Zero(); PrimoFix=0;}

if(E!=0)
  {
  R+=E; E=0;
  if(R>2) {R=2;}
  else if(R<1) R=1;
  lcd.setCursor(12,0);
  if(R==1) lcd.print('a'); // Distanza in linea d'aria
  else lcd.print('r'); // Distanza reale percorsa
  t1=0; // Per assicurare un visualizzazione immediata
  }

if (checkGPS())
  {
  lcd.setCursor(15,0); 
  if (fix.valid.status && (fix.status >= gps_fix::STATUS_STD))
    lcd.print('F'); // Se ha fatto il punto scrive F
    else if (fix.valid.time) lcd.print('t'); // Solo ora e data
    else lcd.write(byte(0)); // NoFix

  lcd.setCursor(0,0);
  lcd.print( ' ' );
  if (fix.valid.location)
    lcd.print( fix.latitude(), 6 );
  else
    lcd.print( F("  LAT. --") );
  lcd.setCursor(0,1);
  lcd.print( ' ' );
  if (fix.valid.location)
    lcd.print( fix.longitude(), 6 );
  else
    lcd.print( F("  LON. --") );

  lcd.setCursor(13,0); //Show satellites count
  if (fix.valid.satellites)
    {
    if (fix.satellites<10)
      lcd.print(' ');
    lcd.print( fix.satellites );
    }
  else
    {
    lcd.print( F("--") );
    }

  xLCD=11; Direzione(); // Qui, contrariamente a quanto avviene nella pagina principale, non c'è il blocco per V<2km/h.

  // Distanza dal punto in metri                                        114.6=2*57.3: /2 (media) e Gradi->Radianti
  DA = fix.location.DistanceKm( POS ) * 1000; // Distanza in linea d'aria
  
  unsigned long Spazio = fix.location.DistanceKm( POSIni ) * 1000;
  
  if(Spazio>49) // Distanza reale a tratte: somma Spazio se sono almeno 50 metri.
    {
    DR += Spazio;
    POSIni=fix.location;
    }
    

  if(digitalRead(0)==LOW)
    {
    Zero();
    while(digitalRead(0)==LOW){delay(200);}
    }

  } // END if new data

if(millis()-t1>999) // Una volta al secondo
  {
  t1=millis();
  if(R==1) {D=DA;} // Distanza in linea d'aria
  else {D=DR;} // Distanza reale percorsa
  
  if(D<10000000)          
    {
    lcd.setCursor(11,1);

    if (D < 1000) {
      if (D<10)
        lcd.print( ' ' );
      if (D<100)
        lcd.print( ' ' );
      lcd.print( ' ' );
      lcd.print( (int)D );
      lcd.print( 'm' );
      }
    else
      {
      if (D < 10000)
        lcd.print( D/1000.0, 1 );
      else
        {
        lcd.print( ' ' );
        lcd.print( (int) (D/1000.0) );
        }
      lcd.print( F("km") );
      }
    }
  }
}

// Direzione di marcia
void Direzione()
{
if (fix.valid.heading)
  {
    int angle = fix.hdg.whole;
    
    if      (angle>=338 || angle<23)  Dir=1;
    else if (angle>=23  && angle<68)  Dir=2;
    else if (angle>=68  && angle<113) Dir=3;
    else if (angle>=113 && angle<158) Dir=4;
    else if (angle>=158 && angle<203) Dir=5;
    else if (angle>=203 && angle<248) Dir=6;
    else if (angle>=248 && angle<293) Dir=7;   
    else if (angle>=293 && angle<338) Dir=8;    

    if(Dir!=Diro)
      {
      Diro=Dir;
      DirCamb=1;
      if(Bussola==1 && Dir!=1 && Dir!=5) DirB=10-Dir;
      else DirB=Dir;
      
      switch (DirB)
        {
        case 1: lcd.createChar(3, Nord); break;
        case 2: lcd.createChar(3, NordEst); break;
        case 3: lcd.createChar(3, Est); break;
        case 4: lcd.createChar(3, SudEst); break;
        case 5: lcd.createChar(3, Sud); break;
        case 6: lcd.createChar(3, SudOvest); break;
        case 7: lcd.createChar(3, Ovest); break;
        case 8: lcd.createChar(3, NordOvest); break;
        }
      lcd.setCursor(xLCD,0); lcd.write(byte(3)); // xLCD: posizione orizzontale impostata prima di chiamare Direzione()
      }
  }
else
  {
  lcd.setCursor( xLCD, 0 );
  lcd.print(' ');
  Diro=0;   // Imposto Diro=0, così qualunque direzione venga poi rilevata sia 
            // diversa e quindi venga visualizzata.
  }
}

void Zero() {POS = fix.location; POSIni = fix.location; DR=0; Bip();}

