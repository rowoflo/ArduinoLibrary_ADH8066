/*
 -------------------------------------------------------------------------------
 ADH8066.h
 ADH8066 Project
 
 CLASS NAME:
    ADH8066
 
 DESCRIPTION:
    description...
 
 FILES:
    ADH8066.h
    ADH8066.cpp

 DEPENDENCIES:
    
 
 CONSTRUCTORS:
    ADH8066();    
 
 PROPERTIES:
    prop1 - description... .

    prop2 - description... .
 
 METHODS:
    type method1(type arg1);
        Description... .
            arg1 - description... .
 
 NOTES:
 
 
 EXAMPLES:
    Example 1: description
    ----------------------------------------------------------------------------
    code...
    ----------------------------------------------------------------------------
 
 
 VERSIONS:
    1.0 - 1/20/13 - Rowland O'Flaherty (rowlandoflaherty.com)
                    Daniel Pickem (danielpickem.com)
 
 -------------------------------------------------------------------------------
 */

#ifndef _ADH8066_h_
#define _ADH8066_h_

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Includes and Namespaces
//------------------------------------------------------------------------------
#include <Arduino.h>
#include <SoftwareSerial.h>


class ADH8066 {
    //--------------------------------------------------------------------------
    // Nested Classes
    //--------------------------------------------------------------------------
    
    
    //--------------------------------------------------------------------------
    // Friends
    //--------------------------------------------------------------------------
    
    
    //--------------------------------------------------------------------------
    // Friend Overloaded Operators
    //--------------------------------------------------------------------------
    
    
public:
    //--------------------------------------------------------------------------
    // Constants, Enums, and Types
    //--------------------------------------------------------------------------
	
    
    //--------------------------------------------------------------------------
    // Lifecycle
    //--------------------------------------------------------------------------
    // Constructors
    ADH8066(int rxPin = 2, int txPin = 3, int onKeyPin = 4, long baud = 9600); // Arduino Rx (Cell Tx), Arduino Tx (Cell Rx)
    ADH8066(const SoftwareSerial &cellSerial);
    
    // Destructor
    virtual ~ADH8066();
    
    // Copy constructor
    ADH8066(const ADH8066& srcObj);

    
    //--------------------------------------------------------------------------
    // Overloaded Operators
    //--------------------------------------------------------------------------
    // Assignment operator
    const ADH8066& operator=(const ADH8066& rhsObj);
    
    
    //--------------------------------------------------------------------------
    // Public Member Functions
    //--------------------------------------------------------------------------
    void initialize();
    bool isListening();
    void listen();
    int available();
    String buffer();
    void clearBuffer();
    bool overflow();
    void cellCmd(const String &str, bool endOfLine = true, bool hexFormat = false);
    bool isOn();
    void toggleOnOff();
    void turnOn();
    void turnOff();
    bool sim();
    int signalQuality();
    bool check();
    String echoOn();
    String echoOff();
    bool sendText(const String &phoneNum, const String &msg);
    String makeCall(const String &phoneNum);
    String answer();
    String hangUp();
    String getWebsite(const String &url);
    bool updateTime();
    
    String getTime();
    
    // Getters
    int year() { return _year;}
    int month() { return _month;}
    int day() { return _day;}
    int hour() { return _hour;}
    int minute() { return _min;}
    
    //--------------------------------------------------------------------------
    // Public Member Variables
    //--------------------------------------------------------------------------
    
private:
    //--------------------------------------------------------------------------
    // Constants, Enums, and Types
    //--------------------------------------------------------------------------
    
    
    //--------------------------------------------------------------------------
    // Lifecycle
    //--------------------------------------------------------------------------
    void constructorHelper();
    void copyHelper(const ADH8066& srcObj);
    
    //--------------------------------------------------------------------------
    // Private Member Functions
    //--------------------------------------------------------------------------
    bool readCellStream(unsigned int readTime = 2000);
    void printChar(char c, bool hexFormat = false);
    void parseTime();
    
    //--------------------------------------------------------------------------
    // Private Member Variables
    //--------------------------------------------------------------------------
    bool _on;      // True if cell is on
    int _rxPin;    // Receive pin
    int _txPin;    // Transmit pin
    int _onKeyPin; // On key pin
    long _baud;    // BAUD rate
    
    static const int _charDelay = 10; // Delay between each character
    static const int _cmdDelay = 500; // Delay between each command
    
    SoftwareSerial _cellSerial; // Softserial object for cell
    String _buffer; // Receive data buffer
    
    // Cell Data
    String _accessPoint;
    
    int _year;
    int _month;
    int _day;
    int _hour;
    int _min;
};

//------------------------------------------------------------------------------
// Other Functions
//------------------------------------------------------------------------------
int stoi(const String &str);
float stof(const String &str);
int hexToInt(char hexChar);
char hexToChar(char hexCharHigh, char hexCharLow);

#endif
