#include "game.h"
#include "graphics.h"
#include "sounds.h"
#include <allegro.h>
#include <stdlib.h>
#include <time.h>

#define TILE_SIZE 40       // Taille d'une case du labyrinthe
#define SPRITE_SCALE 0.7   // Réduction de la taille des sprites (70% de TILE_SIZE)
#define EXPLOSION_SCALE 1 // Taille ajustée de l'explosion (110% de TILE_SIZE)
#define GHOST_SPEED 3      // Réduction de fréquence des déplacements des fantômes

// Taille des sprites réduits
#define SPRITE_SIZE (int)(TILE_SIZE * SPRITE_SCALE)
#define EXPLOSION_SIZE (int)(TILE_SIZE * EXPLOSION_SCALE)

// Décalages pour centrer les sprites
#define SPRITE_OFFSET ((TILE_SIZE - SPRITE_SIZE) / 2)
#define EXPLOSION_OFFSET ((TILE_SIZE - EXPLOSION_SIZE) / 2)

// Calcule le nombre total de points jaunes dans le labyrinthe
int calculate_total_points(int (*labyrinth)[COLS]) {
    int total_points = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (labyrinth[i][j] == 2) {
                total_points++;
            }
        }
    }
    return total_points;
}

// Vérifie si tous les points jaunes sont collectés
int are_all_points_collected(int collected_points, int total_points) {
    return collected_points >= total_points;
}

// Déplace un fantôme aléatoirement en explorant le labyrinthe
void move_ghost(Ghost* ghost, int (*labyrinth)[COLS]) {
    static int ghost_move_counters[6] = {0}; // Un compteur unique pour chaque fantôme
    int ghost_index = ghost->id;
    ghost_move_counters[ghost_index]++;

    if (ghost_move_counters[ghost_index] % GHOST_SPEED != 0) {
        return; // Pas de mouvement cette fois
    }

    int new_x = ghost->x;
    int new_y = ghost->y;
    int random_dir = rand() % 4;

    for (int attempts = 0; attempts < 4; attempts++) {
        switch (random_dir) {
            case 0: new_y = ghost->y - 1; break; // Haut
            case 1: new_y = ghost->y + 1; break; // Bas
            case 2: new_x = ghost->x - 1; break; // Gauche
            case 3: new_x = ghost->x + 1; break; // Droite
        }

        if (new_x >= 0 && new_x < COLS && new_y >= 0 && new_y < ROWS && labyrinth[new_y][new_x] != 1) {
            ghost->x = new_x;
            ghost->y = new_y;
            ghost->direction = random_dir;
            return;
        }

        random_dir = (random_dir + 1) % 4;
    }
}

int check_pixel_collision(BITMAP* sprite1, int x1, int y1, BITMAP* sprite2, int x2, int y2) {
    int width = MIN(sprite1->w, sprite2->w);
    int height = MIN(sprite1->h, sprite2->h);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (x1 + i >= x2 && x1 + i < x2 + sprite2->w &&
                y1 + j >= y2 && y1 + j < y2 + sprite2->h &&
                getpixel(sprite1, i, j) != makecol(255, 0, 255) &&
                getpixel(sprite2, i + (x2 - x1), j + (y2 - y1)) != makecol(255, 0, 255)) {
                return 1;
            }
        }
    }
    return 0;
}

int check_collision(Pacman pacman, Ghost ghost, BITMAP* pacman_sprite) {
    return check_pixel_collision(
        pacman_sprite, pacman.x * TILE_SIZE + SPRITE_OFFSET, pacman.y * TILE_SIZE + SPRITE_OFFSET,
        ghost.sprite, ghost.x * TILE_SIZE + SPRITE_OFFSET, ghost.y * TILE_SIZE + SPRITE_OFFSET
    );
}

void spawn_ghost(Ghost* ghost, int (*labyrinth)[COLS], Pacman pacman) {
    do {
        ghost->x = rand() % COLS;
        ghost->y = rand() % ROWS;
    } while (labyrinth[ghost->y][ghost->x] == 1 || (ghost->x == pacman.x && ghost->y == pacman.y));
}

// Redimensionne un sprite pour qu'il corresponde à la nouvelle taille des sprites
BITMAP* resize_sprite_to_tile(BITMAP* sprite, int size) {
    BITMAP* resized = create_bitmap(size, size);
    stretch_sprite(resized, sprite, 0, 0, size, size);
    return resized;
}

int play_game(int (*labyrinth)[COLS], int level) {
    srand(time(NULL));

    // Charger et redimensionner les sprites de Pacman
    BITMAP* pac_up = resize_sprite_to_tile(load_sprite("assets/characters/pacman/pac_up.bmp"), SPRITE_SIZE);
    BITMAP* pac_down = resize_sprite_to_tile(load_sprite("assets/characters/pacman/pac_down.bmp"), SPRITE_SIZE);
    BITMAP* pac_left = resize_sprite_to_tile(load_sprite("assets/characters/pacman/pac_left.bmp"), SPRITE_SIZE);
    BITMAP* pac_right = resize_sprite_to_tile(load_sprite("assets/characters/pacman/pac_right.bmp"), SPRITE_SIZE);
    BITMAP* pac_default = resize_sprite_to_tile(load_sprite("assets/characters/pacman/pacman.bmp"), SPRITE_SIZE);

    // Charger et redimensionner les sprites des fantômes
    BITMAP* ghost_sprites[] = {
        resize_sprite_to_tile(load_sprite("assets/characters/ghosts/blinky.bmp"), SPRITE_SIZE),
        resize_sprite_to_tile(load_sprite("assets/characters/ghosts/azul.bmp"), SPRITE_SIZE),
        resize_sprite_to_tile(load_sprite("assets/characters/ghosts/amarelo.bmp"), SPRITE_SIZE),
        resize_sprite_to_tile(load_sprite("assets/characters/ghosts/rosa.bmp"), SPRITE_SIZE),
        resize_sprite_to_tile(load_sprite("assets/characters/ghosts/verde.bmp"), SPRITE_SIZE),
        resize_sprite_to_tile(load_sprite("assets/characters/ghosts/gburro1.bmp"), SPRITE_SIZE)
    };

    // Charger et redimensionner l'explosion
    BITMAP* explosion = resize_sprite_to_tile(load_sprite("C:\\Users\\yanis\\Desktop\\PROJET ALLEGRO\\PACMAN\\assets\\others\\exp.bmp"), EXPLOSION_SIZE);

    Pacman pacman = {1, 1, 3};

    int ghost_count = (level == 1) ? 1 : (level == 2) ? 3 : 6;
    Ghost ghosts[ghost_count];
    for (int i = 0; i < ghost_count; i++) {
        ghosts[i].sprite = ghost_sprites[i];
        ghosts[i].direction = rand() % 4;
        ghosts[i].id = i;
        spawn_ghost(&ghosts[i], labyrinth, pacman);
    }

    // Initialisation des points
    int total_points = calculate_total_points(labyrinth);
    int collected_points = 0;

    while (!key[KEY_ESC]) {
        if (key[KEY_UP]) pacman.direction = 0;
        if (key[KEY_DOWN]) pacman.direction = 1;
        if (key[KEY_LEFT]) pacman.direction = 2;
        if (key[KEY_RIGHT]) pacman.direction = 3;

        int new_x = pacman.x, new_y = pacman.y;
        if (pacman.direction == 0) new_y--;
        if (pacman.direction == 1) new_y++;
        if (pacman.direction == 2) new_x--;
        if (pacman.direction == 3) new_x++;
        if (labyrinth[new_y][new_x] != 1) {
            pacman.x = new_x;
            pacman.y = new_y;
            if (labyrinth[pacman.y][pacman.x] == 2) {
                labyrinth[pacman.y][pacman.x] = 0;
                collected_points++;
            }
        }

        for (int i = 0; i < ghost_count; i++) {
            move_ghost(&ghosts[i], labyrinth);
        }

        for (int i = 0; i < ghost_count; i++) {
            if (check_collision(pacman, ghosts[i], pac_default)) {
                clear_to_color(screen, makecol(0, 0, 0)); // Efface l'écran
                draw_labyrinth(screen, labyrinth);
                draw_sprite(screen, explosion, pacman.x * TILE_SIZE + EXPLOSION_OFFSET, pacman.y * TILE_SIZE + EXPLOSION_OFFSET);
                rest(1000); // Affiche l'explosion pendant 1 seconde
                return 0; // Fin du jeu
            }
        }

        clear_to_color(screen, makecol(0, 0, 0));
        draw_labyrinth(screen, labyrinth);

        // Dessine Pacman
        draw_pacman(screen, (pacman.direction == 0) ? pac_up : (pacman.direction == 1) ? pac_down :
                            (pacman.direction == 2) ? pac_left : (pacman.direction == 3) ? pac_right : pac_default,
                    pacman.x * TILE_SIZE + SPRITE_OFFSET, pacman.y * TILE_SIZE + SPRITE_OFFSET);

        // Dessine les fantômes
        for (int i = 0; i < ghost_count; i++) {
            draw_sprite(screen, ghosts[i].sprite, ghosts[i].x * TILE_SIZE + SPRITE_OFFSET, ghosts[i].y * TILE_SIZE + SPRITE_OFFSET);
        }

        // Affiche le score en haut à droite
        textprintf_ex(screen, font, SCREEN_W - 100, 10, makecol(255, 255, 255), -1, "%d/%d", collected_points, total_points);

        if (are_all_points_collected(collected_points, total_points)) {
            return 1; // Niveau terminé
        }

        rest(120);
    }

    return 0;
}
