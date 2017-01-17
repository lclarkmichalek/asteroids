#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define ALLEGRO_NO_MAGIC_MAIN
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "./game.h"
#include "./main.h"

#define FPS 60

ALLEGRO_KEYBOARD_STATE *keys;

int real_main(int argc, char **argv) {
	SRAND48();

    if (!al_init()) {
        puts("Could not initialise allegro");
        return 1;
    }

    if (!al_install_keyboard()) {
        puts("Could not initialise allegro keyboard subsystem");
        return 1;
    }

	if (!al_init_primitives_addon()) {
		puts("Could not initialise allegro primitives addon");
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

    Vector size = new_vector();
    size.x = al_get_display_width(display);
    size.y = al_get_display_height(display);

    Game* game = new_game(size);
    int i;
    for (i = 0; i < ASTEROIDN; i++)
        spawn_asteroid(game);

    ALLEGRO_TIMER* timer = al_create_timer(1.0/FPS);

    ALLEGRO_EVENT_QUEUE *timereq = al_create_event_queue();
    ALLEGRO_EVENT_QUEUE *genericeq = al_create_event_queue();
    if (!timereq || !genericeq) {
        puts("Could not create allegro event queue");
        return 1;
    }
    al_register_event_source(timereq, al_get_timer_event_source(timer));

    al_register_event_source(genericeq, al_get_keyboard_event_source());
    al_register_event_source(genericeq, al_get_display_event_source(display));

    al_start_timer(timer); // Start generating timer events
    ALLEGRO_EVENT *timerevent = malloc(sizeof(ALLEGRO_EVENT));
    ALLEGRO_EVENT *genericevent = malloc(sizeof(ALLEGRO_EVENT));

    float last_drawn, now;
    last_drawn = now = al_get_time();
    while (game->status != Quit) {
        al_get_keyboard_state(keys);

        al_wait_for_event(timereq, NULL);
        al_get_next_event(timereq, timerevent);
        // No need to fill up the queue if we are late drawing frames
        al_flush_event_queue(timereq);

        handle_key_status(game, keys);

        while(al_get_next_event(genericeq, genericevent))
            switch(genericevent->type) {
            case ALLEGRO_EVENT_KEY_DOWN:
                handle_key_event(game, genericevent->keyboard.keycode);
                break;

            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                game->size.x = genericevent->display.x;
                game->size.y = genericevent->display.y;
                al_acknowledge_resize(display);
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game->status = Quit;
                break;
            }

        now = al_get_time();
        update_game(game, now - last_drawn);
        last_drawn = now;

        draw_game(game, game->status == Playing ? 1 : 0.2);
        switch(game->status) {
        case Playing:
            break;
        case Paused:
            draw_paused(game);
            break;
        case Won:
            draw_won(game);
            break;
        case Lost:
            draw_lost(game);
            break;
        default:
            break;
        }

        al_flip_display();
    }

    free(timerevent);
    free(genericevent);
    delete_game(game);
    return 0;
}

int main(int argc, char **argv) {
  return al_run_main(argc, argv, real_main);
}
