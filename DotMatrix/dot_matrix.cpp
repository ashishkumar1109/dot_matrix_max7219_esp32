#include "dot_matrix.h"
#include <string.h>

/* -------------------------------------------------------------
   5x7 font – one column per byte, LSB = top row
   ------------------------------------------------------------- */
const byte DotMatrix::_font[][5] = {
    { 0x00,0x00,0x00,0x00,0x00 },   // space
    { 0x00,0x00,0x5F,0x00,0x00 },   // !
    { 0x00,0x07,0x00,0x07,0x00 },   // "
    { 0x14,0x7F,0x14,0x7F,0x14 },   // #
    { 0x24,0x2A,0x7F,0x2A,0x12 },   // $
    { 0x23,0x13,0x08,0x64,0x62 },   // %
    { 0x36,0x49,0x55,0x22,0x50 },   // &
    { 0x00,0x05,0x03,0x00,0x00 },   // '
    { 0x00,0x1C,0x22,0x41,0x00 },   // (
    { 0x00,0x41,0x22,0x1C,0x00 },   // )
    { 0x14,0x08,0x3E,0x08,0x14 },   // *
    { 0x08,0x08,0x3E,0x08,0x08 },   // +
    { 0x00,0x50,0x30,0x00,0x00 },   // ,
    { 0x08,0x08,0x08,0x08,0x08 },   // -
    { 0x00,0x60,0x60,0x00,0x00 },   // .
    { 0x20,0x10,0x08,0x04,0x02 },   // /
    { 0x3E,0x51,0x49,0x45,0x3E },   // 0
    { 0x00,0x42,0x7F,0x40,0x00 },   // 1
    { 0x42,0x61,0x51,0x49,0x46 },   // 2
    { 0x21,0x41,0x45,0x4B,0x31 },   // 3
    { 0x18,0x14,0x12,0x7F,0x10 },   // 4
    { 0x27,0x45,0x45,0x45,0x39 },   // 5
    { 0x3C,0x4A,0x49,0x49,0x30 },   // 6
    { 0x01,0x71,0x09,0x05,0x03 },   // 7
    { 0x36,0x49,0x49,0x49,0x36 },   // 8
    { 0x06,0x49,0x49,0x29,0x1E },   // 9
    { 0x00,0x36,0x36,0x00,0x00 },   // :
    { 0x00,0x56,0x36,0x00,0x00 },   // ;
    { 0x08,0x14,0x22,0x41,0x00 },   // <
    { 0x14,0x14,0x14,0x14,0x14 },   // =
    { 0x00,0x41,0x22,0x14,0x08 },   // >
    { 0x02,0x01,0x51,0x09,0x06 },   // ?
    { 0x32,0x49,0x79,0x41,0x3E },   // @
    { 0x7E,0x11,0x11,0x11,0x7E },   // A
    { 0x7F,0x49,0x49,0x49,0x36 },   // B
    { 0x3E,0x41,0x41,0x41,0x22 },   // C
    { 0x7F,0x41,0x41,0x22,0x1C },   // D
    { 0x7F,0x49,0x49,0x49,0x41 },   // E
    { 0x7F,0x09,0x09,0x09,0x01 },   // F
    { 0x3E,0x41,0x49,0x49,0x7A },   // G
    { 0x7F,0x08,0x08,0x08,0x7F },   // H
    { 0x00,0x41,0x7F,0x41,0x00 },   // I
    { 0x20,0x40,0x41,0x3F,0x01 },   // J
    { 0x7F,0x08,0x14,0x22,0x41 },   // K
    { 0x7F,0x40,0x40,0x40,0x40 },   // L
    { 0x7F,0x02,0x0C,0x02,0x7F },   // M
    { 0x7F,0x04,0x08,0x10,0x7F },   // N
    { 0x3E,0x41,0x41,0x41,0x3E },   // O
    { 0x7F,0x09,0x09,0x09,0x06 },   // P
    { 0x3E,0x41,0x51,0x21,0x5E },   // Q
    { 0x7F,0x09,0x19,0x29,0x46 },   // R
    { 0x26,0x49,0x49,0x49,0x32 },   // S
    { 0x01,0x01,0x7F,0x01,0x01 },   // T
    { 0x3F,0x40,0x40,0x40,0x3F },   // U
    { 0x1F,0x20,0x40,0x20,0x1F },   // V
    { 0x7F,0x20,0x18,0x20,0x7F },   // W
    { 0x63,0x14,0x08,0x14,0x63 },   // X
    { 0x03,0x04,0x78,0x04,0x03 },   // Y
    { 0x61,0x51,0x49,0x45,0x43 }    // Z
};

DotMatrix::DotMatrix ( uint8_t dataPin, uint8_t clkPin,  uint8_t csPin, uint8_t numDevices ) {
    _dataPin    = dataPin;
    _clkPin     = clkPin;
    _csPin      = csPin;
    _numDevices = std::min ( numDevices, ( uint8_t ) MAX_DEVICES );
}

void DotMatrix::begin ( ) {
    SPI.begin ( _clkPin, -1, _dataPin, _csPin );
    pinMode ( _csPin, OUTPUT );
    digitalWrite ( _csPin, HIGH );
    initMax7219 ( );
    clear ( );
}

void DotMatrix::sendCommand ( uint8_t reg, uint8_t value ) {
    digitalWrite ( _csPin, LOW );
    for ( int i = 0; i < _numDevices; ++i ) {
        SPI.transfer ( reg );
        SPI.transfer ( value );
    }
    digitalWrite ( _csPin, HIGH );
}

void DotMatrix::initMax7219 ( ) {
    sendCommand ( REG_SCANLIMIT, 0x07 );
    sendCommand ( REG_DECODEMODE, 0x00 );
    sendCommand ( REG_DISPLAYTEST, 0x00 );
    sendCommand ( REG_INTENSITY, 0x03 );
    sendCommand ( REG_SHUTDOWN, 0x01 );
    for ( int i = 1; i <= 8; ++i ) {
        sendCommand ( i, 0x00 );
    }
}

void DotMatrix::clearBuffer ( ) {
    memset ( _buffer, 0, sizeof ( _buffer ) );
}

void DotMatrix::clear ( ) {
    clearBuffer ( );
    updateDisplay ( );
}

void DotMatrix::setIntensity ( uint8_t intensity ) {
    if ( intensity > 15 ) {
        intensity = 15;
    } else {
        //DO NOTHING
    }
    sendCommand ( REG_INTENSITY, intensity );
}

void DotMatrix::shiftRight ( ) {
    for ( int r = 0; r < 8; ++r ) {
        for ( int c = _numDevices * 8 - 1; c > 0; --c ) {
            _buffer[r][c] = _buffer[r][c - 1];
        }
        _buffer[r][0] = 0;
    }
}

void DotMatrix::drawColumnAtLeft ( byte colBits ) {
    for ( int r = 0; r < 8; ++r ) {
        _buffer[7 - r][0] = ( colBits >> r ) & 1;
    }
}

void DotMatrix::updateDisplay ( ) {
    for ( int row = 0; row < 8; ++row ) {
        digitalWrite ( _csPin, LOW );
        for ( int dev = 0; dev < _numDevices; ++dev ) {
            byte colData = 0;
            int baseCol = dev * 8;
            for ( int bit = 0; bit < 8; ++bit ) {
                if ( _buffer[row][baseCol + ( 7 - bit )] ) {
                    colData |= ( 1 << bit );
                } else {
                    //DO NOTHING
                }
            }
            SPI.transfer ( row + 1 );
            SPI.transfer ( colData );
        }
        digitalWrite ( _csPin, HIGH );
    }
}

int DotMatrix::getFontIndex ( char c ) {
    if ( c >= ' ' && c <= 'Z' ) {
        return c - ' ';
    } else {
        //DO NOTHING
    }
    return 0;   //space
}

void DotMatrix::scrollText ( const char* text, uint16_t delayMs ) {
    static int charIndex = 0;
    static int colInChar = 0;
    static unsigned long lastShift = 0;
    static const char* currentText = text;

    if ( millis ( ) - lastShift < delayMs ) {
        return;
    } else {
        //DO NOTHING
    }
    lastShift = millis ( );

    int idx = getFontIndex ( currentText[charIndex] );
    byte col = ( colInChar < 5 ) ? _font[idx][colInChar] : 0x00;

    drawColumnAtLeft ( col );
    shiftRight ( );
    updateDisplay ( );

    ++colInChar;
    if ( colInChar > 5 ) {
        colInChar = 0;
        ++charIndex;
        if ( currentText[charIndex] == '\0' ) {
            charIndex = 0;
        } else {
            //DO NOTHING
        }
    } else {
        //DO NOTHING
    }
}

void DotMatrix::displayStatic ( const char* text ) {
    clearBuffer ( );

    size_t len = strlen ( text );
    int totalCols = _numDevices * 8;
    int textWidth = static_cast<int> ( len * 6 );
    int startCol = ( textWidth < totalCols ) ? ( totalCols - textWidth ) / 2 : 0;

    for ( size_t i = 0; i < len; ++i ) {
        int idx = getFontIndex ( text[i] );
        int colOffset = startCol + static_cast<int> ( i * 6 );
        if ( colOffset >= totalCols ) {
            break;
        } else {
            //DO NOTHING
        }

        for ( int c = 0; c < 5; ++c ) {
            int targetCol = colOffset + c;
            if ( targetCol >= totalCols ) {
                break;
            } else {
                //DO NOTHING
            }
            byte colBits = _font[idx][c];
            for ( int r = 0; r < 8; ++r ) {
                if ( colBits & ( 1 << r ) ) {
                    _buffer[7 - r][targetCol] = 1;
                } else {
                    //DO NOTHING
                }
            }
        }
    }
    updateDisplay ( );
}
