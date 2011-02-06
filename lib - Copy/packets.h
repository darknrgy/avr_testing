#ifndef PACKETS_H
#define PACKETS_H

/*
#ifndef PACKETS_DEVICEADDRESS
#error "No SimpleProtocol address assigned to this device";
#endif 
*/


#include <string.h>
#include "usart.h"

#define PACKETS_MAXLENGTH 		8 // 8 bytes
#define PACKETS_QUEUESIZE		8 

// packet structure
typedef struct Packet{
	uint8_t address;
	char data[PACKETS_MAXLENGTH];
	uint16_t crc;
} Packet;




// dequeue a full packet from the queue
Packet Packets_getPacket();

// parse a recieved string for packets
void Packets_rxCallback(char* rx);


#endif // #ifndef PACKETS_H
