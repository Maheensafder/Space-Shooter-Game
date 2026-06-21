#include <iostream>
#include "help.h"
#include <Windows.h>
#include <fstream>


//Initialising and Declaring Variables

using namespace std;
const int full_screen_width = 1300;
const int full_screen_height = 700;
const int alien_rows = 10;
const int alien_columns = 5;
const int alien_width = 30;
const int alien_height = 30;
int frameCounter = 0;
int highScore = 0;
int gridmovesx = 1250;
int gridmovesy = 70;
int initial_x = 200;
int initial_y = 100;
int a[5];
int maxScore;
int PlayerLives = 3;
int score = 0;
int gtime = 0;
int aliens_x[alien_rows][alien_columns];
int aliens_y[alien_rows][alien_columns];
bool aliens_alive[alien_rows][alien_columns];
void initializeAliens(int start_x, int start_y, int space_x, int space_y) {
	for (int i = 0; i < alien_rows; i++) {
		for (int j = 0; j < alien_columns; j++) {
			aliens_x[i][j] = start_x - i * space_x;
			aliens_y[i][j] = start_y + j * space_y;
			aliens_alive[i][j] = true;
		}
	}
}
void compressAlienRows()
{
	for (int j = 0; j < alien_columns; j++)
	{
		int aliveCount = 0;

		for (int i = 0; i < alien_rows; i++)
		{
			if (aliens_alive[i][j])
				aliveCount++;
		}

		int nextPos = alien_rows - 1;

		for (int i = alien_rows - 1; i >= 0; i--)
		{
			if (aliens_alive[i][j])
			{
				if (i != nextPos)
				{
					aliens_alive[nextPos][j] = true;
					aliens_x[nextPos][j] = aliens_x[i][j];
					aliens_y[nextPos][j] = aliens_y[i][j];

					aliens_alive[i][j] = false;
				}

				nextPos--;
			}
		}
	}
}
int getLeftMostAliveAlien()
{
	int leftMost = 99999;

	for (int i = 0; i < alien_rows; i++)
	{
		for (int j = 0; j < alien_columns; j++)
		{
			if (aliens_alive[i][j])
			{
				if (aliens_x[i][j] < leftMost)
					leftMost = aliens_x[i][j];
			}
		}
	}

	return leftMost;
}
int getTopMostAliveAlien()
{
	int topMost = 99999;

	for (int i = 0; i < alien_rows; i++)
	{
		for (int j = 0; j < alien_columns; j++)
		{
			if (aliens_alive[i][j])
			{
				if (aliens_y[i][j] < topMost)
					topMost = aliens_y[i][j];
			}
		}
	}

	return topMost;
}
int getBottomMostAliveAlien()
{
	int bottomMost = -99999;

	for (int i = 0; i < alien_rows; i++)
	{
		for (int j = 0; j < alien_columns; j++)
		{
			if (aliens_alive[i][j])
			{
				if (aliens_y[i][j] > bottomMost)
					bottomMost = aliens_y[i][j];
			}
		}
	}

	return bottomMost;
}

void updateVerticalDirection(int& directionofy, int topBound, int bottomBound)
{
	if (getBottomMostAliveAlien() >= bottomBound)
	{
		directionofy = -1;
	}
	else if (getTopMostAliveAlien() <= topBound)
	{
		directionofy = 1;
	}
}

//Checking for Collision between bullets and aliens
//Blacking aliens out once that happens

void checkBulletCollision(float& bullet_x, int& bullet_y, bool& bullet_active)
{
	if (!bullet_active)
		return;

	for (int j = 0; j < alien_columns; j++)     // for each row
	{
		// Find the frontmost living alien in this row
		int frontAlien = -1;

		for (int i = 0; i < alien_rows; i++)
		{
			if (aliens_alive[i][j])
			{
				if (frontAlien == -1 || aliens_x[i][j] < aliens_x[frontAlien][j])
					frontAlien = i;
			}
		}

		// Check collision only with that alien
		if (frontAlien != -1)
		{
			int i = frontAlien;

			if (bullet_x + 10 >= aliens_x[i][j] - alien_width / 2 &&
				bullet_x <= aliens_x[i][j] + alien_width / 2 &&
				bullet_y + 2 >= aliens_y[i][j] - alien_height / 2 &&
				bullet_y <= aliens_y[i][j] + alien_height / 2)
			{
				score += 10;

				aliens_alive[i][j] = false;

				bullet_active = false;
				bullet_x = -1;
				bullet_y = -1;

				return;
			}
		}
	}
}

//Drawing the alien

void drawAlien(int x, int y, int R, int G, int B) {

	myEllipse(x - 15, y - 15, x + 15, y + 15, R, G, B, R, G, B);
	myLine(x, y - 10, x, y - 30, R, G, B);
	myLine(x + 20, y, x + 30, y, R, G, B);
	myLine(x, y + 20, x, y + 30, R, G, B);
	myLine(x - 20, y, x - 30, y, R, G, B);


	myRect(x - 8, y - 4, x - 4, y, 0, 0, 0);
	myRect(x + 4, y - 4, x + 8, y, 0, 0, 0);
	myRect(x - 8, y + 4, x + 8, y + 10, 0, 0, 0);
}

//Making a grid of Aliens

void GridOfAliens(int x, int y, int R, int G, int B) {
	for (int i = 0; i < alien_rows; i++) {
		for (int j = 0; j < alien_columns; j++) {
			if (aliens_alive[i][j]) {
				drawAlien(aliens_x[i][j], aliens_y[i][j], R, G, B);
			}

		}
	}
}


//Making the fins of the spaceship

void fins(int x, int y, int z, int R = 0, int G = 0, int B = 0) {
	int top_x1 = x;
	int top_y1 = y;
	int top_x2 = x - z / 2;
	int top_y2 = y;
	int top_x3 = x;
	int top_y3 = y + z / 2;
	myLine(top_x1, top_y1, top_x2, top_y2, R, G, B);
	myLine(top_x2, top_y2, top_x3, top_y3, R, G, B);
	myLine(top_x3, top_y3, top_x1, top_y1, R, G, B);

	int bottom_x1 = x;
	int bottom_y1 = y + z;
	int bottom_x2 = x - z / 2;
	int bottom_y2 = y + z;
	int bottom_x3 = x;
	int bottom_y3 = y + z / 2;
	myLine(bottom_x1, bottom_y1, bottom_x2, bottom_y2, R, G, B);
	myLine(bottom_x2, bottom_y2, bottom_x3, bottom_y3, R, G, B);
	myLine(bottom_x3, bottom_y3, bottom_x1, bottom_y1, R, G, B);
}

// Checking if the aliens collide with the spaceship
// Decreasing lives at the same time

void aliencollisionwithspaceship(int xposition, int yposition, int widthofspaceship, int heightofspaceship) {
	for (int i = 0; i < alien_rows; i++) {
		for (int j = 0; j < alien_columns; j++) {
			if (aliens_alive[i][j]) {
				bool x_collision = (aliens_x[i][j] + alien_width / 2 > xposition - widthofspaceship / 2) &&
					(aliens_x[i][j] - alien_width / 2 < xposition + widthofspaceship / 2);
				bool y_collision = (aliens_y[i][j] + alien_height / 2 > yposition - heightofspaceship / 2) &&
					(aliens_y[i][j] - alien_height / 2 < yposition + heightofspaceship / 2);
				if (x_collision && y_collision)
				{
					PlayerLives--;

					aliens_alive[i][j] = false;

					// Respawn ship on left side
					initial_x = 200;
					initial_y = 100;

					if (PlayerLives <= 0)
					{


						myRect(20, 20,
							full_screen_width,
							full_screen_height,
							0, 0, 0);


						drawText(60, 400, 250, 180, 255, 180, "Game Over!");

						drawText(40, 500, 450, 180, 255, 180, "You Lose!");

						while (true)
						{
							Sleep(100);
						}
					}
				}
			}
		}
	}
}

// Making the head of the spaceship

void spaceship_head(int body_x, int body_y, int body_width, int body_height, int head_height, int R = 0, int G = 0, int B = 0) {

	int x1 = body_x + body_width;
	int y1 = body_y;

	int x2 = body_x + body_width;
	int y2 = body_y + body_height;

	int x3 = body_x + body_width + head_height;
	int y3 = body_y + body_height / 2;


	myLine(x1, y1, x2, y2, R, G, B);
	myLine(x2, y2, x3, y3, R, G, B);
	myLine(x3, y3, x1, y1, R, G, B);
}
void win() {
	if (score == 500) {
		drawText(30, 100, 450, 180, 255, 180, "YOU WIN!");
		drawText(20, 150, 200, 180, 255, 180, "Earth is safe, thanks to you!Humanity owes you its freedom!");
	}
}
void loadHighScore(const char* filename) {

	ifstream file(filename);
	if (file.is_open()) {
		file >> highScore;
		file.close();
	}
}
void saveHighScore(const char* filename) {
	ofstream file(filename);
	if (file.is_open()) {
		file << highScore;
		file.close();
	}
}
void updateHighScore(int currentScore, const char* filename) {
	if (currentScore > highScore) {
		highScore = currentScore;
		saveHighScore(filename);
	}
	cout << "High Score: " << highScore << endl; // Replace with drawText for your game display
}



// Main Program

int main() {
	myRect(20, 20, full_screen_width, full_screen_height, 207, 159, 255);                    // Making the big rectangle inwhich the game takes place

	// Declaring and Initially Variables



	int whichkey;
	bool previousSpaceState = false;
	const int MAX_BULLETS = 20;

	float bullet_x[MAX_BULLETS];
	int bullet_y[MAX_BULLETS];
	bool bullet_active[MAX_BULLETS];

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullet_active[i] = false;
		bullet_x[i] = -1;
		bullet_y[i] = -1;
	}
	int gridmovesx = 1250;
	int gridmovesy = 70;
	int speedofaliensx = 1;
	int directionofy = 1;
	int directionofx = -1;
	int speedofaliensy = 1;
	bool paused = false;
	int count = 0;

	
	const DWORD targetFrameTime = 16; // ~60 FPS (1000ms / 60 ≈ 16ms)
	DWORD frameStart;
	// ---------------------------------------------------------------------


	const int alien_visual_radius = 30;
	const int topBound = 20 + alien_visual_radius;
	const int bottomBound = full_screen_height - alien_visual_radius;
	const int leftBound = 20 + alien_visual_radius;
	const int rightBound = full_screen_width - alien_visual_radius;

	int maxScore = a[0];
	int xposition = 200;
	int yposition = 100;
	int heightofspaceship = 35;
	int widthofspaceship = 90;

	initializeAliens(1250, 70, 50, 100);
	const char* highScoreFile = "highscore.txt";
	int currentScore = score;
	loadHighScore(highScoreFile);





	while (true) {

		// Mark the start of this frame so we can pace it at the bottom of the loop.
		frameStart = GetTickCount();

		// When the escape key is pressed the game pauses
	   //When pressed again it resumes

		if (isKeyPressed(whichkey) && whichkey == 7) {
			paused = !paused;
			if (paused) {

				drawText(20, 200, 250, 250, 250, 250, "Game paused The aliens wont wait forever!");
				drawText(18, 350, 350, 250, 250, 250, "Press ESC again to resume");



				// Blacking the screen out once the game pauses

				myRect(initial_x, initial_y, initial_x + 60, initial_y + 30, 0, 0, 0);
				spaceship_head(initial_x, initial_y, 60, 30, 30, 0, 0, 0);
				fins(initial_x, initial_y, 30, 0, 0, 0);
				GridOfAliens(gridmovesx, gridmovesy, 0, 0, 0);
			}
			else {

				myRect(20, 20, full_screen_width, full_screen_height, 207, 159, 255);
			}
		}
		if (paused) {
			// Don't busy-spin while paused — that just keeps hammering
			// GetAsyncKeyState/GDI for no reason and contributes to the
			// same buffering problem. Sleep a bit between pause checks.
			Sleep(50);
			continue;
		}

		

		myRect(20, 20, full_screen_width, full_screen_height, 191, 64, 191);
		for (int i = 0; i < alien_rows; i++) {
			for (int j = 0; j < alien_columns; j++) {
				if (!aliens_alive[i][j])
					continue;

				aliens_x[i][j] += directionofx;
				aliens_y[i][j] += directionofy * speedofaliensy;

				// Clamp so no living alien is ever drawn outside the box
				if (aliens_y[i][j] < topBound)
					aliens_y[i][j] = topBound;
				if (aliens_y[i][j] > bottomBound)
					aliens_y[i][j] = bottomBound;
				if (aliens_x[i][j] < leftBound)
					aliens_x[i][j] = leftBound;
				if (aliens_x[i][j] > rightBound)
					aliens_x[i][j] = rightBound;
			}
		}

		updateVerticalDirection(directionofy, topBound, bottomBound);

		if (getLeftMostAliveAlien() <= leftBound) {
			speedofaliensx = -speedofaliensx;
			for (int i = 0; i < alien_rows; i++) {
				for (int j = 0; j < alien_columns; j++) {
					if (!aliens_alive[i][j])
						continue;

					aliens_y[i][j] += 50;

					if (aliens_y[i][j] < topBound)
						aliens_y[i][j] = topBound;
					if (aliens_y[i][j] > bottomBound)
						aliens_y[i][j] = bottomBound;
				}
			}
			updateVerticalDirection(directionofy, topBound, bottomBound);
		}


		gridmovesx -= speedofaliensx;
		gridmovesy += directionofy * speedofaliensy;

		updateVerticalDirection(directionofy, topBound, bottomBound);


	

		if (getLeftMostAliveAlien() <= leftBound) {
			updateHighScore(score, highScoreFile);
			saveHighScore(highScoreFile);


			myRect(initial_x, initial_y, initial_x + 60, initial_y + 30, 0, 0, 0);
			spaceship_head(initial_x, initial_y, 60, 30, 30, 0, 0, 0);
			fins(initial_x, initial_y, 30, 0, 0, 0);
			GridOfAliens(gridmovesx, gridmovesy, 0, 0, 0);

			drawText(60, 400, 200, 180, 255, 180, "Game Over!");
			drawText(40, 500, 400, 180, 255, 180, "You Lose!");
			drawText(19, 25, 600, 180, 255, 180, "Humanity will remember your bravery.Try again to reclaim victory!");
			break;



		}


		// Shooting the Bullet

		bool keypressed = isKeyPressed(whichkey);
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bullet_active[i])
			{
				{
					// First check if the bullet hit something
					checkBulletCollision(bullet_x[i], bullet_y[i], bullet_active[i]);

					// If the bullet hit an alien, stop processing it
					if (!bullet_active[i])
						continue;

					// Erase old bullet
					myRect(bullet_x[i], bullet_y[i],
						bullet_x[i] + 15, bullet_y[i] + 2,
						0, 0, 0);

					// Move bullet
					bullet_x[i] += 10;

					// Remove bullet if it leaves the screen
					if (bullet_x[i] > full_screen_width)
					{
						bullet_active[i] = false;
						bullet_x[i] = -1;
						bullet_y[i] = -1;
					}
					else
					{
						// Draw bullet
						myRect(bullet_x[i], bullet_y[i],
							bullet_x[i] + 10, bullet_y[i] + 2,
							255, 192, 203);
					}

				}
			}
		}
		frameCounter++;

		const int shipMinX = 20 + 15;                                  // keeps fin tip >= box left edge
		const int shipMaxX = full_screen_width - 20 - 90;              // keeps nose tip <= box right edge
		const int shipMinY = 20;                                       // keeps top edge >= box top edge
		const int shipMaxY = full_screen_height - 20 - 31;             // keeps bottom edge <= box bottom edge

		// Move Left
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			initial_x -= 20;
			if (initial_x < shipMinX)
				initial_x = shipMinX;
		}

		// Move Up
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			initial_y -= 20;
			if (initial_y < shipMinY)
				initial_y = shipMinY;
		}

		// Move Right
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			initial_x += 20;
			if (initial_x > shipMaxX)
				initial_x = shipMaxX;
		}

		// Move Down
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			initial_y += 20;
			if (initial_y > shipMaxY)
				initial_y = shipMaxY;
		}

		// Shoot
	// Shoot with Spacebar
		bool currentSpaceState = (GetAsyncKeyState(VK_SPACE) & 0x8000);

		if (currentSpaceState && !previousSpaceState)
		{
			// Space was just pressed
			for (int i = 0; i < MAX_BULLETS; i++)
			{
				if (!bullet_active[i])
				{
					bullet_active[i] = true;
					bullet_x[i] = initial_x + 90;
					bullet_y[i] = initial_y + 15;
					break;
				}
			}
		}

		previousSpaceState = currentSpaceState;
		//Checks if bullet goes out of bounds or not\

		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bullet_active[i])
			{
				// Erase old bullet
				myRect(bullet_x[i], bullet_y[i],
					bullet_x[i] + 15, bullet_y[i] + 2,
					0, 0, 0);

				// Move bullet
				bullet_x[i] += 40;

				// Check if bullet leaves screen
				if (bullet_x[i] > full_screen_width)
				{
					bullet_active[i] = false;
					bullet_x[i] = -1;
					bullet_y[i] = -1;
				}
				else
				{
					// Draw bullet at new position
					myRect(bullet_x[i], bullet_y[i],
						bullet_x[i] + 10, bullet_y[i] + 2,
						255, 192, 203);
				}
			}
		}

		//Calling all functions 
		//Printing Spaceship
		win();

		myRect(initial_x, initial_y, initial_x + 60, initial_y + 31, 255, 255, 255);
		spaceship_head(initial_x, initial_y, 60, 30, 30, 255, 0, 0);
		fins(initial_x, initial_y, 30, 255, 255, 0);
		GridOfAliens(gridmovesx, gridmovesy, 0, 255, 0);

		// These comments will be present permenantly on screen


		drawText(20, 1000, 750, 255, 0, 230, "Defend your territory!");
		drawText(20, 1050, 800, 255, 0, 230, "Shoot them down!");
		static DWORD lastTime = GetTickCount();

		if (GetTickCount() - lastTime >= 1000)
		{
			gtime++;
			lastTime = GetTickCount();
		}

		myRect(1450, 200, 1600, 120, 255, 0, 255);
		char timetext[20];
		sprintf_s(timetext, "Timer: %d", gtime);
		drawText(30, 1400, 60, 0, 255, 255, timetext);

		aliencollisionwithspaceship(initial_x, initial_y, widthofspaceship, heightofspaceship);


		myRect(1450, 290, 1600, 370, 218, 112, 214);
		char livesText[20];
		sprintf_s(livesText, "Lives: %d", PlayerLives);
		drawText(30, 1400, 230, 220, 190, 240, livesText);

		myRect(1450, 480, 1600, 560, 201, 159, 255);
		char scoretext[20];
		sprintf_s(scoretext, "Score: %d", score);
		drawText(30, 1400, 420, 0, 255, 255, scoretext);

		if (frameCounter % 10 == 0)
		{
			drawText(18, 50, 700, 0, 255, 255,
				"Press esc to pause");
			drawText(18, 50, 750, 0, 255, 255,
				"Press space bar to shoot");
			drawText(18, 50, 800, 0, 255, 255,
				"If aliens reach left side you lose");
			drawText(18, 50, 850, 0, 255, 255,
				"If aliens touch spaceship you lose a life");
			drawText(18, 50, 900, 0, 255, 255,
				"You have 3 lives!");
		}

		// --- Cap frame rate -------------------------------------------
		// Sleep off whatever's left of the ~16ms frame budget so the loop
		// can't outrun the screen's ability to actually draw the queued
		// GDI calls. This is what removes the startup buffering.
		{
			DWORD frameTime = GetTickCount() - frameStart;
			if (frameTime < targetFrameTime)
				Sleep(targetFrameTime - frameTime);
		}
		// -----------------------------------------------------------------

	}
	Sleep(1);


	return 0;
}