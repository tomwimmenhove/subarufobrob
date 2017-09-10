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


#endif // PROTOCOL_H
