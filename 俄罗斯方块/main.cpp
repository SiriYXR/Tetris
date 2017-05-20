#include<stdio.h>
#include<graphics.h>
#include"tetris.h"


int main()
{
	int nfps = 120;	//����֡��
	initgr();	//��ʼ��ȫ������

	Game game;	//����Game����
	initGame(&game, 10, 20, 24, 24, nfps / 2, 10);	//��Ϸ��ʼ������

	//���ô��ڸ���ģʽΪ�ֶ�����
	setrendermode(RENDER_MANUAL);

	//��Ϸ����ѭ�������ȶ���֡�����У�ֱ���ر���Ϸ���ڽ�����Ϸ
	for (; is_run(); delay_fps(nfps))
	{
		update(&game);	//�߼�����������
		render(&game);	//ͼ�θ���������
	}

	delimage(game.m_pcb);	//ɾ��PIMAGE����
	getch();
	closegraph();	//�ر�ͼ�λ���
	return 0;
}