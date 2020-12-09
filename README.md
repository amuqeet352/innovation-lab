# Innovation-lab

AERAS  (version 1.0.1)
Air Quality Monitoring System


Components Used:
=> Arduino Mega
=> 3.2" TFT ILI9481 LCD
=> SDS 011
=> DHT22

Connections:
	Sensors      Mega

	DHT <--> PIN 12
	SDS <--> Serial3  (Swap wires if dont work)
	Put TFT LCD on top of Mega

Libraries Used:
	=> SdsDustSensor.h
	=> DHT.h
	=? TFT_HX8357.h

Operation:
	Just make the connections properly and burn the code. You'll get Temperature, Humidity, PM2.5, PM10 and AQI levels around your environment on your screen.
	STL Files for the enclosure are also attached, if you want to 3D print and make a cool device in place of Table stand.

Short Demo Video of Device:
https://drive.google.com/file/d/1zIghLL1Dk488MAq8KQTiflVMG0ZGnygQ/view?usp=sharing
