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
		//��ȡ�������Ϣ
		msg = GetMouseMsg();

		//ͨ��chess����������clickBoard�������ж������Ƿ���Ч���Լ����ӵĹ���
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBord(msg.x, msg.y, &pos)) {
			break;
		}
	}
	//printf("�����������(%d,%d)\n", msg.x,msg.y);
	//printf("���ӵ��к���(%d,%d)\n", pos.row, pos.col);
	//����
	chess->chessDown(&pos, CHESS_BLACK);
}
