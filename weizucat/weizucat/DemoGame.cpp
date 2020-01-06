#include "DemoGame.h"
#include <time.h>
CDemoGame::CDemoGame(HINSTANCE hInstance, HWND hWnd, int nFps ):CGame(hInstance, hWnd, nFps)
{
}


CDemoGame::~CDemoGame(void)
{
}
void CDemoGame::HandleMouseUp(WPARAM wParam, LPARAM lParam)
{
	switch (gamestate)
	{
	case State::start:
		//开始界面开始按钮的响应
		if (startbutton->IsSelected(LOWORD(lParam), HIWORD(lParam))){
			start->SetVisible(false);			
			startbutton->SetVisible(false);	//清除开始界面的元素
			bushu = 0;						//出初始化步数
			clearacir();					//将路线全设为空
			initCat();						//将神经猫放置在游戏中央
			addobstacle(15);				//随机增加15个围栏
			gamestate = State::game;		//切换游戏状态
		}
		break;
	case State::game:
		//游戏界面玩家鼠标添加障碍物的响应
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++){
				if (acir[i][j].blanksprite->IsSelected(LOWORD(lParam), HIWORD(lParam))){
					if (acir[i][j].type == Alltype::blank){					//如果玩家点击的路面是空的
						bushu ++;											//增加步数
					acir[i][j].blanksprite->SetVisible(false);				
					acir[i][j].obstaclesprite->SetVisible(true);			//将路面设为红色
					acir[i][j].type = Alltype::obstacle;					//路面类型设为障碍物
					acir[i][j].weight = 9999;							//权重设为最大
					updateWeight();										//更新所有的权重
					updateCatPos();										//更新神经猫的位置
					}
				}


			}
		}
		//判断猫是否完全被围住
		if (acir[aCat.x][aCat.y].weight ==10000){
			gamestate = State::win;
		}
		//判断猫是否成功逃离
		if (acir[aCat.x][aCat.y].weight ==0){
			gamestate = State::fail;
		}
		break;
	case State::win:
		//成功界面重新开始按钮的响应
		if (again->IsSelected(LOWORD(lParam), HIWORD(lParam))){
			success->SetVisible(false);
			again->SetVisible(false);
			bushu = 0;
			clearacir();
			initCat();
			addobstacle(15);
			gamestate = State::game;
			for (int i = 0; i < 3; i++){
				if (shuzi[i]){
					shuzi[i]->SetVisible(false);
				}
			}
		}
		break;
	case State::fail:
		//失败界面重新开始按钮的响应
		if (again->IsSelected(LOWORD(lParam), HIWORD(lParam))){
			fail->SetVisible(false);
			again->SetVisible(false);
			bushu = 0;
			clearacir();
			initCat();
			addobstacle(15);
			
			gamestate = State::game;
		}
		break;
	}
	
}
//神经猫的初始化
void CDemoGame::initCat(){
	aCat.cat->SetVisible(true);
	aCat.x = 4;
	aCat.y = 4;
	acir[4][4].type = Alltype::cat;
	setCatPoi(4, 4);				//将位置设置在第五行第五列
	
	
}
//根据地面下标把猫设置在指定的位置
void  CDemoGame::setCatPoi(int i, int j){
	if (acir[i][j].type == Alltype::blank || acir[i][j].type == Alltype::cat){		//如果该位置的类型不是障碍物
		acir[aCat.x][aCat.y].type = Alltype::blank;									//将猫原来的位置设置为可走类型
		aCat.cat->SetPos(D2D1::Point2F(acir[i][j].blanksprite->GetPos().x - 10, acir[i][j].blanksprite->GetPos().y - 70)); //将神经猫移动到指定的位置
		aCat.x = i;
		aCat.y = j;			//记录神经猫当前处在的下标
		acir[i][j].type = Alltype::cat;		//将神经猫处在的位置的类型标记为猫
	}
}
//游戏初始化操作
void CDemoGame::InitGame(){
	srand(time(0));				//随机种子的设置
	for (int i = 0; i < 3; i++){
		shuzi[i] = NULL;
	}
	
	pSpritesManager = CSpritesManager::GetInstance();
	pResPool = CResourcesPool::GetInstance();
	//加载资源
	pResPool->AddTexture(L"bg", L"res/bg.jpg");
	pResPool->AddTexture(L"start", L"res/start.png");
	pResPool->AddTexture(L"startbutton", L"res/startbutton.png");
	pResPool->AddTexture(L"blank", L"res/blank.png");
	pResPool->AddTexture(L"obstacle", L"res/obstacle.png");
	pResPool->AddTexture(L"cat", L"res/cat01.png");
	pResPool->AddTexture(L"cat2", L"res/cat02.png");
	pResPool->AddTexture(L"fail", L"res/fail.png");
	pResPool->AddTexture(L"success", L"res/shengli.png");
	pResPool->AddTexture(L"again", L"res/again.png");
	pResPool->AddTexture(L"shuzi", L"res/shuzi.png");
	//创建背景
	bg = pSpritesManager->CreateSprite(L"bg", D2D1::Point2F(0, 0));
	bg->SetScaleFactor(0.8, 0.8);
	start = pSpritesManager->CreateSprite(L"start", D2D1::Point2F(64,100));
	startbutton = pSpritesManager->CreateSprite(L"startbutton", D2D1::Point2F(165, 600));
	//创建地砖
	int Left =20;
	int Top = 280;
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if (i % 2){
				acir[i][j].obstaclesprite = pSpritesManager->CreateSprite(L"obstacle", D2D1::Point2F(Left + 22.5 + j * 50, Top + i * 45));
				acir[i][j].blanksprite = pSpritesManager->CreateSprite(L"blank", D2D1::Point2F(Left + 22.5 + j * 50, Top + i * 45));
			}
			else{
				acir[i][j].obstaclesprite = pSpritesManager->CreateSprite(L"obstacle", D2D1::Point2F(Left + j * 50, Top + i * 45));
				acir[i][j].blanksprite = pSpritesManager->CreateSprite(L"blank", D2D1::Point2F(Left+j * 50,Top+ i * 45));
			}
			acir[i][j].obstaclesprite->SetVisible(false);
			acir[i][j].blanksprite->SetVisible(false);
			acir[i][j].type = Alltype::blank;
			acir[i][j].x = i;
			acir[i][j].y = j;
		}
	}
	//创建主角
	aCat.cat = pSpritesManager->CreateAnimationSprite(L"cat", 16, 16, D2D1::Point2F(acir[4][4].blanksprite->GetPos().x - 10, acir[4][4].blanksprite->GetPos().y - 70), 61, 93);
	aCat.cat->SetVisible(false);
	aCat.cat->Play();
	//失败成功窗口
	fail = pSpritesManager->CreateSprite(L"fail", D2D1::Point2F(32, 100));
	success = pSpritesManager->CreateSprite(L"success", D2D1::Point2F(32, 120));
	again = pSpritesManager->CreateSprite(L"again", D2D1::Point2F(165, 500));
	success->SetVisible(false);
	again->SetVisible(false);
	fail->SetVisible(false);

	
	gamestate = State::start;
}

//随机添加num个障碍物
void CDemoGame::addobstacle(int num){

	int a = rand() % 9;
	for (int i = 0; i < num;) {
		int randomI = rand() % 9;
		int randomJ = rand() % 9;
		if (acir[randomI][randomJ].type==Alltype::blank) {
			acir[randomI][randomJ].weight = 9999;
			acir[randomI][randomJ].type = Alltype::obstacle;
			acir[randomI][randomJ].obstaclesprite->SetVisible(true);
			acir[randomI][randomJ].blanksprite->SetVisible(false);
			i++;
		}
	}
	updateWeight();
}

//将路面全设置为可走
void CDemoGame::clearacir(){
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){

			acir[i][j].obstaclesprite->SetVisible(false);
			acir[i][j].blanksprite->SetVisible(true);
			acir[i][j].type = Alltype::blank;
		}
	}

	for (int k = 0; k < 5;k++){
	for (int i = k; i < 9-k; i++){
		for (int j = k; j < 9 - k; j++){
			acir[i][j].weight = k;
		}
	}
	
}
	
}

//查找神经猫六个邻接点中权重最小的一个点并朝该点移动
void CDemoGame::updateCatPos(){
	int i = aCat.x;
	int j = aCat.y;
	//查找六个邻接点
	if (i % 2){
		int a[6] = { acir[i][j - 1].weight, acir[i - 1][j + 1].weight, acir[i - 1][j].weight, acir[i + 1][j].weight, acir[i][j + 1].weight, acir[i + 1][j + 1].weight };
		//选出权重最小的一个邻接点
		int minindex = 0;
		for (int z = 1; z < 6; z++){
			if (a[minindex]>a[z])minindex = z;
		}
		//设置到相应的位置
		switch (minindex)
		{
		case 0:
		
			setCatPoi(i, j - 1);
			break;
		case 1:
			setCatPoi(i - 1, j + 1);
			break;
		case 2:
			setCatPoi(i - 1, j);
			break;
		case 3:
			setCatPoi(i + 1, j);
			break;
		case 4:
			setCatPoi(i, j + 1);
			break;
		case 5:
			setCatPoi(i + 1, j + 1);
			break;
		default:
			break;
		}
	}
	else{
		int a[6] = { acir[i][j - 1].weight, acir[i - 1][j - 1].weight, acir[i - 1][j].weight, acir[i + 1][j].weight, acir[i][j + 1].weight, acir[i + 1][j - 1].weight };
		int minindex = 0;
		for (int z = 1; z < 6; z++){
			if (a[minindex]>a[z])minindex = z;
		}
		switch (minindex)
		{
		case 0:
			setCatPoi(i, j - 1);
			break;
		case 1:
			setCatPoi(i - 1, j - 1);
			break;
		case 2:
			setCatPoi(i - 1, j);
			break;
		case 3:
			setCatPoi(i + 1, j);
			break;
		case 4:
			setCatPoi(i, j + 1);
			break;
		case 5:
			setCatPoi(i + 1, j - 1);
			break;
		default:
			break;
		}
	}
}
//更新所有的权重（障碍物为9999，其他点为到边界的距离）
void CDemoGame::updateWeight(){
	for (int i = 1; i < 8; i+=2){
		for (int j = 1; j < 8; j++){
			if (acir[i][j].type == Alltype::obstacle){ continue; }
			//获取邻接点
			int a[6] = { acir[i][j - 1].weight, acir[i - 1][j + 1].weight, acir[i - 1][j].weight, acir[i + 1][j].weight, acir[i][j + 1].weight, acir[i + 1][j + 1].weight };
			//获取邻接点中最小的值
			int min = a[0];
			for (int z = 1; z < 6; z++){
				if (min>a[z])min = a[z];
			}
			//该点的值等于六个邻接点权重中最小的值加一
			acir[i][j].weight = min+1;

		}
	}
	for (int i = 2; i < 8; i += 2){
		for (int j = 2; j < 8; j ++){
			if (acir[i][j].type == Alltype::obstacle){ continue; }
			//获取邻接点
	int a[6] = { acir[i][j - 1].weight, acir[i - 1][j - 1].weight, acir[i - 1][j].weight, acir[i + 1][j].weight, acir[i][j + 1].weight, acir[i + 1][j - 1].weight };
	//获取邻接点中最小的值
	int min = a[0];
	for (int z = 1; z < 6; z++){
		if (min>a[z])min = a[z];
	}
	//该点的值等于六个邻接点权重中最小的值加一
	acir[i][j].weight = min+1;

		}
	}
		
		}

//帧循环
void CDemoGame::Update(float fDeltaTime){
	switch (gamestate)
	{
	case State::start:
		
		break;
	case State::game:
		

		break;
	case State::win:
		//显示成功界面
		success->SetVisible(true);
		again->SetVisible(true);
		printbushu();
		break;
	case State::fail:
		//显示失败界面
		fail->SetVisible(true);
		again->SetVisible(true);
		break;
	}
	CGame::Update(fDeltaTime);
}
//显示玩家点击的步数
void CDemoGame::printbushu(){
	int digit;
	int tmp = bushu;
	for (int nDigit = 0; nDigit < 3; nDigit++){
		digit = tmp % 10;
		if (!shuzi[nDigit]){
			shuzi[nDigit] = pSpritesManager->CreateAnimationSprite(L"shuzi", 1, 1, D2D1::Point2F(300 - 20 * (nDigit + 1), 350), 20, 30);
		}
		shuzi[nDigit]->SetTexPos((digit-1) * 20, 0);
		shuzi[nDigit]->SetVisible(true);
		tmp = tmp / 10;
		if (tmp == 0)
			break;
	}


}

