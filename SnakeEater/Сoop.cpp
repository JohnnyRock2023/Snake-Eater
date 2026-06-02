#include "Header.h"

int clientSock;
int playerID;

map<int, Vector2f> players = map<int, Vector2f>();

queue<Package*> *sendPackageQueue = new queue<Package*>();
queue<Package*> *recvPackageQueue = new queue<Package*>();

void createSendPackage() {
	Package* pckg = new Package(game_status, score, bestScore, playerID, playerPosX, playerPosY, snakes, antidotes);
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


void createSock() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	inet_pton(AF_INET, "192.168.1.101", &serverAddress.sin_addr);

	connect(clientSock, (const sockaddr*)&serverAddress, sizeof(serverAddress));

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

void createServer() {
	createSock();
	sendObjects();
	sendFloat(playerPosX);
	sendFloat(playerPosY);
	playerID = recvInt();
	createSendPackage();
	sendToServer(getSendPackage());
}

void connectToServer() {
	createSock();
	sendFloat(playerPosX);
	sendFloat(playerPosY);
	playerID = recvInt();
	recvObjects();
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
	sendFloat(pckg->playerPosX);
	sendFloat(pckg->playerPosY);

	if (playerID == 0) {
		sendInt(snakes.size());
		for (Snake snake : pckg->snakes) {
			sendInt(snake.getSize());
			sendInt(snake.getDirect());
			for (SnakeBody bodyPart : snake.getBody()) {
				sendInt(bodyPart.bodyDirect);
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
		float playersPosX = recvFloat();
		float playersPosY = recvFloat();
		pckg->players.insert({ id, {playersPosX, playersPosY} });
	}
	if (playerID != 0) {
		int snakesNum = recvInt();
		for (int i = 0; i < snakesNum; i++) {
			int snakeSize = recvInt();
			int direct = recvInt();
			vector<SnakeBody> body = vector<SnakeBody>();
			for (int j = 0; j < snakeSize; j++) {
				int bodyDirect = recvInt();
				float posX = recvFloat();
				float posY = recvFloat();
				body.push_back(SnakeBody(bodyDirect, { posX, posY }));
			}
			pckg->snakes.push_back(Snake(snakeSize, direct, body));
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

