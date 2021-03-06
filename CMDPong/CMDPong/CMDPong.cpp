#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>

using namespace std;

const int MAP_WIDTH = 70;
const int MAP_HEIGHT = 30;

struct paddle {
	int xpos = 0;
	int ypos = 0;
	int speed = 2;
	int halfWidth = 2;
	char image = '|';

	public : paddle(int x, int y)
	{
		ypos = y;
		xpos = x;
	}
};

struct point
{
	int x = 0;
	int y = 0;
};

struct ball {
	int xpos = 0;
	int ypos = 0;
	point moveVector;
	int speed = 1;
	char image = '@';
};

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void buildMap(paddle p1, paddle p2, ball b)
{
	setCursorPosition(0, 0);

	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			if (y <= p1.ypos + p1.halfWidth && y >= p1.ypos - p1.halfWidth && x == p1.xpos)
				cout << p1.image;
			else if (y <= p2.ypos + p2.halfWidth && y >= p2.ypos - p2.halfWidth && x == p2.xpos)
				cout << p2.image;
			else if (x == b.xpos && y == b.ypos)
				cout << b.image;
			else
				cout << " ";
		}
		cout << endl;
	}
}

void resetBall(ball &myBall);

void checkCollisionWithPaddle(ball &myBall, paddle &player2);

int main()
{
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 800, 800, TRUE);

	int scores[2];
	scores[0] = scores[1] = 0;

	paddle player1 = paddle(5, MAP_HEIGHT/2);
	paddle player2 = paddle(MAP_WIDTH-6, MAP_HEIGHT/2);
	ball myBall = ball();
	myBall.xpos = MAP_WIDTH / 2;
	myBall.ypos = MAP_HEIGHT / 2;

	while (true)
	{
		if (_kbhit())
		{
			char c = _getch();
			if (myBall.moveVector.x == 0 && myBall.moveVector.y == 0 && c == ' ')
			{
				int arr[] = { 1,-1 };
				myBall.moveVector.x = arr[rand() % 2];
				myBall.moveVector.y = arr[rand() % 2];
			}	
			else if (c == 's')
			{
				player1.ypos += 2;
				if (player1.ypos > MAP_HEIGHT - player1.halfWidth)
					player1.ypos = MAP_HEIGHT - player1.halfWidth;
			}
			else if (c == 'w')
			{
				player1.ypos -= 2;
				if (player1.ypos < player1.halfWidth)
					player1.ypos = player1.halfWidth;
			}
		}
		if (player2.ypos < myBall.ypos)
			player2.ypos += 1;
		else if (player2.ypos > myBall.ypos)
			player2.ypos -= 1;
		for (int y = 0; y < myBall.speed; ++y)
		{
			myBall.ypos += myBall.moveVector.y;

			if (myBall.ypos == 0 || myBall.ypos == MAP_HEIGHT - 1)
				myBall.moveVector.y *= -1;

			for (int x = 0; x < myBall.speed; ++x)
			{
				myBall.xpos += myBall.moveVector.x;

				if (myBall.xpos == 0) {
					resetBall(myBall);
					scores[1]++;
				}
				else if (myBall.xpos == MAP_WIDTH - 1) {
					resetBall(myBall);
					scores[0]++;
				}

				checkCollisionWithPaddle(myBall, player1);
				checkCollisionWithPaddle(myBall, player2);
			}
		}

		buildMap(player1, player2, myBall);
		cout << "Player1: " << scores[0] << "\t\t\t" << "Player2: " << scores[1] << endl;
	}
}

void checkCollisionWithPaddle(ball &ball, paddle &player)
{
	if (ball.ypos <= player.ypos + player.halfWidth && ball.ypos >= player.ypos - player.halfWidth && ball.xpos == player.xpos)
		ball.moveVector.x *= -1;
}

void resetBall(ball &myBall)
{
	myBall.xpos = MAP_WIDTH / 2;
	myBall.ypos = MAP_HEIGHT / 2;
	myBall.moveVector.x = 0;
	myBall.moveVector.y = 0;
}
