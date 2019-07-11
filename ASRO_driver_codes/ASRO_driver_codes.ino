
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

#include <MPU6050.h>


#define takeoff;//define a function to be called at takeoff
#define landing;//define a function to be called at landing

#include <NMEAGPS.h>//include neo gps library to guide rocket during landing
#include <Stepper.h>//one stepper signal is sent to four stepper motors simultanously;through a stepper motor shield..freakin library refuses to recognise four stepper motors
#include <Servo.h>
Servo  extractionvalve, refillingvalve, mainoxygenvalve, mainkerosenevalve, turbineoxygenvalve, turbinekerosenevalve, chamberoxygenvalve, chamberkerosenevalve, hypergolicstuff, thruster1, thruster2, thruster3, thruster4, Xaxis, Yaxis; //declare servo types
const int startermotorrelay1 = 13; //set startermotorrelay1 to pin 12 (power starter motor with external power)
const int startermotorrelay2 = 14; //set startermotorrelay2 to pin 13(power starter motor with internal-on board battery power)
const int sparkplug1 = 14;//lights up turbine engine
const int sparkplug2 = 15;//lights up combustion engine
const int parachuterelay = 16;//lights up explosive bolts to eject parachute



const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper locks(stepsPerRevolution, 8, 9, 10, 11);

void setup() {






  //------------------------------------------------------
  //setup bmp280 sensor
  Serial.begin(9600);
  while (!Serial);   // time to get serial running
  Serial.println(F("BME280 test"));

  unsigned status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
  //----------------------------------------------------------
  //set up landing legs
  // step one revolution  in one direction to close the lander legs (all three legs have steppers that move a threaded bolt that lock onto a nut to clamp the legs onto body ahead of launch):
  Serial.println("moving clockwise:now clamping legs tight");
  locks.step(stepsPerRevolution);
  delay(500);


  // set the speed at 60 rpm:
  locks.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(startermotorrelay1, OUTPUT);
  pinMode(startermotorrelay2, OUTPUT);
  pinMode(sparkplug1, OUTPUT);
  pinMode(sparkplug2, OUTPUT);
  pinMode(parachuterelay, OUTPUT);

  //------------------------------------------------------
  //set up all valves
  //attach valves to pins
  extractionvalve.attach(15);
  refillingvalve.attach(16);
  mainoxygenvalve.attach(17);
  mainkerosenevalve.attach(18);
  turbineoxygenvalve.attach(19);
  turbinekerosenevalve.attach(20);
  chamberoxygenvalve.attach(21);
  chamberkerosenevalve.attach(22);
  hypergolicstuff.attach(22);
  thruster1.attach(22);
  thruster2.attach(23);
  thruster3.attach(24);
  thruster4.attach(25);
  Xaxis.attach(26);
  Yaxis.attach(27);





  //--------------------------------------------------------------
  //close all valves for safety
  //-------------------------------------------------------
  refillingvalve.write(0);//close oxygen  refilling valve at the start before powering rocket
  mainoxygenvalve.write(0);//fully close main oxygen valve
  mainkerosenevalve.write(0);//fully close main kerosene valve
  turbineoxygenvalve.write(0);//fully close
  turbinekerosenevalve.write(0);//fully close
  chamberoxygenvalve.write(0);//fully close chamber oxygenvalve
  chamberkerosenevalve.write(0);//fully close chamber kerosene valve
  hypergolicstuff.write(0);//fully close hypergolic chamber valve
  thruster1.write(0);//close cold thruster valve
  thruster2.write(0);//close cold thruster valve
  thruster3.write(0);//close cold thruster valve
  thruster4.write(0);//close cold thruster valve
}

void loop() {
  if (Serial.available() > 0) {
    char data = Serial.read();
    switch (data) {
      case '1'://case for takeoff
        //turbine engine start
        //  ---------------------------------------------------------- -
        extractionvalve.write(20);//open exraction valve a bit to replalce outgoing kerosene,eabling rocket to function efficiently
        mainoxygenvalve.write(90);//fully open main oxygen valve
        mainkerosenevalve.write(90);//fully open main kerosene valve

        digitalWrite(startermotorrelay1, HIGH); //turn on starter motor with relay that routes power from launch pad into the motor to spin turbine
        delay(5000);//delay 5 seconds for turbine to get up to full speed

        digitalWrite(sparkplug1, HIGH); //turn on a spark to ignite turbine pump
        for (int pos1 = 0; pos1 >= 70; pos1++) {
          turbineoxygenvalve.write(pos1);//slighty open turbine oxygen valve so as not to flood the the small engine with high pressure fuel from pump
        }
        for (int pos2 = 0; pos2 >= 70; pos2++) {
          turbinekerosenevalve.write(pos2);//slighty open turbine kerosene valveso as not to flood the the small engine with high pressure gas from pump
          //pos1 and pos2 can be adjusted to get a specific ratio of fuel to oxygen
          //pos1 and pos2 should not exceed 70 otherwise will flood it with reactants for high pressure lines
        }
        delay(6000);//delay 4 seconds for turbine to warm up to full speed

        //main engine start
        //----------------------------------------------------------------
        chamberoxygenvalve.write(90);//fully open chamber oxygenvalve to allow oxygen in
        chamberkerosenevalve.write(90);//fully open chamber kerosene valve to allow fuel in
        //-----------and take off----------------------------------------
        break;

      case '0':
        int height;
        Serial.print("Pressure = ");
        Serial.print(bme.readPressure() / 100.0F);
        Serial.println(" hPa");

        Serial.print("Approx. Altitude = ");
        height = bme.readAltitude(SEALEVELPRESSURE_HPA);
        Serial.print(height);
        Serial.println(" m");
//----------------------------------------
//tilt the craft to fire into the trajectory to slow down
//get data from accelerometers
//get data to cause cold thrusters to flip the craft
//get the hypergolic stuf to react
//open valves to let fuel and oxygen react to produce thrust and slow the craft
//get data from gyro/acelerometer to bring the craft into vertical position for decent using cold thrusters
//get drid fins to manipulate rocket position for a while.
//get rocket to riignite using hypergolic stuff to slow it down
//get a  combination of rocket burns to slow the rocket down a bit
//at a sulitable altitude get the parachute to deploy.
//use gps data to control the accuracy of the descent using vectored-thrust....enabled by using hydraulic or servo gimbals
//we can let the descent be piloted
//at 700 metres fire rockets to slow it down
//at 350 metres unclamp the legs
//since were usingparrachutes...vectored thrust can take the rocket to very accurate points
//this will need to be man guided using google maps in real time        
        if (height <= 60000) {//if height is less than 60 metres...deploy parachute..by exploding the explosive bolts to eject upper cone that pulls up the upper section of parrachute that drags along the mainchute
          digitalWrite(parachuterelay, HIGH);

        }
        if (height <= 700) { //if height is less than 200 metres try to land and deploy legs
          //map deceleration to effective thrust such that increased deceleration speedleads to increesd thrust

          digitalWrite(startermotorrelay2, HIGH); //turn on starter motor with relay that routes power from battery into the motor to spin turbine
          digitalWrite(sparkplug1, HIGH); //turn on a spark to ignite reaction in turbine pump
          digitalWrite(sparkplug2, HIGH); //turn on a spark to ignite reaction in combustion chamber

          for (int pos1 = 0; pos1 >= 70; pos1++) {
            turbineoxygenvalve.write(pos1);//slighty open turbine oxygen valve so as not to flood the the small engine with high pressure fuel from pump
          }
          for (int pos2 = 0; pos2 >= 70; pos2++) {
            turbinekerosenevalve.write(pos2);//slighty open turbine kerosene valveso as not to flood the the small engine with high pressure gas from pump
            //pos1 and pos2 can be adjusted to get a specific ratio of fuel to oxygen
            //pos1 and pos2 should not exceed 70 otherwise will flood it with reactants for high pressure lines
          }

          extractionvalve.write(20);//open exraction valve a bit to replalce outgoing kerosene,eabling rocket to function efficiently
          mainoxygenvalve.write(90);//fully open main oxygen valve
          mainkerosenevalve.write(90);//fully open main kerosene valve
          //--------------------------------------------------------------
         
          if (height<=350){//if height is less than 350 metres unclamp the legs and allow them to extend
          // unclamp legs
          // step one revolution  in one direction to open the lander legs(all three legs have steppers that move a threaded bolt that lock onto a nut to clamp the legs onto body ahead of launch):
          Serial.println("moving anti-clockwise:now un-clamping legs tight");//since the legs have pressurised helium/nitrogen bottles they naturally deflate/extend upon unlock...no need for moving parts
          locks.step(stepsPerRevolution);
          delay(500);
          }

          break;

        }

        if (height <= 1) {//if height is less than one metre turn off every thing

          digitalWrite(startermotorrelay2, LOW); //turn off starter motor with relay that routes power from battery into the motor to spin turbine
          digitalWrite(sparkplug1, LOW);
          digitalWrite(sparkplug2, LOW);
          turbineoxygenvalve.write(0);
          turbinekerosenevalve.write(0);
          extractionvalve.write(0);
          mainoxygenvalve.write(0);
          mainkerosenevalve.write(0);
           //rocket sholuld reener at less than 500kph...then let the parachute absorb much of that speed
          //get accelerometrers to read its orientation then flip the rocket to face opposite the trajectory using cold thrusters...fire rocket into trajectory to slow it down for landing
          //get gps to lead the rocket to a slow touch down
          // i have no freakin idea...lemme get gps library to guide the rocket down
          //also have to get mpu-6050 library to see how...the idea is to wait and see if the rocket starts decending...at the point of apogee..the first stage separates
          //....cold gas thrusters fire to change orientation of the fist stage into the opposite direction..engines light up and fire into the new direction...to slow down the space
          //craft....then grid fins deploy to guide the craft into desired trajectory.....then it fires engines to slow down a bit....grid fins continue guiding properly
          //at a desired altiude say 1km....engines fire to bring the space craft to a halt on the ground...
          //near the gound the mpu6050 maps changes in y acceleration/y gyro and slightly maps the readings to y servo to adjust orientation for one straight deceleration

        }
    }
  }
}
