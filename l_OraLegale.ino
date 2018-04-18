void OraLegale() // Compara anno, mese, giorno e ora correnti con quelli di inizio e fine ora legale di ogni anno.
                 // I valori finiscono tutti per "02" perché l'ora legale inizia e finisce alle ore 2 solari.
{
  static const int32_t          zone_hours   = +1L; // CET
  static const NeoGPS::clock_t  zone_offset  = zone_hours* NeoGPS::SECONDS_PER_HOUR;

  static const uint8_t springMonth =  3;
  static const uint8_t springDate  = 31; // latest last Sunday
  static const uint8_t springHour  =  1;
  static const uint8_t fallMonth   = 10;
  static const uint8_t fallDate    = 31; // latest last Sunday
  static const uint8_t fallHour    =  1;

  NeoGPS::clock_t seconds = fix.dateTime; // convert to seconds

  //  Calculate DST changeover times once per reset and year!
  static NeoGPS::time_t  changeover;
  static NeoGPS::clock_t springForward, fallBack;

  if ((springForward == 0) || (changeover.year != fix.dateTime.year)) {

    //  Calculate the spring changeover time (seconds)
    changeover.year    = fix.dateTime.year;
    changeover.month   = springMonth;
    changeover.date    = springDate;
    changeover.hours   = springHour;
    changeover.minutes = 0;
    changeover.seconds = 0;
    changeover.set_day();
    // Step back to a Sunday, if day != SUNDAY
    changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
    springForward = (NeoGPS::clock_t) changeover;

    //  Calculate the fall changeover time (seconds)
    changeover.month   = fallMonth;
    changeover.date    = fallDate;
    changeover.hours   = fallHour - 1; // to account for the "apparent" DST +1
    changeover.set_day();
    // Step back to a Sunday, if day != SUNDAY
    changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
    fallBack = (NeoGPS::clock_t) changeover;
  }

  //  First, offset from UTC to the local timezone
  seconds += zone_offset;

  //  Then add an hour if DST is in effect
  if ((springForward <= seconds) && (seconds < fallBack))
    seconds += NeoGPS::SECONDS_PER_HOUR;

  fix.dateTime = seconds; // convert seconds back to a date/time structure

} // END OraLegale
