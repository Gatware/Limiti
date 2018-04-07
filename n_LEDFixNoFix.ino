void LEDFixNoFix()
{
if(!GPS.fix) // Se non c'è FIX:
  {
  F=0; Voff(); // Sicuramente il verde va spento
  if(GPS.satellites==0) {Gosc=0; Rosc=1;} // Se i satelliti sono 0, fa oscillare il LED Rosso.
  else {Rosc=0; Gosc=1;} // Se ce n'è almeno 1, fa oscillare il LED Giallo.
                          // Entrambi sono preceduti dallo spegnimento dell'altro: Goff() e Roff().
  if(Rosc)   // Se i satelliti sono 0:
    {
    t3=((millis()%1500)>>4); // va da 0 a 62
    if(t3<10) t3=10;
    if(t3<46) analogWrite(6,t3-10); // fase crescente
    else analogWrite(6,103-t3-10); // fase decrescente
    }
  else Roff();
  
  if(Gosc)   // Se i satelliti sono 1 o più:
    {
    t3=((millis()%1500)>>4); // va da 0 a 93
    if(t3<10) t3=10;
    if(t3<46) analogWrite(5,t3-10); // fase crescente
    else analogWrite(5,103-t3-10); // fase decrescente
    }
  else Goff();
  }
  
else // C'è FIX:
  {
  if(F==0) {Roff(); Goff(); F=1;} // Spegne R e G solo quando passa da NoFix a Fix.
  if(V>0) // V>0
    {
    if(V<L1){Von(); GRoff(); LEDG=0; LEDR=0;} // Acc.VERDE: più di 0, meno di L1 e Sp.GIALLO e ROSSO
    else // V>=L1
      {
      Voff(); // Sp.1 (VERDE)
      if(V<LIM)
      {
      Roff(); Gon(); LEDR=0; // Sp.ROSSO e Acc.GIALLO: fra -7% e LIM
      if(BGR && LEDG==0) {LEDG=1; Bip();} // Se BGR è attivo, fa Bip e mette LEDG a 1 per ricordarsi che ha già suonato
      } 
      else // V>=LIM
        {
        Goff(); LEDG=0; // Sp.GIALLO
        if(V<MAX)
          {
          Ron();  // Acc.ROSSO: in tolleranza
          if(BGR && LEDR==0){LEDR=1; Bip(); delay(100); Bip();} // Se BGR è attivo e LED sta ancora a 0, fa Bip-Bip
          }                                                     // e mette LEDR a 1 per ricordarsi che ha già suonato.
        else // V>=MAX: ROSSO intermittente
          {
          if (t==0)  
            {
            Ron(); //Acc.ROSSO
            if(BVM==1)Bip(); // V>=MAX: BIP intermittente
            t=millis();
            }
          t2=millis();
          if(t2-t>18 && n==0)
            {
            Roff(); //Sp.ROSSO
            n=1;
            }
          if(t2-t>100)
            {
            n=0;
            t=0;
            }
          } // END V>=MAX
        } // END V>=LIM
      } // END V>=L1
    } // END V>0
  else {GRoff(); Voff();} // se FIX e V=0, spegne i 3 LED
  } // END c'è FIX

} // END FixNoFix

