#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath>
#include <fstream>
#include <vector>


using namespace std;
using namespace sf;

// Constants
const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;

const int gameRows = resolutionX / boxPixelsX;      // 30
const int gameColumns = resolutionY / boxPixelsY;   // 20


const int highScoreCount = 10;
string playername = "Mustafa";
// Enum for game states
enum GameState { MENU, PLAYING, EXIT, LEVEL_SELECT, GAME_OVER, GAME_COMPLETED,HIGH_SCORE };
//High score struct
struct HighScore {
    string name = "Mustafa";
    int score;
};


vector<HighScore> highScores;


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
// Write your functions declarations here. Some have been written for you. //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


// Function declarations
//draw player
void drawPlayer(RenderWindow& window, float player_x, float player_y, Sprite& playerSprite);
//Move bullet
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
//draw bullet
void drawBullet(RenderWindow& window, float bullet_x, float bullet_y, Sprite& bulletSprite);
//draw cans on side
void drawSprayCans(RenderWindow& window, Sprite& spraySprite, int totalSprayCans, int sprayLevels[], int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns, Font& font);
void useSpray(int sprayLevels[], int& currentSprayIndex, int& bulletsFired, int sprayReductionThreshold, int totalSprayCans, int& sprayCansLeft);
//Check Function for sprays
bool areAllSprayCansEmpty(int sprayLevels[], int totalSprayCans);
//main menu function
void drawMenu(RenderWindow& window, Text menuOptions[], int menuSize, int selectedIndex);
//submenu function
void drawSubMenu(RenderWindow& window, Text subMenuOptions[], int subMenuSize, int selectedIndex);
// Spawn bEES
void spawnBees(float beePositionsX[], float beePositionsY[], float beeDirections[], bool isFastBee[], int& beesSpawned, Clock& spawnClock, int totalBees, int fastBees, bool beeActive[], bool beesExiting[], int& fastBeesSpawned);
//Hive function
bool checkAndCreateHiveForBee(int i,
    float beePositionsX[], float beePositionsY[], float beeDirections[],
    bool beeActive[], bool beesExiting[],
    const bool isHoneycomb[], const float honeycombX[], const float honeycombY[],
    int beesSpawned,
    const float preHoneycombX[], const float preHoneycombY[], const bool preHoneycombActive[], int totalPreHoneycombs,
    bool beehiveActive[], float beehiveX[], float beehiveY[],
    int maxBeehives, int& beehivesCreated);
//Function to move bees
void moveBees(float beePositionsX[], float beePositionsY[], float honeycombX[], float honeycombY[], float beeDirections[], bool isFastBee[], bool isRedHoneycombActive[],
    int beesSpawned, bool beeActive[], bool beePaused[], float beePauseTime[], Clock& beeMoveClock, int resolutionX,
    int boxPixelsX, int boxPixelsY, int gameColumns, int& firstBeeReachedBottom, bool flowers[], int& leftFlowerIndex,
    int& rightFlowerIndex, bool& firstBeeTriggered, bool& placeLeftFlowerNext, float player_y, bool& levelNeedsReset,
    bool beesExiting[], bool isHoneycomb[], float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs,
    bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated);

void drawBees(RenderWindow& window, float beePositionsX[], float beePositionsY[], Sprite beeSprites[], Sprite fastBeeSprite, bool isFastBee[], bool beeActive[], int beesSpawned);

// Updated to include isRedHoneycombActive array
void BulletBeeCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float beePositionsX[], float beePositionsY[], bool beeActive[], bool isHoneycomb[], bool isFastBee[], bool isRedHoneycombActive[], int totalBees, float honeycombX[], float honeycombY[], int& beesDestroyed, bool flowers[], int& leftFlowerIndex, int& rightFlowerIndex, int gameRows, int& score);

// Updated to include isRedHoneycombActive array
void BulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float honeycombX[], float honeycombY[], bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score);

// Updated to include isRedHoneycombActive array
void drawHoneycombs(RenderWindow& window, float honeycombX[], float honeycombY[], Sprite& honeycombSprite, Sprite& redHoneycombSprite, bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees);
//Level setting function
void setupLevel(int currentLevel, int& totalBees, int& fastBees, int& totalHoneycombs, Texture& backgroundTexture,
    Music& bgMusic, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[],
    int maxPreHoneycombs, int& totalPreHoneycombs);
//humming bird movement function
void moveHummingbird(float& hummingbird_x, float& hummingbird_y, float& hummingbird_directionX, float& hummingbird_directionY, bool& hummingbird_paused, float& hummingbird_pauseTime, Clock& hummingbirdMoveClock, float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY);
//draw bird
void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite, float hummingbird_x, float hummingbird_y, bool hummingbird_visible);

// Updated to include isRedHoneycombActive array
void HummingbirdHoneycombCollision(float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, float hummingbird_x, float hummingbird_y, int& score, int boxPixelsX, int boxPixelsY, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool isRedHoneycombActive[]);
//Bullet and humming bird collision check function
void BulletHummingbirdCollision(float bullet_x, float bullet_y, bool& bullet_exists, float hummingbird_x, float hummingbird_y, int& hummingbird_health, bool& hummingbird_visible, Clock& hummingbirdRespawnClock, Sprite& hummingbirdSprite, int boxPixelsX, int boxPixelsY);
//flowers spawning function
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, bool flowers[], int gameRows, int boxPixelsX, int boxPixelsY, int resolutionY);
//Player sprite restriction due to flowers function
void checkPlayerRestriction(float& player_x, bool flowers[], int gameRows, int boxPixelsX, int& sprayCansLeft, int totalSprayCans, int sprayLevels[], int& currentSprayIndex, int maxSpraysPerCan, GameState& currentState);
//Beehive spawn and setting
void initBeeHives(bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated);
void drawBeeHives(RenderWindow& window, Sprite& beehiveSprite,
    const bool beehiveActive[], const float beehiveX[], const float beehiveY[],
    int beehivesCreated);

// New function to handle beehive completion and scoring
void BeehiveCompletion(bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score);

// New function to draw spray counts
void drawSprayCounts(RenderWindow& window, Font& font, int sprayLevels[], int totalSprayCans, int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns);
void loadHighScores();
void saveHighScores();
void displayHighScores(RenderWindow& window, Font& font);
void updateHighScores(string name, int score);

int main() {
    srand(static_cast<unsigned>(time(0)));
    //Load high score
    loadHighScores();

   
    Font font;
    if (!font.loadFromFile("Textures/Font1.ttf")) {
        cout << "Failed to load font" << endl;
        return -1;
    }

    GameState currentState = MENU;
    //Initializing Variables
    int selectedMenuIndex = 0;
    int selectedLevelIndex = 0;
    int currentLevel = 1;
    int score = 0; // Initialize score
    int highscore = 10;
    //Player Data
    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;
    //Bee and Honeycomb Data
    const int maxBees = 30;
    const int maxBeehives = 10;
    const int maxPreHoneycombs = 50;
    int totalHoneycombs = 0;

    bool isHoneycomb[maxBees] = { false };

    bool isRedHoneycombActive[maxBees] = { false }; // New array for red honeycombs
    float honeycombX[maxBees], honeycombY[maxBees];
    //Spray and Bullet Data
    const int maxSpraysPerCan = 56;
    const int totalSprayCans = 3;
    int sprayLevels[totalSprayCans] = { maxSpraysPerCan, maxSpraysPerCan, maxSpraysPerCan };
    int currentSprayIndex = 0;
    int bulletsFired = 0;
    const int sprayReductionThreshold = 8;
    int sprayCansLeft = totalSprayCans;
    float bullet_x = player_x;
    float bullet_y = player_y;
    bool bullet_exists = false;
    //Bees Data
    int beesSpawned = 0;
    int totalBees = 0;
    int fastBees = 0;
    int fastBeesSpawned = 0;
    int beesDestroyed = 0;
    int firstBeeReachedBottom = 0;
    bool firstBeeTriggered = false;
    float beePositionsX[maxBees];
    float beePositionsY[maxBees];
    float beeDirections[maxBees];
    bool beeActive[maxBees] = { false };
    bool beePaused[maxBees] = { false };
    float beePauseTime[maxBees] = { 0.0f };
    bool beesExiting[maxBees] = { false };
    bool isFastBee[maxBees] = { false };
    bool placeLeftFlowerNext = true;
    bool levelNeedsReset = false;
  
    //Humming Bird Data
    float hummingbird_x = resolutionX / 2.0f;
    float hummingbird_y = resolutionY / 2.0f;
    float hummingbird_directionX = 0.0f;
    float hummingbird_directionY = 0.0f;
    bool hummingbird_paused = false;
    float hummingbird_pauseTime = 0.0f;
    int hummingbird_health = 3;
    bool hummingbird_visible = true;

    //Inititalizing Positions
    for (int i = 0; i < maxBees; i++) {
        beePositionsX[i] = 0.0f;
        beePositionsY[i] = 0.0f;
        beeDirections[i] = 1.0f;
        honeycombX[i] = 0.0f;
        honeycombY[i] = 0.0f;
    }

    //Beehive data
    bool beehiveActive[maxBeehives];
    float beehiveX[maxBeehives];
    float beehiveY[maxBeehives];
    int beehivesCreated = 0;
  //Flower
    bool flowers[gameRows];
    for (int i = 0; i < gameRows; i++) {
        flowers[i] = false;
    }
    int leftFlowerIndex = -1;
    int rightFlowerIndex = gameRows;
    // Initial Honeycombs
    float pregenHoneycombX[maxPreHoneycombs];
    float pregenHoneycombY[maxPreHoneycombs];
    bool pregenHoneycombActive[maxPreHoneycombs] = { false };
    int totalPreHoneycombs = 0;
    
    //boss - specific variables
       


    // Load Textures
    Texture beeTexture, fastBeeTexture, honeycombTexture, redHoneycombTexture, backgroundTexture, hummingbirdTexture, beehiveTexture, flowerTexture, playerTexture, bulletTexture;
    if (!beeTexture.loadFromFile("Textures/Regular_bee.png")) {
        cout << "Failed to load bee.png" << endl;
    }
    if (!fastBeeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Failed to load fast_bee.png" << endl;
    }
    if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
        cout << "Failed to load honeycomb.png" << endl;
    }
    if (!redHoneycombTexture.loadFromFile("Textures/honeycomb_red.png")) {
        cout << "Failed to load honeycomb_red.png" << endl;
    }
    if (!hummingbirdTexture.loadFromFile("Textures/bird.png")) {
        cout << "Failed to load hummingbird.png" << endl;
    }
    if (!beehiveTexture.loadFromFile("Textures/hive.png")) {
        cout << "Failed to load hive.png" << endl;
    }
    if (!flowerTexture.loadFromFile("Textures/obstacles.png")) {
        cout << "Failed to load flower.png" << endl;
    }
    if (!playerTexture.loadFromFile("Textures/spray.png")) {
        cout << "Failed to load spray.png" << endl;
    }
    if (!bulletTexture.loadFromFile("Textures/bullet.png")) {
        cout << "Failed to load bullet.png" << endl;
    }

    // Initialize Sprites
    //Flower Sprite
    Sprite flowerSprite(flowerTexture);
    flowerSprite.setScale(2.0f, 2.0f);
    //Honeycomb Sprite
    Sprite honeycombSprite(honeycombTexture);
    Sprite redHoneycombSprite(redHoneycombTexture);
    //Bees Sprite
    Sprite beeSprites[maxBees];
    for (int i = 0; i < maxBees; i++) {
        beeSprites[i].setTexture(beeTexture);
        beeSprites[i].setScale(1.0f, 1.0f);
    }
    //Fast Bee Sprite
    Sprite fastBeeSprite(fastBeeTexture);
    fastBeeSprite.setScale(1.0f, 1.0f);
    //Beehive Sprite
    Sprite beehiveSprite(beehiveTexture);
    Sprite playerSpriteObj(playerTexture);
    playerSpriteObj.setScale(1.0f, 1.0f);
    //Bullet Sprite
    Sprite bulletSpriteObj(bulletTexture);
    bulletSpriteObj.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));
    //HummingBirdSprite
    Sprite hummingbirdSprite(hummingbirdTexture);
    hummingbirdSprite.setScale(2.0f, 2.0f);
    hummingbirdSprite.setTextureRect(IntRect(0, 0, 32, 32));

    RectangleShape groundRectangle(Vector2f(resolutionX, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);
    //Player Sprite
    Sprite spraySprite(playerTexture);
    spraySprite.setScale(1, 1);

    Sprite backgroundSprite;

    // Load Sounds
    SoundBuffer bulletFireBuffer;
    if (!bulletFireBuffer.loadFromFile("Sound Effects/fire.wav")) {
        cout << "Failed to load fire.wav" << endl;
    }
    Sound bulletFireSound;
    bulletFireSound.setBuffer(bulletFireBuffer);

    Music bgMusic;
    bgMusic.setVolume(10);

   

    // Initialize Menu Texts
    const int menuSize = 4;
    Text menuOptions[menuSize];
    //Ptr Declaration for Texts
    const char* menuTexts[menuSize] = { "Start Game", "Select Level", "Highscores","Exit"};
    for (int i = 0; i < menuSize; i++) {
        menuOptions[i].setFont(font);
        menuOptions[i].setString(menuTexts[i]);
        menuOptions[i].setCharacterSize(50);
        menuOptions[i].setPosition(340.0f, 190.0f + i * 80.0f);
        menuOptions[i].setFillColor(Color::White);
    }
    //Submenu Setting
    const int subMenuSize = 4;
    Text subMenuOptions[subMenuSize];
    const char* subMenuTexts[subMenuSize] = { "Level 1", "Level 2", "Level 3","Level 4"};
    for (int i = 0; i < subMenuSize; i++) {
        subMenuOptions[i].setFont(font);
        subMenuOptions[i].setString(subMenuTexts[i]);
        subMenuOptions[i].setCharacterSize(50);
        subMenuOptions[i].setPosition(340.0f, 190.0f + i * 80.0f);
        subMenuOptions[i].setFillColor(Color::White);
    }
    //Declaring Window
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Bee Game", Style::Close);
    window.setPosition(Vector2i(500, 200));

    //Declaring Clocks
    Clock spawnClock, beeMoveClock, bulletClock, hummingbirdMoveClock, hummingbirdRespawnClock, hummingbirdAnimationClock;

    // Initialize beehives
    initBeeHives(beehiveActive, beehiveX, beehiveY, maxBeehives, beehivesCreated);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                //Menu Condition
                if (currentState == MENU) {
                    if (event.key.code == Keyboard::Up) {
                        selectedMenuIndex = (selectedMenuIndex - 1 + menuSize) % menuSize;
                    }
                    else if (event.key.code == Keyboard::Down) {
                        selectedMenuIndex = (selectedMenuIndex + 1) % menuSize;
                    }
                    else if (event.key.code == Keyboard::Enter) {
                        if (selectedMenuIndex == 0) {
                            // Start Game with currentLevel
                            setupLevel(currentLevel, totalBees, fastBees, totalHoneycombs, backgroundTexture, bgMusic,
                                pregenHoneycombX, pregenHoneycombY, pregenHoneycombActive,
                                maxPreHoneycombs, totalPreHoneycombs);
                            backgroundSprite.setTexture(backgroundTexture);
                            currentState = PLAYING;

                            // Reset game variables
                            beesSpawned = 0;

                            fastBeesSpawned = 0;
                            for (int i = 0; i < maxBees; i++) {
                                beeActive[i] = false;
                                isHoneycomb[i] = false;
                                isFastBee[i] = false;
                                isRedHoneycombActive[i] = false; // Initialize red honeycombs
                                beesExiting[i] = false;
                                beePaused[i] = false;
                                beePauseTime[i] = 0.0f;
                            }

                            for (int i = 0; i < totalSprayCans; i++) {
                                sprayLevels[i] = maxSpraysPerCan;
                            }
                            sprayCansLeft = totalSprayCans;
                            currentSprayIndex = 0;
                            bulletsFired = 0;
                            bullet_exists = false;

                            hummingbird_x = resolutionX / 2.0f;
                            hummingbird_y = resolutionY / 2.0f;
                            hummingbird_directionX = 0.0f;
                            hummingbird_directionY = 0.0f;
                            hummingbird_paused = false;
                            hummingbird_pauseTime = 0.0f;
                            hummingbird_health = 3;
                            hummingbird_visible = true;
                            hummingbirdSprite.setColor(Color::White);

                            for (int i = 0; i < maxBeehives; i++) {
                                beehiveActive[i] = false;
                                beehiveX[i] = 0.0f;
                                beehiveY[i] = 0.0f;
                            }
                            beehivesCreated = 0; // Initialize once after setup

                            for (int i = 0; i < gameRows; i++) {
                                flowers[i] = false;
                            }
                            leftFlowerIndex = -1;
                            rightFlowerIndex = gameRows;
                            beesDestroyed = 0;
                            firstBeeReachedBottom = 0;
                            firstBeeTriggered = false;
                            placeLeftFlowerNext = true;
                            levelNeedsReset = false;

                            player_x = (gameRows / 2) * boxPixelsX;
                            player_y = (gameColumns - 4) * boxPixelsY;

                        }
                        else if (selectedMenuIndex == 1) {
                            currentState = LEVEL_SELECT;
                        }
                        else if (selectedMenuIndex == 2) {
                            currentState = HIGH_SCORE;
                            loadHighScores();
                            displayHighScores(window, font);
                        }
                        else if (selectedMenuIndex == 3) {
                            window.close();
                        }
                    }
                }
                else if (currentState == HIGH_SCORE) {
                    /*if (highScores.empty()) {
                        loadHighScores();
                        
                    }*/
                    displayHighScores(window, font);

                    if (event.key.code == Keyboard::Escape) {
                        currentState = MENU;
                    }

            }

                else if (currentState == LEVEL_SELECT) {
                    if (event.key.code == Keyboard::Up) {
                        selectedLevelIndex = (selectedLevelIndex - 1 + subMenuSize) % subMenuSize;
                    }
                    else if (event.key.code == Keyboard::Down) {
                        selectedLevelIndex = (selectedLevelIndex + 1) % subMenuSize;
                    }
                    else if (event.key.code == Keyboard::Enter) {
                        currentLevel = selectedLevelIndex + 1;
                        currentState = MENU;
                    }
                    else if (event.key.code == Keyboard::Escape) {
                        currentState = MENU;
                    }
                }
                else if (currentState == PLAYING) {
                    int playerColumn = static_cast<int>(player_x / boxPixelsX);
                    if (event.key.code == Keyboard::Left) {
                        if (playerColumn > 0 && !flowers[playerColumn - 1]) {
                            player_x -= boxPixelsX;
                        }
                    }
                    else if (event.key.code == Keyboard::Right) {
                        if (playerColumn < gameRows - 1 && !flowers[playerColumn + 1]) {
                            player_x += boxPixelsX;
                        }
                    }

                    if (event.key.code == Keyboard::Space && !bullet_exists) {
                        if (!areAllSprayCansEmpty(sprayLevels, totalSprayCans)) {
                            bullet_x = player_x;
                            bullet_y = player_y;
                            bullet_exists = true;
                            bulletFireSound.play();
                            useSpray(sprayLevels, currentSprayIndex, bulletsFired, sprayReductionThreshold, totalSprayCans, sprayCansLeft);
                        }
                        else {
                            currentState = GAME_OVER;
                        }
                    }
                }
                else if (currentState == GAME_OVER || currentState == GAME_COMPLETED) {
                    if (event.key.code == Keyboard::Enter) {
                        updateHighScores("Mustafa", score);
                        // Reset game state
                        currentState = MENU;
                        score = 0;
                        currentLevel = 1; // Reset to Level 1
                        
                        
                    }
                }

            }
        }

        window.clear();

        if (currentState == MENU) {
            drawMenu(window, menuOptions, menuSize, selectedMenuIndex);
        }
        else if (currentState == LEVEL_SELECT) {
            drawSubMenu(window, subMenuOptions, subMenuSize, selectedLevelIndex);
        }
        else if (currentState == PLAYING) {
            window.draw(backgroundSprite);

            // Draw pre-generated honeycombs
            for (int i = 0; i < totalPreHoneycombs; i++) {
                if (pregenHoneycombActive[i]) {
                    honeycombSprite.setPosition(pregenHoneycombX[i], pregenHoneycombY[i]);
                    window.draw(honeycombSprite);
                }
            }

            // Handle bullet rendering and movement
            if (bullet_exists) {
                drawBullet(window, bullet_x, bullet_y, bulletSpriteObj);
                moveBullet(bullet_y, bullet_exists, bulletClock);
                BulletBeeCollision(bullet_x, bullet_y, bullet_exists, beePositionsX, beePositionsY, beeActive, isHoneycomb, isFastBee, isRedHoneycombActive, totalBees, honeycombX, honeycombY, beesDestroyed, flowers, leftFlowerIndex, rightFlowerIndex, gameRows, score);
                BulletHoneycombCollision(bullet_x, bullet_y, bullet_exists, honeycombX, honeycombY, isHoneycomb, isRedHoneycombActive, totalBees, pregenHoneycombX, pregenHoneycombY, pregenHoneycombActive, totalPreHoneycombs, beehiveActive, beehiveX, beehiveY, beehivesCreated, score);
                BulletHummingbirdCollision(bullet_x, bullet_y, bullet_exists, hummingbird_x, hummingbird_y, hummingbird_health, hummingbird_visible, hummingbirdRespawnClock, hummingbirdSprite, boxPixelsX, boxPixelsY);
            }

            // Spawn bees
            spawnBees(beePositionsX, beePositionsY, beeDirections, isFastBee, beesSpawned, spawnClock, totalBees, fastBees, beeActive, beesExiting, fastBeesSpawned);

            // Move bees
            moveBees(
                beePositionsX, beePositionsY, honeycombX, honeycombY, beeDirections, isFastBee, isRedHoneycombActive, beesSpawned, beeActive, beePaused, beePauseTime,
                beeMoveClock, resolutionX, boxPixelsX, boxPixelsY, gameColumns, firstBeeReachedBottom, flowers, leftFlowerIndex, rightFlowerIndex,
                firstBeeTriggered, placeLeftFlowerNext, player_y, levelNeedsReset,beesExiting, isHoneycomb, pregenHoneycombX, pregenHoneycombY, pregenHoneycombActive, totalPreHoneycombs,
                beehiveActive, beehiveX, beehiveY, maxBeehives, beehivesCreated
            );

            // Move hummingbird
            if (hummingbird_visible) {
                moveHummingbird(hummingbird_x, hummingbird_y, hummingbird_directionX, hummingbird_directionY,
                    hummingbird_paused, hummingbird_pauseTime, hummingbirdMoveClock,
                    honeycombX, honeycombY, isHoneycomb, beesSpawned, resolutionX, resolutionY, boxPixelsX, boxPixelsY);
            }

            // Animate hummingbird
            if (hummingbird_visible && hummingbirdAnimationClock.getElapsedTime().asMilliseconds() > 190) {
                IntRect currentRect = hummingbirdSprite.getTextureRect();
                if (currentRect.left == 0) {
                    hummingbirdSprite.setTextureRect(IntRect(32, 0, 32, 32));
                }
                else {
                    hummingbirdSprite.setTextureRect(IntRect(0, 0, 32, 32));
                }
                hummingbirdAnimationClock.restart();
            }

            // Handle hummingbird collisions and scoring
            HummingbirdHoneycombCollision(honeycombX, honeycombY, isHoneycomb, beesSpawned, hummingbird_x, hummingbird_y, score, boxPixelsX, boxPixelsY, pregenHoneycombX, pregenHoneycombY, pregenHoneycombActive, totalPreHoneycombs, isRedHoneycombActive);

            window.draw(groundRectangle);

            // Draw game elements
            drawSprayCans(window, spraySprite, totalSprayCans, sprayLevels, currentSprayIndex, boxPixelsX, boxPixelsY, gameColumns, font);
            drawSprayCounts(window, font, sprayLevels, totalSprayCans, currentSprayIndex, boxPixelsX, boxPixelsY, gameColumns);
            drawPlayer(window, player_x, player_y, playerSpriteObj);
            drawBees(window, beePositionsX, beePositionsY, beeSprites, fastBeeSprite, isFastBee, beeActive, beesSpawned);
            drawHoneycombs(window, honeycombX, honeycombY, honeycombSprite, redHoneycombSprite, isHoneycomb, isRedHoneycombActive, beesSpawned);
            drawFlowers(window, flowerSprite, flowers, gameRows, boxPixelsX, boxPixelsY, resolutionY);
            drawHummingbird(window, hummingbirdSprite, hummingbird_x, hummingbird_y, hummingbird_visible);

            // Draw beehives
            drawBeeHives(window, beehiveSprite, beehiveActive, beehiveX, beehiveY, beehivesCreated);

            // Display score by setting its position
            Text scoreText;
            scoreText.setFont(font);
            scoreText.setString("Score: " + to_string(score));
            scoreText.setCharacterSize(50);
            scoreText.setFillColor(Color::White);
            scoreText.setPosition(640, 570);
            window.draw(scoreText);

            // Handle hummingbird respawn
            if (!hummingbird_visible && hummingbirdRespawnClock.getElapsedTime().asSeconds() >= 5.0f) {
                hummingbird_visible = true;
                hummingbird_health = 3;
                hummingbirdSprite.setColor(Color::White);
                hummingbird_x = (resolutionX+2.0f) / 1.90f;
                hummingbird_y = (resolutionY+2.0f) / 1.90f;
            }

            // Check player restrictions
            checkPlayerRestriction(player_x, flowers, gameRows, boxPixelsX, sprayCansLeft, totalSprayCans, sprayLevels, currentSprayIndex, maxSpraysPerCan, currentState);

            // Handle level reset if needed
            if (levelNeedsReset) {
                for (int i = 0; i < gameRows; i++) {
                    flowers[i] = false;
                }
                leftFlowerIndex = -1;
                rightFlowerIndex = gameRows;

                if (sprayCansLeft > 0) {
                    sprayCansLeft--;
                    for (int i = 0; i < totalSprayCans; i++) {
                        sprayLevels[i] = maxSpraysPerCan;
                    }
                    currentSprayIndex = 0;
                }
                else {
                    currentState = GAME_OVER;
                }

                player_x = (gameRows / 2) * boxPixelsX;
                player_y = (gameColumns - 4) * boxPixelsY;

                for (int i = 0; i < maxBees; i++) {
                    beeActive[i] = false;
                    beesExiting[i] = false;
                    isHoneycomb[i] = false;
                    isFastBee[i] = false;
                    isRedHoneycombActive[i] = false; // Reset red honeycombs
                }

                firstBeeTriggered = false;
                placeLeftFlowerNext = true;
                beesSpawned = 0;
                levelNeedsReset = false;
            }

            // Check if all bees are gone to end the game or proceed to next level
            if (beesSpawned >= totalBees) {
                bool allbeesGone = true;
                for (int i = 0; i < beesSpawned; i++) {
                    if (beeActive[i]) {
                        allbeesGone = false;
                        break;
                    }
                }
                if (allbeesGone) {
                    // Handle beehive completion and scoring
                    BeehiveCompletion(beehiveActive, beehiveX, beehiveY, beehivesCreated, score);

                    // Determine if there is a next level
                    if (currentLevel < 3) {
                        currentLevel++; // Proceed to next level
                        currentState = PLAYING; // Continue playing
                        setupLevel(currentLevel, totalBees, fastBees, totalHoneycombs, backgroundTexture, bgMusic,
                            pregenHoneycombX, pregenHoneycombY, pregenHoneycombActive,
                            maxPreHoneycombs, totalPreHoneycombs);
                        backgroundSprite.setTexture(backgroundTexture);

                        // Reset game variables for the new level
                        beesSpawned = 0;
                        fastBeesSpawned = 0;
                        for (int i = 0; i < maxBees; i++) {
                            beeActive[i] = false;
                            isHoneycomb[i] = false;
                            isFastBee[i] = false;
                            isRedHoneycombActive[i] = false; // Initialize red honeycombs
                            beesExiting[i] = false;
                            beePaused[i] = false;
                            beePauseTime[i] = 0.0f;
                        }

                        for (int i = 0; i < totalSprayCans; i++) {
                            sprayLevels[i] = maxSpraysPerCan;
                        }
                        sprayCansLeft = totalSprayCans;
                        currentSprayIndex = 0;
                        bulletsFired = 0;
                        bullet_exists = false;
                        // Humming bird Setting 
                        hummingbird_x = resolutionX / 2.0f;
                        hummingbird_y = resolutionY / 2.0f;
                        hummingbird_directionX = 0.0f;
                        hummingbird_directionY = 0.0f;
                        hummingbird_paused = false;
                        hummingbird_pauseTime = 0.0f;
                        hummingbird_health = 3;
                        hummingbird_visible = true;
                        hummingbirdSprite.setColor(Color::White);

                        for (int i = 0; i < maxBeehives; i++) {
                            beehiveActive[i] = false;
                            beehiveX[i] = 0.0f;
                            beehiveY[i] = 0.0f;
                        }
                        beehivesCreated = 0; // Reset beehives

                        for (int i = 0; i < gameRows; i++) {
                            flowers[i] = false;
                        }
                        leftFlowerIndex = -1;
                        rightFlowerIndex = gameRows;
                        beesDestroyed = 0;
                        firstBeeReachedBottom = 0;
                        firstBeeTriggered = false;
                        placeLeftFlowerNext = true;
                        levelNeedsReset = false;

                        player_x = (gameRows / 2) * boxPixelsX;
                        player_y = (gameColumns - 4) * boxPixelsY;

                    }
                    else {
                        // If last level completed, show game completed screen
                        currentState = GAME_COMPLETED;
                        bgMusic.stop();
                    }
                }
            }
        }
        else if (currentState == GAME_OVER || currentState == GAME_COMPLETED) {
            // Display appropriate messages
            if (currentState == GAME_OVER) {

                //Text setting
                Text gameOverText;
                gameOverText.setFont(font);
                gameOverText.setString("Game Over!\nPress Enter to return to Menu");
                gameOverText.setCharacterSize(50);
                gameOverText.setFillColor(Color::Red);
                gameOverText.setPosition(150, 300);
                window.draw(gameOverText);

                // Display final score
                Text finalScoreText;
                finalScoreText.setFont(font);
                finalScoreText.setString("Final Score: " + to_string(score));
                finalScoreText.setCharacterSize(40);
                finalScoreText.setFillColor(Color::White);
                finalScoreText.setPosition(300, 500);
                window.draw(finalScoreText);
            }
            else if (currentState == GAME_COMPLETED) {
                //Game Completed text
                Text gameCompletedText;
                gameCompletedText.setFont(font);
                gameCompletedText.setString("Congratulations!\nYou've completed all levels!\nPress Enter to return to Menu");
                gameCompletedText.setCharacterSize(50);
                gameCompletedText.setFillColor(Color::Green);
                gameCompletedText.setPosition(150, 250);
                window.draw(gameCompletedText);

                // Display final score
                Text finalScoreText;
                finalScoreText.setFont(font);
                finalScoreText.setString("Final Score: " + to_string(score));
                finalScoreText.setCharacterSize(40);
                finalScoreText.setFillColor(Color::White);
                finalScoreText.setPosition(300, 500);
                window.draw(finalScoreText);
            }
        }

        window.display();
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


void drawPlayer(RenderWindow& window, float player_x, float player_y, Sprite& playerSprite) {
    //set player 
    playerSprite.setPosition(player_x, player_y);
    window.draw(playerSprite);
}

void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
    // Bullet movement criteria
    if (bulletClock.getElapsedTime().asMilliseconds() > 10) {
        bullet_y -= 30.0f;
        bulletClock.restart();
    }
    if (bullet_y < 0) {
        //bullet dissappear when exit screen
        bullet_exists = false;
    }
}

void drawBullet(RenderWindow& window, float bullet_x, float bullet_y, Sprite& bulletSprite) {
    //set bullet
    bulletSprite.setScale(2, 2);
    bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}

void drawSprayCans(RenderWindow& window, Sprite& spraySprite, int totalSprayCans, int sprayLevels[], int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns, Font& font) {
    for (int i = 0; i < totalSprayCans; i++) {
        spraySprite.setPosition((float)i * (boxPixelsX + 10), (float)(gameColumns - 2) * boxPixelsY);
        //red when can finish
        if (sprayLevels[i] <= 0) {
            spraySprite.setColor(Color::Red);
        }
        else {
            spraySprite.setColor(Color::White);
        }
        window.draw(spraySprite);

        // Highlight the current spray can
        if (i == currentSprayIndex) {
            RectangleShape outline(Vector2f(boxPixelsX * spraySprite.getScale().x, boxPixelsY * spraySprite.getScale().y));
            outline.setPosition((float)i * (boxPixelsX + 10), (float)(gameColumns - 2) * boxPixelsY);
            outline.setFillColor(Color::Transparent);
            outline.setOutlineThickness(2.0f);
            outline.setOutlineColor(Color::Yellow);
            window.draw(outline);
        }
    }
}

void drawSprayCounts(RenderWindow& window, Font& font, int sprayLevels[], int totalSprayCans, int currentSprayIndex, int boxPixelsX, int boxPixelsY, int gameColumns) {
    for (int i = 0; i < totalSprayCans; i++) {
        // Create a text object for each spray can
        Text sprayCountText;
        sprayCountText.setFont(font);
        sprayCountText.setString(to_string(sprayLevels[i]));
        sprayCountText.setCharacterSize(20);
        //Color and position
        sprayCountText.setFillColor(Color::White);
        sprayCountText.setPosition((float)i * (boxPixelsX + 10) + 10, (float)(gameColumns - 2) * boxPixelsY - 30);
        window.draw(sprayCountText);
    }
}

void useSpray(int sprayLevels[], int& currentSprayIndex, int& bulletsFired, int sprayReductionThreshold, int totalSprayCans, int& sprayCansLeft) {
    //Reduction in spray level when  bullet fired
    if (sprayLevels[currentSprayIndex] > 0) {
        sprayLevels[currentSprayIndex]-=1;
        bulletsFired+=1;
        if (sprayLevels[currentSprayIndex] == 0) {
            //When one can finishes
            sprayCansLeft-=1;
            currentSprayIndex+=1;
            if (currentSprayIndex >= totalSprayCans) {
                //When all 3 finish
                currentSprayIndex = 0;
            }
        }
    }
}

bool areAllSprayCansEmpty(int sprayLevels[], int totalSprayCans) {
    //loop until all 3 empty
    for (int i = 0; i < totalSprayCans; i++) {
        if (sprayLevels[i] > 0)
        {
            return false;
        }
    }
    return true;
}

void drawMenu(RenderWindow& window, Text menuOptions[], int menuSize, int selectedIndex) {
    for (int i = 0; i < menuSize; i++) {
        //Yellow filling on selected option
        menuOptions[i].setFillColor(i == selectedIndex ? Color::Yellow : Color::White);
        window.draw(menuOptions[i]);
    }
}

void drawSubMenu(RenderWindow& window, Text subMenuOptions[], int subMenuSize, int selectedIndex) {
    for (int i = 0; i < subMenuSize; i++) {
        //Yellow filling on selected option
        subMenuOptions[i].setFillColor(i == selectedIndex ? Color::Yellow : Color::White);
        window.draw(subMenuOptions[i]);
    }
}
//draw bees
void spawnBees(float beePositionsX[], float beePositionsY[], float beeDirections[], bool isFastBee[], int& beesSpawned, Clock& spawnClock, int totalBees, int fastBees, bool beeActive[], bool beesExiting[], int& fastBeesSpawned) {
    const float spawn_Interval = 2.0f;
    if (spawnClock.getElapsedTime().asSeconds() >= spawn_Interval && beesSpawned < totalBees) {
        //initial bee position
        beePositionsX[beesSpawned] = 0.0f;
        beePositionsY[beesSpawned] = 0;
        beeDirections[beesSpawned] = 1.0f;
        beeActive[beesSpawned] = true;
        beesExiting[beesSpawned] = false;

        if (fastBeesSpawned < fastBees) {
            isFastBee[beesSpawned] = true;
            fastBeesSpawned+=1;
        }
        else {
            isFastBee[beesSpawned] = false;
        }

        beesSpawned+=1;
        spawnClock.restart();
    }
}

bool checkAndCreateHiveForBee(int i,
    float beePositionsX[], float beePositionsY[], float beeDirections[], bool beeActive[], bool beesExiting[],const bool isHoneycomb[], const float honeycombX[], const float honeycombY[],int beesSpawned,
    const float preHoneycombX[], const float preHoneycombY[], const bool preHoneycombActive[], int totalPreHoneycombs, bool beehiveActive[], float beehiveX[], float beehiveY[],int maxBeehives, int& beehivesCreated) {
    if (!beeActive[i] || beesExiting[i])
        return false;
    float bee_X = beePositionsX[i];
    float bee_Y = beePositionsY[i];
    float beeWidth = (float)boxPixelsX;
    float beeHeight = (float)boxPixelsY;
    bool leftHoneycomb = false;
    bool rightHoneycomb = false;

    // Check honeycombs from bees
    for (int j = 0; j < beesSpawned; j++) {
        if (isHoneycomb[j]) {
            // Check left side honeycomb
            if (bee_X - beeWidth > honeycombX[j] && (bee_X - beeWidth) < (honeycombX[j] + boxPixelsX) &&
                bee_Y + beeHeight > honeycombY[j] && bee_Y < honeycombY[j] + boxPixelsY) {
                leftHoneycomb = true;
            }

            // Check right side honeycomb
            if ((bee_X + beeWidth) > honeycombX[j] && (bee_X + beeWidth) < (honeycombX[j] + boxPixelsX) &&
                bee_Y + beeHeight > honeycombY[j] && bee_Y < honeycombY[j] + boxPixelsY) {
                rightHoneycomb = true;
            }
        }
    }

    // Check pre-generated honeycombs
    for (int h = 0; h < totalPreHoneycombs; h++) {
        if (!preHoneycombActive[h])
        {
            continue;
        }
        //Check bees with honeycombs if get stuck
        // Check left side pre-honeycomb
        if (bee_X - beeWidth > preHoneycombX[h] && (bee_X - beeWidth) < (preHoneycombX[h] + boxPixelsX) &&
            bee_Y + beeHeight > preHoneycombY[h] && bee_Y < preHoneycombY[h] + boxPixelsY) {
            leftHoneycomb = true;
        }

        // Check right side pre-honeycomb
        if ((bee_X + beeWidth) > preHoneycombX[h] && (bee_X + beeWidth) < (preHoneycombX[h] + boxPixelsX) &&
            bee_Y + beeHeight > preHoneycombY[h] && bee_Y < preHoneycombY[h] + boxPixelsY) {
            rightHoneycomb = true;
        }
    }

    // If stuck between honeycombs on both sides, create a hive
    if (leftHoneycomb && rightHoneycomb && beehivesCreated < maxBeehives) {
        beeActive[i] = false;
        for (int j = 0; j < maxBeehives; j++) {
            if (!beehiveActive[j]) {
                //Display beehive
                beehiveActive[j] = true;
                //Beehive coordinates
                beehiveX[j] = bee_X;
                beehiveY[j] = bee_Y;
                beehivesCreated+=1;
                return true;
            }
        }
    }

    return false;
}

void moveBees(float beePositionsX[], float beePositionsY[], float honeycombX[], float honeycombY[], float beeDirections[], bool isFastBee[], bool isRedHoneycombActive[],
    int beesSpawned, bool beeActive[], bool beePaused[], float beePauseTime[], Clock& beeMoveClock, int resolutionX,int boxPixelsX, int boxPixelsY, int gameColumns, int& firstBeeReachedBottom, bool flowers[], int& leftFlowerIndex,
    int& rightFlowerIndex, bool& firstBeeTriggered, bool& placeLeftFlowerNext, float player_y, bool& levelNeedsReset,
    bool beesExiting[], bool isHoneycomb[], float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs,
    bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated) {

    //Movement Dynamics
    const float moveInterval = 50.0f;
    //Reg bee
    const float beeSpeed = 10.0f;
    //Fast bee
    const float beeSpeedFast = 20.0f;
    const float tierHeight = 30.0f;

    //Bee Dimensions
    int beeWidth = boxPixelsX;
    int beeHeight = boxPixelsY;
    
    float tierAbovePlayer = player_y - boxPixelsY;
    //Movement of bees
    if (beeMoveClock.getElapsedTime().asMilliseconds() > moveInterval) {
        float deltaTime = beeMoveClock.getElapsedTime().asSeconds();
        for (int i = 0; i < beesSpawned; i++) {
            if (!beeActive[i]) { continue; }

            if (beePaused[i]) {
                beePauseTime[i] -= deltaTime;
                if (beePauseTime[i] <= 0.0f) {
                    beePaused[i] = false;
                    beePauseTime[i] = 0.0f;
                }
                if (beePaused[i]) { continue; }
            }
            else {
                if ((rand() % 1000) < 5) {
                    beePaused[i] = true;
                    beePauseTime[i] = 1.0f;
                    continue;
                }
            }

            float currentSpeed = isFastBee[i] ? beeSpeedFast : beeSpeed;
            beePositionsX[i] += beeDirections[i] * currentSpeed;

            // For normal bees: check honeycomb collisions
            if (!beesExiting[i]) {
                if (!isFastBee[i]) {
                    // Check pre honeycomb collision for normal bees
                    for (int h = 0; h < totalPreHoneycombs; h++) {
                        if (!preHoneycombActive[h]) continue;
                        bool collidesX = (beePositionsX[i] + beeWidth > preHoneycombX[h]) && (beePositionsX[i] < preHoneycombX[h] + boxPixelsX);
                        bool collidesY = (beePositionsY[i] + beeHeight > preHoneycombY[h]) && (beePositionsY[i] < preHoneycombY[h] + boxPixelsY);
                        if (collidesX && collidesY) {
                            beeDirections[i] *= -1;
                            beePositionsY[i] += tierHeight;
                        }
                    }

                    // Check bee honeycomb collision
                    for (int j = 0; j < beesSpawned; j++) {
                        if (!isHoneycomb[j]) continue;
                        // If fast bee encountered red honeycomb: skip bounce
                        // But we are in normal bee section, so no check needed here.
                        bool collidesX = (beePositionsX[i] + beeWidth > honeycombX[j]) && (beePositionsX[i] < (honeycombX[j] + boxPixelsX));
                        bool collidesY = (beePositionsY[i] + beeHeight > honeycombY[j]) && (beePositionsY[i] < (honeycombY[j] + boxPixelsY));
                        if (collidesX && collidesY) {
                            beeDirections[i] *= -1;
                            beePositionsY[i] += tierHeight;
                            break;
                        }
                    }
                }
                else {
                    // Fast bees skip bounce if honeycomb is red
                    // They still bounce on normal honeycomb
                    for (int j = 0; j < beesSpawned; j++) {
                        if (!isHoneycomb[j]) continue;
                        bool collidesX = (beePositionsX[i] + beeWidth > honeycombX[j]) && (beePositionsX[i] < honeycombX[j] + boxPixelsX);
                        bool collidesY = (beePositionsY[i] + beeHeight > honeycombY[j]) && (beePositionsY[i] < honeycombY[j] + boxPixelsY);
                        if (collidesX && collidesY) {
                            if (!isRedHoneycombActive[j]) {
                                beeDirections[i] *= -1;
                                beePositionsY[i] += tierHeight;
                            }
                            break;
                        }
                    }
                }
            }

            // Boundaries
            if (!beesExiting[i]) {
                if (beePositionsX[i] <= 0 || beePositionsX[i] >= resolutionX - beeWidth) {
                    beeDirections[i] *= -1;
                    beePositionsY[i] += tierHeight;
                }
            }

            // Tier above player 
            //If bee exit 
            if (beePositionsY[i] >= tierAbovePlayer && !beesExiting[i]) {
                if (!firstBeeTriggered) {
                    firstBeeTriggered = true;
                    firstBeeReachedBottom = 1;

                    leftFlowerIndex+=1;
                    rightFlowerIndex-=1;

                    if (leftFlowerIndex >= 0 && leftFlowerIndex < gameRows) flowers[leftFlowerIndex] = true;
                    if (rightFlowerIndex >= 0 && rightFlowerIndex < gameRows) flowers[rightFlowerIndex] = true;
                }
                else {
                    //Check to place flower left
                    if (placeLeftFlowerNext) {
                        leftFlowerIndex+=1;
                        if (leftFlowerIndex >= 0 && leftFlowerIndex < gameRows) flowers[leftFlowerIndex] = true;
                        placeLeftFlowerNext = false;
                    }
                    else {
                        rightFlowerIndex-=1;
                        if (rightFlowerIndex >= 0 && rightFlowerIndex < gameRows) flowers[rightFlowerIndex] = true;
                        placeLeftFlowerNext = true;
                    }
                }
                beesExiting[i] = true;
            }

            if (beesExiting[i]) {
                beePositionsX[i] += beeDirections[i] * currentSpeed * 2.0f;
                if (beePositionsX[i] < -boxPixelsX || beePositionsX[i] > resolutionX + boxPixelsX) {
                    beeActive[i] = false;
                }
            }

            // After moving bees, create a hive if conditions met
            checkAndCreateHiveForBee(i, beePositionsX, beePositionsY, beeDirections, beeActive, beesExiting,
                isHoneycomb, honeycombX, honeycombY, beesSpawned,
                preHoneycombX, preHoneycombY, preHoneycombActive, totalPreHoneycombs,
                beehiveActive, beehiveX, beehiveY, maxBeehives, beehivesCreated);
        }

        // Check if player is trapped from flowers
        bool playerTrapped = true;
        //Loop for rows not completed
        for (int c = 0; c < gameRows; c++) {
            if (!flowers[c]) {
                
                playerTrapped = false;
                break;
            }
        }
        //level reset
        if (playerTrapped) levelNeedsReset = true;

        beeMoveClock.restart();
    }
}
//Function to draw bees
void drawBees(RenderWindow& window, float beePositionsX[], float beePositionsY[], Sprite beeSprites[], Sprite fastBeeSprite, bool isFastBee[], bool beeActive[], int beesSpawned) {
    for (int i = 0; i < beesSpawned; i++) {
        if (!beeActive[i])
        {
            continue;
        }
        if (isFastBee[i]) {
            //draw fastbee
            fastBeeSprite.setPosition(beePositionsX[i], beePositionsY[i]);
            window.draw(fastBeeSprite);
        }
        else {
            //draw reg bee
            beeSprites[i].setPosition(beePositionsX[i], beePositionsY[i]);
            window.draw(beeSprites[i]);
        }
    }
}

// Modified handleBulletBeeCollision with scoring
void BulletBeeCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float beePositionsX[], float beePositionsY[], bool beeActive[], bool isHoneycomb[], bool isFastBee[], bool isRedHoneycombActive[], int totalBees, float honeycombX[], float honeycombY[], int& beesDestroyed, bool flowers[], int& leftFlowerIndex, int& rightFlowerIndex, int gameRows, int& score) {
    for (int i = 0; i < totalBees; i++) {
        //condition to check if bullet and bee exist and their coordinates coincide
        if (bullet_exists && beeActive[i] &&
            bullet_x + (boxPixelsX * 2) > beePositionsX[i] && bullet_x < beePositionsX[i] + boxPixelsX &&
            bullet_y + (boxPixelsY * 2) > beePositionsY[i] && bullet_y < beePositionsY[i] + boxPixelsY) {

            beeActive[i] = false;
            //Honeycomb form
            isHoneycomb[i] = true;
            honeycombX[i] = beePositionsX[i];
            honeycombY[i] = beePositionsY[i];
            //Score Accumulation
            if (isFastBee[i]) {
                isRedHoneycombActive[i] = true;
                score += 1000; // Fast bee destroyed
            }
            else {
                isRedHoneycombActive[i] = false;
                score += 100; // Regular bee destroyed
            }

            bullet_exists = false;
            beesDestroyed+=1;
        }
    }
}

//  handleBulletHoneycombCollision with scoring
void BulletHoneycombCollision(float& bullet_x, float& bullet_y, bool& bullet_exists, float honeycombX[], float honeycombY[], bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score) {
    // Check pre honeycombs
    for (int i = 0; i < totalPreHoneycombs; i++) {
        if (!preHoneycombActive[i])
        {
            continue;
        }
        bool collidesX = (bullet_x + (boxPixelsX * 2) > preHoneycombX[i]) && (bullet_x < preHoneycombX[i] + boxPixelsX);
        bool collidesY = (bullet_y + (boxPixelsY * 2) > preHoneycombY[i]) && (bullet_y < preHoneycombY[i] + boxPixelsY);
        if (collidesX && collidesY) {
            bullet_exists = false;
            preHoneycombActive[i] = false;

            // Determine tier based on y-coordinate
            int tier = static_cast<int>(preHoneycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { // Yellow Honeycomb

                if (tier <= 2) {
                    score += 1000;
                }
                else if (tier <= 5) {
                    score += 800;
                }
                else {
                    score += 500;
                }
            }
            else { // Red Honeycomb
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1800;
                }
                else {
                    score += 1500;
                }
            }

            return;
        }
    }

    // Check honeycombs from bees
    for (int i = 0; i < totalBees; i++) {
        //Check whether bullet and honeycomb collide
        if (bullet_exists && isHoneycomb[i] &&
            bullet_x + (boxPixelsX * 2) > honeycombX[i] && bullet_x < honeycombX[i] + boxPixelsX &&
            bullet_y + (boxPixelsY * 2) > honeycombY[i] && bullet_y < honeycombY[i] + boxPixelsY) {

            // Determine tier based on y-coordinate
            int tier = static_cast<int>(honeycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { // Yellow Honeycomb
                if (tier <= 2) {
                    score += 1000;
                }
                else if (tier <= 5) {
                    score += 800;
                }
                else {
                    score += 500;
                }
            }
            else { // Red Honeycomb
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1800;
                }
                else {
                    score += 1500;
                }
            }

            // Destroy the honeycomb
            bullet_exists = false;
            isHoneycomb[i] = false;
            isRedHoneycombActive[i] = false;
        }
    }

    // Handle beehives
    for (int i = 0; i < beehivesCreated; i++) {
        if (beehiveActive[i]) {
            bool collidesX = (bullet_x + (boxPixelsX * 2) > beehiveX[i]) && (bullet_x < beehiveX[i] + boxPixelsX);
            bool collidesY = (bullet_y + (boxPixelsY * 2) > beehiveY[i]) && (bullet_y < beehiveY[i] + boxPixelsY);
            if (collidesX && collidesY) {
                // Determine tier based on y-coordinate
                int tier = static_cast<int>(beehiveY[i] / boxPixelsY) + 1;

                // Scoring based on tier when destroying a beehive
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1600;
                }
                else {
                    score += 1000;
                }

                // Destroy the beehive
                bullet_exists = false;
                beehiveActive[i] = false;  // Destroy the hive
                return; // Stop checking once bullet is gone
            }
        }
    }
}

void drawHoneycombs(RenderWindow& window, float honeycombX[], float honeycombY[], Sprite& honeycombSprite, Sprite& redHoneycombSprite, bool isHoneycomb[], bool isRedHoneycombActive[], int totalBees) {
    for (int i = 0; i < totalBees; i++) {
        //Check for whether the a collision with bullet occured ,so the respected honeycomb should be drawn
        if (isHoneycomb[i]) {
            if (isRedHoneycombActive[i]) {
                //redhoneycomb setup
                redHoneycombSprite.setPosition(honeycombX[i], honeycombY[i]);
                window.draw(redHoneycombSprite);
            }
            else {
                //yellohoneycomb setup
                honeycombSprite.setPosition(honeycombX[i], honeycombY[i]);
                window.draw(honeycombSprite);
            }
        }
    }
}

void setupLevel(int currentLevel, int& totalBees, int& fastBees, int& totalHoneycombs, Texture& backgroundTexture,
    Music& bgMusic, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int maxPreHoneycombs, int& totalPreHoneycombs) {
    // Stop any currently playing music
    bgMusic.stop();
    // Object declarations for each of the levels
    if (currentLevel == 1) {
        totalBees = 20;
        totalHoneycombs = 3;
        if (!backgroundTexture.loadFromFile("Textures/scifi_main_menu.jpg")) {
            cout << "Failed to load background1.png" << endl;
        }
        if (!bgMusic.openFromFile("Music/Music3.ogg")) {
            cout << "Failed to load Music1.ogg" << endl;
        }
    }
    else if (currentLevel == 2) {
        totalBees = 20;
        fastBees = 5;
        totalHoneycombs = 9;
        if (!backgroundTexture.loadFromFile("Textures/background2.jpg")) {
            cout << "Failed to load background2.png" << endl;
        }
        if (!bgMusic.openFromFile("Music/magical_journey.ogg")) {
            cout << "Failed to load Music2.ogg" << endl;
        }
    }
    else if (currentLevel == 3) {
        totalBees = 30;
        fastBees = 10;
        totalHoneycombs = 15;
        if (!backgroundTexture.loadFromFile("Textures/background3.jpeg")) {
            cout << "Failed to load background3.png" << endl;
        }
        if (!bgMusic.openFromFile("Music/risk.ogg")) {
            cout << "Failed to load Music3.ogg" << endl;
        }
    }
    else if (currentLevel == 4) { // New Level 4
        totalBees = 35; // Increase challenge
        fastBees = 15;  // More fast enemies
        totalHoneycombs = 15; // More obstacles
        if (!backgroundTexture.loadFromFile("Textures/bg4.jpg")) {
            cout << "Failed to load level4_bg.png" << endl;
        }
        if (!bgMusic.openFromFile("Music/risk.ogg")) {
            cout << "Failed to load level4.ogg" << endl;
        }
    }


    bgMusic.setLoop(true);
    bgMusic.play();

    totalPreHoneycombs = totalHoneycombs;
    for (int i = 0; i < totalPreHoneycombs; i++) {
        bool positionOK = false;
        float x, y;
        int attempts = 0;
        const int maxAttempts = 100;
        while (!positionOK && attempts < maxAttempts) {
            x = static_cast<float>(rand() % (resolutionX - boxPixelsX));
            y = static_cast<float>(rand() % ((gameColumns - 3) * boxPixelsY));

            positionOK = true;
            for (int j = 0; j < i; j++) {
                float dir_x = x - preHoneycombX[j];
                float dir_y = y - preHoneycombY[j];
                float distance = sqrtf(dir_x * dir_x + dir_y * dir_y);
                if (distance < boxPixelsX) {
                    positionOK = false;
                    break;
                }
            }
            attempts++;
        }
        preHoneycombX[i] = x;
        preHoneycombY[i] = y;
        preHoneycombActive[i] = true;
    }
}

void moveHummingbird(float& hummingbird_x, float& hummingbird_y, float& hummingbird_directionX, float& hummingbird_directionY, bool& hummingbird_paused, float& hummingbird_pauseTime, Clock& hummingbirdMoveClock, float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, int resolutionX, int resolutionY, int boxPixelsX, int boxPixelsY) {
    const float hummingbirdSpeed = 5.0f;
    const float moveInterval = 0.05f;
    const float pausetime = 1.0f;
    //Humming bird movement
    if (hummingbirdMoveClock.getElapsedTime().asSeconds() >= moveInterval) {
        if (hummingbird_paused) {
            hummingbird_pauseTime -= moveInterval;
            if (hummingbird_pauseTime <= 0.0f) {
                hummingbird_paused = false;
                bool honeycomb_exist = false;
                float targetX = 0.0f;
                float targetY = 0.0f;
                float minDistance = 100000.0f;

                for (int i = 0; i < totalBees; i++) {
                    if (isHoneycomb[i]) {
                        float dx = honeycombX[i] - hummingbird_x;
                        float dy = honeycombY[i] - hummingbird_y;
                        float distance = sqrt(dx * dx + dy * dy);
                        //sET TARGET OF NEAREST HONEYCOMB AVAILABLE
                        if (distance < minDistance) {
                            minDistance = distance;
                            targetX = honeycombX[i];
                            targetY = honeycombY[i];
                            honeycomb_exist = true;
                        }
                    }
                }
                //MOVE TOWARDS HONEYCOMB
                if (honeycomb_exist) {
                    float dir_x = targetX - hummingbird_x;
                    float dir_y = targetY - hummingbird_y;
                    float magnitude = sqrt(dir_x * dir_x + dir_y * dir_y);
                    if (magnitude != 0) {
                        hummingbird_directionX = dir_x / magnitude;
                        hummingbird_directionY = dir_y / magnitude;
                    }
                }
                else {
                    // Random direction: -1, 0, or 1(horizontal and vertical)
                    hummingbird_directionX = static_cast<float>(rand() % 3 - 1);
                    hummingbird_directionY = static_cast<float>(rand() % 3 - 1);
                }
            }
        }
        else {
            hummingbird_x += hummingbird_directionX * hummingbirdSpeed;
            hummingbird_y += hummingbird_directionY * hummingbirdSpeed;

            // Boundary checks to stay within the screen
            if (hummingbird_x < 0) {
                hummingbird_x = 0;
                hummingbird_directionX *= -1.0f;
                hummingbird_y += boxPixelsY;
            }
            if (hummingbird_x > resolutionX - boxPixelsX) {
                hummingbird_x = resolutionX - boxPixelsX;
                hummingbird_directionX *= -1.0f;
                hummingbird_y += boxPixelsY;
            }
            if (hummingbird_y < 0) {
                hummingbird_y = 0;
                hummingbird_directionY *= -1.0f;
            }
            if (hummingbird_y > resolutionY - boxPixelsY) {
                hummingbird_y = resolutionY - boxPixelsY;
                hummingbird_directionY *= -1.0f;
            }

            // Random pause for hummingbird
            if ((rand() % 990) < 10) {
                hummingbird_paused = true;
                hummingbird_pauseTime = pausetime;
            }
        }
        hummingbirdMoveClock.restart();
    }
}

void drawHummingbird(RenderWindow& window, Sprite& hummingbirdSprite, float hummingbird_x, float hummingbird_y, bool hummingbird_visible) {
    //hummingbird setup
    if (hummingbird_visible) {
        hummingbirdSprite.setPosition(hummingbird_x, hummingbird_y);
        window.draw(hummingbirdSprite);
    }
}

// HummingbirdHoneycombCollision with scoring
void HummingbirdHoneycombCollision(float honeycombX[], float honeycombY[], bool isHoneycomb[], int totalBees, float hummingbird_x, float hummingbird_y, int& score, int boxPixelsX, int boxPixelsY, float preHoneycombX[], float preHoneycombY[], bool preHoneycombActive[], int totalPreHoneycombs, bool isRedHoneycombActive[]) {
    // Handle pre-generated honeycombs
    for (int i = 0; i < totalPreHoneycombs; i++) {
        if (!preHoneycombActive[i]) 
        { continue; }
        bool collide_X = (hummingbird_x + boxPixelsX > preHoneycombX[i]) && (hummingbird_x < preHoneycombX[i] + boxPixelsX);
        bool collide_Y = (hummingbird_y + boxPixelsY > preHoneycombY[i]) && (hummingbird_y < preHoneycombY[i] + boxPixelsY);
        //Condtion to check collission
        if (collide_X && collide_Y) {
            // Determine tier based on y-coordinate
            int tier = static_cast<int>(preHoneycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { // Yellow Honeycomb
                //Score Accumulation
                if (tier <= 2) {
                    score += 1000;
                }
                else if (tier <= 5) {
                    score += 800;
                }
                else {
                    score += 500;
                }
            }
            else { // Red Honeycomb
                if (tier <= 2) {
                    score += 2000;
                }
                else if (tier <= 5) {
                    score += 1800;
                }
                else {
                    score += 1500;
                }
            }

            // Consume the honeycomb
            preHoneycombActive[i] = false;
        }
    }

    // Handle honeycombs from bees
    for (int i = 0; i < totalBees; i++) {
        if (isHoneycomb[i] &&
            hummingbird_x + boxPixelsX > honeycombX[i] && hummingbird_x < honeycombX[i] + boxPixelsX &&
            hummingbird_y + boxPixelsY > honeycombY[i] && hummingbird_y < honeycombY[i] + boxPixelsY) {

            // Determine tier based on y-coordinate
            int screentier = static_cast<int>(honeycombY[i] / boxPixelsY) + 1;

            if (!isRedHoneycombActive[i]) { // Yellow Honeycomb
                //Score Accumulation
                if (screentier <= 2) {
                    score += 1000;
                }
                else if (screentier <= 5) {
                    score += 800;
                }
                else {
                    score += 500;
                }
            }
            else { // Red Honeycomb
                if (screentier <= 2) {
                    score += 2000;
                }
                else if (screentier <= 5) {
                    score += 1800;
                }
                else {
                    score += 1500;
                }
            }

            // Consume the honeycomb
            isHoneycomb[i] = false;
            isRedHoneycombActive[i] = false;
        }
    }
}
void BulletHummingbirdCollision(float bullet_x, float bullet_y, bool& bullet_exists, float hummingbird_x, float hummingbird_y, int& hummingbird_health, bool& hummingbird_visible, Clock& hummingbirdRespawnClock, Sprite& hummingbirdSprite, int boxPixelsX, int boxPixelsY) {
    if (bullet_exists && hummingbird_visible &&
        bullet_x + (boxPixelsX * 2) > hummingbird_x && bullet_x < hummingbird_x + boxPixelsX &&
        bullet_y + (boxPixelsY * 2) > hummingbird_y && bullet_y < hummingbird_y + boxPixelsY) {
        hummingbird_health-=1;
        bullet_exists = false;
        //Disappear if health finishes
        if (hummingbird_health <= 0) {
            hummingbird_visible = false;
            hummingbirdRespawnClock.restart();
        }

        else if (hummingbird_health == 1) {
            //when humming bird hit twice , color change
            hummingbirdSprite.setColor(Color::Blue);
        }
    }
}
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, bool flowers[], int gameRows, int boxPixelsX, int boxPixelsY, int resolutionY) {
    for (int i = 0; i < gameRows; i++) {
        //Flowers setting 
        if (flowers[i]) {
            flowerSprite.setPosition((float)(i * boxPixelsX), (float)(resolutionY - boxPixelsY * 3.8f));
            window.draw(flowerSprite);
        }
    }
}

void checkPlayerRestriction(float& player_x, bool flowers[], int gameRows, int boxPixelsX, int& sprayCansLeft, int totalSprayCans, int sprayLevels[], int& currentSprayIndex, int maxSpraysPerCan, GameState& currentState) {
    int playerColumn = static_cast<int>(player_x / boxPixelsX);
    bool restricted = false;

    bool leftBlocked = (playerColumn <= 0) || (playerColumn > 0 && flowers[playerColumn - 1]);
    bool rightBlocked = (playerColumn >= gameRows - 1) || (playerColumn < gameRows - 1 && flowers[playerColumn + 1]);
    if (leftBlocked && rightBlocked) {
        restricted = true;
    }

    if (restricted) {
        bool SpaceFound = false;
        for (int i = 0; i < gameRows; i++) {
            if (!flowers[i]) {
                player_x = (float)(i * boxPixelsX);
                SpaceFound = true;
                break;
            }
        }
        if (!SpaceFound) {
            if (sprayCansLeft > 0) {
                for (int i = 0; i < gameRows; i++) {
                    flowers[i] = false;
                }
                sprayCansLeft-=1;
                for (int i = 0; i < totalSprayCans; i++) {
                    sprayLevels[i] = maxSpraysPerCan;
                }
                currentSprayIndex = 0;
                player_x = (float)((gameRows / 2) * boxPixelsX);
            }
            else {
                currentState = GAME_OVER;
            }
        }
    }
}

void initBeeHives(bool beehiveActive[], float beehiveX[], float beehiveY[], int maxBeehives, int& beehivesCreated) {
    for (int i = 0; i < maxBeehives; i++) {
        //initializing bee hives
        beehiveActive[i] = false;
        beehiveX[i] = 0.0f;
        beehiveY[i] = 0.0f;
    }
    beehivesCreated = 0;
}

void drawBeeHives(RenderWindow& window, Sprite& beehiveSprite,
    const bool beehiveActive[], const float beehiveX[], const float beehiveY[], int beehivesCreated) {
    for (int i = 0; i < beehivesCreated; i++) {
        if (beehiveActive[i]) {
            beehiveSprite.setPosition(beehiveX[i], beehiveY[i]);
            window.draw(beehiveSprite);
        }
    }
}

// New function to handle beehive completion and scoring
void BeehiveCompletion(bool beehiveActive[], float beehiveX[], float beehiveY[], int beehivesCreated, int& score) {

    for (int i = 0; i < beehivesCreated; i++) {
        if (beehiveActive[i]) {
            // Determine tier based on y-coordinate
            int tier = static_cast<int>(beehiveY[i] / boxPixelsY) + 1;

            // Scoring based on tier when completing a level with a beehive
            if (tier <= 2) {
                score += 2000;
            }
            else if (tier <= 5) {
                score += 1600;
            }
            else {
                score += 1000;
            }

            // Optionally, deactivate the beehive after scoring
            beehiveActive[i] = false;
        }
    }
}
// Load high scores from file
void loadHighScores() {
    ifstream file("highscores.txt");
    if (!file.is_open())
    {
        return;
    }

    highScores.clear();
     if (file.is_open())
    {
        string name;
        int score;
        while (file >> name >> score) {
            highScores.push_back({ name, score });
        }
        file.close();
    }
    
}

// Save high scores to file
void saveHighScores() {
    ofstream file("highscores.txt");
    if (file.is_open()) {
        for (int i = 0; i < min(10, (int)highScores.size()); ++i) {
            file << highScores[i].name << " " << highScores[i].score << endl;
        }
        file.close();
    }
}

// Display high scores
void displayHighScores(RenderWindow& window, Font& font) {
    cout << "highscore function called";
    Text title("High Scores", font, 60);
    title.setPosition(200, 100);
    title.setFillColor(Color::White);
    window.draw(title);

    for (size_t i = 0; i < highScores.size() && i < highScoreCount; i++) {
        //Entry According to the number , name and score
        Text entry(to_string(i + 1) + ". " + highScores[i].name + " - " + to_string(highScores[i].score), font, 30);
        entry.setPosition(300, 150 + i * 40);
        entry.setFillColor(Color::White);
        window.draw(entry);
    }
    Text back("Press ESC to return to menu", font, 20);
    back.setPosition(300, 600);
    back.setFillColor(Color::White);
    window.draw(back);
    window.display();
}

// Update high scores
void updateHighScores(string playername, int newscore) {
    //New entry of highscore
    highScores.push_back({ playername, newscore });
    //Sorting
    sort(highScores.begin(), highScores.end(), [](const HighScore& a, const HighScore& b) {
        return a.score > b.score;
        });
    //If number exeed of 10 then last one removed 
    if (highScores.size() > highScoreCount) {
        highScores.pop_back();
    }
    saveHighScores();
}



