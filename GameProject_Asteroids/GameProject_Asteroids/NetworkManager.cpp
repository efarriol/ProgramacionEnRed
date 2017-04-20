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
	int message;
	imbs.Read(&message);
	if (message == 2) {
		return true;
	}
	//Send
	if (message != 1) {
		OutputMemoryBitStream ombs;
		ombs.Write(3);
		ombs.WriteString("EloiTonto");
		socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
	}
	return false;
}
