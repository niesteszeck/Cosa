/**
 * @file Cosa/OutputPin.hh
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

#ifndef __COSA_OUTPUT_PIN_HH__
#define __COSA_OUTPUT_PIN_HH__

#include "Cosa/Pin.hh"

/**
 * Abstract output pin. 
 */
class OutputPin : public Pin {
public:
  /**
   * Construct an abstract output pin for given Arduino pin number.
   * @param[in] pin number.
   * @param[in] initial value.
   */
  OutputPin(Board::DigitalPin pin, uint8_t initial = 0) : 
    Pin((uint8_t) pin) 
  { 
    synchronized {
      *DDR() |= m_mask; 
      _set(initial);
    }
  }

  /**
   * Set output pin to mode.
   * @param[in] pin number.
   * @param[in] initial value.
   */
  static void set_mode(uint8_t pin, uint8_t initial = 0)
  {
    synchronized {
      *DDR(pin) |= MASK(pin); 
    }
    write(pin, initial);
  }

  /**
   * Set the output pin. Unprotected version.
   */
  void _set() __attribute__((always_inline)) 
  { 
    *PORT() |= m_mask; 
  }

  /**
   * Set the output pin.
   */
  void set() __attribute__((always_inline)) 
  { 
    synchronized {
      *PORT() |= m_mask; 
    }
  }

  /**
   * Set the output pin.
   */
  void high() __attribute__((always_inline)) 
  { 
    synchronized {
      *PORT() |= m_mask; 
    }
  }

  /**
   * Set the output pin.
   */
  void on() __attribute__((always_inline))   
  { 
    synchronized {
      *PORT() |= m_mask; 
    }
  }

  /**
   * Clear the output pin. Unprotected version.
   */
  void _clear() __attribute__((always_inline)) 
  { 
    *PORT() &= ~m_mask; 
  }

  /**
   * Clear the output pin.
   */
  void clear() __attribute__((always_inline)) 
  { 
    synchronized {
      *PORT() &= ~m_mask; 
    }
  }

  /**
   * Clear the output pin.
   */
  void low() __attribute__((always_inline))
  { 
    synchronized {
      *PORT() &= ~m_mask; 
    }
  }

  /**
   * Clear the output pin.
   */
  void off() __attribute__((always_inline)) 
  { 
    synchronized {
      *PORT() &= ~m_mask; 
    }
  }

  /**
   * Toggle the output pin. Unprotected version.
   */
  void _toggle() __attribute__((always_inline)) 
  { 
    *PIN() = m_mask; 
  }

  /**
   * Toggle the output pin.
   */
  void toggle() __attribute__((always_inline)) 
  { 
    synchronized {
      *PIN() = m_mask; 
    }
  }

  /**
   * Toggle the output pin.
   * @param[in] pin number.
   */
  static void toggle(uint8_t pin) __attribute__((always_inline))
  { 
    synchronized {
      *PIN(pin) = MASK(pin); 
    }
  }

  /**
   * Set the output pin with the given value. Zero(0) to clear
   * and non-zero to set. Unprotected version.
   * @param[in] value to set.
   */
  void _set(bool value) __attribute__((always_inline)) 
  { 
    if (value) {
      *PORT() |= m_mask; 
    }
    else {
      *PORT() &= ~m_mask; 
    }
  }

  /**
   * Set the output pin with the given value. Zero(0) to clear
   * and non-zero to set.
   * @param[in] value to set.
   */
  void set(bool value) __attribute__((always_inline)) 
  { 
    synchronized {
      if (value) {
	*PORT() |= m_mask; 
      }
      else {
	*PORT() &= ~m_mask; 
      }
    }
  }

  /**
   * Set the output pin with the given value. Zero(0) to clear
   * and non-zero to set. Unprotected version.
   * @param[in] value to set.
   */
  void _write(bool value) __attribute__((always_inline))
  { 
    if (value) {
      *PORT() |= m_mask; 
    }
    else {
      *PORT() &= ~m_mask; 
    }
  }

  /**
   * Set the output pin with the given value. Zero(0) to clear
   * and non-zero to set.
   * @param[in] value to write.
   */
  void write(uint8_t value) __attribute__((always_inline))
  { 
    synchronized {
      if (value) {
	*PORT() |= m_mask; 
      }
      else {
	*PORT() &= ~m_mask; 
      }
    }
  }

  /**
   * Shift out given byte to the output pin using the given clock
   * output pin. Shift out according to given direction.
   * @param[in] value to write.
   * @param[in] clk output pin.
   * @param[in] order bit first.
   */
  void write(uint8_t value, OutputPin& clk, Direction order = MSB_FIRST);

  /**
   * Shift out given value and number of bits to the output pin using
   * the given pulse length in micro-seconds. Shift out from LSB(0) to
   * MSB(bits-1). This operation is synchronized and will turn off
   * interrupt handling during the transmission period.
   * @param[in] value to write.
   * @param[in] bits to write.
   * @param[in] us micro-second bit period.
   */
  void write(uint16_t value, uint8_t bits, uint16_t us);

  /**
   * Set the given output pin with the given value. Zero(0) to 
   * clear and non-zero to set.
   * @param[in] pin number.
   * @param[in] value to write.
   */
  static void write(uint8_t pin, uint8_t value) __attribute__((always_inline))
  { 
    volatile uint8_t* port = PORT(pin);
    const uint8_t mask = MASK(pin);
    synchronized {
      if (value) {
	*port |= mask;
      }
      else {
	*port &= ~mask;
      }
    }
  }

  /**
   * Set the output pin with the given value. Zero(0) to clear
   * and non-zero to set. Output operator syntax.
   * @param[in] value to write.
   * @return output pin.
   */
  OutputPin& operator<<(uint8_t value) __attribute__((always_inline))
  {
    set(value);
    return (*this);
  }

  /**
   * Toggle the output pin to form a pulse with given length in
   * micro-seconds.
   * @param[in] us pulse width in micro seconds
   */
  void pulse(uint16_t us) __attribute__((always_inline))
  {
    toggle();
    DELAY(us);
    toggle();
  }
};

#endif
