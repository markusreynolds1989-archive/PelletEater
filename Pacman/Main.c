#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct Player {
	Vector2 position;
	Vector2 speed;
} Player;

typedef struct Pellet {
	Vector2 position;
} Pellet;

typedef struct Ghost {
	Vector2 position;
	Vector2 speed;
	Color color;
} Ghost;

void UpdateGame();
void DrawGame();
void InitGame();
void UpdateDrawFrame();

int32_t gameOver = 1;
int32_t score = 0;
static Player player = { 0 };
static Ghost pinky = { 0 };
//Array of structs
Pellet pellets[25] = { 0 };
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
	player.position = (Vector2){ 100,100 };
	player.speed = (Vector2){ 0,0 };
	for (int32_t i = 5; i < 25; i++)
	{
		pellets[i].position = (Vector2){ 20, i * 30 };
	}
	pinky.color = PINK;
	pinky.speed = (Vector2){ 0,0 };
	pinky.position = (Vector2){ 300,400 };
}

/* Main Event Handler */
void UpdateGame() {
	if (gameOver > 0) {
		/* Init Speed */
		player.position.y += player.speed.y;
		player.position.x += player.speed.x;
		pinky.position.y += pinky.speed.y;
		pinky.position.x += pinky.speed.x;
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
		/* Player Logic */
		if (player.position.y >= 782) player.position.y = 781;
		if (player.position.x >= 582) player.position.x = 581;
		if (player.position.y <= 50) player.position.y = 51;
		if (player.position.x <= 12) player.position.x = 13;

		/* Ghost logic */
		if (pinky.position.y >= 800) pinky.position.y = 0;
		if (pinky.position.x >= 600) pinky.position.x = 0;
		if (pinky.position.y <= -1) pinky.position.y = 800;
		if (pinky.position.x <= -1) pinky.position.x = 600;

		/* Pellet Logic */
		for (int32_t i = 0; i < 25; i++)
		{
			if ((player.position.x < (pellets[i].position.x + 5)
				&& (player.position.x + 10) > pellets[i].position.x
				&& player.position.y < (pellets[i].position.y + 5)
				&& player.position.y + 10>(pellets[i].position.y)))
			{
				pellets[i].position = (Vector2){ -5,-5 };
				score++;
			}
		}
		/* Collison with pinky the ghost */
		if ((player.position.x < (pinky.position.x + 10)
			&& (player.position.x + 10) > pinky.position.x
			&& player.position.y < (pinky.position.y + 10)
			&& player.position.y + 10>(pinky.position.y))) gameOver = 0;

		/* Pinky follow logic? */
	}
}

void DrawGame() {
	/* Drawing */
	BeginDrawing();
	ClearBackground(BLACK);
	/*Walls */
	/*Left Wall*/
	DrawRectangle(0, 64, 8, 700, PURPLE);
	/*Top Wall*/
	DrawRectangle(0, 34, 600, 8, PURPLE);
	/*Bottom Wall*/
	DrawRectangle(0, 792, 600, 8, PURPLE);
	/*Right Wall*/
	DrawRectangle(592, 64, 8, 700, PURPLE);
	/*End*/

	/*Ghost Box*/
	/*End*/
	scoreBuffer = malloc(32);
	if (gameOver > 0) {
		DrawCircleV(player.position, 10, YELLOW);
		for (int32_t i = 0; i < 25; i++)
		{
			DrawCircleV(pellets[i].position, 4, WHITE);
		}
		sprintf_s(scoreBuffer, 32, "Score:%d", score);
		DrawText(scoreBuffer, 480, 10, 20, WHITE);
		DrawCircleV(pinky.position, 10, pinky.color);
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