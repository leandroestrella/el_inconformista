# El Inconformista

>*[...] Ma non è tanto questo, è il medium di massa in sé. Dal momento in cui qualcuno ci ascolta dal video, ha verso di noi un rapporto da inferiore a superiore che è un rapporto spaventosamente antidemocratico.*

<p align="right">Pier Paolo Pasolini (interviewed by Enzo Biagi), Terza B Facciamo l'Appello, RAI, 3/11/1971 (https://vimeo.com/290826)</p>

![el inconformista animated gif](https://github.com/leandroestrella/el_inconformista/blob/master/images/inconformista.gif)

>*The installation's general concept is the protest, taking as inspiration the South American Cacerolazo.*

The interactive work is composed by two sound elements:
- The authority represented by the radio broadcast;
- The protester represented by the wooden fork banging the saucepan; this action is triggered when the spectator goes into the electronic device control zone.

These sounds have the general form A AB A, i.e.:
- [A]	the noise of the FM radio
- [AB]	the noise of the FM radio + the wooden fork banging the saucepan
- [A]	the noise of the FM radio

Software:
- Arduino

Hardware:
- Metal saucepan
- Wooden fork
- Wooden box
- Arduino Duemilanove
- Modified speaker
- TEA5767 FM Radio Breakout Board for Arduino
- HS-53 Servo
- SRF02 Ultra sonic range finder
- LM386 Audio Amplifier Module

Dependencies:
- Wire library (https://www.arduino.cc/en/Reference/Wire) by Nicholas Zambetti and James Tichenor
- SRF02 library (https://www.grappendorf.net/arduino-atmel-avr/arduino-libraries/srf02-ultrasonic-distance-sensor.html) by Dirk Grappendorf
- TEA5767 FM radio Module library (https://github.com/big12boy/TEA5767) by big12boy 
- VarSpeedServo library (https://github.com/netlabtoolkit/VarSpeedServo) by Philip van Allen

Links & Documentation:
- Installation: https://vimeo.com/169427165 / https://youtu.be/Py2dP_EuIX4
- Presentation: https://issuu.com/leandrostar/docs/leandroestrella_el-inconformista_20_b8ae947dd1d674 [ENG] / https://issuu.com/leandrostar/docs/leandroestrella_el-inconformista_20 [ITA]
- Development: https://flic.kr/s/aHsm4aBXuT

![el inconformista schematics](https://github.com/leandroestrella/el_inconformista/blob/master/schematics/inconformista_schematics.png)