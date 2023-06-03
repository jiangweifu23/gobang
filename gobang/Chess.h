#pragma once
#include <graphics.h>//easyxͼ�ο��ͷ�ļ�
#include <vector>
using namespace std;

//����λ��
struct chessPos{
	int row;
	int col;

	chessPos(int r=0, int c=0):row(r),col(c){}
};
//���ӻ��ǰ���
typedef enum{
	CHESS_WHITE = -1,
	CHESS_BLACK = 1
}chess_kind_t;

class Chess
{
public:
	Chess(int gradeSize, int marginX, int marginY, float chessSize);
	void init();//��ʼ��
	int getGradeSize();//��ȡ���̴�С
	void chessDown(chessPos* pos, chess_kind_t kind);//����
	bool clickBord(int x, int y, chessPos* pos);//���ӵ�λ���Ƿ�Ϸ�
	int getChessData(chessPos* pos);//��ȡָ��λ�õ����ӵ���ɫ
	int getChessData(int row, int col);
	bool checkOver();//�������Ƿ��Ѿ�����

	//ʹ��ͼ�ο�easyx����ʾ���ǵ����̡�����
private:
	IMAGE chessBlackImg;//����
	IMAGE chessWhiteImg;//����

	int gradeSize;//���̵Ĵ�С
	int margin_x;//�������߽����
	int margin_y;//���̵Ķ����ı߽����
	float chessSize;//���ӵĴ�С

	//�洢��ǰ��ֵķֲ����
	vector<vector<int>> chessMap;
	//��ʾ��ǰ˭������
	bool playerFlag;//true:��ʾ������  flase��ʾ������
	//������ֲַ����
	void updataGameMap(chessPos* pos);
	//�ж�����Ƿ��Ѿ�����
	bool checkWin();//true��ʾ�ֳ�ʤ����false��ʾʤ��δ��

	chessPos curPos;//��¼��ǰ���ӵ��λ�ã��Ա������ж�����Ƿ����
};

