#include <allegro.h>
#include "audio.h"

// Variables globales pour les sons
SAMPLE *collect_sound = NULL;
SAMPLE *game_over_sound = NULL;
SAMPLE *intro_music = NULL;
SAMPLE *ghost_sound = NULL; // Son pour les fantômes

// Initialisation du système audio
void init_audio() {
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
        allegro_message("Erreur : Impossible d'initialiser le son !");
        exit(1); // Quitter en cas d'échec
    }
}

// Charger les fichiers audio
void load_sounds() {
    collect_sound = load_sample("pacman_chomp.wav");
    if (!collect_sound) {
        allegro_message("Erreur : Impossible de charger pacman_chomp.wav");
    }

    game_over_sound = load_sample("pacman_death.wav");
    if (!game_over_sound) {
        allegro_message("Erreur : Impossible de charger game_over.wav");
    }

    intro_music = load_sample("pacman_beginning.wav");
    if (!intro_music) {
        allegro_message("Erreur : Impossible de charger pacman_beginning.wav");
    }

    ghost_sound = load_sample("Voicy_Ghost Siren sound.wav"); // Chargez le fichier WAV pour les fantômes
    if (!ghost_sound) {
        allegro_message("Erreur : Impossible de charger ghost_sound.wav");
    }
}

// Jouer le son de collecte
void play_collect_sound() {
    if (collect_sound) {
        play_sample(collect_sound, 255, 128, 1000, 0);
    }
}

// Jouer le son de game over
void play_game_over_sound() {
    if (game_over_sound) {
        play_sample(game_over_sound, 255, 128, 1000, 0);
    }
}

// Jouer la musique d'intro
void play_intro_music() {
    if (intro_music) {
        play_sample(intro_music, 255, 128, 1000, 0);
    }
}

// Jouer le son des fantômes
void play_ghost_sound() {
    if (ghost_sound) {
        play_sample(ghost_sound, 255, 128, 1000, 1); // Boucle activée
    }
}

// Nettoyer les sons pour libérer la mémoire
void cleanup_audio() {
    if (collect_sound) {
        destroy_sample(collect_sound);
        collect_sound = NULL;
    }
    if (game_over_sound) {
        destroy_sample(game_over_sound);
        game_over_sound = NULL;
    }
    if (intro_music) {
        destroy_sample(intro_music);
        intro_music = NULL;
    }
    if (ghost_sound) {
        destroy_sample(ghost_sound);
        ghost_sound = NULL;
    }
}
