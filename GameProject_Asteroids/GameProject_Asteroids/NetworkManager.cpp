#include "NetworkManager.h"



NetworkManager::NetworkManager()
{

}


NetworkManager::~NetworkManager()
{

}

bool NetworkManager::ConnectionEstablishment(Player* &player, OnlinePlayer* &onlinePlayer, AsteroidsManager* &asteroidsManager)
{
	sf::sleep(sf::milliseconds(50));

	socket.setBlocking(false);
	//Receive
	char messageBuffer[2000];
	size_t messageSize = 0;
	status = socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
	PlayerInfo::PacketType packetType = PlayerInfo::PacketType::PT_EMPTY;
	if (status == sf::Socket::Status::Done) {
		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		if (messageSize > 0) {
			imbs.Read(&player->id, 1);
			imbs.Read(&packetType, 3);
		}

		if (packetType == PlayerInfo::PacketType::PT_GAMESTART) {
			int messageId = 0;
			std::vector<Asteroid*> tempAsteroids;
			int numAsteroids = 0;

			imbs.Read(&onlinePlayer->id, 1);
			imbs.Read(&messageId, 5);
			imbs.Read(&numAsteroids, 4);

			for (int i = 0; i < numAsteroids; i++) {
				int id = 0;
				sf::Vector2i asteroidPosition = sf::Vector2i(0, 0);
				sf::Vector2i asteroidDirection = sf::Vector2i(0, 0);
				int speed = 0; 
				int sign = 0;
				imbs.Read(&id, 4);
				imbs.Read(&asteroidPosition.x, 30);
				imbs.Read(&asteroidPosition.y, 30);
				imbs.Read(&asteroidDirection.x, 10);
				imbs.Read(&sign, 1);
				if (sign == NEGATIVE) asteroidDirection.x *= -1;
				imbs.Read(&asteroidDirection.y, 10);
				imbs.Read(&sign, 1);
				if (sign == NEGATIVE) asteroidDirection.y *= -1;
				imbs.Read(&speed, 3);
				tempAsteroids.push_back(new Asteroid(Vector2D(asteroidPosition.x, asteroidPosition.y), Vector2D(asteroidDirection.x, asteroidDirection.y), speed));
			}
			asteroidsManager = new AsteroidsManager(tempAsteroids);
			return true;
		}
		else if (packetType == PlayerInfo::PacketType::PT_WELCOME) isWelcomed = true;
	}
		if (!isWelcomed) {
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

	
		//acumulation and send...
		/*std::cout << "x:" << player->GetAccumuledMovement().x << std::endl;
		std::cout << "y: " << player->GetAccumuledMovement().y << std::endl;
*/

		player->UpdatePosition();
		onlinePlayer->UpdatePosition(absolutePositions, lambda);

		if (deltaTime.asMilliseconds() > 200) {
			absolutePos = sf::Vector2i((int)player->GetPixelsPosition().x, (int)player->GetPixelsPosition().y);
			OutputMemoryBitStream movementOmbs;
			movementOmbs.Write(player->id, 1);
			movementOmbs.Write(PlayerInfo::PacketType::PT_MOVEMENT, 3);
			movementOmbs.Write(player->GetAccumuledMovement().x, 30);
			if (player->GetAccumuledMovement().x >= 0) movementOmbs.Write(POSITIVE, 1);
			else movementOmbs.Write(NEGATIVE, 1);
			movementOmbs.Write(player->GetAccumuledMovement().y, 30);
			if (player->GetAccumuledMovement().y >= 0) movementOmbs.Write(POSITIVE, 1);
			else movementOmbs.Write(NEGATIVE, 1);
			movementOmbs.Write(absolutePos.x, 10);
			movementOmbs.Write(absolutePos.y, 10);
			movementOmbs.Write(player->GetAngle(), 9);
			socket.send(movementOmbs.GetBufferPtr(), movementOmbs.GetByteLength(), serverIP, 5001);
			player->RestartAccumuledMovement();
			deltaClock.restart();
	}

	//Receive
	bool pointIsInVector = false;
	char messageBuffer[2000];
	size_t messageSize = 0;
	status = socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
	if (status == sf::Socket::Status::Done) {
		absolutePos = sf::Vector2i(0, 0);
		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		PlayerInfo::PacketType packetType = PlayerInfo::PacketType::PT_EMPTY;
			int id = 0;
			int messageId = 0;
			if (messageSize > 0) {
				imbs.Read(&id, 1);
				imbs.Read(&packetType, 3);
			}

			sf::Vector2i receivedAccumulationMovement = sf::Vector2i(0, 0);
			int sign = 0;
			int opponentid = 0;

			OutputMemoryBitStream ombs;
			switch (packetType)
			{
			case PlayerInfo::PT_PING:
				ombs.Write(player->id, 1);
				ombs.Write(PlayerInfo::PacketType::PT_PING, 4);
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, senderPort);
				break;
			case PlayerInfo::PT_MOVEMENT:
				imbs.Read(&receivedAccumulationMovement.x, 30);
				imbs.Read(&sign, 1);
				if (sign == NEGATIVE)receivedAccumulationMovement.x *= -1;
				imbs.Read(&receivedAccumulationMovement.y, 30);
				imbs.Read(&sign, 1);
				if (sign == NEGATIVE)receivedAccumulationMovement.x *= -1;
				imbs.Read(&absolutePos.x, 10);
				imbs.Read(&absolutePos.y, 10);
				if (id == player->id) {
					imbs.Read(&receivedAngle, 9);
					player->UpdatePosition(receivedAccumulationMovement, absolutePos, receivedAngle);
				}
				else if (id == onlinePlayer->id) {
					imbs.Read(&receiveOpponentAngle, 9);
					for (int i = 0; i < absolutePositions.size(); i++) {
						if (absolutePositions[i].x == absolutePos.x && absolutePositions[i].y == absolutePos.y) {
							onlinePlayer->UpdateAngle(receiveOpponentAngle);
							pointIsInVector = true;
						}
					}
					if (!pointIsInVector) absolutePositions.push_back(sf::Vector3i(absolutePos.x, absolutePos.y, receiveOpponentAngle));
				}				
				break;
			case PlayerInfo::PT_GAMESTART:
				imbs.Read(&opponentid, 1);
				imbs.Read(&messageId, 5);
				ombs.Write(id, 1);
				ombs.Write(PlayerInfo::PacketType::PT_ACK, 3);
				ombs.Write(messageId, 5);
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
				break;
			case PlayerInfo::PT_DISCONNECT:
				Disconnect(player, true);
				break;
			default:
				break;
		}
	}
}

void NetworkManager::Disconnect(Player* &player, bool partner)
{
	OutputMemoryBitStream ombs;
	ombs.Write(player->id, 1);
	ombs.Write(PlayerInfo::PacketType::PT_DISCONNECT, 4);
	socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), serverIP, 5001);
	if (partner) SM.SetCurScene<EndScene>();
	else exit(0);
}
