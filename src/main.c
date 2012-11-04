#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "./game.h"

#define FPS 60

ALLEGRO_KEYBOARD_STATE *keys;

int main() {
    if (!al_init()) {
        puts("Could not initialise allegro");
        return 1;
    }
    if (!al_install_keyboard()) {
        puts("Could not initialise allegro keyboard subsystem");
        return 1;
    }
    keys = malloc(sizeof(ALLEGRO_KEYBOARD_STATE));
    if (!init_font()) {
        puts("Could not initialise allegro font subsystem");
        return 1;
    }

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

    ALLEGRO_DISPLAY* display = al_create_display(1, 1);
    if (!display) {
        puts("Could not initialise allegro display");
        return 1;
    }

    Vector size = {al_get_display_width(display),
                   al_get_display_height(display),
                   0
                  };

    Game* game = new_game(size);
    spawn_asteroid(game);

    ALLEGRO_TIMER* timer = al_create_timer(1.0/FPS);

    ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();
    if (!eq) {
        puts("Could not create allegro event queue");
        return 1;
    }
    al_register_event_source(eq, al_get_timer_event_source(timer));
    al_register_event_source(eq, al_get_keyboard_event_source());
    al_register_event_source(eq, al_get_display_event_source(display));

    al_start_timer(timer); // Start generating timer events
    ALLEGRO_EVENT* event = malloc(sizeof(ALLEGRO_EVENT));

    while (game->status == Playing) {
        al_get_keyboard_state(keys);

        if (al_key_down(keys, ALLEGRO_KEY_ESCAPE))
            game->status = Paused;
        if (al_key_down(keys, ALLEGRO_KEY_LEFT))
            rotate_ship_left(game);
        if (al_key_down(keys, ALLEGRO_KEY_RIGHT))
            rotate_ship_right(game);
        if (al_key_down(keys, ALLEGRO_KEY_UP))
            accelerate_ship(game);
        if (al_key_down(keys, ALLEGRO_KEY_DOWN))
            deccelerate_ship(game);
        if (al_key_down(keys, ALLEGRO_KEY_SPACE))
            shoot_bullet(game->bulletmanager, game->ship);

        al_wait_for_event(eq, NULL);
        if (!al_get_next_event(eq, event)) {
            continue;
        }
        switch(event->type) {
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            game->size.x = event->display.x;
            game->size.y = event->display.y;
            al_acknowledge_resize(display);
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            game->status = Quit;
            break;

        case ALLEGRO_EVENT_TIMER:
            update_game(game);

            draw_game(game);

            al_flip_display();
            break;

            /*        case ALLEGRO_EVENT_KEY_CHAR:
              if (event->keyboard.keycode == ALLEGRO_KEY_SPACE)
                  shoot_bullet(game->bulletmanager, game->ship);
                  break;*/
        }
    }

    free(event);
    delete_game(game);
    return 0;
}
