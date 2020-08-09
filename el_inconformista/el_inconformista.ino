/**************************************************************************************************************
 * "Ma non è tanto questo, è il medium di massa in sé. Dal momento in cui qualcuno ci ascolta dal video,      *
 *  ha verso di noi un rapporto da inferiore a superiore che è un rapporto spaventosamente antidemocratico."  *
 *                                                                                      - Pier Paolo Pasolini *
 * "El Inconformista || The Nonconformist "                                                                   *
 *  by Leandro Estrella [https://cargocollective.com/leandroestrella]                                         *
 *************************************************************************************************************/
 
/* LIBRARIES */
/* Sensor (SRF02) */
/* Wire by Nicholas Zambetti and James Tichenor [https://www.arduino.cc/en/Reference/Wire] */
#include <Wire.h>
/* SRF02 by Dirk Grappendorf
    [https://www.grappendorf.net/arduino-atmel-avr/arduino-libraries/srf02-ultrasonic-distance-sensor.html] */
#include <SRF02.h>

/* Radio (TEA5767) */
/* TEA5767 FM Radio Module by big12boy [https://github.com/big12boy/TEA5767] */
#include <TEA5767.h>

/* Servo (HS-53) */
/* VarSpeedServo by Philip van Allen [https://github.com/netlabtoolkit/VarSpeedServo] */
#include <VarSpeedServo.h>

/* CONSTANTS */
/* Sensor (SRF02) */
SRF02 sensor (0x70, SRF02_CENTIMETERS); /* SRF02 instance with the specified (7-bit) device id and measurement
                                           mode (SRF02_INCHES, SRF02_CENTIMETERS, SRF02_MICROSECONDS) */
#define triggerValue 225                /* sensor value to trigger servo */
#define srfWait 1000                    /* millis to wait to allow SRF02 print */

/* Radio (TEA5767) */
TEA5767 radio = TEA5767();  /* pinout SLC and SDA - Arduino uno pins A5 and A4 */
#define radioMinLvl 10      /* radio minimum required signal level (0-5=unuseable, 6-9=poor, 10-12=good,
                                 13-14=really good, 15=excellent)  */
#define radioWait 4000      /* millis to wait before changing station (delay) */

/* Servo (HS-53) */
#define servoPin 3          /* servo motor i/o pin */
VarSpeedServo servo;        /* create servo object */
#define servoSpeed 105      /* movement speed */
#define servoContact 35     /* contact point with saucepan */
#define servoIdle 50        /* idle position */
#define servoWait 75        /* millis to wait to allow servoEnable (delay) */

/* Debug */
bool printEnabled = true;   /* sets serial debug (enabled(disabled) */

/* VARIABLES */
/* Sensor (SRF02) */
unsigned long nextSRFPrint = 0;   /* stores next time to print sensor read (in millis) */
int lastSensorValue= 0;           /* stores last sensor read value */
int sensorValue = 0;              /* stores the value coming from the sensor */

/* Radio (TEA5767) */
unsigned long nextRadioPrint = 0; /* stores next time to print radio station (in millis) */

/* Servo (HS-53) */
bool servoEnabled = false;        /* stores servo state (enabled/disabled) */
int servoPositions[4];            /* stores servo positions (notes) */
int servoRepeat;                  /* times to repeat servo loop */
unsigned long servoDelay = 0;     /* stores next time to enable servo (in millis) */

void setup() {
  randomSeed(analogRead(0));      /* if analog input pin 0 is unconnected, random analog noise will cause
                                     the call to randomSeed() to generate different seed numbers each time
                                     the sketch runs. randomSeed() will then shuffle the random function. */
  Wire.begin();                   /* join i2c bus (address optional for master) */
  SRF02::setInterval(70);         /* set the measurement intervall in milliseconds */
  radio.init(radioMinLvl);        /* initialize needed features and search for stations */
  servo.attach(servoPin);                   /* attach servo to servoPin */
  servo.write(servoIdle, servoSpeed, true); /* sets the initial position */
  servo.detach();                           /* stop attached servo from pulsing its i/o pin */

  /* Enable/Disable Printing */
  if (printEnabled) {
    Serial.begin(9600);                 /* start serial communication at 9600bps */
    Serial.println("Starting Radio, Servo and Sensor");
    Serial.print("Found ");
    Serial.print(radio.getStations());  /* get number of stations from library */
    Serial.println(" station(s)");
  }
}

void loop() {
  if (servoEnabled) {
    /* assign random values to servoPositions array */
    for (byte i = 0; i < (sizeof(servoPositions) / sizeof(servoPositions[0])); i++) {
      servoPositions[i] = random(servoIdle, 100);
    }
    servo.attach(servoPin); /* attach servo to servoPin */
    servoRepeat = random(4, 8);
    
    /* print servo positions and repeat times */
    if (printEnabled) {
      Serial.print("Servo Positions: ");
      for (byte i = 0; i < (sizeof(servoPositions) / sizeof(servoPositions[0])); i++) {
        Serial.print(servoPositions[i]);
        Serial.print(", ");
      }
      Serial.print("Repeat Times: ");
      Serial.println(servoRepeat);
    }

    /* play servo notes */
    for (int i = 0 ; i < servoRepeat ; i++) {
      for (byte i = 0; i < (sizeof(servoPositions) / sizeof(servoPositions[0])); i++) {
        /* go to servoPosition/servoContact at speed of servoSpeed and wait till finish */
        servo.write(servoPositions[i], servoSpeed, true);
        servo.write(servoContact, servoSpeed, true);
      }
    }
    servo.write(servoIdle, servoSpeed, true); /* sets the initial position */
    servo.detach();                           /* stop attached servo from pulsing its i/o pin */
    servoEnabled = false;
    servoDelay = millis() + servoWait;
    
    /* print servo state */
    if (printEnabled) {
      Serial.print("Servo Disabled, waiting for: ");
      Serial.println(servoDelay);
    }
    
  } else if (!servoEnabled) {
    SRF02::update();              /* update sensor data */
    sensorValue = sensor.read();  /* read the current distance value */
    
    /* print sensor read */
    if (printEnabled) {
      /* check if its different that last read */
      if ((millis() > nextSRFPrint) && (sensorValue != lastSensorValue) && (sensorValue >= 0)) {
        Serial.print("SRF02 ");
        Serial.print(sensorValue);
        Serial.println("cm");
        lastSensorValue = sensorValue;
        nextSRFPrint = millis() + srfWait;
      }
    }
    if (millis() > servoDelay) {
      if ((sensorValue > 0) && (sensorValue < triggerValue)) {
        servoEnabled = true;
        
        /* print servo status */
        if (printEnabled) {
          Serial.println("Servo Enabled");
        }
      }
    }
    if (millis() > nextRadioPrint) {
      radio.nextStation();  /* go to next station */
      nextRadioPrint = millis() + radioWait;
      
      /* print radio status */
      if (printEnabled) {
        Serial.print("Radio Frequency: ");
        Serial.print(radio.getFrequency());       /* current frequency */
        Serial.print(" Signal Level: ");
        Serial.println(radio.getSignalLevel());   /* current signal level (0-15) */
      }
    }
  }
}
