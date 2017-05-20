#ifndef _TETRIS_H
#define _TETRIS_H
#include<graphics.h>

#define VK_F2      0x71 //F2��
#define VK_LEFT    0x25 //�������
#define VK_RIGHT   0x27 //�����Ҽ�
#define VK_DOWN	   0x28 //�����¼�
#define VK_UP	   0x26 //�����ϼ�
#define VK_NUMPAD0 0x60 //С����0��
#define VK_SPACE   0x20 //�ո��

//״̬��
enum
{
	ST_START,  //��Ϸ���¿�ʼ
	ST_NEXT,   //׼����һ������
	ST_NORMAL, //��ҿ��ƽ׶�
	ST_OVER    //��Ϸ������F2���¿�ʼ
};

typedef struct TetrisGame
{
	int m_gamepool_w, m_gamepool_h;	//��Ϸ��ͼ��͸�
	int m_gamepool[30][30];	//��1Ϊ��ʼ�±꣬�±�0���ڱ߽���ײ���

	/**********************/
	/*  �����Ʒ��������  */
	/**********************/

	int m_ctl_x, m_ctl_y;
	int m_ctl_t;	//��n��״̬��ĳ�ֱ��η���
	int m_ctl_s;	//7��״̬�е�һ��
	float m_ctl_dx, m_ctl_dy;	//������x,y����
	int m_next_s;	//��һ������
	int m_forbid_down;	//�����Ƿ���������ʶ
	int m_colormap[10];	//�����10����ɫ
	int score=0;
	
	PIMAGE m_pcb;	//������Ϊm_pcb��PIMAGE����
}Game;

//��ʼ��ȫ�����ݺ���
void initgr();
//��Ϸ��ʼ������
void initGame(Game *obj, int w, int h, int bw, int bh, int droptime, int movetime);
//�߼����º���
void update(Game *obj);
//״̬ת��������
int deal(Game *obj);
//��ײ��⺯��
bool isCrash(Game *obj);
//ͼ�͸��º���
void render(Game *obj);
//�ϲ������д�����
void merge(Game *obj);
//�ڷ������滭�߿���,����������
void drawedge(int x, int y, int w, int h);
//�����麯��
void drawtile(int x, int y, int w, int h, int color);
//��ͼ�α߿���
void drawframe(int x, int y, int w, int h, int d = 0);
//�����Ʒ��麯��
void draw44(Game *obj, int bx, int by, int mat[4][4], float dx = 0, float dy = 0);

#endif
