#include "AI.h"

void AI::init(Chess* chess)
{
	this->chess = chess;
	
	int size = chess->getGradeSize();
	//注意，尚未分配内存空间，所以不能用scoreMap[i][j] = 0 进行初始化，否则会内存越界
	for (int i = 0; i < size; i++) {
		vector<int> row;
		for (int j = 0; j < size; j++) {
			row.push_back(0);
		}
		scoreMap.push_back(row);
	}
}

void AI::go()
{
	chessPos pos = think();
	Sleep(1000);//AI假装在思考，给点面子
	//落子
	chess->chessDown(&pos, CHESS_WHITE);
}

void AI::calculateScore()
{
	int personNum;//棋手方（黑棋）已经有多少个连续的棋子
	int aiNum;//AI方（白棋）已经有多少个连续的棋子
	int emptyNum;//该方向上，没被对手棋子封锁的空白位数

	//ai每次落子都要重新对棋局进行评估，即要重置棋局评分
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap[0].size(); j++) {
			scoreMap[i][j] = 0;
		}
	}

	//对棋局进行评估
	int size = chess->getGradeSize();
	bool direction;//true表示正向，false表示反向
	int count = 0;//表示必须走完正反两个方向才能接着判断其他方向
	//对每个点进行计算
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col)) continue;//当前位置不能落子，继续查找能落子的位置						

			//从4个方向(四条线)分别对落子点进行评分			
			for (int x = -1; x <= 1; x++) {

				direction = false;
				personNum = 0;//棋手方（黑棋）已经有多少个连续的棋子
				aiNum = 0;//AI方（白棋）已经有多少个连续的棋子
				emptyNum = 0;//该方向上，没被对手棋子封锁的空白位数

				for (int y = -1; y <= 0;) {
					if (x != 1 && y == 0) {
						y++;
						continue;
					}

					direction = !direction;//先正向
					if (direction && count == 2) {
						count = 0;
						y++;
						direction = !direction;//重置
						continue;
					}
																					
					//假设黑棋落在该位置，会构成怎样的棋局					
					for (int i = 1; i <= 4; i++) {
						int curRow;
						int curCol;
						if (direction) {//正向
							curRow = row + i * y;
							curCol = col + i * x;
						}
						else {//反向
							curRow = row - i * y;
							curCol = col - i * x;
						}

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							personNum++;//该位置是黑棋
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;//该位置为空白位
							break;
						}
						else {
							break; //该位置超出棋盘范围，或者该位置是白棋
						}
					}					

					//黑棋落子得分情况存入scoreMap
					if (personNum == 0)
					{
						scoreMap[row][col] += 5;
					}
					else if (personNum == 1) {//连2
						scoreMap[row][col] += 10;
					}
					else if (personNum == 2) {
						if (emptyNum == 1) {//死3
							scoreMap[row][col] += 30;
						}
						else if (emptyNum == 2) {//活三
							scoreMap[row][col] += 40;
						}
					}
					else if (personNum == 3) {
						if (emptyNum == 1) {//死4
							scoreMap[row][col] += 50;
						}
						else if (emptyNum == 2) {//活4
							scoreMap[row][col] += 200;
						}
					}
					else if (personNum == 4) {//连5
						scoreMap[row][col] += 20000;
					}

					//白棋落在该位置，会构成怎样的棋局
					emptyNum = 0;
					for (int i = 1; i <= 4; i++) {
						int curRow;
						int curCol;
						if (direction) {//正向
							curRow = row + i * y;
							curCol = col + i * x;
						}
						else {//反向
							curRow = row - i * y;
							curCol = col - i * x;
						}

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aiNum++;//该位置是白棋
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;//该位置为空白位
							break;
						}
						else {
							break; //该位置是黑棋,或者该位置超出棋盘的范围
						}
					}
					

					//白棋落子得分情况存入scoreMap
					if (aiNum == 0) {
						scoreMap[row][col] += 5;
					}
					else if (aiNum == 1) {//连2
						scoreMap[row][col] += 10;
					}
					else if (aiNum == 2) {
						if (emptyNum == 1) {//死3
							scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2) {//活3
							scoreMap[row][col] += 60;
						}
					}
					else if (aiNum == 3) {
						if (emptyNum == 1) {//死4
							scoreMap[row][col] += 55;
						}
						else if (emptyNum == 2) {//活4
							scoreMap[row][col] += 10000;
						}
					}
					else if (aiNum == 4) {
						scoreMap[row][col] += 30000;
					}
					count++;
				}
			}
		}
	}

}

chessPos AI::think()
{
	//首先AI对棋局进行评分
	calculateScore();

	vector<chessPos> maxPoints;//用来存放最大评分的点
	int maxScore = 0;
	int size = chess->getGradeSize();
	//然后选择评分最高的点进行落子
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col) == 0) {//当前位置是空的，才能进行落子操作
				if (scoreMap[row][col] > maxScore) {
					maxScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(chessPos(row, col));
				}
				else if (scoreMap[row][col] == maxScore) {
					maxPoints.push_back(chessPos(row, col));
				}
			}
		}
	}
	int index = rand() % maxPoints.size();//随机选取一个最有价值的点进行落子
	return maxPoints[index];
}
