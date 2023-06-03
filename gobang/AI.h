#pragma once
#include "Chess.h"
class AI
{
public:
	void init(Chess* chess);
	void go();
private:
	Chess* chess;
	//用于存放棋盘每个点的权重
	vector<vector<int>> scoreMap;

	//计算评分,返回落子的位置
	void calculateScore();
	//AI进行思考
	chessPos think();
};

