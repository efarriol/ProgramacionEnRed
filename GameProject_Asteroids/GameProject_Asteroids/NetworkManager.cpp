#include "NetworkManager.h"



NetworkManager::NetworkManager()
{

}


NetworkManager::~NetworkManager()
{

}

bool NetworkManager::ConnectionEstablishment(Player* &player)
{
	sf::sleep(sf::milliseconds(50));

	socket.setBlocking(false);
	//Receive
	char messageBuffer[2000];
	size_t messageSize = 0;
	socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
	InputMemoryBitStream imbs(messageBuffer, messageSize*8);
	PlayerInfo::PacketType packetType = PlayerInfo::PacketType::PT_EMPTY;
	if (messageSize > 0) {
		imbs.Read(&player->id, 1);
		imbs.Read(&packetType, 3);
	}

	if (packetType == PlayerInfo::PacketType::PT_GAMESTART) {
		return true;
	}

	OutputMemoryBitStream ombs;
	ombs.Write(player->id, 1);
	ombs.Write(PlayerInfo::PacketType::PT_HELLO, 3);
	socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);

	return false;
}

void NetworkManager::IngameConnection(Player* &player)
{
	if (firstClock) {
		deltaClock.restart();
		!firstClock;
	}
	deltaTime = deltaClock.getElapsedTime();
	if (deltaTime.asMilliseconds() > 100) {
		//acumulation and send...
		OutputMemoryBitStream movementOmbs;
		movementOmbs.Write(player->id, 1);
		movementOmbs.Write(PlayerInfo::PacketType::PT_MOVEMENT, 3);
		movementOmbs.Write(player->GetAccumuledMovement().x, 64);
		movementOmbs.Write(player->GetAccumuledMovement().y, 64);
		movementOmbs.Write(player->GetAngle(), 5);
		socket.send(movementOmbs.GetBufferPtr(), movementOmbs.GetByteLength(), serverIP, 5001);
		player->RestartAccumuledMovement();
		deltaClock.restart();
	}

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

	Vector2D receivedAccumulationMovement = 0;
	float receivedAngle = 0;
	OutputMemoryBitStream ombs;
	switch (packetType)
	{
	case PlayerInfo::PT_PING:
		break;
	case PlayerInfo::PT_MOVEMENT:
		ombs.Write(&receivedAccumulationMovement.x, 64);
		ombs.Write(&receivedAccumulationMovement.y, 64);
		ombs.Write(&receivedAngle, 64);
		player->UpdatePosition(receivedAccumulationMovement);
		player->UpdateAngle(receivedAngle);
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
