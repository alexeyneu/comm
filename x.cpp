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


int main(void)
{
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	RakNet::RakPeerInterface *client=RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* p;

	// Record the first client that connects to us so we can pass it to the ping function
	RakNet::SystemAddress clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	bool packetFromServer;
	RakNet::SocketDescriptor socketDescriptor(19132, 0);
	socketDescriptor.port = 0;
	client->Startup(1, &socketDescriptor, 1);


	bool b = client->Ping( "localhost", 19132, false);
	puts("yeah");
	if (b == false) puts("nothing"); 

	while(1)
	{
		p = client->Receive();
		if (p == 0) continue;

		puts("ball");
		switch (p->data[0])
		{
			case ID_UNCONNECTED_PONG:
				puts("ID UNCONNECTED PONG");
				{
					unsigned int dataLength;
					RakNet::TimeMS time;
					unsigned long timel;
					RakNet::BitStream bsIn(p->data, p->length, false);
					bsIn.IgnoreBytes(1);
					RakNet::RakString b;

					bsIn.Read(time);
//					time = timel;

					bsIn.IgnoreBytes(24);
					bsIn.Read(b);


					dataLength = p->length - 5;
					printf("ID_UNCONNECTED_PONG from SystemAddress %s.\n", p->systemAddress.ToString(true));
					printf("Time is %i\n", time);
					printf("Ping is %i\n", (unsigned int)(RakNet::GetTimeMS() - time));
					printf("Data is %i bytes long.\n", dataLength);
					if (dataLength > 0)
					{
						std::locale::global(std::locale(""));
						b.Printf();
						
						std::cout << std::endl << std::endl;;
					}
				}
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
		client->DeallocatePacket(p);
		break;
	}
	RakNet::RakPeerInterface::DestroyInstance(client);
}


