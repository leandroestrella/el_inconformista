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
/* Debug */
bool printEnabled = true;   /* sets serial debug (enabled(disabled) */

/* Sensor (SRF02) */
SRF02 sensor (0x70, SRF02_CENTIMETERS); /* SRF02 instance with the specified (7-bit) device id and measurement
                                           mode (SRF02_INCHES, SRF02_CENTIMETERS, SRF02_MICROSECONDS) */
#define sensorTrigger 225               /* sensor value to trigger servo */
#define sensorWait 1000                 /* millis to wait to allow sensor print */

/* Radio (TEA5767) */
TEA5767 radio = TEA5767();  /* pinout SLC and SDA - Arduino uno pins A5 and A4 */
#define radioSignal 9       /* radio minimum required signal level (0-5=unuseable, 6-9=poor, 10-12=good,
                                 13-14=really good, 15=excellent)  */
#define radioWaitMin 2500   /* millis to wait before changing station (delay) */

/* Servo (HS-53) */
VarSpeedServo servo;        /* create servo object */
#define servoPin 3          /* servo motor i/o pin */
#define servoSpeed 95       /* movement speed */
#define servoContact 36     /* contact point with saucepan */
#define servoIdle 90        /* idle position */
#define servoWait 500       /* millis to wait to allow servoEnable (delay) */
#define servoRepeatMin 6    /* minimum ammount of times to repeat servo loop */

/* VARIABLES */
/* Sensor (SRF02) */
unsigned long sensorNextPrint = 0;  /* stores next time to print sensor read (in millis) */
int sensorLastValue = 0;            /* stores last sensor read value */
int sensorValue = 0;                /* stores the value coming from the sensor */

/* Radio (TEA5767) */
unsigned long radioNextPrint = 0; /* stores next time to print radio station (in millis) */
unsigned long radioWait = 0;      /* stores next time to wait to change station (in millis) */

/* Servo (HS-53) */
bool servoEnabled = false;        /* stores servo state (enabled/disabled) */
const byte servoNotes = 6;        /* number of positions (notes) and speeds */
int servoPositions[servoNotes];   /* stores servo positions (notes) */
int servoSpeeds[servoNotes];      /* stores servo speeds to move between notes */
int servoRepeat = 0;              /* times to repeat servo loop */
unsigned long servoDelay = 0;     /* stores next time to enable servo (in millis) */

void setup() {
  randomSeed(analogRead(0));      /* if analog input pin 0 is unconnected, random analog noise will cause
                                     the call to randomSeed() to generate different seed numbers each time
                                     the sketch runs. randomSeed() will then shuffle the random function. */
  Wire.begin();                   /* join i2c bus (address optional for master) */
  SRF02::setInterval(70);         /* set the measurement intervall in milliseconds */
  radio.init(radioSignal);        /* initialize needed features and search for stations */
  servo.attach(servoPin);                   /* attach servo to servoPin */
  servo.write(servoIdle, servoSpeed, true); /* sets the initial position */
  servo.detach();                           /* stop attached servo from pulsing its i/o pin */

  /* Enable/Disable Printing */
  if (printEnabled) {
    Serial.begin(9600);                 /* start serial communication at 9600bps */
    Serial.println("El Inconformista by Leandro Estrella");
    Serial.println("Starting Radio, Servo and Sensor");
    Serial.print("Found ");
    Serial.print(radio.getStations());  /* get number of stations from library */
    Serial.println(" station(s)");
  }
}

void loop() {
  if (servoEnabled) {
    /* assign random values to servoPositions and servoSpeeds array */
    for (byte i = 0; i < (sizeof(servoPositions) / sizeof(servoPositions[0])); i++) {
      servoPositions[i] = random(servoIdle/2, servoIdle);
      servoSpeeds[i] = random(servoSpeed, servoSpeed*2);
    }
    servo.attach(servoPin); /* attach servo to servoPin */
    servoRepeat = random(servoRepeatMin, servoRepeatMin*2); /* set servoRepeat random value */
    
    /* print servo positions and repeat times */
    if (printEnabled) {
      Serial.println("Servo: ");
      for (byte i = 0; i < (sizeof(servoPositions) / sizeof(servoPositions[0])); i++) {
        Serial.print("position: ");
        Serial.print(servoPositions[i]);
        Serial.print(", speed: ");
        Serial.println(servoSpeeds[i]);
      }
      Serial.print("x");
      Serial.print(servoRepeat);
      Serial.println(" times");
    }

    /* play servo notes */
    for (int i = 0 ; i < servoRepeat ; i++) {
      for (byte i = 0; i < (sizeof(servoPositions) / sizeof(servoPositions[0])); i++) {
        /* go to servoPosition/servoContact at speed of servoSpeed and wait till finish */
        servo.write(servoPositions[i], servoSpeeds[i], true);
        servo.write(servoContact, servoSpeed, true);
      }
    }
    servo.write(servoIdle, servoSpeed, true); /* sets the initial position */
    servo.detach();                           /* stop attached servo from pulsing its i/o pin */
    servoEnabled = false;                     /* set servo state as disabled */
    servoDelay = millis() + servoWait;
    
    /* print servo state */
    if (printEnabled) {
      Serial.print("Servo disabled, waiting for: ");
      Serial.print(int(millis() - servoDelay));
      Serial.println(" millis");
    }
      
  } else if (!servoEnabled) {
    SRF02::update();              /* update sensor data */
    sensorValue = sensor.read();  /* read the current distance value */
    
    /* print sensor read */
    if (printEnabled) {
      /* check if its different than last read and higher than 0 */
      if ((millis() > sensorNextPrint) && (sensorValue != sensorLastValue) && (sensorValue >= 0)) {
        Serial.print("Sensor: ");
        Serial.print(sensorValue);
        Serial.print("cm");
        sensorLastValue = sensorValue;
        sensorNextPrint = millis() + sensorWait;
        Serial.print(", next read in: ");
        Serial.print(int(sensorNextPrint - millis())); /* millis to next sensor read */
        Serial.println(" millis");
      }
    }
    
    if (millis() > servoDelay) {
      /* check if someone is inside the sensor range */
      if ((sensorValue > 0) && (sensorValue < sensorTrigger)) {
        servoEnabled = true;  /* set servo state as enabled */
        
        /* print servo status */
        if (printEnabled) {
          Serial.println("Servo enabled");
        }
      }
    }
    
    if (millis() > radioNextPrint) {
      radio.nextStation();  /* go to next station */
      radioWait = random(radioWaitMin, (radioWaitMin*2));
      radioNextPrint = millis() + radioWait;
      
      /* print radio status */
      if (printEnabled) {
        Serial.print("Radio:  ");
        Serial.print(radio.getFrequency());             /* current frequency */
        Serial.print(", level: ");
        Serial.print(radio.getSignalLevel());           /* current signal level (0-15) */
        Serial.print(", change in: ");
        Serial.print(int(radioNextPrint - millis())); /* millis to next radio */
        Serial.println(" millis");
      }
    }
  }
}
