#include <raylib.h>
#include <iostream>
#include <string>

enum State
{
	PLAYING,
	GAMEOVER
} state;

struct Snake
{
	int x;
	int y;
	int preX;
	int preY;
	Color color;
};

struct Apple
{
	int x;
	int y;
	Color color;
} apple;

int frames = 0;
int width = 600;
int height = 600;
int tileSize = 41;
int xDifference = width / tileSize;
int yDifference = height / tileSize;

const int snakeLength = 529;
int snakesCurrentLength = 0;
Snake segments[snakeLength];

int horizontal = 0;
int vertical = 0;
int speed = 10;

void input()
{
	if (IsKeyPressed(KEY_UP))
	{
		if (vertical == 1) { return; }
		horizontal = 0;
		vertical = -1;
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		if (vertical == -1) { return; }
		horizontal = 0;
		vertical = 1;
	}
	if (IsKeyPressed(KEY_LEFT))
	{
		if (horizontal == 1) { return; }
		horizontal = -1;
		vertical = 0;
	}
	if (IsKeyPressed(KEY_RIGHT))
	{
		if (horizontal == -1) { return; }
		horizontal = 1;
		vertical = 0;
	}
}

void draw()
{
	for (int x = 0; x < width / tileSize; x++)
	{
		for (int y = 0; y < height / tileSize; y++)
		{
			DrawRectangle(x * tileSize + xDifference, y * tileSize + yDifference * 1.5, tileSize - 1, tileSize - 1, WHITE);
		}
	}

	for (int i = 0; i <= snakesCurrentLength; i++)
	{
		DrawRectangle(segments[i].x * tileSize + xDifference, segments[i].y * tileSize + yDifference * 1.5, tileSize - 1, tileSize - 1, segments[i].color);
	}

	DrawRectangle(apple.x * tileSize + xDifference, apple.y * tileSize + yDifference * 1.5, tileSize - 1, tileSize - 1, apple.color);
}

void start()
{
	for (int i = 0; i <= snakesCurrentLength; i++)
	{
		segments[i].color = BLANK;
	}
	snakesCurrentLength = 0;
	state = PLAYING;

	segments[0].x = ((width / tileSize) / 2) - 4;
	segments[0].y = ((height / tileSize) / 2);
	segments[0].preX = 0;
	segments[0].preY = 0;
	segments[0].color = DARKGREEN;

	apple.x = ((width / tileSize) / 2) + 3;
	apple.y = ((height / tileSize) / 2);
	apple.color = RED;
}

void updateFrame()
{
	if (state != GAMEOVER)
	{
		for (int i = 0; i <= snakesCurrentLength; i++)
		{
			if (i == 0)
			{
				segments[i].preX = segments[i].x;
				segments[i].preY = segments[i].y;
				segments[i].x += horizontal;
				segments[i].y += vertical;
				if (segments[i].x > 13 || segments[i].x < 0 || segments[i].y < 0 || segments[i].y > 13)
				{
					state = GAMEOVER;
					segments[0].color = BLANK;
				}
			}
			else
			{
				segments[i].preX = segments[i].x;
				segments[i].preY = segments[i].y;
				segments[i].x = segments[i - 1].preX;
				segments[i].y = segments[i - 1].preY;
				segments[i].color = GREEN;
			}

			if (i != 0)
			{
				if (segments[0].x == segments[i].x && segments[0].y == segments[i].y)
				{
					state = GAMEOVER;
					segments[0].color = BLANK;
				}
			}
		}
	}

	if ((segments[0].x == apple.x) && (segments[0].y == apple.y))
	{
		apple.x = GetRandomValue(0, (width / tileSize) - 1);
		apple.y = GetRandomValue(0, (height / tileSize) - 1);
		for (int i = 0; i < snakesCurrentLength; i++)
		{
			if (apple.x == segments[i].x && apple.y == segments[i].y)
			{
				apple.x = GetRandomValue(0, (width / tileSize) - 1);
				apple.y = GetRandomValue(0, (height / tileSize) - 1);
			}
		}
		snakesCurrentLength++;
		if (snakesCurrentLength % 20 == 0)
		{
			speed--;
		}
	}
}

void updateDraw()
{
	if (state == GAMEOVER)
	{
		horizontal = 0;
		vertical = 0;

		std::string text = "Score: " + std::to_string(snakesCurrentLength) + " | " + "GAMEOVER - Spacebar";
		DrawText(text.c_str(), (width / 2) - 120, 3, 18, WHITE);

		draw();
		if (IsKeyDown(KEY_SPACE)) { start(); }
	}
	else
	{
		std::string text = "Score: " + std::to_string(snakesCurrentLength);
		DrawText(text.c_str(), (width / 2) - 30, 3, 18, WHITE);

		input();
		draw();
	}
}

int main()
{
	InitWindow(width, height, "Snake");
	SetTargetFPS(60);

	start();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		frames++;
		if (frames%speed == 0)
		{
			frames = 0;
			updateFrame();
		}

		updateDraw();

		EndDrawing();
	}

	return 0;
}