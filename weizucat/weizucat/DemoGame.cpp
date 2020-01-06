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
		//��ʼ���濪ʼ��ť����Ӧ
		if (startbutton->IsSelected(LOWORD(lParam), HIWORD(lParam))){
			start->SetVisible(false);			
			startbutton->SetVisible(false);	//�����ʼ�����Ԫ��
			bushu = 0;						//����ʼ������
			clearacir();					//��·��ȫ��Ϊ��
			initCat();						//����è��������Ϸ����
			addobstacle(15);				//�������15��Χ��
			gamestate = State::game;		//�л���Ϸ״̬
		}
		break;
	case State::game:
		//��Ϸ��������������ϰ������Ӧ
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++){
				if (acir[i][j].blanksprite->IsSelected(LOWORD(lParam), HIWORD(lParam))){
					if (acir[i][j].type == Alltype::blank){					//�����ҵ����·���ǿյ�
						bushu ++;											//���Ӳ���
					acir[i][j].blanksprite->SetVisible(false);				
					acir[i][j].obstaclesprite->SetVisible(true);			//��·����Ϊ��ɫ
					acir[i][j].type = Alltype::obstacle;					//·��������Ϊ�ϰ���
					acir[i][j].weight = 9999;							//Ȩ����Ϊ���
					updateWeight();										//�������е�Ȩ��
					updateCatPos();										//������è��λ��
					}
				}


			}
		}
		//�ж�è�Ƿ���ȫ��Χס
		if (acir[aCat.x][aCat.y].weight ==10000){
			gamestate = State::win;
		}
		//�ж�è�Ƿ�ɹ�����
		if (acir[aCat.x][aCat.y].weight ==0){
			gamestate = State::fail;
		}
		break;
	case State::win:
		//�ɹ��������¿�ʼ��ť����Ӧ
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
		//ʧ�ܽ������¿�ʼ��ť����Ӧ
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
//��è�ĳ�ʼ��
void CDemoGame::initCat(){
	aCat.cat->SetVisible(true);
	aCat.x = 4;
	aCat.y = 4;
	acir[4][4].type = Alltype::cat;
	setCatPoi(4, 4);				//��λ�������ڵ����е�����
	
	
}
//���ݵ����±��è������ָ����λ��
void  CDemoGame::setCatPoi(int i, int j){
	if (acir[i][j].type == Alltype::blank || acir[i][j].type == Alltype::cat){		//�����λ�õ����Ͳ����ϰ���
		acir[aCat.x][aCat.y].type = Alltype::blank;									//��èԭ����λ������Ϊ��������
		aCat.cat->SetPos(D2D1::Point2F(acir[i][j].blanksprite->GetPos().x - 10, acir[i][j].blanksprite->GetPos().y - 70)); //����è�ƶ���ָ����λ��
		aCat.x = i;
		aCat.y = j;			//��¼��è��ǰ���ڵ��±�
		acir[i][j].type = Alltype::cat;		//����è���ڵ�λ�õ����ͱ��Ϊè
	}
}
//��Ϸ��ʼ������
void CDemoGame::InitGame(){
	srand(time(0));				//������ӵ�����
	for (int i = 0; i < 3; i++){
		shuzi[i] = NULL;
	}
	
	pSpritesManager = CSpritesManager::GetInstance();
	pResPool = CResourcesPool::GetInstance();
	//������Դ
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
	//��������
	bg = pSpritesManager->CreateSprite(L"bg", D2D1::Point2F(0, 0));
	bg->SetScaleFactor(0.8, 0.8);
	start = pSpritesManager->CreateSprite(L"start", D2D1::Point2F(64,100));
	startbutton = pSpritesManager->CreateSprite(L"startbutton", D2D1::Point2F(165, 600));
	//������ש
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
	//��������
	aCat.cat = pSpritesManager->CreateAnimationSprite(L"cat", 16, 16, D2D1::Point2F(acir[4][4].blanksprite->GetPos().x - 10, acir[4][4].blanksprite->GetPos().y - 70), 61, 93);
	aCat.cat->SetVisible(false);
	aCat.cat->Play();
	//ʧ�ܳɹ�����
	fail = pSpritesManager->CreateSprite(L"fail", D2D1::Point2F(32, 100));
	success = pSpritesManager->CreateSprite(L"success", D2D1::Point2F(32, 120));
	again = pSpritesManager->CreateSprite(L"again", D2D1::Point2F(165, 500));
	success->SetVisible(false);
	again->SetVisible(false);
	fail->SetVisible(false);

	
	gamestate = State::start;
}

//������num���ϰ���
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

//��·��ȫ����Ϊ����
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

//������è�����ڽӵ���Ȩ����С��һ���㲢���õ��ƶ�
void CDemoGame::updateCatPos(){
	int i = aCat.x;
	int j = aCat.y;
	//���������ڽӵ�
	if (i % 2){
		int a[6] = { acir[i][j - 1].weight, acir[i - 1][j + 1].weight, acir[i - 1][j].weight, acir[i + 1][j].weight, acir[i][j + 1].weight, acir[i + 1][j + 1].weight };
		//ѡ��Ȩ����С��һ���ڽӵ�
		int minindex = 0;
		for (int z = 1; z < 6; z++){
			if (a[minindex]>a[z])minindex = z;
		}
		//���õ���Ӧ��λ��
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
//�������е�Ȩ�أ��ϰ���Ϊ9999��������Ϊ���߽�ľ��룩
void CDemoGame::updateWeight(){
	for (int i = 1; i < 8; i+=2){
		for (int j = 1; j < 8; j++){
			if (acir[i][j].type == Alltype::obstacle){ continue; }
			//��ȡ�ڽӵ�
			int a[6] = { acir[i][j - 1].weight, acir[i - 1][j + 1].weight, acir[i - 1][j].weight, acir[i + 1][j].weight, acir[i][j + 1].weight, acir[i + 1][j + 1].weight };
			//��ȡ�ڽӵ�����С��ֵ
			int min = a[0];
			for (int z = 1; z < 6; z++){
				if (min>a[z])min = a[z];
			}
			//�õ��ֵ���������ڽӵ�Ȩ������С��ֵ��һ
			acir[i][j].weight = min+1;

		}
	}
	for (int i = 2; i < 8; i += 2){
		for (int j = 2; j < 8; j ++){
			if (acir[i][j].type == Alltype::obstacle){ continue; }
			//��ȡ�ڽӵ�
	int a[6] = { acir[i][j - 1].weight, acir[i - 1][j - 1].weight, acir[i - 1][j].weight, acir[i + 1][j].weight, acir[i][j + 1].weight, acir[i + 1][j - 1].weight };
	//��ȡ�ڽӵ�����С��ֵ
	int min = a[0];
	for (int z = 1; z < 6; z++){
		if (min>a[z])min = a[z];
	}
	//�õ��ֵ���������ڽӵ�Ȩ������С��ֵ��һ
	acir[i][j].weight = min+1;

		}
	}
		
		}

//֡ѭ��
void CDemoGame::Update(float fDeltaTime){
	switch (gamestate)
	{
	case State::start:
		
		break;
	case State::game:
		

		break;
	case State::win:
		//��ʾ�ɹ�����
		success->SetVisible(true);
		again->SetVisible(true);
		printbushu();
		break;
	case State::fail:
		//��ʾʧ�ܽ���
		fail->SetVisible(true);
		again->SetVisible(true);
		break;
	}
	CGame::Update(fDeltaTime);
}
//��ʾ��ҵ���Ĳ���
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

