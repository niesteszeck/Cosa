/**
 * @file Cosa/OutputPin.cpp
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

#include "Cosa/OutputPin.hh"

void 
OutputPin::write(uint8_t value, OutputPin& clk, Direction order)
{
  uint8_t bits = CHARBITS;
  if (order == MSB_FIRST) {
    synchronized do {
      _write(value & 0x80);
      clk._toggle();
      value <<= 1;
      clk._toggle();
    } while (--bits);
  }
  else {
    synchronized do {
      _write(value & 0x01);
      clk._toggle();
      value >>= 1;
      clk._toggle();
    } while (--bits);
  }
}

void 
OutputPin::write(uint16_t value, uint8_t bits, uint16_t us)
{
  if (bits == 0) return;
  synchronized {
    do {
      _write(value & 0x01);
      DELAY(us);
      value >>= 1;
    } while (--bits);
  }
}

