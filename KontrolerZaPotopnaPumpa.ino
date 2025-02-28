// Kristijan Toleski 15/02/2025

/*

  Definicija na menia:
    - Meni 0: Postavuvanje pritisok na uklucuvanje
    - Meni 1: Postavuvanje pritisok na isklucvenje

  Definicija na greski:
    - Greska 0: Vrednosta na senzorot e pod negoviot minimalen izlez
    - Greska 1: Vrednosta na senzorot e nad negoviot maksimalen izlez
    - Greska 2: Pumpata raboti podolgo vreme od maksimalno rabotno vreme na pumpata

*/
  
// Definicija na pinoj
int senzorZaPritisokPin = A0;

int potopnaPumpaPin = 2;

int ledZaGreskaPin = 3;

int dugme1Pin = 12;
int dugme2Pin = 13;

int aSegmentPin = 4;
int bSegmentPin = 5;
int cSegmentPin = 6;
int dSegmentPin = 7;
int eSegmentPin = 8;
int fSegmentPin = 9;
int gSegmentPin = 10;
int dpSegmentPin = 11;

int cifra1Pin = A5;
int cifra2Pin = A4;
int cifra3Pin = A3;
int cifra4Pin = A2;

// Promenlivi
int pritisokNaUklucvenje = 2700; // Vo milibari
int pritisokNaIsklucvenje = 6700; // Vo milibari

int momentalnaCifra = 1;

int brojNaGreska;

int pritisokZaNa7Segmenten = 0;

int programModeMeniIndex = 0; // 0 - pritisok na ukl., 1 - pritisok na iskl.
int programModeTempPodatok = pritisokNaUklucvenje;

byte sobiracZaTempPodatok = 1;

unsigned int brojNaCitanja = 0;

unsigned long vremeNaPritiskanje = 0;
unsigned long poslednaAktivnost = 0;
unsigned long poslednoUklucuvanjeNaPumpa = 0;

bool imatGreska = false;

bool potopnaPumpaUklucena = false;

bool Ekran7SegmentenUklucen = true;

bool dugme1Status, dugme2Status;
bool imatDugmeKlik = false;
bool dugme1PoslednoKlik = false;

bool programMode = false;

// Konstanti
const int maksimalenPritisokNaSenzor = 10000; // Vo milibari
const int minimalenIzlezNaSenzor = 102;
const int maksimalenIzlezNaSenzor = 922;

const int intervalNaObnovaNa7Segmenten = 50;

const int vremeNaDrzenjeNaDugminjaZaProgramMode = 5000; // Vo milisekundi
const int vremeNaDrzenjeNaDugmeZaPromenaNaMeni = 1500; // Vo milisekundi
const int vremeNaDrzenjeNaDugmeZaKlik = 100; // Vo milisekundi

const int intervalZaIzlezOdProgramMode = 30000; // Vo milisekundi

const int intervalZaResetiranjeSobirac = 1000; // Vo milisekundi

const int brojNaMenia = 2;

const int opsegNaSenzor = maksimalenIzlezNaSenzor - minimalenIzlezNaSenzor;

const unsigned int intervalNaIsklucuvanjeNa7Segmenten = 65535; // Minimum 0, maksimum 65535

const long maksimalnoRabotnoVremeNaPumpa = 10000; // Vo milisekundi

const char programModeKarakteri[] =  {'F', 'F', 'F', 'F'};
const char nanapredKarakteri[] = {' ', ' ', '=', '-'};
const char nanazadKarakteri[] =  {'-', '=', ' ', ' '};
const char zacuvajKarakteri[] =  {'_', '_', '_', '_'};

void setup() {
  //Serial.begin(9600);
  //intervalNaIsklucuvanjeNa7Segmenten--;

  pinMode(potopnaPumpaPin, OUTPUT);
  digitalWrite(potopnaPumpaPin, 0);

  pinMode(ledZaGreskaPin, OUTPUT);
  digitalWrite(ledZaGreskaPin, 0);

  pinMode(dugme1Pin, INPUT_PULLUP);
  pinMode(dugme2Pin, INPUT_PULLUP);

  pinMode(aSegmentPin, OUTPUT);
  pinMode(bSegmentPin, OUTPUT);
  pinMode(cSegmentPin, OUTPUT);
  pinMode(dSegmentPin, OUTPUT);
  pinMode(eSegmentPin, OUTPUT);
  pinMode(fSegmentPin, OUTPUT);
  pinMode(gSegmentPin, OUTPUT);
  pinMode(dpSegmentPin, OUTPUT);

  digitalWrite(aSegmentPin, 0);
  digitalWrite(bSegmentPin, 0);
  digitalWrite(cSegmentPin, 0);
  digitalWrite(dSegmentPin, 0);
  digitalWrite(eSegmentPin, 0);
  digitalWrite(fSegmentPin, 0);
  digitalWrite(gSegmentPin, 0);
  digitalWrite(dpSegmentPin, 0);

  pinMode(cifra1Pin, OUTPUT);
  pinMode(cifra2Pin, OUTPUT);
  pinMode(cifra3Pin, OUTPUT);
  pinMode(cifra4Pin, OUTPUT);

  digitalWrite(cifra1Pin, 0);
  digitalWrite(cifra2Pin, 0);
  digitalWrite(cifra3Pin, 0);
  digitalWrite(cifra4Pin, 0);
}

void loop() { 
  // Rabota so dugme 1 i 2 (0 - off, 1 - on)
  dugme1Status = !digitalRead(dugme1Pin);
  dugme2Status = !digitalRead(dugme2Pin);

  if (dugme1Status || dugme2Status){
    // Uklucuvanje na 7Segmenten
    Ekran7SegmentenUklucen = true;
    brojNaCitanja = 0;
    poslednaAktivnost = millis();

    if (dugme1Status && !dugme2Status) dugme1PoslednoKlik = true;
    else if (!dugme1Status && dugme2Status) dugme1PoslednoKlik = false;
    
    if (!programMode){
      if (dugme1Status && dugme2Status){
        if (vremeNaPritiskanje == 0) vremeNaPritiskanje = millis();
        else if (vremeNaDrzenjeNaDugminjaZaProgramMode <= millis() - vremeNaPritiskanje){
          programMode = true;

          unsigned long tempMillis = millis();
          while(millis() - tempMillis <= 500){
            PrikaziKarakteri(programModeKarakteri);
            delay(15);
          }

          vremeNaPritiskanje = millis();
        }
      }
      else vremeNaPritiskanje = millis();
    }
  }
  else{
    if (vremeNaDrzenjeNaDugmeZaKlik <= millis() - vremeNaPritiskanje) imatDugmeKlik = true;

    vremeNaPritiskanje = 0;
  }

  // Program mode
  if (programMode){
    if (millis() - poslednaAktivnost >= intervalZaIzlezOdProgramMode) programMode = false;
    if (millis() - poslednaAktivnost >= intervalZaResetiranjeSobirac) sobiracZaTempPodatok = 1;

    // Nanapred i nanazad po program menia
    if (dugme1Status || dugme2Status){
      if (vremeNaDrzenjeNaDugmeZaPromenaNaMeni <= millis() - vremeNaPritiskanje){
        char karakteriZaMeni[4] = {'F', ' ', ' '};

        if (dugme1Status && !dugme2Status){
          if (programModeMeniIndex <= 0) programModeMeniIndex = brojNaMenia - 1;
          else programModeMeniIndex--;

          unsigned long tempMillis = millis();
          while(millis() - tempMillis <= 500){
            PrikaziKarakteri(nanazadKarakteri);
            delay(15);
          }
          tempMillis = millis();
          karakteriZaMeni[3] = programModeMeniIndex + 48;
          while(millis() - tempMillis <= 500){
            PrikaziKarakteri(karakteriZaMeni);
            delay(15);
          }
        }
        else if (!dugme1Status && dugme2Status){
          if (programModeMeniIndex >= brojNaMenia - 1) programModeMeniIndex = 0;
          else programModeMeniIndex++;

          unsigned long tempMillis = millis();
          while(millis() - tempMillis <= 500){
            PrikaziKarakteri(nanapredKarakteri);
            delay(15);
          }
          tempMillis = millis();
          karakteriZaMeni[3] = programModeMeniIndex + 48;
          while(millis() - tempMillis <= 500){
            PrikaziKarakteri(karakteriZaMeni);
            delay(15);
          }
        }
        else{
          if (programModeMeniIndex == 0) pritisokNaUklucvenje = programModeTempPodatok;
          else if (programModeMeniIndex == 1) pritisokNaIsklucvenje = programModeTempPodatok;

          unsigned long tempMillis = millis();
          while(millis() - tempMillis <= 750){
            PrikaziKarakteri(zacuvajKarakteri);
            delay(15);
          }
        }

        if (programModeMeniIndex == 0) programModeTempPodatok = pritisokNaUklucvenje;
        else if (programModeMeniIndex == 1) programModeTempPodatok = pritisokNaIsklucvenje;

        vremeNaPritiskanje = millis();
      }     
    }
    else vremeNaPritiskanje = millis();

    if (imatDugmeKlik){
      // Nanapred postavuvanje vrednost
      if (dugme1PoslednoKlik){
        programModeTempPodatok -= sobiracZaTempPodatok;
        if (programModeTempPodatok < 0) programModeTempPodatok = 0;
      }
      // Nanazad postavuvanje vrednost
      else{
        programModeTempPodatok += sobiracZaTempPodatok;
        if (programModeTempPodatok > 9999) programModeTempPodatok = 9999;
      }

      sobiracZaTempPodatok++;

      imatDugmeKlik = false;
    }

    PrikaziBrojNa7Segmenten(programModeTempPodatok);
    delay(5);

    return;
  }

  // Odenje vo rezim na greska
  if (imatGreska){
    digitalWrite(ledZaGreskaPin, 1);
    digitalWrite(potopnaPumpaPin, 0);
    potopnaPumpaUklucena = false;

    digitalWrite(cifra3Pin, 1);
    digitalWrite(cifra2Pin, 1);
    digitalWrite(dpSegmentPin, 0);

    PrikaziGreska();
    delay(10);
    return;
  }

  // Citanje senzor
  int vrednostNaSenzor = analogRead(senzorZaPritisokPin);
  if (brojNaCitanja < intervalNaIsklucuvanjeNa7Segmenten) brojNaCitanja++;

  // Presmetvenje pritisok od senzor
  int pritisok = ((vrednostNaSenzor - minimalenIzlezNaSenzor) / (float)opsegNaSenzor) * maksimalenPritisokNaSenzor; // Pritisokot e vo milibari
  if (brojNaCitanja % intervalNaObnovaNa7Segmenten == 0) pritisokZaNa7Segmenten = pritisok;

  // Detekcija na greska na senzor (Greski 0 i 1)
  if (vrednostNaSenzor < minimalenIzlezNaSenzor || vrednostNaSenzor > maksimalenIzlezNaSenzor){
    imatGreska = true;
    brojNaGreska = 0;
    if (vrednostNaSenzor > maksimalenIzlezNaSenzor) brojNaGreska = 1;

    return;
  }

  // Uklucvenje pumpa
  if (!potopnaPumpaUklucena && pritisok <= pritisokNaUklucvenje){
    digitalWrite(potopnaPumpaPin, 1);
    potopnaPumpaUklucena = true;
    poslednoUklucuvanjeNaPumpa = millis();
  }
  else if (potopnaPumpaUklucena && pritisok >= pritisokNaIsklucvenje){
    digitalWrite(potopnaPumpaPin, 0);
    potopnaPumpaUklucena = false;
  }
  // Detekcija na greska na pumpa (Greska 2 i 3)
  else if (potopnaPumpaUklucena){
    if(millis() - poslednoUklucuvanjeNaPumpa >= maksimalnoRabotnoVremeNaPumpa){
      imatGreska = true;
      brojNaGreska = 2;
    }
    //else if () // Greska 3 TO DO
  }

  // Prikazuvanje pritisok na 7 segmenten so 4 cifri
  if (brojNaCitanja < intervalNaIsklucuvanjeNa7Segmenten && !programMode) PrikaziBrojNa7Segmenten(pritisokZaNa7Segmenten);
  else if (Ekran7SegmentenUklucen && brojNaCitanja >= intervalNaIsklucuvanjeNa7Segmenten) Izgasi7Segmenten();

  /* Pustanje na podatocite na USB
  Serial.println("----------------------------------");
  Serial.print("Vrednost na senzor: ");
  Serial.println(vrednostNaSenzor);

  Serial.print("Pritisok: ");
  Serial.println(pritisok);

  Serial.print("Potopna pumpa: ");
  Serial.println(potopnaPumpaUklucena);*/

  delay(5);
}

void Izgasi7Segmenten(){
  digitalWrite(aSegmentPin, 0);
  digitalWrite(bSegmentPin, 0);
  digitalWrite(cSegmentPin, 0);
  digitalWrite(dSegmentPin, 0);
  digitalWrite(eSegmentPin, 0);
  digitalWrite(fSegmentPin, 0);
  digitalWrite(gSegmentPin, 0);
  digitalWrite(dpSegmentPin, 0);

  digitalWrite(cifra1Pin, 0);
  digitalWrite(cifra2Pin, 0);
  digitalWrite(cifra3Pin, 0);
  digitalWrite(cifra4Pin, 0);

  Ekran7SegmentenUklucen = false;
}

void AktivirajSegmentPoBroj(int broj){
  switch(broj){
    case 0:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 1);
      digitalWrite(fSegmentPin, 0);
      digitalWrite(gSegmentPin, 1);
      break;

    case 1:
      digitalWrite(aSegmentPin, 0);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 0);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 0);
      digitalWrite(gSegmentPin, 0);
      break;

    case 2:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 0);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 1);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 0);
      break;

    case 3:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 0);
      break;

    case 4:
      digitalWrite(aSegmentPin, 0);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 0);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 1);
      break;

    case 5:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 1);
      break;

    case 6:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 1);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 1);
      break;

    case 7:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 0);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 0);
      digitalWrite(gSegmentPin, 0);
      break;

    case 8:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 1);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 1);
      break;

    case 9:
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 1);
      digitalWrite(cSegmentPin, 1);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 1);
      break;
  }
}
void AktivirajSegmentPoKarakter(char bukva){
  if (isDigit(bukva)) AktivirajSegmentPoBroj((int)bukva - 48);

  switch(bukva){
    case ' ':
      digitalWrite(aSegmentPin, 0);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 0);
      digitalWrite(dSegmentPin, 0);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 0);
      digitalWrite(gSegmentPin, 0);
      digitalWrite(dpSegmentPin, 0);
      break;
    case 'E':
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 0);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 1);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 1);
      digitalWrite(dpSegmentPin, 0);
      break;
    case 'F':
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 0);
      digitalWrite(dSegmentPin, 0);
      digitalWrite(eSegmentPin, 1);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 1);
      digitalWrite(dpSegmentPin, 0);
      break;
    case '_':
      digitalWrite(aSegmentPin, 0);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 0);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 0);
      digitalWrite(gSegmentPin, 0);
      digitalWrite(dpSegmentPin, 0);
      break;
    case '=':
      digitalWrite(aSegmentPin, 1);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 0);
      digitalWrite(dSegmentPin, 1);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 0);
      digitalWrite(gSegmentPin, 0);
      digitalWrite(dpSegmentPin, 0);
      break;
    case '-':
      digitalWrite(aSegmentPin, 0);
      digitalWrite(bSegmentPin, 0);
      digitalWrite(cSegmentPin, 0);
      digitalWrite(dSegmentPin, 0);
      digitalWrite(eSegmentPin, 0);
      digitalWrite(fSegmentPin, 1);
      digitalWrite(gSegmentPin, 0);
      digitalWrite(dpSegmentPin, 0);
      break;
  }
}

void PrikaziBrojNa7Segmenten(int pritisok){
  if (momentalnaCifra == 1){
    momentalnaCifra++;    

    digitalWrite(cifra4Pin, 1);
    digitalWrite(dpSegmentPin, 1);

    int cifra = pritisok / 1000;

    AktivirajSegmentPoBroj(cifra);
    digitalWrite(cifra1Pin, 0);
  }
  else if (momentalnaCifra == 2){
    momentalnaCifra++;

    digitalWrite(dpSegmentPin, 0);
    digitalWrite(cifra1Pin, 1);  

    int cifra = pritisok / 100 % 10;

    AktivirajSegmentPoBroj(cifra);
    digitalWrite(cifra2Pin, 0);
  }
  else if (momentalnaCifra == 3){
    momentalnaCifra++;

    digitalWrite(cifra2Pin, 1);  

    int cifra = pritisok / 10 % 10;

    AktivirajSegmentPoBroj(cifra);
    digitalWrite(cifra3Pin, 0);
  }
  else{
    momentalnaCifra = 1;

    digitalWrite(cifra3Pin, 1);     

    int cifra = pritisok % 10;

    AktivirajSegmentPoBroj(cifra);
    digitalWrite(cifra4Pin, 0);  
  }
}

void PrikaziGreska(){
  if (momentalnaCifra == 1){
    momentalnaCifra = 4;

    digitalWrite(cifra4Pin, 1);

    AktivirajSegmentPoKarakter('E');

    digitalWrite(cifra1Pin, 0);
  }
  else{
    momentalnaCifra = 1;

    digitalWrite(cifra1Pin, 1);     

    AktivirajSegmentPoBroj(brojNaGreska);
    digitalWrite(cifra4Pin, 0);  
  }
}

void PrikaziKarakteri(char cetiriKarakteri[]){
  if (momentalnaCifra == 1){
    momentalnaCifra++;    

    digitalWrite(cifra4Pin, 1);
    AktivirajSegmentPoKarakter(cetiriKarakteri[0]);
    digitalWrite(cifra1Pin, 0);
  }
  else if (momentalnaCifra == 2){
    momentalnaCifra++;

    digitalWrite(cifra1Pin, 1);  
    AktivirajSegmentPoKarakter(cetiriKarakteri[1]);
    digitalWrite(cifra2Pin, 0);
  }
  else if (momentalnaCifra == 3){
    momentalnaCifra++;

    digitalWrite(cifra2Pin, 1);  
    AktivirajSegmentPoKarakter(cetiriKarakteri[2]);
    digitalWrite(cifra3Pin, 0);
  }
  else{
    momentalnaCifra = 1;

    digitalWrite(cifra3Pin, 1);     
    AktivirajSegmentPoKarakter(cetiriKarakteri[3]);
    digitalWrite(cifra4Pin, 0);  
  }
}






