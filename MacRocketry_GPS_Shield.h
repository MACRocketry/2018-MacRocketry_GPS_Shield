#ifndef MacRocketry_GPS_Shield_h //include guard
#define MacRocketry_GPS_Shield_h

#include <Arduino.h>        //Arduino Library
#include <SoftwareSerial.h> //SoftwareSerial library

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

class MacRocketry_GPS_Shield
{
public:
  String data;     //NMEA string
  float utc, altitude; //only need time and altitude
  int fix;             //check for fix data (0 - invalid, 1 - GPS, 2 - DGPS)

  MacRocketry_GPS_Shield(void); //default constructor
  void sendCommand(String cmd); //send PMTK command
  bool readData(void);
  void displayData(void);

private:
  SoftwareSerial serial; //serial to GPS
  String serialStr;      //needed for bufferSerial()
  char serialChar;       //needed for bufferSerial()
  int indexPre, indexEnd; //needed for readData()

  void init(void);  //init all variables to null
  void start(void); //set up GPS
  bool bufferSerial(void);
};

#endif