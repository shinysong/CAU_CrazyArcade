#include <iostream>
#include <bangtal>
#include <ctime>

using namespace bangtal;
using namespace std;

extern void one_main();
extern void two_main();
extern void three_main();
ScenePtr lobbyScene;
SoundPtr lobbySound;
ScenePtr lobby_description;
ObjectPtr des_exitButton;
ObjectPtr descriptionButton;
int level = 1;

void lobby_main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	lobbyScene = Scene::create("", "Images/����.png");
	lobbySound = Sound::create("music/lobby.mp3");
	lobby_description = Scene::create("", "Images/���Ӽ���.png");
	lobbySound->play();

	auto ob = Object::create("Images/pan.png", lobbyScene, 0, 0);

	des_exitButton = Object::create("Images/������.png", lobby_description, 50, 620);
	des_exitButton->setScale(0.7f);
	descriptionButton = Object::create("Images/���ӹ��.png", lobbyScene, 700, 100);

	ob->setOnMouseCallback([&](ObjectPtr, int x, int y, MouseAction) -> bool {
		// cout << x << " " << y << endl;
		if (x > 380 && x < 800 && y > 475 && y < 575) {
			lobbySound->stop();
			one_main();
		}
		else if (x > 815 && x < 1235 && y > 475 && y < 575) {
			if (level < 3) {
				showMessage("2�ܰ踦 ���߸� PLAY�� �� �ֽ��ϴ�.");
				return true;
			}
			lobbySound->stop();
			three_main();
		}
		else if (x > 380 && x < 800 && y > 370 && y < 470) {
			
			if (level < 2) {
				showMessage("1�ܰ踦 ���߸� PLAY�� �� �ֽ��ϴ�.");
				return true;
			}
			lobbySound->stop();
			two_main();
		} 
		return true;
	});

	descriptionButton->setOnMouseCallback([&](ObjectPtr, int x, int y, MouseAction) -> bool {
		lobby_description->enter();
		return true;
	});

	des_exitButton->setOnMouseCallback([&](ObjectPtr, int x, int y, MouseAction) -> bool {
		lobbyScene->enter();
		return true;
	});

	enterScene(lobbyScene->ID());
}