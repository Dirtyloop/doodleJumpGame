#include <windows.h> 
#include <stdio.h> 
#include "Sprite.h"
#include "Framework.h"

width = 100;
height = 100;
maxXPos = 800;
maxYPos = 600;
xPos = 0;
yPos = 0;
dxPos = 0;
dyPos = 0;
angle = 0;
score = 0;
distance = 0;

Sprite::Sprite(int x, int y) {
	xPos = x;
	yPos = y;
};

Sprite::Sprite() {
	xPos = 0;
	yPos = 0;
};

int Sprite::getXPos() {
	return xPos;
}

int Sprite::getYPos() {
	return yPos;
}

void Sprite::setXPos(int x) {
	xPos = x;
}

void Sprite::setYPos(int y) {
	yPos = y;
}

int Sprite::getScore() const {
	return score;
}

void Sprite::setScore(int newScore) {
	score += newScore;
}