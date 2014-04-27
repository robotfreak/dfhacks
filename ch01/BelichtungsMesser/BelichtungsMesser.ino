/* Digital Fotografie Hacks
 * Belichtungsmesser: 
 * Prinzip eines Nachfuehr Belichtungsmessers
 * v0.1, 2014-01-31, Peter Recktenwald 
*/

/*-----[Konstanten Deklaration]--------*/
#define btnTimePin     11  /* Taster zur Zeiteinstellung */
#define btnAperturePin 10  /* Taster zur Blendeneinstellung */

#define ledGreenPin     7  /* gruene LED fuer Ueberbelichtung */
#define ledRedPin       6  /* rote LED fuer Unterbelichtung */

#define ldrPin         A0  /* analoger Eingang fuer LDR */
#define levelPin       A2  /* analoger Eingang fuer Poti */

enum eMode 
{
  MODE_TIME,    /* 0 Mode Zeitmessung */
  MODE_APERTURE /* 1 Mode Blendenmessung */
};

/*-----[Variablen Deklaration]--------*/
int LW, oldLW;     /* Lichtwert */
int exposure;      /* Belichtung */
int LWt, oldLWt;   /* Zeitleitwert */
int LWk, oldLWk;   /* Blendenleitwert */
int ldrVal;        /* LDR Wert */
int levelVal;      /* Potentiometer Wert */
eMode mode;        /* Mode Zeit- oder Blendenmessung */

/* Zeitleitwert in Belichtungszeit Wert Umrechnungs Tabelle */
char timeTbl[11][5] = { "1", "2", "4", "8", "15", "30", "60", "125", "250", "500", "1000" };  
/* Blendenleitwert in Blendenwert Umrechnungs Tabelle */
char apertureTbl[11][4] = { "1", "1,4", "2", "2,8", "4", "5,6", "8", "11", "16", "22", "32" };

void setup()
{
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  
  Serial.begin(57600);
  Serial.println("Belichtungsmesser v0.1");
  
  mode = MODE_TIME;
  LWt = 0;
  LWk = 0;
  oldLWt = -1;
  oldLWk = -1;
}


void loop()
{
  /*---- Eingabe ----*/
  ldrVal = analogRead(ldrPin);
  levelVal = analogRead(levelPin);
  if (digitalRead(btnTimePin) == LOW)
  {
    while (digitalRead(btnTimePin) == LOW);
    mode = MODE_TIME;  
  }
  else if (digitalRead(btnAperturePin) == LOW)
  {
    while (digitalRead(btnAperturePin) == LOW);
    mode = MODE_APERTURE;  
  }
  /*---- Verarbeitung ----*/
  LW = map(ldrVal, 0, 1023, 0, 20);
  switch (mode)
  {
    case MODE_TIME: 
      LWt = map(levelVal, 0, 1023, 0, 10);
    break;
    case MODE_APERTURE: 
      LWk = map(levelVal, 0, 1023, 0, 10);
    break;
    default:
    break;    
  }
  exposure = LW - (LWt + LWk);
  
  /*---- Ausgabe ----*/
  if (oldLWt != LWt || oldLWk != LWk || LW != oldLW)
  {
    Serial.print("Lichtwert: ");
    Serial.print(LW, DEC);
    Serial.print(" ");
    oldLW = LW;
    Serial.print("\tZeit: 1/");
    Serial.print(timeTbl[LWt]);
    Serial.print(" s");
    oldLWt = LWt;
    oldLWt = LWt;
    Serial.print("\tBlende: f/");
    Serial.print(apertureTbl[LWk]);
    Serial.print(" ");
    oldLWk = LWk;
    if (exposure < 0)         /* Unterbelichtung */
    {
      Serial.println("unterbelichtet!");  
      digitalWrite(ledRedPin, HIGH);
      digitalWrite(ledGreenPin, LOW);
    }  
    else if (exposure > 0)    /* Ueberbelichtung */
    {
      Serial.println("ueberbelichtet!");  
      digitalWrite(ledRedPin, LOW);
      digitalWrite(ledGreenPin, HIGH);
    }  
    else                       /* korrekte Belichtung */
    {
      Serial.println("korrekt belichtet!");  
      digitalWrite(ledRedPin, HIGH);
      digitalWrite(ledGreenPin, HIGH);
    }  
  }
  delay(100);
}
  
