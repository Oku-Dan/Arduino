#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial subSerial(2, 3);
TinyGPSPlus gps;
bool wakedup = false;

void setup()
{
	Serial.begin(9600);
	subSerial.begin(9600);
}

void loop()
{
	while (subSerial.available() > 0)
	{
		char c = subSerial.read();
		gps.encode(c);
		/*if (gps.location.isUpdated())
		{
			wakedup = true;
			Serial.print("LAT=");
			Serial.println(gps.location.lat(), 8);
			Serial.print("LONG=");
			Serial.println(gps.location.lng(), 8);
			Serial.print("ALT=");
			Serial.println(gps.altitude.meters());
		}*/
		if(!wakedup)Serial.print(c);
	}
}