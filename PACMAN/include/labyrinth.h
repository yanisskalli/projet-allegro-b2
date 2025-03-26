#ifndef LABYRINTH_H
#define LABYRINTH_H

#define ROWS 10
#define COLS 20

extern int labyrinth_level1[ROWS][COLS];
extern int labyrinth_level2[ROWS][COLS];
extern int labyrinth_level3[ROWS][COLS];

// Prototypes
void initialize_labyrinths();  // Sauvegarde des labyrinthes originaux
void reset_labyrinths();       // Réinitialise les labyrinthes à l'état initial
int (*load_labyrinth(int level))[COLS];

#endif
