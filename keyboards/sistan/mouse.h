/* Copyright 2018 tobynet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MY_MOUSE_H
#define MY_MOUSE_H

void send_mouse_click_with_wait(uint32_t wait);
void send_mouse_click(void);
void send_mouse_down(uint8_t code);
void send_mouse_up(uint8_t code);
void send_mouse_move(int8_t delta_x, int8_t delta_y);

#endif
