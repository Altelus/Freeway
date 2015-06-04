#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

// Game Settings
#define PLAYER_SPEED 10.f

// Windowing
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

#define WINDOWED sf::Style::Default
#define FULLSCREEN sf::Style::Fullscreen

#define DEFAULT_WINDOW_MODE WINDOWED

#define SFML_DEPTH_BITS 32
#define SFML_ANTI_ALIAS 0
#define SFML_VERTICAL_SYNC false
#define SFML_CURSOR_VISIBLE false

#define VIEW_BORDER_SIZE 8

// OpenGl
#define FOV_FAR 10000.f
#define COLOR_BLACK 0.f, 0.f, 0.f, 1.f
#define COLOR_GREY .5f, .5f, .5f, 1.f
#define COLOR_WHITE 1.f, 1.f, 1.f, 1.f
#define CLEAR_COLOR COLOR_BLACK

// Lighting
#define LIGHT_AMBIENT_COL {0.2f, 0.2f, 0.2f, 1.f}
#define LIGHT_DEFUSE_COL {1.f, 1.f, 1.f, 1.f}
#define LIGHT_SPECULAR_COL {0.992f, 0.722f, 0.075f, 1.f};

// Text
#define STUDIO "Gamesmiths"
#define GAME_TITLE "Freeway"

// Filenames


// Input


#endif