extern void OraLegale();

void OraeData()
{
  if (GPS.available( gpsSerial ))
    {
    fix = GPS.read();

    lcd.setCursor(15,0); 
    if (fix.valid.status && (fix.status >= gps_fix::STATUS_STD))
      lcd.print('F'); // Se ha fatto il punto scrive F
    else
      lcd.print( ' ' ); // NoFix

    lcd.setCursor(11,1);
    if (fix.valid.altitude)
      {
      // Altitudine:
      int alt = fix.alt.whole;
      if (alt < 1000)
        lcd.print( ' ' );
      if (alt < 10)
        lcd.print( ' ' );
      lcd.print( alt );
      if (alt < 100)
        lcd.print( ' ' );
      }
    else
      {
      lcd.print(F(" -- ")); // Cancella l'altitudine
      }
    lcd.print( 'm' );
    
    if (fix.valid.date && fix.valid.time) {
      OraLegale();

      // Orario  
      lcd.setCursor(0,0);
      if (fix.dateTime.hours < 10)
        lcd.print( ' ' );
      lcd.print(fix.dateTime.hours);
      lcd.print(':');
      if (fix.dateTime.minutes < 10)
        lcd.print( ' ' );
      lcd.print(fix.dateTime.minutes);
      lcd.print(':');
      if (fix.dateTime.seconds < 10)
        lcd.print( ' ' );
      lcd.print(fix.dateTime.seconds);

      // Data
      lcd.setCursor(0,1);
      if (fix.dateTime.date < 10)
        lcd.print( ' ' );
      lcd.print(fix.dateTime.date);
      
      lcd.print( '/' );
      if (fix.dateTime.month < 10)
        lcd.print( ' ' );
      lcd.print(fix.dateTime.month);
      
      lcd.print( '/' );      
      if (fix.dateTime.year < 10)
        lcd.print( '0' );
      lcd.print(fix.dateTime.year);
      }
    else
      {
      lcd.setCursor(0,0); lcd.print( F("--:--:--") );
      lcd.setCursor(0,1); lcd.print( F("--/--/--") );
      }
      
    lcd.setCursor(13,0);
    if (fix.valid.satellites)
      {
      lcd.print(fix.satellites);
      if (fix.satellites < 10)
        lcd.print( ' ' );
      }
    else
      {
      lcd.print( F(" -") );
      }
  }
}
