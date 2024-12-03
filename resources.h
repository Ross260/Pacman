#ifndef RESOURCES_H
#define RESOURCES_H

#include <allegro.h>

// Nombre de fant�mes
#define GHOST_COUNT 3

// D�clarations des bitmaps pour Pacman et les fant�mes
extern BITMAP *pacman_image;
extern BITMAP *ghost_images[GHOST_COUNT];

// Prototypes des fonctions
void load_resources();
void destroy_resources();

#endif // RESOURCES_H
