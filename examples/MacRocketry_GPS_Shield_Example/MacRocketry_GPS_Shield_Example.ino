#include <MacRocketry_GPS_Shield.h>

MacRocketry_GPS_Shield gps; //create an object

void setup() {
  Serial.begin(115200); //for serial monitor
  Serial.println("Start GPS...");
  
}

void loop() {
  //need to call function readData() every loop
  //if there is new data, the function returns true
  if (gps.readData()){
    Serial.print(gps.data); //access NMEA data
    Serial.print("UTC: ");
    Serial.print(gps.utc); //access UTC [float]
    Serial.print(" Fix: ");
    Serial.print(gps.fix); //acess fix [int]
    Serial.print(" Altitude ");
    Serial.println(gps.altitude); //acess altitude [float]
  }
}
