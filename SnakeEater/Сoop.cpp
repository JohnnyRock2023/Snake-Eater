#include "Header.h"

int clientSock;
int playerID;

map<int, Vector2f> players = map<int, Vector2f>();

mutex sendMtx;
mutex recvMtx;
queue<Package*> *sendPackageQueue = new queue<Package*>();
queue<Package*> *recvPackageQueue = new queue<Package*>();

void createSendPackage() { /// For main thread
	sendMtx.lock();
	Package *pckg = new Package(game_status, score, bestScore, playerID, playerPosX, playerPosY, snakes, antidotes);
	sendPackageQueue->push(pckg);
	sendMtx.unlock();
}

Package *getSendPackage() { /// For thread
	Package* pckg = nullptr;
	sendMtx.lock();
	if (!sendPackageQueue->empty()) {
		pckg = sendPackageQueue->front();
		sendPackageQueue->pop();
	}
	sendMtx.unlock();
	return pckg;
}

///////////

void createRecvPackage(Package *pckg) { /// For thread
	recvMtx.lock();
	recvPackageQueue->push(pckg);
	recvMtx.unlock();
}

void getRecvPackage() { /// For main thread
	recvMtx.lock();
	if (!recvPackageQueue->empty()) {
		Package *pckg = recvPackageQueue->front();
		recvPackageQueue->pop();
		game_status = pckg->game_status;
		score = pckg->score;
		bestScore = pckg->bestScore;
		players.clear();
		players = pckg->players;
		playerPosX = pckg->playerPosX;
		playerPosY = pckg->playerPosY;
		snakes.clear();
		snakes = pckg->snakes;
		antidotes.clear();
		antidotes = pckg->antidotes;
		delete pckg;
	}
	recvMtx.unlock();
}

void syncData() {
	Package* pckg = nullptr;
	while (true) {
		pckg = getSendPackage();
		if (pckg != nullptr) {
			sendToServer(pckg);
			delete pckg;
		}
		this_thread::sleep_for(chrono::milliseconds(100));
		pckg = getFromServer();
		if (pckg != nullptr) {
			createRecvPackage(pckg);
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
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

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
	int32_t v;
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
	uint32_t v;
	recvAll(clientSock, &v, sizeof(v));
	v = ntohl(v);
	float f;
	memcpy(&f, &v, sizeof(f));
	return f;
}


void createServer() {
	createSendPackage();
	createSock();
	sendObjects();
	sendFloat(playerPosX);
	sendFloat(playerPosY);
	playerID = recvInt();
	sendToServer(getSendPackage());
}

void connectToServer() {
	createSock();
	sendFloat(playerPosX);
	sendFloat(playerPosY);
	playerID = recvInt();
	recvObjects();
	getFromServer();
	getRecvPackage();
}

void sendObjects() {
	sendInt(objects.size());
	for (Object obj : objects) {
		sendInt(obj.getType());
		sendFloat(obj.getPos().x);
		sendFloat(obj.getPos().y);
	}
}


void sendToServer(Package *pckg) {
	if (pckg == nullptr) return;
	sendInt(pckg->game_status);
	sendInt(pckg->score);
	sendInt(pckg->bestScore);
	sendInt(pckg->playerID);
	sendFloat(pckg->playerPosX);
	sendFloat(pckg->playerPosY);
	sendInt(pckg->snakes.size());
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

void recvObjects() {
	int objCount = recvInt();
	playerID = recvInt();
	objects.clear();
	for (int i = 0; i < objCount; i++) {
		int type = recvInt();
		float posX = recvFloat();
		float posY = recvFloat();
		objects.push_back(Object(type, posX, posY));
	}
}

Package* getFromServer() {
	Package* pckg = new Package();
	recvInt(); 
	pckg->game_status = 1;
	//pckg->game_status = recvInt();
	pckg->score = recvInt();
	pckg->bestScore = recvInt();
	pckg->players.clear();
	int playersNum = recvInt();
	for (int i = 0; i < playersNum; i++) {
		int id = recvInt();
		float playersPosX = recvFloat();
		float playersPosY = recvFloat();
		pckg->players.insert({ id, {playersPosX, playersPosY} });
	}
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
	return pckg;
}
