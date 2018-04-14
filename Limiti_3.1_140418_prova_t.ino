#include <NMEAGPS.h> // Using Neo-GPS lib
#include <EEPROM.h>
#include <NeoSWSerial.h> //Serial com
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 12, 13, 7); // RS, EN, D4, D5, D6, D7

unsigned long XVref=0.940*1080; // 0,94 * Vref in mV. Inserire qui il valore letto con il multimetro sul pin 21 dell'ATmega328P
                                // o il valore del riferimento di tensione esterno montato.
byte L[]={40, 50, 60, 70, 80, 90, 100, 110, 120, 130}; // Limiti preimpostati per L[0...9]
int Z; // Accumula le letture dell'encoder per indirizzare L[]; E' sempre l'indice del limite corrente (verificato).
int Zo;// Indice del limite precedente (per vedere, se Ult=1, se è cambiato e dopo 10 sec. memorizzare).
byte Ult=0; // Se Ult=1 [338-339] significa dopo 10 sec. di limite invariato lo deve impostare come iniziale.
int Y; // Accumula le letture dell'encoder per indirizzare L[]
int V;  // Velocità rilevata
// int vdx[7]; // fino a 7 valori di vd per fare la media (media disattivata).
byte cm=1; // Conta l'indice da 1 a... (fino a 7) per fare la media

byte LIM; // Limite di velocità corrente
byte L1; // = LIM-7% (=tachimetro)
byte MAX;  // = L+5 o 5% (Massima nella tolleranza)
byte P; // Lettura del pulsante dell'encoder
byte Po=1;// Lettura precedente del pulsante dell'encoder
uint8_t PN;  // Preset selezionato
unsigned long int t0; // Per la lettura periodica della velocità
unsigned long int t1; // Memorizza il tempo quando viene premuto il pulsante
volatile byte S; // Lettura dei due valori dell'encoder
volatile byte So;// Lettura precedente
volatile int E; // Risultato della routine encoder(): 1, -1, 0.
volatile int X; // Usato in encoder() per evitare letture multiple
byte Vi=40; // Limite di Velocità iniziale accumulata dall'encoder
byte n=0; // Per il lampeggiamento del rosso, contatore locale del numero di Bip emessi al richiamo di un Preset,
          // contatore del numero del Preset in fase di modifica durante il menu, contatore per la cancellazione
          // del set di caratteri in m_Batteria, scorrimento dei caratteri nel Setup.
int B=0; // Bip on/off
int Bt=0; // Btemporaneo per non far cambiare l'impostazione del Bip mentre si imposta il Bip
int BVM=0;  // Bip-Bip a VMAX on/off
int BGR=0; // Bip per Giallo e Rosso (sono int poiché nella rotazione dell'encoder potrebbero diventare per un attimo -1)
byte LEDG=0; // 1: LED Giallo acceso. Ha già fatto Bip se BGR=1.
byte LEDR=0; // 1: LED Rosso acceso. Ha già fatto Bip se BGR=1.
byte Gosc=0; // 1: LED Giallo oscillante.
byte Rosc=0; // 1: LED Rosso oscillante.
byte F=0; // 1: Fix (per i LED oscillanti).
int Bussola=1; // Se è a 1, la freccia indica il Nord; se è a 0, il Nord sta fisso in alto.
// unsigned long Tout=100000; // impostazione del timeout lungo o breve
unsigned long t=0; // per il conteggio del tempo durante l'intermittenza. Usato anche in OraeData e nella lettura della tensione della batteria
unsigned long t2=0; // per il conteggio del tempo durante l'intermittenza
int t3=0; // Ci metto il resto di millis() diviso 2500 per i LED oscillanti.
unsigned long t4=0; // per il conteggio del tempo di limite invariato (se Ult=1).
unsigned long t5=0; // per la cancellazione della scritta "ViSET"
unsigned long t6=0; // per il controllo periodico dello stato della batteria
unsigned long t7=0; // per il controllo periodico del collegamento dell'alimentazione
int k; // Costante di proporzionalità fra frequenza del segnale e velocità
byte h; // Fattore percentuale di correzione fine della velocità
int qp; // Quanti Preset? Impostazione della quantità di Preset
unsigned long Vb; // Tensione della batteria, letta su A3. Deve essere long per permettere i calcoli per la lettura della tensione
byte bat=7;  // Stato corrente della batteria (indice dell'icona)
int Vcar=0; // Valore della tensione di carica letto su A4
byte Car=0; // 1: Batteria in carica
int VSB=0; // Velocità di scarica della batteria in mV/h per determinare l'autonomia
//Per la parte del GPS:
volatile int state = LOW;
static const int RXPin = A5, TXPin = 6; // TXPin = A4;
NeoGPS::Location_t POS; // Memorizza la latitudine/longitudine di un punto
NeoGPS::Location_t POSIni; // Latitudine/longitudine iniziale per il calcolo della distanza reale a tratte
byte Dir=0; // Direzione (1=Nord...8=SudOvest)
byte Diro=0; // Direzione precedente
byte DirB=0; // Dir tenendo anche conto dell'impostazione Bussola.
byte DirCamb=0; // Per la distanza reale: 1:la direzione è cambiata, quindi somma la tratta misurata
byte Fermo=0; // 1: velocità <1km/h
byte xLCD=0; // Posizione x in cui scrivere la freccia indicatrice di direzione
long DA=0; // Distanza in linea d'aria calcolata tra la posizione attuale e il punto memorizzato
long DR=0; // Distanza reale percorsa (somma di tratte) tra la posizione attuale e il punto memorizzato
long D=0; // Distanza selezionata fra le due per la visualizzazione
byte R=1; // Distanza in linea d'Aria (1) o Reale percorsa (2) 
byte PrimoFix=1; // La prima volte che fa il Fix dall'accensione memorizza le coordinate in POSlatIni e POSlonIni
long month=0;  //    (12x10000=120.000 MAX) long: -2.147.483.648...2.147.483.647
long year=0; // (50x1000000=50.000.000 MAX) long: -2.147.483.648...2.147.483.647
long Adesso=0;
byte Leg=0; // Ora solare: 0; Ora legale: 1.
int Contr=0; // Correzione manuale al contrasto automatico (-10...+10)

NeoSWSerial gpsSerial(RXPin, TXPin);
NMEAGPS GPS;  //Setting Serial
gps_fix fix;

void detectRTCtimeOnly()
{
  if(fix.valid.status && (fix.status == gps_fix::STATUS_TIME_ONLY))
    {
    // The MTK3339 will never send this value,
    // but this is how you might detect it on ublox GPS devices.
    return;
    }

  bool noSatellites = not fix.valid.satellites or (fix.satellites == 0);
  bool haveTime     = fix.valid.date or fix.valid.time;
  bool tracking     = fix.valid.status and (fix.status >= gps_fix::STATUS_STD);

  if(not tracking)
    {
    // Look for tracked satellites from the GSV sentences
    for (uint8_t i=0; i < GPS.sat_count; i++)
      {
      if (GPS.satellites[ i ].tracked) {tracking = true; break;}
      }
    }
    
  if(haveTime and tracking and noSatellites)
    {
    // Force the fix status even though no NMEA sentences set it
    fix.valid.status = true;
    fix.status       = gps_fix::STATUS_TIME_ONLY;
    }
} // detectRTCtimeOnly


bool checkGPS()
{
bool newData = GPS.available( gpsSerial );
if(newData)
  {
  // Once per second, new GPS information is ready.
  fix = GPS.read();
  detectRTCtimeOnly();
  }
return newData;
}
