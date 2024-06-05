# C
24/05/22

大老二，不用防呆


已完成 : 發牌、洗牌，用2個array去儲存手牌


待完成 : 出牌、對弈過程、判斷勝負、放入連線架構


發牌、洗牌、判斷勝負放在SERVER端，出牌兩端都要有

有多餘時間 : 防呆，歡迎畫面，輸入玩家名，遊戲選擇

[撲克牌遊戲本體](localtest/Project1/test.cpp)

[參考網址](https://edisonx.pixnet.net/blog/post/54675090)

[參考網址2](https://edu.hhb01.com/2020/10/c-4.html)有把手牌牌好


IP:172.1842.175

char* recv_str() {
	int iResult;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	char replied[] = "Received!";
	char mayerAnswer[DEFAULT_BUFLEN];
	iResult = recv(theSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		// received
		recvbuf[iResult] = 0;

		// display incoming message received
		printf(" <Mayer> : %s\n", recvbuf);

		// Echo back to the sender
		iSendResult = send(theSocket, replied, 11, 0);
		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(theSocket);
			WSACleanup();
			return NULL;
		}

	}
	else if (iResult == 0)
		printf("Connection closing...\n");
	else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(theSocket);
		WSACleanup();
		return NULL;
	}
	return recvbuf;
}
