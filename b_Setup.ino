void setup()
{
analogReference (INTERNAL); // Imposta per l'ADC il riferimento interno a 1,1V nominali (+/-10%)
t2=millis(); // Per i LED oscillanti (n. di satelliti).
// ADC veloce:
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 
// Imposta il prescaler dell'ADC (esponenti di 2):
  sbi(ADCSRA,ADPS2); // 4 SI
  cbi(ADCSRA,ADPS1); // 2 no
  cbi(ADCSRA,ADPS0); // 1 no

attachInterrupt(1,encoder,CHANGE);
attachInterrupt(0,encoder,CHANGE);

// Il prescaler è impostato a 2^4=16, quindi 16MHz/16=1MHz.
lcd.begin(16,2); 
pinMode(0,INPUT); // Pulsante ACQ in: PORTD=B0000000x
digitalWrite(0,HIGH); // Pull up
pinMode(2,INPUT); // Encoder in A: PORTD=B00000x00
digitalWrite(2,HIGH); // Pull up
pinMode(3,INPUT); // Encoder in B: PORTD=B0000x000
digitalWrite(3,HIGH); // Pull up
pinMode(10,INPUT);// Encoder in SW:PORTB=B00000x00
digitalWrite(10,HIGH);// Pull up
pinMode(A0,INPUT);//   Era Ingresso segnale: PORTC=B0000000x
                  //   Diventa l'ingresso per il rilevamento dello spegnimento (v2.6)
pinMode(A3,INPUT);//   Ingresso tensione dell'elemento al litio
pinMode(A4,INPUT);//   Ingresso tensione di carica
pinMode(1,OUTPUT);// LED V:       PORTD=B000000x0
pinMode(5,OUTPUT);// LED G:       PORTD=B00x00000
pinMode(6,OUTPUT);// LED R:       PORTD=B0x000000
pinMode(A1,OUTPUT);//  Buzzer 5V: PORTC=B000000x0
pinMode(A2,OUTPUT);//  1/2 floop: PORTC=B00000x00
pinMode(11, OUTPUT);// PWM out per contrasto automatico

Vb=int(analogRead(A3)*XVref/1000L);

// Verifica che il contenuto della EEPROM non sia stato azzerato dalla programmazione con EESAVE non attivo;
// in caso di azzeramento (tutti a 0xFF=255: controllo solo il primo) carica nella EEPROM i valori di default:
if(EEPROM.read(0)==255)
  {
  Contr=80;
  Bip(); delay(100); Bip(); delay(100); Bip(); delay(100); Bip();
  delay(600); // Aspetta che il PWM regoli il contrasto
  lcd.setCursor(0,0); lcd.print(" EEPROM VERGINE!");
  lcd.setCursor(0,1); lcd.print("Carico i default");
  EEPROM.update(0,9); // Lim. iniz.: 130
  EEPROM.update(1,1); // Preset 1: 50
  EEPROM.update(2,5); // Preset 2: 90
  EEPROM.update(3,9); // Preset 3:130
  EEPROM.update(5,1); // k low byte
  EEPROM.update(6,0); // k high byte
  EEPROM.update(7,2); // Bip: preset e menu
  EEPROM.update(8,1); // Bip VMAX: Sì
  EEPROM.update(9,100);// Corr. fine: 100
  EEPROM.update(10,3); // Quanti preset?: 3
  EEPROM.update(11,1); // Ult: 1
  EEPROM.update(12,1); // BGR: 1
  EEPROM.update(13,25); // Contr: 80
  EEPROM.update(14,30); // VSB (Vel. di scarica della batteria): 30mV/h
  EEPROM.update(15,1); // 1: Bussola; 0: Nord sempre in alto
  delay(4000);
  lcd.clear();
  }  
  
k=EEPROM.read(5) + (EEPROM.read(6)<<8); // Rapporto k=F/V
B=EEPROM.read(7); // Bip on/off
BVM=EEPROM.read(8); // Bip-Bip a VMAX on/off
h=EEPROM.read(9); // Corrrezione fine della velocità
qp=EEPROM.read(10); // Quanti Preset
Ult=EEPROM.read(11); // Se è 1, è attiva la memorizzazione allo spegnimento 
                     // e la rilettura all'accensione dell'ultima velocità impostata
BGR=EEPROM.read(12); // Se è 1, attiva il Bip all'accensione del Giallo e del Rosso.
Contr=EEPROM.read(13); // Carica il valore del contrasto
VSB=EEPROM.read(14); // Carica la velocità di scarica della batteria
Bussola=EEPROM.read(15); // Carica l'impostazione della bussola (!: Bussola; 0: Nord sempre in alto

analogWrite(11,Contr); // Applica il contrasto memorizzato

// Simboli:
  lcd.createChar(0, noFix); // Carica il carattere "NF" verticale nella RAM del display.
  lcd.createChar(1, cerchietto); // Carica il carattere "°" nella RAM del display.
  lcd.createChar(2, spina); // Carica il carattere di carica in corso nella RAM del display.
 
// Per la parte del GPS:   
  GPS.begin(9600);        //Starting GPS at a 9600 baud rate
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Sending command for RMCGGA output: recommended minimum and GGA (fix data)
  // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);    //Sending command for 1Hz update rate
  // GPS.sendCommand(PGCMD_ANTENNA);               //Setting External Antenna
  //GPS.sendCommand("$PMTK251,14400*27<CR><LF>");
  //mySerial.println("$PMTK251,14400*27<CR><LF>");  
  //delay(1000); 
  useInterrupt(true);       //Setting useInterrupt true
if(digitalRead(10)==0) taratura();

for(n=16;n>0;n--)
   {
    lcd.setCursor(n,0);
    lcd.print("Corri sicuro!  ");
    lcd.setCursor(38-n,0); lcd.print("        "+ver); // (40,0) è il primo carattere della seconda riga =(0,1)
    delay(25);
   }
Von();  // Spento >> LED V
delay(100);
Voff(); Gon(); // LED V >> LED G
delay(100);
Goff(); Ron(); // LED G >> LED R
delay(100);
Roff(); Gon(); // LED R >> LED G
delay(100);
Goff(); Von(); // LED G >> LED V
delay(100);
Voff();  // LED V >> Spento
  
// lcd.clear(); lcd.print("  Corri sicuro! "); lcd.setCursor(6,1); lcd.print(ver);
delay(2000);
if(B>1) Bip();
    
Z=EEPROM.read(0); // Legge l'indice del Limite di Velocità iniziale
Zo=Z;
LIM=L[Z];
L1=LIM*0.93;
if (LIM<=100) {MAX=LIM+5;}
else {MAX=LIM*1.05;}
if(LIM<100) {StrLim=" "+String(LIM);}
else {StrLim=String(LIM);}

lcd.clear();
LCDmask();
}
