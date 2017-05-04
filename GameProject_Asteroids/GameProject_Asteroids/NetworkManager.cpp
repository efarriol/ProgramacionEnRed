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
		//acumulation and send...
		/*std::cout << "x:" << player->GetAccumuledMovement().x << std::endl;
		std::cout << "y: " << player->GetAccumuledMovement().y << std::endl;
*/
		OutputMemoryBitStream movementOmbs;
		movementOmbs.Write(player->id, 1);
		movementOmbs.Write(PlayerInfo::PacketType::PT_MOVEMENT, 3);
		movementOmbs.Write(player->GetAccumuledMovement().x, 30);
		if (player->GetAccumuledMovement().x >= 0) movementOmbs.Write(POSITIVE, 1);
		else movementOmbs.Write(NEGATIVE, 1);
		movementOmbs.Write(player->GetAccumuledMovement().y, 30);
		if (player->GetAccumuledMovement().y >= 0) movementOmbs.Write(POSITIVE, 1);
		else movementOmbs.Write(NEGATIVE, 1);
		movementOmbs.Write(player->GetAngle(), 9);
		std::cout << player->GetAngle() << std::endl;
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
	}

	sf::Vector2i receivedAccumulationMovement = sf::Vector2i(0, 0);
	int receivedAngle = 0;
	int sign = 0;
	int opponentid = 0;

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
		if(id == player->id) player->UpdatePosition(receivedAccumulationMovement, receivedAngle);
		else if(id == onlinePlayer->id) onlinePlayer->UpdatePosition(receivedAccumulationMovement, receivedAngle);
		//player->UpdateAngle(receivedAngle);
		break;
	case PlayerInfo::PT_GAMESTART:
		imbs.Read(&opponentid, 1);
		imbs.Read(&messageId, 5);
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
