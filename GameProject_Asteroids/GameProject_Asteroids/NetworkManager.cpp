#include "NetworkManager.h"



NetworkManager::NetworkManager()
{

}


NetworkManager::~NetworkManager()
{

}

bool NetworkManager::ConnectionEstablishment()
{
	sf::sleep(sf::milliseconds(50));

	socket.setBlocking(false);
	//Receive
	char messageBuffer[2000];
	size_t messageSize = 0;
	socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
	InputMemoryBitStream imbs(messageBuffer, messageSize*8);
	PlayerInfo::PacketType packetType = PlayerInfo::PacketType::PT_EMPTY;
	int id;
	imbs.Read(&id);
	imbs.Read(&packetType);
	if (packetType != PlayerInfo::PacketType::PT_WELCOME) {
		OutputMemoryBitStream ombs;
		ombs.Write(id);
		ombs.Write(PlayerInfo::PacketType::PT_HELLO);
		socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
	}
	else if(PlayerInfo::PacketType::PT_GAMESTART){
		return true;
	}

	return false;
}
