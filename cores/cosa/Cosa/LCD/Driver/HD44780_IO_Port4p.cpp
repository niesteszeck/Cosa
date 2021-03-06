/**
 * @file Cosa/LCD/Driver/HD44780_IO_Port4p.cpp
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014, Mikael Patel
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

#include "Cosa/Board.hh"
#if !defined(__ARDUINO_TINYX5__)
#include "Cosa/LCD/Driver/HD44780.hh"

bool
HD44780::Port4p::setup()
{
  return (false);
}

void 
HD44780::Port4p::write4b(uint8_t data)
{
  synchronized {
    m_d0._set(data & 0x01);
    m_d1._set(data & 0x02);
    m_d2._set(data & 0x04);
    m_d3._set(data & 0x08);
    m_en._toggle();
    m_en._toggle();
  }
}

void 
HD44780::Port4p::write8b(uint8_t data)
{
  synchronized {
    m_d0._set(data & 0x10);
    m_d1._set(data & 0x20);
    m_d2._set(data & 0x40);
    m_d3._set(data & 0x80);
    m_en._toggle();
    m_en._toggle();
    m_d0._set(data & 0x01);
    m_d1._set(data & 0x02);
    m_d2._set(data & 0x04);
    m_d3._set(data & 0x08);
    m_en._toggle();
    m_en._toggle();
  }
  DELAY(SHORT_EXEC_TIME);
}

void 
HD44780::Port4p::set_mode(uint8_t flag)
{
  m_rs.write(flag);
}

void 
HD44780::Port4p::set_backlight(uint8_t flag)
{
  m_bt.write(flag);
}

#endif
