#ifndef MacRocketry_GPS_Shield_h //include guard
#define MacRocketry_GPS_Shield_h

#include <Arduino.h>                //include Arduino library
#include <SoftwareSerial.h>         //SoftwareSerial library

//need to decide whether GGA is good enough, or want RMC as well
#define PMTK_SET_NMEA_OUTPUT_GGA "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
//note: RMCGGA does not yet work on UNO due to UNO being a potato chip

//PMTK command for rate
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"      //fix rate 10Hz
#define PMTK_SET_NMEA_BAUDRATE_38400 "$PMTK251,38400*27" //baud rate 38400
//#define PMTK_SET_NMEA_BAUDRATE_57600 "$PMTK251,57600*2C" //does not yet work

#define GPS_RX_PIN 8
#define GPS_TX_PIN 7

class MacRocketry_GPS_Shield {
  public:
    MacRocketry_GPS_Shield(void);   //default constructor
    void sendCommand(String cmd);   //send PMTK command
    
    bool readSerialBuffer(void);
    bool clearSerialBuffer(void);
    bool parseData(void);
    void displayData(void);

    //getters --------------------
    float getUTC(void);
    float getAltitude(void);
    uint8_t getFix(void);
    String getData();
    
  private:
    void init(void);    //init all variables to null
    void start(void);   //set up GPS

    String data;          //NMEA string
    float utc, alt;       //only need time and altitude
    uint8_t fix;          //check for fix data (0 - invalid, 1 - GPS, 2 - DGPS)
    
    SoftwareSerial serial;      //serial to GPS
    String serialStr;           //needed for readSerialBuffer()
};

#endif