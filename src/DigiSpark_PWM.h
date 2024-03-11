/* DigiSpark_PWM Library
   TheYo Tester - https://github.com/theyo-tester/DigiSpark_2_PWM
   forked from soylentOrange - https://github.com/soylentOrange/DigiSpark_PWM Thank you soylentOrange!
*/

#ifndef __DIGISPARK_PWM_HPP
#define __DIGISPARK_PWM_HPP

#include "Arduino.h"

#if !defined(ATTINY_CORE) || !defined(__AVR_ATtinyX5__)
#error This library is designed only for AVR_ATtinyX5 boards using ATTinyCore
#endif

#define ERROR_OK 0
#define ERROR_NOT_INITIALIZED 1
#define ERROR_INVALID_PIN 2
class DigiSpark_PWM {
protected:
  uint32_t _frequency;
  uint8_t _TOP_value;
  uint8_t _dutyCyclePercentPB1;
  uint8_t _dutyCyclePercentPB4;
  bool _PWM_initialized;
  bool _usePB1;
  bool _usePB4;
  uint8_t setDutyCycleAtPin(uint8_t* i_dutyCyclePercent,uint8_t pin);

public:
  DigiSpark_PWM();   // use only PB4
  DigiSpark_PWM(boolean withPB1,boolean withPB4=false);  // when specifying only first boolean, you use only PB1

  uint8_t begin(uint32_t frequency = 25000, uint8_t dutyCyclePercentPB1 = 0, uint8_t dutyCyclePercentPB4 = 0);
  inline uint8_t begin(uint32_t frequency = 25000, uint8_t dutyCyclePercentPB1_4_or_both = 0) { return begin(frequency, dutyCyclePercentPB1_4_or_both, dutyCyclePercentPB1_4_or_both); }
  uint8_t setDutyCycle(uint8_t dutyCyclePercentPB1=0, uint8_t dutyCyclePercentPB4=0);
  /// \brief
  /// setDutyCycle
  /// \details
  /// sets the duty cycle given in percent for the activated pins (PB1, PB4 or both)
  /// \return
  /// 0 on success, an error otherwise
  inline uint8_t setDutyCycle(uint8_t dutyCyclePercentPB1_4_or_both) { return setDutyCycle(dutyCyclePercentPB1_4_or_both, dutyCyclePercentPB1_4_or_both); }
  uint8_t setFrequency(uint32_t frequency = 25000);
  // uint8_t getPin();
  inline bool isUsingPB1 () { return _usePB1; }
  inline bool isUsingPB4 () { return _usePB4; }
};

#endif  //__DIGISPARK_PWM_HPP
