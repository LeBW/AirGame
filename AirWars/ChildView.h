#include <list>
#include <vector>
#define MYPLANE 'm'
#define ENEMYPLANE 'e'
// ChildView.h : CChildView 类的接口
//
class Plane{
public:
	friend class CChildView;
	char type;
	CRect Pos;
	int life;
	CImage plane;
	bool valid;
	int speed;
public:
	Plane(){

	}
	Plane(char t, int left, int top, int l, int sp){
		type = t;
		Pos.left = left;
		Pos.right = left + 80;
		Pos.top = top;
		Pos.bottom = top + 80;
		life = l;
		speed = sp;
	}
	bool move_up();
	bool move_down();
	bool move_left();
	bool move_right();
};
class	Bullet{
public:
	char type;
	CRect Pos;
	CImage bullet;
	bool valid;
	int speed;
public:
	bool move_up();
	bool move_down();
	bool move_left();
	bool move_right();
};
enum State{ Start, Running, Pause, End};
#pragma once

// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	friend class Plane;
	CRect m_client;
	/*CRect m_myplanePos;
	CImage m_myplane;*/
	//缓存
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;
	//游戏开始界面的数据
	CImage start_png;
	CRect start_pos;

	CImage exit_png;
	CRect exit_pos;

	CImage again_png;
	CRect again_pos;

	CImage airwars_png;
	CRect airwars_pos;
	//游戏开始中的数据
	Plane *myplane = new Plane(MYPLANE, 100, 500, 5, 10);
	std::vector<Bullet> mybullet;
	std::vector<Plane> enemy;
	std::vector<Bullet> enemybullet;
	State game_state;
	int m_score;
	struct gbk{
		CImage fir;
		CImage sec;
		BOOL isFir;
		int curx;
	}m_gameBk;
// 操作
public:
	void draw_bg();	//贴背景
	void check_for_attack();   //检测碰撞
	void run();	//运行时的画面函数
	void start(); //开始时的画面函数
	void end();//结束时的画面函数
	void initial_game();//初始化游戏数据的函数
	void end_game();//结束游戏
	void pause_game();//暂停游戏
	void go_on_game();//继续游戏
	void wait_for_key();
// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
};
void TransparentPNG(CImage *png);



