#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

#define DEBUG false

// Window size
#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 1000

// Timer
#define GAME_TIMER_INTERVAL 10

// Card
#define CARD_WIDTH 120
#define CARD_HEIGHT 180
#define CARD_X_INTERVAL 40
#define CARD_Y_INTERVAL 250
#define CARD_SELECTED_Y_MOVEMENT 30
#define CARD_TOP (680)

enum Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades
};

const QString SUITS[4] = {"clubs", "diamonds", "hearts", "spades"};

#define GAME_REPORTER_X (50)
#define GAME_REPORTER_Y (50)
#define GAME_REPORTER_WIDTH (WINDOW_WIDTH / 4 + 50)
#define GAME_REPORTER_HEIGHT (WINDOW_HEIGHT - 100  - 100)

#define RIGHT_VIEW_X (GAME_REPORTER_X + GAME_REPORTER_WIDTH + 50)

#define COUNTDOWN_TIMES 10
#define COUNTDOWN_INTERVAL 1000
#define COUNTDOWN_DISPLAY_X (WINDOW_WIDTH - 150)
#define COUNTDOWN_DISPLAY_Y 600


#endif // CONSTANTS_H
