#include<stdio.h>
#include<graphics.h>
#include"tetris.h"


int main()
{
	int nfps = 120;	//设置帧率
	initgr();	//初始化全局数据

	Game game;	//定义Game对象
	initGame(&game, 10, 20, 24, 24, nfps / 2, 10);	//游戏初始化处理

	//设置窗口更新模式为手动更新
	setrendermode(RENDER_MANUAL);

	//游戏不断循环并以稳定的帧率运行，直到关闭游戏窗口结束游戏
	for (; is_run(); delay_fps(nfps))
	{
		update(&game);	//逻辑更新主函数
		render(&game);	//图形更新主函数
	}

	delimage(game.m_pcb);	//删除PIMAGE对象
	getch();
	closegraph();	//关闭图形环境
	return 0;
}