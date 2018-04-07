
void encoder()
{
S=3-(PIND>>2)&B00000011; // Gli I/O 2 e 3 sono PD2 e PD3, perciò scorro a destra di 2.
// Il valore binario di S rappresenta A e B. Il centrale dell'encoder è a massa,
                    // quindi faccio complemento a 3 (11)  
S^=S>>1; // Faccio XOR (^) fra S (gray) e il suo bit 1, facendolo scorrere a Dx: AB XOR A,
         // ottenendo un binario che per ogni scatto fa 0-1-2-3-0 oppure 0-3-2-1-0.
if (S!=So && S==0) X=0;
if (X==0)
  {
  if (So==1&&S==2)
    {E=1; X=1; if(B==3)Bip();}
  if (So==3&&S==2)
    {E=-1; X=1; if(B==3)Bip();}
  if (S==0)
    {E=0; X=0;}
  So=S;  
  }
}

void CalcoloLimiti()
{
LIM=L[Z];
L1=LIM*0.93;

if(LIM<=100) {MAX=LIM+5;}
else {MAX=LIM*1.05;}

if(LIM<100) StrLim=" "+String(LIM);
else StrLim=String(LIM);
}
