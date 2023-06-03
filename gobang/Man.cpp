#include "Man.h"

void Man::init(Chess *chess)
{
	this->chess = chess;
}

void Man::go()
{
	MOUSEMSG msg;
	chessPos pos;

	while (1) {
		//获取鼠标点击信息
		msg = GetMouseMsg();

		//通过chess对像，来调用clickBoard函数来判断落子是否有效，以及落子的功能
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBord(msg.x, msg.y, &pos)) {
			break;
		}
	}
	//printf("鼠标点击的坐标(%d,%d)\n", msg.x,msg.y);
	//printf("落子的行和列(%d,%d)\n", pos.row, pos.col);
	//落子
	chess->chessDown(&pos, CHESS_BLACK);
}
