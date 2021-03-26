#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <bangtal.h>
#include <stdlib.h>
#include <time.h>



SceneID scene;
TimerID timer;
ObjectID game_board[16],original_board[16], start;
int blank = 15;
float timeValue = 0.05f;
bool game = false;

int mixCount;


int indexToX(int i)
{
	return 214 + 214 * (i % 4);
}
int indexToY(int i)
{
	return 480 - 120 * (i / 4);
}

int game_index(ObjectID object) 
{
	for (int i = 0; i < 16; i++)
	{
		if (game_board[i] == object)return i;
	}
	return -1;
}
void game_move(int i)
{
	ObjectID object = game_board[i];
	game_board[i] = game_board[blank];
	locateObject(game_board[i], scene, indexToX(i), indexToY(i));
	game_board[blank] = object;
	locateObject(game_board[blank], scene, indexToX(blank), indexToY(blank));
	blank = i;
}
bool possible_move(int i) 
{
	if (i % 4 > 0 && blank == i - 1) return true;
	if (i % 4 < 3 && blank == i + 1) return true;
	if (i / 4 > 0 && blank == i - 4) return true;
	if (i / 4 < 3 && blank == i + 4) return true;

	return false;
}

int random_move()
{
	int i = -1;



	while (i == -1){
		switch (rand() % 4){
		case 0:if (blank % 4 > 0) i = blank - 1;
			break;
		case 1:if (blank % 4 < 3) i = blank + 1;
			break;
		case 2:if (blank / 4 > 0) i = blank - 4;
			break;
		case 3:if (blank / 4 < 3) i = blank + 4;
			break;

		}
	}
	return i;
}
	

bool game_end() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != original_board[i])return false;
	}
	return true;
}


void game_start() 
{
	blank = 15;
	hideObject(game_board[blank]);

	mixCount = 100;

	setTimer(timer, timeValue);
	startTimer(timer);

}

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true)
{
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (game) {
		int i = game_index(object);
		if (possible_move(i)) {
			game_move(i);

			if (game_end()) {
				game = false;
				showObject(start);
				showObject(game_board[blank]);

				showMessage("성공 (≧ω≦)/ ~~");

			}

		}
			
	}
	else {
		if (object == start) {
			game_start();
		}
	}
}



void timeCallback(TimerID timer) 
{
	game_move(random_move());
	
	mixCount--;
	if(mixCount > 0){
		setTimer(timer, timeValue);
		startTimer(timer);
	}
	else{
		game = true;
		hideObject(start);
	}

	
}




 SceneID game_init()
{
	scene = createScene("퍼즐 맞추기 게임", "Images/background.png");


	char path[50];
	for (int i = 0; i < 16; i++) {
		sprintf(path, "Images/%d.png", i + 1);
		game_board[i] = createObject(path, scene, indexToX(i), indexToY(i));
		original_board[i] = game_board[i];
	}
	start = createObject("Images/start.png", scene, 642, 60);

	timer = createTimer(timeValue);

	return scene;
}

int main()
{
	srand(time(NULL));
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timeCallback);


	startGame(game_init());
}

