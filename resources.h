#ifndef RESOURCES_H
#define RESOURCES_H

#include <allegro.h>

// Nombre de fantômes
#define GHOST_COUNT 3

// Déclarations des bitmaps pour Pacman et les fantômes
extern BITMAP *pacman_image;
extern BITMAP *ghost_images[GHOST_COUNT];

// Prototypes des fonctions
void load_resources();
void destroy_resources();

#endif // RESOURCES_H
