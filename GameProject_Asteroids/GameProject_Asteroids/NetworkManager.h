#pragma once
#define NM NetworkManager::Instance()

class NetworkManager
{
private:

public:
	inline static NetworkManager &Instance(void) {
		static NetworkManager networkManager;
		return networkManager;
	}

	NetworkManager();
	~NetworkManager();
	bool ConnectionEstablishment();
	void Receive();
	void Send();
};

