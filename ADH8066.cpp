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
// Defines
//------------------------------------------------------------------------------


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
ADH8066::ADH8066(int rxPin, int txPin, int onKeyPin, long baud) {
    _rxPin = rxPin;
    _txPin = txPin;
    _onKeyPin = onKeyPin;
    _baud = baud;
    constructorHelper();
}

ADH8066::ADH8066(const SoftwareSerial &cellSerial) {
    _rxPin = -1;
    _txPin = -1;
    _cellSerial = cellSerial;
    _cellSerial.listen();
    constructorHelper();
}

// Constructor helper
void ADH8066::constructorHelper() {
    // _accessPoint = "wap.cingular";
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
    _txPin = srcObj._txPin;
    _rxPin = srcObj._rxPin;
    _onKeyPin = srcObj._onKeyPin;
    _baud = srcObj._baud;
    _cellSerial = srcObj._cellSerial;
    _buffer = srcObj._buffer;
    _accessPoint = srcObj._accessPoint;
    _year = srcObj._year;
    _month = srcObj._month;
    _day = srcObj._day;
    _hour = srcObj._hour;
    _min = srcObj._min;
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
    
    pinMode(_onKeyPin, OUTPUT);
    digitalWrite(_onKeyPin, HIGH);
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

String ADH8066::buffer() {
    return _buffer;
}


void ADH8066::clearBuffer () {
    _buffer = "";
}

bool ADH8066::overflow() {
    return _cellSerial.overflow();
}

void ADH8066::cellCmd(const String &str, bool endOfLine, bool hexFormat) {    
    for (int i = 0; i < str.length(); i++) {
        printChar(str[i], hexFormat);
        delay(_charDelay);
    }
    if (endOfLine) {
        if (hexFormat) {
            printChar('\0', hexFormat); delay(_charDelay);
            printChar('\r', hexFormat); delay(_charDelay);
            printChar('\0', hexFormat); delay(_charDelay);
            printChar('\n', hexFormat); delay(_charDelay);
        } else {
            printChar('\r', hexFormat); delay(_charDelay);
            printChar('\n', hexFormat); delay(_charDelay);
        }
    }
    delay(_cmdDelay);
}

bool ADH8066::isOn() {
    clearBuffer();
    cellCmd(String("at"));
    readCellStream(500);
    clearBuffer();
    cellCmd(String("at"));
    readCellStream(500);
    if (_buffer.indexOf(String("OK")) != -1) {
        return true;
    } else {
        return false;
    }
    clearBuffer();
}

void ADH8066::toggleOnOff() {
    clearBuffer();
    digitalWrite(_onKeyPin, LOW);
    delay(3000);
    digitalWrite(_onKeyPin, HIGH);
    delay(3000);
    clearBuffer();
}

void ADH8066::turnOn() {
    if (!isOn()) {
        toggleOnOff();
    }
}

void ADH8066::turnOff() {
    if (isOn()) {
        toggleOnOff();
    }
}

bool ADH8066::sim() {
    clearBuffer();
    cellCmd(String("at+cpin?"));
    readCellStream();
    
    if (_buffer.indexOf(String("+CPIN: READY")) == -1) {
        return false;
    } else {
        return true;
    }
}

int ADH8066::signalQuality() {
    clearBuffer();
    cellCmd(String("at+csq"));
    readCellStream();
    
    int index = _buffer.indexOf(String("+CSQ:"));
    String signalStr = _buffer.substring(index+6);
    signalStr = signalStr.substring(0, signalStr.indexOf(','));
    
    return stoi(signalStr);
}

bool ADH8066::check() {
    return (isOn() && sim() && signalQuality() >= 7);
}

String ADH8066::echoOn() {
    clearBuffer();
    cellCmd(String("ate1"));
    readCellStream(500);
    return _buffer;
}

String ADH8066::echoOff() {
    clearBuffer();
    cellCmd(String("ate0"));
    readCellStream(500);
    return _buffer;
}

bool ADH8066::sendText(const String &phoneNum, const String &msg) {
    clearBuffer();
    int subChar = 26;
    cellCmd(String("at+cmgf=1"));
    cellCmd(String("at+cmgs=\"") + phoneNum + String("\""));
    cellCmd(msg,false);
    cellCmd(String((char)subChar),false);
    readCellStream();
    
    if (_buffer.indexOf(String("OK")) != -1) {
        return true;
    } else {
        return false;
    }
}

String ADH8066::makeCall(const String &phoneNum) {
    clearBuffer();
    cellCmd(String("atd") + phoneNum);
    readCellStream();
    return _buffer;
}

String ADH8066::answer() {
    clearBuffer();
    cellCmd(String("ata"));
    readCellStream();
    return _buffer;
}

String ADH8066::hangUp() {
    clearBuffer();
    cellCmd(String("ath"));
    return _buffer;
}

String ADH8066::getWebsite(const String &url) {
    clearBuffer();
    
    cellCmd(String("at+aipdcont=\"") + _accessPoint + String("\""));
    cellCmd(String("at+aipa=1"));
    cellCmd(String("at+aipo=1,,\"") + url + String("\",80,0,,1"));
    cellCmd(String("at+aipw=1,\""),false);
    
    cellCmd(String("GET /~sehrlich/ HTTP/1.0"),true,true);
    cellCmd(String("Host: ") + url,true,true);
    cellCmd(String(""),true,true);
    
    cellCmd(String("\""),true);
    readCellStream();
    
    return _buffer;
}

bool ADH8066::updateTime() {
    clearBuffer();
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

String ADH8066::getTime() {
    String timeStr;
    timeStr += String(_year) + String('-') + String(_month) + String('-') + String(_day) + String(' ');
    timeStr += String(_hour) + String(':') + String(_min);
    return timeStr;
}


//------------------------------------------------------------------------------
// Protected Member Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Private Member Functions
//------------------------------------------------------------------------------
bool ADH8066::readCellStream(unsigned int readTime) {
    unsigned long time0 = millis();
    unsigned long time = time0;
    unsigned long maxTime = time + readTime;
    bool dataReceived = false;
    
    while ((time < maxTime && time >= time0)) {
        if (_cellSerial.available()) {
            while (_cellSerial.available()) {
                _buffer += (char)_cellSerial.read();
            }
            dataReceived = true;
            delay(10);
        }
        time = millis();
    }
    return dataReceived;
}

void ADH8066::printChar(char c, bool hexFormat) {
    if (hexFormat) {
        _cellSerial.print(c, HEX);      
    } else {
       _cellSerial.print(c);

    }
}

void ADH8066::parseTime() {
    int msgStartInd = _buffer.indexOf(String("+CCLK:"));
    
    _buffer = _buffer.substring(msgStartInd+8);
    Serial.println(_buffer);
    
    _year = stoi(_buffer.substring(0,2));
    _month = stoi(_buffer.substring(3,5));
    _day = stoi(_buffer.substring(6,8));
    _hour = stoi(_buffer.substring(9,11));
    _min = stoi(_buffer.substring(12,14));
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

