#pragma once
#include <graphics.h>//easyx图形库的头文件
#include <vector>
using namespace std;

//落子位置
struct chessPos{
	int row;
	int col;

	chessPos(int r=0, int c=0):row(r),col(c){}
};
//黑子还是白子
typedef enum{
	CHESS_WHITE = -1,
	CHESS_BLACK = 1
}chess_kind_t;

class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize);
	void init();//初始化
	int getGradeSize();//获取棋盘大小
	void chessDown(chessPos* pos, chess_kind_t kind);//落子
	bool clickBord(int x, int y, chessPos* pos);//落子的位置是否合法
	int getChessData(chessPos* pos);//获取指定位置的棋子的颜色
	int getChessData(int row, int col);
	bool checkOver();//检查棋局是否已经结束

	//使用图形库easyx来显示我们的棋盘、棋子
private:
	IMAGE chessBlackImg;//黑棋
	IMAGE chessWhiteImg;//白棋

	int gradeSize;//棋盘的大小
	int margin_x;//棋盘左侧边界距离
	int margin_y;//棋盘的顶部的边界距离
	float chessSize;//棋子的大小

	//存储当前棋局的分布情况
	vector<vector<int>> chessMap;
	//表示当前谁该落子
	bool playerFlag;//true:表示黑棋走  flase表示白起走
	//更新棋局分布情况
	void updataGameMap(chessPos* pos);
	//判断棋局是否已经结束
	bool checkWin();//true表示分出胜负，false表示胜负未分

	chessPos curPos;//记录当前落子点的位置，以便用来判断棋局是否结束
};

