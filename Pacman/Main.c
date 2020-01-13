#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>


typedef struct Player {
	Vector2 position;
	Vector2 speed;
	int size;
} Player;
typedef struct Pellet {
	Vector2 position;
	int size;
} Pellet;
typedef struct Wall {
	Vector2 startPos;
	Vector2 endPos;
	float width;
}Wall;
typedef struct Ghost {
	Vector2 position;
	Vector2 speed;
	int size;
	Color color;
} Ghost;
enum playerSpeed {
	up = -2,
	down = 2,
	right = 2,
	left = -2,
};

void UpdateGame();
void DrawGame();
void InitGame();
void UpdateDrawFrame();
void EventHandler();
void CheckPlayerWall();
int32_t gameOver = 1;
int32_t score = 0;
static Player player = { 0 };
static Ghost pinky = { 0 };
const int wallBuffer = 2;
const int lineBuffer = 3;
static Wall topWall = { 44 , 64, 554, 64, 12 };
static Wall leftWall = { 4, 58, 4, 800,  12 };
static Wall rightWall = { 594, 58, 594, 800,  12 };
static Wall bottomWall = { 44 , 794, 554, 794, 12 };
uint8_t* scoreBuffer;

int32_t main() {
	InitWindow(600, 800, "Pacman");
	SetTargetFPS(60);
	InitGame();
	while (!WindowShouldClose()) {
		UpdateDrawFrame();
	}
	CloseWindow();
	return 0;
}

void InitGame() {
	/* Player */
	player.position = (Vector2){ 100,100 };
	player.speed = (Vector2){ 0,0 };
	player.size = 10;
	/* Generate pellets/Power pellet */
	/* Ghosts */
	pinky.color = PINK;
	pinky.speed = (Vector2){ 0,0 };
	pinky.size = 10;
	pinky.position = (Vector2){ 300,400 };
}

/* Main Event Handler */
void UpdateGame() {
	enum speed speed;
	if (gameOver > 0) {
		player.position.y += player.speed.y;
		player.position.x += player.speed.x;
		pinky.position.y += pinky.speed.y;
		pinky.position.x += pinky.speed.x;
		EventHandler();
	}
}

void DrawGame() {
	/* Drawing */
	BeginDrawing();
	ClearBackground(BLACK);

	/*Walls */
	/*Top Wall*/
	DrawLineEx(topWall.startPos, topWall.endPos, topWall.width, PURPLE);
	/*Left Wall*/
	DrawLineEx(leftWall.startPos, leftWall.endPos, leftWall.width, PURPLE);
	/*Bottom Wall*/
	DrawLineEx(bottomWall.startPos, bottomWall.endPos, bottomWall.width, PURPLE);
	/*Right Wall*/
	DrawLineEx(rightWall.startPos, rightWall.endPos, rightWall.width, PURPLE);
	/*End*/

	/*Ghost Box*/
	DrawRectangleLines(250, 350, 100, 100, PURPLE);

	/*End*/
	scoreBuffer = malloc(32);

	//Init
	if (gameOver > 0) {
		DrawCircleV(player.position, player.size, YELLOW);
		sprintf_s(scoreBuffer, 32, "Score:%d", score);
		DrawText(scoreBuffer, 480, 10, 20, WHITE);
		DrawCircleV(pinky.position, pinky.size, pinky.color);
	}

	if (gameOver == 0) {
		DrawText("Game Over", 250, 400, 20, WHITE);
	}
	free(scoreBuffer);
	EndDrawing();
}

void UpdateDrawFrame() {
	UpdateGame();
	DrawGame();
}

void CheckPlayerWall() {
	/* Player Wall Collision Logic */
	if (player.position.x <= leftWall.startPos.x + leftWall.width + wallBuffer) {
		player.position.x = leftWall.width + leftWall.startPos.x + lineBuffer;
	}
	if (player.position.x >= rightWall.startPos.x - rightWall.width - wallBuffer) {
		player.position.x = rightWall.startPos.x - rightWall.width - lineBuffer;
	}
	if (player.position.y <= topWall.startPos.y + topWall.width + wallBuffer) {
		player.position.y = topWall.startPos.y + topWall.width + lineBuffer;
	}
	if (player.position.y >= bottomWall.startPos.y - bottomWall.width - wallBuffer) {
		player.position.y = bottomWall.startPos.y - bottomWall.width - lineBuffer;
	}
}

void EventHandler() {
	/* Event Handler */
	if (IsKeyDown(KEY_DOWN)) {
		if (player.speed.y != 2) player.speed.y += 2;
		player.speed.x = 0;
	}
	if (IsKeyDown(KEY_UP)) {
		if (player.speed.y != -2) player.speed.y -= 2;
		player.speed.x = 0;
	}
	if (IsKeyDown(KEY_LEFT)) {
		if (player.speed.x != -2) player.speed.x -= 2;
		player.speed.y = 0;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		if (player.speed.x != 2) player.speed.x += 2;
		player.speed.y = 0;
	}

	CheckPlayerWall();


	/* Ghost Move Logic */
	if (pinky.position.y >= 782) pinky.position.y = 781;
	if (pinky.position.x >= 582) pinky.position.x = 581;
	if (pinky.position.y <= 50) pinky.position.y = 51;
	if (pinky.position.x <= 12) pinky.position.x = 13;

	/* Pellet Logic
	for (int32_t i = 0; i < 25; i++)
	{
		if ((player.position.x < (pellets[i].position.x + pellets[i].size)
			&& (player.position.x + player.size) > pellets[i].position.x
			&& player.position.y < (pellets[i].position.y + pellets[i].size)
			&& player.position.y + player.size >(pellets[i].position.y)))
		{
			pellets[i].position = (Vector2){ -5,-5 };
			score++;
		}
	} */

	/* Collison with pinky the ghost */
	if ((player.position.x < (pinky.position.x + player.size)
		&& (player.position.x + player.size) > pinky.position.x
		&& player.position.y < (pinky.position.y + pinky.size)
		&& player.position.y + player.size >(pinky.position.y))) gameOver = 0;

	/* Pinky Follow Logic?
	if (pinky.position.x >= player.position.x
		&& pinky.position.y >= player.position.y) {
		pinky.speed.x = -1.5;
		pinky.speed.y = -1.5;
	}
	if (pinky.position.x <= player.position.x
		&& pinky.position.y <= player.position.y) {
		pinky.speed.x = 1.5;
		pinky.speed.y = 1.5;
	}
	if (pinky.position.x <= player.position.x
		&& pinky.position.y >= player.position.y) {
		pinky.speed.x = 1.5;
		pinky.speed.y = -1.5;
	}
	if (pinky.position.x >= player.position.x
		&& pinky.position.y <= player.position.y) {
		pinky.speed.x = -1.5;
		pinky.speed.y = 1.5;
	} */
}
