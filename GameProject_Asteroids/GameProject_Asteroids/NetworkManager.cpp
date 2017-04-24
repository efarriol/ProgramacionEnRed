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
	int id = 0;
	if (messageSize > 0) {
		imbs.Read(&id, 1);
		imbs.Read(&packetType, 3);
	}

	if (packetType == PlayerInfo::PacketType::PT_GAMESTART) {
		return true;
	}

	OutputMemoryBitStream ombs;
	ombs.Write(id, 1);
	ombs.Write(PlayerInfo::PacketType::PT_HELLO, 3);
	socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);

	return false;
}

void NetworkManager::IngameConnection()
{
	//Receive
	char messageBuffer[2000];
	size_t messageSize = 0;
	socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
	InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
	PlayerInfo::PacketType packetType = PlayerInfo::PacketType::PT_EMPTY;
	int id = 0;
	int messageId = 0;
	if (messageSize > 0) {
		imbs.Read(&id, 1);
		imbs.Read(&packetType, 3);
		imbs.Read(&messageId, 5);
	}
	OutputMemoryBitStream ombs;

	switch (packetType)
	{
	case PlayerInfo::PT_PING:
		break;
	case PlayerInfo::PT_MOVEMENT:
		break;
	case PlayerInfo::PT_GAMESTART:
		ombs.Write(id , 1);
		ombs.Write(PlayerInfo::PacketType::PT_ACK, 3);
		ombs.Write(messageId, 5);
		socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
		break;
	case PlayerInfo::PT_DISCONNECT:
		break;
	default:
		break;
	}
}

void NetworkManager::Disconnect()
{
	exit(0);
}
