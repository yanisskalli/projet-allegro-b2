#ifndef GAME_H
#define GAME_H

#include "labyrinth.h"
#include <allegro.h>

// Structures pour Pacman et les fantômes
typedef struct {
    int x, y;         // Position en cases
    int direction;    // Direction (0 = haut, 1 = bas, 2 = gauche, 3 = droite)
} Pacman;

typedef struct {
    int x, y;         // Position en cases
    int direction;    // Direction (0 = haut, 1 = bas, 2 = gauche, 3 = droite)
    int id;           // Identifiant unique du fantôme
    BITMAP* sprite;   // Sprite du fantôme
} Ghost;

// Fonctions principales
int are_all_points_collected(int collected_points, int total_points);
int calculate_total_points(int (*labyrinth)[COLS]);
void move_ghost(Ghost* ghost, int (*labyrinth)[COLS]);
int check_collision(Pacman pacman, Ghost ghost, BITMAP* pacman_sprite);
void spawn_ghost(Ghost* ghost, int (*labyrinth)[COLS], Pacman pacman);
int check_pixel_collision(BITMAP* sprite1, int x1, int y1, BITMAP* sprite2, int x2, int y2);
int play_game(int (*labyrinth)[COLS], int level);

#endif
