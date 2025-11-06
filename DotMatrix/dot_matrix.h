#ifndef _DOT_MATRIX_H_
#define _DOT_MATRIX_H_

#include <Arduino.h>
#include <SPI.h>

#define MAX_DEVICES     4       //maximum modules you will cascade

class DotMatrix {
    public:
        DotMatrix ( uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices = MAX_DEVICES);
        void begin ( );
        void clear ( );
        void setIntensity ( uint8_t intensity );          //0-15
        void scrollText ( const char* text, uint16_t delayMs = 80 );
        void displayStatic ( const char* text );          //centered static text

    private:
        uint8_t _dataPin, _clkPin, _csPin;
        uint8_t _numDevices;
        byte    _buffer[8][MAX_DEVICES * 8];

        //MAX7219 registers
        static const uint8_t REG_NOOP        = 0x00;
        static const uint8_t REG_DIGIT0      = 0x01;
        static const uint8_t REG_DECODEMODE  = 0x09;
        static const uint8_t REG_INTENSITY   = 0x0A;
        static const uint8_t REG_SCANLIMIT   = 0x0B;
        static const uint8_t REG_SHUTDOWN    = 0x0C;
        static const uint8_t REG_DISPLAYTEST = 0x0F;

        //5x7 font – LSB = top row of the character
        static const byte _font[][5];

        void sendCommand ( uint8_t reg, uint8_t value );
        void initMax7219 ( );
        void clearBuffer ( );
        void shiftRight ( );
        void drawColumnAtLeft ( byte colBits );
        void updateDisplay ( );
        int  getFontIndex ( char c );
};

#endif  //_DOT_MATRIX_H_