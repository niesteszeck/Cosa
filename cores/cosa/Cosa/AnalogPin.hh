/**
 * @file Cosa/AnalogPin.hh
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2012-2014, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * This file is part of the Arduino Che Cosa project.
 */

#ifndef __COSA_ANALOG_PIN_HH__
#define __COSA_ANALOG_PIN_HH__

#include "Cosa/Pin.hh"
#include "Cosa/Interrupt.hh"
#include "Cosa/Event.hh"

/**
 * Abstract analog pin. Allows asynchronous sampling.
 */
class AnalogPin : public Pin, public Interrupt::Handler, public Event::Handler 
{
public:
  /**
   * Construct abstract analog pin for given Arduino pin with reference and
   * conversion completion interrupt handler.
   * @param[in] pin number.
   * @param[in] ref reference voltage.
   */
  AnalogPin(Board::AnalogPin pin, Board::Reference ref = Board::AVCC_REFERENCE) :
    Pin((uint8_t) pin),
    m_reference(ref),
    m_value(0),
    m_event(Event::NULL_TYPE)
  {
  }

  /**
   * Set reference voltage for conversion.
   * @param[in] ref reference voltage.
   */
  void set_reference(Board::Reference ref) 
  {
    m_reference = ref; 
  }

  /**
   * Get latest sample. 
   * @return sample value.
   */
  uint16_t get_value() 
  { 
    return (m_value); 
  }

  /**
   * Set analog conversion clock prescale (2..128, step power 2).
   * @param[in] factor prescale.
   */
  static void prescale(uint8_t factor);

  /**
   * Sample analog pin. Wait for conversion to complete before 
   * returning with sample value.
   * @param[in] pin number.
   * @param[in] ref reference voltage.
   * @return sample value.
   */
  static uint16_t sample(uint8_t pin, Board::Reference ref = Board::AVCC_REFERENCE);

  /**
   * Get power supply voltage in milli-volt. May be used for low battery
   * detection. Uses the internal 1V1 bandgap reference.
   * @param[in] vref reference voltage in milli-volt (default is 1100).
   * @return milli-volt.
   */
  static uint16_t bandgap(uint16_t vref = 1100);

  /**
   * Enable analog conversion.
   */
  static void powerup() __attribute__((always_inline))
  {
    bit_set(ADCSRA, ADEN);
  }

  /**
   * Disable analog conversion.
   */
  static void powerdown() __attribute__((always_inline))
  {
    bit_clear(ADCSRA, ADEN);
  }

  /**
   * Sample analog pin. Wait for conversion to complete before 
   * returning with sample value.
   * @return sample value.
   */
  uint16_t sample() __attribute__((always_inline))
  {
    return (m_value = AnalogPin::sample(m_pin, (Board::Reference) m_reference));
  }

  /**
   * Sample analog pin. Wait for conversion to complete before 
   * returning with sample value.
   * @param[out] var variable to receive the value.
   * @return analog pin.
   */
  AnalogPin& operator>>(uint16_t& var) __attribute__((always_inline))
  { 
    var = sample();
    return (*this);
  }

  /**
   * Request sample of analog pin. Pushes given event on completion. 
   * Default event is null/no event pushed for sample_await().
   * @param[in] event to push on completion.
   * @return bool.
   */
  bool sample_request(uint8_t event = Event::NULL_TYPE)
  {
    m_event = event;
    return (sample_request(m_pin, (Board::Reference) m_reference));
  }

  /**
   * Await conversion to complete. Returns sample value
   * @return sample value.
   */
  uint16_t sample_await();

  /**
   * @override Interrupt::Handler
   * Interrupt service on conversion completion.
   * @param[in] arg sample value.
   */
  virtual void on_interrupt(uint16_t arg);

  /**
   * @override AnalogPin
   * Default on change function. 
   * @param[in] value.
   */
  virtual void on_change(uint16_t value) {}

protected:
  static AnalogPin* sampling_pin;
  uint8_t m_reference;
  uint16_t m_value;
  uint8_t m_event;
  
  /**
   * Internal request sample of analog pin. Set up sampling of given pin
   * with given reference voltage.
   * @param[in] pin number.
   * @param[in] ref reference voltage.
   * @return bool.
   */
  bool sample_request(uint8_t pin, uint8_t ref);

  /**
   * @override Event::Handler
   * Handle analog pin periodic sampling and sample completed event.
   * Will call virtual method on_change() if the pin value has changed since
   * latest sample.
   * @param[in] type the type of event.
   * @param[in] value the event value.
   */
  virtual void on_event(uint8_t type, uint16_t value);

  /** Interrupt Service Routine */
  friend void ADC_vect(void);
};

#endif
