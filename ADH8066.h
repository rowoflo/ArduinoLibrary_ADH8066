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
    ADH8066(int rxPin = 2, int txPin = 3);
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
    bool overflow();
    void check();
    void turnOn();
    void turnOff();
    void sendText(const String &phoneNum, const String &msg);
    void makeCall(const String &phoneNum);
    void hangUp();
    void getWebsite(const String &url);
    void getTime();
    
    //--------------------------------------------------------------------------
    // Public Member Variables
    //--------------------------------------------------------------------------
    
protected:
    //--------------------------------------------------------------------------
    // Protected Member Functions
    //--------------------------------------------------------------------------
    
    
    //--------------------------------------------------------------------------
    // Protected Member Variables
    //--------------------------------------------------------------------------
    
    
    
private:
    //--------------------------------------------------------------------------
    // Constants, Enums, and Types
    //--------------------------------------------------------------------------
    
    
    //--------------------------------------------------------------------------
    // Lifecycle
    //--------------------------------------------------------------------------
    void copyHelper(const ADH8066& srcObj);
    
    //--------------------------------------------------------------------------
    // Private Member Functions
    //--------------------------------------------------------------------------
    void cellCmd(const String &str, bool endOfLine = true, bool hexFormat = false);
    void printChar(char c, bool hexFormat = false);
    
    //--------------------------------------------------------------------------
    // Private Member Variables
    //--------------------------------------------------------------------------
    bool _echo; // Cell commands will be echoed to serial line
    int _rxPin; // Receive pin
    int _txPin; // Transmit pin
    
    
    static const int _baud = 9600; // BAUD rate
    static const int _cmdDelay = 500; // Delay between each command
    
    SoftwareSerial _cellSerial; // Softserial object for GPS
    String _buffer; // Receive data buffer
    
    static const String _accessPoint = "wap.cingular";
};

//------------------------------------------------------------------------------
// Postfix Increment Operators
//------------------------------------------------------------------------------


#endif
