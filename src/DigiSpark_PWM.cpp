/// @file

/* DigiSpark_PWM Library
   TheYo Tester - https://github.com/theyo-tester/DigiSpark_2_PWM
   forked from soylentOrange - https://github.com/soylentOrange/DigiSpark_PWM Thank you soylentOrange!
*/

#include "DigiSpark_PWM.h"

// re-import of avr/libraries/ATTinyCore/src/ATTinyCore.h
// from current dev-version (2.0.0) of ATTinyCore.h https://github.com/SpenceKonde/ATTinyCore
// also fixing an error in setTimer1Prescaler
#if defined(__AVR_ATtinyX5__)
#if !defined(TIMERONE_CLOCK_t)
#define RE_IMPORT_FROM_2_0_0 true
typedef enum TIMERONE_CLOCK_enum {
  TIMER1_FCPU = (0x00),
  TIMER1_64M_PLL = (0x06),
  TIMER1_32M_PLL = (0x86)
} TIMERONE_CLOCK_t;

typedef enum TIMERONE_PRESC_enum {
  TIMER1_STOPPED = (0x00),
  TIMER1_DIV1 = (0x01),
  TIMER1_DIV2 = (0x02),
  TIMER1_DIV4 = (0x03),
  TIMER1_DIV8 = (0x04),
  TIMER1_DIV16 = (0x05),
  TIMER1_DIV32 = (0x06),
  TIMER1_DIV64 = (0x07),
  TIMER1_DIV128 = (0x08),
  TIMER1_DIV256 = (0x09),
  TIMER1_DIV512 = (0x0A),
  TIMER1_DIV1024 = (0x0B),
  TIMER1_DIV2048 = (0x0C),
  TIMER1_DIV4096 = (0x0D),
  TIMER1_DIV8192 = (0x0E),
  TIMER1_DIV16384 = (0x0F)
} TIMER1_PRESCALE_t;

void setTimer1Clock(TIMERONE_CLOCK_t clk) {
  uint8_t t = PLLCSR & 0x87;
  if ((t & 0x86) == clk) {  // already set right
    return;
  }

  uint8_t oldtccr = TCCR1;
  TCCR1 = oldtccr & 0xF0;
  if ((clk & 0x04) && ((t & 0x03) != 3)) {
    // PLL requested and not currently running
    if (t & 0x02 == 0) {
      PLLCSR = clk & 0x02;  // enable the PLL
    }
    while ((PLLCSR & 0x01) == 0)
      ;  // Wait for lock
  }
  PLLCSR = clk;     // now we can set the PLLCSR.
  TCCR1 = oldtccr;  // and turn back on the timer.
}

void setTimer1Prescaler(TIMER1_PRESCALE_t presc) {
  uint8_t oldtccr = TCCR1;
  oldtccr &= 0xF0;   // fixed
  oldtccr |= presc;  // fixed
  TCCR1 = oldtccr;   // fixed
}

void setTimer1Top(uint8_t newtop) {
  OCR1C = newtop;
}

uint8_t setTimer1Frequency(uint32_t target) {
  while (true) {
    uint8_t pll = PLLCSR;
    pll &= 0x84;
    unsigned long speed;
    if (pll == 0x84) {
// slow asynchronous mode
#if F_CPU == 16500000
      speed = 33000000;
#else
      speed = 32000000;
#endif
    } else if (pll) {
// fast asynchronous mode
#if F_CPU == 16500000
      speed = 66000000;
#else
      speed = 64000000;
#endif
    } else {
      // synchronous mode
      speed = F_CPU;
    }

    uint32_t topval = speed / target;
    if (topval < 64 && pll != 0x04) {
      setTimer1Clock(TIMER1_64M_PLL);
    } else if (topval < 128 && pll == 0) {
      setTimer1Clock(TIMER1_32M_PLL);
    } else {
      uint8_t newpresc = 1;
      while ((topval > 255) && (newpresc < 15)) {
        newpresc++;
        topval >>= 1;
      }
      uint8_t top;
      if (topval > 255) {
        top = 255;
      } else {
        top = topval;
      }
      OCR1C = top;
      uint8_t tccr = TCCR1;
      tccr &= 0xF0;
      tccr |= newpresc;
      TCCR1 = tccr;
      return top;
    }
  }
}
#endif
#endif

// re-import of ATTinyCore/avr/cores/tiny/wiring_analog.c
// from current dev-version (2.0.0) of ATTinyCore.h https://github.com/SpenceKonde/ATTinyCore
// stripped down for ATtiny85
#if !defined(RE_IMPORT_FROM_2_0_0)
/// \brief
/// set duty-cycle of PWM
/// \details
/// This is just a wrapper around analogWrite
/// to set the duty-cycle of PWM by writing the compare register.
/// Will also connect the pin to the timer.
/// Pin needs to be on TIMER1. Possible values are:
/// - PIN_B1 (the onboard LED is connected here)
/// - PIN_B4
/// \return
/// 0 on success, an error otherwise
uint8_t analogWritePWM(uint8_t pin, int val) {
  if ((pin != PIN_B1) && (pin != PIN_B4)) {
    return ERROR_INVALID_PIN;
  }

  // pass through to analogWrite
  analogWrite(pin, val);

  // ...and hope that everything went fine...
  return 0;
}
#else
/// \brief
/// set duty-cycle of PWM
/// \details
/// This is a modified re-import of analogWrite from the current ATTinyCore
/// Sets duty-cycle of PWM by writing the compare register.
/// Will also connect the pin to the timer.
/// Pin needs to be on TIMER1. Possible values are:
/// - PIN_B1 (the onboard LED is connected here)
/// - PIN_B4
/// \return
/// 0 on success, an error otherwise
uint8_t analogWritePWM(uint8_t pin, int val) {

  if ((pin != PIN_B1) && (pin != PIN_B4)) {
    return ERROR_INVALID_PIN;
  }

  // make sure the pin is an output
  pinMode(pin, OUTPUT);

  uint8_t timer = digitalPinToTimer(pin);
  if (timer == TIMER1A) {
    // connect PIN_PB1 to Pulse Width Modulator channel A
    TCCR1 |= _BV(COM1A1);
    OCR1A = val;  // set compare register
  } else if (timer == TIMER1B) {
    // connect PIN_PB4 to Pulse Width Modulator channel B
    GTCCR |= _BV(COM1B1);
    OCR1B = val;  // set compare register
  } else {
    // shouldn't happen...
    return ERROR_INVALID_PIN;
  }

  // Return without error
  return 0;
}
#endif



/// \brief
/// Constructor of DigiSpark_PWM
/// \details
/// This creates a DigiSpark_PWM object.
/// Output of PWM defaults to Pin PB4
/// Remember to call begin to actually use the PWM
/// \return
/// An object of type DigiSpark_PWM
DigiSpark_PWM::DigiSpark_PWM() {
  _PWM_initialized = false;
  usePB4 = true;
  usePB1 = false;
}

/// \brief
/// Constructor of DigiSpark_PWM
/// \details
/// This creates a DigiSpark_PWM object.
/// Output of PWM will be mapped to given Pins
/// Pins need to be on TIMER1. Possible values are:
/// - PIN_B1 (the onboard LED is connected here)
/// - PIN_B4
/// Remember to call begin to actually use the PWM
/// \return
/// An object of type DigiSpark_PWM
DigiSpark_PWM::DigiSpark_PWM(boolean withPB1, boolean withPB4) {
  _PWM_initialized = false;
  usePB1 = withPB1;
  usePB4 = withPB4;
}

/// \brief
/// begin
/// \details
/// Initializes the library
/// Pin needs to be on TIMER1. Possible values are:
/// - PIN_B1 (the onboard LED is connected here)
/// - PIN_B4
/// \return
/// 0 on success, an error otherwise
uint8_t DigiSpark_PWM::begin(uint32_t frequency, uint8_t dutyCyclePercentPB1 = 0, uint8_t dutyCyclePercentPB4 = 0) {

  // initialize the Pin
  // will also disconect the pin from PWM output
  if (usePB1) {
    pinMode(PB1, OUTPUT);
    digitalWrite(PB1, LOW);
  }
  if (usePB4) {
    pinMode(PB4, OUTPUT);
    digitalWrite(PB4, LOW);
  }

  // save Frequency
  _frequency = frequency;

  // save requested duty-cycle
  _dutyCyclePercentPB1 = dutyCyclePercentPB1;
  _dutyCyclePercentPB4 = dutyCyclePercentPB4;
  // check for bounds
  if (_dutyCyclePercentPB1 > 100)
    _dutyCyclePercentPB1 = 100;
  if (_dutyCyclePercentPB4 > 100)
    _dutyCyclePercentPB4 = 100;

  // set frequency for Timer 1
  _PWM_initialized = true;
  _TOP_value = setTimer1Frequency(_frequency);

  // enable PWM (channel A and/or channel B on timer 1)
  uint8_t tccr = TCCR1;
  tccr &= 0x0F;
  tccr |= _BV(CTC1);

  // PB1 =>> PWM Channel A
  if(usePB1){
    // enable Pulse Width Modulator channel A,
    // leave PIN_PB1 unconnected (will be connected in analogWrite)
    // set CTC1 to 1 to Clear Timer/Counter on Compare Match
    tccr |= _BV(PWM1A);
    // set compare register to zero for now
    OCR1A = 0;
  }
  // PB4 =>> PWM Channel B
  if(usePB4){
    // enable Pulse Width Modulator channel B
    // connect PIN_PB4 to Pulse Width Modulator channel B
    // leave PIN_PB4 unconnected  (will be connected in analogWrite)
    GTCCR = _BV(PWM1B);
    // set compare register to zero for now
    OCR1B = 0;
  } else {
    // disable channel B if PB4 is not active
    GTCCR = 0;
  }
  TCCR1 = tccr;   // finally write back the configured register

  // set duty-cycle and connect pin
  uint8_t err = 0;
  if ((err = setDutyCycle(_dutyCyclePercentPB1,_dutyCyclePercentPB4))) {
    _PWM_initialized = false;
    return err;
  } else {
    return 0;
  }
}

/// \brief
/// setDutyCycleAtPin
/// \details
/// will only be used internal (protected) 
/// \return
/// 0 on success, an error otherwise
uint8_t DigiSpark_PWM::setDutyCycleAtPin(uint8_t* dutyCyclePercent,uint8_t pin) {
  if (*dutyCyclePercent == 0) {
    // set pin to LOW, when DC should be 0%
    // digital write also deactivates PWM
    digitalWrite(pin, LOW);
    *dutyCyclePercent=0;
    return 0;
  } else if (*dutyCyclePercent >= 100) {
    // set pin to HIGH, when DC should be 100%
    // digital write also deactivates PWM
    digitalWrite(pin, HIGH);
    *dutyCyclePercent=100;
    return 0;
  }
  
  // map duty-cycle from 0-100% range to compare value
  uint8_t analogValue;
  analogValue = map(*dutyCyclePercent, 0, 100, 0, _TOP_value);

  // set comapare value via analog write
  return analogWritePWM(pin, analogValue);
}

/// \brief
/// setDutyCycle
/// \details
/// sets the duty cycle given in percent 
/// First parameter: duty cycle on PB1
/// Second parameter: duty cycle on PB4
/// If pin PB1 is not used, you can just specicy 0 for the first parameter, it will be ignored
/// If only pin PB1 is used, you can just pass the first parameter
/// \return
/// 0 on success, an error otherwise
uint8_t DigiSpark_PWM::setDutyCycle(uint8_t dutyCyclePercentPB1, uint8_t dutyCyclePercentPB4) {
  if (!_PWM_initialized) {
    return ERROR_NOT_INITIALIZED;
  }
  uint8_t retVal;
  if(usePB1) {
    retVal=setDutyCycleAtPin(&dutyCyclePercentPB1,PB1);
    if(retVal!=0) return retVal;
    _dutyCyclePercentPB1=dutyCyclePercentPB1;
  }
  if(usePB4) {
    retVal=setDutyCycleAtPin(&dutyCyclePercentPB4,PB4);
    if(retVal!=0) return retVal;
    _dutyCyclePercentPB4=dutyCyclePercentPB4;
  }
  return 0;
}

/// \brief
/// setFrequency
/// \details
/// sets the frequency given in Hz
/// \return
/// 0 on success, an error otherwise
uint8_t DigiSpark_PWM::setFrequency(uint32_t frequency) {
  if (!_PWM_initialized)
    return ERROR_NOT_INITIALIZED;

  // save Frequency
  _frequency = frequency;

  // temporarily deactivate PWM output
  if(usePB1) digitalWrite(PB1, LOW);
  if(usePB4) digitalWrite(PB4, LOW);

  // set new frequency for Timer 1
  _TOP_value = setTimer1Frequency(_frequency);

  // set duty-cycle (using updated TOP-value)
  return setDutyCycle(_dutyCyclePercentPB1,_dutyCyclePercentPB4);
}
