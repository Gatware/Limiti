void OraLegale() // Compara anno, mese, giorno e ora correnti con quelli di inizio e fine ora legale di ogni anno.
                 // I valori finiscono tutti per "02" perché l'ora legale inizia e finisce alle ore 2 solari.
{
year=GPS.year; // la variabile year, essendo long, può contenere il risultato di GPS.year*1000000
month=GPS.month;
day=GPS.day;
hour=GPS.hour;
Adesso=1000000*year +10000*month +100*day +hour;
     if(Adesso>=15032902 && Adesso<15102502) Leg=1;
else if(Adesso>=16032702 && Adesso<16103002) Leg=1;
else if(Adesso>=17032602 && Adesso<17102902) Leg=1;
else if(Adesso>=18032502 && Adesso<18102802) Leg=1;
else if(Adesso>=19033102 && Adesso<19102702) Leg=1;
else if(Adesso>=20032902 && Adesso<20102502) Leg=1;
else if(Adesso>=21032802 && Adesso<21103102) Leg=1;
else if(Adesso>=22032702 && Adesso<22103002) Leg=1;
else if(Adesso>=23032602 && Adesso<23102902) Leg=1;
else if(Adesso>=24033102 && Adesso<24102702) Leg=1;
else if(Adesso>=25033002 && Adesso<25102602) Leg=1;
else if(Adesso>=26032902 && Adesso<26102502) Leg=1;
else if(Adesso>=27032802 && Adesso<27103102) Leg=1;
else if(Adesso>=28032602 && Adesso<28102902) Leg=1;
else if(Adesso>=29032502 && Adesso<29102802) Leg=1;
else if(Adesso>=30033102 && Adesso<30102702) Leg=1;
else if(Adesso>=31033002 && Adesso<31102602) Leg=1;
else if(Adesso>=32032802 && Adesso<32103102) Leg=1;
else if(Adesso>=33032702 && Adesso<33103002) Leg=1;
else if(Adesso>=34032602 && Adesso<34102902) Leg=1;
else if(Adesso>=35032502 && Adesso<35102802) Leg=1;
else if(Adesso>=36033002 && Adesso<36102602) Leg=1;
else if(Adesso>=37032902 && Adesso<37102502) Leg=1;
else if(Adesso>=38032802 && Adesso<38103102) Leg=1;
else if(Adesso>=39032702 && Adesso<39103002) Leg=1;
else if(Adesso>=40032502 && Adesso<40102802) Leg=1;
else if(Adesso>=41033102 && Adesso<41102702) Leg=1;
else if(Adesso>=42033002 && Adesso<42102602) Leg=1;
else if(Adesso>=43032902 && Adesso<43102502) Leg=1;
else if(Adesso>=44032702 && Adesso<44103002) Leg=1;
else if(Adesso>=45032602 && Adesso<45102902) Leg=1;
else if(Adesso>=46032502 && Adesso<46102802) Leg=1;
else if(Adesso>=47033102 && Adesso<47102702) Leg=1;
else if(Adesso>=48032902 && Adesso<48102502) Leg=1;
else if(Adesso>=49032802 && Adesso<49103102) Leg=1;
else if(Adesso>=50032702 && Adesso<50103002) Leg=1;
else Leg=0;

GPS_hf=GPS.hour+1+Leg; // 1: Fuso orario dell'Europa Centrale
      
if(GPS_hf>23)
  {
  GPS_hf-=24;
  GPS.day+=1; 
  
  // Controlliamo se ci sono riporti al mese e all'anno: 
  if(GPS.day==32) // Può esserlo solo se il mese in corso è di 31 giorni
    {
    GPS.day=1;
    GPS.month+=1;
    if(GPS.month==13) // Significa che è Capodanno
      {
      GPS.month=1;
      GPS.year+=1;
      }
    }          
  if((GPS.day==31) && (GPS.month==4||GPS.month==6||GPS.month==9||GPS.month==11)) // E' un mese di 30 giorni
    {
    GPS.day=1;
    GPS.month+=1;
    }        
  if((GPS.day==30) && (GPS.month==2) && (GPS.year%4==0)) // E' un anno bisestile
    {
    GPS.day=1;
    GPS.month+=1;
    }    
  if((GPS.day==29) && (GPS.month==2) && (GPS.year%4!=0)) // E' un anno normale
    {
    GPS.day=1;
    GPS.month+=1;
    }
  }    
} // END OraLegale
