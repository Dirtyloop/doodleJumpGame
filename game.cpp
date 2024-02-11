#define _USE_MATH_DEFINES
#include <stdio.h> 
#include <iostream>
#include <string.h>
#include <vector>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include "Sprite.h"
#include "Framework.h"

int windowWidth;
int windowHeight;
bool LPressed = false;
bool RPressed = false;
int deltaTime = 0;
float speed = 1.0f;
const float minSpeed = 0.5f;
const float maxSpeed = 3.0f;
const float incSpeed = 1.15f;
const float decSpeed = 0.95f;
unsigned int tickCount = 0;
unsigned int lastTickCount = 0;
int edge;
int platformQuantity = 16;
std::vector<Sprite*> platforms(platformQuantity, nullptr);
std::vector<Sprite*> bullets;
Sprite* background;
int lastPlatform = -1;
int fakePlatform = -1;
bool isFakePlatform = true;
int enemyPlatform = -1;
int immunityTime = 20000;

class MyFramework : public Framework {

public:
	
	Sprite* Doodle;
	Sprite* Bullet;
	Sprite* Enemy;

	bool CreatePlatforms() {
		fakePlatform = rand() % platformQuantity;
		enemyPlatform = rand() % platformQuantity;

		for (int i = 0; i < platformQuantity; i++) {
			int x = rand() % (windowWidth - 50);
			int y = rand() % (windowHeight - 20);
			if (i == fakePlatform) {
				platforms[i] = createSprite("graph/fakePlatform.png");
			}
			else {
				platforms[i] = createSprite("graph/platform.png");
			}
			if ((Enemy) && (i == enemyPlatform)) {
				Enemy->xPos = x - 11;
				Enemy->yPos = y - 45;
			}
			platforms[i]->xPos = x;
			platforms[i]->yPos = y;
		}
		return true;
	}

	bool drawBullets(std::vector<Sprite*> &bullets) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]) {
				drawSprite(bullets[i], bullets[i]->xPos, bullets[i]->yPos);
			}
		}
		return true;
	}

	bool drawPlatforms(std::vector<Sprite*> &platforms) {
		for (int i = 0; i < platformQuantity; i++) {
			if (platforms[i]) {
				drawSprite(platforms[i], platforms[i]->xPos, platforms[i]->yPos);
			}
		}
		return true;
	}

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = windowWidth;
		height = windowHeight;
		fullscreen = false;
	}

	bool UpdateLocation(int x, int y) {
		Doodle->setXPos(x);
		Doodle->setYPos(y);
		return true;
	}

	virtual bool Init() {
		speed = 1;
		immunityTime = 20000;
		background = createSprite("graph/back.png");
		background->xPos = 0;
		background->yPos = 0;
		Doodle = createSprite("graph/soccer-right.png");
		Doodle->xPos = windowWidth/2 - 31;
		Doodle->yPos = windowHeight/2 - 30;
		Doodle->angle = 0;
		Doodle->score = 0;
		Doodle->distance = 0;
		CreatePlatforms();
		return true;
	}

	virtual void Close() {
		if (Doodle) {
			destroySprite(Doodle);
			destroySprite(background);
		}
	}

	virtual bool Tick() {
		drawSprite(background, background->xPos, background->yPos);
		lastTickCount = tickCount;
		tickCount = getTickCount();
		deltaTime = tickCount - lastTickCount;
		if (immunityTime > 0) {
			immunityTime -= deltaTime;
		}
		if(RPressed) Doodle->xPos += 1;
		if(LPressed) Doodle->xPos -= 1;
		if (Doodle) {
			Doodle->dyPos += 0.01 * speed * deltaTime;
			Doodle->yPos += Doodle->dyPos;
			if (Doodle->yPos > windowHeight - 60) {
				if (immunityTime > 0) {
					Doodle->dyPos = -3 * speed - 2;
				}
				else {
					destroySprite(Doodle);
					destroySprite(background);
					Init();
				}
			}
			if(Doodle->yPos<edge) {
				for (int i = 0; i < platformQuantity; i++) {
					Doodle->yPos = edge;
					platforms[i]->yPos -= Doodle->dyPos;
					Doodle->distance -= Doodle->dyPos;
					if ((Enemy) && (i == enemyPlatform)) {
						Enemy->yPos = platforms[i]->yPos - 45;
					}
					if (platforms[i]->yPos > windowHeight) {
						platforms[i]->yPos = 0;
						platforms[i]->xPos = rand() % (windowWidth - 50);
						if (i == enemyPlatform && Enemy) {
							Enemy = nullptr;
							enemyPlatform = rand() % platformQuantity;
						}
						if (i == enemyPlatform && !Enemy) {
							Enemy = createSprite("graph/enemy.png");
							Enemy->yPos = platforms[i]->yPos - 45;
							Enemy->xPos = platforms[i]->xPos - 11;
						}
					}
				}
			}
			if ((Enemy) && (Doodle->xPos + 50 > Enemy->xPos)
				&& (Doodle->xPos + 12 < Enemy->xPos + 78)
				&& (Doodle->yPos + 30 < Enemy->yPos)
				&& (Doodle->yPos + 30 > Enemy->yPos - 45)) {
				if (Doodle->dyPos > 0) {
					Enemy = nullptr;
				}
				else {
					destroySprite(Doodle);
					destroySprite(background);
					Init();
				}
			}
			for (int i = 0; i < platformQuantity; i++) {
				if (i != fakePlatform) {
					if ((Doodle->xPos + 50 > platforms[i]->xPos)
						&& (Doodle->xPos + 12 < platforms[i]->xPos + 56)
						&& (Doodle->yPos + 60 < platforms[i]->yPos)
						&& (Doodle->yPos + 60 > platforms[i]->yPos - 10)
						&& (Doodle->dyPos > 0)) {
						Doodle->dyPos = -3 * speed - 2;

						if (i == lastPlatform) {
							if (speed * incSpeed <= maxSpeed) speed *= incSpeed;
							else speed = maxSpeed;
						}
						else {
							if (speed * decSpeed >= minSpeed) speed *= decSpeed;
							else speed = minSpeed;
							Doodle->score++;
						}
						lastPlatform = i;
						std::cout << "Score: " << Doodle->score << std::endl;
						std::cout << "Distance: " << Doodle->distance / 100 << std::endl;
						std::cout << "Speed: " << speed << std::endl;
						std::cout << "Immunity Time: " << immunityTime << std::endl;
						std::cout << "Angle: " << Doodle->angle << std::endl;
					}
				}
			}
			if (Doodle->xPos > windowWidth) Doodle->xPos -= windowWidth;
			if (Doodle->xPos < 0) Doodle->xPos += windowWidth;

			drawSprite(Doodle, Doodle->xPos, Doodle->yPos);
			drawPlatforms(platforms);
			if (Enemy) {
				drawSprite(Enemy, Enemy->xPos, Enemy->yPos);
			}
			// Update locations of bullets
			for (int i = 0; i < bullets.size(); i++) {
				if (bullets[i]) {
					bullets[i]->dxPos = cosf(bullets[i]->angle) * 0.1 * deltaTime;
					bullets[i]->dyPos = sinf(bullets[i]->angle) * 0.1 * deltaTime;
					bullets[i]->xPos += bullets[i]->dxPos;
					bullets[i]->yPos += bullets[i]->dyPos;
				}
			}
			drawBullets(bullets);
			
		}
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		if (Doodle) {
			Doodle->angle = std::atan2(y - Doodle->yPos, x - Doodle->xPos);
		}
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		switch (button) {
		case FRMouseButton::LEFT: {
			if (isReleased) {
				// Create bullet and add it into vector
				Bullet = createSprite("graph/bubble.png");
				Bullet->angle = Doodle->angle;
				Bullet->xPos = Doodle->xPos;
				Bullet->yPos = Doodle->yPos;
				Bullet->dxPos = 0;
				Bullet->dyPos = 0;
				
				Bullet->score = 0;
				Bullet->distance = 0;
				bullets.push_back(Bullet);
				// For debugging purposes only
					std::cout << "Bullet added! " << std::endl;
					std::cout << "bullets size: "<< bullets.size() << std::endl;
			}
		} break;
		default:
			break;
		}
	}

	virtual void onKeyPressed(FRKey k) {
		switch (k) {
		case FRKey::RIGHT: {
			RPressed = true;
		} break;
		case FRKey::LEFT: {
			LPressed = true;
		} break;
		default:
			break;
		}
	}

	virtual void onKeyReleased(FRKey k) {
		switch (k) {
		case FRKey::RIGHT: {
			RPressed = false;
		} break;
		case FRKey::LEFT: {
			LPressed = false;
		} break;
		default:
			break;
		}
	}

	virtual const char* GetTitle() override
	{
		return "Doodle Jump by Dirtyloop";
	}
};

int main(int argc, char *argv[])
{
	if ((argc == 4) && (_stricmp(argv[1], "-window") == 0)) {
		windowWidth = atoi(argv[2]);
		windowHeight = atoi(argv[3]);
	}
	else {
		windowWidth = 640;
		windowHeight = 1024;
	}
	edge = windowHeight / 3;
	platformQuantity = windowHeight / 64;
	
	return run(new MyFramework);
}
