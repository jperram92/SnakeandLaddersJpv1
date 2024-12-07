#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

using namespace std;

enum GameState { Playing, GameOver, Exit };

// Global variables
sf::RenderWindow window(sf::VideoMode(600, 660), "Snakes and Ladders"); // Increased height for action text section
GameState currentState = Playing;

// Game board and player positions
int board[101] = {0};
int playerPosition = 1, computerPosition = 1;
sf::Font font;  // Font for displaying text on screen
sf::Text currentAction;  // To display only the current action
sf::Texture diceTexture;
sf::Sprite diceSprite;

// Debounce mechanism
sf::Clock gameClock;  // For tracking elapsed time

// Function to set up the board with snakes and ladders
void setupBoard() {
    board[16] = -10; board[47] = -21; board[49] = -38; board[56] = -3;
    board[62] = -43; board[64] = -4; board[87] = -63; board[93] = -20;
    board[95] = -20; board[98] = -19; board[1] = 38; board[4] = 14;
    board[9] = 31; board[21] = 42; board[28] = 84; board[36] = 44;
    board[51] = 67; board[71] = 91; board[80] = 100;
    cout << "Board Setup Complete!" << endl;
}

int rollDice() {
    int roll = rand() % 6 + 1;
    cout << "Dice Rolled: " << roll << endl;  // Debugging output
    return roll;
}

// Function to update the displayed action text
void updateActionText(const string &action) {
    currentAction.setString(action);
    cout << "Updated Action Text: " << action << endl;  // Debugging output
}

// Move function to update positions and handle snakes/ladders
void move(int& position, const string& who) {
    int diceRoll = rollDice();
    stringstream action;
    action << who << " rolled a " << diceRoll;
    updateActionText(action.str());

    position += diceRoll;

    // Check for snakes or ladders
    if (position <= 100 && board[position] != 0) {
        if (board[position] > 0) {
            action.str("");
            action << who << " climbed a ladder!";
        } else {
            action.str("");
            action << who << " got bitten by a snake!";
        }
        updateActionText(action.str());
    }

    // Apply snake or ladder
    if (position <= 100) position += board[position];

    if (position > 100) position = 100;
    cout << who << " is now on position: " << position << endl;  // Debugging output
}

// Draw the game board and current action
void drawBoard(int playerPos, int computerPos) {
    window.clear();

    // Create a white rectangle to cover the top area (above the board)
    sf::RectangleShape actionTextBox(sf::Vector2f(600, 60));  // 60px height
    actionTextBox.setFillColor(sf::Color::White);
    actionTextBox.setPosition(0, 0); // Top of the window
    window.draw(actionTextBox);

    // Draw the action text inside the white box
    currentAction.setFont(font);
    currentAction.setCharacterSize(18);
    currentAction.setFillColor(sf::Color::Black);
    currentAction.setPosition(10, 10);  // Position in the text box
    window.draw(currentAction);

    // Draw the dice image in the top right corner of the white box
    diceSprite.setPosition(530, 10);  // Adjust position for the image
    window.draw(diceSprite);

    // Draw grid in 600x600 area with updated grid size (now 660 to account for text section)
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            sf::RectangleShape tile(sf::Vector2f(60, 60));  // Square size of 60x60
            tile.setPosition(j * 60, i * 60 + 60); // Adjusted for the action text section
            tile.setFillColor(sf::Color(200, 200, 200));
            window.draw(tile);

            // Draw the number for the board position, placing it in the middle-right of each square
            sf::Text boardText;
            boardText.setFont(font);
            boardText.setCharacterSize(12);
            boardText.setFillColor(sf::Color::Black);
            boardText.setString(to_string(i * 10 + j + 1));  // Calculate the number
            boardText.setPosition(j * 60 + 35, i * 60 + 60 + 15); // Adjusting position to the right-middle
            window.draw(boardText);
        }
    }

    // Draw grid lines only below the action box
    for (int i = 0; i <= 10; i++) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(i * 60, 60)),
            sf::Vertex(sf::Vector2f(i * 60, 660))  // Adjusted height
        };
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(0, i * 60 + 60));  // Adjusted for the action text section
        line[1] = sf::Vertex(sf::Vector2f(600, i * 60 + 60));  // Adjusted height
        window.draw(line, 2, sf::Lines);
    }

    // Draw player and computer with smaller size
    sf::CircleShape player(8);  // Smaller radius (8 instead of 10)
    player.setFillColor(sf::Color::Green);
    // Adjust position to place circle to the left of the number and center it vertically
    player.setPosition((playerPos % 10) * 60 + 5, (playerPos / 10) * 60 + 15 + 60);  // Slightly to the left and centered vertically
    window.draw(player);

    sf::CircleShape computer(8);  // Smaller radius (8 instead of 10)
    computer.setFillColor(sf::Color::Red);
    // Adjust position to place circle to the left of the number and center it vertically
    computer.setPosition((computerPos % 10) * 60 + 5, (computerPos / 10) * 60 + 15 + 60);  // Slightly to the left and centered vertically
    window.draw(computer);

    window.display();
}

// Function to handle the Game Over screen
void gameOver(bool playerWon) {
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    // Create the win message
    sf::Text winMessage;
    winMessage.setFont(font);
    winMessage.setCharacterSize(24);
    winMessage.setFillColor(sf::Color::Black);
    winMessage.setString(playerWon ? "Player Wins!" : "Computer Wins!");

    // Create Play Again and Exit message
    sf::Text playAgainText, exitText;
    playAgainText.setFont(font);
    exitText.setFont(font);
    playAgainText.setString("Press P to Play Again");
    exitText.setString("Press Q to Quit");

    playAgainText.setCharacterSize(20);
    exitText.setCharacterSize(20);

    playAgainText.setFillColor(sf::Color::Black);
    exitText.setFillColor(sf::Color::Black);

    playAgainText.setPosition(200, 300);
    exitText.setPosition(200, 350);

    bool isGameOver = true;
    while (isGameOver) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    playerPosition = 1;
                    computerPosition = 1;
                    setupBoard();
                    currentState = Playing;  // Set game state back to playing
                    isGameOver = false;
                }

                if (event.key.code == sf::Keyboard::Q) {
                    window.close();
                    isGameOver = false;
                }
            }
        }

        window.clear();  // Clear the window before rendering the game-over screen
        window.draw(winMessage);
        window.draw(playAgainText);
        window.draw(exitText);
        window.display();
    }
}

// Main game function
void playGame() {
    setupBoard();

    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    if (!diceTexture.loadFromFile("C:\\Users\\james\\OneDrive\\Desktop\\Web Development Projects\\New Projects\\Snake Game C++\\dice.gif")) {
    cout << "Error loading dice image!" << endl;
    return;
    }
    else {
        cout << "Dice image loaded successfully!" << endl;
    }
    diceSprite.setTexture(diceTexture);  // Set dice sprite texture
    diceSprite.setScale(0.25f, 0.25f);  // This will scale down the image to 50% of its original size.


    bool playerTurn = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameClock.getElapsedTime().asSeconds() > 0.5f) {
            if (playerTurn) {
                move(playerPosition, "Player");
                playerTurn = false;
            } else {
                move(computerPosition, "Computer");
                playerTurn = true;
            }

            gameClock.restart();
            sf::sleep(sf::seconds(0.5));
        }

        if (playerPosition == 100 || computerPosition == 100 || playerPosition > 100 || computerPosition > 100) {
            bool playerWon = (playerPosition >= 100);
            currentState = GameOver;
            gameOver(playerWon);
        }

        if (currentState == Playing) {
            drawBoard(playerPosition, computerPosition);
        }
    }
}

int main() {
    srand(time(0));  // Seed random number generator
    playGame();
    return 0;
}
