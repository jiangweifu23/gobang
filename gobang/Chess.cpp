#include "Chess.h"
#include <cmath>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>

bool dis(int x1, int y1, int x2, int y2, int offset) {
    double dis = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    if (dis < offset) {
        return true;
    }
    else {
        return false;
    }
}

void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
    // ������ʼ��
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;    //���Դ������صĽǱ�

    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
            }
        }
    }
}

Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
    //�������̵Ĳ���
    this->gradeSize = gradeSize;
    this->margin_x = marginX;
    this->margin_y = marginY;
    this->chessSize = chessSize;
    playerFlag = CHESS_BLACK;//Ĭ�Ϻ�������

    //��ʼ������״̬
    for (int i = 0; i < gradeSize; i++) {
        vector<int> row;
        for (int j = 0; j < gradeSize; j++) {
            row.push_back(0);
        }
        chessMap.push_back(row);
    }
}

void Chess::init()
{
    //������Ϸ����
    initgraph(651, 650);//EW_SHOWCONSOLE

    //��ʾ����ͼƬ
    loadimage(0, "res/����2-1.jpg");
    //���ſ�ʼ��ʾ��
    mciSendString("play res/start.wav", 0, 0, 0);

    //���غ���Ͱ������ӵ�ͼƬ
    loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
    loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

    //�������״̬
    for (int i = 0; i < gradeSize; i++) {
        for (int j = 0; j < gradeSize; j++) {
            chessMap[i][j] = 0;
        }
    }

    playerFlag = CHESS_BLACK;
}

int Chess::getGradeSize()
{
    return gradeSize;
}

void Chess::chessDown(chessPos* pos, chess_kind_t kind)
{
    mciSendString("play res/down7.wav", 0, 0, 0);
    //��������ͼ������Ͻ�����
    int x = margin_x + chessSize * pos->col - 0.5 * chessSize;
    int y = margin_y + chessSize * pos->row - 0.5 * chessSize;

    //ʵ������Ч��
    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &chessWhiteImg);
    }
    else {
        putimagePNG(x, y, &chessBlackImg);
    }
    //���Ӻ������ֵ�״̬
    updataGameMap(pos);
}

bool Chess::clickBord(int x, int y, chessPos* pos)
{
    //�������Ͻǵ����ڵ��к���
    int row = (y - margin_y) / chessSize;
    int col = (x - margin_x) / chessSize;
    //printf("���Ͻǵ��к���(%d,%d)", row, col);

    int offset = chessSize * 0.4;//������ֵ
    bool ret = false;

    do 
    {
        //�ж����Ͻǵĵ��Ƿ�����ֵ��Χ��
        int leftTopX = col * chessSize + margin_x;
        int leftTopY = row * chessSize + margin_y;
        if (dis(leftTopX, leftTopY, x, y, offset)) {
            pos->row = row;
            pos->col = col;
            //�����λ��û�����ӣ���˵����������
            if (chessMap[pos->row][pos->col] == 0){
                ret = true;
            }   
            break;
        }

        //�ж����Ͻǵĵ��Ƿ�����ֵ��Χ��
        int rightTopX = leftTopX + chessSize;
        int rightTopY = leftTopY;
        if (dis(rightTopX, rightTopY, x, y, offset)) {
            pos->row = row;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0) {
                ret = true;
            }  
            break;
        }

        //�ж����½ǵĵ��Ƿ�����ֵ��Χ��
        int leftDownX = leftTopX;
        int leftDownY = leftTopY + chessSize;
        if (dis(leftDownX, leftDownY, x, y, offset)) {
            pos->row = row + 1;
            pos->col = col;
            if (chessMap[pos->row][pos->col] == 0) {
                ret = true;
            }
            break;
        }

        //�ж����½ǵĵ��Ƿ�����ֵ��Χ��
        int rightDownX = leftTopX + chessSize;
        int rightDownY = leftTopY + chessSize;
        if (dis(rightDownX, rightDownY, x, y, offset)) {
            pos->row = row + 1;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0) {
                ret = true;
            }
            break;
        }

    } while (0);

    return ret;
}

int Chess::getChessData(chessPos* pos)
{
    return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
    return chessMap[row][col];
}

bool Chess::checkOver()
{
    if (checkWin()) {
        Sleep(000);
        if (playerFlag == false) {//��һ�����ӵ��Ǻڷ������ַ�������ʾ����ʤ��
            mciSendString("play res/����.mp3", 0, 0, 0);
            loadimage(0, "res/ʤ��.jpg",651, 650, true);
        }
        else {
            mciSendString("play res/ʧ��.mp3", 0, 0, 0);
            loadimage(0, "res/ʧ��.jpg", 651, 650, true);
        }
        _getch;//��ͣ 
        return true;
    }
    return false;
}

void Chess::updataGameMap(chessPos* pos)
{    
    curPos = *pos;//��¼���ӵ�
    chessMap[pos->row][pos->col] = playerFlag ? 1 : -1;
    //�ɹ������Ժ���Ҫ�����ڰ׷�����
    playerFlag = !playerFlag;
}

bool Chess::checkWin()
{
    //���ӵ���к���
    int row = curPos.row;
    int col = curPos.col;
    int allNum ;//��¼�ܹ��ж��ٸ�ͬɫ�����ӣ����5��������ֽ���

    //�����������ж�����Ƿ����
    bool direction;//true��ʾ����false��ʾ����
    int count = 0;//����̽����������������ſ��Ա任
    for (int x = -1; x <= 1; x++) {

        allNum = 0;
        direction = false;

        for (int y = -1; y <= 0;) {           

            if (x != 1 && y == 0) {
                y++;
                continue;
            }

            direction = !direction;//������
            if (direction && count == 2) {
                count = 0;
                y++;
                direction = !direction;
                continue;
            }        

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
                if (curRow >= 0 && curRow < gradeSize &&
                    curCol >= 0 && curCol < gradeSize &&
                    chessMap[curRow][curCol] == chessMap[row][col]) {
                    allNum++;//����ͬɫ��������һ
                }
                else {
                    break;
                }               
            }
            if (allNum == 4) {//����
                return true;
            }
            count++;
        }
    }
    return false;
}
