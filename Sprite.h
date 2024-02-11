#pragma once
#include "Framework.h"

class Sprite {

public:

	int width;
	int height;
	int maxXPos;
	int maxYPos;
	int xPos;
	int yPos;
	float dxPos;
	float dyPos;
	float angle;
	int score;
	int distance;

public:
	Sprite(int x, int y);

	Sprite();

	int getXPos();
	int getYPos();

	void setXPos(int x);
	void setYPos(int y);

	int getScore() const;
	void setScore(int newScore);
};