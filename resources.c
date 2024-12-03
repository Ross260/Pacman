#include <allegro.h>
#include "resources.h"

// Déclarations des bitmaps
BITMAP *pacman_image = NULL;
BITMAP *ghost_images[GHOST_COUNT] = {NULL};

// Fonction pour charger les ressources
void load_resources() {
    // Chargement de l'image de Pacman
    pacman_image = load_bitmap("asset/pacman.bmp", NULL);
    if (!pacman_image) {
        allegro_message("Erreur");
        exit(1); // Quitte le jeu en cas d'échec
    }

    // Chargement des images des fantômes
    ghost_images[0] = load_bitmap("asset/fred.bmp", NULL); // Rouge
    if (!ghost_images[0]) {
        allegro_message("Erreur");
        exit(1);
    }

    ghost_images[1] = load_bitmap("asset/fblue.bmp", NULL); // Bleu
    if (!ghost_images[1]) {
        allegro_message("Erreur");
        exit(1);
    }

    ghost_images[2] = load_bitmap("asset/fpink.bmp", NULL); // Rose
    if (!ghost_images[2]) {
        allegro_message("Erreur");
        exit(1);
    }
}

// Fonction pour libérer les ressources
void destroy_resources() {
    if (pacman_image) {
        destroy_bitmap(pacman_image);
        pacman_image = NULL;
    }

    for (int i = 0; i < GHOST_COUNT; i++) {
        if (ghost_images[i]) {
            destroy_bitmap(ghost_images[i]);
            ghost_images[i] = NULL;
        }
    }
}
