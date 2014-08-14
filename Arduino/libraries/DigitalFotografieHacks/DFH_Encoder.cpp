/*
 * DFH_Encoder.cpp - Library for quadrature encoders
 *
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

#define PHASE_A		digitalRead(_encA)
#define PHASE_B		digitalRead(_encB)

static volatile int8_t encDelta;
static int8_t last;
static uint8_t _encA, _encB;
static int32_t enc;


ISR( TIMER2_COMPA_vect )
{
  int8_t val, diff;

//  digitalWrite(ledPin, HIGH);
  val = 0;
  if( PHASE_A )
    val = 3;
  if( PHASE_B )
    val ^= 1;					// convert gray to binary
  diff = last - val;				// difference last - new
  if( diff & 1 )
  {				// bit 0 = value (1)
    last = val;				// store new as next last
    encDelta += (diff & 2) - 1;		// bit 1 = direction (+/-)
  }

//  digitalWrite(ledPin, LOW);
}

DFH_Encoder::DFH_Encoder(uint8_t encA, uint8_t encB)
{
  int8_t val;

  _encA = encA; 
  _encB = encB;

  cli();
  TIMSK2 |= (1<<OCIE2A);
  sei();
  pinMode(_encA, INPUT);
  pinMode(_encB, INPUT);

  val=0;
  if (PHASE_A)
    val = 3;
  if (PHASE_B)
    val ^= 1;
  last = val;
  encDelta = 0;

  enc = 0;
}

void DFH_Encoder::reset( void )
{
  enc = 0;
}

int32_t DFH_Encoder::read( void )			// read single step encoders
{
  cli();
  enc += encDelta;
  encDelta = 0;
  sei();
  return enc;					// counts since last call
}

int32_t DFH_Encoder::read2( void )         // read two step encoders
{
  int8_t val;
 
  cli();
  val = encDelta;
  encDelta = val & 1;
  enc += (val >> 1);
  sei();
  return enc;
}
 
 
int32_t DFH_Encoder::read4( void )         // read four step encoders
{
  int8_t val;
 
  cli();
  val = encDelta;
  encDelta = val & 3;
  enc += (val >> 2);
  sei();
  return enc;
}

void DFH_Encoder::write( int32_t _val )
{
  enc = _val;
}

