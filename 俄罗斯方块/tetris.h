#ifndef _TETRIS_H
#define _TETRIS_H
#include<graphics.h>

#define VK_F2      0x71 //F2键
#define VK_LEFT    0x25 //方向左键
#define VK_RIGHT   0x27 //方向右键
#define VK_DOWN	   0x28 //方向下键
#define VK_UP	   0x26 //方向上键
#define VK_NUMPAD0 0x60 //小键盘0键
#define VK_SPACE   0x20 //空格键

//状态表
enum
{
	ST_START,  //游戏重新开始
	ST_NEXT,   //准备下一个方块
	ST_NORMAL, //玩家控制阶段
	ST_OVER    //游戏结束，F2重新开始
};

typedef struct TetrisGame
{
	int m_gamepool_w, m_gamepool_h;	//游戏地图宽和高
	int m_gamepool[30][30];	//从1为起始下标，下标0用于边界碰撞检测

	/**********************/
	/*  被控制方块的属性  */
	/**********************/

	int m_ctl_x, m_ctl_y;
	int m_ctl_t;	//第n种状态中某种变形方块
	int m_ctl_s;	//7种状态中的一种
	float m_ctl_dx, m_ctl_dy;	//方块在x,y方向
	int m_next_s;	//下一个方块
	int m_forbid_down;	//方块是否可以下落标识
	int m_colormap[10];	//方块的10种颜色
	int score=0;
	
	PIMAGE m_pcb;	//定义名为m_pcb的PIMAGE对象
}Game;

//初始化全局数据函数
void initgr();
//游戏初始化函数
void initGame(Game *obj, int w, int h, int bw, int bh, int droptime, int movetime);
//逻辑更新函数
void update(Game *obj);
//状态转换处理函数
int deal(Game *obj);
//碰撞检测函数
bool isCrash(Game *obj);
//图型更新函数
void render(Game *obj);
//合并，消行处理函数
void merge(Game *obj);
//在方块外面画边框函数,起到美化作用
void drawedge(int x, int y, int w, int h);
//画方块函数
void drawtile(int x, int y, int w, int h, int color);
//画图形边框函数
void drawframe(int x, int y, int w, int h, int d = 0);
//画控制方块函数
void draw44(Game *obj, int bx, int by, int mat[4][4], float dx = 0, float dy = 0);

#endif
