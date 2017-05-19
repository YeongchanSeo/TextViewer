#pragma once
#include <stack>
#include <list>
enum{
	KEY_LEFT = 1, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_QUIT,
	KEY_ENTER, KEY_BACK, KEY_PAGEUP, KEY_PAGEDOWN, KEY_HOME,
	KEY_END, F2, F4, F3
};

enum{
	CH_TOPLEFT = 1, CH_TOPRIGHT, CH_BOTTOMLEFT, CH_BOTTOMRIGHT
	, CH_VERTICAL, CH_HORIZONTAL
};

typedef char screen_t[24][80];

class textViewer
{
private:
	screen_t screen;
	std::list<char*> l;
	std::stack<char*> s;
	char CDRIVE[1000];
	char EXTENSION[1000];
	char slash[10];
	char* totalAddr[1000];
	char* FOLDER[1000];
	char* FILE[1000];
	int pos, fol, fil, j, x, y;
public:
	textViewer();
	void initScreen();
	void drawMenu(int x, int y, char* content, int width);
	void showScreen();
	char* GetFileExtenstion(char * file_name);
	int isFileOrDir(char* s);
	void helpMe();
	int getKey();
	void run();
	void drawBox(int x, int y, int width, int row);
	void drawCell(int x, int y, int size, char ch);
	void reset();
	void printTime(char* filename, int x, int y);
	char* timeToString(struct tm *t);
	void dataLoad(char* str, char* name);
	int fileLoad(std::FILE* fp);
	void viewPrint(void* p, int* colNum, int sy);
	void output(int colNum);
	int inputCh(int* colNum, int totalNum);
	void findFile(char* name, char* temp, int& size);
	void addFile(char* temp, char* name, int& size);
	int drawFile();
	void popBack();
	void myMemcpy(char* temp);
	void distroy();
	void makeEnter(char* str, int& num);
	void divideScreen(char* str, char* name);
	int dividedFileLoad(std::FILE* fp);
	void dividedMakeEnter(char* str, int& num);
	void dividedOutput(int colNum);
	void dividedViewPrint(void* p, int* colNum, int sy);
	void start();
	void end();
	void wait(int delay);
	void startEndLoad(std::FILE* fp);
};