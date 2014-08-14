/*
 * DFH_Encoder.h - Quadrature Encoder Bibliothek
 *
 * Credits:
 * based on code from Peter Dannegger
 * http://www.mikrocontroller.net/articles/Drehgeber
 */


#ifndef __DFH_ENCODER_H__
#define __DFH_ENCODER_H__

extern "C" { 
#include <avr/io.h>
#include <inttypes.h>
}

class DFH_Encoder {

public:  

  DFH_Encoder(uint8_t encA, uint8_t encB);   //constructor

  void reset( void );

  int32_t read( void );
  int32_t read2( void );
  int32_t read4( void );

  void write( int32_t _val);
};

#endif /* __DFH_ENCODER_H__ */


