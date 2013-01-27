/*
 -------------------------------------------------------------------------------
 ADH8066.cpp
 ADH8066 Project
 
 Initially created by Rowland O'Flaherty and Daniel Pickem 
 (rowlandoflaherty.com and danielpickem.com) on 1/20/13.
 
 Version 1.0
 -------------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
// Includes and Namespaces
//------------------------------------------------------------------------------
#include "ADH8066.h"

//------------------------------------------------------------------------------
// Friends
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Friend Overloaded Operators
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Lifecycle
//------------------------------------------------------------------------------
// Constructors
ADH8066::ADH8066(int rxPin, int txPin) {
    _rxPin = rxPin;
    _txPin = txPin;
}

ADH8066::ADH8066(const SoftwareSerial &cellSerial) {
    _rxPin = -1;
    _txPin = -1;
    _cellSerial = cellSerial;
    _cellSerial.listen();
}

// Constructor helper
void ADH8066::constructorHelper() {
    _echo = true;
    _accessPoint = "wap.cingular";
}

// Destructor
ADH8066::~ADH8066() {
    
}

// Copy constructor
ADH8066::ADH8066(const ADH8066& srcObj) {
    copyHelper(srcObj);
}

// Copy helper function
void ADH8066::copyHelper(const ADH8066& srcObj) {
    _echo = srcObj._echo;
    _txPin = srcObj._txPin;
    _rxPin = srcObj._rxPin;
    _cellSerial = srcObj._cellSerial;
    _buffer = srcObj._buffer;
    _accessPoint = srcObj._accessPoint;
    _year = srcObj._year;
    _month = srcObj._month;
    _day = srcObj._day;
    _hour = srcObj._hour;
    _min = srcObj._min;
    _sec = srcObj._sec;
}

//------------------------------------------------------------------------------
// Overloaded Operators
//------------------------------------------------------------------------------

// Assignment operator
const ADH8066& ADH8066::operator=(const ADH8066& rhsObj) {
	// Self-assignment check
    if (this == &rhsObj) {
        return (*this);
    }
    
    // Free old memory
    
    // Copy new memory
    copyHelper(rhsObj);
    
    return (*this);
}

//------------------------------------------------------------------------------
// Public Member Functions
//------------------------------------------------------------------------------
void ADH8066::initialize() {
    _cellSerial = SoftwareSerial(_rxPin, _txPin);
    _cellSerial.begin(_baud);
    _cellSerial.listen();
}

bool ADH8066::isListening() {
    return _cellSerial.isListening();
}

void ADH8066::listen() {
    _cellSerial.listen();
}

int ADH8066::available() {
    return _cellSerial.available();
}

bool ADH8066::overflow() {
    return _cellSerial.overflow();
}

void ADH8066::check() {
    cellCmd(String("at"));
    cellCmd(String("at+cpin?"));
    cellCmd(String("at+csq"));
}

void ADH8066::turnOn() {
    
}

void ADH8066::turnOff() {
    
}

void ADH8066::sendText(const String &phoneNum, const String &msg) {
    int subChar = 26;
    cellCmd(String("at+cmgf=1"));
    cellCmd(String("at+cmgs=\"") + phoneNum + String("\""));
    cellCmd(msg,false);
    cellCmd(String((char)subChar),false);
}

void ADH8066::makeCall(const String &phoneNum) {
    cellCmd(String("atd") + phoneNum);
}

void ADH8066::hangUp() {
    
}

void ADH8066::getWebsite(const String &url) {
    cellCmd(String("at+aipdcont=\"") + _accessPoint + String("\""));
    cellCmd(String("at+aipa=1"));
    cellCmd(String("at+aipo=1,,\"") + url + String("\",80,0,,1"));
    cellCmd(String("at+aipw=1,\""),false);
    
    cellCmd(String("GET /~sehrlich/ HTTP/1.0"),true,true);
    cellCmd(String("Host: ") + url,true,true);
    cellCmd(String(""),true,true);
    
    cellCmd(String("\""),true);
}

bool ADH8066::getTime() {
    cellCmd(String("at+cclk?"));
    
    bool received;
    if (readCellStream()) {
        parseTime();
        received = true;
    } else {
        received = false;
    }
    return received;
}


//------------------------------------------------------------------------------
// Protected Member Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Private Member Functions
//------------------------------------------------------------------------------
bool ADH8066::readCellStream() {
    if (!_cellSerial.available()) {
        return false;
    }
	while (_cellSerial.available()) {
        _buffer += (char)_cellSerial.read();
    }
    return true;
}

void ADH8066::cellCmd(const String &str, bool endOfLine, bool hexFormat) {
    for (int i = 0; i < str.length(); i++) {
        printChar(str[i], hexFormat);
    }
    if (endOfLine) {
        if (hexFormat) {
            printChar('\0', hexFormat); printChar('\r', hexFormat); printChar('\0', hexFormat); printChar('\n', hexFormat);
        } else {
            printChar('\r', hexFormat); printChar('\n', hexFormat);
        }
        if (_echo) {
            Serial.println();
        }
    }  
    delay(_cmdDelay);
}

void ADH8066::printChar(char c, bool hexFormat) {
    if (hexFormat) {
        _cellSerial.print(c, HEX);
        if (_echo)
            Serial.print(c, HEX);
    } else {
       _cellSerial.print(c);
        if (_echo) {
            if (c == '\0') {
                Serial.print("\\0");
            } else if (c == '\r') {
                Serial.print("\\r");
            } else if (c == '\n') {
                Serial.print("\\n");
            } else if (c == 26) {
                Serial.println("\\u");
            } else {
                Serial.print(c);
            }
        }
    }
}

void ADH8066::parseTime() {
    _year = stoi(_buffer.substring(0,2));
    _month = stoi(_buffer.substring(3,5));
    _day = stoi(_buffer.substring(6,8));
    _hour = stoi(_buffer.substring(9,11));
    _min = stoi(_buffer.substring(12,14));
    _sec = stoi(_buffer.substring(15,16));
}


//------------------------------------------------------------------------------
// Other Functions
//------------------------------------------------------------------------------
int stoi(const String &str) {
    char charBuffer[str.length()+1];
    str.toCharArray(charBuffer, str.length()+1);
    return (atoi(charBuffer));
}

float stof(const String &str) {
    char charBuffer[str.length()+1];
    str.toCharArray(charBuffer, str.length()+1);
    return (atof(charBuffer));
}

int hexToInt(char hexChar) {
    switch (hexChar) {
        case '0':
            return 0;
            break;
            
        case '1':
            return 1;
            break;
            
        case '2':
            return 2;
            break;
            
        case '3':
            return 3;
            break;
            
        case '4':
            return 4;
            break;
            
        case '5':
            return 5;
            break;
            
        case '6':
            return 6;
            break;
            
        case '7':
            return 7;
            break;
            
        case '8':
            return 8;
            break;
            
        case '9':
            return 9;
            break;
            
        case 'A':
        case 'a':
            return 10;
            break;
            
        case 'B':
        case 'b':
            return 11;
            break;
            
        case 'C':
        case 'c':
            return 12;
            break;
            
        case 'D':
        case 'd':
            return 13;
            break;
            
        case 'E':
        case 'e':
            return 14;
            break;
            
        case 'F':
        case 'f':
            return 15;
            break;

        default:
            return -1;
            break;
    }
}

char hexToChar(char hexCharHigh, char hexCharLow) {
    int hexInt = hexToInt(hexCharHigh)*16 + hexToInt(hexCharLow);
    return (char)hexInt;
}

