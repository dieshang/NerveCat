#pragma once
#include "framework\game.h"
using namespace std;
enum Alltype{
	cat,
	obstacle,
	blank
};
enum State{
	start,
	game,
	win,
	fail
};
struct Circle{
	int x;
	int y;
	int weight;
	Alltype type;
	CSprite*blanksprite;
	CSprite*obstaclesprite;
};
struct Cat{
	int x;
	int y;
	CAnimationSprite *cat;
};
class CDemoGame :public CGame
{
public:
	CDemoGame(HINSTANCE hInstance, HWND hWnd, int nFps = 0);
	virtual ~CDemoGame(void);
public:
	void InitGame();
	void Update(float fDeltaTime);
	void HandleMouseUp(WPARAM wParam, LPARAM lParam);
private:
	State gamestate;
	CSprite*bg;
	CSprite*start;
	CSprite*startbutton;
	CSprite*fail;
	CSprite*success;
	CSprite*again;
	CAnimationSprite*shuzi[3];
	Circle acir[9][9];
	Cat aCat;
	int bushu;
	void printbushu();//��ӡ���ʹ�õĲ���
	void addobstacle(int num);//������num���ϰ���
	void clearacir();//��ʼ������
	void initCat();//��ʼ��è
	void setCatPoi(int i, int j); //���ݵ����±��è������ָ����λ��
	void updateWeight();//���µ���Ȩֵ
	void updateCatPos();//����è��λ��
	bool issurrounded(int x,int y);//�ж��Ƿ��г�·
};

