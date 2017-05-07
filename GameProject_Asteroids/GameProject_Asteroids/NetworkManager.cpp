#include "NetworkManager.h"



NetworkManager::NetworkManager()
{

}


NetworkManager::~NetworkManager()
{

}

bool NetworkManager::ConnectionEstablishment(Player* &player, OnlinePlayer* &onlinePlayer)
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
		imbs.Read(&onlinePlayer->id, 1);
		return true;
	}
	else if (packetType != PlayerInfo::PacketType::PT_WELCOME) {
		OutputMemoryBitStream ombs;
		ombs.Write(player->id, 1);
		ombs.Write(PlayerInfo::PacketType::PT_HELLO, 3);
		ombs.WriteString(player->name);
		socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
	}

	return false;
}

void NetworkManager::IngameConnection(Player* &player, OnlinePlayer* &onlinePlayer)
{
	if (firstClock) {
		player->RestartAccumuledMovement();
		deltaClock.restart();
		firstClock = false;
	}
	deltaTime = deltaClock.getElapsedTime();

	if (deltaTime.asMilliseconds() > 100) {
		absolutePos = sf::Vector2i(player->GetPosition().x, player->GetPosition().y);
		movementMessages.push_back(MovementMessage(idCount, player->GetAccumuledMovement(), absolutePos, player->GetAngle()));
		SendMovementMessage(idCount, player);
		idCount++;
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
	if (messageSize > 0) {
		imbs.Read(&id, 1);
		imbs.Read(&packetType, 3);
	}

	sf::Vector2i receivedAccumulationMovement = sf::Vector2i(0, 0);
	int receivedAngle = 0;
	int sign = 0;
	int opponentid = 0;	
	int criticalId = 0;

	bool acknowledged = false;

	std::vector<int> criticalIds;

	OutputMemoryBitStream ombs;
	switch (packetType)
	{
	case PlayerInfo::PT_PING:
		break;
	case PlayerInfo::PT_MOVEMENT:
		imbs.Read(&receivedAccumulationMovement.x, 30);
		imbs.Read(&sign, 1);
		if (sign == NEGATIVE)receivedAccumulationMovement.x *= -1;
		imbs.Read(&receivedAccumulationMovement.y, 30);
		imbs.Read(&sign, 1);
		if (sign == NEGATIVE)receivedAccumulationMovement.x *= -1;
		imbs.Read(&receivedAngle, 9);
		imbs.Read(&criticalId, 30);
		for (int i = 0; i < criticalIds.size(); i++) {
			if (criticalId == criticalIds[i]) acknowledged = true;
		}
		if(!acknowledged){
			if (id == player->id) player->UpdatePosition(receivedAccumulationMovement, receivedAngle);
			else if (id == onlinePlayer->id) onlinePlayer->UpdatePosition(receivedAccumulationMovement, receivedAngle);
		}

		criticalIds.push_back(criticalId);
		ombs.Write(id, 1);
		ombs.Write(PlayerInfo::PacketType::PT_ACK, 3);
		ombs.Write(criticalId, 30);
		socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
		//player->UpdateAngle(receivedAngle);
		break;
	case PlayerInfo::PT_GAMESTART:
		imbs.Read(&opponentid, 1);
		imbs.Read(&criticalId, 30);
		criticalIds.push_back(criticalId);
		ombs.Write(id , 1);
		ombs.Write(PlayerInfo::PacketType::PT_ACK, 3);
		ombs.Write(criticalId, 30);
		socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
		break;
	case PlayerInfo::PT_DISCONNECT:
		break;
	default:
		break;
	}
}

void NetworkManager::SendMovementMessage(int messageId, Player* &player){
	OutputMemoryBitStream movementOmbs;
	movementOmbs.Write(player->id, 1);
	movementOmbs.Write(PlayerInfo::PacketType::PT_MOVEMENT, 3);
	movementOmbs.Write(movementMessages[messageId].id, 70);		//CONTROLAR MAX ID
	movementOmbs.Write(movementMessages[messageId].delta.x, 30);
	if (movementMessages[messageId].delta.x >= 0) movementOmbs.Write(POSITIVE, 1);
	else movementOmbs.Write(NEGATIVE, 1);
	movementOmbs.Write(movementMessages[messageId].delta.y, 30);
	if (movementMessages[messageId].delta.y >= 0) movementOmbs.Write(POSITIVE, 1);
	else movementOmbs.Write(NEGATIVE, 1);
	movementOmbs.Write(movementMessages[messageId].absolutePos.x, 10);
	movementOmbs.Write(movementMessages[messageId].absolutePos.y, 10);
	movementOmbs.Write(movementMessages[messageId].angle, 9);
	socket.send(movementOmbs.GetBufferPtr(), movementOmbs.GetByteLength(), serverIP, 5001);
}

void NetworkManager::Disconnect()
{
	exit(0);
}
