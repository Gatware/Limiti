void Bip()
{
PORTC|=B00000010;
delay(30);
PORTC&=B11111101;
}

void Bippino()
{
PORTC|=B00000010;
delay(10);
PORTC&=B11111101;
}

// ------- Pilotaggio dei LED a catodo comune (chiusura a Vcc) ------- 
void Von()  {PORTD|=B00000010;}
void Voff() {PORTD&=B11111101;}
void Gon()  {analogWrite(5,255);} // Essendo G e R accesi in PWM, il PORTD&=... non funziona!
void Goff() {analogWrite(5,0);}
void Ron()  {analogWrite(6,255);}
void Roff() {analogWrite(6,0);}
void GRoff(){analogWrite(5,0); analogWrite(6,0);}
