/*
Copyright(C) 2017 Nicholas Koza

This file is part of SwiftCarve.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CONFIG_h
#define _CONFIG_h

#define PREHEAT_DELAY_IN_MS       500         // Give the coils a bit to warm up before the air pump enables
#define FOG_DURATION_IN_MS        (750)  // How long to run the fogger heat coils for
#define PUMP_COOLDOWN_DELAY_IN_MS (1000)  // Leaves the pump on for a little bit while the fogger is off, to help cool it down and pump out any last fog
#define FOG_COOLDOWN_DELAY_IN_MS  (1000 * 5)  // Lock out the fog system from running again for a duration, to give it time to cool back down

#define SANDISK_CHIP_SELECT_PIN  4
#define SPEAKER_PIN              9

#define SHOW_TRIGGER_PIN         14 // INPUT. The fog, the pump, and the spark lights are together known as "show"
#define MOTOR_TRIGGER_PIN        15 // INPUT
#define MOTOR_RELAY_PIN          8  // OUTPUT
#define FOG_RELAY_PIN            7  // OUTPUT
#define PUMP_RELAY_PIN           2  // OUTPUT
#define FORWARD_LIGHT_PIN        3  // PWM
#define SPARK_LIGHT_PIN_1        5  // PWM
#define SPARK_LIGHT_PIN_2        9  // PWM

#endif