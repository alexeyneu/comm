#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "GetTime.h"
#include "BitStream.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "Gets.h"
#include "RakString.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <clocale>
#include <locale>
#include <algorithm>
#include <sstream>



const	std::string bin2hex(const unsigned char *p, size_t length) {
	std::stringstream f;
	f << std::hex << std::setfill('0');
	for (int i = 0; i < length; i++) f << std::setw(2) << (int)p[i];
	return f.str();
}

size_t hex2bin(unsigned char *p , const char *hexstr,const size_t length) {
	size_t wcount = 0;
	while ( wcount++ < length && *hexstr && *(hexstr + 1)) {    //last condition cause np if check fails on middle one.thats coz of short-circuit evaluation
		sscanf(hexstr, "%2hhx",p++);  //7x slower than tables but doesnt metter 
		hexstr = hexstr+2;
	}
	return  --wcount;     // error check here is a waste  
}	

struct tr {
#pragma pack(1)
	unsigned char id;
	unsigned long time;
	unsigned long server_uuid;
	unsigned char offline_id[16];
	unsigned short raknetstrl;
	unsigned char merk[97];	
};

int main(void)
{
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program

	RakNet::RakPeerInterface *server = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* p;

	// Record the first client that connects to us so we can pass it to the ping function

	int i = server->GetNumberOfAddresses();

	bool packetFromServer;
	tr f = {};
	f.id = 0x1c;
	f.time = RakNet::GetTimeMS();
	f.server_uuid = RakNet::SystemAddress::ToInteger(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	hex2bin(f.offline_id, "00ffff00fefefefefdfdfdfd12345678", 16);
	f.raknetstrl = 97;
	memcpy(f.merk, "MCPE;Dedicated Server;390;1.14.60;0;10;13253860892328930865;Bedrock level;Survival;1;19132;19133;", f.raknetstrl );
	std::reverse((unsigned char *)&f.raknetstrl, (unsigned char *)&f.raknetstrl + 2);
	server->SetOfflinePingResponse((const char *)(unsigned char *)&f.server_uuid, 97 + 2 + 1 + 16 + 8);

	RakNet::SocketDescriptor socketDescriptor(19132, 0);
	bool b = server->Startup(2, &socketDescriptor, 1)==RakNet::RAKNET_STARTED;
	server->SetMaximumIncomingConnections(2);

	while(1)
	{
		p = server->Receive();
		if (p == 0) continue;

		puts("ball");
		switch (p->data[0])
		{
			case ID_UNCONNECTED_PONG:
				puts("ID UNCONNECTED PONG");
				break;
			case ID_UNCONNECTED_PING:
				puts("UNCONNECTED PING");
				break;
			case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
				puts("ID UNCONNECTED PING OPEN CONNECTIONS");
				break;
			default:
				puts("unknown");
				break;
		}

		server->DeallocatePacket(p);
	}

	RakNet::RakPeerInterface::DestroyInstance(server);
}


