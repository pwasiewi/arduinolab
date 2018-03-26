String readString;    	//odczytany string
char parametr;        	//przechowuje wartosc parametru z (pierwszego znaku) z odebranego stringa
String odczytanyString;	//string odczytany z wejścia
String wartosc;       	//string bez pierwszego znaku - jako wartosc do parametru
int dlStringa;        	//pomocniczo przechowuje dlugosc odebranego stringa
int liczba;           	//wartosc zamieniona na liczbe
char a;
int wartoscR, wartoscB, wartoscG, wartoscRed;//wartości dla led-ow

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT); //RGB-niebieski
  pinMode(10, OUTPUT); //RGB-zielony
  pinMode(11, OUTPUT); //RGB-czerwony
  pinMode(6, OUTPUT); //LED-czerwona
}

void loop() 
{
  analogWrite(9, wartoscB); 
  analogWrite(10, wartoscG);
  analogWrite(11, wartoscR);  
  analogWrite(6, wartoscRed);

  czytajSerial();

  if (odczytanyString.length() >0){
    parametr = poznajParametr();
    liczba = poznajWartoscDlaParametru();
    wyborOpcji();
  }
  odczytanyString="";
}


// wyciagam co się da do oddzielnych funkcji

// czytanie z seriala i rozpoznawanie parametrow

void czytajSerial(){ //czytam serial
  while (Serial.available()) {
    delay(50);//opozniam o 50 ms, bo bez tego nie zawsze i nie wszystko było odczytywane tak jak powinno. Kazdy znak na wejsciu serial byl traktowany jak oddzielny string
    char c = Serial.read();
    readString += c;
    odczytanyString=readString;
  }
  readString="";
}

int poznajParametr(){ //rozpoznaje parametr
  Serial.println(odczytanyString); //testowe sprawdzenie co odczytał
  char parametr=odczytanyString[0];//odczytanie parametru zakladam ze parametr to pierwszy znak
  return parametr;
}

int poznajWartoscDlaParametru(){ // rozpoznaje wartosc parametru 
  dlStringa=odczytanyString.length();//sprawdzenie dlugosci stringa

  for (int i=1; i<dlStringa; i++){ //pobieram do stringa wartosci od drugiego znaku, to bedzie wartosc dla parametru
    char d = odczytanyString[i];
    wartosc += d;
  }
  int liczba = wartosc.toInt();//zmieniam stringa z wartosci na liczbe zeby sprawdzac warunkami czy np miesci sie w zakresie jasnosci diody 0-256


  wartosc="";
  return liczba;
}

//obsluga polecen

void ustawWartoscLed(char a){ 
  int barwa;
  char* kolor[]= {
    "RGB-czerwona","RGB-niebieski","RGB-zielony","czerwona"    };  
  int wartosc = liczba;
  if (wartosc < 0 || wartosc > 255) // sprawdzanie czy dobry zakres
  {
    return; // wyjscie z funkcji
  }
  if (a =='r'){
    barwa=0;
    wartoscR=wartosc;
  }
  else if (a =='b'){
    barwa=1;
    wartoscB=wartosc;
  }
  else if (a=='g'){
    barwa=2;
    wartoscG=wartosc;
  }
  else if (a=='c'){
    barwa=3;
    wartoscRed=wartosc;
  }

}

int wlWylLed(int b){ //jesli dioda pali sie choc odrobine to gasi ja na 0, jesli jest zgaszona zupelnie to zapala na max
  int wartoscWlWyl=b;
  if (wartoscWlWyl > 0){
    wartoscWlWyl=0; 
  }
  else if (wartoscWlWyl < 255){
    wartoscWlWyl=255; 
  }
  int w=wartoscWlWyl;
  return w;
}


void wyborOpcji() {
  switch (parametr)//odczytywanie z parametru co ma byc zrobione
  {

  case 'r'://ustawia jasnosc koloru R w diodzie led RGB na wartosc z parametru
    ustawWartoscLed('r');
    break; 
  case 'g'://ustawia jasnosc koloru G w diodzie led RGB na wartosc z parametru
    ustawWartoscLed('g');
    break;
  case 'b'://ustawia jasnosc koloru B w diodzie led RGB na wartosc z parametru
    ustawWartoscLed('b');
    break;
  case 'c'://ustawia jasnosc czerwonej led na wartosc z parametru
    ustawWartoscLed('c');
    break;
  case '0'://gasi wszystko na max
    wartoscR=0; 
    wartoscB=0;
    wartoscG=0;
    wartoscRed=0;
    break;
  case '1'://odpala wszystko na max
    wartoscR=255; 
    wartoscB=255;
    wartoscG=255;
    wartoscRed=255;
    break;
  default:
    break;
  }
}




