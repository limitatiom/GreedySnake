//
// Created by DELL on 2022/12/7.
//
#include "Snack.h"


int main() {
    InitAll();
    LoadSource();
    const Uint8 *KeyValue = SDL_GetKeyboardState(NULL);
    ThemeUI(KeyValue);
    Quit();
    return 0;
}


void LoadSource() {
    char FileName[40];
    save = fopen("MySave.txt", "r");
    fscanf(save, "%d", &Highest);
    sprintf(HintMySave, "Highest score:\n%d", Highest);
    printf("%s\n", HintMySave);
    fclose(save);
    save = NULL;
    Mix_VolumeMusic(BGMVolume);
    Mix_Volume(-1, SoundVolume);
    LargeFont = TTF_OpenFont("res/font/GenshinDefault.ttf", 60);
    MiddleFont = TTF_OpenFont("res/font/GenshinDefault.ttf", 36);
    SmallFont = TTF_OpenFont("res/font/GenshinDefault.ttf", 24);
    for (int i = 0; i < 5; i++) {
        sprintf_s(FileName, 30, "res/audio/BGM%d.mp3", i);
        BGM[i] = Mix_LoadMUS(FileName);
        sprintf_s(FileName, 30, "res/image/BG%d.png", i);
        SurBG[i] = IMG_Load(FileName);
    }
    sprintf_s(FileName, 30, "res/image/Snake%d.png", 0);
    SurMySnack = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/block%d.png", 0);
    SurBlock = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/food%d.png", 0);
    SurFood = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/Enemy%d.png", 0);
    SurEnemy = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/Fire%d.png", 0);
    SurFire = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/Heart%d.png", 0);
    SurHeart = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/Bullet%d.png", 0);
    SurBullet = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/Godlen%d.png", 0);
    SurGolden = IMG_Load(FileName);
    sprintf_s(FileName, 30, "res/image/Angry%d.png", 0);
    SurAngry = IMG_Load(FileName);
}//加载资源

void PrintBG(char HintName[], int mode, TTF_Font *FontSize, bool isStart) {
    SDL_Texture *bg = SDL_CreateTextureFromSurface(renderer, SurBG[mode]);
    SDL_RenderCopy(renderer, bg, NULL, &RectBG);
    SDL_DestroyTexture(bg);
    PrintHintName(HintName, mode > 3 ? 2 : 4 + mode, FontSize);
    if (isStart)
        PrintHintName(HintMySave, -1, LargeFont);
    SDL_RenderPresent(renderer);
}

int GameUI(const Uint8 *KeyValue) {
    clock_t FStartTime = clock();
    Mix_PlayMusic(BGM[1], -1);
    while (1) {
        if (player.heart <= 0)
            if (GameOver())
                return 0;
        Move();
        CountTime();
        MyAction(GetKey(KeyValue));
        Collide(&block);
        Collide(&food);
        MoveNode(&SHead);
        PrintAnime();
        if (!player.score % 50 && player.score)
            Mix_PlayMusic(BGM[3], 1);
        while (SDL_PollEvent(&MainEvent))
            if (MainEvent.type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return 0;
        ControlFPS(FStartTime);
    }
}


void Quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
};

void ThemeUI(const Uint8 *KeyValue) {
    ResetFontColor();//重置颜色
    while (1) {
        PrintBG(HintTheme, 0, MiddleFont, true);
        CgAngle -= CgAngle > 0 ? 4 : -356;
        ChangeFontColor();//改变颜色
        Mix_PlayMusic(BGM[0], -1);
        while (SDL_WaitEventTimeout(&MainEvent, mSPF)) {
            if (MainEvent.type == SDL_QUIT || KeyValue[SDL_SCANCODE_ESCAPE])
                return;
            if (KeyValue[SDL_SCANCODE_RETURN] || KeyValue[SDL_SCANCODE_KP_ENTER]) {
                printf("Success\n");
                if (!GameUI(KeyValue))
                    return;
            }
        }
    }
}

void InitAll() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);

    TTF_Init();
    Mix_Init(MIX_INIT_MP3);
    window = SDL_CreateWindow("Greedy Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    srand((unsigned int) time(NULL));
}

void Move() {
    if (!CD_Create || CD_Create == INIT_CREATE) {
        CD_Create = INIT_CREATE;
        for (int i = 0; i < 5; i++) {
            block = CreateNode(&block, SizeMy, 0, 0, Block);
            food = CreateNode(&food, SizeMy, 0, 0, Food);
        }
    }
    if (!(player.score % 10) && player.score || !nature_golden) {
        nature_golden = !nature_golden ? NATURE_GOLDEN : nature_golden;
        props = CreateNode(&props,SizeMy,0,0,GoldenBody);
    }
    if (!(player.score % 15) && player.score || !CD_Enemy) {
        CD_Enemy = !CD_Enemy ? INIT_ENEMY : CD_Enemy;
        for(int i = 0;i<5;i++)
            enemy = CreateNode(&enemy,SizeMy,0,0,Enemy);
    }
    if (!CD_Fire) {
        CD_Fire = INIT_FIRE;
        for(int i = 0;i<3;i++) {
            props = CreateNode(&props, SizeMy, 0, 0, Fire);
            props = CreateNode(&props,SizeMy,0,0,Heart);
        }
    }
}

void MyAction(int Input) {
    if (Input & MoveUp && Snake.FRect.y >= 0)
        Snake.FRect.y -= Snake.FSpeed;
    if (Input & MoveDown && Snake.FRect.y <= HEIGHT - Snake.FRect.h)
        Snake.FRect.y += Snake.FSpeed;
    if (Input & MoveLeft && Snake.FRect.x >= 0)
        Snake.FRect.x -= Snake.FSpeed;
    if (Input & MoveRight && Snake.FRect.x <= WIDTH - Snake.FRect.w)
        Snake.FRect.x += Snake.FSpeed;
}//更改坐标
int GetKey(const Uint8 *KeyValue) {
    int input = 0;
    if (KeyValue[SDL_SCANCODE_ESCAPE])
        return 0;
    else {
        if (KeyValue[SDL_SCANCODE_W])
            input |= MoveUp;
        if (KeyValue[SDL_SCANCODE_S])
            input |= MoveDown;
        if (KeyValue[SDL_SCANCODE_A])
            input |= MoveLeft;
        if (KeyValue[SDL_SCANCODE_D])
            input |= MoveRight;
        return input;
    }
}

OP *CreateNode(OP **who, float size, float FSpeed, double Angle, int type) {
    OP *New = (OP *) malloc(sizeof(OP));
    OP *head = *who;
    if (!head)
        goto loops;
    if (head->type == Body) {
        while (head->next)
            head = head->next;
        FindXY(New, head, size, type, true);
    } else {
        loops:
        FindXY(New, head, size, type, false);
        if (!head)
            *who = New;
    }
    return *who;
}

bool isCollideSnake(OP **New, bool mode) {
    OP *item = *New;
    float dis = XYDis(item->FRect.x, Snake.FRect.x, item->FRect.y, Snake.FRect.y);
    if (mode)
        return dis < 0.3 * Snake.FRect.w;
    else
        return dis < 9 * Snake.FRect.w && dis > 7 * Snake.FRect.w;
}

void MoveNode(OP **List) {
    if (!*List)
        return;
    else {
        OP *Now = *List;
        while (Now->next)
            Now = Now->next;
        while (Now->last) {
            Now->FRect = Now->last->FRect;
            Now->FCenter = Now->last->FCenter;
            Now = Now->last;
        }
    }
}

void RemoveNode(OP **List, int nums) {
    OP *Now = *List;
    if (!*List)
        return;
    else {
        for (int i = 0; i < nums; i++) {
            OP *tmp1 = Now->next;
            OP *tmp2 = Now->last;
            free(Now);
            if (!tmp2 && tmp1) {
                *List = tmp1;
                tmp1->last = NULL;
            }
            if (!tmp1 && tmp2)
                tmp2->next = NULL;
            if (!tmp1 && !tmp2);
            if (tmp1 && tmp2) {
                tmp2->next = tmp1;
                tmp1->last = tmp2;
            }
            Now = tmp1;
        }
    }
}

float XYDis(float x1, float x2, float y1, float y2) {
    return sqrtf(powf(fabs(x2 - x1), 2) + powf(fabs(y2 - y1), 2));
}//计算距离

void PrintList(OP **list, SDL_Surface *Surface) {
    for (OP *i = *list; i != NULL; i = i->next) {
        SDL_Texture *TexList = SDL_CreateTextureFromSurface(renderer, Surface);//二级指针
        SDL_RenderCopyExF(renderer, TexList, NULL, &i->FRect, i->Angle, &i->FCenter, 0);
        SDL_DestroyTexture(TexList);
    }
}

void ChangeFontColor() {
    FontCgColor.a += (int) (9 * sin(CgAngle * D2R));
}

void ResetFontColor() {
    CgAngle = 360;
    FontCgColor.a = 0xFF;
}//重置颜色

void PrintHintName(char *name, int mode, TTF_Font *FontSize) {
    SDL_Surface *SurHint = TTF_RenderUTF8_Blended(FontSize, name, FontCgColor);//大小 文本 颜色
    SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, SurHint);
    SDL_Rect RectHint = {(WIDTH - SurHint->w) / 2, HEIGHT / 2 + (SurHint->h + 5) * mode, SurHint->w, SurHint->h};
    SDL_RenderCopy(renderer, Texture, NULL, &RectHint);//画笔 纹理 源大小 目标大小
    SDL_FreeSurface(SurHint);
    SDL_DestroyTexture(Texture);
}

void PrintGameBG() {
    SDL_Texture *GameBg = SDL_CreateTextureFromSurface(renderer, SurBG[1]);
    SDL_RenderCopy(renderer, GameBg, NULL, &RectGameBG);
    SDL_DestroyTexture(GameBg);
}

void PrintAnime() {
    SDL_RenderClear(renderer);
    PrintGameBG();
    PrintInfo();
    PrintList(&SHead, startAngry ? SurAngry : SurMySnack);
    PrintList(&block, SurBlock);
    PrintList(&food, SurFood);
    SDL_RenderPresent(renderer);
}

void PrintInfo() {
    char PlayScore[30];
    sprintf_s(PlayScore, 30, "Heart:%d\nScore:%d", player.heart, player.score);
    SDL_Surface *SurScore = TTF_RenderUTF8_Blended(SmallFont, PlayScore, FontColor);
    SDL_Texture *Score = SDL_CreateTextureFromSurface(renderer, SurScore);
    SDL_Rect RectScore = {10, HEIGHT - 15 - 27 - SurScore->h, SurScore->w, SurScore->h};
    SDL_RenderCopy(renderer, Score, NULL, &RectScore);
    SDL_FreeSurface(SurScore);
    SDL_DestroyTexture(Score);
}

void Collide(OP **list) {
    OP *head = *list;
    if (!head)
        return;
    while (head) {
        if (isCollideSnake(&head, true)) {
            if (head->type == Block || head->type == Enemy) {
                player.heart -= 1;
                return;
            } else if (head->type == Food) {
                player.score += 1;
                CreateNode(&SHead, SizeMy / 2, Snake.FSpeed, Snake.Angle, Body);
                RemoveNode(&head, 1);
            } else if (head->type == Heart) {
                player.heart += 1;
                RemoveNode(&head, 1);
            } else if (head->type == GoldenBody) {

            } else if (head->type == Bullet) {

            } else;
        }
        head = head->next;
    }
}

void FindXY(OP *New, OP *head, float size, int type, bool mode) {
    if (!head) {
        head = New;
        New->FCenter.x = (float) SizeMy / 2;
        New->FCenter.y = (float) SizeMy / 2;
        New->last = NULL;
        New->next = NULL;
        New->FSpeed = 0;
        New->Angle = 180;
        New->type = type;
    } else {
        while (head->next)
            head = head->next;
        head->next = New;
        New->last = head;
        New->next = NULL;
        New->FCenter = head->FCenter;
        New->FSpeed = head->FSpeed;
        New->Angle = head->Angle;
        New->type = head->type;
    }
    if (mode) {
        New->FRect = head->FRect;
    } else {
        New->FRect.x = 0;
        New->FRect.y = 0;
        while (isCollideSnake(&New, false)) {
            New->FRect.x = rand() % WIDTH + 1;
            New->FRect.y = rand() % HEIGHT + 1;
        }
        New->FRect.w = size / 2;
        New->FRect.h = size / 2;
    }
}

int GameOver() {
    Mix_HaltMusic();
    Mix_PlayMusic(BGM[2], 1);
    ResetFontColor();
    int CDDeath = 60;
    if (player.score > Highest)
        Highest = player.score;
    save = fopen("MySave.txt", "w");
    fprintf(save, "%d", Highest);
    fclose(save);
    while (CDDeath) {
        SDL_Delay(mSPF);
        CDDeath--;
    }
    while (1) {
        PrintBG(HintLose, 3, MiddleFont, false);
        int MsgReturn = MsgAction();
        if (MsgReturn) {
            if (MsgReturn == 2)
                return 1;
            return 0;
        }
    }
}

int MsgAction() {
    CgAngle -= CgAngle > 0 ? 4 : -356;
    ChangeFontColor();
    while (SDL_WaitEventTimeout(&MainEvent, mSPF)) {
        if (MainEvent.type == SDL_QUIT || MainEvent.key.keysym.sym == SDLK_ESCAPE)
            return 2;
        if (MainEvent.key.keysym.sym == SDLK_RETURN || MainEvent.key.keysym.sym == SDLK_KP_ENTER) {
            Mix_HaltMusic();
            return 1;
        }
    }
    return 0;
}

void ControlFPS(clock_t FStartTime) {
    FDurTime = clock() - FStartTime;
    if (FDurTime < mSPF)
        SDL_Delay(mSPF - FDurTime);
    else
        SDL_Delay(mSPF);
    FDurTime = clock() - FStartTime;
}

void CountTime() {
    CD_Enemy -= CD_Enemy > 0 ? 1 : 0;
    CD_Create -= CD_Create > 0 ? 1 : 0;
    CD_Fire -= CD_Fire > 0 ? 1 : 0;
    nature_golden -= nature_golden > 0 ? 1 : 0;
    if (startAngry)
        CD_GoldenBody -= CD_GoldenBody > 0 ? 1 : 0;
}