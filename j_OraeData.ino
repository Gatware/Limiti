void OraeData()
{
if(! usingInterrupt) 
   {
    char c = GPS.read();  //Read GPS op Char
    if (GPSECHO)
      if (c) Serial.print(c);    
   }
   
if(GPS.newNMEAreceived()) // If new Data is received
  {      
  if(GPS.parse(GPS.lastNMEA()))    // Parse the data
    { // Visualizza ora, data, altitudine
    if(GPS.fix) //If there is a GPS FIX
      {
      lcd.setCursor(15,0); lcd.print("F"); // Se ha fatto il punto scrive F
      
      // Altitudine:
      if(GPS.altitude<10) alt="  "+String(int(GPS.altitude))+" m";
      else if(GPS.altitude<100)  alt=" "+String(int(GPS.altitude))+" m";
      else if(GPS.altitude<1000) alt=" "+String(int(GPS.altitude))+"m";
      else alt=String(int(GPS.altitude))+"m";
      lcd.setCursor(11,1); lcd.print(alt);
      }
    else
      {
      lcd.setCursor(15,0); lcd.write(byte(0)); // NoFix
      lcd.setCursor(11,1); lcd.print(" -- m"); // Cancella l'altitudine
      }
    
    if(GPS.year!=80) // Non è detto che l'orario sia preciso, ma quantomeno
                    // l'orologio non è azzerato al 1980!
      {
      OraLegale();

      // Orario  
      ore=String(GPS_hf); if(GPS_hf<10) {ore=" "+ore;}
      minuti=String(GPS.minute); if(GPS.minute<10){minuti="0"+minuti;}
      secondi=String(GPS.seconds); if(GPS.seconds<10){secondi="0"+secondi;}
      lcd.setCursor(0,0); lcd.print(ore+':'+minuti+':'+secondi);
      // Data
      giorno=String(GPS.day); if(GPS.day<10) giorno=" "+giorno;
      mese=String(GPS.month); if(GPS.month<10) mese=" "+mese;
      anno=String(GPS.year);
      lcd.setCursor(0,1); lcd.print(giorno+"/"+mese +"/"+anno);
      }
    else
      {
      lcd.setCursor(0,0); lcd.print("--:--:--");
      lcd.setCursor(0,1); lcd.print("--/--/--");
      }
      
    lcd.setCursor(13,0); lcd.print(GPS.satellites); //Show satellites count
    if(GPS.satellites<10) lcd.print(" ");

    lcd.setCursor(11,1);
    } // End Display ora e data  
  return;
  } // END if new data
}
