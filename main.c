#include <allegro.h>
#include "game.h"
#include "audio.h"
#include "levels.h"

int main() {
    // Initialisation Allegro
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_W, SCREEN_H, 0, 0);

      // Initialisation de l'audio
    init_audio();
    load_sounds();

     // Jouer la musique d'intro
    play_intro_music();

    // Attendre la fin de la musique d'intro (environ 4 secondes)
    rest(4000);

    // Jouer le son des fantômes en boucle
    play_ghost_sound();

    // Charger les ressources
    load_resources();

    // Initialisation du buffer et des entités
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    init_game();

    // Afficher le menu d'accueil
    if (!display_menu(buffer)) {
        destroy_bitmap(buffer);
        return 0; // Quitter si le menu échoue
    }

    // Boucle principale du jeu
    while (!key[KEY_ESC]) {
        // Gérer l'entrée utilisateur
        if (keypressed()) {
            int key = readkey() >> 8;
            handle_input(key);
        }

        // Mettre à jour le jeu
        update_game();

        // Dessiner le jeu
        draw_game(buffer);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        // Limiter la vitesse de la boucle
        rest(20);
    }

    // Nettoyage audio
    cleanup_audio();
    destroy_resources();
    destroy_bitmap(buffer);
    return 0;
}
END_OF_MAIN();
