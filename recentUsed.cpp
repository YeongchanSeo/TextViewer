#pragma warning(disable: 4996)
#include "textViewer.h"
#include "recent.h"
#include <list>
#include <conio.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

void recent::recentUsed(char* str, char* tname)
{
	char* p;
	char* name;
	p = new char[strlen(str) + 1];
	name = new char[strlen(tname) + 1];
	strcpy(p, str);
	strcpy(name, tname);
	l.push_back(p);
	l2.push_back(name);
}

void recent::printRecentUse()
{
	int y, pos = 0;
	int x;
	char temp[1000];
	std::list<char*>::reverse_iterator rit;
	std::list<char*>::reverse_iterator rit2;
	while (1)
	{
		y = 0;
		system("cls");
		initScreen();
		drawMenu(56, 1, "F2 : 단축키", 11);
		drawMenu(68, 1, "ESC : 종료", strlen("ESC : 종료"));
		drawMenu(2, 1, "최근 실행한 문서", strlen("최근 실행한 문서"));
		drawBox(1, 2, 77, 21);

		int num = l.size();
		for (int i = 17; i < num; i++)
		{
			l.pop_front();
			l2.pop_front();
		}
		for (rit = l.rbegin(); rit != l.rend(); ++rit)
		{
			if (strlen(*rit)>34)
			{
				strcpy(temp, *rit);
				for (x = 30; x < 34; x++)
				{
					if (temp[x] == ' ')
					{
						strcpy(temp + x, "...");
						break;
					}
				}
				if (x == 34)
					strcpy(temp + 34, "...");
				drawMenu(8, 4 + y, temp, strlen(temp));
			}
			else
				drawMenu(8, 4 + y, *rit, strlen(*rit));
			printRecentTime(*rit, 50, 4 + y++);
		}
		if (y)
			drawMenu(4, pos + 4, "☞", strlen("☞"));
		showScreen();
		int key = getKey();
		if (key == KEY_ENTER)
		{
			if (!l.empty())
			{
				rit = l.rbegin();
				rit2 = l2.rbegin();
				for (int i = 0; i < pos; i++)
				{
					++rit;
					++rit2;
				}
				dataLoad(*rit, *rit2);
			}
		}
		else if (key == KEY_BACK)
			break;
		else if (key == KEY_DOWN)
		{
			if (pos < l.size() - 1)
				pos++;
		}
		else if (key == KEY_UP)
		{
			if (pos != 0)
				pos--;
		}
		else if (key == KEY_PAGEDOWN)
		{
			pos = l.size() - 1;
		}
		else if (key == KEY_PAGEUP)
		{
			pos = 0;
		}
		else if (key == KEY_HOME)
		{
			pos = 0;
		}
		else if (key == KEY_END)
		{
			pos = l.size() - 1;
		}
		else if (key == KEY_QUIT)
		{
			end();
			exit(0);
		}
		else if (key == F2)
			helpMe();
	}
}


void recent::printRecentTime(char* name, int x, int y) {
	struct _stat buf;

	if (_stat(name, &buf) != 0) {
		switch (errno) {
		case ENOENT:
			fprintf(stderr, "File not found.\n"); break;
		case EINVAL:
			fprintf(stderr, "Invalid parameter to _stat.\n"); break;
		default:
			fprintf(stderr, "Unexpected error in _stat.\n");
		}
	}
	else {
		drawMenu(x, y, timeToString(localtime(&buf.st_atime)), strlen(timeToString(localtime(&buf.st_atime))));
		/*printf("%s\n", filename);
		printf("\tTime Creation     : %s\n", timeToString(localtime(&buf.st_ctime)));
		printf("\tTime Last Written : %s\n", timeToString(localtime(&buf.st_mtime)));
		printf("\tTime Last Access  : %s\n", timeToString(localtime(&buf.st_atime)));*/
	}
}