/*
 *  Copyright 2017 Tom Wimmenhove<tom.wimmenhove@gmail.com>
 *
 *  This file is part of Subarufobrob
 *
 *  Subarufobrob is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Subarufobrob is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Subarufobrob.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

unsigned char getCrc(unsigned char *packet);
void setCrc(unsigned char *packet, unsigned char crc);
unsigned char calcCrc(unsigned char *packet);
int isValidPacket(unsigned char *packet);
unsigned char getCommand(unsigned char *packet);
char* commandName(unsigned char command);
void setCommand(unsigned char *packet, unsigned char command);
unsigned int getCode(unsigned char *packet);
void setCode(unsigned char *packet, unsigned int code);

#define COMMAND_LOCK	0x1
#define COMMAND_UNLOCK	0x2
#define COMMAND_TRUNK	0xb
#define COMMAND_PANIC	0xa

#endif // PROTOCOL_H
