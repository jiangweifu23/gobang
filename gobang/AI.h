#pragma once
#include "Chess.h"
class AI
{
public:
	void init(Chess* chess);
	void go();
private:
	Chess* chess;
	//���ڴ������ÿ�����Ȩ��
	vector<vector<int>> scoreMap;

	//��������,�������ӵ�λ��
	void calculateScore();
	//AI����˼��
	chessPos think();
};

