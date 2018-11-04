#include <Arduino.h>                //Arduino Library
#include <MacRocketry_GPS_Shield.h> //header file
#include <SoftwareSerial.h>         //SoftwareSerial library

MacRocketry_GPS_Shield::MacRocketry_GPS_Shield() : serial(GPS_RX_PIN, GPS_TX_PIN) //constructor
{
    start();
    init();
}

void MacRocketry_GPS_Shield::init()
{ //initialize variables to null
    data = "";
    serialStr = "";
    serialChar = "";

    //does not have to initialize, but for consistency sake
    utc = 0;
    altitude = 0;
    fix = 0;
    indexPre = 0;
    indexEnd = 0;
}

void MacRocketry_GPS_Shield::start()
{                                              //set up GPS
    serial.begin(9600);                        //default baud rate for GPS
    sendCommand(PMTK_SET_NMEA_BAUDRATE_38400); //set GPS baud rate
    serial.begin(38400);                       //restart with faster baud rate
    sendCommand(PMTK_SET_NMEA_OUTPUT_GGA);
    sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
}

void MacRocketry_GPS_Shield::sendCommand(String cmd)
{ //send PMTK command to GPS
    serial.flush();
    serial.println(cmd); //send command
}

bool MacRocketry_GPS_Shield::bufferSerial()
{ //buffer serial data
    while (serial.available())
    {                               //while still data
        serialChar = serial.read(); //read byte
        serialStr += serialChar;    //append to string
        if (serialStr.endsWith("\r\n"))
        {                     //if end of sentence
            data = serialStr; //record data
            serialStr = "";   //reset string buffer
            return true;
        }
    }
    return false;
}

bool MacRocketry_GPS_Shield::readData()
{
    if (bufferSerial())
    { //check for data

        if (data.startsWith("$GPGGA"))
        { //code below is for intepreting GGA

            //read utc data
            indexPre = data.indexOf(",") + 1;
            indexEnd = data.indexOf(",", indexPre);
            utc = data.substring(indexPre, indexEnd).toFloat(); //read time

            //read fix data
            indexPre = indexEnd + 1;
            for (int i = 0; i < 4; i++)
            {
                indexPre = data.indexOf(",", indexPre) + 1;
            }
            indexEnd = data.indexOf(",", indexPre);
            fix = data.substring(indexPre, indexEnd).toInt();

            //read altitude data
            indexPre = indexEnd + 1;
            for (int i = 0; i < 2; i++)
            {
                indexPre = data.indexOf(",", indexPre) + 1;
            }
            indexEnd = data.indexOf(",", indexPre);
            altitude = data.substring(indexPre, indexEnd).toFloat();

            return true; //if there is new data in buffer
        }
        //if setup didn't work, resend command
        else if (data.startsWith("$GP"))
        {
            sendCommand(PMTK_SET_NMEA_OUTPUT_GGA);
            sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
        }
    }
    return false; //no new data in buffer
}

void MacRocketry_GPS_Shield::displayData()
{ //display to serial monitor
    Serial.print(data);
    Serial.print("UTC: ");
    Serial.print(utc);
    Serial.print(" Fix: ");
    Serial.print(fix);
    Serial.print(" Altitude ");
    Serial.println(altitude);
}