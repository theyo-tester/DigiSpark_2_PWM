# DigiSpark_2_PWM
*Original badges:<br>*
[![arduino-library-badge](https://www.ardu-badge.com/badge/DigiSpark_PWM.svg?)](https://www.ardu-badge.com/DigiSpark_PWM)
[![GitHub release](https://img.shields.io/github/release/soylentOrange/DigiSpark_PWM.svg)](https://github.com/soylentOrange/DigiSpark_PWM/releases)
[![GitHub](https://img.shields.io/github/license/soylentOrange/DigiSpark_PWM)](https://github.com/soylentOrange/DigiSpark_PWM/blob/master/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/soylentOrange/DigiSpark_PWM)](https://github.com/soylentOrange/DigiSpark_PWM/issues)

---

## Abstract
Forked from [Original Library](https://github.com/soylentOrange/DigiSpark_PWM)
##### I liked this library and its idea, but I missed the option to have the hardware based PWM on ***2***** pins simultaneously. I modified how you specifiy the pins at initialisation and I introduced two different duty cycles for each pin separately. *Frequency can not be adjusted separately, only the duty cycle!* But this is, at least for my use cases, not needed.
This hardware-based Pulse-Width-Modulation (PWM) library enables you to use the Hardware-PWM on **Arduino AVR ATtiny85-based boards (Digispark)** using [**ATTinyCore**](https://github.com/SpenceKonde/ATTinyCore). The **Timer/Counter1** is used in _asynchronous mode_ to give a PWM-output on **Pin PB1** **AND**/or **Pin PB4** up to 500 kHz. The maximum resolution is **8-bit**, the effective resolution typically between **7-bit** and **8-bit**.

**Please note:** If your board is working at 16Mhz (instead of 8Mhz), the ferquency set in this library will be **doubled**. This also applies to the original library. So whether you burn the bootloader to 8MHz or you just take this into account when setting the frequency
 
---

## Table of Contents

* [Why do we need this library](#why-do-we-need-this-library)
* [Example](#example)
* [Function Reference](#functions)
* [Installation](#installation)
* [To Do](#todo)

---

## Why do we need this library

I needed a hardware PWM for a [**DigiSpark-board**](https://www.azdelivery.de/en/products/digispark-board) to drive a fan at 25kHz and didn't found a library supporting the [**ATTinyCore**](https://github.com/SpenceKonde/ATTinyCore).  
If you have one of the boards and need a flexible hardware-based PWM, this library might also fit for you. Otherwise, I'm still happy to use it ;).

> See the provided example on how to use the library.

--

## Example
A bare minimum example is given below:
```c++
#include "DigiSpark_PWM.h"  // https://github.com/theyo-tester/DigiSpark_2_PWM

// Create instance of DigiSpark_PWM-class, connected to Pin-PB1 AND Pin-PB4
// (Pin PB1 is connected to the onboard LED)
DigiSpark_PWM pwm = DigiSpark_PWM(true,true);

// For connecting to Pin PB4 simply use:
// DigiSpark_PWM pwm = DigiSpark_PWM();

// For connecting to Pin PB1 you use:
// DigiSpark_PWM pwm = DigiSpark_PWM(true);   // PB4 is than by default deactivated

// begin PWM-output
void setup() {
  // initialize and start PWM-output @1Hz with 50% duty-cycle on pin PB1 and 70% duty-cycle on pin PB4
  pwm.begin(1, 50,70);
  // initialize and start PWM-output @1Hz with 50% duty-cycle on **the activated pins (PB1, PB4 or both)**
  // pwm.begin(1, 50);
}

void loop() {
  // nothing to do here, the LED will blink driven by the hardware PWM
  // latter add your code and conditions which would lead to a global frequency and/or dutyCycle change;
  // pwm.setDutyCycle(100,100)    //f.i. set full power on both outputs
}
```

### Get example

*This is not yet supported! By following this, you will get the example from the original Repo! Alternatively you just copy the example given above with copy paste into your sketch*<br>
The example can be obtained from within the Arduino IDE in File->Examples->DigiSpark_PWM->DigiSpark_PWM_example.

---

## Functions
#### uint8_t begin(uint32_t frequency, uint8_t dutyCyclePercentPB1 = 0, uint8_t dutyCyclePercentPB4 = 0)**
This function initializes the library. Call before use...  
Initial frequency (in Hz) and duty cycle (in percent) *for PB1 and/or PB4* are given here. If you just use PB4 (3rd param), you can just specify 0 for the second parameter. 
Default duty cycle (if omitted) is 0 =>OFF.
The funcion will return 
* an error if PWM is unavailable:
* or 0 if everything went well.
#### uint8_t begin(uint32_t frequency, uint8_t dutyCyclePercentPB1_4_or_both) 
This function initializes the library. Call before use...  
Initial frequency (in Hz) and duty cycle (in percent) *for the activated Pins (PB1,PB4 or both)*** are given here.
The funcion will return 
* an error if PWM is unavailable:
* or 0 if everything went well.

#### uint8_t setDutyCycle(uint8_t dutyCyclePercentPB1=0, uint8_t dutyCyclePercentPB4=0)**
Dynamically set the duty cycle (in percent) *for the pins PB1 and/or PB4. If you didn't activate a pin at initialization, the parameter for the given pin will be ignored***
The funcion will return an error if PWM is unavailable:
* ERROR_NOT_INITIALIZED  (0x01) - pwm is not initialized yet,
* or 0 if everything went well.
#### uint8_t setDutyCycle(uint8_t dutyCyclePercentPB1_4_or_both) 
Dynamically set the duty cycle (in percent) *for the activated pins (PB1, PB4 or both)***
The funcion will return an error if PWM is unavailable:
* ERROR_NOT_INITIALIZED  (0x01) - pwm is not initialized yet,
* or 0 if everything went well.

#### uint8_t setFrequency(uint32_t frequency) 
Dynamically set the freuqncy of the pwm (in Hz) *for all activated pins***. The duty cycle will match the prevous setting.
The funcion will return an error if PWM is unavailable:
* ERROR_NOT_INITIALIZED  (0x01) - pwm is not initialized yet,
* or 0 if everything went well.
#### bool isUsingPB1 ()**
Find out whether this was initialized with PB1 (true) or not (false)
#### bool isUsingPB4 ()**
Find out whether this was initialized with PB4 (true) or not (false)<br>
~~uint8_t getPin()
Returns the pin given during instanciation.~~**


** means this is new!
---

## Installation
#### By now you can only add this library manually to your IDE and it will override the original one. 
#### Library Manager 
*This works by now only for the original library:*<br>
Open up the `Arduino Library Manager` in the Arduino IDE and search for [**DigiSpark_PWM**](https://github.com/soylentOrange/Forced-DigiSpark_PWM/). Select / install the the latest version. Click this badge [![arduino-library-badge](https://www.ardu-badge.com/badge/DigiSpark_PWM.svg?)](https://www.ardu-badge.com/DigiSpark_PWM) for more detailed instructions.

#### Download
Press the green _clone or download_ button in the upper-right and download as _.ZIP_. Then go to the Arduino IDE and go to _Sketch_>Use Library->Add .ZIP Library_ and select the just downloaded zip file.

## ToDo
* Rename this library in source code to *DigiSpark_2PWM* f.i., to avoid confusion or talk to the author of the original lib to include this breaking changes to the original library
* More tests to see if this works as expected in every circumstance.
