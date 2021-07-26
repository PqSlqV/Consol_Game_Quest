#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<locale.h>


#define mapheight 25
#define mapwidth 80
#define Knpc 5
#define Kobj 4


struct
{
	char map[mapheight][mapwidth + 1];
	POINT size;
}loc;
struct 
{
	char name[20];
	POINT pos;
	int xloc, yloc;
	char inv[20][20];
	int q,d;
	int acc;
	int get;
	char quest[30];
}player; 
struct
{
	char name[20];
	POINT pos;
	int xloc, yloc;
}NPC[Knpc+1];
struct
{
	char name;
	POINT pos;
	int xloc, yloc;
}Obj[Kobj];

char map[mapheight][mapwidth + 1];
void Dialog(int);

void locloadmap()
{
	char filename[20];
	sprintf(filename,"map_%d_%d.txt", player.yloc, player.xloc);
	memset(&loc.map,' ', sizeof(loc));
	for (int i = 0; i < mapheight; i++)
		loc.map[i][mapwidth] = '\0';

	FILE* f = fopen(filename, "r");
	if (f == NULL)
		exit(0);
	char c[mapwidth];
	int line = 0;
	loc.size.x = 0;
	while (!feof(f))
	{
		fgets(c, mapwidth, f);
		
			int cnt = strlen(c);
			if (c[cnt - 1] == '\n') cnt--;
			strncpy(loc.map[line], c, cnt);
			if(cnt>loc.size.x)loc.size.x = cnt;
			line++;
		
	}
	loc.size.y = line;
	fclose(f);
}

void locPutonmap()
{
	memcpy(map, loc.map, sizeof(map));
}

void Player_control()
{
	POINT temp = player.pos;
	if (GetKeyState('A') < 0) player.pos.x += -1;
	if (GetKeyState('D') < 0) player.pos.x += 1;
	if (GetKeyState('W') < 0) player.pos.y += -1;
	if (GetKeyState('S') < 0) player.pos.y += 1;

	if ((player.pos.x == NPC[player.q].pos.x) && (player.pos.y == NPC[player.q].pos.y) && (player.xloc == NPC[player.q].xloc) && (player.yloc == NPC[player.q].yloc))
		Dialog(player.q);
	if ((player.pos.x == Obj[0].pos.x) && (player.pos.y == Obj[0].pos.y) && (player.xloc == Obj[0].xloc) && (player.yloc == Obj[0].yloc) && (Obj[0].name != ' '))
		Dialog(10);
	if ((player.pos.x == Obj[1].pos.x) && (player.pos.y == Obj[1].pos.y) && (player.xloc == Obj[1].xloc) && (player.yloc == Obj[1].yloc) && (Obj[1].name != ' '))
		Dialog(11);
	if ((map[player.pos.y][player.pos.x] == '@') && (player.get != 5))
	{
		for (int j = 0; j < 20; j++)
			if (player.inv[j][0] == ' ')
			{
				sprintf(player.inv[j], "������");
				player.get += 1;
				break;
			}
	}	
	if ((map[player.pos.y][player.pos.x] == '=') && (player.get != 5))
	{
		for (int j = 0; j < 20; j++)
			if (player.inv[j][0] == ' ')
			{
				sprintf(player.inv[j], "����");
				player.get += 1;
				Obj[3].pos.x += 4;
				break;
			}
	}
	if (map[player.pos.y][player.pos.x] != ' ')
		player.pos = temp;
	
	if (player.pos.x == (loc.size.x-1))
	{
		player.xloc++;
		locloadmap();
		player.pos.x = 1;
	}
	if (player.pos.x == 0)
	{
		player.xloc--;
		locloadmap();
		player.pos.x = loc.size.x - 2;
	}
	if (player.pos.y == (loc.size.y-1))
	{
		player.yloc++;
		locloadmap();
		player.pos.y = 1;
	}
	if (player.pos.y == 0)
	{
		player.yloc--;
		locloadmap();
		player.pos.y = loc.size.y - 2;
	}
}

void Player_putmap()
{
	map[player.pos.y][player.pos.x] = player.name[0];
}

void Show()
{
	for (int i = 0; i < mapheight; i++)
	{
		printf("%s", map[i]);
		if (i < 20)
			printf("\t%s", player.inv[i]);
		printf("\n");
	}
	printf("\n\t%s",player.quest);
}

void Curs()
{
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
	SetConsoleCursorPosition(handle, coord);
}

void Player_init(char* name)
{
	sprintf(player.name, name);
	player.pos.x = 3;
	player.pos.y = 3;
	player.xloc = 0;
	player.yloc = 0;
	player.q = 0;
	player.d = 0;
	player.acc = 0;
	player.get = 0;
	sprintf(player.quest, " ");
	for (int i = 0; i < 20; i++)
		sprintf(player.inv[i]," ");
	
}

void Nps_init(int i, char* name, int x, int y, int xloc, int yloc)
{
	sprintf(NPC[i].name, name);
	NPC[i].pos.x = x;
	NPC[i].pos.y = y;
	NPC[i].xloc = xloc;
	NPC[i].yloc = yloc;
}

void Obj_init(int i, char* name, int x, int y, int xloc, int yloc)
{
	Obj[i].name = name;
	Obj[i].pos.x = x;
	Obj[i].pos.y = y;
	Obj[i].xloc = xloc;
	Obj[i].yloc = yloc;
}

void Init()
{
	Nps_init(0,"Laski",32,5,0,0);
	Nps_init(1,"Ryuu",10,9,1,0);
	Nps_init(2,"Nika",25,6,0,-1);
	Nps_init(3,"Mia",9,1,0,-1);
	Nps_init(4,"Angell",11,16,1,0);
	if (player.d == 0)
		Obj_init(0, '/', 29, 11, 0, 0);	
	else 
		Obj_init(0, ' ', 29, 11, 0, 0);
	if (player.d == 2)
		Obj_init(1, ' ', 41, 15, 0, 0);
	else
		Obj_init(1, '/', 41, 15, 0, 0);
	Obj_init(2, '@', 9, 13, 0, 0);
	Obj_init(3, '=', 23, 12, 1, 0);

}

void Nps_put()
{
	if(player.q < Knpc)
		if ((player.xloc == NPC[player.q].xloc) && (player.yloc == NPC[player.q].yloc))
			map[NPC[player.q].pos.y][NPC[player.q].pos.x] = NPC[player.q].name[0];
	for (int i = 0; i < Kobj; i++)
		if ((player.xloc == Obj[i].xloc) && (player.yloc == Obj[i].yloc))
		{
			if ((i == 0) && (player.d < 1))
				map[Obj[i].pos.y][Obj[i].pos.x] = Obj[i].name;
			if ((i == 1) && (player.d < 2))
				map[Obj[i].pos.y][Obj[i].pos.x] = Obj[i].name;

			if(player.q == 0)
				if ((player.acc == 1) && (i == 2))
				{
					for (int j = 0; j < 3; j++)
						for (int j1 = 0; j1 < 3; j1++)
						{
							map[Obj[i].pos.y + j][Obj[i].pos.x + j1] = Obj[i].name;
						}
				}
			if (player.q == 1)
				if ((player.acc == 1) && (i == 3) && (player.get != 5))
				{
					map[Obj[i].pos.y][Obj[i].pos.x] = Obj[i].name;
				}
		}
}

void clean_stdin(void)
{
	int c;
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

void Dialog(int d)
{
	system("cls");

	if (d == 0)
	{
		printf("\n\n\n\n\n\n\n\n\n\t������ %s, ���� ����� %s.", player.name, NPC[player.q].name);
		printf("\n\t� ��� ������������, ��� ���� �� ��������� ��� 5 �����, � ������ ����");
		if (player.acc == 0)
		{
			printf("\n\n\t\t1. ������, � ������� �� ����!");
			printf("\n\t\t2. ��� - ������ � ������ ���...");
			while (TRUE)
			{
				if (GetKeyState('1') < 0)
				{
					sprintf(player.quest, "����� ����� 5 �����");
					player.acc = 1;
					break;
				}
				if (GetKeyState('2') < 0)
					break;
				Sleep(100);
			}
		}
		else if (player.acc == 1)
		{

			if (player.get == 5)
			{
				printf("\n\n\t\t1. � ������ �� ����!");
				printf("\n\t\t2. ��� - ������ � ������ ���...");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						for (int i = 0; i < 20; i++)
							if (player.inv[i][0] == '�')
								sprintf(player.inv[i], " ");
						player.acc = 0;
						player.q = 1;
						player.get = 0;
						system("cls");
						printf("\n\n\n\n\n\n\n\n\n\t������� ����, %s. ������ � %s � �� ��������� ���� ��� ������", player.name, NPC[player.q].name);
						printf("\n\n\t\t1. ������!");
						while (TRUE)
						{
							if (GetKeyState('1') < 0)
								break;
							Sleep(100);
						}
						sprintf(player.quest, "����� %s", NPC[player.q].name);
						break;
					}
					if (GetKeyState('2') < 0)
						break;
					Sleep(100);
				}
			}
			else
			{
				printf("\n\n\t\t1. ����� �������");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}
		}

	}

	if (d == 1)
	{
		printf("\n\n\n\n\n\n\n\n\n\t������ %s, ���� ����� %s. ����� �� ������ �������� ����? ", player.name, NPC[player.q].name);
		printf("\n\t������� ��� ��� ���� �� ��������, � ��� ���� ������ ����");
		if (player.acc == 0)
		{
			printf("\n\n\t\t1. ������, � ������� �� ����!");
			printf("\n\t\t2. ��� - ������ � ������ ���...");
			while (TRUE)
			{
				if (GetKeyState('1') < 0)
				{
					sprintf(player.quest, "�������� ���� (5)");
					player.acc = 1;
					break;
				}
				if (GetKeyState('2') < 0)
					break;
				Sleep(100);
			}
		}
		else if (player.acc == 1)
		{

			if (player.get == 5)
			{
				printf("\n\n\t\t1. � ������ �� ����!");
				printf("\n\t\t2. ��� - ������ � ������ ���...");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						for (int i = 0; i < 20; i++)
							if (player.inv[i][0] == '�')
								sprintf(player.inv[i], " ");
						player.acc = 0;
						player.q = 2;
						player.get = 0;
						printf("\n\n\n\n\n\n\n\n\n\t������� ����, %s. ����� ���� ����. ������ ������ %s � �� ��������� ���� ��� ������", player.name, NPC[player.q].name);
						printf("\n\n\t\t1. �������!");
						sprintf(player.inv[0], "1. ���� �� �����");
						sprintf(player.inv[1], "������");
						sprintf(player.quest, "������� ������ %s", NPC[player.q].name);
						while (TRUE)
						{
							if (GetKeyState('1') < 0)
								break;
							Sleep(100);
						}
						break;
					}
					if (GetKeyState('2') < 0)
						break;
					Sleep(100);
				}
			}
			else
			{
				printf("\n\n\t\t1. ����� �������");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}

			}
		}

	}

	if (d == 2)
	{
		char ans[20];
		if (player.acc == 0)
		{
			printf("\n\n\n\n\n\n\n\n\n\t������ %s, ������� ��� ������ ��� ������. ���� ����� %s.", player.name, NPC[player.q].name);
			printf("\n\t������ �� ��� ������� ��������� � � ������ ����");
			sprintf(player.inv[1], " ");
			if (player.acc == 0)
			{
				printf("\n\n\t\t1. ������, � ���������� �� ��� ��������");
				printf("\n\t\t2. ��� - ������ � ������ ���...");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						sprintf(player.quest, "�������� �� �������");
						player.acc = 1;
						break;
					}
					if (GetKeyState('2') < 0)
					{
						sprintf(player.quest, " ");
						break;
					}
					Sleep(100);
				}
			}
		}
		if (player.acc == 1)
		{
			sprintf(ans, " ");
			clean_stdin();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\t�� ����, ������ �������.");
			printf("\n\t����� ��� �����, � ������� ���� ����� - ���������");
			printf("\n\t������ - ������, ������ - �������, ��������� ���������?");
			printf("\n\n\t\t��� ����� - ");
			fgets(ans, 20, stdin);
			if (((ans[0] == '�') || (ans[0] == '�')) && (ans[1] == '�') && (ans[2] == '�') && (ans[3] == '�'))
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������! ���������?");
				printf("\n\n\t\t1. ��");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						player.acc = 2;
						break;
					}
					Sleep(100);
				}
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������, �������� �������� � ����� ��� �����");
				printf("\n\n\t\t1.����");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}


		}
		if (player.acc == 2)
		{
			sprintf(ans, " ");
			clean_stdin();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\t������ �������");
			printf("\n\t� ����� ����� 5 \"e\" � ������� ������ �������?");
			printf("\n\n\t\t��� ����� - ");
			fgets(ans, 20, stdin);
			if (((ans[0] == '�') || (ans[0] == '�')) && (ans[1] == '�') && (ans[2] == '�') && (ans[3] == '�') && (ans[4] == '�') && (ans[5] == '�') && (ans[6] == '�') && (ans[7] == '�') && (ans[8] == '�') && (ans[9] == '�') && (ans[10] == '�'))
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������! ���������?");
				printf("\n\n\t\t1. ��");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						player.acc = 3;
						break;
					}
					Sleep(100);
				}
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������, �������� �������� � ����� ��� �����");
				printf("\n\n\t\t1.����");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}


		}
		if (player.acc == 3)
		{
			sprintf(ans, " ");
			clean_stdin();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\t��������� �������");
			printf("\n\t����� �������, � �� �� ������ ����� �� � �����, ���� ���� ��� ������� � ����. ��� ��� �����?");
			printf("\n\n\t\t��� ����� - �� ");
			fgets(ans, 20, stdin);
			if (((ans[0] == '�') || (ans[0] == '�')) && (ans[1] == '�') && (ans[2] == '�') && (ans[3] == '�') && (ans[4] == '�') && (ans[5] == '�') && (ans[6] == '�'))
			{
				system("cls");
				player.acc = 0;
				player.q = 3;
				player.get = 0;
				printf("\n\n\n\n\n\n\n\n\n\t�������, %s. ������ �� ������ ����������� � %s � �� ���� ���� ��������� ����", player.name, NPC[player.q].name);
				printf("\n\n\t\t1. ������");
				sprintf(player.quest, "������� � %s", NPC[player.q].name);
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������, �������� �������� � ����� ��� �����");
				printf("\n\n\t\t1.����");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}
		}


	}

	if (d == 3)
	{
		char ans[20];
		if (player.acc == 0)
		{
			printf("\n\n\n\n\n\n\n\n\n\t������ %s, � ������� ���� %s.", player.name, NPC[player.q].name);
			printf("\n\t���� ���� ����� ����, ������ �� ��� �������. ������?");
			if (player.acc == 0)
			{
				printf("\n\n\t\t1. ��");
				printf("\n\t\t2. ��� - ������ � ������ ���...");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						sprintf(player.quest, "�������� �� ������� %s", NPC[player.q].name);
						player.acc = 1;
						break;
					}
					if (GetKeyState('2') < 0)
						break;
					Sleep(100);
				}
			}
		}
		if (player.acc == 1)
		{

			sprintf(ans, " ");
			clean_stdin();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\t� ��� � ������ �������.");
			printf("\n\t��������, ����� ����� ��������� � ��������� ������������������ �����: 1, 2, 2, 4, 8, 11, �, 37, 148.");
			printf("\n\n\t\t��� ����� - ");
			fgets(ans, 20, stdin);
			if ((ans[0] == '3') && (ans[1] == '3'))
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������! ���������?");
				printf("\n\n\t\t1. ��");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						player.acc = 2;
						break;
					}
					Sleep(100);
				}
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������, �������� �������� � ����� ��� �����");
				printf("\n\n\t\t1. ����");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}


		}
		if (player.acc == 2)
		{
			sprintf(ans, " ");
			clean_stdin();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\t������ �������");
			printf("\n\t��������, ����� ����� ���������� ��� ������ �������: �, �, �, �, �, �, �, �, �, �, �, ?");
			printf("\n\n\t\t��� ����� - ");
			fgets(ans, 20, stdin);
			if ((ans[0] == '�') || (ans[0] == '�'))
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������! ���������?");
				printf("\n\n\t\t1. ��");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
					{
						player.acc = 3;
						break;
					}
					Sleep(100);
				}
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������, �������� �������� � ����� ��� �����");
				printf("\n\n\t\t1. ����");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}


		}
		if (player.acc == 3)
		{
			sprintf(ans, " ");
			clean_stdin();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\t��������� �������");
			printf("\n\t ����� ����� ������ ����� 5, 5, 6, 10, 9, 11, .. ?");
			printf("\n\n\t\t��� ����� - ");
			fgets(ans, 20, stdin);
			if ((ans[0] == '1') && (ans[1] == '1'))
			{
				system("cls");
				player.acc = 0;
				player.q = 4;
				player.get = 0;
				printf("\n\n\n\n\n\n\n\n\n\t�������, %s. ����� ���� ����. ������ �� ������� ����������� � %s", player.name, NPC[player.q].name);
				sprintf(player.inv[1], "2. ���� �� �����");
				printf("\n\n\t\t1. ������");
				sprintf(player.quest, "������� � %s", NPC[player.q].name);
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}
			else
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t�������, �������� �������� � ����� ��� �����");
				printf("\n\n\t\t1. ����");
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						break;
					Sleep(100);
				}
			}
		}


	}

	if (d == 4)
	{
		printf("\n\n\n\n\n\n\n\n\n\t������ %s, ���� ����� %s.", player.name, NPC[player.q].name);
		printf("\n\t� �������� ����. ���������� �� ���� ��� � �� �������-�� ������� ���������!");
		printf("\n\n\t\t1. ���!");
		while (TRUE)
		{
			if (GetKeyState('1') < 0)
			{
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\t���� ��������� ������ � ����� ������..");
				printf("\n\t��� ���� ����?");
				printf("\n\n\t\t1. �����!");
				printf("\n\n\t\t2. ���������, �� ������");
				printf("\n\n\t\t3. ������");
				Sleep(50);
				while (TRUE)
				{
					if (GetKeyState('1') < 0)
						Dialog(99);
					if (GetKeyState('2') < 0)
						Dialog(98);
					if (GetKeyState('3') < 0)
						exit(0);

					Sleep(100);
				}
			}
			Sleep(100);
		}
	}
	
	if (d == 10)
	{
		printf("\n\n\n\n\n\n\n\n\n\t����� ����");
		printf("\n\n\t\t1. ����������� �������");
		printf("\n\n\t\t2. ����");
		while (TRUE)
		{
			if ((GetKeyState('1') < 0) && (player.inv[0][0] == '1'))
			{
				Obj[0].name = ' ';
				sprintf(player.inv[0], " ");
				player.d = 1;
				break;
			}
			if ((GetKeyState('2') < 0))
				break;
			Sleep(100);
		}
	}

	if (d == 11)
	{
		printf("\n\n\n\n\n\n\n\n\n\t����� ����");
		printf("\n\n\t\t1. ����������� �������");
		printf("\n\n\t\t2. ����");
		while (TRUE)
		{
			if ((GetKeyState('1') < 0) && (player.inv[1][0] == '2'))
			{
				Obj[1].name = ' ';
				sprintf(player.inv[1], " ");
				player.d = 1;
				break;
			}
			if ((GetKeyState('2') < 0))
				break;
			Sleep(100);
		}
	}
	
	if (d == 98)
	{
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n\t%s, �� � ��� � �����, ������ ����..", player.name);
		printf("\n\n\t\t1. �����...");
		while (TRUE)
		{
			if (GetKeyState('1') < 0)
				exit(0);
			Sleep(100);
		}
	}
	if (d == 99)
	{
		system("cls");
		printf("\n\n\n\n\n\n\n\n\n\t%s, ��� ���� ����������� ������!����������!", player.name);
		printf("\n\t� ������ ���� �� �������� ���������, �� ������� ������ 1 ���������");
		printf("\n\n\t\t1. ���! �����...");
		while (TRUE)
		{
			if (GetKeyState('1') < 0)
				exit(0);
			Sleep(100);
		}
	}
	if (d == 111)
	{
		char s[20];
		printf("\n\t������� ���-�� ��������� ������ �����?");
		printf("\n\t��� ���� �����, �����?");
		printf("\n\n\t\t���� ����� - ");
		fgets(s, 20, stdin);
		for (int i = 0; s[i] != '\n'; i++)
			player.name[i] = s[i];
	}
	system("cls");
}

void Player_save()
{
	FILE* f = fopen(player.name, "wb");
	fwrite(&player, 1, sizeof(player), f);
	fclose(f);
}

void Player_load(char* name)
{
	FILE* f = fopen(name, "rb");
	if (f == NULL) Player_init(name);
	else 
	{
		fread(&player, 1, sizeof(player), f);
		fclose(f);
	}

}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Dialog(111);
	Player_load(player.name);
	locloadmap();
	Init();
	do
	{
		Curs();
		Player_control();
		locPutonmap();
		Player_putmap();
		Nps_put();
		Show();
		Sleep(50);
	} while (GetKeyState(VK_ESCAPE) >= 0);
	Player_save();
	exit(0);
}