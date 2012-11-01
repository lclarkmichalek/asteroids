#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "./game.h"

#define FPS 60

ALLEGRO_FONT *ttf_font = NULL;

int main() {
    if (!al_init()) {
        puts("Could not initialise allegro\n");
        return 1;
    }
    if (!al_install_keyboard()) {
        puts("Could not initialise allegro keyboard subsystem\n");
        return 1;
    }
    if (!al_init_ttf_addon()) {
        puts("Could not initialise allegro font subsystem\n");
        return 1;
    }
    ttf_font = al_load_ttf_font("font.ttf", 30, 0);
    if (!ttf_font) {
        puts("Could not load font, make sure file font.ttf is in current directory\n");
        return 1;
    }

    Vector size = {500, 500, 0};

    ALLEGRO_DISPLAY* display = al_create_display(size.x, size.y);
    if (!display) {
        puts("Could not initialise allegro display\n");
        return 1;
    }

    Game* game = new_game(size);

    ALLEGRO_TIMER* timer = al_create_timer(1.0/FPS);

    ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();
    if (!eq) {
        puts("Could not create allegro event queue\n");
        return 1;
    }
    al_register_event_source(eq, al_get_timer_event_source(timer));
    al_register_event_source(eq, al_get_keyboard_event_source());
    al_register_event_source(eq, al_get_display_event_source(display));

    al_start_timer(timer); // Start generating timer events
    ALLEGRO_EVENT* event = malloc(sizeof(ALLEGRO_EVENT));

    bool running = true;
    while (running) {
        al_wait_for_event(eq, NULL);
        if (!al_get_next_event(eq, event)) {
            puts("No Event\n");
            continue;
        }
        switch(event->type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;
        case ALLEGRO_EVENT_KEY_CHAR:
            switch(event->keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                running = false;
                break;
            case ALLEGRO_KEY_LEFT:
                rotate_ship_left(game);
                break;
            case ALLEGRO_KEY_RIGHT:
                rotate_ship_right(game);
                break;
            }
            break;
        case ALLEGRO_EVENT_TIMER:
            update_game(game);
            if (game->lives < 0) {
                running = false;
                break;
            }

            draw_game(game);

            al_flip_display();
        }
    }

    free(event);
    delete_game(game);
    return 0;
}
