#include<time.h>
#include<stdio.h>
#include<string.h>
#include<ege.h>
#include"tetris.h"
#include"graphics.h"

const int g_width = 400;	//窗口的宽度
const int g_height = 520;	//窗口的高度

int m_base_x, m_base_y;
int  m_base_w, m_base_h;	//方块的高度和宽度

//相关速度的定义
int m_droptime;	//下落速度
int m_curtime;
int m_cursubtime;
int m_movxtime;	//横向移动速的
int m_curxtime;

//游戏状态和键盘定义
int m_state;	//游戏主状态
int m_Keys[8];	//键盘事件
int m_KeyFlag[8];	//按键标识
int m_KeyState[8];	//键盘状态

//方块图形数组，记录7种形状及其4种旋转状态
static int g_trs_map[8][4][4][4];

//各图形形状的变化数目表
static int g_map_mod[] = { 1, 4, 4, 4, 2, 2, 2, 1, 0 };

/*初始化全局数据*/
void initgr()
{
	//图形窗口初始化，窗口尺寸400*520
	initgraph(g_width, g_height);
	//设置游戏字体
	setfont(12, 6, "楷体");


	//初始化7种形状，4种变化，每种变化4*4矩阵表示
	int Trs_map[8][4][4][4] = {
		{{{0}}},
		{
			{ { 0,0,0,0 },{ 1,1,1,0 },{ 0,1,0,0 },{ 0,0,0,0 } },
			{ { 0,1,0,0 },{ 1,1,0,0 },{ 0,1,0,0 },{ 0,0,0,0 } } ,
			{ { 0,1,0,0 },{ 1,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } } ,
			{ { 0,1,0,0 },{ 0,1,1,0 },{ 0,1,0,0 },{ 0,0,0,0 } } ,
		},
		{
			{ { 2,2,0,0 },{ 0,2,0,0 },{ 0,2,0,0 },{ 0,0,0,0 } },
			{ { 0,0,2,0 },{ 2,2,2,0 },{ 0,0,0,0 },{ 0,0,0,0 } } ,
			{ { 0,2,0,0 },{ 0,2,0,0 },{ 0,2,2,0 },{ 0,0,0,0 } } ,
			{ { 0,0,0,0 },{ 2,2,2,0 },{ 2,0,0,0 },{ 0,0,0,0 } } ,
		},
		{
			{ { 0,3,3,0 },{ 0,3,0,0 },{ 0,3,0,0 },{ 0,0,0,0 } },
			{ { 0,0,0,0 },{ 3,3,3,0 },{ 0,0,3,0 },{ 0,0,0,0 } } ,
			{ { 0,3,0,0 },{ 0,3,0,0 },{ 3,3,0,0 },{ 0,0,0,0 } } ,
			{ { 3,0,0,0 },{ 3,3,3,0 },{ 0,0,0,0 },{ 0,0,0,0 } } ,
		},
		{
			{ { 4,4,0,0 },{ 0,4,4,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
			{ { 0,0,4,0 },{ 0,4,4,0 },{ 0,4,0,0 },{ 0,0,0,0 } } ,
		},
		{
			{ { 0,5,5,0 },{ 5,5,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
			{ { 0,5,0,0 },{ 0,5,5,0 },{ 0,0,5,0 },{ 0,0,0,0 } } ,
		},
		{
			{ { 0,0,0,0 },{ 6,6,6,6 },{ 0,0,0,0 },{ 0,0,0,0 } } ,
			{ { 0,0,6,0 },{ 0,0,6,0 },{ 0,0,6,0 },{ 0,0,6,0 } } ,
		},
		{
			{ { 0,0,0,0 },{ 0,7,7,0 },{ 0,7,7,0 },{ 0,0,0,0 } } ,
		},
	};
	//将数组Trs_map中的数据拷贝到方块图形数组g_trs_map中
	memcpy(g_trs_map, Trs_map, sizeof(Trs_map));
}

/*******************************/
/*obj:游戏Game结构体指针变量*/
/*w.h:游戏画布的宽和高*/
/*bw.bh:单位方块的宽和高*/
/*droptime：方块下落速度*/
/*movetime:方块横向移动速的*/
/*******************************/
//游戏初始化函数
void initGame(Game *obj, int w, int h, int bw, int bh, int droptime, int movetime)
{
	//初始化游戏用到的10种颜色
	int colormap[10] = { 0,0xFF0000,0xFF4500,0xFFD700,0x00FF00,0x00FFFF,0x1874CD,0xFF1493,0x808080,0xFFFFFF };
	memcpy(obj->m_colormap, colormap, sizeof(obj->m_colormap));

	//定义键盘事件
	int Keys[8] = { VK_F2,VK_LEFT,VK_RIGHT,VK_DOWN,VK_UP,VK_NUMPAD0,VK_SPACE };
	memcpy(m_Keys, Keys, sizeof(Keys));
	//初始化键盘状态
	memset(m_KeyState, 0, sizeof(m_KeyState));

	//初始化游戏池的高和宽
	obj->m_gamepool_w = w;	//设为10
	obj->m_gamepool_h = h;	//设为20

	//初始化单位方块的高和宽
	m_base_w = bw;	//宽设为24
	m_base_h = bh;	//高设为24

	//初始化方块图形下落速度和横向移动速度
	m_droptime = droptime;
	m_movxtime = movetime;

	//随机数初始化
	randomize();

	//当前被控方块的属性
	obj->m_ctl_t = -1;

	//创建一个图形对象
	obj->m_pcb = newimage();

	for (int i = 0; i < 10; ++i)
	{
		drawtile(bw*i, 0, bw, bh, colormap[i]);
	}

	//从屏幕获取图像
	getimage(obj->m_pcb, 0, 0, bw * 10, bh);

	//设置游戏主状态未开始状态
	m_state = ST_START;
}

//逻辑更新函数
void update(Game *obj)
{
	key_msg key;
	int k = kbmsg();	//检测键盘消息

	while (k)
	{
		key = getkey();	//获取键盘消息
		for (int i = 0; i < 8; ++i)
		{
			if (key.key == m_Keys[i])
			{
				if (key.msg == key_msg_down)	//键盘按下
				{
					m_KeyFlag[i]++;
					m_KeyState[i] = 1;
				}
				else if (key.msg == key_msg_up)	//键盘弹起
				{
					m_KeyFlag[i] = 0;
					m_KeyState[i] = 0;
					if (i == 3)
						obj->m_forbid_down = 0;
				}
			}
		}
		k = kbmsg();
	}
	while (deal(obj));	//状态转换处理
}

//状态转换处理函数
int deal(Game *obj)
{
	int nRet = 0;
	switch (m_state)
	{
	case ST_START:	//初始化
		int x, y;
		obj->m_next_s = random(7) + 1;

		memset(obj->m_gamepool, 255, sizeof(obj->m_gamepool));
		for (y = 1; y <= obj->m_gamepool_h; ++y)
			for (x = 1; x <= obj->m_gamepool_w; ++x)
			{
				obj->m_gamepool[y][x] = 0;
			}

		obj->m_forbid_down = 0;
		obj->m_ctl_t = -1;
		nRet = 1;
		m_state = ST_NEXT;
		break;
	case ST_NEXT:	//准备下一个方块
		//方块出现的初始位置
		obj->m_ctl_x = (obj->m_gamepool_w - 4) / 2 + 1;
		obj->m_ctl_y = 1;
		obj->m_ctl_t = 0;
		obj->m_ctl_s = obj->m_next_s;
		obj->m_ctl_dy = 0;
		obj->m_next_s = random(7) + 1;
		m_curtime = m_droptime-(int)(obj->score/100);	//方块下落速度
		m_curxtime = 0;
		nRet = 1;
		if (isCrash(obj))
			m_state = ST_OVER;
		else
			m_state = ST_NORMAL;
		break;
	case ST_NORMAL:	//玩家控制阶段
		//处理方块自由落下
		int i, j;
		if (m_KeyState[3] == 0 || obj->m_forbid_down)
		{
			--m_curtime;
			m_cursubtime = 1;
		}
		if (m_curxtime)
			if (m_curxtime < 0)
				m_curxtime++;
			else
				m_curxtime--;

		//处理按键
		for (i = 1, j = 1; i <= 2; ++i, j -= 2)
			//水平上根据按左右方向键的次数重新定位方块
			for (; m_KeyFlag[i] > 0; --m_KeyFlag[i])
			{
				obj->m_ctl_x -= j;	//左键减1，右键加1
				if (isCrash(obj))
					obj->m_ctl_x += j;
				else
					m_curxtime = m_movxtime*j;
			}

		//处理x方向平滑
		obj->m_ctl_dx = float(double(m_curxtime) / m_movxtime);

		//旋转变形
		for (i = 4, j = 1; i <= 5; ++i, j -= 2)
			for (int t; m_KeyFlag[i] > 0; --m_KeyFlag[i])
			{
				obj->m_ctl_t = ((t = obj->m_ctl_t) + g_map_mod[obj->m_ctl_s] + j) % g_map_mod[obj->m_ctl_s];
				if (isCrash(obj))
				{
					obj->m_ctl_t = t;
				}
			}

		if (obj->m_forbid_down == 0 && (m_KeyState[3]))
		{
			m_curtime -= m_cursubtime++;	//按下下键后加速下落
		}

		if (m_curtime < 0)	//超时
		{
			++obj->m_ctl_y;	//下落一行
			if (isCrash(obj))
			{
				--obj->m_ctl_y;
				merge(obj);	//合并消行
				obj->m_ctl_dy = 0;
				obj->m_ctl_dx = 0;
				obj->m_ctl_t = -1;
				if (m_KeyState[3])
				{
					obj->m_forbid_down = 1;	//已经合并，不能再下落
				}
				m_state = ST_NEXT;
			}
			else
				m_curtime += m_droptime- (int)(obj->score / 100);
		}
		if (m_state == ST_NORMAL)
		{
			//处理y方向平滑
			obj->m_ctl_dy = float(double(m_curtime) / (m_droptime - (int)(obj->score / 100)));
		}
		break;
	case ST_OVER:	//游戏结束
		if (m_KeyFlag[0] > 0)
			m_state = ST_START;
		break;
	default:
		break;
	}
	memset(m_KeyFlag, 0, sizeof(m_KeyFlag));
	return nRet;
}

//碰撞检测函数
bool isCrash(Game *obj)
{
	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x)
			if (g_trs_map[obj->m_ctl_s][obj->m_ctl_t][y][x])
				if (obj->m_ctl_y + y < 0 || obj->m_ctl_x + x < 0 || obj->m_gamepool[obj->m_ctl_y + y][obj->m_ctl_x + x])
					return true;
	return false;
}

//图型更新函数
void render(Game *obj)
{
	int x, y, c, bx, by;

	//读取最高记录
	FILE *fp;
	char ch = 0, l_highest[10],str;
	int highest,i,j,book[10];
	str = NULL;
	fp = fopen("highest_score.txt", "r");
	if (fp == NULL)
		highest = 0;
	else
	{
		i = 0;
		while (str != EOF)
		{
			str = fgetc(fp);
			l_highest[i] = str;
			i++;
		}
		l_highest[i] = '\0';
		highest = atoi(l_highest);
	}
	fclose(fp);

	//画背景框
	cleardevice();	//清屏
	

	//地图背景框
	drawframe(m_base_x + 5 * m_base_w, m_base_y, obj->m_gamepool_w*m_base_w, obj->m_gamepool_h*m_base_h);

	//下一方块背景框
	drawframe(m_base_x+6, m_base_y+43, 4 * m_base_w+4, 4 * m_base_h);

	//画游戏池
	bx = m_base_x + 4 * m_base_w;	//控制方块在游戏池中的x坐标
	by = m_base_y - 1 * m_base_h;	//控制方块在游戏池中的y坐标
	for (y = obj->m_gamepool_h; y >= 1; --y)
		for (x = 1; x <= obj->m_gamepool_w; ++x)
			if (c = obj->m_gamepool[y][x])
				putimage(bx + x*m_base_w, by + y*m_base_h, m_base_w, m_base_h, obj->m_pcb, c*m_base_w, 0);

	//画控制方块
	if (obj->m_ctl_t >= 0)
	{
		bx = m_base_x + (obj->m_ctl_x + 4) * m_base_w;
		by = m_base_y + (obj->m_ctl_y - 1) * m_base_h;
		draw44(obj, bx, by, g_trs_map[obj->m_ctl_s][obj->m_ctl_t], obj->m_ctl_dx, obj->m_ctl_dy);
	}

	//画下一方块
	bx = m_base_x+8;
	by = m_base_y+6;
	draw44(obj, bx, by+50, g_trs_map[obj->m_next_s][0]);

	//得分
	drawframe(5,185, 110, 150);
	setcolor(0xFFFFFF);
	setbkmode(TRANSPARENT);
	char string[20];
	setfont(22, 0, "楷体");
	outtextxy(5, 200, "得分");
	sprintf(string, "%d", obj->score);
	outtextxy(10, 230, string);
	outtextxy(5, 270, "历史最高分");
	sprintf(string, "%d", highest);
	outtextxy(10, 300, string);

	if (m_state == ST_OVER)
	{
		//存档
		if (obj->score > highest)
		{
			i = 0;
			if (obj->score > 0 && obj->score < 9)
			{
				i = 1;
				book[1] = obj->score;
			}
			else if (obj->score >= 10 && obj->score < 100)
			{
				i = 2;
				book[1] = obj->score / 10;
				book[2] = obj->score % 10;
			}
			else if (obj->score >= 100 && obj->score < 1000)
			{
				i = 3;
				book[1] = obj->score / 100;
				book[2] = (obj->score % 100) / 10;
				book[3] = (obj->score % 100) % 10;
			}
			else if (obj->score >= 1000 && obj->score < 10000)
			{
				i = 4;
				book[1] = obj->score / 1000;
				book[2] = (obj->score % 1000) / 100;
				book[3] = (obj->score % 100) / 10;
				book[4] = (obj->score % 100) % 10;
			}
			else if (obj->score >= 10000 && obj->score < 100000)
			{
				i = 5;
				book[1] = obj->score / 10000;
				book[2] = (obj->score % 10000) / 1000;
				book[3] = (obj->score % 1000) / 100;
				book[4] = (obj->score % 100) / 10;
				book[5] = (obj->score % 100) % 10;
			}
			else if (obj->score >= 100000 && obj->score < 1000000)
			{
				i = 6;
				book[1] = obj->score / 100000;
				book[2] = (obj->score % 100000) / 10000;
				book[3] = (obj->score % 10000) / 1000;
				book[4] = (obj->score % 1000) / 100;
				book[5] = (obj->score % 100) / 10;
				book[6] = (obj->score % 100) % 10;
			}
			fp = fopen("highest_score.txt", "w");
			fp = fopen("highest_score.txt", "a+");
			for (j = 1; j <= i; j++)
			{
				str = book[j] + 48;
				fputc(str, fp);
			}
			fclose(fp);
		}

		setcolor(0xFFFFFF);	//将绘图前景色设置为白色
		setfont(48, 0, "楷体");
		setbkmode(TRANSPARENT);
		outtextxy(m_base_x + 5 * m_base_w, m_base_y+200, "请按F2重玩");
		obj->score = 0;
	}

}

//合并，消行处理函数
void merge(Game *obj)
{
	int x, y, cy = obj->m_gamepool_h;

	for (y = 0; y < 4; ++y)
		for (x = 0; x < 4; ++x)
			if (g_trs_map[obj->m_ctl_s][obj->m_ctl_t][y][x])
				obj->m_gamepool[obj->m_ctl_y + y][obj->m_ctl_x + x] = g_trs_map[obj->m_ctl_s][obj->m_ctl_t][y][x];

	//消行计算
	for (y = obj->m_gamepool_h; y >= 1; --y)	//从底往上遍历消行
	{
		for (x = 1; x <= obj->m_gamepool_w; ++x)
		{
			if (obj->m_gamepool[y][x] == 0)
				break;
		}
		if (x <= obj->m_gamepool_w)
		{
			if (cy != y)
			{
				for (x = 1; x <= obj->m_gamepool_w; ++x)
					//将可消行的上一行覆盖可消行
					obj->m_gamepool[cy][x] = obj->m_gamepool[y][x];
			}
			--cy;
		}
	}

	//处理可消行上无行的情况，即可消行是当前的最顶行
	for (y = cy; y >= 1; --y)
		for (x = 1; x <= obj->m_gamepool_w; ++x)
		{
			obj->m_gamepool[y][x] = 0;
			obj->score++;
		}
}

//在方块外面画边框函数,起到美化作用
void drawedge(int x, int y, int w, int h)
{
	
	setcolor(EGERGB(0xFF, 0x14, 0x93));
	line(x, y + h, x, y);
	line(x-1, y + h, x-1, y);
	line(x - 2, y + h, x - 2, y);
	
	line(x-2, y+h, x+w, y+h);
	line(x-2, y + h+1, x + w, y + h+1);
	line(x-2, y + h+2, x + w, y + h+2);

	line(x+w+2, y + h+2, x+w+2, y);
	line(x+w +1, y + h+2, x+w +1, y);
	line(x+w, y + h+2, x+w , y);

	line(x-2, y, x+w+2, y);
	line(x-2, y+1, x + w+2, y +1);
	line(x-2, y+2, x + w+2, y +2);
}

void drawedge_bar(int x, int y, int w, int h)
{
	setcolor(EGERGB(0xCD, 0xCD, 0xB4));
	line(x, y + h, x, y);
	lineto(x + w, y);
	setcolor(EGERGB(0x8B, 0x8B, 0x7A));
}

//画方块函数
void drawtile(int x, int y, int w, int h, int color)
{
	w--, h--;	//不能取到边界值
	setfillcolor(color);	//设置方块的填充颜色
	bar(x + 1, y + 1, x + w, y + h);
	drawedge_bar(x, y, w, h);
}

//画图形边框函数
void drawframe(int x, int y, int w, int h, int d)
{
	setfillcolor(0x010101);	//设置背景框的填充颜色
	bar(x, y, x + w--, y + h--);
	drawedge(x, y, w, h);
}

//画控制方块函数
void draw44(Game *obj, int bx, int by, int mat[4][4], float dx, float dy)
{
	for (int y = 3; y >= 0; --y)
		for (int x = 0, c; x < 4; ++x)
			if (c = mat[y][x])
				drawtile(int(bx + (x + dx)*m_base_w + 1000.5) - 1000, int(by + (y - dy)*m_base_h + 1000.5) - 1000, m_base_w, m_base_h, obj->m_colormap[c]);
}