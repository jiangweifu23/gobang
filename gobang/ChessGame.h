#pragma once
#include"AI.h"
#include"Chess.h"
#include"Man.h"
class ChessGame
{
public:
	ChessGame(Man* man, Chess* chess, AI* ai);
	void play();
private:
	Man* man;
	Chess* chess;
	AI* ai;
};

