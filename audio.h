#ifndef AUDIO_H
#define AUDIO_H

#include <allegro.h>

// Déclarations des sons
extern SAMPLE *collect_sound;
extern SAMPLE *game_over_sound;
extern SAMPLE *intro_music;
extern SAMPLE *ghost_sound; // Son pour les fantômes

// Fonctions pour les effets sonores
void init_audio();
void load_sounds();
void play_collect_sound();
void play_game_over_sound();
void play_intro_music();
void play_ghost_sound();
void cleanup_audio();

#endif
