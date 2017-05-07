#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <InputOutputMemory\InputMemoryBitStream.h>
#include <InputOutputMemory\InputMemoryStream.h>
#include <InputOutputMemory\OutputMemoryBitStream.h>
#include <InputOutputMemory\OutputMemoryStream.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <PlayersInfo.h>
#include "CriticalMessage.h"
#define POSITIVE 1
#define NEGATIVE 0
#define MAX_DELTA 100000000000000000
#define OFFSET 10000

void VerifyMovement(int messageId, int &criticalCount, int playerID, std::vector<MovementMessage> &movementMessages, sf::UdpSocket &socket, std::vector<CriticalMessage*> &criticalMessages) {
	for (int i = 0; i < movementMessages.size(); i++) {
		std::cout << movementMessages[i].delta.x << std::endl;

		if (movementMessages.size() > 1 && i > 0) {
			if (movementMessages[i].id == messageId) {
				if (movementMessages[i - 1].id == (messageId-1)) {
					if (/*(movementMessages[i - 1].absolutePos.x + movementMessages[i - 1].delta.x) >= movementMessages[i].absolutePos.x-OFFSET && (movementMessages[i - 1].absolutePos.x + movementMessages[i - 1].delta.x) <= movementMessages[i].absolutePos.x + OFFSET &&
						(movementMessages[i - 1].absolutePos.y + movementMessages[i - 1].delta.y) >= movementMessages[i].absolutePos.y - OFFSET && (movementMessages[i - 1].absolutePos.y + movementMessages[i - 1].delta.y) <= movementMessages[i].absolutePos.y + OFFSET &&*/
						movementMessages[i].delta.x < MAX_DELTA && movementMessages[i].delta.y < MAX_DELTA) {
						//enviamos la posicion
						//last confirmed position
						criticalMessages.push_back(new CriticalMessage(criticalCount, playerID, PlayerInfo::PacketType::PT_MOVEMENT, movementMessages[i].id, movementMessages[i].delta, movementMessages[i].absolutePos, movementMessages[i].angle));
						movementMessages.erase(movementMessages.begin() + (i - 1));

						criticalCount++;
					}
					else {
						//Negamos posicion
					}
				}
				else {
					int lostPackets = movementMessages[i].id - movementMessages[i - 1].id;
					if (abs(movementMessages[i].absolutePos.x - movementMessages[i - 1].absolutePos.x) < MAX_DELTA*lostPackets &&
						abs(movementMessages[i].absolutePos.y - movementMessages[i - 1].absolutePos.y) < MAX_DELTA*lostPackets &&
						movementMessages[i].delta.x < MAX_DELTA && movementMessages[i].delta.y < MAX_DELTA) {
						// confirmamos posición
						movementMessages.erase(movementMessages.begin() + i - 1);
						criticalMessages.push_back(new CriticalMessage(criticalCount, playerID, PlayerInfo::PacketType::PT_MOVEMENT, movementMessages[i].id, movementMessages[i].delta, movementMessages[i].absolutePos, movementMessages[i].angle));
						criticalCount++;
					}
					else {
						//negamos posicion
					}
				}
			}
		}
		else {
			criticalMessages.push_back(new CriticalMessage(criticalCount, playerID, PlayerInfo::PacketType::PT_MOVEMENT, movementMessages[i].id, movementMessages[i].delta, movementMessages[i].absolutePos, movementMessages[i].angle));
			criticalCount++;
		}
	}
}

int main() {
	srand(time(NULL));
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::IpAddress senderIP;
	std::vector<CriticalMessage*> criticalMessages;	
	//PlayerInfo
	PlayerInfo player[2];
	bool setupDone = false;
	std::vector<MovementMessage> movementMessages;
	int playersCount = 0;
	int criticalCount = 0;
	unsigned short senderPort;
	sf::UdpSocket socket;

	sf::Clock deltaClock;
	sf::Time deltaTime;
	deltaClock.restart();


	socket.setBlocking(false);
	sf::Socket::Status status = socket.bind(5001);
	if (status != sf::Socket::Done) {
		std::cout << "Can't bind to port 5000" << std::endl;
	}
	while (true) {
		deltaTime = deltaClock.getElapsedTime();

		PlayerInfo::PacketType packetType;
		packetType = PlayerInfo::PacketType::PT_EMPTY;
		char messageBuffer[2000];
		int playerID = 0;
		size_t messageSize = 0;
		socket.receive(messageBuffer, sizeof(messageBuffer), messageSize, senderIP, senderPort);
		std::string stringMessage;
		InputMemoryBitStream imbs(messageBuffer, messageSize * 8);
		if (messageSize > 0) {
			imbs.Read(&playerID, 1);
			imbs.Read(&packetType, 3);
		}
		int messageId = 0;
		int sign = 0;
		OutputMemoryBitStream ombs;
		MovementMessage newMessage;
		switch (packetType)
		{
		case PlayerInfo::PT_ACK:
			imbs.Read(&messageId, 30);
			for (int i = 0; i < criticalMessages.size(); i++) if (messageId == criticalMessages[i]->messageId) criticalMessages.erase(criticalMessages.begin() + i);
			break;
		case PlayerInfo::PT_PING:
			break;
		case PlayerInfo::PT_HELLO:
			if (playersCount < 2 && senderPort != player[0].port && senderPort != player[1].port) {
				player[playersCount].id = playersCount;
				player[playersCount].ipAdress = senderIP;
				player[playersCount].port = senderPort;
				playersCount++;
			}
			if (senderPort == player[0].port) {
				ombs.Write(player[0].id, 1);
				ombs.Write(PlayerInfo::PacketType::PT_WELCOME, 3);
				socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), senderIP, senderPort);
			}
			else if (senderPort == player[1].port && !setupDone) {
				criticalMessages.push_back(new CriticalMessage(criticalCount, player[0].id, PlayerInfo::PacketType::PT_GAMESTART, player[1].id));
				criticalCount++;
				criticalMessages.push_back(new CriticalMessage(criticalCount, player[1].id, PlayerInfo::PacketType::PT_GAMESTART, player[0].id));
				criticalCount++;
				setupDone = true;
			}
			break;
		case PlayerInfo::PT_MOVEMENT:
			imbs.Read(&newMessage.id, 70); // CONTROLAR MAX ID
			imbs.Read(&newMessage.delta.x, 30);
			imbs.Read(&sign, 1);
			if (sign == NEGATIVE) newMessage.delta.x *= -1;
			imbs.Read(&newMessage.delta.y, 30);
			imbs.Read(&sign, 1);
			if (sign == NEGATIVE) newMessage.delta.y *= -1;
			imbs.Read(&newMessage.absolutePos.x, 10);
			imbs.Read(&newMessage.absolutePos.y, 10);
			imbs.Read(&newMessage.angle, 9);
			movementMessages.push_back(newMessage);

			VerifyMovement(newMessage.id, criticalCount, playerID, movementMessages, socket, criticalMessages);
			//ombs.Write(playerID, 1); 
			//ombs.Write(PlayerInfo::PacketType::PT_MOVEMENT, 3);
			//ombs.Write(player[playerID].accumulatedMovement.x, 30);
			//if (player[playerID].accumulatedMovement.x >= 0) ombs.Write(POSITIVE, 1);
			//else ombs.Write(NEGATIVE, 1);
			//ombs.Write(player[playerID].accumulatedMovement.y, 30);
			//if (player[playerID].accumulatedMovement.y >= 0) ombs.Write(POSITIVE, 1);
			//else ombs.Write(NEGATIVE, 1);
			//ombs.Write(player[playerID].angle, 9);
			////CRITICAL?
			//socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), player[0].ipAdress, player[0].port);
			//socket.send(ombs.GetBufferPtr(), ombs.GetByteLength(), player[1].ipAdress, player[1].port);
			break;
		case PlayerInfo::PT_DISCONNECT:
			break;
		default:
			break;
		}
		//Send criticalMessages
		if (deltaTime.asMilliseconds() > 100) {
			for (int i = 0; i < criticalMessages.size(); i++) {
				OutputMemoryBitStream ombs2;
				ombs2.Write(criticalMessages[i]->playerId, 1);
				ombs2.Write(criticalMessages[i]->packetType, 3);
				switch (criticalMessages[i]->packetType) {
				case PlayerInfo::PacketType::PT_GAMESTART:
					ombs2.Write(criticalMessages[i]->opponentId, 1);
					ombs2.Write(criticalMessages[i]->messageId, 30); // CONRTOLAR MAX CRITICAL

					break;
				case PlayerInfo::PacketType::PT_MOVEMENT:
					ombs2.Write(criticalMessages[i]->delta.x, 30);
					if (criticalMessages[i]->delta.x >= 0) ombs.Write(POSITIVE, 1);
					else ombs.Write(NEGATIVE, 1);
					ombs2.Write(criticalMessages[i]->delta.y, 30);
					if (criticalMessages[i]->delta.y >= 0) ombs.Write(POSITIVE, 1);
					else ombs.Write(NEGATIVE, 1);
					ombs2.Write(criticalMessages[i]->angle, 9);
					ombs2.Write(criticalMessages[i]->messageId, 30); // CONRTOLAR MAX CRITICAL

					break;
				}
				socket.send(ombs2.GetBufferPtr(), ombs2.GetByteLength(), player[criticalMessages[i]->playerId].ipAdress, player[criticalMessages[i]->playerId].port);
			}
			deltaClock.restart();
		}
	}
}