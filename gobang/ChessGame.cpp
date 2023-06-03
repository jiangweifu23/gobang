#include "ChessGame.h"

ChessGame::ChessGame(Man* man, Chess* chess, AI* ai)
{
	this->man = man;
	this->chess = chess;
	this->ai = ai;

	man->init(chess);
	ai->init(chess);
}

void ChessGame::play()
{
	chess->init();
	while (1) {
		//人先落子
		man->go();
		if (chess->checkOver()) {
			Sleep(3000);
			chess->init();
			continue;
		}

		//AI落子
		ai->go();
		if (chess->checkOver()) {
			Sleep(3000);
			chess->init();
			continue;
		}
	}
}
