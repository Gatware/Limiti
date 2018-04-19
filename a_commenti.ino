// 2.4 Ho aggiunto il Bip()
// 2.4a Ho usato gli else negli if dei LED
// 2.4e Ho aggiunto la monitoria a fLoop/2 su A2: fLoop/2=70Hz circa.
// 2.4e1 Ho velocizzato un po' la scrittura su LCD evitando di ripetere la scrittura 
//        di VEL= e LIM= a ogni ciclo. fLoop/2=124Hz circa.
// 2.4f Ho velocizzato ancora scrivendo sul display il valore della velocità solo 
//       una volta ogni 500mS, quando misura la velocità. fLoop/2=220~250Hz circa (220Hz a V=0).
// 2.4g Ho aggiunto la correzione fine.
// 2.4h Ho aggiunto l'impostazione della quantità di Preset da 1 a 4 e ho corretto le formule con h e k,
//       che non funzionavano per problemi di interi e simili.
// 2.5  Ho aggiunto la parte del GPS
// 2.5a Ho aggiustato gli spazi e corretto qualcosa riguardo fix, decimali e posizioni delle velocità.
// 2.5a1 Ho aggiunto la visualizzazione di Ora e Data nel menu Taratura
// 2.5a2 Ho eliminato la velocità media, ponendo vm=vd, se no la variazione rallenta troppo perché già il modulo GPS fa la media.
//       Ho sistemato la visualizzazione dell'ora e ho aggiunto la data.
// 2.5b Provo ad aggiungere la possibilità di memorizzare il limite corrente come iniziale quando 
//       la velocità scende al di sotto di 6km/h. Non funziona. E' complesso... >:(
// 2.5a3 27/11/15 Ho modificato la visualizzazione del numero di satelliti per cancellare la prima cifra dopo aver indicato 10 o più.
// 2.5b1 27/11/15 Ho iniziato la memorizzazione dell'ultimo limite impostato (dopo 10sec. senza variazioni), ma senza partire dalla 2.5b! 
//       Ho portato la velocità minima rilevata a 5km/h, poiché spesso da fermo indicava 2~3km/h [245]
//       Ho messo a tutte le voci del menu l'uscita tenendo premuto per 1 sec.
//       Ho disegnato il ° per i gradi, poiché quello disponibile è quadrato!
//                    Sto lavorando alla [202] per la memorizzazione dell'ultimo limite.
// 2.5b2 28/11/15 Ho sistemato la visualizzazione della data e dell'altitudine.
// 2.5b3 28/11/15 Uso una forma più pulita, con un while loop, per l'identificazione della pressione lunga.
// 2.5b5a 1/12/15 Ho corretto errori che avevo aggiunto all'ora e alla visualizzazione della velocità. Inoltre adesso velocità minori di 
//                 5km/h, oltre a non venire visualizzate, vengono considerate 0km/h anche nel pilotaggio dei LED.
// 2.5c   1/12/15 Compatto la parte dei Preset nel menu con il contatore n. Funziona!
// 2.5c1  1/12/15 Provo la memorizzazione dell'ultima velocità quando manca tensione su A3 (abbandonato, perché se il condensatore di 
//                 memoria poi non si scarica sufficientemente, Arduino non si riavvia e va resettato a mano).
// 2.5c2  3/12/15 Ho messo il menu2 in cascata dal menu1 e ho risolto il problema dell'uscita dal menu2 con il goto alla riga 14 del menu1.
//                Sembra funzionare tutto!
// 2.5c3  4/12/15 Ho corretto errori negli spazi prima della velocità rilevata.
// 2.5d   9/12/15 Ho aggiunto l'acquisizione della velocità corrente come limite, chiudendo a massa I/O 0.
// 2.5e   9/12/15 Ho aggiunto la lettura dello stato dell'elemento al litio.
// 2.5e1 10/12/15 Ho rinominato la funzione Batt in BattIco e ho aggiunto BattVolt, che visualizza la tensione.
//                Nell'LCDmask() ho aggiunto la visualizzazione del Preset (se PN>0) per visualizzarlo a ogni ritorno alla pagina principale.
// 2.5e1b 10/12/15 Uguale alla 2.5e1 tranne che Pr, velocità, limite e relativi valori appaiono più a destra di 1 posizione (da 9 a 10). Non mi piace.
// 2.5e2 10/12/15 Uso 2 set di caratteri per fare tutte le icone della batteria (1 set ha al massimo 8 caratteri, tra cui ho già NF e °).
// 2.5e3   1/1/15 Ora non solo se V=0 spegne tutti i LED, ma anche se non c'è fix! Spegne i LED anche durante la visualizzazione di ora e data
//                 e delle coordinate, poiché la velocità non viene rilevata.
// 2.5e4  21/3/16 Ho aggiunto nella scheda j_OraeData il controllo dell'anno letto dal GPS: se è 80 (1980), vuol dire che l'orologio del modulo GPS
//                 si è azzerato: in tal caso al posto di ora e data viene visualizzato -- -- --. Qualora l'anno sia diverso da 80, appaiono ora e data.
// 2.5e5  22/3/16 Ho aggiunto il riporto dei giorni con l'ora legale e con il fuso orario. L'ho spostato da j_OraeData a l_OraLegale.
// 2.5e5a 23/3/16 Come 2.5e5, tranne per la data a lunghezza variabile senza aggiungere spazi in mezzo quando giorno e/o mese sono di una cifra.
// 2.5e6  24/3/16 Proseguo dalla 2.5e5a aggiungendo l'indicatore di carica in corso. Per fare questo devo liberare un'icona, quindi dispongo diversamente
//                 quelle della batteria. Il controllo della carica avviene ogni 500mS e l'icona viene scritta o cancellata solo se lo stato è cambiato.
//                Per alleggerire questa scheda, sposto Variabili e GPS in una separata, eccetto la versione. 
//                L'indicazione del Preset dovrà avvenire su due righe, per fare spazio all'indicazione di carica.
//        27/3/16 A IDE 1.6.8 non piace la scheda Variabili_GPS separata... >:(
// 2.5e7  27/3/16 IDE 1.6.8 non gradisce che la scheda con il nome del file porti solo i commenti e gli Include. Ho messo i commenti nella 2a scheda.
//                La cancellazione del Preset dovrà anch'essa avvenire su due righe!
//                Timeout e t3, enttrambe unsigned long, non usando la frequenza per misurare la velocità, non servono più.
//                Visualizza o cancella la spina solo se è appena stata collegata o staccata.
//                VbH e bat vanno benissimo di tipo byte.
//                Ho tolto l'lcd.begin in LCDmask per (Vb>500), che cancellava la "U" di Ult (se attivo) appena iniziato il loop!
// 2.5e8  30/3/16 Ho rimesso l'ADC veloce in b_Setup, che avevo già messo nella prima v2.5e7 ma ho dimenticato quando l'ho riscritta a memoria.
// 2.5e9   1/4/16 Ho corretto il voltmetro in m_Batteria, che non indicava lo "0" come prima cifra dopo la virgola e trasformava 1...9 centesimi in decimi di Volt!
//                Ho spostato la visualizzazione nel display dell'altitudine da Ora e Data a Coordinate.
//                Ho ridotto l'intervallo di aggiornamento dell'icona della batteria da 2 minuti a 30 secondi.
//                Ho aumentato l'intervallo di aggiornamento della tensione della batteria da 1 a 2 secondi.
//                Ho corretto un errore nell'algoritmo di calcolo e visualizzazione della tensione della batteria in BattVolt(), per cui sbagliava
//                per Vb da 815 a 819, saltando (mi sembra) da 4.1 a 3.99 (!)
//  c.s.   2/4/16 Ho aggiunto lo scrolling delle scritte iniziali.
// 2.5f    3/4/16 Ho rimesso l'altitudine in OraeData e ho aggiunto in Coordinate il calcolo della distanza da un punto memorizzato.
// 2.5f1  21/4/16 Per alimentare il circuito direttamente dai 4,2...3V di un elemento al Litio (LCD permettendo...) bisogna dare all'ADC una tensione di riferimento
//                 di 2,5V (ad es. con un REF25Z) e la tensione della batteria che va al pin 26 (A3) dovrà passare attraverso un partitore realizzato 
//                 con due resistenze uguali (ad es. 1Mohm). Anche la tensione di carica che va al pin 27 (A4) dovrà passare in un partitore uguale.
//                Ho rallentato l'animazione nel Setup() portando il delay da 20 a 25.
// 2.5f2  24/4/16 Arduino dispone di un riferimento interno a 1,1V(!), selezionabile mediante analogReference(INTERNAL). Serviranno, quindi, partitori con rapporto
//                 di resistenza pari a 3,9:1,1, ad es. 1M2:330k (-1,3%) o 1M8:510k (+0,4%) o 1M+1M:560k (+0,7%). Scelgo 1M:270k.
// 2.5g   25/4/16 Aggiungo l'impostazione dela tensione di riferimento (all'inizio della dichiarazione delle variabili). Usando il riferimento interno, bisogna
//                 leggere con il multimetro la tensione sul pin Vref e scriverlo nella variabile Vref.
//  temp2 27/4/16 Se sul uC non è attivo EESAVE, una programmazione ISP azzera il contenuto della EEPROM. In tal caso, nel Setup() viene rilevato, appare un messaggio
//                 e vengono caricati i valori di default.
// 2.5h    1/5/16 Passo il led V da I/O 11 (pin 17) a I/O 1 (pin 3) per liberare un I/O PWM.
//         6/5/16 Ho corretto Vcar nel Loop(), che doveva essere diviso per 1000, e ho aggiustato sperimentalmente il valore di confronto di Vcar a 860.
//                Ho aggiunto il Bip() quando viene collegata l'alimentazione esterna e il doppio Bip() quando viene scollegata.
//                Poiché a volte al ritorno dal menu 2 si attiva un Preset, aggiungo un delay(500) nel Loop() dove ritorna.
//                Ho aggiunto la visualizzazione dell'alimentazione esterna in LCDmask, poiché all'uscita dal Menu2 non riappariva.
// 2.6    16/6/16 Tolgo la memorizzazione automatica del limite impostato dopo 10 sec. Adesso legge lo stato di A0: se vede che va alto, perché ho spento l'interruttore
//                 (che accende chiudendo a massa), memorizza il limite corrente e fa Bip. Per fare questo dovrò aggiungere un MOSFET per ritardare lo spegnimento.
//                Ho velocizzato la lettura di A0 mediante PINC & B00000001.
//                Ho abbreviato un po' (da 200+1000+200=1400mS a 100+800+100=1000mS) il BIIIIP in caso di pressione del pulsante ACQ (si chiamava MEM) con V<40km/h.
//                Devo risolvere il problema della distanza, che indica qualcosa come 70 volte di più del reale! L'algoritmo è giusto, perché facendo i calcoli viene giusto.
// 2.6a   20/6/16 Ho sostituito il BIIIIP di cui sopra con 3 Bippini a 100mS uno dall'altro.
// 2.7    20/6/16 Aggiungo la possibilità di Bip all'accensione di Giallo e Rosso.
//                Ho corretto l'errore nella misura della distanza: GPS.latitude e GPS.longitude sono espressi in GRADI*100 + minuti e decimi di minuto!
// 2.7a   20/6/16 Aggiungo un Bip all'acquisizione del punto per misurare la distanza
// 2.8    20/6/16 Aggiungo la visualizzazione della direzione di marcia (frecce)
// 2.8a   21/6/16 Aggiungo la correzione manuale del contrasto in Menu1.
// 2.8b   21/6/16 Aggiungo la visualizzazione della direzione anche nella pagina principale. Per questo l'ho spostata in una routine nella scheda Coordinate.
// 2.8c   22/6/16 Nella pagina principale blocco la freccia per V<0,5km/h.
// 2.8d   22/6/16 Alla misura della tensione aggiungo l'autonomia. L'assorbimento è 45~50mA, quindi sono circa 50h a piena carica.
// 2.8e   23/6/16 Aggiungo in Taratura l'impostazione della capacità della batteria in base alla velocità di scarica in mV/h.
//                Abbasso la soglia di carica da 870 a 860 (da 4,6 a 4,55V) poiché, collegato al computer con una prolunga, pur caricando, non indica in carica.
// 2.8f   24/6/16 Aggiungo in Taratura() l'impostazione dell'indicatore di direzione (Bussola): bussola oppure Nord sempre in alto
//                Nelle impostazioni Sì/No dei menu tolgo l'alternanza Sì/No ad ogni click, limitando a 0 a sinistra e a 1 a destra.
// 2.8g   25/6/16 Abbandonata
// 2.8h   26/6/16 Riprendo lo switch/case per la direzione dalla 2.8g. La modalità Bussola dava indicazioni errate. Adesso non va a modificare GPS.angle ma il valore di Dir.
//                Correggo il Bip per Giallo e Rosso, che suonava ininterrottamente nella 2.8f, aggiungendo le variabili LEDG e LEDR per indicare che il LED G o R è già
//                 acceso, perciò ha già fatto Bip.
//                Faccio le frecce Nord/Est/Sud/Ovest con la punta più piccola, perché è sproporzionata rispetto alle altre. NO: troppo piccole! :-)
//                In Taratura() per Bussola rimetto l'alternanza poiché, non avendo un valore numerico, non si sa in che verso ruotare.
// 2.8i   27/6/16 Modifico il calcolo dell'autonomia, poiché aveva una risoluzione di 3 ore! Moltiplicando per 1000 prima, ottengo una risoluzione di 1 ora.
//                Ho eliminato il caricamento delle 4 icone per volta in BattIco() (m_batteria). Adesso carica solo, di volta in volta, l'icona che serve.
//                Ho messo in BattIco() anche la riga di disegno dell'icona, che prima stava fuori dalla routine.
// 2.8j    6/7/16 Sposto la regolazione del contrasto da taratura() a Impostazione del contrasto in menu 2 prolungato (pressione di almeno 4 sec.)
//                Ho migliorato i menu: 1 bip:menu1; 2 bip:menu2; 3 bip:contrasto. Adesso il nome del menu appare subito, prima del Bip.
// 2.9     6/7/16 Parte dalla 2.8j - Versione senza contrasto automatico per alimentazione di display e processore con convertitore a 5V.
//                Contrasto (da 0 a 100) fino a 10 a passi di 1, poi a passi di 5.
//                Ho eliminato la variabile "a" e ho usato direttamente GPS.altitude, poiché avevo posto a=GPS.altitude.
// 2.9a    8/7/16 Ho aggiunto l'animazione dei 3 LED all'accensione, dopo l'animazione sul display.
//                Ho aggiustato un po' le soglie delle icone della batteria.
//                Nel calcolo dell'autonomia residua introduco la corezione *Vb/859, che va da 1 a 0,8 durante la scarica della batteria, per considerare l'aumento
//                 della corrente assorbita causato dall'inserimento dell'alimentatore switching da V Litio a 5V.
//                Riduco la distanza dei Bip-Bip e Bip-Bip-Bip dei menu da 150mS a 100mS
// 2.9b   11/7/16 Aggiungo le centinaia di metri alla misura della distanza tra 1,0 e 9,9km.
//                Aggiungo B>0 per non far suonare il Bip nei menu che ho modificato nella v2.9 e aggiusto un po' i Bip sì o no.
//                Sposto il pilotaggio dei LED in delle routine in m_Batteria, per variare facilmente fra Catodo comune e Anodo comune.
// 2.9c   14/7/16 Come la v2.9b, ma in m_batteria abilito le routine per pilotare i LED al contrario (Anodo comune: accesi=uscita bassa) per usare un convertitore a 5V 
//                e un generatore di corrente costante dalla Vbat diretta.
// 2.9d   18/7/16 Modifico l'icona della batteria, aggiungendo il contenitore vuoto. Versione per LED a catodo comune.
// 2.9e   18/7/16 Modifico l'icona della batteria, aggiungendo il contenitore vuoto. Versione per LED ad anodo comune.
// 2.9f   29/8/16 Parto dalla 2.9d. Provo a usare gli interrupt per l'encoder. Per non dover cambiare le connessioni, uso la libreria pinChangeInterrupt, che permette
//                di usare pin arbitrari. Ho scoperto che va notoriamente in conflitto con SoftwareSerial e altro, perciò sposto l'encoder da I/O 8-9 a I/O 2-3
//                (pin da 14-15 a 4-5) e RS e EN dell'LCD da I/O 2-3 a I/O 8-9.
// 2.9g   22/9/16 Sposto il LED G e il LED R da I/O 12-13 a I/O 5-6 (pin da 18-19 a 11-12), poiché I/O 5 e I/O 6 hanno il PWM che mi serve per far oscillare l'intensità
//                del LED R quando i satelliti sono 0 e del LED G quando sono 1 o 2 o 3. D5 e D6 del display ora vengono alimentati da I/O 12-13 (pin 18-19).
// 2.9h   2/10/16 Poiché ancora i LED smettono di funzionare leggendo l'ora, le coordinate o altro, introduco 2 nuove funzioni: LEDNoFix e LEDFix in cui metto il pilotaggio
//                dei LED quando non c'è Fix e quando c'è per chiamarle ogni volta che serve.
//                Correggo anche lo spegnimento dei LED all'inizio dei menu, mettendo Voff() e GRoff() al posto del port management che non uso più causa il PWM.
// 2.10   2/10/16 Scambio menu1 con menu2 e azzero il conteggio del tempo di pressione del tasto all'inizio di ogni menu per contare sempre fino a 1000, anziché contare
//                fino a 1200, poi 2500 e poi a 4000.
// 2.11   17/1/17 Aggiungo il calcolo della distanza reale, non in linea d'aria, sommando le tratte ad ogni cambiamento di direzione.
// 3.1     9/4/18 Parte dalla 2.11 con le modifiche di /dev (arduino forum/github) per avere un'indicazione della sincronizzazione dell'ora quando ancora non è stato agganciato
//                completamente almeno un satellite e migliorarlo usando la sua libreria gps al posto di quella Adafruit.
//        15/4/18 Adesso cancella "Pr" e il numero di preset quando viene premuto il taso ACQ.
//                Adesso occuma meno memoria, meno RAM, però ancora non rileva la sincronizzazione dell'ora.
//        19/4/18 Finalmente, con la collaborazione di /dev, dopo aver anche letto e decodificato Tx e Rx del GPS con l'osclloscopio, il rilevamento della sincronizzazione
//                di ora e data sembra funzionare! :)

const char verChars[] PROGMEM = " v3.1";  // Don't print this variable
const __FlashStringHelper * ver = (const __FlashStringHelper *) verChars; // ok

/* La tolleranza sui limiti è del 5% o, comunque, di almeno 5km/h:
50 -> 55 (5km/h)
90 -> 95 (5km/h)
100->105 (5km/h=5%)
110->115 (5%)
120->126 (5%)
130->136 (5%)
 *
 * ----------- LCD -----------
 * LCD RS pin to I/O 8
 * LCD En pin to I/O 9
 * LCD D4 pin to I/O 4
 * LCD D5 pin to I/O 12
 * LCD D6 pin to I/O 13
 * LCD D7 pin to I/O 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD V0 pin to PWM I/O 11 via R+C
 * LCD VCC pin to 5V
 * 
 * ------- TUTTI I PIN --------
 * I/O 0 (pin 2) Pulsante ACQ
 * I/O 1 (pin 3) LED V
 * I/O 2 (pin 4) Encoder - A
 * I/O 3 (pin 5) Encoder - B
 * I/O 8 (pin 14) LCD RS
 * I/O 9 (pin 15) LCD En
 * I/O 4 (pin 6)  LCD D4
 * I/O 12 (pin 18)LCD D5
 * I/O 13 (pin 19)LCD D6
 * I/O 7 (pin 13) LCD D7
 * I/O 8 (pin 14) LCD RS
 * I/O 9 (pin 15) LCD En
 * I/O 10 (pin 16) Encoder - Pulsante
 * I/O 11 (pin 17) PWM per la regolazione del contrasto via 270 ohm e con 47uF a massa
 * I/O 5 (pin 11) LED G PD5
 * I/O 6 (pin 12) LED R PD6
 * Vref   (pin 21) Tensione di riferimento dell'ADC 1,1V nom. (+/-10%) 
 * I/O A0 (I/O 14) (pin 23) Verifica se spento (Gate del MOSFET a 1)
 * I/O A1 (I/O 15) (pin 24) Buzzer 5V
 * I/O A2 (I/O 16) (pin 25) 1/2 floop out
 * I/O A3 (I/O 17) (pin 26) Vbat in
 * I/O A4 (I/O 18) (pin 27) Tensione di carica in (via 10k o altra)
 * I/O A5 (I/O 19) (pin 28) GPS in (Rx)
 * 
 * ---------- EEPROM ----------
 * 0,1,2,3,4: Indici del limite iniziale (0) e dei 4 preset disponibili (1,2,3,4)
 *     5,6: Rapporto k =F/V: LOW byte (5) e HIGH byte (6)
 *       7: Bip Si/No
 *       8: Bip-Bip a VMAX Si/No
 *       9: k: correzione fine della velocità fra 50% e 150%
 *      10: qp: Quanti Preset 
 *      11: Ult 0/1: Se è 1, all'accensione viene richiamata l'ultima velocità impostata
 *      12: BGR: Bip Giallo e Rosso Si/No
 *      13: Contr: Correzione del contrasto -10...+10
 *      14: VSB: Velocità di scarica della batteria [mV/h]
 *      15: Bussola 0/1: Se è a 1, la freccia indica il Nord; se è a 0, il Nord sta fisso in alto.
 * ---------- CARATTERI LCD createChar ----------
 * 0: NoFix
 * 1: ° (rotondo, anzichè quello quadrato standard dell'LCD)
 * 2: spina
 * 3: freccia della direzione (viene caricata di volta in volta quella da visualizzare)
 * 4,5,6,7: 4 icone della batteria (vengono caricate le prime 4 o le seconde 4 secondo lo stato della batteria)
 * 
 
*/
