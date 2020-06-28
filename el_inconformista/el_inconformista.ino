/**************************************************************************************
 * "Ma non è tanto questo, è il medium di massa in sé. Dal momento in cui qualcuno ci *
 *  ascolta dal video, ha verso di noi un rapporto da inferiore a superiore che è un  *
 *  rapporto spaventosamente antidemocratico."                                        *
 *                                                                Pier Paolo Pasolini *
 * "El Inconformista || The Nonconformist "                                           *
 *  by Leandro Estrella [https://cargocollective.com/leandroestrella]                 *
 **************************************************************************************/
 
/* Libraries */
#include <VarSpeedServo.h>  // include VarSpeedServo library by Philip van Allen
#include <Wire.h>           // include Wire library by Nicholas Zambetti and James Tichenor
#include <SRF02.h>          // include SRF02 library by Dirk Grappendorf
#include <TEA5767.h>        // include TEA5767 FM radio Module library by Vasilakis Michalis

/* Constants (won't change) */
SRF02 sensor (0x70, SRF02_CENTIMETERS);
TEA5767 Radio;              // Pinout SLC and SDA - Arduino uno pins A5 and A4
const int servoPin = 3;     // the pin number for the servo signal
#define MS 105              // movement speed
#define NN 18               // number of notes
/* notes */
#define X 35                // contact point angle for servo motor
#define A 50
#define B 60
#define C 90
#define D 120

/* Variables (will change) */
/* Servo */
VarSpeedServo myservo;            // create servo object to control a servo
int servoPosition = 50;           // the current angle of the servo.

/* SRF02 */
unsigned long currentMillis = 0;  // stores the value of millis() in each iteration of loop()
unsigned long nextPrintSRF = 0;   // ...
int sensorValueSRF = 0;           // variable to store the value coming from the sensor
int sensorValueSRFPrev = 0;       // variable to store the previous value of the sensor

/* TEA5767 */
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned long nextPrintFM = 0;    // ...

void setup() {
  Serial.begin(9600);
  Serial.println("Starting FM Radio, Servo and Sensor");
  Wire.begin();
  Radio.init();
  Radio.set_frequency(106.00);      // on power on go to station
  myservo.write(servoPosition);     // sets the initial position
}

void loop() {
  // Notice that none of the action happens in loop() apart from reading
  // millis() it just calls the functions that have the action code
  currentMillis = millis();
  sensorSense();              //  starts srf02 sensor loop
  servoSong();                //  starts servo protest song loop
  fmRadio();                  //  starts tea5767 fm radio loop
}

void sensorSense() {
  SRF02::update();
  if (millis() > nextPrintSRF) {
    sensorValueSRF = sensor.read();
    if (sensorValueSRF != sensorValueSRFPrev) {
    Serial.print("SRF02 ");
    Serial.print(sensorValueSRF);
    Serial.println("cm");
    sensorValueSRFPrev = sensorValueSRF;
    nextPrintSRF = millis () + 75;
    }
  }
}

void servoSong() {
  int sensorValue = sensor.read();
  if (millis() > nextPrintSRF) {
    if ((sensorValue > 0) && (sensorValue < 150)) {
      myservo.attach(servoPin);
      myservo.write(C, MS, true);     // go to position C at speed of MS and wait till finish
      myservo.write(X, MS, true);
      myservo.write(B, MS, true);
      myservo.write(X, MS, true);
      myservo.write(C, MS, true);
      myservo.write(X, MS, true);
      myservo.write(B, MS, true);
      myservo.write(X, MS, true);
      myservo.write(C, MS, true);
      myservo.write(X, MS, true);
      myservo.write(B, MS, true);
      myservo.write(X, MS, true);
      myservo.write(C, MS, true);
      myservo.write(D, MS, true);
      myservo.write(C, MS, true);
      myservo.write(D, MS, true);
      myservo.write(C, MS, true);
      myservo.write(D, MS, true);
      myservo.write(C, MS, true);     // go to position C at speed of MS and wait till finish
      myservo.write(X, MS, true);
      myservo.write(B, MS, true);
      myservo.write(X, MS, true);
      myservo.write(C, MS, true);
      myservo.write(X, MS, true);
      myservo.write(B, MS, true);
      myservo.write(X, MS, true);
      myservo.write(C, MS, true);
      myservo.write(X, MS, true);
      myservo.write(B, MS, true);
      myservo.write(X, MS, true);
      myservo.write(C, MS, true);
      myservo.write(D, MS, true);
      myservo.write(C, MS, true);
      myservo.write(D, MS, true);
      myservo.write(C, MS, true);
      myservo.write(D, MS, true);
    } else {
      myservo.detach();
    }
  }
}

void fmRadio() {
  int sensorValueFM = sensor.read();
  unsigned char buf[5];
  int stereo;
  int signal_level;
  double current_freq;

  if (millis() > nextPrintFM) {
    if (Radio.read_status(buf) == 1) {
      current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
      stereo = Radio.stereo(buf);
      signal_level = Radio.signal_level(buf);
      Serial.print("FM: ");
      Serial.print(current_freq);
      Serial.print(" Signal level: ");
      Serial.print(signal_level);
      if (stereo) {
        Serial.println(" STEREO ");
      } else {
        Serial.println(" MONO ");
      }
    }
    if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
        search_mode = 0;
      }
    }
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
    nextPrintFM = millis () + 2500;
  }
}
