#include "textViewer.h"
#include <conio.h>

void textViewer::dataLoad(char* str, char* name)
{
	std::FILE* fp;

	int totalNum;
	int colNum;
	fp = fopen(str, "r");
	colNum = 1;
	totalNum = fileLoad(fp);
	char buf[256];
	char temp[100];
	int num;
	while (1)
	{
		sprintf(buf, "%2d /%2d", (colNum + 17) / 19 + 1, totalNum / 19 + 1);
		initScreen();
		drawBox(1, 1, 77, 21);
		drawMenu(2, 23, "파일명 : ", strlen("파일명 : "));
		drawMenu(36, 23, buf, strlen(buf));
		drawMenu(57, 23, "BackSpace : 뒤로 가기", strlen("BackSpace : 뒤로 가기"));
		if (strlen(name) > 20)
		{
			strcpy(temp, name);
			strcpy(temp + 20, "...");
			drawMenu(12, 23, temp, strlen(temp));
		}
		else
			drawMenu(12, 23, name, strlen(name));

		if (totalNum / 19 + 1 > 1)
		{
			drawBox(73, 2, 3, 19);
			drawMenu(74, 3, "▲", strlen("▲"));
			drawMenu(74, 20, "▼", strlen("▼"));
			if (18 / (totalNum / 19 + 1))
				drawBox(74, 4 + (15 - 18 / (totalNum / 19 + 1))*((double)((colNum + 17) / 19) / (totalNum / 19)), 1, 18 / (totalNum / 19 + 1));
			else
				drawBox(74, 4 + (14 - 18 / (totalNum / 19 + 1))*(((double)(colNum + 17) / 19) / (totalNum / 19)), 1, 1);
		}
		output(colNum);
		showScreen();
		num = inputCh(&colNum, totalNum);
		if (num == 1){
			fseek(fp, 0, SEEK_SET);
			divideScreen(str, name);
			fileLoad(fp);
		}
		else if (num == -1){ break; }
	}
	distroy();
	fclose(fp);
}

int textViewer::fileLoad(std::FILE* fp)
{
	char *res, temp[1000];
	int num = 0;
	while (1)
	{
		res = fgets(temp, sizeof(temp), fp);
		if (res == NULL){ break; }
		makeEnter(temp, num);
	}

	return num;
}

void textViewer::makeEnter(char* str, int& num)
{
	if (strlen(str) > 53)
	{
		char ttemp[1000];
		for (int i = 53; i < strlen(str); i++)
		{
			if (str[i] == ' ')
			{
				str[i] = '\0';
				strcpy(ttemp, str + i + 1);
				myMemcpy(str);
				num++;
				makeEnter(ttemp, num);
				break;
			}
			else if (str[i + 1] == '\0')
			{
				str[strlen(str) - 1] = '\0';
				myMemcpy(str);
				num++;
				return;
			}
		}
	}
	else
	{
		str[strlen(str) - 1] = '\0';
		myMemcpy(str);
		num++;
		return;
	}

}

void textViewer::output(int colNum)
{
	int i;
	int sy = 3;
	system("cls");
	std::list<char*>::iterator it;
	for (it = l.begin(), i = 1; i < colNum; ++i)
	{
		++it;
	}

	while (it != l.end() && colNum < 18 + i)
	{
		viewPrint(*it, &colNum, sy++);
		++it;
	}

	return;
}

void textViewer::viewPrint(void* p, int* colNum, int sy)
{
	drawMenu(6, sy, (char*)p, strlen((char*)p));
	*colNum += strlen((char*)p) / 80 + 1;
}

int textViewer::inputCh(int* colNum, int totalNum)
{
	char ch;
	ch = getKey();
	if (ch == KEY_BACK)return -1;
	switch (ch)
	{
	case KEY_UP: if (*colNum > 1){ (*colNum)--; } break;
	case KEY_DOWN: if (*colNum <= totalNum - 18){ (*colNum)++; } break;
	case KEY_PAGEUP: (*colNum<18) ? *colNum = 1 : *colNum -= 18; break;
	case KEY_PAGEDOWN: (*colNum + 18 > totalNum - 18) ? *colNum = totalNum - 17 : *colNum += 18; break;
	case KEY_HOME: *colNum = 1; break;
	case KEY_END: *colNum = totalNum - 17; break;
	case F4: return 1;  break;
	}

	return 0;
}

void textViewer::myMemcpy(char* temp)
{
	char* p;
	p = new char[strlen(temp)];
	strcpy(p, temp);
	l.push_back(p);
}

void textViewer::distroy()
{
	std::list<char*>::iterator it;
	while (!l.empty())
	{
		l.pop_front();
	}
}

