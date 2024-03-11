/* DigiSpark_PWM Library
   TheYo Tester - https://github.com/theyo-tester/DigiSpark_2_PWM
   forked from soylentOrange - https://github.com/soylentOrange/DigiSpark_PWM Thank you soylentOrange!
*/

#include "DigiSpark_PWM.h"  // https://github.com/theyo-tester/DigiSpark_2_PWM

uint8_t g_dutyCycle_pb1;   // DC in % fo PB1
uint8_t g_dutyCycle_pb4;   // DC in % fo PB4
uint32_t g_frequency_;  // frequency in Hz

// Create instance of DigiSpark_PWM-class, connected to Pin-PB1 AND Pin-PB4
// (Pin PB1 is connected to the onboard LED)
DigiSpark_PWM pwm = DigiSpark_PWM(true,true);

// For connecting to Pin PB4 simply use:
// DigiSpark_PWM pwm = DigiSpark_PWM();

// For connecting to Pin PB1 you use:
// DigiSpark_PWM pwm = DigiSpark_PWM(true);   // PB4 is than by default deactivated

// begin PWM-output
void setup() {
  // initialize and start PWM-output @1Hz with 50% duty-cycle
  g_frequency = 1;   // 1Hz
  g_dutyCycle_pb1 = 50;  // 50%
  g_dutyCycle_pb4 = 70;  // 70%
  pwm.begin(g_frequency, g_dutyCycle);
  // initialize and start PWM-output @1Hz with 50% duty-cycle on **the activated pins (PB1, PB4 or both)**
  // pwm.begin(1, 50);
}

void loop() {
  // nothing to do here, the LED will blink driven by the hardware PWM
  // latter add your code and conditions which would lead to a global frequency and/or dutyCycle change;
  // pwm.setDutyCycle(100,100)    //f.i. set full power on both outputs
}





