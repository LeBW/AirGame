#include <list>
#include <vector>
#define MYPLANE 'm'
#define ENEMYPLANE 'e'
// ChildView.h : CChildView ��Ľӿ�
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

// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	friend class Plane;
	CRect m_client;
	/*CRect m_myplanePos;
	CImage m_myplane;*/
	//����
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;
	//��Ϸ��ʼ���������
	CImage start_png;
	CRect start_pos;

	CImage exit_png;
	CRect exit_pos;

	CImage again_png;
	CRect again_pos;

	CImage airwars_png;
	CRect airwars_pos;
	//��Ϸ��ʼ�е�����
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
// ����
public:
	void draw_bg();	//������
	void check_for_attack();   //�����ײ
	void run();	//����ʱ�Ļ��溯��
	void start(); //��ʼʱ�Ļ��溯��
	void end();//����ʱ�Ļ��溯��
	void initial_game();//��ʼ����Ϸ���ݵĺ���
	void end_game();//������Ϸ
	void pause_game();//��ͣ��Ϸ
	void go_on_game();//������Ϸ
	void wait_for_key();
// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
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



