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
	void printbushu();//打印玩家使用的步数
	void addobstacle(int num);//随机添加num个障碍物
	void clearacir();//初始化地面
	void initCat();//初始化猫
	void setCatPoi(int i, int j); //根据地面下标把猫设置在指定的位置
	void updateWeight();//更新地面权值
	void updateCatPos();//更新猫的位置
	bool issurrounded(int x,int y);//判断是否有出路
};

