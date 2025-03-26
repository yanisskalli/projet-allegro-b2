#include "sounds.h"
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

#define SOUND_PATH "assets/sounds/"

void init_sounds() {
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
        allegro_message("Erreur : Impossible d'initialiser le son !");
        exit(EXIT_FAILURE);
    }
}

void play_sound(const char* file_name) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s", SOUND_PATH, file_name);

    SAMPLE* sound = load_sample(file_path);
    if (!sound) {
        fprintf(stderr, "Erreur : Impossible de charger le son : %s\n", file_path);
        return;
    }

    play_sample(sound, 255, 128, 1000, 0);
    destroy_sample(sound);
}

void cleanup_sounds() {
    remove_sound();
}
