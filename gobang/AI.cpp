#include "AI.h"

void AI::init(Chess* chess)
{
	this->chess = chess;
	
	int size = chess->getGradeSize();
	//ע�⣬��δ�����ڴ�ռ䣬���Բ�����scoreMap[i][j] = 0 ���г�ʼ����������ڴ�Խ��
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
	Sleep(1000);//AI��װ��˼������������
	//����
	chess->chessDown(&pos, CHESS_WHITE);
}

void AI::calculateScore()
{
	int personNum;//���ַ������壩�Ѿ��ж��ٸ�����������
	int aiNum;//AI�������壩�Ѿ��ж��ٸ�����������
	int emptyNum;//�÷����ϣ�û���������ӷ����Ŀհ�λ��

	//aiÿ�����Ӷ�Ҫ���¶���ֽ�����������Ҫ�����������
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap[0].size(); j++) {
			scoreMap[i][j] = 0;
		}
	}

	//����ֽ�������
	int size = chess->getGradeSize();
	bool direction;//true��ʾ����false��ʾ����
	int count = 0;//��ʾ����������������������ܽ����ж���������
	//��ÿ������м���
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col)) continue;//��ǰλ�ò������ӣ��������������ӵ�λ��						

			//��4������(������)�ֱ�����ӵ��������			
			for (int x = -1; x <= 1; x++) {

				direction = false;
				personNum = 0;//���ַ������壩�Ѿ��ж��ٸ�����������
				aiNum = 0;//AI�������壩�Ѿ��ж��ٸ�����������
				emptyNum = 0;//�÷����ϣ�û���������ӷ����Ŀհ�λ��

				for (int y = -1; y <= 0;) {
					if (x != 1 && y == 0) {
						y++;
						continue;
					}

					direction = !direction;//������
					if (direction && count == 2) {
						count = 0;
						y++;
						direction = !direction;//����
						continue;
					}
																					
					//����������ڸ�λ�ã��ṹ�����������					
					for (int i = 1; i <= 4; i++) {
						int curRow;
						int curCol;
						if (direction) {//����
							curRow = row + i * y;
							curCol = col + i * x;
						}
						else {//����
							curRow = row - i * y;
							curCol = col - i * x;
						}

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							personNum++;//��λ���Ǻ���
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;//��λ��Ϊ�հ�λ
							break;
						}
						else {
							break; //��λ�ó������̷�Χ�����߸�λ���ǰ���
						}
					}					

					//�������ӵ÷��������scoreMap
					if (personNum == 0)
					{
						scoreMap[row][col] += 5;
					}
					else if (personNum == 1) {//��2
						scoreMap[row][col] += 10;
					}
					else if (personNum == 2) {
						if (emptyNum == 1) {//��3
							scoreMap[row][col] += 30;
						}
						else if (emptyNum == 2) {//����
							scoreMap[row][col] += 40;
						}
					}
					else if (personNum == 3) {
						if (emptyNum == 1) {//��4
							scoreMap[row][col] += 50;
						}
						else if (emptyNum == 2) {//��4
							scoreMap[row][col] += 200;
						}
					}
					else if (personNum == 4) {//��5
						scoreMap[row][col] += 20000;
					}

					//�������ڸ�λ�ã��ṹ�����������
					emptyNum = 0;
					for (int i = 1; i <= 4; i++) {
						int curRow;
						int curCol;
						if (direction) {//����
							curRow = row + i * y;
							curCol = col + i * x;
						}
						else {//����
							curRow = row - i * y;
							curCol = col - i * x;
						}

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aiNum++;//��λ���ǰ���
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;//��λ��Ϊ�հ�λ
							break;
						}
						else {
							break; //��λ���Ǻ���,���߸�λ�ó������̵ķ�Χ
						}
					}
					

					//�������ӵ÷��������scoreMap
					if (aiNum == 0) {
						scoreMap[row][col] += 5;
					}
					else if (aiNum == 1) {//��2
						scoreMap[row][col] += 10;
					}
					else if (aiNum == 2) {
						if (emptyNum == 1) {//��3
							scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2) {//��3
							scoreMap[row][col] += 60;
						}
					}
					else if (aiNum == 3) {
						if (emptyNum == 1) {//��4
							scoreMap[row][col] += 55;
						}
						else if (emptyNum == 2) {//��4
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
	//����AI����ֽ�������
	calculateScore();

	vector<chessPos> maxPoints;//�������������ֵĵ�
	int maxScore = 0;
	int size = chess->getGradeSize();
	//Ȼ��ѡ��������ߵĵ��������
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (chess->getChessData(row, col) == 0) {//��ǰλ���ǿյģ����ܽ������Ӳ���
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
	int index = rand() % maxPoints.size();//���ѡȡһ�����м�ֵ�ĵ��������
	return maxPoints[index];
}
