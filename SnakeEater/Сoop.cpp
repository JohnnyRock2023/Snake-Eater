#include "Header.h"

int clientSock;
int playerID;
int inviteCode;

map<int, Player> players = map<int, Player>();

queue<Package*> *sendPackageQueue = new queue<Package*>();
queue<Package*> *recvPackageQueue = new queue<Package*>();

void createSendPackage() {
	Package* pckg = new Package(game_status, score, bestScore, playerID, playerDirection, playerPosX, playerPosY, snakes, antidotes);
	sendPackageQueue->push(pckg);
}

Package* getSendPackage() {
	Package* pckg = nullptr;
	if (!sendPackageQueue->empty()) {
		pckg = sendPackageQueue->front();
		sendPackageQueue->pop();
	}
	return pckg;
}

void createRecvPackage(Package* pckg) {
	recvPackageQueue->push(pckg);
}

void getRecvPackage() {
	if (!recvPackageQueue->empty()) {
		Package* pckg = recvPackageQueue->front();
		recvPackageQueue->pop();
		if (playerID) {
			snakes.clear();
			snakes = pckg->snakes;
			antidotes.clear();
			antidotes = pckg->antidotes;
			game_status = pckg->game_status;
		}
		if (!playerID) {
			int* indexes = new int[usedAntidotes.size()];
			int i = 0;
			for (int j = 0; j < antidotes.size(); j++) {
				for (Vector2f usedAntidote : usedAntidotes) {
					if (antidotes[j] == usedAntidote) {
						indexes[i++] = j;
					}
				}
			}
			for (int j = 0; j < i; j++) {
				antidotes.erase(antidotes.begin() + indexes[j]);
			}
			delete [] indexes;
			usedAntidotes.clear();
		}
		if (score < pckg->score) {
			score = pckg->score;
		}
		if (bestScore < pckg->bestScore) {
			bestScore = pckg->bestScore;
		}
		players.clear();
		players = pckg->players;
		delete pckg;
	}
}

void syncData(std::stop_token stoken) {
	Package* pckg = nullptr;
	while (!stoken.stop_requested()) {
		pckg = getFromServer();
		if (pckg != nullptr) {
			createRecvPackage(pckg);
		}
		createSendPackage();
		pckg = getSendPackage();
		if (pckg != nullptr) {
			sendToServer(pckg);
			delete pckg;
		}
	}
}


bool createSock(const char* ipAddress) {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	fd_set set;
	FD_ZERO(&set);
	FD_SET(clientSock, &set);
	timeval timeout{};
	timeout.tv_sec = 2;
	u_long mode = 1;
	ioctlsocket(clientSock, FIONBIO, &mode);
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	inet_pton(AF_INET, ipAddress, &serverAddress.sin_addr);
	connect(clientSock, (const sockaddr*)&serverAddress, sizeof(serverAddress));
	int result = select(0, nullptr, &set, nullptr, &timeout);
	mode = 0;
	ioctlsocket(clientSock, FIONBIO, &mode);
	if (result > 0) {
		int error = 0;
		int len = sizeof(int);
		getsockopt(
			clientSock,
			SOL_SOCKET,
			SO_ERROR,
			(char*)&error,
			&len
		);
		if (error) {
			return false;
		}
	}
	return true;
}

bool sendAll(SOCKET s, const void* data, int size) {
	const char* buf = (const char*)data;
	while (size > 0) {
		int sent = send(s, buf, size, 0);
		if (sent <= 0) return false;
		buf += sent;
		size -= sent;
	}
	return true;
}

bool recvAll(SOCKET s, void* data, int size) {
	char* buf = (char*)data;
	while (size > 0) {
		int recvd = recv(s, buf, size, 0);
		if (recvd <= 0) return false;
		buf += recvd;
		size -= recvd;
	}
	return true;
}

void sendInt(int32_t v) {
	v = htonl(v);
	sendAll(clientSock, &v, sizeof(v));
}

int32_t recvInt() {
	int32_t v = 0;
	recvAll(clientSock, &v, sizeof(v));
	return ntohl(v);
}

void sendFloat(float f) {
	uint32_t v;
	memcpy(&v, &f, sizeof(f));
	v = htonl(v);
	sendAll(clientSock, &v, sizeof(v));
}

float recvFloat() {
	uint32_t v = 0;
	recvAll(clientSock, &v, sizeof(v));
	v = ntohl(v);
	float f;
	memcpy(&f, &v, sizeof(f));
	return f;
}

void createServer(string * ipAddress) {
	if (createSock(ipAddress->c_str())) {
		restart();
		mtx_game_status.lock();
		sendInt(1);
		sendObjects();
		sendInt(playerDirection);
		sendFloat(playerPosX);
		sendFloat(playerPosY);
		playerID = recvInt();
		inviteCode = recvInt();
		createSendPackage();
		sendToServer(getSendPackage());
		game_status = 1;
		mtx_game_status.unlock();
		mtx_coop_mode.lock();
		coop_mode = 3;
		mtx_coop_mode.unlock();
		StopMenuMusic();
		AudioTrack();
	}
	else {
		closesocket(clientSock);
		WSACleanup();
		mtx_coop_mode.lock();
		coop_mode = 0;
		mtx_coop_mode.unlock();
	}
}

void connectToServer(string * ipAddress, int code) {
	if (createSock(ipAddress->c_str())) {
		snakes.clear();
		objects.clear();
		antidotes.clear();
		sendInt(2);
		sendInt(code);
		int status = recvInt();
		if (!status) {
			closesocket(clientSock);
			WSACleanup();
			mtx_coop_mode.lock();
			coop_mode = 0;
			mtx_coop_mode.unlock();
			return;
		}
		sendInt(playerDirection);
		sendFloat(playerPosX);
		sendFloat(playerPosY);
		playerID = recvInt();
		inviteCode = recvInt();
		recvObjects();
		mtx_game_status.lock();
		game_status = 1;
		mtx_game_status.unlock();
		mtx_coop_mode.lock();
		coop_mode = 3;
		mtx_coop_mode.unlock();
		StopMenuMusic();
		AudioTrack();
	}
	else {
		closesocket(clientSock);
		WSACleanup();
		mtx_coop_mode.lock();
		coop_mode = 0;
		mtx_coop_mode.unlock();
	}
}

void sendObjects() {
	sendInt(objects.size());
	for (Object obj : objects) {
		sendInt(obj.getType());
		sendFloat(obj.getPos().x);
		sendFloat(obj.getPos().y);
	}
}


void sendToServer(Package* pckg) {
	if (pckg == nullptr) return;
	sendInt(pckg->game_status);
	sendInt(pckg->score);
	sendInt(pckg->bestScore);
	sendInt(pckg->playerID);
	sendInt(pckg->playerDirection);
	sendFloat(pckg->playerPosX);
	sendFloat(pckg->playerPosY);
	if (playerID == 0) {
		sendInt(snakes.size());
		for (Snake snake : pckg->snakes) {
			sendInt(snake.getLength());
			sendInt(snake.getSize());
			sendInt(snake.getDirect());
			for (SnakeBody bodyPart : snake.getBody()) {
				sendInt(bodyPart.bodyDirect1);
				sendInt(bodyPart.bodyDirect2);
				sendFloat(bodyPart.pos.x);
				sendFloat(bodyPart.pos.y);
			}
		}
		sendInt(pckg->antidotes.size());
		for (Vector2f antidotePos : pckg->antidotes) {
			sendFloat(antidotePos.x);
			sendFloat(antidotePos.y);
		}
	}
	else {
		sendInt(hittedSnakes.size());
		for (int i = 0; i < hittedSnakes.size(); i++) {
			sendInt(hittedSnakes[i]);
			hittedSnakes[i] = 0;
		}
		sendInt(usedAntidotes.size());
		for (int i = 0; i < usedAntidotes.size(); i++) {
			sendFloat(usedAntidotes[i].x);
			sendFloat(usedAntidotes[i].y);
		}
		usedAntidotes.clear();
	}
}

void recvObjects() {
	int objCount = recvInt();
	for (int i = 0; i < objCount; i++) {
		int type = recvInt();
		float posX = recvFloat();
		float posY = recvFloat();
		objects.push_back(Object(type, posX, posY));
	}
}

Package* getFromServer() {
	Package* pckg = new Package();
	int32_t status = recvInt();
	pckg->game_status = status;
	pckg->score = recvInt();
	pckg->bestScore = recvInt();
	pckg->players.clear();
	int playersNum = recvInt();
	if (playersNum < 0 || playersNum > 100) {
		delete pckg;
		return nullptr;
	}
	for (int i = 0; i < playersNum; i++) {
		int id = recvInt();
		int direct = recvInt();
		float playersPosX = recvFloat();
		float playersPosY = recvFloat();
		pckg->players.insert({ id, Player(direct, playersPosX, playersPosY)});
	}
	if (playerID != 0) {
		int snakesNum = recvInt();
		for (int i = 0; i < snakesNum; i++) {
			int length = recvInt();
			int snakeSize = recvInt();
			int direct = recvInt();
			vector<SnakeBody> body = vector<SnakeBody>();
			for (int j = 0; j < snakeSize; j++) {
				int bodyDirect1 = recvInt();
				int bodyDirect2 = recvInt();
				float posX = recvFloat();
				float posY = recvFloat();
				body.push_back(SnakeBody(bodyDirect1, bodyDirect2, { posX, posY }));
			}
			pckg->snakes.push_back(Snake(length, direct, body));
		}
		int antidotesNum = recvInt();
		for (int i = 0; i < antidotesNum; i++) {
			float posX = recvFloat();
			float posY = recvFloat();
			pckg->antidotes.push_back({ posX, posY });
		}
	}
	else {
		int hittedSnakesLen = recvInt();
		for (int i = 0; i < hittedSnakesLen; i++) {
			int snakeHits = recvInt();
			for (int j = 0; j < snakeHits; j++) {
				snakes[i].hitSnake();
			}
		}
		int usedAntidotesLen = recvInt();
		for (int i = 0; i < usedAntidotesLen; i++) {
			float posX = recvFloat();
			float posY = recvFloat();
			usedAntidotes.push_back({ posX, posY });
		}
	}
	return pckg;
}

