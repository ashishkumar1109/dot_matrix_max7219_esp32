#include "dot_matrix.h"

//DATA, CLK, CS, Module
DotMatrix matrix ( 23, 18, 5, 4 );

void setup ( ) {
    matrix.begin ( );
    matrix.setIntensity ( 8 );
}

void loop ( ) {
    matrix.scrollText ( "HELLO #ESP32     ", 70 );
}