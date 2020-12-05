#include <iostream>
#include <bangtal>
#include <ctime>

using namespace bangtal;
using namespace std;

extern void lobby_main();

ScenePtr openScene;
ScenePtr description;
SoundPtr openSound;
ObjectPtr exitButton;

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	openScene = Scene::create("", "Images/시작화면.png");
	description = Scene::create("", "Images/게임설명.png");
	openSound = Sound::create("music/opening.mp3");
	exitButton = Object::create("Images/나가기.png", description, 50, 620);
	exitButton->setScale(0.7f);
	openSound->play();

	auto ob = Object::create("Images/pan.png", openScene, 0, 0);

	ob->setOnMouseCallback([&](ObjectPtr, int x, int y, MouseAction) -> bool {
		//cout << x << " " << y << endl;

		if (x > 930 && x < 1210 && y > 40 && y < 105) {
			openSound->stop();
			lobby_main();
		}

		if (x > 930 && x < 1210 && y > 120 && y < 190) {
			description->enter();			
		}

		return true;
		});

	exitButton->setOnMouseCallback([&](ObjectPtr, int x, int y, MouseAction) -> bool {
		openScene->enter();
		return true;
		});

	startGame(openScene);
	return 0;
}