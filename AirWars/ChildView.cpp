
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "AirWars.h"
#include "ChildView.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SPEED 10
#define HEIGHT 590
#define WIDTH 800
#define TIMER_BG 1
#define TIMER_ENEMY 2
#define TIMER_BULLET 3
#define TIMER_PAINT 4
// CChildView
using namespace std;


CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
//	ON_WM_CLOSE()
ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	m_gameBk.fir.Load(_T("res\\timg.jpg"));
	m_gameBk.sec.Load(_T("res\\timg1.jpg"));
	m_gameBk.curx = 0;
	m_gameBk.isFir = TRUE;

	start_png.Load(_T("res\\start.png"));
	start_pos.left = 320;
	start_pos.right = 420;
	start_pos.top = 250;
	start_pos.bottom = 320;
	exit_png.Load(_T("res\\exit.png"));
	exit_pos.left = 320;
	exit_pos.right = 420;
	exit_pos.top = 350;
	exit_pos.bottom = 420;
	again_png.Load(_T("res\\Again.png"));
	again_pos.left = 320;
	again_pos.right = 420;
	again_pos.top = 250;
	again_pos.bottom = 320;

	airwars_png.Load(_T("res\\airwars.png"));
	airwars_pos.left = 180;
	airwars_pos.right = 580;
	airwars_pos.top = 100;
	airwars_pos.bottom = 200;
	myplane->plane.Load(L"res\\Plane.png");
	TransparentPNG(&myplane->plane);

	//初始化工作
	mybullet.resize(40);
	for (int i = 0; i < 40; i++)
	{
		mybullet[i].type = MYPLANE;
		mybullet[i].valid = false;
		mybullet[i].bullet.Load(_T("res\\mybullet.png"));
		mybullet[i].speed = 10;
	}

	enemy.resize(40);
	for (int i = 0; i < 40; i++)
	{
		enemy[i].type = ENEMYPLANE;
		enemy[i].speed = 2;
		enemy[i].life = 1;
		enemy[i].plane.Load(_T("res\\Enemy.png"));
		enemy[i].valid = false;
	}

	enemybullet.resize(40);
	for (int i = 0; i < 40; i++)
	{
		enemybullet[i].type = ENEMYPLANE;
		enemybullet[i].valid = false;
		enemybullet[i].bullet.Load(_T("res\\enemybullet.png"));
		enemybullet[i].speed = 3;
	}
	game_state = Start;
	/*m_myplanePos.left = 100;
	m_myplanePos.right = 100 + 60;
	m_myplanePos.top = 500;
	m_myplanePos.bottom = 500 + 60;*/
	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO:  在此处添加消息处理程序代码
	CDC *cDC = this->GetDC();
	GetClientRect(&m_client);
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height() *2);
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	//画背景
	draw_bg();
	//根据状态贴图
	switch (game_state)
	{
	case Start:
		start();
		break;
	case Running:
		//画我方子弹,敌人，画敌方子弹
		run();
		break;
	case End:
		end();
		break;
	}
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);
	ValidateRect(&m_client);
	m_cacheDC.DeleteDC();
	m_cacheCBitmap.DeleteObject();
	ReleaseDC(cDC);
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

void TransparentPNG(CImage *png)
{
	for (int i = 0; i <png->GetWidth(); i++)
	{
		for (int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i, j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}

void CChildView::draw_bg()
{
	if (m_gameBk.curx >= m_gameBk.fir.GetHeight())
	{
		m_gameBk.curx = 0;
		m_gameBk.isFir = m_gameBk.isFir ? 0 : 1;
	}
	int height = m_gameBk.fir.GetHeight();
	if (m_gameBk.isFir)
	{
		m_gameBk.fir.BitBlt(m_cacheDC,
			0, m_gameBk.curx, SRCCOPY);

		m_gameBk.sec.BitBlt(m_cacheDC,
			0, m_gameBk.curx - height, SRCCOPY);
	}
	else
	{
		m_gameBk.sec.BitBlt(m_cacheDC,
			0, m_gameBk.curx, SRCCOPY);

		m_gameBk.fir.BitBlt(m_cacheDC,
			0, m_gameBk.curx - height, SRCCOPY);
	}
}
bool Plane::move_up()
{
	if (Pos.top - speed > 1)
	{
		Pos.top -= speed;
		Pos.bottom -= speed;
		return true;
	}
	return false;
}
bool Plane::move_down()
{
	if (Pos.bottom + speed < HEIGHT)
	{
		Pos.bottom += speed;
		Pos.top += speed;
		return true;
	}
	return false;
}
bool Plane::move_left()
{
	if (Pos.left - speed > 1)
	{
		Pos.left -= speed;
		Pos.right -= speed;
		return true;
	}
	return false;
}
bool Plane::move_right()
{
	if (Pos.right + speed < WIDTH)
	{
		Pos.right += speed;
		Pos.left += speed;
		return true;
	}
	return false;
}
bool Bullet::move_up()
{
	if (Pos.top - SPEED > 1)
	{
		Pos.top -= SPEED;
		Pos.bottom -= SPEED;
		return true;
	}
	return false;
}
bool Bullet::move_down()
{
	if (Pos.bottom + speed < HEIGHT)
	{
		Pos.bottom += speed;
		Pos.top += speed;
		return true;
	}
	return false;
}
bool Bullet::move_left()
{
	if (Pos.left - speed > 1)
	{
		Pos.left -= speed;
		Pos.right -= speed;
		return true;
	}
	return false;
}
bool Bullet::move_right()
{
	if (Pos.right + speed < WIDTH)
	{
		Pos.right += speed;
		Pos.left += speed;
		return true;
	}
	return false;
}
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
//	case 'd':
//	case 'D':
//		myplane->move_right();
//		break;
//	case 'a':
//	case 'A':
//		myplane->move_left();
//		break;
//	case 'w':
//	case 'W':
//		myplane->move_up();
//		break;
//	case 's':
//	case 'S':
//		myplane->move_down();
//		break;
	case VK_SPACE:
//		mybullet.push_back(Bullet(MYPLANE, (myplane->Pos.left + myplane->Pos.right) / 2, myplane->Pos.top + 5));
//		bullet = new Bullet(MYPLANE, (myplane->Pos.left + myplane->Pos.right) / 2, myplane->Pos.top + 5);
		for (size_t i = 0; i < mybullet.size(); i++)
		{
			if (mybullet[i].valid == false)
			{
				int x = (myplane->Pos.left + myplane->Pos.right) / 2;
				int y = myplane->Pos.top + 20;
				mybullet[i].Pos.left = x - 20;
				mybullet[i].Pos.right = x + 20;
				mybullet[i].Pos.top = y - 60;
				mybullet[i].Pos.bottom = y;
				mybullet[i].valid = true;
				PlaySound(TEXT("res\\attack.wav"),
					NULL, SND_FILENAME | SND_ASYNC);
				break;
			}
		}
		break;
	}
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER_PAINT:
		check_for_attack();
		OnPaint();
		
		break;
	case TIMER_BG:
		m_gameBk.curx += 1;
		for (size_t i = 0; i < mybullet.size(); i++)
		{
			if (mybullet[i].valid == true && !mybullet[i].move_up())
				mybullet[i].valid = false;
			if (enemy[i].valid == true && !enemy[i].move_down())
				enemy[i].valid = false;
			if (enemybullet[i].valid && !enemybullet[i].move_down())
				enemybullet[i].valid = false;
		}
		wait_for_key();
		break;
	case TIMER_ENEMY:
		for (size_t i = 0; i < enemy.size(); i++)
		{
			if (enemy[i].valid == true)
			{
				for (size_t j = 0; j < enemybullet.size(); j++)
				{
					if (!enemybullet[j].valid)
					{
						int x = (enemy[i].Pos.left + enemy[i].Pos.right) / 2;
						int y = enemy[i].Pos.bottom - 25;
						enemybullet[j].Pos.left = x - 20;
						enemybullet[j].Pos.right = x + 20;
						enemybullet[j].Pos.top = y;
						enemybullet[j].Pos.bottom = y + 60;
						enemybullet[j].valid = true;
						break;
					}
				}
			}
		}
		for (size_t i = 0; i < enemy.size(); i++)
		{
			if (enemy[i].valid == false)
			{
				enemy[i].Pos.top = 0;
				enemy[i].Pos.bottom = 60;
				enemy[i].Pos.left = rand() % 700;
				enemy[i].Pos.right = enemy[i].Pos.left + 60;
				enemy[i].life = 1;
				enemy[i].valid = true;
				break;
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	return 0;
}

void CChildView::check_for_attack()
{
	//检查我方子弹是否打中敌人
	for (size_t i = 0; i < mybullet.size(); i++)
	{
		if (mybullet[i].valid)
		{
			CRect r = mybullet[i].Pos;
			int index = -1;
			for (size_t j = 0; j < enemy.size(); j++)
			{
				if ((enemy[j].valid) &&
					(enemy[j].Pos.PtInRect(CPoint(r.left, r.top)) ||
					enemy[j].Pos.PtInRect(CPoint(r.left, r.bottom)) ||
					enemy[j].Pos.PtInRect(CPoint(r.right, r.bottom)) ||
					enemy[j].Pos.PtInRect(CPoint(r.right, r.top))
					))
				{
					index = j;
					break;
				}
			}
			if (index != -1)
			{
				if (--(enemy[index].life) <= 0)
					enemy[index].valid = false;
				mybullet[i].valid = false;
				m_score += 10;
			}
		}
	}
	//检查我方飞机是否被敌方子弹打中
	CRect temp = myplane->Pos;
	temp.left += 10;
	temp.right -= 10;
	temp.top += 10;
	temp.bottom -= 10;
	for (size_t i = 0; i < enemybullet.size(); i++)
	{
		if (enemybullet[i].valid)
		{
			CRect r = enemybullet[i].Pos;
			if (temp.PtInRect(CPoint(r.left, r.top)) ||
				temp.PtInRect(CPoint(r.left, r.bottom)) ||
				temp.PtInRect(CPoint(r.right, r.top)) ||
				temp.PtInRect(CPoint(r.right, r.bottom))
				)
			{
				if (--(myplane->life) <= 0)
				{
					game_state = End;
					end_game();
				}
				enemybullet[i].valid = false;
			}
		}
	}
}
void CChildView::start()
{
	CString str;
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -25;
	CFont font;
	font.CreateFontIndirectW(&lf);
	str.Format(_T("Designed by LBW"));
	m_cacheDC.SelectObject(font);
	m_cacheDC.SetTextColor(RGB(0x8a, 0x2b, 0xe2));
	m_cacheDC.SetBkMode(TRANSPARENT);
	m_cacheDC.TextOutW(550, 520, str);
	font.DeleteObject();
	start_png.Draw(m_cacheDC, start_pos);
	exit_png.Draw(m_cacheDC, exit_pos);
	airwars_png.Draw(m_cacheDC, airwars_pos);
}
void CChildView::run()
{
	for (size_t i = 0; i < mybullet.size(); i++)
	{
		if (mybullet[i].valid == true)
			mybullet[i].bullet.Draw(m_cacheDC, mybullet[i].Pos);
		if (enemy[i].valid == true)
			enemy[i].plane.Draw(m_cacheDC, enemy[i].Pos);
		if (enemybullet[i].valid)
			enemybullet[i].bullet.Draw(m_cacheDC, enemybullet[i].Pos);
	}
	//画我方飞机
	myplane->plane.Draw(m_cacheDC, myplane->Pos);

	//贴生命 分数
	CString str;
	CString str_life;
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -25;
	CFont font;
	font.CreateFontIndirectW(&lf);
	str.Format(_T("Score: %d"), m_score);
	str_life.Format(_T("Life: %d"), myplane->life);
	m_cacheDC.SelectObject(font);
	m_cacheDC.SetTextColor(RGB(255, 255, 255));
	m_cacheDC.SetBkMode(TRANSPARENT);
	m_cacheDC.TextOutW(650, 500, str);
	m_cacheDC.TextOutW(650, 530, str_life);
	font.DeleteObject();
}
void CChildView::end()
{
	//打印结束
	CString str;
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -25;
	CFont font;
	font.CreateFontIndirectW(&lf);
	str.Format(_T("Your final Score is : %d"), m_score);
	m_cacheDC.SelectObject(font);
	m_cacheDC.SetTextColor(RGB(0x8a, 0x2b, 0xe2));
	m_cacheDC.SetBkColor(RGB(0x87, 0xce, 0xfa));
	//m_cacheDC.SetBkMode(TRANSPARENT);
	m_cacheDC.TextOutW(250, 200, str);
	font.DeleteObject();
	again_png.Draw(m_cacheDC, again_pos);
	exit_png.Draw(m_cacheDC, exit_pos);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (game_state)
	{
	case Start:
		if (start_pos.PtInRect(point))
		{
			game_state = Running;
			initial_game();
		}
		if (exit_pos.PtInRect(point))
		{
			exit(0);
		}
		break;
	case End:
		if (again_pos.PtInRect(point))
		{
			game_state = Running;
			initial_game();
		}
		if (exit_pos.PtInRect(point))
		{
			exit(0);
		}
		break;
	}
	CWnd::OnLButtonDown(nFlags, point);
}
void CChildView::initial_game()
{
	//mybullet.resize(40);
	for (int i = 0; i < 40; i++)
	{
		mybullet[i].valid = false;
		enemy[i].valid = false;
		enemybullet[i].valid = false;
	}

	//enemy.resize(40);
	/*for (int i = 0; i < 40; i++)
	{
		enemy[i].type = ENEMYPLANE;
		enemy[i].speed = 2;
		enemy[i].life = 1;
		enemy[i].plane.Load(_T("res\\Enemy.png"));
		
	}*/

	/*enemybullet.resize(40);*/
	//for (int i = 0; i < 40; i++)
	//{
	//	enemybullet[i].type = ENEMYPLANE;
	//	enemybullet[i].valid = false;
	//	enemybullet[i].bullet.Load(_T("res\\enemybullet.png"));
	//	enemybullet[i].speed = 3;
	//}

	m_score = 0;
	myplane->life = 5;
	SetTimer(TIMER_PAINT, 10, NULL);
	SetTimer(TIMER_BG, 20, NULL);
	SetTimer(TIMER_ENEMY, 2000, NULL);
}
void CChildView::end_game()
{
	KillTimer(TIMER_PAINT);
	KillTimer(TIMER_BG);
	KillTimer(TIMER_ENEMY);
}

//void CChildView::OnClose()
//{
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
//
//	CWnd::OnClose();
//}


void CChildView::OnDestroy()
{
	CWnd::OnDestroy();
	end_game();

	start_png.ReleaseGDIPlus();
	exit_png.ReleaseGDIPlus();
	again_png.ReleaseGDIPlus();

	m_gameBk.fir.ReleaseGDIPlus();
	m_gameBk.sec.ReleaseGDIPlus();

	myplane->plane.ReleaseGDIPlus();


	// TODO:  在此处添加消息处理程序代码
}
void CChildView::wait_for_key()
{
	if (GetAsyncKeyState('W'))
		myplane->move_up();;
	if (GetAsyncKeyState('S'))
		myplane->move_down();
	if (GetAsyncKeyState('A'))
		myplane->move_left();
	if (GetAsyncKeyState('D'))
		myplane->move_right();
	/*if (GetAsyncKeyState(VK_SPACE))
	{
		for (size_t i = 0; i < mybullet.size(); i++)
		{
			if (mybullet[i].valid == false)
			{
				int x = (myplane->Pos.left + myplane->Pos.right) / 2;
				int y = myplane->Pos.top + 20;
				mybullet[i].Pos.left = x - 20;
				mybullet[i].Pos.right = x + 20;
				mybullet[i].Pos.top = y - 60;
				mybullet[i].Pos.bottom = y;
				mybullet[i].valid = true;
				PlaySound(TEXT("res\\attack.wav"),
					NULL, SND_FILENAME | SND_ASYNC);
				break;
			}
		}
	}*/
}
