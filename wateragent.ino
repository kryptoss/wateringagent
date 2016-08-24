/**
Author Cristina Santana krispu13<>gmail
Author Fernando Cerezal kryptos21<>gmail

    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


This software is to be used in a watering system conected to:
Relay 1: Water pump
Relay 2: Valve, to avoid communicating vessels problem if plants are lower than water pump
Potentiometer: For selecting how much time watering the plants. Useful mainly for testing.
Two strip wire inside tank. While water touchs both wire, there is enough water, else water pump should not be started.

Using 2 relays, power should be connected directly, because arduino hos not enough current in its pins.
Potentiometer should be connected with side pins to Vcc and ground, and central pin to an arduino analog pin
A strip wire should be connected to 5V and the other to GPIO pin

**/

#define version "0.8"
const int valvePin = 7;      // the number of the riego pin (output)
const int pumpPin = 4;
const int wateringTimeSelector = A2;   //number of analog pin, potentiometer
const int emptyTankSensor= 9; //



long delayTime = 100L;          //delay for avoid inifinite loop doing nothing
long limitCounterPerDay=24L*60L*60L*(1000L/delayTime);  //number delayTime units in 24 hours
long dailyCounter = (24L*60L*60L-60)*(1000L/delayTime);  //first time watering a minute after switch on the device

void setup() {
  pinMode(valvePin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(emptyTankSensor, INPUT);
  pinMode(wateringTimeSelector, INPUT);
  Serial.begin(9600);
  // set initial watering state
  digitalWrite(valvePin, HIGH);// off, relays active at low state
  digitalWrite(pumpPin, HIGH);// 
  Serial.println(version);
}

void watering() {
  
  if(emptyTankSensor==HIGH){
      int val = analogRead(wateringTimeSelector);
      Serial.print("read value ");
      Serial.println(val);
      val = map(val, 0, 1024, 5, 120);//maps read value (0-1024 10 bits adc) to between 5 to 120 seconds
      Serial.print("Time on: ");
      Serial.println(val);
      digitalWrite(valvePin, LOW);//relays active at low state
      digitalWrite(pumpPin, LOW);
      Serial.println("Watering on");
      val = val*1000;//
      delay(val);
      digitalWrite(valvePin, HIGH);
      digitalWrite(pumpPin, HIGH);
      dailyCounter=dailyCounter+(val/100);//adds time used for watering in 100ms units
      Serial.println("Watering off");
      
  }
}

void loop() {
  if (dailyCounter == limitCounterPerDay){
    watering();
    dailyCounter = 0;
  }else{
      delay(delayTime);
      dailyCounter++;

      if(dailyCounter%10==0){
      Serial.print("daily counter: ");
      Serial.println(dailyCounter);
      Serial.print("Limit counter: ");
      Serial.println(limitCounterPerDay);
      Serial.print("real: ");
      Serial.println(millis()/100);
      Serial.println("#######");
      }
  }
}
