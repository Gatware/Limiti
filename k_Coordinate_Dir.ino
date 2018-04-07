void Coordinate()
{
if(! usingInterrupt) 
   {
    char c = GPS.read();  //Read GPS op Char
    if (GPSECHO)
      if (c) Serial.print(c);    
   }
if (PrimoFix) {CoordinateInDecimale(); Zero(); PrimoFix=0;}
if(E!=0)
  {
  R+=E; E=0;
  if(R>2) {R=2;}
  else if(R<1) R=1;
  lcd.setCursor(12,0);
  if(R==1) lcd.print("a"); // Distanza in linea d'aria
  else lcd.print("r"); // Distanza reale percorsa
  t1=0; // Per assicurare un visualizzazione immediata
  }

if(GPS.newNMEAreceived()) // If new Data is received
  {      
  if(GPS.parse(GPS.lastNMEA()))    // Parse the data
    { // display
    if(GPS.fix) //If there is a GPS FIX
      {
      LAT=10000*GPS.latitude;
      LON=10000*GPS.longitude;
      lcd.setCursor(15,0); lcd.print("F"); // Se ha fatto il punto scrive F
      
      // Latitudine:
      if(LAT/1000000<10) g=" " +String(int(LAT/1000000)); else g=String(int(LAT/1000000));
      if((LAT%1000000)/10000<10) p=" " +String(int((LAT%1000000)/10000)); else p=String(int((LAT%1000000)/10000));
      if((LAT%10000)*6/1000<10) s=" " +String((LAT%10000)*6/1000); else s=String((LAT%10000)*6/1000);
      lcd.setCursor(0,0); lcd.print(g); lcd.write(byte(1)); lcd.print(p +"'" +s +char(34) +String(GPS.lat)); 
      
      // Longitudine:
      if(LON/1000000<10) g=" " +String(int(LON/1000000)); else g=String(int(LON/1000000));
      if((LON%1000000)/10000<10) p=" " +String(int((LON%1000000)/10000)); else p=String(int((LON%1000000)/10000));
      if((LON%10000)*6/1000<10) s=" " +String((LON%10000)*6/1000); else s=String((LON%10000)*6/1000);
      lcd.setCursor(0,1); lcd.print(g); lcd.write(byte(1)); lcd.print(p +"'" +s +char(34) +String(GPS.lon)); 

      xLCD=11; Direzione(); // Qui, contrariamente a quanto avviene nella pagina principale, non c'è il blocco per V<2km/h.

      CoordinateInDecimale();
      // Distanza dal punto in metri                                        114.6=2*57.3: /2 (media) e Gradi->Radianti
      DA=1000*sqrt( sq((POSlat-GPSlatdec)/360*40050) + sq((POSlon-GPSlondec)/360*40050*cos((POSlat+GPSlatdec)/114.6)) ); // Distanza in linea d'aria
      
      unsigned long Spazio=1000*sqrt( sq((POSlatIni-GPSlatdec)/360*40050) + sq((POSlonIni-GPSlondec)/360*40050*cos((POSlatIni+GPSlatdec)/114.6)) );
      if(Spazio>49) // Distanza reale a tratte: somma Spazio se sono almeno 50 metri.
        {
        DR+=Spazio;
        POSlatIni=GPSlatdec; POSlonIni=GPSlondec;
        }
        

      if(digitalRead(0)==LOW)
        {
        Zero();
        while(digitalRead(0)==LOW){delay(200);}
        }

      } // END if(GPS.fix)
    else
      {
      lcd.setCursor(15,0); lcd.write(byte(0)); // Se non ha fatto il punto scrive il carattere "NF"
      lcd.setCursor(0,0); lcd.print("   LAT. --");
      lcd.setCursor(0,1); lcd.print("   LON. --");
      }
    
    lcd.setCursor(13,0); //Show satellites count
    if(GPS.satellites<10) lcd.print(" ");
    lcd.print(String((int)GPS.satellites));
    
    } // END display
    // return;
  } // END if new data

if(millis()-t1>999) // Una volta al secondo
  {
  t1=millis();
  if(R==1) {D=DA;} // Distanza in linea d'aria
  else {D=DR;} // Distanza reale percorsa
  
  if(D<10000000)          
    {
    if(D<10) Distanza="   "+String(int(D))+"m";
    else if(D<100) Distanza="  "+String(int(D))+"m";
    else if(D<1000) Distanza=" "+String(int(D))+"m";
    else if (D<10000) Distanza=String(int(D/1000))+"."+String(int(D%1000)/100) +"km";
    else if (D<100000) Distanza=" "+String(int(D/1000))+"km";
    else Distanza=String(int(D/1000))+"km";
    lcd.setCursor(11,1); lcd.print(Distanza);
    }
  }
}

// Direzione di marcia
void Direzione()
{
if(GPS.fix)
  {
    if      (GPS.angle>=338 || GPS.angle<23)  Dir=1;
    else if (GPS.angle>=23  && GPS.angle<68)  Dir=2;
    else if (GPS.angle>=68  && GPS.angle<113) Dir=3;
    else if (GPS.angle>=113 && GPS.angle<158) Dir=4;
    else if (GPS.angle>=158 && GPS.angle<203) Dir=5;
    else if (GPS.angle>=203 && GPS.angle<248) Dir=6;
    else if (GPS.angle>=248 && GPS.angle<293) Dir=7;   
    else if (GPS.angle>=293 && GPS.angle<338) Dir=8;    

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
else {lcd.setCursor(xLCD,0); lcd.print(" "); Diro=0;} // Imposto Diro=0, così qualunque direzione venga poi rilevata sia 
                                                    // diversa e quindi venga visualizzata.
}

void Zero() {POSlat=GPSlatdec; POSlon=GPSlondec; POSlatIni=GPSlatdec; POSlonIni=GPSlondec; DR=0; Bip();}

void CoordinateInDecimale()
{
// GPS.latitude, GPS.longitude, POSlat e POSlon sono espressi in GRADI*100 + minuti e decimi di minuto!
GPSlatdec=(int)(GPS.latitude/100)+(GPS.latitude-GPS.latitude/100)/60;
GPSlondec=(int)(GPS.longitude/100)+(GPS.longitude-GPS.longitude/100)/60;
}

