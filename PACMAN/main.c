#include <allegro.h>
#include "labyrinth.h"
#include "game.h"
#include "graphics.h"
#include "sounds.h"

// Fonction pour afficher et gérer le menu de replay
int replay_menu() {
    clear_to_color(screen, makecol(0, 0, 0)); // Efface l'écran
    textout_centre_ex(screen, font, "REPLAY?", SCREEN_W / 2, SCREEN_H / 3, makecol(255, 255, 255), -1);
    textout_centre_ex(screen, font, "YES - Press 'Y'", SCREEN_W / 2, SCREEN_H / 2, makecol(0, 255, 0), -1);
    textout_centre_ex(screen, font, "NO  - Press 'N'", SCREEN_W / 2, SCREEN_H / 2 + 20, makecol(255, 0, 0), -1);

    while (1) {
        if (key[KEY_Y]) return 1; // YES sélectionné
        if (key[KEY_N]) return 0; // NO sélectionné
        rest(50); // Petit délai pour éviter une boucle infinie
    }
}

// Fonction pour démarrer une partie
void start_game() {
    init_sounds();
    play_sound("beggining.wav");

    int current_level = 1;
    reset_labyrinths();

    while (current_level <= 3) {
        int (*labyrinth)[COLS] = load_labyrinth(current_level);

        if (!play_game(labyrinth, current_level)) {
            play_sound("wasted.wav");
            textout_centre_ex(screen, font, "GAME OVER!", SCREEN_W / 2, SCREEN_H / 2, makecol(255, 0, 0), -1);
            rest(2000);
            return;
        }
        current_level++;
    }

    if (current_level > 3) {
        play_sound("you win.wav");
        textout_centre_ex(screen, font, "YOU WIN!", SCREEN_W / 2, SCREEN_H / 2, makecol(0, 255, 0), -1);
        rest(2000);
    }

    cleanup_sounds();
}

int main() {
    int tile_size = 40;
    int window_width = COLS * tile_size;
    int window_height = ROWS * tile_size;

    allegro_init();
    install_keyboard();
    set_color_depth(32);

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, window_width, window_height, 0, 0) != 0) {
        allegro_message("Erreur : Impossible de définir le mode graphique.");
        return 1;
    }

    initialize_labyrinths();
    int replay = 1;

    while (replay) {
        start_game();
        replay = replay_menu();
    }

    return 0;
}
END_OF_MAIN();
