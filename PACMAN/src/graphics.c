#include "graphics.h"

#define PACMAN_PATH "assets/characters/pacman/" // Chemin d'acc�s aux sprites de Pacman
#define GHOST_PATH "assets/characters/ghosts/" // Chemin d'acc�s aux sprites des fant�mes

// Fonction pour charger un sprite � partir d'un fichier
BITMAP* load_sprite(const char* file_path) {
    BITMAP* sprite = load_bitmap(file_path, NULL); // Charge l'image du fichier sp�cifi� // BITMAP = POINTEUR VERS UN OBJET DE TYPE BITMAP, LOAD_BITMAP = FONCTION ALLEGRO
    if (!sprite) { // V�rifie si le chargement a �chou�
        allegro_message("Impossible de charger le sprite : %s", file_path); // Affiche un message d'erreur avec le chemin
    }
    return sprite; // Retourne le sprite charg� ou NULL en cas d'erreur
}

// Fonction pour dessiner Pacman sur le buffer graphique
void draw_pacman(BITMAP* buffer, BITMAP* sprite, int x, int y) {
    // buffer : c'est une zone temporaire o� on dessine tout avant de l'afficher � l'�cran
    // x, y : ce sont les coordonn�es (position) o� Pacman doit �tre dessin�
    draw_sprite(buffer, sprite, x, y); // Dessine l'image de Pacman � la position (x, y) sur le buffer
}


// Fonction pour dessiner le labyrinthe sur le buffer graphique
void draw_labyrinth(BITMAP* buffer, int (*labyrinth)[COLS]) {
    // Parcourt chaque case du tableau repr�sentant le labyrinthe
    for (int i = 0; i < ROWS; i++) { // Parcourt les lignes
        for (int j = 0; j < COLS; j++) { // Parcourt les colonnes
            if (labyrinth[i][j] == 1) {
                // Si la case vaut 1, c'est un mur : on dessine un rectangle bleu
                rectfill(buffer, j * 40, i * 40, (j + 1) * 40 - 1, (i + 1) * 40 - 1, makecol(0, 0, 255));
            } else if (labyrinth[i][j] == 2) {
                // Si la case vaut 2, c'est un point jaune : on dessine un petit carr� jaune centr�
                rectfill(buffer, j * 40 + 15, i * 40 + 15, (j + 1) * 40 - 15, (i + 1) * 40 - 15, makecol(255, 255, 0));
            }
        }
    }
}
