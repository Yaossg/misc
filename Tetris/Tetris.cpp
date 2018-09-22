#include "Tetris.h"

//变量、数组、结构体
ScoreInfo score;
COLORREF BACKCOLOR = BLACK;
BlockInfo Cur, Nxt;
system_clock::time_point start_time;
ButtonManager mainMng, scoreMng;
bool gamezone[MHEIGHT][MWIDTH] = { false };

const Blocks Ts[7] = {
	{ 0x0F00, 0x4444, 0x0F00, 0x4444, red },	//I字形
	{ 0x0660, 0x0660, 0x0660, 0x0660, orange },	//方形
	{ 0x4460, 0x02E0, 0x0622, 0x0740, yellow },	//正L字形
	{ 0x2260, 0x0E20, 0x0644, 0x0470, green },	//倒L字形
	{ 0x0C60, 0x2640, 0x0C60, 0x2640, cyan },	//正Z字形
	{ 0x0360, 0x4620, 0x0360, 0x4620, blue },	//倒Z字形
	{ 0x04E0, 0x4C40, 0x0E40, 0x4640, purple },	//T字形
};

void buildgraph(int x, int y)
{
	setbkcolor(BACKCOLOR);
	cleardevice();
	Resize(NULL, CELL * x, CELL * y);
	setorigin(CELL * 8, CELL);
	drawScore();
	setorigin(0, 0);
}

/*该函数可以初始化游戏环境*/
void initEnviroment()
{
	buildgraph(30, 24);
	setorigin(CELL * 8, CELL);
	//主界面
	rectangle(-1, -1, CELL * MWIDTH, CELL * MHEIGHT);
	//预览区
	rectangle(CELL * (MWIDTH + 1) - 1, -1,
		CELL * (MWIDTH + AWIDTH + 1), CELL * AHEIGHT);
	memset(gamezone, false, sizeof(gamezone));
	randomblocks();
	RECT r = { CELL * MWIDTH + CELL, CELL * AHEIGHT + CELL * 4,
		CELL * MWIDTH + CELL * 8,
		CELL * AHEIGHT + CELL * 8 };
	drawtext(TEXT("按ESC退出"), &r, DT_LEFT | DT_VCENTER);
}

void drawBlocks(BlockInfo const& block)
{
	//设置颜色
	setfillcolor(Ts[block.t].color);
	setlinecolor(WHITE);
	FORBLOCK(Ts[block.t].form[block.s], i, j,
		fillrectangle(AS_BLOCK(block.x + j, block.y + i));
	);
}

void eraseBlocks(BlockInfo const& block)
{
	FORBLOCK(Ts[block.t].form[block.s], i, j,
		clearrectangle(AS_BLOCK(block.x + j, block.y + i));
	);
}

void drawScore()
{
	setbkcolor(BACKCOLOR);
	wstring ws = static_cast<wstringstream&>(wstringstream()
		<< TEXT("分数:") << score.score << endl
		<< TEXT("当前难度:") << score.difficulty).str();
	settextstyle(CELL, 0, TEXT("宋体"));
	RECT r = { CELL * MWIDTH + CELL, CELL * AHEIGHT + CELL,
		CELL * MWIDTH + CELL * 16, CELL * AHEIGHT + CELL * 8 };
	drawtext(ws.c_str(), &r, DT_LEFT | DT_VCENTER);
}

bool tryFall()
{
	system_clock::time_point stop_time = system_clock::now();
	unsigned long long dur_time =
		duration_cast<milliseconds>(stop_time - start_time).count();
	if (dur_time > 1350u / (1 + score.difficulty * 0.365))
	{
		start_time = stop_time;
		return true;
	}
	return false;
}

void moveBlock(BlockInfo (BlockInfo::* pmf)()const)
{
	BlockInfo Tmp = (Cur.*pmf)();
	if (movableAndRotatable(Tmp) && fallable(Tmp))
	{
		BatchDrawer drawer;
		eraseBlocks(Cur);
		drawBlocks(Cur = Tmp);
	}
}

bool fallDown()
{
	BlockInfo Tmp = Cur.downFall();
	BatchDrawer drawer;
	if (fallable(Tmp))
	{
		eraseBlocks(Cur);
		drawBlocks(Cur = Tmp);
		return true;
	}
	else
	{
		setblock(Cur);
		clearLine();
		return spawnblocks();
	}
}

bool movableAndRotatable(BlockInfo const& block)
{
	FORBLOCK(Ts[block.t].form[block.s], i, j, //判断是否超出左右边界
		if ((block.x + j >= MWIDTH) || (block.x + j < 0))
			return false;
	);
	return true;
}

bool fallable(BlockInfo const& block)
{
	FORBLOCK(Ts[block.t].form[block.s], i, j,
		if (block.y + i >= MHEIGHT || gamezone[block.y + i][block.x + j])
			return false;
	);
	return true;
}

bool spawnblocks()
{
	BatchDrawer drawer;
	eraseBlocks(Nxt);
	Cur.t = Nxt.t;
	Cur.s = Nxt.s;
	Cur.x = (MWIDTH - 4) / 2;
	Cur.y = 0;
	if (blockfull(Cur))
		return false;
	drawBlocks(Cur);
	randomblocks();
	drawBlocks(Nxt);
	return true;
}

void randomblocks()
{
	default_random_engine e(random_device{}());
	using dist = uniform_int_distribution<int>;
	Nxt.t = dist{ 0, 6 }(e);
	Nxt.s = dist{ 0, 3 }(e);
	Nxt.x = MWIDTH + 1;
	Nxt.y = 0;
}

void setblock(BlockInfo const& block)
{
	FORBLOCK(Ts[block.t].form[block.s], i, j,
		gamezone[Cur.y + i][Cur.x + j] = true;
	);
}

void clearLine()
{
	int i = MHEIGHT - 1, count = 0;
	while (i >= 0)
	{
		if (find(gamezone[i], gamezone[i + 1], false) == gamezone[i + 1])
		{
			IMAGE img;
			setfillcolor(BLACK);
			solidrectangle(0, CELL * i, CELL * MWIDTH - 1, CELL * (i + 1) - 1);
			for (int a = i; a > 0; a--)
				for (int b = 0; b < MWIDTH; b++)
					gamezone[a][b] = gamezone[a - 1][b];
			getimage(&img, 0, 0, CELL * MWIDTH, CELL * i);
			putimage(0, CELL, &img);
			++count;
		}
		else --i;
	}
	score.clearLine(count);
	drawScore();
}

bool blockfull(BlockInfo const& block)
{
	for (int i = 0; i < AHEIGHT; i++)
		for (int j = 3; j < 3 + AWIDTH; j++)
			if (gamezone[i][j])return true;
	return false;
}

int getKey()
{
	if (tryFall())return DOWN;
	if (_kbhit())//检测键盘是否有按键按下
	{
		switch (_getch())
		{
		case DIR:
			switch (_getch())
			{
			case UP:	return UP;
			case DOWN:	return DOWN;
			case LEFT:	return LEFT;
			case RIGHT:	return RIGHT;
			}break;
		case 'W': case 'w': return UP;
		case 'A': case 'a': return LEFT;
		case 'S': case 's': return DOWN;
		case 'D': case 'd': return RIGHT;
		case 'P': case 'p': return SCREENSHOT;
		case ESC:			return ESC;
		}
	}
	return DEF;
}

int execute()
{
	switch (getKey())
	{
	case UP:
		moveBlock(BlockInfo::upRotate);
		break;
	case LEFT:
		moveBlock(BlockInfo::leftMove);
		break;
	case RIGHT:
		moveBlock(BlockInfo::rightMove);
		break;
	case DOWN:
		if (!fallDown())
			return LOST;
		break;
	case ESC:
		switch (MessageBox(GetHWnd(), TEXT("\"是\":保存并退出\n\"否\":"
			"直接退出\"取消:\"不退出"), TEXT("您确定要退出并保存吗？"),
			MB_YESNOCANCEL | MB_ICONWARNING))
		{
		case IDYES:
			saveGame();
		case IDNO:
		default:
			return ESC;
		case IDCANCEL:
			;
		}break;
	case SCREENSHOT:
	{
		time_t t = time(nullptr);
		tm *tp = localtime(&t);
		saveimage(static_cast<wstringstream&>(wstringstream()
			<< "screenshot" << 1900 + tp->tm_year
			<< setw(2) << 1 + tp->tm_mon
			<< tp->tm_mday << tp->tm_hour
			<< tp->tm_min << tp->tm_sec
			<< ".bmp").str().c_str());
		MessageBox(GetHWnd(), TEXT("截图已保存"), TEXT("截图"), MB_OK);
	}break;
	}
	return DEF;
}

void initButtons()
{
	mainMng.emplace(CELL * 2, CELL, CELL * 10, CELL * 3,
		red, TEXT("新游戏"), newGame, true);
	mainMng.emplace(CELL * 2, CELL * 4, CELL * 10, CELL * 6,
		orange, TEXT("继续游戏"), oldGame, true);
	mainMng.emplace(CELL * 2, CELL * 7, CELL * 10, CELL * 9,
		cyan, TEXT("排行榜"), showScoreList, true);
	mainMng.emplace(CELL * 2, CELL * 10, CELL * 10, CELL * 12,
		purple, TEXT("关于"), showInfo, false);
	mainMng.emplace(CELL * 2, CELL * 13, CELL * 10, CELL * 15,
		blue, TEXT("退出"), tryExit, false);

	scoreMng.emplace(CELL * 3, CELL * 16, CELL * 8, CELL * 18,
		cyan, TEXT("返回"), [] {}, true);
}

void Tetris()
{
	while (true)
	{
		buildgraph(16, 20);
		setlinecolor(WHITE);
		clearrectangle(0, 0, CELL * 16, CELL * 20);
		RECT r = { 0, 0, CELL * 16, CELL * 2 - 1 };
		settextstyle(CELL * 2, 0, TEXT("宋体"));
		drawtext(TEXT("俄罗斯方块"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		setorigin(CELL * 2, CELL * 2);
		rectangle(-1, -1, CELL * 12, CELL * 16);
		mainMng.draw();
		do
			while( !mainMng.exec());
		while (!mainMng.repaint);
	}
}

void showInfo()
{
	MessageBox(GetHWnd(), info,
		TEXT("关于"), MB_OK | MB_ICONINFORMATION);
}

void tryExit()
{
	if (MessageBox(GetHWnd(), TEXT("您确定要退出吗?"),
		TEXT("退出"), MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		closegraph();
		exit(EXIT_SUCCESS);
	}
}

void newGame()
{
	start_time = system_clock::now();
	int ret;
	score.name = TEXT("");
	do
	{
		score.difficulty = 1;
		initEnviroment();
		spawnblocks();
		while ((ret = execute()) == DEF);
	} while (ret == LOST && gameLost() == IDYES);
}

void oldGame()
{
	askName(TEXT("加载游戏"));
	if (!SAVES_AVAILABLE)
	{
		MessageBox(GetHWnd(), TEXT("没有可用的存档"), TEXT("加载存档"), MB_OK);
		return;
	}
	start_time = system_clock::now();
	loadGame();
	fs::remove(score.name + TEXT("save"));
	fs::remove(score.name + TEXT("save.bmp"));
	int ret;
	while ((ret = execute()) == DEF);
	if (ret == LOST && gameLost() == IDYES)
		newGame();
}

int gameLost()
{
	if(score.name.empty())askName(TEXT("游戏结束"));
	putScoreList();
	return MessageBox(GetHWnd(), static_cast<wstringstream&>(
		wstringstream() << TEXT("分数:") << score.score
		<< endl << TEXT("还有再来一盘吗？")
		).str().c_str(), TEXT("游戏结束"), MB_YESNO | MB_ICONWARNING);
}

void initScoreList()
{
	wofstream file("ScoreList.txt");
	for (int i = 0; i < SCORELISTSIZE; i++)
		file << i << ' ' << TEXT("-") << ' ' << 0 << endl;
}

void putScoreList()
{
	ScoreList scorelist(SCORELISTSIZE);
	getScoreList(scorelist);
	scorelist.push_back(score);
	sort(scorelist.begin(), scorelist.end(),
		[](ScoreInfo const& a, ScoreInfo const& b) {
			return a.score > b.score;
	});
	scorelist.pop_back();
	wofstream file("ScoreList.txt");
	for (int i = 0; i < SCORELISTSIZE; i++)
		file << scorelist[i].name << '\t' << scorelist[i].score << endl;
}

void getScoreList(ScoreList& scorelist)
{
	wifstream file("ScoreList.txt");
	if (file.fail())
	{
		initScoreList();
		file.open("ScoreList.txt");
	}
	scorelist.resize(SCORELISTSIZE);
	for (int i = 0; i < SCORELISTSIZE; i++)
		file >> scorelist[i].name >> scorelist[i].score;
}

void showScoreList()
{
	buildgraph(16, 21);
	setfillcolor(WHITE);
	setlinecolor(WHITE);
	RECT r = { 0, 0, CELL * 16, CELL * 2 - 1 };
	settextstyle(CELL * 2, 0, TEXT("宋体"));
	drawtext(TEXT("排行榜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setorigin(CELL * 2, CELL * 2);
	rectangle(-1, -1, CELL * 12, CELL * 15);
	ScoreList scorelist;
	getScoreList(scorelist);
	r.right = CELL * 12;
	r.bottom = CELL;
	settextstyle(CELL, 0, NULL);
	for (int i = 0; i < SCORELISTSIZE; i++)
	{
		drawtext(static_cast<wstringstream&>(
			wstringstream() << left
			<< setw(3) << i + 1
			<< setw(10) << scorelist[i].name
			<< setw(8) << scorelist[i].score
			).str().c_str(), &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		r.bottom += CELL * 3;
	}
	scoreMng.draw();
	while (!scoreMng.exec());
}

void saveGame()
{
	if(score.name == TEXT(""))askName(TEXT("保存游戏"));
	fs::remove(score.name + TEXT("save"));
	fs::remove(score.name + TEXT("save.bmp"));
	IMAGE img;
	getimage(&img, 0, 0, CELL * 30, CELL * 24);
	wofstream file(score.name + TEXT("save"));
	file << hex;
	for (int i = 0; i < MHEIGHT; i++)
	{
		int line = 0;
		for (bool value : gamezone[i])
		{
			line |= value;
			line <<= 1;
		}
		file << line << ' ';
	}
	file << Cur.s << ' ' << Cur.t << ' ' << Cur.x << ' ' << Cur.y << ' '
		<< Nxt.s << ' ' << Nxt.t << ' '
		<< score.difficulty << ' ' << score.score << ' ' << score.name;
	saveimage((score.name + TEXT("save.bmp")).c_str(), &img);
}

void loadGame()
{
	wifstream file(score.name + TEXT("save"));
	buildgraph(30, 24);
	setorigin(8 * CELL, CELL);
	//主界面
	rectangle(-1, -1, CELL * MWIDTH, CELL * MHEIGHT);
	//预览区
	rectangle(CELL * (MWIDTH + 1) - 1, -1,
		CELL * (MWIDTH + AWIDTH + 1), CELL * AHEIGHT);
	loadimage(NULL, (score.name + TEXT("save.bmp")).c_str());
	file >> hex;
	for (int i = 0; i < MHEIGHT; i++)
	{
		int line = 0;
		file >> line;
		for (bool& value : gamezone[i])
		{
			value = line & 01000;
			line <<= 1;
		}
	}
	file >> Cur.s >> Cur.t >> Cur.x >> Cur.y
		>> Nxt.s >> Nxt.t;
	Nxt.x = MWIDTH + 1;
	Nxt.y = 0;
	file >> score.difficulty >> score.score >> score.name;
}

void askName(wstring title)
{
	enum { buf_size = 16 };
	wchar_t buf[buf_size];
	do
	{
		InputBox(buf, buf_size, TEXT("请输入昵称："), title.c_str());
		score.name = buf;
		if (score.name.empty())
			MessageBox(GetHWnd(), TEXT("您输入的昵称过短，请重新输入"),
				TEXT("昵称过短"), MB_OK);
		if (score.name.length() > 10)
			MessageBox(GetHWnd(), TEXT("您输入的昵称过长，请重新输入"),
				TEXT("昵称过长"), MB_OK);
	} while (score.name.length() > 10);
}
