#include <iostream>
#include "ChessGame.h"

int main(void) {
	Man man;
	Chess chess(13, 33, 32, 48.83);//��ʼ������
	AI ai;
	ChessGame game(&man, &chess, &ai);
	game.play();
	return 0;
}