//
// Created by DELL on 2022/12/7.
//

#ifndef MAIN_CPP_SNACK_H
#define MAIN_CPP_SNACK_H


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#define WIDTH 700
#define HEIGHT 800
#define BGMVolume 40
#define SoundVolume 40//音量
#define MyInitX 353
#define MyInitY 398
#define SizeMy 72.0
#define D2R 0.01745329252 // 1/R2D
#define INIT_CREATE 100
#define INIT_GOLDEN 100
#define INIT_ENEMY 600
#define INIT_FIRE 100
#define NATURE_GOLDEN 600
#define R2D 57.29577951
#undef main
#pragma comment

typedef struct OP {
    enum {
        Block = -1,
        Food = 1,
        Body = 2,
        Fire = 3,
        GoldenBody = 4,
        Enemy = 5,
        Heart = 6,
        Bullet = 7,
    }type;
    SDL_FRect FRect;
    SDL_FPoint FCenter;
    float FSpeed;
    double Angle;
    struct OP *next;
    struct OP *last;
} OP;

typedef struct Player {
    int heart;
    size_t score;
} Player;
//创建指针
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Event MainEvent;
TTF_Font *LargeFont = NULL;
TTF_Font *MiddleFont = NULL;
TTF_Font *SmallFont = NULL;
SDL_Color FontCgColor = {0, 0, 0, 0xFF};
SDL_Color FontColor = {0, 0, 0, 0x99};//不会变的
//音乐区
Mix_Music *BGM[30] = {NULL};
//图片区
SDL_Surface *SurMySnack= NULL;//0存放头,后面存放身体
SDL_Surface *SurBlock = NULL;
SDL_Surface *SurFood = NULL;
SDL_Surface *SurFire = NULL;
SDL_Surface *SurGolden = NULL;
SDL_Surface *SurHeart = NULL;
SDL_Surface *SurEnemy = NULL;
SDL_Surface *SurBullet = NULL;
SDL_Surface *SurAngry = NULL;
SDL_Surface *SurBG[30] = {NULL};

SDL_Rect RectBG = {0, 0, WIDTH, HEIGHT};
SDL_Rect RectGameBG = {0, 0, WIDTH, HEIGHT};

const Uint8 *KeyValue;//键盘初始
int eat_cnt = 0;//记录吃东西的次数


enum InputAction {
    MoveUp = 0x0001,
    MoveDown = 0x0002,
    MoveLeft = 0x0004,
    MoveRight = 0x0008,
};
OP Snake = {
        Body,
        {MyInitX,MyInitY,SizeMy,SizeMy},//坐标 + 大小
        {(float)SizeMy/2,(float)SizeMy/2}, //中心点
        15,
        0,
        NULL,
        NULL,
};
OP* SHead = &Snake;

OP *block = NULL;
OP *food = NULL;
OP* bullet = NULL;//存放子弹
OP* enemy = NULL;//存放敌人
OP* props = NULL;
OP* fire = NULL;

int CgAngle;
int CD_GoldenBody = INIT_GOLDEN;
int CD_Enemy = INIT_ENEMY;
int CD_Create = INIT_CREATE;
int CD_Fire = INIT_FIRE;
int nature_golden = NATURE_GOLDEN;
FILE* save = NULL;

Player player = {
        10,
        0,
};

void Quit();

void LoadSource();

void ThemeUI(const Uint8 *KeyValue);

int GameUI(const Uint8 *KeyValue);

void InitAll();

void PrintBG(char Hint[],int mode,TTF_Font* FontSize,bool isStart);

bool isCollideSnake(OP** New,bool mode);

void Move();

void MyAction(int input);

int GetKey(const Uint8 *KeyValue);

OP *CreateNode(OP** who,float size,float FSpeed,
               double Angle,int type);

void MoveNode(OP** List);

void RemoveNode(OP** List,int nums);

float XYDis(float x1,float x2,float y1,float y2);

void PrintList(OP** list, SDL_Surface* Surface);

void ChangeFontColor();

void ResetFontColor();

void PrintHintName(char* HintName,int mode,TTF_Font* FontSize);

void PrintAnime();

void PrintInfo();

void Collide(OP** list);

void FindXY(OP* New,OP* head,float size,int type,bool mode);

void PrintGameBG();

int GameOver();

int MsgAction();

void ControlFPS(clock_t FStartTime);

void CountTime();

void OthersCollide(OP** );

char HintMySave[20];
char HintTheme[] = "Press \"Enter\" to Start!";
char HintLose[] = "You \" die \" !! ";
size_t mSPF = 40;
size_t Highest = 0;
clock_t FDurTime;
bool startAngry = false;//血怒
bool startFire = false;
#endif //MAIN_C_SNACK_H
