/* Digital Fotografie Hacks 
 * DFH_Encoder.h
 * Quadrature Encoder Bibliothek
 * v0.1, 2014-08-11, Peter Recktenwald  
 * Credits:
 * based on code from Peter Dannegger
 * http://www.mikrocontroller.net/articles/Drehgeber
 */


#ifndef __DFH_ENCODER_H__
#define __DFH_ENCODER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern "C" { 
#include <avr/io.h>
#include <inttypes.h>
}

/*-----( Class Definition )-----*/
class DFH_Encoder {

public:  

  DFH_Encoder(uint8_t encA, uint8_t encB);   // Konstruktor
  void reset( void );          // Setze Encoder Zaehler zurück        
  int32_t read( void );        // Lese 1 Schritt Encoder
  int32_t read2( void );       // Lese 2 Schritt Encoder
  int32_t read4( void );       // Lese 4 Schritt Encoder    
  void write( int32_t _val);   // Setze Encoder Zaehler auf bestimmten Wert        
};

#endif /* __DFH_ENCODER_H__ */

