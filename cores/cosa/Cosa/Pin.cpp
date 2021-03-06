/**
 * @file Cosa/Pin.cpp
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

#include "Cosa/Pin.hh"
#include "Cosa/OutputPin.hh"

uint8_t 
Pin::read(OutputPin& clk, Direction order)
{
  uint8_t value = 0;
  uint8_t bits = CHARBITS;
  if (order == MSB_FIRST) {
    do {
      clk.set();
      value <<= 1;
      if (is_set()) value |= 0x01;
      clk.clear();
    } while (--bits);
  }
  else {
    do {
      clk.set();
      value >>= 1;
      if (is_set()) value |= 0x80;
      clk.clear();
    } while (--bits);
  }
  return (value);
}

IOStream& operator<<(IOStream& outs, Pin& pin)
{
  outs << PSTR("Pin(pin = ") << (uint8_t) pin.m_pin 
       << PSTR(", sfr = ") << (void*) pin.m_sfr 
       << PSTR(", mask = ") << bin << (uint8_t) pin.m_mask << ')';
  return (outs);
}
