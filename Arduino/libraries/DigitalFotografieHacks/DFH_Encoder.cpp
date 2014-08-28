/* Digital Fotografie Hacks 
 * DFH_Encoder.cpp
 * Quadrature Encoder Bibliothek
 * v0.1, 2014-08-11, Peter Recktenwald  
 * Credits:
 * based on code from Peter Dannegger
 * http://www.mikrocontroller.net/articles/Drehgeber
 */
 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif 
#include "DFH_Encoder.h"

extern "C" { 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
}

#define PHASE_A  digitalRead(_encA)
#define PHASE_B  digitalRead(_encB)

static volatile int8_t encDelta;
static int8_t last;
static uint8_t _encA, _encB;
static int32_t enc;

/*-----( Interrupt Funktion )-----*/  
ISR( TIMER2_COMPA_vect )
{
  int8_t val, diff;

  val = 0;
  if( PHASE_A )
    val = 3;
  if( PHASE_B )
    val ^= 1;         // Wandle Gray in Binaer
  diff = last - val;  // Differenz letzter Wert - neuer Wert
  if( diff & 1 )
  {                   // bit 0 = Wert (1)
    last = val;       // speichere neuen Wert als naechsten letzten Wert
    encDelta += (diff & 2) - 1;  // bit 1 = Richtung (+/-)
  }
}

/*-----( Konstruktor )-----*/ 

DFH_Encoder::DFH_Encoder(uint8_t encA, uint8_t encB)
{
  int8_t val;

  _encA = encA; 
  _encB = encB;
  // Timer2 Interrupt erlauben
  cli();
  TIMSK2 |= (1<<OCIE2A);
  sei();
  // Encoder Pins als Eingaenge
  pinMode(_encA, INPUT);
  pinMode(_encB, INPUT);
  // PullUp Widerstaende einschalten
  digitalWrite(_encA, HIGH);    
  digitalWrite(_encB, HIGH);
  
  // initialisiere Encoder Anfangszustand  
  val=0;
  if (PHASE_A)
    val = 3;
  if (PHASE_B)
    val ^= 1;
  last = val;
  encDelta = 0;
  
  // Setze Encoder Zaehler zurueck 
  enc = 0;
}

/*-----( seset )-----*/ 

void DFH_Encoder::reset( void )  // Setze Encoder Zaehler zurueck
{
  enc = 0;
}

/*-----( read )-----*/ 

int32_t DFH_Encoder::read( void )  // Lese 1 Schritt Encoder
{
  cli();
  enc += encDelta;
  encDelta = 0;
  sei();
  return enc;                      
}

/*-----( read2 )-----*/ 

int32_t DFH_Encoder::read2( void ) // Lese 2 Schritt Encoder
{
  int8_t val;
 
  cli();
  val = encDelta;
  encDelta = val & 1;
  enc += (val >> 1);
  sei();
  return enc;
}
 
/*-----( read4 )-----*/ 
 
int32_t DFH_Encoder::read4( void ) // Lese 4 Schritt Encoder
{
  int8_t val;
 
  cli();
  val = encDelta;
  encDelta = val & 3;
  enc += (val >> 2);
  sei();
  return enc;
}

/*-----( write )-----*/ 

void DFH_Encoder::write( int32_t _val )  // Setze Encoder Zaehler auf bestimmten Wert
{
  enc = _val;
}

