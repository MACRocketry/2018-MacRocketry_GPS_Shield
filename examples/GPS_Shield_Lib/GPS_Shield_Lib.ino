//Start of Header ====================================================================================================
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
    bool readData(void);
    void displayData(void);

    //getters --------------------
    float getUTC(void);
    float getAltitude(void);
    uint8_t getFix(void);
    String getData();
    
  private:
    void init(void);    //init all variables to null
    void start(void);   //set up GPS
    bool readSerialBuffer(void);

    String data;          //NMEA string
    float utc, alt;       //only need time and altitude
    uint8_t fix;          //check for fix data (0 - invalid, 1 - GPS, 2 - DGPS)
    
    SoftwareSerial serial;      //serial to GPS
    String serialStr;           //needed for readSerialBuffer()
};

#endif
//End of Header ======================================================================================================
//Start of Source ====================================================================================================
#include <Arduino.h>                //include Arduino library
//#include <MacRocketry_GPS_Shield.h> //header file
#include <SoftwareSerial.h>         //SoftwareSerial library

MacRocketry_GPS_Shield::MacRocketry_GPS_Shield() : serial(GPS_RX_PIN, GPS_TX_PIN) //constructor
{
    start();
    init();
}

void MacRocketry_GPS_Shield::init(){
  //initialize variables to null
  data = "";
  serialStr = "";
  
  //does not have to initialize, but for consistency sake
  utc = 0.0;
  alt = 0.0;
  fix = 0;
}

void MacRocketry_GPS_Shield::start(){         //set up GPS
  serial.begin(9600);                         //default baud rate for GPS
  sendCommand(PMTK_SET_NMEA_BAUDRATE_38400);  //set GPS baud rate
  serial.begin(38400);                        //restart with faster baud rate
  sendCommand(PMTK_SET_NMEA_OUTPUT_GGA);
  sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
}

//getters and setters --------------------------------------------------
float MacRocketry_GPS_Shield::getUTC(void){ return utc; }
float MacRocketry_GPS_Shield::getAltitude(void){ return alt; }
uint8_t MacRocketry_GPS_Shield::getFix(void){ return fix; }
String MacRocketry_GPS_Shield::getData(void){ return data; }

//serial commands --------------------------------------------------
void MacRocketry_GPS_Shield::sendCommand(String cmd){
  //send PMTK command to GPS
  serial.flush();       //flush anything in buffer
  serial.println(cmd);  //send command
}

bool MacRocketry_GPS_Shield::readSerialBuffer(void){
  while (serial.available()){           //if data available
    serialStr += serial.read();					//append to string
    if (serialStr.endsWith("\r\n")){    //if end of sentence
      data = serialStr;                 //record data
      serialStr = "";                   //reset string buffer
      return true;
    }
  }
  return false;
}

bool MacRocketry_GPS_Shield::readData(void){
  if (false == readSerialBuffer()) return false; //check for data
  if (data.startsWith("$GPGGA")){ //code below is for intepreting GGA
    
    //read utc data
    uint8_t delimIndex = data.indexOf(",") + 1;                                   //skip to utc data
    utc = data.substring(delimIndex, data.indexOf(",", delimIndex)).toFloat();    //read utc

    //read fix data
    for (int i = 0; i < 5; i++) delimIndex = data.indexOf(",", delimIndex) + 1;   //skip to fix data
    fix = data.substring(delimIndex, data.indexOf(",", delimIndex)).toInt();      //read fix

    //read altitude data
    for (int i = 0; i < 3; i++) delimIndex = data.indexOf(",", delimIndex) + 1;   //skip to fix data
    alt = data.substring(delimIndex, data.indexOf(",", delimIndex)).toFloat();    //read alt

    return true; //if there is new data in buffer
  }
  
  //since setup did not work, resend command
  if (data.startsWith("$GP")) sendCommand(PMTK_SET_NMEA_OUTPUT_GGA);
  else sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
  return false;
}

void MacRocketry_GPS_Shield::displayData(){
  //display to serial monitor
  Serial.print(getData());
  Serial.print("UTC: "); Serial.print(getUTC());
  Serial.print(" Fix: "); Serial.print(getFix());
  Serial.print(" Altitude "); Serial.println(getAltitude());
}


//End of Source ======================================================================================================
//Start of Main ======================================================================================================

MacRocketry_GPS_Shield gps; //create an object

void setup() {
  Serial.begin(115200); //for serial monitor
  while (!Serial); //wait for serial to be initialized
  Serial.println("Start GPS...");
  
}

void loop() {
  //need to call function readData() every loop
  //if there is new data, the function returns true
  if (gps.readData()){
    Serial.print(gps.getData()); //access NMEA data
    Serial.print("UTC: ");
    Serial.print(gps.getUTC()); //access UTC [float]
    Serial.print(" Fix: ");
    Serial.print(gps.getFix()); //acess fix [int]
    Serial.print(" Altitude ");
    Serial.println(gps.getAltitude()); //acess altitude [float]
  }
}
