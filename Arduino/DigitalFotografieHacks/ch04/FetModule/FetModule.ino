/* Digital Fotografie Hacks
 * FETModule: 
 * FET Module Test
 * v0.1, 2014-08-24, Peter Recktenwald 
*/  
/*-----( Declare Constants )-----*/ 
#define valvePin    7

#define DROP_LENGTH  20 
#define DROP_DELAY  20  

void launchDrops()
{
  digitalWrite(valvePin, HIGH);
  delay(DROP_LENGTH);
  digitalWrite(valvePin, LOW);
  delay(DROP_DELAY);
  digitalWrite(valvePin, HIGH);
  delay(DROP_LENGTH);
  digitalWrite(valvePin, LOW);
  
} 

void setup() {
  pinMode(valvePin, OUTPUT);
  digitalWrite(valvePin, LOW); 
}

void loop() {
  launchDrops();
  delay5000():
}
