#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid


// The following exist purely for readability, and do not contribute to any
// functions or code, they're just so you could understand the code more
// properly. They only indicate the index of the array.
const int x = 0;
const int y = 1;
const int exists = 2;
const int isHead = 3;

/////////////////////////////////////////////////////////////////////////////
// //
//					Function Declarations.								   //
// //
/////////////////////////////////////////////////////////////////////////////

void showScore(RenderWindow& window, Text& scoreText, int& score);
void showLevel(RenderWindow& window, Text& levelText, int& level);
void drawLives(RenderWindow& window, bool maxLives[], Sprite MaxLives[]);
void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite);
void drawBullets(int maxBullets, RenderWindow& window, float bullets[][3], Sprite bulletSprites[]);
void drawCentipede(RenderWindow& window, float centipede[][4], Sprite Centipede[], int centipedeLength);
void drawScorpion(RenderWindow& window, Sprite& scorpionSprite, float scorpion[]);
void drawSpider(RenderWindow& window, Sprite& spiderSprite, float spider[]);
void drawFlea(RenderWindow& window, Sprite& fleaSprite, float flea[]);
void moveCentipede(float& delay, bool hasReversed[], int mushroomRow, int mushroomCol, float centipede[][4], int centipedeLength, Clock& centipedeClock, bool mushrooms[][30], float xMove[], float afterYMove[], Sprite Centipede[], int& level, int& prevLevel);
void fireBullets(int maxBullets, float player[], float bullets[][3], Clock& bulletCoolDownClock);
void moveBullets(int level, bool hasReversed[], int maxBullets, int mushroomRow, int mushroomCol, float bullets[][3], Clock& bulletClock, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30], float centipede[][4], int centipedeLength, int& score, float xMove[], Sprite Centipede[], Texture& centipedeHeadTexture, Clock& levelClock);
void checkBulletsMushroom(int mushroomRow, int mushroomCol, int i, float bullets[][3], bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30], int& score);
void checkBulletsCentipede(int level, bool hasReversed[], int mushroomRow, int mushroomCol, int i, float bullets[][3], float centipede[][4], int centipedeLength, bool mushrooms[][30], int& score, float xMove[], Sprite Centipede[], Texture& centipedeHeadTexture, Clock& levelClock);
void checkBulletsScorpion(float scorpion[], int maxBullets, float bullets[][3], int& score, Clock& scorpionDelayClock);
void checkBulletsSpider(float player[], float spider[], int maxBullets, float bullets[][3], int& score, Clock& spiderDelayClock);
void checkPlayerCentipede(float player[], float centipede[][4], int centipedeLength, float xMove[], Clock& playerClock);
void checkPlayerScorpion(float player[], Clock& playerClock, float scorpion[]);
void checkPlayerSpider(float player[], Clock& playerClock, float spider[]);
void checkPlayerFlea(float player[], Clock& playerClock, float flea[]);
void drawMushrooms(int mushroomRow, int mushroomCol, RenderWindow& window, bool mushrooms[][30], Sprite Mushrooms[][30]);
void resetCentipede(bool hasReversed[], int mushroomRow, int mushroomCol, Sprite Centipede[], Texture& centipedeHeadTexture, Texture& centipedeBodyTexture, float centipede[][4], int centipedeLength, float xMove[], bool mushrooms[][30]);
void moveScorpion(Clock& scorpionClock, Sprite& scorpionSprite, int& scorpionMultiple, int& scorpionOperation, float scorpion[], float& scorpionXMove, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]);
void moveSpider(Clock& spiderClock, Sprite& spiderSprite, float spider[], float& spiderXMove, float& spiderYMove, int spiderUpBound, int spiderLowBound, bool mushrooms[][30], int mushroomLives[][30]);
void moveFlea(float flea[], Clock& fleaClock, Clock& fleaDelayClock, float& fleaYMove, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]);
void increaseMushrooms(int mushroomRow, int mushroomCol, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]);
void nextLevel(bool hasReversed[], int mushroomRow, int mushroomCol, Sprite Centipede[], Texture& centipedeHeadTexture, Texture& centipedeBodyTexture, float centipede[][4], int centipedeLength, float xMove[], bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]);
void checkPlayerPoisonous(float player[], bool mushrooms[][30], int mushroomLives[][30], Clock& playerClock);
bool centipedeIsKilled(float centipede[][4], int centipedeLength);
void regenerateMushrooms(int mushroomRow, int mushroomCol, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30], int& score);
void drawMenu(RenderWindow& window, Text Menu[], Text& flex, Sprite& menuSprite, Sprite dummyCentipede[], Sprite& dummyScorpion, Sprite& dummySpider, Sprite& dummyFlea);
void drawHelp(RenderWindow& window, Text Help[], Sprite& helpSprite);
void drawHighScores(RenderWindow& window, Text HighScores[], Text& heading, int highscores[], Sprite& HighScoreSprite);
void drawGameOver(RenderWindow& window, Text GameOver[]);
void sortHighScores(int highScores[]);
string convertToString(int score);

int main() {
	srand(time(0));

	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Centipede", Style::Close | Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(100, 0));

	// Initializing Background Music.
	Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(50);
	bgMusic.setLoop(true);

	// Initializing Background.
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("Textures/background.png");
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.30)); // Reduces Opacity to 25%

	// Initializing Player and Player Sprites.
	float player[3] = {};
	player[x] = 15; player[y] = 25; player[exists] = true;
	Texture playerTexture;
	playerTexture.loadFromFile("Textures/spaceship.png");
	Sprite playerSprite(playerTexture);
	playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	Clock playerClock;

	// Initializing lives
	bool maxLives[5] = { false, false, false, true, true };
	Sprite MaxLives[5];
	for (int i = 0; i < 5; i++) {
		MaxLives[i].setTexture(playerTexture);
		MaxLives[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	}
	int thisLife = 3;

	// Initializing Bullets and Bullet Sprite.
	const int maxBullets = 3;
	float bullets[maxBullets][3] = {};
	Clock bulletClock, bulletCoolDownClock;
	Texture bulletTexture;
	bulletTexture.loadFromFile("Textures/bullet.png");
	Sprite bulletSprites[maxBullets];
	for (int i = 0; i < maxBullets; i++) {
		bullets[i][exists] = false;
		bulletSprites[i].setTexture(bulletTexture);
		bulletSprites[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	}

	// Initializing and setting mushrooms
	const int mushroomRow = 29;
	const int mushroomCol = 30;

	Texture mushroomTexture;
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	Sprite Mushrooms[mushroomRow][mushroomCol] = {};
	bool mushrooms[mushroomRow][mushroomCol] = { false };
	int mushroomLives[mushroomRow][mushroomCol] = { 0 };

	int mushroomCount = 20 + rand() % 11;
	int decision;
	while (mushroomCount > 0) {
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < mushroomCol; j++) {
				j += rand() % 30;
				if (j >= 25) {
					break;
				}
				decision = rand() % 2;
				if (decision == 1 && !mushrooms[i][j]) {
					mushrooms[i][j] = 1;
					mushroomLives[i][j] = 2;
					mushroomCount--;
					if (mushroomCount == 0) {
						break;
					}
				}
			}
			if (mushroomCount == 0) {
				break;
			}
		}
	}
	for (int i = 0; i < mushroomRow; i++) {
		for (int j = 0; j < mushroomCol; j++) {
			Mushrooms[i][j].setTexture(mushroomTexture);
			if (i < 25) {
				Mushrooms[i][j].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
			}
			else {
				Mushrooms[i][j].setTextureRect(IntRect(0, boxPixelsY, boxPixelsX, boxPixelsY));
			}
		}
	}

	// Initializing Centipede
	Texture centipedeHeadTexture, centipedeBodyTexture;
	centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");
	centipedeBodyTexture.loadFromFile("Textures/c_body_left_walk.png");

	const int centipedeLength = 12;
	float centipede[centipedeLength][4] = {};
	Sprite Centipede[centipedeLength] = {};

	float xMove[centipedeLength]; float afterYMove[centipedeLength];
	bool hasReversed[centipedeLength] = {};
	for (int i = 0; i < centipedeLength; i++) {
		xMove[i] = -1;
		afterYMove[i] = 1;
		centipede[i][exists] = true;
	}

	Centipede[0].setTexture(centipedeHeadTexture);
	Centipede[0].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	for (int i = 1; i < centipedeLength; i++) {
		Centipede[i].setTexture(centipedeBodyTexture);
		Centipede[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	}
	Clock centipedeClock;

	bool flag;
	do {
		flag = false;
		centipede[0][x] = rand() % 19;
		centipede[0][y] = rand() % 25;

		for (int i = centipede[0][x]; i < centipede[0][x] + centipedeLength; i++) {
			if (mushrooms[int(centipede[0][y])][i] == 1) {
				flag = true;
				break;
			}
		}
	} while (flag);

	for (int i = 1; i < centipedeLength; i++) {
		centipede[i][x] = centipede[i - 1][x] + 1;
		centipede[i][y] = centipede[0][y];
	}
	centipede[0][isHead] = true;
	float delay = 300; // speed of centipede

	// Intitializing Scorpion
	Clock scorpionClock, scorpionDelayClock;
	float scorpionDelay = 5 + rand() % 6;
	Texture scorpionTexture;
	scorpionTexture.loadFromFile("Textures/scorpion.png");
	Sprite scorpionSprite(scorpionTexture);
	scorpionSprite.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
	float scorpion[3] = {};
	float scorpionXMove;
	const float scorpionPosition[2] = { -1, 29 };
	int scorpionMultiple = 2, scorpionOperation = 1;

	// Initializing Spider
	Clock spiderClock, spiderDelayClock;
	float spiderDelay = 8 + rand() % 6;
	Texture spiderTexture;
	spiderTexture.loadFromFile("Textures/spider_and_score.png");
	Sprite spiderSprite(spiderTexture);
	spiderSprite.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
	float spider[3] = {};
	float spiderXMove, spiderYMove = 1;
	const float spiderPosition[2] = { -1, 29 };
	int spiderUpBound, spiderLowBound;

	// Intitializing Flea
	Clock fleaClock, fleaDelayClock;
	float fleaDelay = 15 + rand() % 6;
	Texture fleaTexture;
	fleaTexture.loadFromFile("Textures/flea.png");
	Sprite fleaSprite(fleaTexture);
	fleaSprite.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
	float flea[3] = {};
	float fleaYMove;

	// Initializing Score and Level
	Font scoreFont, levelFont;
	if (!scoreFont.loadFromFile("Fonts/score_font.otf") || !levelFont.loadFromFile("Fonts/level_font.ttf")) {
		cout << "error" << endl;
	}
	scoreFont.loadFromFile("Fonts/score_font.otf"); levelFont.loadFromFile("Fonts/level_font.ttf");
	Text scoreText, levelText;
	scoreText.setFont(scoreFont); levelText.setFont(levelFont);
	scoreText.setCharacterSize(50); levelText.setCharacterSize(30);
	scoreText.setFillColor(Color(200, 150, 20, 255 * 0.7)); levelText.setFillColor(Color(200, 150, 20, 255 * 0.7));
	int score = 0, level = 1, prevLevel = 1;
	int difference = 5000; // for one-ups
	Clock levelClock;

	float dimmer = 0.2; // for dimming the screen when game over

	// Initializing Menu
	Text Menu[5], flex;
	Font menuFont;
	if (!menuFont.loadFromFile("Fonts/menu_font.ttf")) {
		cout << "error" << endl;
	}
	menuFont.loadFromFile("Fonts/menu_font.ttf");
	flex.setFont(menuFont);
	flex.setCharacterSize(200);
	flex.setFillColor(Color::Green);
	flex.setString("CENTIPEDE");
	flex.setPosition(25, 20);
	for (int i = 0; i < 5; i++) {
		Menu[i].setFont(menuFont);
		Menu[i].setCharacterSize(80);
		if (i == 0) {
			Menu[i].setFillColor(Color::Red);
		}
		else {
			Menu[i].setFillColor(Color::White);
		}
	}
	Menu[0].setString("START"); Menu[0].setPosition(395, 250);
	Menu[1].setString("RESTART"); Menu[1].setPosition(350, 400);
	Menu[2].setString("HELP"); Menu[2].setPosition(410, 550);
	Menu[3].setString("HIGHSCORES"); Menu[3].setPosition(275, 700);
	Menu[4].setString("EXIT"); Menu[4].setPosition(410, 850);

	bool showMenu = true, showHighScore = false, showHelp = false, gameOver = false, pause = false, hasStarted = false, hasRestarted = false, hasLost = false;
	int menuIndex = 0;

	Sprite dummyCentipede[centipedeLength];
	dummyCentipede[0].setTexture(centipedeHeadTexture);
	dummyCentipede[0].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	for (int i = 1; i < centipedeLength; i++) {
		dummyCentipede[i].setTexture(centipedeBodyTexture);
		dummyCentipede[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	}
	Sprite dummyScorpion(scorpionTexture);
	dummyScorpion.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
	Sprite dummySpider(spiderTexture);
	dummySpider.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
	Sprite dummyFlea(fleaTexture);
	dummyFlea.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));

	// Initializing High Scores
	int highScores[10] = {};
	ifstream scores("highscores.txt");
	if (!scores.is_open()) {
		cerr << "error" << endl;
		return 0;
	}
	for (int i = 0; i < 10; i++) {
		scores >> highScores[i];
	}
	scores.close();
	Text HighScores[10], heading;
	heading.setFont(menuFont);
	heading.setCharacterSize(110);
	heading.setFillColor(Color::Green);
	heading.setString("TOP 10 HIGHSCORES");
	heading.setPosition(15, 10);
	int pos = 150;
	for (int i = 0; i < 10; i++) {
		HighScores[i].setFont(menuFont);
		HighScores[i].setCharacterSize(60);
		if (i < 9) HighScores[i].setString(convertToString(0) + convertToString(i + 1) + ". -------------------------- " + convertToString(highScores[i]));
		else HighScores[i].setString(convertToString(i + 1) + ". -------------------------- " + convertToString(highScores[i]));
		HighScores[i].setFillColor(Color::White);
		HighScores[i].setPosition(50, pos);
		pos += 80;
	}

	Text Help[10];
	for (int i = 0; i < 10; i++) Help[i].setFont(menuFont);
	Help[0].setString("WELCOME TO CENTIPEDE!");
	Help[0].setCharacterSize(90);
	Help[0].setPosition(10, 0);
	Help[0].setFillColor(Color::Green);
	Help[1].setString("CONTROLS");
	Help[1].setCharacterSize(70);
	Help[1].setPosition(345, 120);
	Help[1].setFillColor(Color::Blue);
	Help[2].setString("UP -> UP");
	Help[2].setCharacterSize(50);
	Help[2].setPosition(700, 200);
	Help[2].setFillColor(Color::Red);
	Help[3].setString("DOWN -> DOWN");
	Help[3].setCharacterSize(50);
	Help[3].setPosition(620, 250);
	Help[3].setFillColor(Color::Red);
	Help[4].setString("LEFT -> LEFT");
	Help[4].setCharacterSize(50);
	Help[4].setPosition(653, 300);
	Help[4].setFillColor(Color::Red);
	Help[5].setString("RIGHT -> RIGHT");
	Help[5].setCharacterSize(50);
	Help[5].setPosition(615, 350);
	Help[5].setFillColor(Color::Red);
	Help[6].setString("FIRE -> F");
	Help[6].setCharacterSize(50);
	Help[6].setPosition(20, 200);
	Help[6].setFillColor(Color::Red);
	Help[7].setString("PAUSE -> ESC");
	Help[7].setCharacterSize(50);
	Help[7].setPosition(20, 350);
	Help[7].setFillColor(Color::Red);
	Help[8].setString("GAMEPLAY");
	Help[8].setCharacterSize(70);
	Help[8].setPosition(345, 420);
	Help[8].setFillColor(Color::Blue);
	Help[9].setString("YOU WILL BE FACED BY A CENTIPEDE ADVANCING FROM THE TOP.\n UPON HITTING THE CENTIPEDE, IT WILL SPLIT THE CENTIPEDE\nINTO TWO PORTIONS AT THAT POINT WHERE EACH PIECE WILL\nCONTINUE INDEPENDENTLY ON ITS WAY DOWN. IN THE EARLY 2\n LEVELS, TAKING THE HEADSHOT OF THE CENTIPEDE WILL KILL\n  THE WHOLE SEGMENT ATTACHED TO IT BUT IN LATER LEVELS,\n  THE REAR SEGMENT BECOMES THE HEAD. YOU HAVE 4 LIVES IN\n   THE BEGINNING BUT YOU CAN GET 1UPS BY ACCUMULATING\n  POINTS. WHATEVER YOU DO, JUST DON'T LET THE CENTIPEDE\n          COME INSIDE YOUR MOVEMENT AREA!!! GOOD LUCK!");
	Help[9].setCharacterSize(35);
	Help[9].setPosition(0, 515);
	Help[9].setFillColor(Color::White);

	Font gameOverFont;
	if (!gameOverFont.loadFromFile("Fonts/gameover_font.ttf")) {
		cout << "error" << endl;
	}
	gameOverFont.loadFromFile("Fonts/gameover_font.ttf");
	Text GameOver[2];
	GameOver[0].setFont(gameOverFont); GameOver[1].setFont(gameOverFont);
	GameOver[0].setCharacterSize(100); GameOver[1].setCharacterSize(40);
	GameOver[0].setFillColor(Color::Red); GameOver[1].setFillColor(Color::White);
	GameOver[0].setPosition(15, 400); GameOver[1].setPosition(120, 600);
	GameOver[0].setString("GAME OVER");

	while (window.isOpen()) {

		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Right && !showMenu && player[exists] && player[x] + 1 < gameColumns) {
					player[x]++;
				}
				else if (e.key.code == Keyboard::Left && !showMenu && player[exists] && player[x] - 1 >= 0) {
					player[x]--;
				}
				else if (e.key.code == Keyboard::Up) {
					if (showMenu && !showHelp && !showHighScore) {
						if (menuIndex - 1 >= 0) {
							Menu[menuIndex].setFillColor(Color::White);
							menuIndex--;
							Menu[menuIndex].setFillColor(Color::Red);
						}
					}
					else if (player[exists] && player[y] - 1 >= 25) {
						player[y]--;
					}
				}
				else if (e.key.code == Keyboard::Down) {
					if (showMenu && !showHelp && !showHighScore) {
						if (menuIndex + 1 < 5) {
							Menu[menuIndex].setFillColor(Color::White);
							menuIndex++;
							Menu[menuIndex].setFillColor(Color::Red);
						}
					}
					else if (player[exists] && player[y] + 1 < gameRows) {
						player[y]++;
					}
				}
				else if (e.key.code == Keyboard::Escape) {
					if (showHelp) showHelp = false;
					else if (showHighScore) showHighScore = false;
					else if (gameOver) {
						showMenu = true;
						gameOver = false;
						hasRestarted = false;
						ifstream scores("highscores.txt");
						if (!scores.is_open()) {
							cerr << "error" << endl;
							return 0;
						}
						for (int i = 0; i < 10; i++) {
							scores >> highScores[i];
						}
						scores.close();
						for (int i = 0; i < 10; i++) {
							if (i < 9) HighScores[i].setString(convertToString(0) + convertToString(i + 1) + ". -------------------------- " + convertToString(highScores[i]));
							else HighScores[i].setString(convertToString(i + 1) + ". -------------------------- " + convertToString(highScores[i]));
						}
					}
					else showMenu = true;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		// //
		//					Functions Execution.					 //
		//														     //
		// //
		///////////////////////////////////////////////////////////////


		if (showMenu && !showHelp && !showHighScore && !gameOver) {
			window.draw(backgroundSprite);
			drawMenu(window, Menu, flex, backgroundSprite, dummyCentipede, dummyScorpion, dummySpider, dummyFlea);
			window.display();
			window.clear();
			if (Keyboard::isKeyPressed(Keyboard::Return)) {
				switch (menuIndex) {
				case 0:
					if (!hasStarted) {
						Menu[0].setString("RESUME");
						Menu[0].setPosition(370, 250);
						hasStarted = true;
						showMenu = false;
						scorpionDelayClock.restart();
						spiderDelayClock.restart();
						fleaDelayClock.restart();
					}
					if ((hasRestarted || !gameOver) && !hasLost) {
						showMenu = false;
						scorpionDelayClock.restart();
						spiderDelayClock.restart();
						fleaDelayClock.restart();
					}
					break;
				case 1: // restart the whole game
					if (hasStarted) {
						showMenu = false;
						gameOver = false;
						hasRestarted = true;
						hasLost = false;
						Menu[menuIndex].setFillColor(Color::White);
						menuIndex = 0;
						Menu[menuIndex].setFillColor(Color::Red);
						backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.3));
						for (int i = 0; i < centipedeLength; i++) {
							Centipede[i].setColor(Color(255, 255, 255, 255));
						}
						for (int i = 0; i < mushroomRow; i++) {
							for (int j = 0; j < mushroomCol; j++) {
								Mushrooms[i][j].setColor(Color(255, 255, 255, 255));
							}
						}
						scoreText.setFillColor(Color(200, 150, 20, 255 * 0.7));
						levelText.setFillColor(Color(200, 150, 20, 255 * 0.7));
						player[x] = 15; player[y] = 25; player[exists] = true;
						for (int i = 0; i < 5; i++) {
							if (i < 3) maxLives[i] = false;
							else maxLives[i] = true;
						}
						thisLife = 3;
						for (int i = 0; i < maxBullets; i++) {
							bullets[i][exists] = false;
						}
						for (int i = 0; i < mushroomRow; i++) {
							for (int j = 0; j < mushroomCol; j++) {
								mushrooms[i][j] = false;
								mushroomLives[i][j] = 0;
							}
						}
						mushroomCount = 20 + rand() % 11;
						while (mushroomCount > 0) {
							for (int i = 0; i < 25; i++) {
								for (int j = 0; j < mushroomCol; j++) {
									j += rand() % 30;
									if (j >= 25) {
										break;
									}
									decision = rand() % 2;
									if (decision == 1 && !mushrooms[i][j]) {
										mushrooms[i][j] = 1;
										mushroomLives[i][j] = 2;
										mushroomCount--;
										if (mushroomCount == 0) {
											break;
										}
									}
								}
								if (mushroomCount == 0) {
									break;
								}
							}
						}
						for (int i = 0; i < mushroomRow; i++) {
							for (int j = 0; j < mushroomCol; j++) {
								Mushrooms[i][j].setTexture(mushroomTexture);
								if (i < 25) {
									Mushrooms[i][j].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
								}
								else {
									Mushrooms[i][j].setTextureRect(IntRect(0, boxPixelsY, boxPixelsX, boxPixelsY));
								}
							}
						}
						for (int i = 0; i < centipedeLength; i++) {
							xMove[i] = -1;
							afterYMove[i] = 1;
							hasReversed[i] = false;
							centipede[i][isHead] = false;
							centipede[i][exists] = true;
						}

						Centipede[0].setTexture(centipedeHeadTexture);
						Centipede[0].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
						for (int i = 1; i < centipedeLength; i++) {
							Centipede[i].setTexture(centipedeBodyTexture);
							Centipede[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
						}

						do {
							flag = false;
							centipede[0][x] = rand() % 19;
							centipede[0][y] = rand() % 25;

							for (int i = centipede[0][x]; i < centipede[0][x] + centipedeLength; i++) {
								if (mushrooms[int(centipede[0][y])][i] == 1) {
									flag = true;
									break;
								}
							}
						} while (flag);

						for (int i = 1; i < centipedeLength; i++) {
							centipede[i][x] = centipede[i - 1][x] + 1;
							centipede[i][y] = centipede[0][y];
						}
						centipede[0][isHead] = true;
						delay = 300;
						score = 0; difference = 5000; level = 1; prevLevel = 1;

						// restart scorpion
						scorpionSprite.setColor(Color(255, 255, 255, 255));
						scorpionClock.restart(), scorpionDelayClock.restart();
						scorpionDelay = 5 + rand() % 6;
						scorpionSprite.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
						scorpion[exists] = false;
						scorpionMultiple = 2; scorpionOperation = 1;

						// restart spider
						spiderSprite.setColor(Color(255, 255, 255, 255));
						spiderClock.restart(), spiderDelayClock.restart();
						spiderDelay = 8 + rand() % 6;
						spiderSprite.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
						spider[exists] = false;
						spiderYMove = 1;

						// restart flea
						fleaSprite.setColor(Color(255, 255, 255, 255));
						fleaClock.restart(), fleaDelayClock.restart();
						fleaDelay = 15 + rand() % 6;
						fleaSprite.setTextureRect(IntRect(0, 0, 2 * boxPixelsX, boxPixelsY));
						flea[exists] = false;
					}
					break;
				case 2:
					showHelp = true;
					break;
				case 3:
					showHighScore = true;
					break;
				case 4: window.close();
					break;
				}
			}
		}
		else if (showHelp) {
			window.clear();
			drawHelp(window, Help, backgroundSprite);
			window.display();
		}
		else if (showHighScore) {
			window.clear();
			drawHighScores(window, HighScores, heading, highScores, backgroundSprite);
			window.display();
		}
		else {
			window.clear();
			window.draw(backgroundSprite);
			drawMushrooms(mushroomRow, mushroomCol, window, mushrooms, Mushrooms);
			moveCentipede(delay, hasReversed, mushroomRow, mushroomCol, centipede, centipedeLength, centipedeClock, mushrooms, xMove, afterYMove, Centipede, level, prevLevel);

			if (scorpionDelayClock.getElapsedTime().asSeconds() > scorpionDelay && !scorpion[exists]) {
				// spawn scorpion
				scorpion[x] = scorpionPosition[rand() % 2];
				if (scorpion[x] == -1) scorpionXMove = 1;
				else scorpionXMove = -1;
				scorpion[y] = rand() % 29;
				scorpion[exists] = true;
				scorpionDelay = 5 + rand() % 6;
			}
			if (scorpion[exists]) {
				moveScorpion(scorpionClock, scorpionSprite, scorpionMultiple, scorpionOperation, scorpion, scorpionXMove, mushrooms, mushroomLives, Mushrooms);
				drawScorpion(window, scorpionSprite, scorpion);
			}
			if (spiderDelayClock.getElapsedTime().asSeconds() > spiderDelay && !spider[exists]) {
				// spawn spider
				spider[x] = spiderPosition[rand() % 2];
				if (spider[x] == -1) spiderXMove = 1;
				else spiderXMove = -1;
				spider[y] = rand() % 21;
				spiderUpBound = spider[y];
				spiderLowBound = spider[y] + 10;
				spider[exists] = true;
				spiderDelay = 8 + rand() % 6;
			}
			if (spider[exists]) {
				moveSpider(spiderClock, spiderSprite, spider, spiderXMove, spiderYMove, spiderUpBound, spiderLowBound, mushrooms, mushroomLives);
				drawSpider(window, spiderSprite, spider);
			}
			if (fleaDelayClock.getElapsedTime().asSeconds() > fleaDelay && !flea[exists]) {
				// spawn flea
				flea[x] = rand() % 29; flea[y] = -1;
				fleaYMove = 1;
				flea[exists] = true;
				fleaDelay = 15 + rand() % 6;
			}
			if (flea[exists]) {
				moveFlea(flea, fleaClock, fleaDelayClock, fleaYMove, mushrooms, mushroomLives, Mushrooms);
				drawFlea(window, fleaSprite, flea);
			}

			for (int i = 0; i < maxBullets; i++) {
				if (bullets[i][exists] == true) {
					moveBullets(level, hasReversed, maxBullets, mushroomRow, mushroomCol, bullets, bulletClock, mushrooms, mushroomLives, Mushrooms, centipede, centipedeLength, score, xMove, Centipede, centipedeHeadTexture, levelClock);
					checkBulletsScorpion(scorpion, maxBullets, bullets, score, scorpionDelayClock);
					checkBulletsSpider(player, spider, maxBullets, bullets, score, spiderDelayClock);
					drawBullets(maxBullets, window, bullets, bulletSprites);
				}
			}
			if (centipedeIsKilled(centipede, centipedeLength)) {
				if (levelClock.getElapsedTime().asSeconds() > 1.5) {
					nextLevel(hasReversed, mushroomRow, mushroomCol, Centipede, centipedeHeadTexture, centipedeBodyTexture, centipede, centipedeLength, xMove, mushrooms, mushroomLives, Mushrooms);
					level++;
				}
			}

			if (player[exists]) {
				fireBullets(maxBullets, player, bullets, bulletCoolDownClock);
			}
			drawCentipede(window, centipede, Centipede, centipedeLength);

			if (!gameOver) {
				if (player[exists] == false && playerClock.getElapsedTime().asSeconds() > 2) {
					for (int i = 0; i < maxBullets; i++) {
						bullets[i][exists] = false;
					}
					maxLives[thisLife] = false;
					thisLife++;
					if (thisLife <= 5) regenerateMushrooms(mushroomRow, mushroomCol, mushrooms, mushroomLives, Mushrooms, score);
					if (thisLife > 5) {
						gameOver = true;
						hasLost = true;
						GameOver[1].setString("Your Score: " + convertToString(score) + "\nPress ESC to Exit");
						highScores[9] = score;
						sortHighScores(highScores);
						ofstream scores("highscores.txt");
						if (!scores.is_open()) {
							cerr << "error" << endl;
							return 0;
						}
						for (int i = 0; i < 10; i++) {
							scores << highScores[i] << endl;
						}
						scores.close();
					}
					else {
						player[x] = 15; player[y] = 25; player[exists] = true;
					}
				}
			}

			if (player[exists]) {
				drawPlayer(window, player, playerSprite);
				checkPlayerCentipede(player, centipede, centipedeLength, xMove, playerClock);
				checkPlayerScorpion(player, playerClock, scorpion);
				checkPlayerSpider(player, playerClock, spider);
				checkPlayerFlea(player, playerClock, flea);
				checkPlayerPoisonous(player, mushrooms, mushroomLives, playerClock);
			}
			if (thisLife >= 0) {
				if (difference - score <= 0) {
					thisLife--;
					maxLives[thisLife] = true;
					difference = score * 2;
					while (difference % 10) difference--;
				}
			}

			showScore(window, scoreText, score);
			showLevel(window, levelText, level);
			drawLives(window, maxLives, MaxLives);
			if (gameOver) {
				backgroundSprite.setColor(Color(255, 255, 255, 255 * dimmer));
				for (int i = 0; i < centipedeLength; i++) {
					if (centipede[i][exists]) {
						Centipede[i].setColor(Color(255, 255, 255, 255 * dimmer));
					}
				}
				for (int i = 0; i < mushroomRow; i++) {
					for (int j = 0; j < mushroomCol; j++) {
						if (mushrooms[i][j]) {
							Mushrooms[i][j].setColor(Color(255, 255, 255, 255 * dimmer));
						}
					}
				}
				scorpionSprite.setColor(Color(255, 255, 255, 255 * dimmer));
				spiderSprite.setColor(Color(255, 255, 255, 255 * dimmer));
				fleaSprite.setColor(Color(255, 255, 255, 255 * dimmer));
				scoreText.setFillColor(Color(200, 150, 20, 255 * dimmer));
				levelText.setFillColor(Color(200, 150, 20, 255 * dimmer));
				drawGameOver(window, GameOver);
			}
			window.display();
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// //
//						Function Definitions							  //
// //
////////////////////////////////////////////////////////////////////////////

// resposible for drawing the lives of player remaining on the screen
void drawLives(RenderWindow& window, bool maxLives[], Sprite MaxLives[]) {
	for (int i = 0; i < 5; i++) {
		if (maxLives[i] == true) {
			MaxLives[i].setPosition((25 + i) * boxPixelsX, 29 * boxPixelsY);
			window.draw(MaxLives[i]);
		}
	}
}

// resposible for drawing the player on the screen
void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite) {
	if (player[exists]) {
		playerSprite.setPosition(player[x] * boxPixelsX, player[y] * boxPixelsY);
		window.draw(playerSprite);
	}
}

// resposible for drawing score on the screen
void showScore(RenderWindow& window, Text& scoreText, int& score) {
	scoreText.setString("SCORE: " + convertToString(score));
	scoreText.setPosition(14, resolutionY - 1.55 * boxPixelsY);
	window.draw(scoreText);
}

// resposible for drawing level on the screen
void showLevel(RenderWindow& window, Text& levelText, int& level) {
	levelText.setString("LEVEL : " + convertToString(level));
	levelText.setPosition(13 * boxPixelsX, 28.9 * boxPixelsY);
	window.draw(levelText);
}

// this takes centipede position and draws it on the screen
void drawCentipede(RenderWindow& window, float centipede[][4], Sprite Centipede[], int centipedeLength) {
	for (int i = 0; i < centipedeLength; i++) {
		if (centipede[i][exists] && (centipede[i][x] >= 0 && centipede[i][x] <= 29) && (centipede[i][y] >= 0 && centipede[i][y] <= 29)) {
			Centipede[i].setPosition(centipede[i][x] * boxPixelsX, centipede[i][y] * boxPixelsY);
			window.draw(Centipede[i]);
		}
	}
}

// this function is resposible for the movement of bullets on the screen with passing time
void moveBullets(int level, bool hasReversed[], int maxBullets, int mushroomRow, int mushroomCol, float bullets[][3], Clock& bulletClock, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30], float centipede[][4], int centipedeLength, int& score, float xMove[], Sprite Centipede[], Texture& centipedeHeadTexture, Clock& levelClock) {

	if (bulletClock.getElapsedTime().asMilliseconds() < 20) {
		return;
	}
	bulletClock.restart();

	for (int i = 0; i < maxBullets; i++) {
		if (bullets[i][exists] == false) {
			continue;
		}
		else {
			if (bullets[i][y] - 1 > -2) {
				bullets[i][y] -= 1;
				checkBulletsMushroom(mushroomRow, mushroomCol, i, bullets, mushrooms, mushroomLives, Mushrooms, score);
				checkBulletsCentipede(level, hasReversed, mushroomRow, mushroomCol, i, bullets, centipede, centipedeLength, mushrooms, score, xMove, Centipede, centipedeHeadTexture, levelClock);
			}
			else {
				bullets[i][exists] = false;
			}
		}
	}
}

// this function draws the bullets array on the screen
void drawBullets(int maxBullets, RenderWindow& window, float bullets[][3], Sprite bulletSprites[]) {
	for (int i = 0; i < maxBullets; i++) {
		if (bullets[i][exists]) {
			bulletSprites[i].setPosition(bullets[i][x] * boxPixelsX, bullets[i][y] * boxPixelsY);
			window.draw(bulletSprites[i]);
		}
	}
}

// this function fires bullets when the F key is pressed on the keyboard
void fireBullets(int maxBullets, float player[], float bullets[][3], Clock& bulletCoolDownClock) {
	if (Keyboard::isKeyPressed(Keyboard::F) && bulletCoolDownClock.getElapsedTime().asMilliseconds() > 250) {
		for (int i = 0; i < maxBullets; i++) {
			if (bullets[i][exists] == false) {
				bullets[i][x] = player[x];
				bullets[i][y] = player[y];
				bullets[i][exists] = true;
				bulletCoolDownClock.restart();
				break;
			}
		}
	}
}

// drawing of mushrooms in this function
void drawMushrooms(int mushroomRow, int mushroomCol, RenderWindow& window, bool mushrooms[][30], Sprite Mushrooms[][30]) {
	for (int i = 0; i < mushroomRow; i++) {
		for (int j = 0; j < mushroomCol; j++) {
			if (mushrooms[i][j] == 1) {
				Mushrooms[i][j].setPosition(j * boxPixelsX, i * boxPixelsY);
				window.draw(Mushrooms[i][j]);
			}
		}
	}
}

// simple function which checks the collision of bullets and mushrooms and reduces life of mushroom
void checkBulletsMushroom(int mushroomRow, int mushroomCol, int i, float bullets[][3], bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30], int& score) {
	bool flag = false;
	for (int j = 0; j < mushroomRow; j++) {
		for (int k = 0; k < mushroomCol; k++) {
			if (mushrooms[j][k] == 1) {
				if ((bullets[i][y] < mushroomRow) && int(bullets[i][x]) == k && int(bullets[i][y]) == j && mushroomLives[j][k] == 2) {
					Mushrooms[j][k].setTextureRect(IntRect(boxPixelsX * 2, 0, boxPixelsX, boxPixelsY));
					mushroomLives[j][k]--;
					bullets[i][exists] = false;
					flag = true;
					break;
				}
				else if ((bullets[i][y] < mushroomRow) && int(bullets[i][x]) == k && int(bullets[i][y]) == j && mushroomLives[j][k] == 1) {
					Mushrooms[j][k].setTextureRect(IntRect(0, 0, 0, 0));
					mushroomLives[j][k]--;
					score += 1;
					bullets[i][exists] = false;
					mushrooms[j][k] = 0;
					flag = true;
					break;
				}
			}
		}
		if (flag) {
			break;
		}
	}
}

/*
This function is also a major part of the game, as it is resposible for the
fragmenting of centipede, when hit by bullet. if level is less than or equal to
2 then this destroys the whole segment if head is hit and normal fragmentation
other wise.
*/
void checkBulletsCentipede(int level, bool hasReversed[], int mushroomRow, int mushroomCol, int i, float bullets[][3], float centipede[][4], int centipedeLength, bool mushrooms[][30], int& score, float xMove[], Sprite Centipede[], Texture& centipedeHeadTexture, Clock& levelClock) {

	if (!centipedeIsKilled(centipede, centipedeLength)) { // if centipede is not killed only then check for bullet and centipede collision
		int k;
		for (int j = 0; j < centipedeLength; j++) {
			if (centipede[j][exists] && bullets[i][x] == centipede[j][x] && bullets[i][y] == centipede[j][y]) {
				if (level < 3) {
					if (!centipede[j][isHead]) {
						if (xMove[j] < 0) {
							if (!hasReversed[j]) {
								k = j + 1;
								while (k < centipedeLength && centipede[k][exists]) {
									xMove[k] *= -1;
									k++;
									if (centipede[k][exists] == false || k == centipedeLength) {
										Centipede[k - 1].setTexture(centipedeHeadTexture);
										Centipede[k - 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
										centipede[k - 1][isHead] = true;
									}
								}
							}
							else {
								k = j - 1;
								while (k >= 0 && centipede[k][exists]) {
									xMove[k] *= -1;
									k--;
									if (centipede[k][exists] == false || k == centipedeLength) {
										Centipede[k + 1].setTexture(centipedeHeadTexture);
										Centipede[k + 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
										centipede[k + 1][isHead] = true;
									}
								}
							}
						}
						else {
							if (!hasReversed[j]) {
								k = j - 1;
								while (k >= 0 && centipede[k][exists]) {
									xMove[k] *= -1;
									k--;
									if (centipede[k][exists] == false || k == centipedeLength) {
										Centipede[k + 1].setTexture(centipedeHeadTexture);
										Centipede[k + 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
										centipede[k + 1][isHead] = true;
									}
								}
							}
							else {
								k = j + 1;
								while (k < centipedeLength && centipede[k][exists]) {
									xMove[k] *= -1;
									k++;
									if (centipede[k][exists] == false || k == centipedeLength) {
										Centipede[k - 1].setTexture(centipedeHeadTexture);
										Centipede[k - 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
										centipede[k - 1][isHead] = true;
									}
								}
							}
						}

						bullets[i][exists] = false;
						centipede[j][exists] = false;
						if (centipede[j][y] >= 25 && !mushrooms[int(centipede[j][y])][int(centipede[j][x])]) {
							mushrooms[int(centipede[j][y])][int(centipede[j][x])] = true;
						}
						score += 10;
						break;
					}
					else {
						if (xMove[j] < 0) {
							if (!hasReversed[j]) {
								k = j + 1;
								while (k < centipedeLength && centipede[k][exists]) {
									centipede[k][exists] = false;
									if (centipede[k][y] >= 25 && !mushrooms[int(centipede[k][y])][int(centipede[k][x])]) {
										mushrooms[int(centipede[k][y])][int(centipede[k][x])] = true;
									}
									score += 10;
									k++;
								}
							}
							else {
								k = j - 1;
								while (k >= 0 && centipede[k][exists]) {
									centipede[k][exists] = false;
									if (centipede[k][y] >= 25 && !mushrooms[int(centipede[k][y])][int(centipede[k][x])]) {
										mushrooms[int(centipede[k][y])][int(centipede[k][x])] = true;
									}
									score += 10;
									k--;
								}
							}
						}
						else {
							if (!hasReversed[j]) {
								k = j - 1;
								while (k >= 0 && centipede[k][exists]) {
									centipede[k][exists] = false;
									if (centipede[k][y] >= 25 && !mushrooms[int(centipede[k][y])][int(centipede[k][x])]) {
										mushrooms[int(centipede[k][y])][int(centipede[k][x])] = true;
									}
									score += 10;
									k--;
								}
							}
							else {
								k = j + 1;
								while (k < centipedeLength && centipede[k][exists]) {
									centipede[k][exists] = false;
									if (centipede[k][y] >= 25 && !mushrooms[int(centipede[k][y])][int(centipede[k][x])]) {
										mushrooms[int(centipede[k][y])][int(centipede[k][x])] = true;
									}
									score += 10;
									k++;
								}
							}
						}
						bullets[i][exists] = false;
						centipede[j][exists] = false;
						if (centipede[j][y] >= 25 && !mushrooms[int(centipede[j][y])][int(centipede[j][x])]) {
							mushrooms[int(centipede[j][y])][int(centipede[j][x])] = true;
						}
						score += 20;
						break;
					}
				}
				else {
					if (xMove[j] < 0) {
						if (!hasReversed[j]) {
							k = j + 1;
							while (k < centipedeLength && centipede[k][exists]) {
								xMove[k] *= -1;
								k++;
								if (centipede[k][exists] == false || k == centipedeLength) {
									Centipede[k - 1].setTexture(centipedeHeadTexture);
									Centipede[k - 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
									centipede[k - 1][isHead] = true;
								}
							}
						}
						else {
							k = j - 1;
							while (k >= 0 && centipede[k][exists]) {
								xMove[k] *= -1;
								k--;
								if (centipede[k][exists] == false || k == centipedeLength) {
									Centipede[k + 1].setTexture(centipedeHeadTexture);
									Centipede[k + 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
									centipede[k + 1][isHead] = true;
								}
							}
						}
					}
					else {
						if (!hasReversed[j]) {
							k = j - 1;
							while (k >= 0 && centipede[k][exists]) {
								xMove[k] *= -1;
								k--;
								if (centipede[k][exists] == false || k == centipedeLength) {
									Centipede[k + 1].setTexture(centipedeHeadTexture);
									Centipede[k + 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
									centipede[k + 1][isHead] = true;
								}
							}
						}
						else {
							k = j + 1;
							while (k < centipedeLength && centipede[k][exists]) {
								xMove[k] *= -1;
								k++;
								if (centipede[k][exists] == false || k == centipedeLength) {
									Centipede[k - 1].setTexture(centipedeHeadTexture);
									Centipede[k - 1].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
									centipede[k - 1][isHead] = true;
								}
							}
						}
					}

					bullets[i][exists] = false;
					centipede[j][exists] = false;
					if (centipede[j][y] >= 25 && !mushrooms[int(centipede[j][y])][int(centipede[j][x])]) {
						mushrooms[int(centipede[j][y])][int(centipede[j][x])] = true;
					}
					score += 10;
					break;
				}
			}
		}
		if (centipedeIsKilled(centipede, centipedeLength)) {
			levelClock.restart();
		}
	}
}

/*
This is our main function, which is resposible for the movement of centipede,
in which all of the movements are handled. for the centipede, outside player area,
separate movement, for the centipede in player area, separate movement. we make
the body pieces follow their respective heads, using the arrays, prevPos,
prevXMove and prevReversed, which allow to set the position and movement of the
next segment to that which was before it.
*/
void moveCentipede(float& delay, bool hasReversed[], int mushroomRow, int mushroomCol, float centipede[][4], int centipedeLength, Clock& centipedeClock, bool mushrooms[][30], float xMove[], float afterYMove[], Sprite Centipede[], int& level, int& prevLevel) {
	// static bool flags[12] = {true, true, true, true, true, true, true, true, true, true, true, true};
	int yMove = 1;
	float prevPos[12][2] = {};
	float prevXMove[12] = {};
	bool prevReversed[12] = {};
	if (level > prevLevel) {
		// increase speed of centipede if level has increased
		delay /= 2;
		prevLevel = level;
	}
	if (centipedeClock.getElapsedTime().asMilliseconds() < delay) {
		return;
	}

	// copy all elements of centipede in different arrays
	for (int i = 0; i < centipedeLength; i++) {
		prevPos[i][x] = centipede[i][x];
		prevPos[i][y] = centipede[i][y];
		prevXMove[i] = xMove[i];
		prevReversed[i] = hasReversed[i];
	}

	for (int i = 0; i < centipedeLength; i++) {
		if (centipede[i][exists] && centipede[i][isHead]) {
			if (centipede[i][y] > 24) { // if centipede gets in player area
				if (xMove[i] < 0) { // is moving to left
					if (afterYMove[i] > 0) { // is moving down
						if (centipede[i][x] + xMove[i] >= 0) { // can move left
							centipede[i][x] += xMove[i]; // move left
							if (!hasReversed[i]) {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
							else {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
						}
						else { // cannot move left
							if (centipede[i][y] + afterYMove[i] < gameRows) { // check can move down
								centipede[i][y] += afterYMove[i]; // move down
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
								else {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
							}
							else { // cannot move down
								afterYMove[i] *= -1; // start moving up
								centipede[i][y] += afterYMove[i]; // move up
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
								else {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
							}
						}
					}
					else { // is moving up
						if (centipede[i][x] + xMove[i] >= 0) { // can move left
							centipede[i][x] += xMove[i]; // move left
							if (!hasReversed[i]) {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
							else {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
						}
						else { // cannot move left
							if (centipede[i][y] + afterYMove[i] >= 25) { // check can move up
								centipede[i][y] += afterYMove[i]; // move up
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
								else {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
							}
							else { // cannot move up
								afterYMove[i] *= -1; // start moving down
								centipede[i][y] += afterYMove[i]; // move down
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
								else {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
							}
						}
					}
				}
				else { // moving to right
					if (afterYMove[i] > 0) { // is moving down
						if (centipede[i][x] + xMove[i] <= 29) { // can move right
							centipede[i][x] += xMove[i]; // move right
							if (!hasReversed[i]) {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
							else {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
						}
						else { // cannot move right
							if (centipede[i][y] + afterYMove[i] < gameRows) { // check can move down
								centipede[i][y] += afterYMove[i]; // move down
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
								else {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
							}
							else { // cannot move down
								afterYMove[i] *= -1; // start moving up
								centipede[i][y] += afterYMove[i]; // move up
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
								else {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
							}
						}
					}
					else { // is moving up
						if (centipede[i][x] + xMove[i] <= 29) { // can move right
							centipede[i][x] += xMove[i]; // move right
							if (!hasReversed[i]) {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
							else {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
						}
						else { // cannot move right
							if (centipede[i][y] + afterYMove[i] >= 25) { // check can move up
								centipede[i][y] += afterYMove[i]; // move up
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
								else {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
							}
							else { // cannot move up
								afterYMove[i] *= -1; // start moving down
								centipede[i][y] += afterYMove[i]; // move down
								xMove[i] *= -1; // reverse direction
								if (hasReversed[i]) hasReversed[i] = false;
								else hasReversed[i] = true;
								if (hasReversed[i]) {
									for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
										centipede[j][x] = prevPos[j + 1][x];
										centipede[j][y] = prevPos[j + 1][y];
										xMove[j] = prevXMove[j + 1];
										hasReversed[j] = prevReversed[j + 1];
									}
								}
								else {
									for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
										centipede[j][x] = prevPos[j - 1][x];
										centipede[j][y] = prevPos[j - 1][y];
										xMove[j] = prevXMove[j - 1];
										hasReversed[j] = prevReversed[j - 1];
									}
								}
							}
						}
					}
				}
			}
			else { // normal movement
				if (xMove[i] < 0) { // is moving to left
					if (centipede[i][x] + xMove[i] >= 0) { // can move left
						if (mushrooms[int(centipede[i][y])][int(centipede[i][x]) - 1] == 1) {
							centipede[i][y] += yMove;
							xMove[i] *= -1;
							if (hasReversed[i]) hasReversed[i] = false;
							else hasReversed[i] = true;

							if (hasReversed[i]) {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
							else {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
						}
						else {
							centipede[i][x] += xMove[i];

							if (!hasReversed[i]) {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
							else {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
						}
					}
					else {
						centipede[i][y] += yMove;
						xMove[i] *= -1;
						if (hasReversed[i]) hasReversed[i] = false;
						else hasReversed[i] = true;
						if (hasReversed[i]) {
							for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
								centipede[j][x] = prevPos[j - 1][x];
								centipede[j][y] = prevPos[j - 1][y];
								xMove[j] = prevXMove[j - 1];
								hasReversed[j] = prevReversed[j - 1];
							}
						}
						else {
							for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
								centipede[j][x] = prevPos[j + 1][x];
								centipede[j][y] = prevPos[j + 1][y];
								xMove[j] = prevXMove[j + 1];
								hasReversed[j] = prevReversed[j + 1];
							}
						}
					}
				}
				else {
					if (centipede[i][x] + xMove[i] <= 29) {
						if (mushrooms[int(centipede[i][y])][int(centipede[i][x]) + 1] == 1) {
							centipede[i][y] += yMove;
							xMove[i] *= -1;
							if (hasReversed[i]) hasReversed[i] = false;
							else hasReversed[i] = true;

							if (hasReversed[i]) {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
							else {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
						}
						else {
							centipede[i][x] += xMove[i];

							if (!hasReversed[i]) {
								for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
									centipede[j][x] = prevPos[j + 1][x];
									centipede[j][y] = prevPos[j + 1][y];
									xMove[j] = prevXMove[j + 1];
									hasReversed[j] = prevReversed[j + 1];
								}
							}
							else {
								for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
									centipede[j][x] = prevPos[j - 1][x];
									centipede[j][y] = prevPos[j - 1][y];
									xMove[j] = prevXMove[j - 1];
									hasReversed[j] = prevReversed[j - 1];
								}
							}
						}
					}
					else {
						centipede[i][y] += yMove;
						xMove[i] *= -1;
						if (hasReversed[i]) hasReversed[i] = false;
						else hasReversed[i] = true;

						if (hasReversed[i]) {
							for (int j = i - 1; centipede[j][exists] && !centipede[j][isHead] && j >= 0; j--) {
								centipede[j][x] = prevPos[j + 1][x];
								centipede[j][y] = prevPos[j + 1][y];
								xMove[j] = prevXMove[j + 1];
								hasReversed[j] = prevReversed[j + 1];
							}
						}
						else {
							for (int j = i + 1; centipede[j][exists] && !centipede[j][isHead] && j < centipedeLength; j++) {
								centipede[j][x] = prevPos[j - 1][x];
								centipede[j][y] = prevPos[j - 1][y];
								xMove[j] = prevXMove[j - 1];
								hasReversed[j] = prevReversed[j - 1];
							}
						}
					}
				}
			}
		}
	}
	centipedeClock.restart();
}

// this funtion checks if player and centipede collided, and if so, destroys player
void checkPlayerCentipede(float player[], float centipede[][4], int centipedeLength, float xMove[], Clock& playerClock) {
	for (int i = 0; i < centipedeLength; i++) {
		if (centipede[i][exists]) {
			if (centipede[i][x] == player[x] && centipede[i][y] == player[y]) {
				player[exists] = false;
				playerClock.restart();
			}
		}
	}
}

// returns true if the centipede is killed
bool centipedeIsKilled(float centipede[][4], int centipedeLength) {
	for (int i = 0; i < centipedeLength; i++) {
		if (centipede[i][exists]) {
			return false;
		}
	}
	return true;
}

// after the centipede has been killed, this function resets it for the next level
void resetCentipede(bool hasReversed[], int mushroomRow, int mushroomCol, Sprite Centipede[], Texture& centipedeHeadTexture, Texture& centipedeBodyTexture, float centipede[][4], int centipedeLength, float xMove[], bool mushrooms[][30]) {
	Centipede[0].setTexture(centipedeHeadTexture);
	Centipede[0].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	for (int i = 1; i < centipedeLength; i++) {
		Centipede[i].setTexture(centipedeBodyTexture);
		Centipede[i].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
	}
	for (int i = 0; i < centipedeLength; i++) {
		hasReversed[i] = false;
		centipede[i][exists] = true;
		xMove[i] = -1;
	}
	bool flag;
	do {
		flag = false;
		centipede[0][x] = rand() % 19;
		centipede[0][y] = rand() % 25;
		for (int i = centipede[0][x]; i < centipede[0][x] + centipedeLength; i++) {
			if (mushrooms[int(centipede[0][y])][i] == 1) {
				flag = true;
				break;
			}
		}
	} while (flag);

	for (int i = 1; i < centipedeLength; i++) {
		centipede[i][x] = centipede[i - 1][x] + 1;
		centipede[i][y] = centipede[0][y];
	}
	centipede[0][isHead] = true;
	for (int i = 1; i < centipedeLength; i++) {
		centipede[i][isHead] = false;
	}
}

// this function increases the number of mushrooms on the screen after each passed level
void increaseMushrooms(int mushroomRow, int mushroomCol, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]) {
	int count = 0; // count the number of mushrooms existing on the field
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < mushroomCol; j++) {
			if (mushrooms[i][j]) {
				count++;
			}
		}
	}
	int newCount = (0.2 * count) + count; // generate 20% more mushrooms or new random mushrooms if player cleared all before
	if (newCount == 0) newCount = 20 + rand() % 11;

	// draw mushrooms at random locations
	int decision;
	while (newCount > 0) {
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < mushroomCol; j++) {
				j += rand() % 30;
				if (j >= 25) {
					break;
				}
				decision = rand() % 2;
				if (decision == 1 && !mushrooms[i][j]) {
					mushrooms[i][j] = 1;
					mushroomLives[i][j] = 2;
					Mushrooms[i][j].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
					newCount--;
					if (newCount == 0) {
						break;
					}
				}
			}
			if (newCount == 0) {
				break;
			}
		}
	}
}

// this function advances to the next level
void nextLevel(bool hasReversed[], int mushroomRow, int mushroomCol, Sprite Centipede[], Texture& centipedeHeadTexture, Texture& centipedeBodyTexture, float centipede[][4], int centipedeLength, float xMove[], bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]) {
	increaseMushrooms(mushroomRow, mushroomCol, mushrooms, mushroomLives, Mushrooms);
	resetCentipede(hasReversed, mushroomRow, mushroomCol, Centipede, centipedeHeadTexture, centipedeBodyTexture, centipede, centipedeLength, xMove, mushrooms);
}

// this function checks if player has collided with a poisonous mushroom
void checkPlayerPoisonous(float player[], bool mushrooms[][30], int mushroomLives[][30], Clock& playerClock) {
	if (player[y] < 29 && mushroomLives[int(player[y])][int(player[x])] == 0 && mushrooms[int(player[y])][int(player[x])]) {
		player[exists] = false;
		playerClock.restart();
	}
}

// this function will regenerate mushrooms after the player is destroyed.
// partially destroyed mushrooms regain their original form and posionous mushroom is converted back to normal
void regenerateMushrooms(int mushroomRow, int mushroomCol, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30], int& score) {
	for (int i = 0; i < mushroomRow; i++) {
		for (int j = 0; j < mushroomCol; j++) {
			if (mushrooms[i][j]) {
				if (mushroomLives[i][j] == 1) { // for normal mushrooms
					mushroomLives[i][j] = 2;
					Mushrooms[i][j].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
					score += 5;
				}
				else if (mushroomLives[i][j] == 0) { // for poisonous mushrooms
					Mushrooms[i][j].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
					mushroomLives[i][j] = 2;
					score += 5;
				}
			}
		}
	}
}

// movement of scorpion on the screen
void moveScorpion(Clock& scorpionClock, Sprite& scorpionSprite, int& scorpionMultiple, int& scorpionOperation, float scorpion[], float& scorpionXMove, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]) {
	if (scorpionClock.getElapsedTime().asMilliseconds() < 200) {
		return;
	}
	if (scorpionXMove < 0) { // moving to left
		if (scorpion[x] + scorpionXMove >= 0) { // can move left
			if (!mushrooms[int(scorpion[y])][int(scorpion[x] + scorpionXMove)]) { // move left
				scorpion[x] += scorpionXMove;
			}
			else {
				if (mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] == 0) {
					scorpion[x] += scorpionXMove;
				}
				else if (mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] == 1) {
					mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] = 0;
					Mushrooms[int(scorpion[y])][int(scorpion[x] + scorpionXMove)].setTextureRect(IntRect(2 * boxPixelsX, boxPixelsY, boxPixelsX, boxPixelsY));
				}
				else if (mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] == 2) {
					mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] = 0;
					Mushrooms[int(scorpion[y])][int(scorpion[x] + scorpionXMove)].setTextureRect(IntRect(0, boxPixelsY, boxPixelsX, boxPixelsY));
				}
			}
		}
		else {
			scorpionXMove *= -1;
		}
	}
	else { // moving to right
		if (scorpion[x] + scorpionXMove < 29) { // can move right
			if (!mushrooms[int(scorpion[y])][int(scorpion[x] + scorpionXMove)]) { // move right
				scorpion[x] += scorpionXMove;
			}
			else {
				if (mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] == 0) {
					scorpion[x] += scorpionXMove;
				}
				else if (mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] == 1) {
					mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] = 0;
					Mushrooms[int(scorpion[y])][int(scorpion[x] + scorpionXMove)].setTextureRect(IntRect(2 * boxPixelsX, boxPixelsY, boxPixelsX, boxPixelsY));
				}
				else if (mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] == 2) {
					mushroomLives[int(scorpion[y])][int(scorpion[x] + scorpionXMove)] = 0;
					Mushrooms[int(scorpion[y])][int(scorpion[x] + scorpionXMove)].setTextureRect(IntRect(0, boxPixelsY, boxPixelsX, boxPixelsY));
				}
			}
		}
		else {
			scorpionXMove *= -1;
		}
	}
	if (scorpionOperation > 0) {
		scorpionSprite.setTextureRect(IntRect(scorpionMultiple * boxPixelsX, 0, 2 * boxPixelsX, boxPixelsY));
		scorpionMultiple += 2;
		if (scorpionMultiple > 6) {
			scorpionMultiple -= 2;
			scorpionOperation *= -1;
		}
	}
	else {
		scorpionSprite.setTextureRect(IntRect(scorpionMultiple * boxPixelsX, 0, 2 * boxPixelsX, boxPixelsY));
		scorpionMultiple -= 2;
		if (scorpionMultiple < 0) {
			scorpionMultiple += 2;
			scorpionOperation *= -1;
		}
	}

	scorpionClock.restart();
}

// check collision of bullet and scorpion
void checkBulletsScorpion(float scorpion[], int maxBullets, float bullets[][3], int& score, Clock& scorpionDelayClock) {
	for (int i = 0; i < maxBullets; i++) {
		if (bullets[i][exists] == false) continue;
		if (scorpion[exists] && (bullets[i][x] == scorpion[x] || bullets[i][x] == scorpion[x] + 1) && bullets[i][y] == scorpion[y]) {
			bullets[i][exists] = false;
			scorpion[exists] = false;
			score += 1000;
			scorpionDelayClock.restart();
			break;
		}
	}
}

// check player and scorpion collision
void checkPlayerScorpion(float player[], Clock& playerClock, float scorpion[]) {
	if (scorpion[exists] && (player[x] == scorpion[x] || player[x] == scorpion[x] + 1) && player[y] == scorpion[y]) {
		player[exists] = false;
		playerClock.restart();
	}
}

// drawing scorpion on screen
void drawScorpion(RenderWindow& window, Sprite& scorpionSprite, float scorpion[]) {
	scorpionSprite.setPosition(scorpion[x] * boxPixelsX, scorpion[y] * boxPixelsY);
	window.draw(scorpionSprite);
}

// movement of spider on the screen
void moveSpider(Clock& spiderClock, Sprite& spiderSprite, float spider[], float& spiderXMove, float& spiderYMove, int spiderUpBound, int spiderLowBound, bool mushrooms[][30], int mushroomLives[][30]) {
	if (spiderClock.getElapsedTime().asMilliseconds() < 150) {
		return;
	}
	if (spiderXMove > 0) { // moving right
		if (spiderYMove > 0) { // moving down
			if (spider[x] + spiderXMove < 29) { // can move right
				if (spider[y] + spiderYMove < spiderLowBound) { // can move down
					spider[x] += spiderXMove; spider[y] += spiderYMove; // move right and down
					if (spider[y] < 29 && mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else { // cannot move down
					spiderYMove *= -1; // reverse y direction, move up
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
			else { // cannot move right
				spiderXMove *= -1; // reverse x direction, move left
				if (spider[y] + spiderYMove < spiderLowBound) {
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else {
					spiderYMove *= -1;
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
		}
		else { // moving right and up
			if (spider[x] + spiderXMove < 29) { // can move right
				if (spider[y] + spiderYMove >= spiderUpBound) { // can move up
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else { // cannot move up
					spiderYMove *= -1;
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
			else { // cannot move right
				spiderXMove *= -1;
				if (spider[y] + spiderYMove >= spiderUpBound) {
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else {
					spiderYMove *= -1;
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
		}
	}
	else { // moving left
		if (spiderYMove > 0) { // moving down
			if (spider[x] + spiderXMove >= 0) { // can move left
				if (spider[y] + spiderYMove < spiderLowBound) { // can move down
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (spider[y] < 29 && mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else { // cannot move down
					spiderYMove *= -1;
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
			else { // cannot move left
				spiderXMove *= -1;
				if (spider[y] + spiderYMove < spiderLowBound) {
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else {
					spiderYMove *= -1;
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
		}
		else { // moving left and up
			if (spider[x] + spiderXMove >= 0) { // can move left
				if (spider[y] + spiderYMove >= spiderUpBound) { // can move up
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else { // cannot move up
					spiderYMove *= -1;
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
			else { // cannot move left
				spiderXMove *= -1;
				if (spider[y] + spiderYMove >= spiderUpBound) {
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
				else {
					spiderYMove *= -1;
					spider[x] += spiderXMove; spider[y] += spiderYMove;
					if (mushrooms[int(spider[y])][int(spider[x])]) {
						mushrooms[int(spider[y])][int(spider[x])] = false;
						mushroomLives[int(spider[y])][int(spider[x])] = 0;
					}
				}
			}
		}
	}

	spiderClock.restart();
}

// check collision of bullet and spider
void checkBulletsSpider(float player[], float spider[], int maxBullets, float bullets[][3], int& score, Clock& spiderDelayClock) {
	for (int i = 0; i < maxBullets; i++) {
		if (bullets[i][exists] == false) continue;
		if (spider[exists] && (bullets[i][x] == spider[x] || bullets[i][x] == spider[x] + 1) && bullets[i][y] == spider[y]) {
			if (player[y] - spider[y] == 1) score += 900;
			else if (player[y] - spider[y] == 2) score += 600;
			else score += 300;
			bullets[i][exists] = false;
			spider[exists] = false;
			spiderDelayClock.restart();
			break;
		}
	}
}

// checking player and spider collision
void checkPlayerSpider(float player[], Clock& playerClock, float spider[]) {
	if (spider[exists] && (player[x] == spider[x] || player[x] == spider[x] + 1) && player[y] == spider[y]) {
		player[exists] = false;
		playerClock.restart();
	}
}

// drawing spider on the screen
void drawSpider(RenderWindow& window, Sprite& spiderSprite, float spider[]) {
	spiderSprite.setPosition(spider[x] * boxPixelsX, spider[y] * boxPixelsY);
	window.draw(spiderSprite);
}

// movement of flea on screen
void moveFlea(float flea[], Clock& fleaClock, Clock& fleaDelayClock, float& fleaYMove, bool mushrooms[][30], int mushroomLives[][30], Sprite Mushrooms[][30]) {
	if (fleaClock.getElapsedTime().asMilliseconds() < 100) {
		return;
	}
	bool decisions[5] = { 0, 0, 1, 0, 0 }; // decreasing the chance of leaving mushroom
	bool decision = decisions[rand() % 5];
	if (flea[y] + fleaYMove < 29) { // inside mushroom grid
		flea[y] += fleaYMove;
		if (decision && !mushrooms[int(flea[y])][int(flea[x])]) {
			mushrooms[int(flea[y])][int(flea[x])] = true;
			mushroomLives[int(flea[y])][int(flea[x])] = 2;
			Mushrooms[int(flea[y])][int(flea[x])].setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
		}
	}
	else {
		flea[y] += fleaYMove;
	}
	if (flea[y] > 29) {
		flea[exists] = false;
		fleaDelayClock.restart();
	}

	fleaClock.restart();
}

// checking player and flea collision
void checkPlayerFlea(float player[], Clock& playerClock, float flea[]) {
	if (flea[exists] && (player[x] == flea[x] || player[x] == flea[x] + 1) && player[y] == flea[y]) {
		player[exists] = false;
		playerClock.restart();
	}
}

// drawing flea on the screen
void drawFlea(RenderWindow& window, Sprite& fleaSprite, float flea[]) {
	fleaSprite.setPosition(flea[x] * boxPixelsX, flea[y] * boxPixelsY);
	window.draw(fleaSprite);
}

// for showing the menu on the screen
void drawMenu(RenderWindow& window, Text Menu[], Text& flex, Sprite& menuSprite, Sprite dummyCentipede[], Sprite& dummyScorpion, Sprite& dummySpider, Sprite& dummyFlea) {
	window.draw(menuSprite);
	window.draw(flex);
	for (int i = 0; i < 5; i++) {
		window.draw(Menu[i]);
	}
	int pos = 300;
	for (int i = 0; i < 12; i++) {
		dummyCentipede[i].setPosition(pos, 20);
		window.draw(dummyCentipede[i]);
		pos += 32;
	}
	dummyScorpion.setPosition(80, 300);
	dummySpider.setPosition(800, 500);
	dummyFlea.setPosition(80, 700);
	window.draw(dummyScorpion);
	window.draw(dummySpider);
	window.draw(dummyFlea);
}

// drawing of instructions window for the game
void drawHelp(RenderWindow& window, Text Help[], Sprite& helpSprite) {
	window.draw(helpSprite);
	for (int i = 0; i < 10; i++) {
		window.draw(Help[i]);
	}
}

// draw highscores window for the game
void drawHighScores(RenderWindow& window, Text HighScores[], Text& heading, int highscores[], Sprite& HighScoreSprite) {
	window.draw(HighScoreSprite);
	window.draw(heading);
	for (int i = 0; i < 10; i++) {
		window.draw(HighScores[i]);
	}
}

// show game over text if player has lost
void drawGameOver(RenderWindow& window, Text GameOver[]) {
	window.draw(GameOver[0]);
	window.draw(GameOver[1]);
}

// for sorting of highscores to draw on highscores window
void sortHighScores(int highScores[]) {
	int temp;
	bool noSwaps;
	int boundary = 9;
	while (boundary > 0) {
		noSwaps = true;
		for (int i = 0; i < boundary; i++) {
			if (highScores[i] < highScores[i + 1]) {
				temp = highScores[i];
				highScores[i] = highScores[i + 1];
				highScores[i + 1] = temp;
				noSwaps = false;
			}
		}
		boundary--;
	}
}

// for converting score to string to print it
string convertToString(int score) {
	if (score == 0) return "0";
	string str = "", temp = "";
	int digit;
	char character;
	while (score > 0) {
		digit = score % 10;
		character = '0' + digit;
		temp += character;
		score /= 10;
	}
	int length = 0;
	while (temp[length] != '\0') length++;
	for (int i = length - 1; i >= 0; i--) {
		str += temp[i];
	}
	return str;
}