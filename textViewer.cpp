#pragma warning(disable:4996)
#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <stack>
#include <string>
#include "textViewer.h"
#include "recent.h"
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <list>

textViewer::textViewer()
{
	strcpy(CDRIVE, "C:");
	strcpy(EXTENSION, "\\*.*");
	strcpy(slash, "\\");
	pos = 0;
	x = 0;
	y = 0;
}
void textViewer::reset()
{
	fil = 0;
	fol = 0;
	j = 0;
}

void textViewer::addFile(char* temp, char* name, int& size)
{
	if (isFileOrDir(temp))
	{
		if (strcmp(name, "."))
		{
			if (strcmp(name, ".."))
			{
				FOLDER[fol] = new char[strlen(name) + 1];
				strcpy(FOLDER[fol++], name);
				++size;
			}
		}
	}
	else
	{
		if (strcmp(GetFileExtenstion(name), "txt") == 0)
		{
			FILE[fil] = new char[strlen(name) + 1];
			strcpy(FILE[fil++], name);
			++size;
		}
	}
}

int textViewer::drawFile()
{
	int num = 0;
	char temp[1000];
	for (int i = y; i < fol; ++i)
	{
		if (i - y > 16){ break; }
		drawMenu(8, 4 + j, "◎", strlen("◎"));
		if (strlen(FOLDER[i])>34)
		{
			strcpy(temp, FOLDER[i]);
			strcpy(temp + 34, "...");
			drawMenu(11, 4 + j, temp, strlen(temp));
		}
		else
			drawMenu(11, 4 + j, FOLDER[i], strlen(FOLDER[i]));
		printTime(FOLDER[i], 50, 4 + j++);
		num++;
	}
	for (int i = 0; i <fil; ++i)
	{
		if (i + fol - y > 16){ break; }
		drawMenu(9, 4 + j, "*", strlen("*"));
		if (strlen(FILE[i])>34)
		{
			strcpy(temp, FILE[i]);
			strcpy(temp + 34, "...");
			drawMenu(11, 4 + j, temp, strlen(temp));
		}
		else
			drawMenu(11, 4 + j, FILE[i], strlen(FILE[i]));
		printTime(FILE[i], 50, 4 + j++);
		num++;
	}
	return num;
}

void textViewer::popBack()
{
	char temp[1000];
	int len;
	if (!s.empty())
	{
		x--;
		strcpy(temp, s.top());
		len = strlen(s.top());
		s.pop();
		temp[len - 4] = '\0';
		strcpy(CDRIVE, temp);
		pos = 0;
		y = 0;
	}
}

void textViewer::initScreen()
{
	for (int i = 0; i < 24 * 80; i++)
		screen[0][i] = ' ';
	screen[0][24 * 80 - 1] = '\0';
}

void textViewer::findFile(char* name, char* temp, int& size)
{
	if (isFileOrDir(temp))
	{
		if (strcmp(name, ".."))
		{
			FOLDER[fol] = new char[strlen(name) + 1];
			strcpy(FOLDER[fol++], name);
			++size;
		}
	}
	else
	{
		if (strcmp(GetFileExtenstion(name), "txt") == 0)
		{
			FILE[fil] = new char[strlen(name) + 1];
			strcpy(FILE[fil++], name);
			++size;
		}
	}
}

void textViewer::drawMenu(int x, int y, char* content, int width)
{
	memcpy(screen[y] + x, content, width);
}

void textViewer::showScreen()
{
	/*for (int i = 0; i < 24 * 80; ++i)
	putchar(screen[0][i]);*/
	puts(screen[0]);
}

char* textViewer::GetFileExtenstion(char * file_name)
{
	int file_name_len = strlen(file_name);
	file_name += file_name_len;

	char *file_ext = "";
	for (int i = 0; i <file_name_len; ++i)
	{
		if (*file_name == '.')
		{
			file_ext = file_name + 1;
			break;
		}

		file_name--;

	}
	return file_ext;
}

int textViewer::getKey()
{
	const int ext = 224;
	int ch;

	while (true){

		ch = getch();

		if (ch == ext)
		{
			ch = getch();
			if (ch == 75) return KEY_LEFT;
			if (ch == 77) return KEY_RIGHT;
			if (ch == 72) return KEY_UP;
			if (ch == 80) return KEY_DOWN;
			if (ch == 73) return KEY_PAGEUP;
			if (ch == 81) return KEY_PAGEDOWN;
			if (ch == 71) return KEY_HOME;
			if (ch == 79) return KEY_END;
		}
		else
		{
			if (ch == 27)				return KEY_QUIT;
			if (ch == '\r')				return KEY_ENTER;
			if (ch == '\b')				return KEY_BACK;
			if (ch == 60)				return F2;
			if (ch == 61)				return F3;
			if (ch == 62)				return F4;
		}
	}
}

void textViewer::drawBox(int x, int y, int width, int height)
{
	screen[y][x] = CH_TOPLEFT;
	drawCell(x + 1, y, width - 1, CH_HORIZONTAL);
	screen[y][x + width] = CH_TOPRIGHT;

	for (int i = 0; i < height; ++i)
	{
		screen[y + 1][x] = CH_VERTICAL;
		screen[y++ + 1][x + width] = CH_VERTICAL;
	}
	screen[y][x] = CH_BOTTOMLEFT;
	drawCell(x + 1, y, width - 1, CH_HORIZONTAL);
	screen[y][x + width] = CH_BOTTOMRIGHT;

}
void textViewer::drawCell(int x, int y, int size, char ch)
{
	for (int i = 0; i < size; i++)
		screen[y][x++] = ch;
}

int textViewer::isFileOrDir(char* s) {
	_finddatai64_t c_file;
	intptr_t hFile;
	int result;

	if ((hFile = _findfirsti64(s, &c_file)) == -1L)
		result = -1; // 파일 또는 디렉토리가 없으면 -1 반환
	else
		if (c_file.attrib & _A_SUBDIR)
			result = 1; // 디렉토리면 0 반환
		else
			result = 0; // 그밖의 경우는 "존재하는 파일"이기에 1 반환


	_findclose(hFile);

	return result;
}

void textViewer::helpMe()
{
	initScreen();
	system("cls");
	drawMenu(34, 4, "[ 단 축 키 ]", strlen("[ 단 축 키 ]"));
	drawMenu(29, 8, "Page Up : 한 페이지 위", strlen("Page Up : 한 페이지 위"));
	drawMenu(27, 10, "Page Down : 한 페이지 아래", strlen("Page Down : 한 페이지 아래"));
	drawMenu(34, 12, "↑ : 한 줄 위", strlen("↑ : 한 줄 위"));
	drawMenu(34, 14, "↓ : 한 줄 아래", strlen("↓ : 한 줄 아래"));
	drawMenu(32, 16, "Home : 첫 페이지", strlen("Home : 첫 페이지"));
	drawMenu(33, 18, "End : 마지막 페이지", strlen("End : 마지막 페이지"));
	drawMenu(31, 20, "Enter : 실행", strlen("Enter : 실행"));
	showScreen();
	getch();
}


void textViewer::printTime(char* filename, int x, int y) {
	struct _stat buf;
	char temp[1000];
	strcpy(temp, CDRIVE);
	strcat(temp, slash);
	strcat(temp, filename);
	if (_stat(temp, &buf) != 0) {
		switch (errno) {
		case ENOENT:
			fprintf(stderr, "File %s not found.\n", filename); break;
		case EINVAL:
			fprintf(stderr, "Invalid parameter to _stat.\n"); break;
		default:
			fprintf(stderr, "Unexpected error in _stat.\n");
		}
	}
	else {
		drawMenu(x, y, timeToString(localtime(&buf.st_mtime)), strlen(timeToString(localtime(&buf.st_mtime))));
		/*printf("%s\n", filename);
		printf("\tTime Creation     : %s\n", timeToString(localtime(&buf.st_ctime)));
		printf("\tTime Last Written : %s\n", timeToString(localtime(&buf.st_mtime)));
		printf("\tTime Last Access  : %s\n", timeToString(localtime(&buf.st_atime)));*/

	}


}

char* textViewer::timeToString(struct tm *t) {
	static char s[20];

	sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec
		);

	return s;
}

void textViewer::start()
{
	std::FILE* fp;
	fp = fopen("C:\\data_textViewer\\표지.txt", "r");
	initScreen();
	startEndLoad(fp);
	output(1);
	showScreen();
	wait(1);
	distroy();
	fclose(fp);
}

void textViewer::end()
{
	std::FILE* fp;
	fp = fopen("C:\\data_textViewer\\종료.txt", "r");
	initScreen();
	startEndLoad(fp);
	output(1);
	showScreen();
	wait(1);
	distroy();
	fclose(fp);
}

void textViewer::startEndLoad(std::FILE *fp)
{
	char *res, temp[1000];
	int num = 0;
	while (1)
	{
		res = fgets(temp, sizeof(temp), fp);
		if (res == NULL){ break; }
		temp[strlen(temp) - 1] = '\0';
		myMemcpy(temp);
	}

	return;
}

void textViewer::run()
{
	_finddata_t fd;
	long handle;
	int result;
	int size, key;
	char tname[1000], temp[100];
	char ttemp[100];
	recent r;
	while (1)
	{
		reset();
		totalAddr[x] = new char[strlen(CDRIVE) + strlen(EXTENSION)];
		strcpy(totalAddr[x], CDRIVE);
		strcat(totalAddr[x], EXTENSION);
		strcpy(temp, CDRIVE);
		initScreen();
		drawMenu(2, 1, "경로 :", 6);
		drawMenu(42, 1, "F2 : 단축키", 11);
		drawMenu(54, 1, "F3 : 최근문서", strlen("F3 : 최근문서"));
		drawMenu(68, 1, "ESC : 종료", strlen("ESC : 종료"));
		drawBox(1, 2, 77, 21);
		handle = _findfirst(totalAddr[x], &fd);  //현재 폴더 내 모든 파일을 찾는다.  
		size = 0;
		result = 1;
		addFile(temp, fd.name, size);
		if (strlen(temp) > 20)
		{
			strcpy(ttemp, temp);
			int len = strlen(ttemp);
			for (int i = 0; i < len; i++)
			{
				if (ttemp[i] == '\\')
				{
					strcpy(ttemp, ttemp + i);
					if (strlen(ttemp) <= 20)
						break;
				}
				else if (i == 27)
					memcpy(ttemp + i, "...", sizeof("..."));
			}
			drawMenu(10, 1, ttemp, strlen(ttemp));
		}
		else
			drawMenu(10, 1, temp, strlen(temp));

		for (int i = 0; i < y - 16; ++i)
		{
			result = _findnext(handle, &fd);
			if (result == -1)break;
			strcat(temp, slash);
			strcat(temp, fd.name);
			addFile(temp, fd.name, size);
		}

		if (handle == -1)
		{
			system("cls");
			showScreen();

			while (1)
			{
				if (getKey() == KEY_BACK)
				{
					popBack();
					break;
				}
			}
			continue;
		}
		while (result != -1)
		{
			result = _findnext(handle, &fd);
			if (result == -1)break;
			strcpy(temp, CDRIVE);
			strcat(temp, slash);
			strcat(temp, fd.name);
			findFile(fd.name, temp, size);
		}

		if (drawFile())
			drawMenu(4, pos + 4, "☞", strlen("☞"));
		_findclose(handle);
		system("cls");
		showScreen();
		key = getKey();
		if (key == KEY_QUIT)
		{
			end();
			wait(1);
			break;
		}
		else if (key == KEY_ENTER)
		{
			if (y + pos < fol)
			{
				strcpy(tname, FOLDER[y + pos]);
				strcpy(temp, slash);
				strcat(temp, tname);
				strcat(CDRIVE, temp);
				s.push(totalAddr[x++]);
			}
			else
			{
				strcpy(tname, FILE[y + pos - fol]);
				strcpy(temp, CDRIVE);
				strcat(temp, slash);
				strcat(temp, tname);
				dataLoad(temp, tname);
				r.recentUsed(temp, tname);
			}
			pos = 0;
			y = 0;
		}
		else if (key == KEY_BACK)
		{
			popBack();
		}
		else if (key == KEY_DOWN)
		{
			if (size > 16)
			{
				if (pos < 16)
					pos++;
				else
				{
					if (y + 16 < size - 1)
					{
						y++;
					}
				}
			}
			else
			{
				if (pos < size - 1)
					pos++;
			}

		}
		else if (key == KEY_UP)
		{
			if (pos == 0)
			{
				if (y > 0)
					y--;
			}
			else
				pos--;
		}
		else if (key == KEY_PAGEDOWN)
		{
			if (size > 16)
			{
				if (pos == 16)
				{
					if (y + 16 + 16 < size - 1)
					{
						y += 16;
					}
					else
					{
						y = size - 1 - 16;
					}
				}
				else
					pos = 16;
			}
			else
				pos = size - 1;
		}
		else if (key == KEY_PAGEUP)
		{
			if (pos == 0)
			{
				if (y - 16 > 0)
				{
					y -= 16;
				}
				else
					y = 0;
			}
			else
				pos = 0;
		}
		else if (key == KEY_HOME)
		{
			pos = 0;
			y = 0;
		}
		else if (key == KEY_END)
		{
			if (size > 16)
			{
				y = size - 1 - 16;
				pos = 16;
			}
			else
				pos = size - 1;
		}
		else if (key == F2)
			helpMe();
		else if (key == F3)
			r.printRecentUse();
		for (int i = 0; i < fol; ++i)
			delete[] FOLDER[i];
		for (int i = 0; i < fil; ++i)
			delete[] FILE[i];
	}
}

void textViewer::wait(int delay)
{
	clock_t goal = clock() + CLOCKS_PER_SEC*delay;
	while (goal > clock())
		;
}