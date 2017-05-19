#pragma once

class recent :public textViewer
{
private:
	std::list<char*> l;
	std::list<char*> l2;
public:
	void recentUsed(char* str, char* tname);
	void printRecentUse();
	void printRecentTime(char* name, int x, int y);
};