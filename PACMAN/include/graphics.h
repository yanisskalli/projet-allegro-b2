#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <allegro.h>
#include "labyrinth.h"

// Charge un sprite depuis un chemin
BITMAP* load_sprite(const char* file_path);

// Dessine Pacman à une position donnée
void draw_pacman(BITMAP* buffer, BITMAP* sprite, int x, int y);

// Dessine le labyrinthe
void draw_labyrinth(BITMAP* buffer, int (*labyrinth)[COLS]);

#endif
