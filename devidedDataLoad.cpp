#pragma warning (disable:4996)
#include "textViewer.h"
#include <conio.h>

void textViewer::divideScreen(char* str, char* name)
{
	std::FILE* fp;
	int colNum = 1;
	int num;
	int totalNum;
	char buf[256];
	char temp[100];
	distroy();
	fp = fopen(str, "r");
	totalNum = dividedFileLoad(fp);
	while (1)
	{
		initScreen();
		sprintf(buf, "%2d /%2d", (colNum + 17) / 19 + 1, totalNum / 19 + 1);
		drawBox(1, 1, 38, 21);
		drawBox(40, 1, 38, 21);
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
		dividedOutput(colNum);
		showScreen();
		num = inputCh(&colNum, totalNum);
		if (num){ break; }
	}
	distroy();
	fclose(fp);
}

int textViewer::dividedFileLoad(std::FILE* fp)
{
	char *res, temp[1000];
	int num = 0;
	while (1)
	{
		res = fgets(temp, sizeof(temp), fp);
		if (res == NULL){ break; }
		dividedMakeEnter(temp, num);
	}

	return num;
}

void textViewer::dividedMakeEnter(char* str, int& num)
{
	if (strlen(str) > 15)
	{
		char ttemp[1000];

		for (int i = 15; i < strlen(str); i++)
		{
			if (str[i] == ' ')
			{
				str[i] = '\0';
				strcpy(ttemp, str + i + 1);
				myMemcpy(str);
				num++;
				dividedMakeEnter(ttemp, num);
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

void textViewer::dividedOutput(int colNum)
{
	int i;
	int sy = 3;
	int dsy = 3;
	int tempNum;
	system("cls");
	std::list<char*>::iterator it;
	for (it = l.begin(), i = 1; i < colNum; ++i)
	{
		++it;
	}

	tempNum = colNum;

	while (it != l.end() && colNum < 18 + i)
	{
		viewPrint(*it, &colNum, sy++);
		++it;
	}

	while (it != l.end() && tempNum < 18 + i)
	{
		dividedViewPrint(*it, &tempNum, dsy++);
		++it;
		//colNum++;
	}
	return;
}

void textViewer::dividedViewPrint(void* p, int* colNum, int sy)
{
	drawMenu(45, sy, (char*)p, strlen((char*)p));
	*colNum += strlen((char*)p) / 80 + 1;
}