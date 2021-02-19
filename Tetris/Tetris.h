//ͷ�ļ�����
//non-standard libaray
#include <easyx.h>
#include <conio.h>
#include <windows.h>
//standard libaray
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <random>
#include <vector>
#include <chrono>
#include <filesystem>
using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

const wchar_t info[] = //������ֲ�������VS2015 wcout���
LR"raw_string(Tetris����˹����
���ߣ�Yaossg����ҪԴ���롢���������������ϵ����
���� C++17 ��дʹ�� EasyX �⣬�漰�� MFC��ʼ��Ϊ���߳�)raw_string";

//�궨�岿��
#define CELL 20

#define MWIDTH 10
#define MHEIGHT 22

#define AWIDTH 4
#define AHEIGHT 4
/*
ɨ�����������ֽڣ���ASCII���й�ϵ��ASCII����ж�Ӧ���ַ�
��ɨ����ĵ�һ���ֽھ���ASCII�룬�ڶ����ֽھ���0
�ַ�	��һ���ֽ�	�ڶ����ֽ�
������		224			72
������		224			80
������		224			75
������		224			77
*/
#define DIR 224
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define ESC 27
#define SCREENSHOT 233

#define LOST 2
#define DEF -1

#define SCORELISTSIZE 10

#define AS_BLOCK(x, y) (CELL * (x)), (CELL * (y)), \
	(CELL * ((x) + 1) - 1), (CELL * ((y) + 1) - 1)

#define SAVES_AVAILABLE fs::exists(score.name + TEXT("save"))

#define FORBLOCK(init, height, width, code)			\
{													\
	WORD num = init;								\
	for (int height = 0; height < AHEIGHT; height++)\
		for (int width = 0; width < AWIDTH; width++)\
		{											\
			if (0x8000 & num)						\
			{										\
				code;								\
			}										\
			num <<= 1;								\
		}											\
}

enum : COLORREF {
	red = BGR(0xFF2222),
	orange = BGR(0xFF9922),
	yellow = BGR(0xFFFF22),
	green = BGR(0x22FF22),
	cyan = BGR(0x2299FF),
	blue = BGR(0x2222FF),
	purple = BGR(0xFF22FF)
};

struct Blocks
{
	WORD form[4];
	DWORD color;
};
extern bool gamezone[MHEIGHT][MWIDTH];

//�ṹ����������
struct BlockInfo
{
	short x, y, t, s;

	BlockInfo leftMove()const
	{
		return{ x - 1, y, t, s };
	}
	BlockInfo rightMove()const
	{
		return{ x + 1, y, t, s };
	}
	BlockInfo upRotate()const
	{
		return{ x, y, t, (s + 1) % 4 };
	}
	BlockInfo downFall()const
	{
		return{ x, y + 1, t, s };
	}
};

struct ScoreInfo
{
	ScoreInfo() : score(0), name(TEXT("-")),
		lineCleared(0), difficulty(1){}
	int64_t score;
	int32_t lineCleared;
	int16_t difficulty;
	enum { max_difficulty = 64 };
	wstring name;
	system_clock::time_point start_time = system_clock::now();
	int64_t big_count = 0;
	void clearLine(int times = 1)
	{
		if (times != 0)
		{
			lineCleared += times;
			int count = difficulty * times;
			for (int i = 1; i < times; i++)
				count += (difficulty * i + 1) / 2;
			score += count;
			system_clock::time_point stop_time = system_clock::now();
			int64_t dur_time = duration_cast<seconds>(stop_time - start_time).count();
			big_count += count * count;
			if ( difficulty <= max_difficulty
				&&(dur_time + big_count + score + lineCleared
				> int64_t(difficulty + 1) * (difficulty + 2) * (difficulty + 3)))
			{
				++difficulty;
				big_count /= difficulty + 1;
				start_time = stop_time;
			}
		}
	}
};
using ScoreList = vector <ScoreInfo>;

struct BatchDrawer
{
	BatchDrawer()
	{
		BeginBatchDraw();
	}
	~BatchDrawer()
	{
		EndBatchDraw();
	}
};

struct Button
{
	void draw()const
	{
		setfillcolor(color);
		setbkcolor(color);
		fillrectangle(left, top, right, bottom);
		RECT r = { left, top, right, bottom };
		settextstyle(CELL, 0, NULL);
		drawtext(text.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	bool operator()()const
	{
		func();
		return repaint;
	}
	int left, top, right, bottom;
	COLORREF color;
	wstring text;
	void (*func)();
	bool repaint;
};

struct ButtonManager
{
	template<typename... Args>
	void emplace(Args&&... args)
	{
		buttons.push_back({args...});
	}
	void draw()const
	{
		BatchDrawer drawer;
		for (Button button : buttons)
			button.draw();
	}
	bool exec()
	{
		if (MouseHit())
		{
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg)
			{
			case WM_LBUTTONUP:
			{
				for (Button button : buttons)
				{
					if (button.left <= msg.x - CELL * 2
						&& button.right >= msg.x - CELL * 2
						&& button.top <= msg.y - CELL * 2
						&& button.bottom >= msg.y - CELL * 2)
					{
						FlushMouseMsgBuffer();
						repaint = button();
						return true;
					}
				}break;
			}
			}
		}
		return false;
	}
	bool repaint = true;
private:
	std::vector<Button> buttons;
};

//��ʼ��
void buildgraph(int x, int y);
void initEnviroment();

//���ƺ���
void drawBlocks(BlockInfo const& block);
void eraseBlocks(BlockInfo const& block);
void drawScore();

//����˹������ƶ�ϵ��
void moveBlock(BlockInfo(BlockInfo::* pmf)()const);
bool fallDown();

//�ж��ܷ��ƶ�
bool movableAndRotatable(BlockInfo const& block);
bool fallable(BlockInfo const& block);

//���ɷ���
bool spawnblocks();
void randomblocks();

//���ú��������
void setblock(BlockInfo const& block);
void clearLine();
bool blockfull(BlockInfo const& block);

//��Ϸ��ʼ
void newGame();
void oldGame();
int gameLost();

//���̿���
bool tryFall();
int getKey();
int execute();

//������
void initButtons();
void Tetris();
void showInfo();
void tryExit();

//�߷ְ�
void initScoreList();
void putScoreList();
void getScoreList(ScoreList& sorcelist);
void showScoreList();

//��Ϸ���������
void saveGame();
void loadGame();
void askName(wstring title);
