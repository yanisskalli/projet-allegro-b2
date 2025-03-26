#ifndef SOUNDS_H
#define SOUNDS_H

// Initialiser le syst�me de son
void init_sounds();

// Jouer un son depuis un fichier
void play_sound(const char* file_path);

// Nettoyer le syst�me de son
void cleanup_sounds();

#endif
