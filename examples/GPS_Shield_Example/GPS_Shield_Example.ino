#include <MacRocketry_GPS_Shield.h>

MacRocketry_GPS_Shield gps; //create an object

void setup() {
  Serial.begin(115200); //for serial monitor
  while (!Serial); //wait for serial to be initialized
  Serial.println("Start GPS...");
  
}

void loop() {
  //need to call function readData() every loop
  //if there is new data, the function returns true
  if (gps.parseData()){
    Serial.print(gps.getData()); //access NMEA data
    Serial.print("UTC: ");
    Serial.print(gps.getUTC()); //access UTC [float]
    Serial.print(" Fix: ");
    Serial.print(gps.getFix()); //acess fix [int]
    Serial.print(" Altitude ");
    Serial.println(gps.getAltitude()); //acess altitude [float]
  }
}
