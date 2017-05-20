#include<time.h>
#include<stdio.h>
#include<string.h>
#include<ege.h>
#include"tetris.h"
#include"graphics.h"

const int g_width = 400;	//���ڵĿ��
const int g_height = 520;	//���ڵĸ߶�

int m_base_x, m_base_y;
int  m_base_w, m_base_h;	//����ĸ߶ȺͿ��

//����ٶȵĶ���
int m_droptime;	//�����ٶ�
int m_curtime;
int m_cursubtime;
int m_movxtime;	//�����ƶ��ٵ�
int m_curxtime;

//��Ϸ״̬�ͼ��̶���
int m_state;	//��Ϸ��״̬
int m_Keys[8];	//�����¼�
int m_KeyFlag[8];	//������ʶ
int m_KeyState[8];	//����״̬

//����ͼ�����飬��¼7����״����4����ת״̬
static int g_trs_map[8][4][4][4];

//��ͼ����״�ı仯��Ŀ��
static int g_map_mod[] = { 1, 4, 4, 4, 2, 2, 2, 1, 0 };

/*��ʼ��ȫ������*/
void initgr()
{
	//ͼ�δ��ڳ�ʼ�������ڳߴ�400*520
	initgraph(g_width, g_height);
	//������Ϸ����
	setfont(12, 6, "����");


	//��ʼ��7����״��4�ֱ仯��ÿ�ֱ仯4*4�����ʾ
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
	//������Trs_map�е����ݿ���������ͼ������g_trs_map��
	memcpy(g_trs_map, Trs_map, sizeof(Trs_map));
}

/*******************************/
/*obj:��ϷGame�ṹ��ָ�����*/
/*w.h:��Ϸ�����Ŀ�͸�*/
/*bw.bh:��λ����Ŀ�͸�*/
/*droptime�����������ٶ�*/
/*movetime:��������ƶ��ٵ�*/
/*******************************/
//��Ϸ��ʼ������
void initGame(Game *obj, int w, int h, int bw, int bh, int droptime, int movetime)
{
	//��ʼ����Ϸ�õ���10����ɫ
	int colormap[10] = { 0,0xFF0000,0xFF4500,0xFFD700,0x00FF00,0x00FFFF,0x1874CD,0xFF1493,0x808080,0xFFFFFF };
	memcpy(obj->m_colormap, colormap, sizeof(obj->m_colormap));

	//��������¼�
	int Keys[8] = { VK_F2,VK_LEFT,VK_RIGHT,VK_DOWN,VK_UP,VK_NUMPAD0,VK_SPACE };
	memcpy(m_Keys, Keys, sizeof(Keys));
	//��ʼ������״̬
	memset(m_KeyState, 0, sizeof(m_KeyState));

	//��ʼ����Ϸ�صĸߺͿ�
	obj->m_gamepool_w = w;	//��Ϊ10
	obj->m_gamepool_h = h;	//��Ϊ20

	//��ʼ����λ����ĸߺͿ�
	m_base_w = bw;	//����Ϊ24
	m_base_h = bh;	//����Ϊ24

	//��ʼ������ͼ�������ٶȺͺ����ƶ��ٶ�
	m_droptime = droptime;
	m_movxtime = movetime;

	//�������ʼ��
	randomize();

	//��ǰ���ط��������
	obj->m_ctl_t = -1;

	//����һ��ͼ�ζ���
	obj->m_pcb = newimage();

	for (int i = 0; i < 10; ++i)
	{
		drawtile(bw*i, 0, bw, bh, colormap[i]);
	}

	//����Ļ��ȡͼ��
	getimage(obj->m_pcb, 0, 0, bw * 10, bh);

	//������Ϸ��״̬δ��ʼ״̬
	m_state = ST_START;
}

//�߼����º���
void update(Game *obj)
{
	key_msg key;
	int k = kbmsg();	//��������Ϣ

	while (k)
	{
		key = getkey();	//��ȡ������Ϣ
		for (int i = 0; i < 8; ++i)
		{
			if (key.key == m_Keys[i])
			{
				if (key.msg == key_msg_down)	//���̰���
				{
					m_KeyFlag[i]++;
					m_KeyState[i] = 1;
				}
				else if (key.msg == key_msg_up)	//���̵���
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
	while (deal(obj));	//״̬ת������
}

//״̬ת��������
int deal(Game *obj)
{
	int nRet = 0;
	switch (m_state)
	{
	case ST_START:	//��ʼ��
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
	case ST_NEXT:	//׼����һ������
		//������ֵĳ�ʼλ��
		obj->m_ctl_x = (obj->m_gamepool_w - 4) / 2 + 1;
		obj->m_ctl_y = 1;
		obj->m_ctl_t = 0;
		obj->m_ctl_s = obj->m_next_s;
		obj->m_ctl_dy = 0;
		obj->m_next_s = random(7) + 1;
		m_curtime = m_droptime-(int)(obj->score/100);	//���������ٶ�
		m_curxtime = 0;
		nRet = 1;
		if (isCrash(obj))
			m_state = ST_OVER;
		else
			m_state = ST_NORMAL;
		break;
	case ST_NORMAL:	//��ҿ��ƽ׶�
		//��������������
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

		//������
		for (i = 1, j = 1; i <= 2; ++i, j -= 2)
			//ˮƽ�ϸ��ݰ����ҷ�����Ĵ������¶�λ����
			for (; m_KeyFlag[i] > 0; --m_KeyFlag[i])
			{
				obj->m_ctl_x -= j;	//�����1���Ҽ���1
				if (isCrash(obj))
					obj->m_ctl_x += j;
				else
					m_curxtime = m_movxtime*j;
			}

		//����x����ƽ��
		obj->m_ctl_dx = float(double(m_curxtime) / m_movxtime);

		//��ת����
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
			m_curtime -= m_cursubtime++;	//�����¼����������
		}

		if (m_curtime < 0)	//��ʱ
		{
			++obj->m_ctl_y;	//����һ��
			if (isCrash(obj))
			{
				--obj->m_ctl_y;
				merge(obj);	//�ϲ�����
				obj->m_ctl_dy = 0;
				obj->m_ctl_dx = 0;
				obj->m_ctl_t = -1;
				if (m_KeyState[3])
				{
					obj->m_forbid_down = 1;	//�Ѿ��ϲ�������������
				}
				m_state = ST_NEXT;
			}
			else
				m_curtime += m_droptime- (int)(obj->score / 100);
		}
		if (m_state == ST_NORMAL)
		{
			//����y����ƽ��
			obj->m_ctl_dy = float(double(m_curtime) / (m_droptime - (int)(obj->score / 100)));
		}
		break;
	case ST_OVER:	//��Ϸ����
		if (m_KeyFlag[0] > 0)
			m_state = ST_START;
		break;
	default:
		break;
	}
	memset(m_KeyFlag, 0, sizeof(m_KeyFlag));
	return nRet;
}

//��ײ��⺯��
bool isCrash(Game *obj)
{
	for (int y = 0; y < 4; ++y)
		for (int x = 0; x < 4; ++x)
			if (g_trs_map[obj->m_ctl_s][obj->m_ctl_t][y][x])
				if (obj->m_ctl_y + y < 0 || obj->m_ctl_x + x < 0 || obj->m_gamepool[obj->m_ctl_y + y][obj->m_ctl_x + x])
					return true;
	return false;
}

//ͼ�͸��º���
void render(Game *obj)
{
	int x, y, c, bx, by;

	//��ȡ��߼�¼
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

	//��������
	cleardevice();	//����
	

	//��ͼ������
	drawframe(m_base_x + 5 * m_base_w, m_base_y, obj->m_gamepool_w*m_base_w, obj->m_gamepool_h*m_base_h);

	//��һ���鱳����
	drawframe(m_base_x+6, m_base_y+43, 4 * m_base_w+4, 4 * m_base_h);

	//����Ϸ��
	bx = m_base_x + 4 * m_base_w;	//���Ʒ�������Ϸ���е�x����
	by = m_base_y - 1 * m_base_h;	//���Ʒ�������Ϸ���е�y����
	for (y = obj->m_gamepool_h; y >= 1; --y)
		for (x = 1; x <= obj->m_gamepool_w; ++x)
			if (c = obj->m_gamepool[y][x])
				putimage(bx + x*m_base_w, by + y*m_base_h, m_base_w, m_base_h, obj->m_pcb, c*m_base_w, 0);

	//�����Ʒ���
	if (obj->m_ctl_t >= 0)
	{
		bx = m_base_x + (obj->m_ctl_x + 4) * m_base_w;
		by = m_base_y + (obj->m_ctl_y - 1) * m_base_h;
		draw44(obj, bx, by, g_trs_map[obj->m_ctl_s][obj->m_ctl_t], obj->m_ctl_dx, obj->m_ctl_dy);
	}

	//����һ����
	bx = m_base_x+8;
	by = m_base_y+6;
	draw44(obj, bx, by+50, g_trs_map[obj->m_next_s][0]);

	//�÷�
	drawframe(5,185, 110, 150);
	setcolor(0xFFFFFF);
	setbkmode(TRANSPARENT);
	char string[20];
	setfont(22, 0, "����");
	outtextxy(5, 200, "�÷�");
	sprintf(string, "%d", obj->score);
	outtextxy(10, 230, string);
	outtextxy(5, 270, "��ʷ��߷�");
	sprintf(string, "%d", highest);
	outtextxy(10, 300, string);

	if (m_state == ST_OVER)
	{
		//�浵
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

		setcolor(0xFFFFFF);	//����ͼǰ��ɫ����Ϊ��ɫ
		setfont(48, 0, "����");
		setbkmode(TRANSPARENT);
		outtextxy(m_base_x + 5 * m_base_w, m_base_y+200, "�밴F2����");
		obj->score = 0;
	}

}

//�ϲ������д�����
void merge(Game *obj)
{
	int x, y, cy = obj->m_gamepool_h;

	for (y = 0; y < 4; ++y)
		for (x = 0; x < 4; ++x)
			if (g_trs_map[obj->m_ctl_s][obj->m_ctl_t][y][x])
				obj->m_gamepool[obj->m_ctl_y + y][obj->m_ctl_x + x] = g_trs_map[obj->m_ctl_s][obj->m_ctl_t][y][x];

	//���м���
	for (y = obj->m_gamepool_h; y >= 1; --y)	//�ӵ����ϱ�������
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
					//�������е���һ�и��ǿ�����
					obj->m_gamepool[cy][x] = obj->m_gamepool[y][x];
			}
			--cy;
		}
	}

	//��������������е���������������ǵ�ǰ�����
	for (y = cy; y >= 1; --y)
		for (x = 1; x <= obj->m_gamepool_w; ++x)
		{
			obj->m_gamepool[y][x] = 0;
			obj->score++;
		}
}

//�ڷ������滭�߿���,����������
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

//�����麯��
void drawtile(int x, int y, int w, int h, int color)
{
	w--, h--;	//����ȡ���߽�ֵ
	setfillcolor(color);	//���÷���������ɫ
	bar(x + 1, y + 1, x + w, y + h);
	drawedge_bar(x, y, w, h);
}

//��ͼ�α߿���
void drawframe(int x, int y, int w, int h, int d)
{
	setfillcolor(0x010101);	//���ñ�����������ɫ
	bar(x, y, x + w--, y + h--);
	drawedge(x, y, w, h);
}

//�����Ʒ��麯��
void draw44(Game *obj, int bx, int by, int mat[4][4], float dx, float dy)
{
	for (int y = 3; y >= 0; --y)
		for (int x = 0, c; x < 4; ++x)
			if (c = mat[y][x])
				drawtile(int(bx + (x + dx)*m_base_w + 1000.5) - 1000, int(by + (y - dy)*m_base_h + 1000.5) - 1000, m_base_w, m_base_h, obj->m_colormap[c]);
}