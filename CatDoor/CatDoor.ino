/*
 Name:		CatDoor.ino
 Created:	2/09/2018 09:18:25 PM
 Author:	Housame Oueslati
*/

int sensorPinOutDoor = A0; // select the input pin for LDR for the outdoor
int sensorPinInDoor = A1; // select the input pin for LDR for the indoor

int sensorValueOutDoor = 0; // variable to store the value coming from the sensor for OutDoor
int sensorValueInDoor = 0; // variable to store the value coming from the sensor for IutDoor

int inPin = 2;         // the number of the input pin
int inPinOutDoor = 3;     // the number of the input pin ONLY OutDoor
int outPin = 13;       // the number of the output pin for the startButton
int outPinOutDoorOnly = 10;  // the number of the output pin for the startButton for ONLY outdoor

int sensorOutPinOutDoor = 12; // the number of the output pin for the sensor of the outdoor
int sensorOutPinInDoor = 11; // the number of the output pin for the sensor of the indoor

int state = HIGH;      // the current state of the output pin
int stateOutDoor = LOW; // the current state of the output pin for OutDoor ONLY

int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin
int readingOutDoor;           // the current reading from the input pin for OutDoor ONLY
int previousOutDoor = LOW;    // the previous reading from the input pin for OutDoor ONLY

							  // the follow variables are long's because the time, measured in miliseconds,
							  // will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long timeOutDoor = 0;
long debounce = 200;
// variables will change:
void setup() {
	Serial.begin(9600); //sets serial port for communication
	pinMode(inPin, INPUT_PULLUP);
	pinMode(outPin, OUTPUT);
	pinMode(inPinOutDoor, INPUT_PULLUP);
	pinMode(outPinOutDoorOnly, OUTPUT);
}
void loop() {
	// -----------Button on/off------------
	reading = digitalRead(inPin);

	// if the input just went from LOW and HIGH and we've waited long enough
	// to ignore any noise on the circuit, toggle the output pin and remember
	// the time
	if (reading == LOW && previous == HIGH && millis() - time > debounce) {
		if (state == HIGH)
		{
			// The hole system will shut down
			state = LOW;
			//Making sure that the outdoor will also shut down
			stateOutDoor = LOW;
			Serial.println("OFF");
		}
		else
		{
			state = HIGH;
			Serial.println("ON");
		}
		time = millis();
	}
	if (state == HIGH)
	{
		readingOutDoor = digitalRead(inPinOutDoor);

		if (readingOutDoor == LOW && previousOutDoor == HIGH && millis() - timeOutDoor > debounce)
		{

			Serial.println("Second btn pushed");
			if (stateOutDoor == LOW)
			{
				//ONLY entrance is permitted
				stateOutDoor = HIGH;
				Serial.println("Only OutDoor is ON...");
			}
			else
			{
				stateOutDoor = LOW;
				Serial.println("Only OutDoor is OFF...");
			}
			timeOutDoor = millis();
		}

	}

	digitalWrite(outPin, state);
	digitalWrite(outPinOutDoorOnly, stateOutDoor);

	previous = reading;
	previousOutDoor = readingOutDoor;

	//---------- Sensor && output led------------------  
	if (state == HIGH)
	{
		if (stateOutDoor == LOW)
		{
			sensorValueOutDoor = analogRead(sensorPinOutDoor); // read the value from the sensor OutDoor
			sensorValueInDoor = analogRead(sensorPinInDoor); // read the value from the sensor InDoor

			Serial.println(sensorValueOutDoor); //prints the values coming from the sensor on the screen OutDoor
			Serial.println(sensorValueInDoor); //prints the values coming from the sensor on the screen InDoor

			delay(100);
			if (sensorValueOutDoor > 500)
			{
				digitalWrite(sensorOutPinOutDoor, HIGH);
				Serial.println("Door closed/outdoor sensor...");
			}
			if (sensorValueOutDoor < 500)
			{
				digitalWrite(sensorOutPinOutDoor, LOW);
				Serial.println("The cat is coming in/outdoor sensor...");
			}
			if (sensorValueInDoor > 500)
			{
				digitalWrite(sensorOutPinInDoor, HIGH);
				Serial.println("Door closed/indoor sensor...");
			}
			if (sensorValueInDoor < 500)
			{
				digitalWrite(sensorOutPinInDoor, LOW);
				Serial.println("The cat is going out/outdoor sensor...");
			}
		}
		else
		{
			sensorValueOutDoor = analogRead(sensorPinOutDoor); // read the value from the sensor OutDoor
			delay(100);
			if (sensorValueOutDoor > 500)
			{
				digitalWrite(sensorOutPinOutDoor, HIGH);
				Serial.println("Door closed/outdoor sensor...");
			}
			if (sensorValueOutDoor < 500)
			{
				digitalWrite(sensorOutPinOutDoor, LOW);
				Serial.println("The cat is coming in/outdoor sensor...");
			}
		}

	}
	else
	{
		Serial.println("System is OFF");
	}
	//----------------------------------------------
}
