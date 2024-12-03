#ifndef GAME_H
#define GAME_H

// Dimensions du labyrinthe
#define ROWS 21
#define COLS 21
#define TILE_SIZE 32 // Taille d'une case
#define SCREEN_W (COLS * TILE_SIZE) // Largeur de l'écran
#define SCREEN_H (ROWS * TILE_SIZE) // Hauteur de l'écran
#define GHOST_COUNT 3 // Nombre de fantômes

// Constantes et variables globales
#define TICKS_PER_SECOND 60 // Définir le nombre de ticks par seconde

// Variable globale pour le compteur de ticks
extern volatile int ticks;

// Fonction de timer
void ticker();

// Structure représentant une entité
typedef struct {
    int x, y;        // Position
    int direction;   // Direction (0: gauche, 1: droite, 2: haut, 3: bas)
} Entity;

// Déclaration des variables globales
extern Entity pacman;
extern Entity ghosts[GHOST_COUNT];
extern int maze[ROWS][COLS];
extern int score;
extern int lives;

// Déclaration des fonctions
void init_game();
void handle_input(int key);
void update_game();
void draw_game(BITMAP *buffer);
int check_collision(int x, int y);

extern Entity pacman;
extern Entity ghosts[GHOST_COUNT];
extern int maze[ROWS][COLS];

void init_game();
void handle_input(int key);
void update_game();
void draw_game(BITMAP *buffer);
int check_collision(int x, int y);
void check_game_won();

extern int current_level; // Variable pour le niveau actuel


#endif
