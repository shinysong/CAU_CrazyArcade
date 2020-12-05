#include <iostream>
#include <bangtal>
#include <ctime>
#include <Windows.h>
#include "Board.h"

#define EMPTY 0 // �� ĭ
#define BOMB 1 //��ǳ��
#define SPLASH 2 //���ٱ�
#define BLOCK 3 // ���

#define WIDTH 12
#define HEIGHT 12

#define BOMB_TIME 2.5f
#define EXPLODE 3.0f
#define CLEAR 2.0f

#define VERTICAL 0
#define HORIZONTAL 1

#define UP 84
#define DOWN 85
#define LEFT 82
#define RIGHT 83

using namespace std;
using namespace bangtal;

extern ObjectPtr character, monsters[7];
extern BoardPtr board[WIDTH][HEIGHT];
extern TimerPtr boardTimer[WIDTH][HEIGHT], creater, gameTimer, charTimer;
extern SoundPtr background;
extern bool live;
extern int characterX, characterY;

const int b_x1 = 37; //�����ϼ� �ִ� ����
const int b_x2 = 906;
const int b_y1 = 58;
const int b_y2 = 619;

class Mons {
public:
    int x;
    int y;
    int speed;
    bool isshow;
    bool islive;
    Mons(int x, int y, int s, bool isshow) :x(x), y(y), speed(s), isshow(true), islive(true) {}

public:
    Mons() { // �ʱ�ȭ
        x = b_x1;
        y = b_y1;
        speed = 10;
        isshow = true;
        islive = true;
    }
};

Mons mob[5];
TimerPtr monsterTimer[5];
int monsterInt[7] = { 0, 45, 24, 70, 15, 62, 32 }; //���� ��ġ


extern void setStatus(int x, int y, int status, int way = 0);
extern void setEnd(int x, int y, int* d);
extern bool check(int x, int y);
extern bool board_check(int x, int y);
extern bool heart_touch(int mx, int my, int bx, int by, int radius);
extern void lobby_main();

ScenePtr threeScene;

bool blockCheck(int x, int y) {
    int k, w;

    int monDiff[4][2] = {
    {10, 12},
    {10, 44},
    {42, 12},
    {42, 44}
    };

    for (auto d : monDiff) {
        k = (y + d[1] - 58) / 51;
        w = (x + d[0] - 37) / 79;

        if ((board[k][w]->getStatus() == BLOCK) || ( y > 510 && y < 550 && x > 510 && x < 540)) {
            return true;
        }
    }

    return false;

}

void three_main() {
    srand((unsigned int)time(NULL));
	live = true;
	threeScene = Scene::create("", "Images/3ź.png");
	background = Sound::create("music/3.mp3");

	background->play();

    characterX = 510;
    characterY = 510;
    character = Object::create("Images/�����.png", threeScene, characterX, characterY);

    //����ź ������ Ÿ�̸� //�� �ʸ��� ��ǳ�� ��Ÿ����
    creater = Timer::create(BOMB_TIME);
    gameTimer = Timer::create(60.0f);
    charTimer = Timer::create(0.1f);

    for (int i = 0;i < WIDTH;i++) {
        for (int j = 0; j < HEIGHT;j++) {
            //���� �ʱ�ȭ. Ÿ�̸� �ʱ�ȭ �� Ÿ�̸� �ݹ��Լ� ����
            board[i][j] = Board::create("Images/blank.png", threeScene, 37 + 79 * j, 58 + 51 * i, true, EMPTY);
            boardTimer[i][j] = Timer::create(EXPLODE);
            boardTimer[i][j]->setOnTimerCallback([&, i, j](TimerPtr)-> bool {

                if (!live) return true;
                int charX = characterX;
                int charY = characterY;
                int size = board[i][j]->getSize();
                int status = board[i][j]->getStatus();

                int diff[4][2] = {
                    {1, 0},
                    {0 ,1},
                    {-1, 0},
                    {0, -1}
                };

                if (status == BOMB) {
                    auto sound = Sound::create("music/wave.mp3");
                    for (auto d : diff) {
                        int x = i;
                        int y = j;
                        for (int k = 0;k < size;k++) {
                            if (x >= WIDTH || x < 0 || y >= WIDTH || y < 0) {
                                x -= d[0];
                                y -= d[1];
                                if (x == i && y == j) setStatus(x, y, SPLASH);
                                else setEnd(x, y, d);
                                break;
                            }
                            if (board[x][y]->getStatus() == BLOCK || board[x][y]->getStatus() == BOMB) {
                                if (x != i || y != j) {
                                    x -= d[0];
                                    y -= d[1];
                                    if (x == i && y == j) setStatus(x, y, SPLASH);
                                    else setEnd(x, y, d);
                                    break;
                                }
                            }

                            if (d[0] == 0) setStatus(x, y, SPLASH, HORIZONTAL);
                            else setStatus(x, y, SPLASH, VERTICAL);

                            if (k == size - 1) {
                                setEnd(x, y, d);
                            }

                            x += d[0];
                            y += d[1];
                        }

                    }

                    boardTimer[i][j]->set(CLEAR);
                    boardTimer[i][j]->start();
                    sound->play();
                }
                else {
                    for (auto d : diff) {
                        int x = i;
                        int y = j;
                        for (int k = 0;k < size;k++) {
                            if (x >= WIDTH || x < 0 || y >= WIDTH || y < 0) break;
                            if (board[x][y]->getStatus() == BLOCK || board[x][y]->getStatus() == BOMB) break;

                            setStatus(x, y, EMPTY);
                            x += d[0];
                            y += d[1];
                        }
                    }

                }

                check(charX, charY);



                return true;
                });
        }
    }

 

    int boxes[16][2] = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1},
        {10, 10}, {10, 11}, {11, 10}, {11, 11},
        {10, 1}, {11, 1}, {10, 0}, {11, 0},
        {0, 10}, {0, 11}, {1, 10}, {1, 11}
    };

    for (auto b : boxes) {
        board[b[0]][b[1]]->setStatus(BLOCK);
        board[b[0]][b[1]]->setImage("Images/����3.png");
    }

    setStatus(6, 6, BLOCK);
    setStatus(6, 7, BLOCK);
    setStatus(7, 6, BLOCK);
    setStatus(7, 7, BLOCK);
    board[6][6]->setImage("Images/���3.png");
    board[6][7]->setImage("Images/���3.png");
    board[7][6]->setImage("Images/���3.png");
    board[7][7]->setImage("Images/���3.png");

    bool block = false;

    int mons[5][2] = {
        {100, 300},
        {200, 500},
        {700, 250},
        {300, 85},
        {600, 200}
    };
    for (int i = 0;i < 5;i++) {
         
            mob[i].x = mons[i][0];
            mob[i].y = mons[i][1];
            mob[i].speed = 10 + (rand() % 10);
            monsters[i] = Object::create("images/����.png", threeScene, mob[i].x, mob[i].y);
            monsterTimer[i] = Timer::create(0.1f);
            
  
    }

    for (int i = 0; i < 5; i++) {
        monsterTimer[i]->setOnTimerCallback([&, i](TimerPtr t)->bool {
            if (!live) return true;

            int move = monsterInt[i];
            Mons m = mob[i];
            int x = mob[i].x;
            int y = mob[i].y;
            int speed = mob[i].speed;
            ObjectPtr monster = monsters[i];
            
            if (move <= 20) { // ��
                y += speed;
                if (board_check(x, y) == true && !blockCheck(x,y)) {

                    mob[i].y = y;
                    monster->locate(threeScene, mob[i].x, mob[i].y);

                    if (heart_touch(x, y, characterX, characterY, 35)) {

                        background->stop();
                        auto lose = Sound::create("Music/Lose.mp3");
                        showMessage("FAIL!\n�ٽ� �����ϼ���.");
                        lose->play();
                        live = false;

                    }

                }

            }
            else if (move <= 40) { // �Ʒ�
                y -= speed;
                if (board_check(x, y) == true && !blockCheck(x, y)) {
                    mob[i].y = y;
                    monster->locate(threeScene, mob[i].x, mob[i].y);

                    if (heart_touch(x, y, characterX, characterY, 35)) {

                        background->stop();
                        auto lose = Sound::create("Music/Lose.mp3");
                        showMessage("FAIL!\n�ٽ� �����ϼ���.");
                        lose->play();
                        live = false;

                    }

                }
                
            }
            else if (move <= 60) { // ����
                x += speed;
                if (board_check(x, y) == true && !blockCheck(x, y)) {
                    mob[i].x = x;
                    monster->locate(threeScene, mob[i].x, mob[i].y);

                    if (heart_touch(x, y, characterX, characterY, 35)) {

                        background->stop();
                        auto lose = Sound::create("Music/Lose.mp3");
                        showMessage("FAIL!\n�ٽ� �����ϼ���.");
                        lose->play();
                        live = false;

                    }
                }
            }
            else if ( move <= 80) { //��
                x -= speed;
                if (board_check(x, y) == true && !blockCheck(x, y)) {
                    mob[i].x = x;
                    monster->locate(threeScene, mob[i].x, mob[i].y);

                    if (heart_touch(x, y, characterX, characterY, 35)) {

                        background->stop();
                        auto lose = Sound::create("Music/Lose.mp3");
                        showMessage("FAIL!\n�ٽ� �����ϼ���.");
                        lose->play();
                        live = false;

                    }

                }
            }
            
            for (int j = 0;j < 5;j++) monsterInt[j] = rand() % 80;
            monsterTimer[i]->set(0.1f);
            monsterTimer[i]->start();
            return true;
        });

    }


    threeScene->setOnKeyboardCallback([&](ScenePtr, int key, bool) -> bool {
        //ĳ���� ������ �� ���ٱ� Ȯ��

        if (!live) return true;

        int prevX = characterX;
        int prevY = characterY;
        string image;

        switch (key) {
        case UP: characterY += 10; image = "Images/�����.png"; break;
        case DOWN: characterY -= 10; image = "Images/�����.png"; break;
        case LEFT: characterX -= 10; image = "Images/�����.png"; break;
        case RIGHT: characterX += 10; image = "Images/�����.png"; break;
        default: return true;
        }

        if (check(characterX, characterY)) {
            character->locate(threeScene, characterX, characterY);
            character->setImage(image);
        }
        else {
            characterX = prevX;
            characterY = prevY;
        }
        return true;
        });

    creater->setOnTimerCallback([&](TimerPtr) -> bool {

        //�������� ��ź ���� ��ġ ����
        if (!live) return true;

        int bombX = rand() % WIDTH;
        int bombY = rand() % HEIGHT;
        bool possible = false;
        while (board[bombX][bombY]->getStatus() != EMPTY || !possible) {
            possible = true;
            bombX = rand() % WIDTH;
            bombY = rand() % HEIGHT;
            int checker[4][2] = {
                { 7, 7},
                {57, 7},
                { 7,73},
                {57,73}
            };

            for (auto c : checker) {
                int k = (characterY + c[1] - 58) / 51;
                int w = (characterX + c[0] - 37) / 79;

                if (k == bombX && w == bombY) {
                    possible = false;
                }
            }
        }

        int size = (rand() % 9) + 3;

        board[bombX][bombY]->setStatus(BOMB);
        board[bombX][bombY]->setSize(size);
        board[bombX][bombY]->setImage("Images/��ǳ��.png");

        boardTimer[bombX][bombY]->set(EXPLODE);
        //���� �ð����ȸ� ��ǳ�� �� ���ٱ� ����
        boardTimer[bombX][bombY]->start();

        creater->set(BOMB_TIME);
        creater->start();

        return true;
        });

    gameTimer->setOnTimerCallback([&](TimerPtr)-> bool {
        if (!live) return true;

        auto winSound = Sound::create("music/Win.mp3");

        showMessage("CLEAR!\n����� ũ���� ����!");

        winSound->play();
        background->stop();
        gameTimer->stop();
        for (int i = 0;i < WIDTH;i++) {
            for (int j = 0;j < HEIGHT;j++) boardTimer[i][j]->stop();
        }
        creater->stop();
        charTimer->stop();
        hideTimer();
        Sleep(1000);
        lobby_main();

        return true;
        });

    charTimer->setOnTimerCallback([&](TimerPtr)->bool {
        if (live) {
            charTimer->set(0.5f);
            charTimer->start();
            return true;
        }
        Sleep(3000);
        lobby_main();
        return true;

        });
    for (int i =0;i<5;i++) monsterTimer[i]->start();
    creater->start();
    gameTimer->start();
    charTimer->start();
    showTimer(gameTimer);
    enterScene(threeScene->ID());
}