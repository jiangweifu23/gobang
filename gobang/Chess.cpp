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

void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
    // 变量初始化
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
    //设置棋盘的参数
    this->gradeSize = gradeSize;
    this->margin_x = marginX;
    this->margin_y = marginY;
    this->chessSize = chessSize;
    playerFlag = CHESS_BLACK;//默认黑棋先走

    //初始化棋盘状态
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
    //创建游戏窗口
    initgraph(651, 650);//EW_SHOWCONSOLE

    //显示棋盘图片
    loadimage(0, "res/棋盘2-1.jpg");
    //播放开始提示音
    mciSendString("play res/start.wav", 0, 0, 0);

    //加载黑棋和白棋棋子的图片
    loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
    loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

    //重置棋局状态
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
    //计算落子图标的左上角坐标
    int x = margin_x + chessSize * pos->col - 0.5 * chessSize;
    int y = margin_y + chessSize * pos->row - 0.5 * chessSize;

    //实现落子效果
    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &chessWhiteImg);
    }
    else {
        putimagePNG(x, y, &chessBlackImg);
    }
    //落子后更新棋局的状态
    updataGameMap(pos);
}

bool Chess::clickBord(int x, int y, chessPos* pos)
{
    //计算左上角点所在的行和列
    int row = (y - margin_y) / chessSize;
    int col = (x - margin_x) / chessSize;
    //printf("左上角的行和列(%d,%d)", row, col);

    int offset = chessSize * 0.4;//设置阈值
    bool ret = false;

    do 
    {
        //判断左上角的点是否在阈值范围内
        int leftTopX = col * chessSize + margin_x;
        int leftTopY = row * chessSize + margin_y;
        if (dis(leftTopX, leftTopY, x, y, offset)) {
            pos->row = row;
            pos->col = col;
            //如果该位置没有落子，则说明可以落子
            if (chessMap[pos->row][pos->col] == 0){
                ret = true;
            }   
            break;
        }

        //判断右上角的点是否在阈值范围内
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

        //判断左下角的点是否在阈值范围内
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

        //判断右下角的点是否在阈值范围内
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
        if (playerFlag == false) {//上一步落子的是黑方（棋手方），表示棋手胜利
            mciSendString("play res/不错.mp3", 0, 0, 0);
            loadimage(0, "res/胜利.jpg",651, 650, true);
        }
        else {
            mciSendString("play res/失败.mp3", 0, 0, 0);
            loadimage(0, "res/失败.jpg", 651, 650, true);
        }
        _getch;//暂停 
        return true;
    }
    return false;
}

void Chess::updataGameMap(chessPos* pos)
{    
    curPos = *pos;//记录落子点
    chessMap[pos->row][pos->col] = playerFlag ? 1 : -1;
    //成功落子以后，需要交换黑白方下棋
    playerFlag = !playerFlag;
}

bool Chess::checkWin()
{
    //落子点的行和列
    int row = curPos.row;
    int col = curPos.col;
    int allNum ;//记录总共有多少个同色的棋子，最多5个代表棋局结束

    //从四条线上判断棋局是否结束
    bool direction;//true表示正向，false表示反向
    int count = 0;//必须探索完正反两个方向才可以变换
    for (int x = -1; x <= 1; x++) {

        allNum = 0;
        direction = false;

        for (int y = -1; y <= 0;) {           

            if (x != 1 && y == 0) {
                y++;
                continue;
            }

            direction = !direction;//先正向
            if (direction && count == 2) {
                count = 0;
                y++;
                direction = !direction;
                continue;
            }        

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
                if (curRow >= 0 && curRow < gradeSize &&
                    curCol >= 0 && curCol < gradeSize &&
                    chessMap[curRow][curCol] == chessMap[row][col]) {
                    allNum++;//连续同色棋子数加一
                }
                else {
                    break;
                }               
            }
            if (allNum == 4) {//连五
                return true;
            }
            count++;
        }
    }
    return false;
}
