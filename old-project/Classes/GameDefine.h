#ifndef  _Inkmoo_Elimination_h_
#define  _Inkmoo_Elimination_h_

// 定义屏幕宽高，这与所做的图片有关
#define GAME_SCREEN_WIDTH 600
#define GAME_SCREEN_HEIGHT 800

// 定义每个精灵大小与边框大小
#define SPRITE_WIDTH 48
#define BOADER_WIDTH 2

// 游戏精灵行数和列数
#define ROWS 8
#define COLS 8

// 精灵种类总数
#define TOTAL_SPRITE 6

// 普通的精灵
static const char *spriteNormal[TOTAL_SPRITE] = {
	"icon1.png",
	"icon2.png",
	"icon3.png",
	"icon4.png",
	"icon5.png",
	"icon6.png"
};
//英雄HP
#define HERO_HP 5

#define TIME 180

#endif