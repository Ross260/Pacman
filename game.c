#include <allegro.h>
#include "game.h"
#include <stdlib.h> // Pour rand()
#include "audio.h"
#include "resources.h"
#include "levels.h"
#include <stdbool.h>


// Entités
Entity pacman;
Entity ghosts[GHOST_COUNT];

int score = 0;        // Score de Pacman
int lives = 3;        // Vies restantes

int current_level = 0; // Initialisation au niveau 0
int maze[ROWS][COLS];

void init_game() {
    pacman.x = 1;
    pacman.y = 1;
    pacman.direction = -1;

    // Initialisation des trois fantômes
    for (int i = 0; i < GHOST_COUNT; i++) {
        ghosts[i].x = COLS / 2 + (i % 3); // Répartis horizontalement
        ghosts[i].y = ROWS / 2 + (i / 3); // Répartis verticalement
        ghosts[i].direction = rand() % 4; // Direction aléatoire : 0 à 3
    }

    // Charger le labyrinthe du niveau actuel
    load_maze(current_level);
}


int check_collision(int x, int y) {
    if (x < 0 || x >= COLS || y < 0 || y >= ROWS) return 0; // Hors des limites
    return maze[y][x] != 1; // Retourne vrai si ce n'est pas un mur
}



void handle_input(int key) {
    if (key == KEY_LEFT) pacman.direction = 0;
    else if (key == KEY_RIGHT) pacman.direction = 1;
    else if (key == KEY_UP) pacman.direction = 2;
    else if (key == KEY_DOWN) pacman.direction = 3;
}

void update_game() {
    static int ghost_move_timer = 0; // Compteur pour ralentir les fantômes
    ghost_move_timer++;

    // Les fantômes ne bougent qu'une fois tous les 6 cycles
    if (ghost_move_timer < 6) return; // Ajustez ici pour ralentir/accélérer les fantômes
    ghost_move_timer = 0; // Réinitialiser le compteur

    // Mise à jour de Pacman
    int new_x = pacman.x;
    int new_y = pacman.y;

    if (pacman.direction == 0) new_x--;      // Gauche
    else if (pacman.direction == 1) new_x++; // Droite
    else if (pacman.direction == 2) new_y--; // Haut
    else if (pacman.direction == 3) new_y++; // Bas

    if (check_collision(new_x, new_y)) {
        pacman.x = new_x;
        pacman.y = new_y;

        // Vérifier si Pacman collecte un point
        if (maze[new_y][new_x] == 2) {
            maze[new_y][new_x] = 0; // Supprimer le point
            score += 10;           // Ajouter 10 points au score
            play_collect_sound();  // Jouer le son de collecte
        }
    }

    // Déplacement des fantômes
    for (int i = 0; i < GHOST_COUNT; i++) {
        move_ghost_randomly(&ghosts[i]); // Déplacer chaque fantôme

       // Vérifier si Pacman entre en contact avec un fantôme
        if (ghosts[i].x == pacman.x && ghosts[i].y == pacman.y) {
            play_game_over_sound(); // Jouer le son de game over
            lives--; // Réduire le nombre de vies
            if (lives <= 0) {
                if (!game_over_menu()) { // Si le joueur choisit de quitter
                    exit(0);
                } else {
                    reset_game(); // Si le joueur choisit de rejouer
                }
            } else {
                // Réinitialiser Pacman au point de départ
                pacman.x = 1;
                pacman.y = 1;
                pacman.direction = -1;
            }
        }
    }

    // Vérifier si le niveau est terminé
    check_next_level();
}


void draw_game(BITMAP *buffer) {
    clear_to_color(buffer, makecol(0, 0, 0)); // Fond noir

    // Dessiner le labyrinthe
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 1) {
                rectfill(buffer, j * TILE_SIZE, i * TILE_SIZE,
                         (j + 1) * TILE_SIZE, (i + 1) * TILE_SIZE, makecol(0, 0, 255)); // Mur bleu
            } else if (maze[i][j] == 2) {
                circlefill(buffer, j * TILE_SIZE + TILE_SIZE / 2, i * TILE_SIZE + TILE_SIZE / 2, 4, makecol(255, 255, 0)); // Point jaune
            }
        }
    }

    // Dessiner Pacman (redimensionné)
    if (pacman_image) {
        stretch_sprite(buffer, pacman_image, pacman.x * TILE_SIZE, pacman.y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    }

    // Dessiner les fantômes (redimensionnés)
    for (int i = 0; i < GHOST_COUNT; i++) {
        if (ghost_images[i]) {
            stretch_sprite(buffer, ghost_images[i], ghosts[i].x * TILE_SIZE, ghosts[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }

    // Afficher le score et les vies
    textprintf_ex(buffer, font, 10, 10, makecol(255, 255, 255), -1, "Score: %d", score);
    textprintf_ex(buffer, font, 10, 30, makecol(255, 255, 255), -1, "Vies: %d", lives);
}


// Fonction pour afficher le menu d'accueil
int display_menu(BITMAP *buffer) {
    int start_game = 0; // 0 = menu actif, 1 = démarrer le jeu

    // Couleurs
    int background_color = makecol(0, 0, 0);       // Noir
    int text_color = makecol(255, 255, 255);       // Blanc
    int button_color = makecol(0, 128, 255);       // Bleu
    int button_hover_color = makecol(0, 192, 255); // Bleu clair

    // Dimensions du bouton "Démarrer"
    int button_x1 = SCREEN_W / 2 - 75;
    int button_y1 = SCREEN_H / 2;
    int button_x2 = SCREEN_W / 2 + 75;
    int button_y2 = SCREEN_H / 2 + 50;

    show_mouse(screen); // Afficher la souris

    while (!start_game) {
        // Effacer l'écran
        clear_to_color(buffer, background_color);

        // Titre du jeu
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 4, text_color, -1, "Pacman");

        // Instructions
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 3, text_color, -1,
                             "Collectez tous les points sans vous faire attraper par les fantômes !");
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 3 + 20, text_color, -1,
                             "Utilisez les touches fléchées du clavier pour déplacer Pacman.");

        // Dessiner le bouton "Démarrer"
        int is_hovered = (mouse_x >= button_x1 && mouse_x <= button_x2 && mouse_y >= button_y1 && mouse_y <= button_y2);

        // Couleur du bouton (normal ou survolé)
        rectfill(buffer, button_x1, button_y1, button_x2, button_y2, is_hovered ? button_hover_color : button_color);

        // Texte du bouton
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, SCREEN_H / 2 + 15, text_color, -1, "START");

        // Afficher le buffer
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        // Gestion des événements
        if (is_hovered && (mouse_b & 1)) { // Clique gauche sur le bouton
            start_game = 1;
        }

        if (key[KEY_ENTER]) { // Appuyer sur Entrée pour démarrer
            start_game = 1;
        }

        if (key[KEY_ESC]) { // Échap pour quitter
            return 0;
        }

        rest(20); // Réduire la fréquence de la boucle
    }

    clear_keybuf(); // Effacer le buffer des touches
    return start_game;
}

// Déplacement des fantômes : combine aléatoire et poursuite
void move_ghost_randomly(Entity *ghost) {
    int possible_directions[4] = {0, 1, 2, 3}; // Gauche, Droite, Haut, Bas
    int valid_directions[4] = {0, 0, 0, 0};    // Marque les directions valides
    int direction_count = 0;

    // Vérifier les directions valides
    for (int d = 0; d < 4; d++) {
        int test_x = ghost->x;
        int test_y = ghost->y;

        if (possible_directions[d] == 0) test_x--;      // Gauche
        else if (possible_directions[d] == 1) test_x++; // Droite
        else if (possible_directions[d] == 2) test_y--; // Haut
        else if (possible_directions[d] == 3) test_y++; // Bas

        // Vérifiez si la case est valide (pas un mur, pas hors limites)
        if (check_collision(test_x, test_y)) {
            valid_directions[d] = 1; // Marquer comme valide
            direction_count++;
        }
    }

    // Empêcher le retour immédiat dans la direction opposée
    if (ghost->direction == 0 && direction_count > 1) valid_directions[1] = 0; // Gauche -> pas Droite
    else if (ghost->direction == 1 && direction_count > 1) valid_directions[0] = 0; // Droite -> pas Gauche
    else if (ghost->direction == 2 && direction_count > 1) valid_directions[3] = 0; // Haut -> pas Bas
    else if (ghost->direction == 3 && direction_count > 1) valid_directions[2] = 0; // Bas -> pas Haut

    // Si aucune direction valide, reste immobile
    if (direction_count == 0) {
        ghost->direction = -1; // Immobile
        return;
    }

    // Si une seule direction est valide, la suivre automatiquement
    if (direction_count == 1) {
        for (int d = 0; d < 4; d++) {
            if (valid_directions[d]) {
                ghost->direction = d;
                break;
            }
        }
    } else {
        // Choisir une direction valide aléatoire
        int random_choice = rand() % direction_count;
        for (int d = 0; d < 4; d++) {
            if (valid_directions[d]) {
                if (random_choice == 0) {
                    ghost->direction = d;
                    break;
                }
                random_choice--;
            }
        }
    }

    // Appliquer le déplacement
    int new_x = ghost->x;
    int new_y = ghost->y;

    if (ghost->direction == 0) new_x--;      // Gauche
    else if (ghost->direction == 1) new_x++; // Droite
    else if (ghost->direction == 2) new_y--; // Haut
    else if (ghost->direction == 3) new_y++; // Bas

    // Déplacer le fantôme uniquement si la case est valide
    if (check_collision(new_x, new_y)) {
        ghost->x = new_x;
        ghost->y = new_y;
    }
}

int is_occupied_by_ghost(int x, int y) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        if (ghosts[i].x == x && ghosts[i].y == y) {
            return 1; // Occupé
        }
    }
    return 0; // Libre
}

// Fonction pour charger un labyrinthe spécifique
void load_maze(int level) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = levels[level][i][j];
        }
    }
}

// Exemple d'utilisation
void check_next_level() {
    int points_left = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 2) points_left++;
        }
    }

    if (points_left == 0) {
        current_level++;
        if (current_level >= NUM_LEVELS) {
            allegro_message("Félicitations ! Vous avez terminé le jeu !");
            exit(0);
        } else {
            allegro_message("Niveau suivant !");
            load_maze(current_level);
        }
    }
}

void check_game_won() {
    int points_left = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 2) { // Vérifie s'il reste des points.
                points_left++;
            }
        }
    }

    if (points_left == 0) {
        allegro_message("Félicitations, vous avez gagné !");
        exit(0); // Quitter le jeu après la victoire.
    }
}
int game_over_menu() {
    // Efface l'écran et affiche le message
    clear_to_color(screen, makecol(0, 0, 0)); // Fond noir
    textprintf_ex(screen, font, SCREEN_W / 2 - 50, SCREEN_H / 2 - 20, makecol(255, 255, 255), -1, "GAME OVER");
    textprintf_ex(screen, font, SCREEN_W / 2 - 100, SCREEN_H / 2 + 10, makecol(255, 255, 255), -1, "Press ENTER to Restart");
    textprintf_ex(screen, font, SCREEN_W / 2 - 100, SCREEN_H / 2 + 30, makecol(255, 255, 255), -1, "Press ESC to Quit");

    while (true) {
        if (key[KEY_ENTER]) {
            clear_keybuf();
            return 1; // Rejouer
        }
        if (key[KEY_ESC]) {
            clear_keybuf();
            return 0; // Quitter
        }
    }
}

void reset_game() {
    // Réinitialiser Pac-Man
    pacman.x = 1;
    pacman.y = 1;
    pacman.direction = -1;

    // Réinitialiser les vies et le score
    lives = 3;
    score = 0;

    // Recharger le labyrinthe initial
    current_level = 0; // Repartir du premier niveau
    load_maze(current_level);

    // Réinitialiser les fantômes
      for (int i = 0; i < GHOST_COUNT; i++) {
        ghosts[i].x = COLS / 2 + (i % 3);
        ghosts[i].y = ROWS / 2 + (i / 3);
        ghosts[i].direction = rand() % 4;
    }
}
