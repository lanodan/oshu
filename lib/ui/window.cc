/**
 * \file lib/ui/window.cc
 * \ingroup ui_window
 */

#include "ui/window.h"

#include "core/log.h"
#include "game/game.h"
#include "game/tty.h"
#include "video/display.h"

#include "./screens/screens.h"

static void open_display(oshu::ui::window &w)
{
	w.display = new oshu_display {};
	if (oshu_open_display(w.display) < 0)
		throw std::runtime_error("no display, aborting");
	struct oshu_metadata *meta = &w.game.beatmap.metadata;
	std::ostringstream title;
	title << meta->artist << " - " << meta->title << " ♯ " << meta->version << " 𝄞 oshu!";
	SDL_SetWindowTitle(w.display->window, title.str().c_str());
	oshu_reset_view(w.display);
}

namespace oshu {
namespace ui {

window::window(oshu_game &game)
: game(game), screen(&oshu_play_screen)
{
	open_display(*this);
	if (game.beatmap.background_filename)
		oshu_load_background(display, game.beatmap.background_filename, &background);
	oshu_create_metadata_frame(display, &game.beatmap, &game.clock.system, &metadata);
	oshu_create_audio_progress_bar(display, &game.audio.music, &audio_progress_bar);
}

window::~window()
{
	oshu_destroy_background(&background);
	oshu_destroy_metadata_frame(&metadata);
	oshu_destroy_score_frame(&score);
	oshu_destroy_audio_progress_bar(&audio_progress_bar);
	oshu_close_display(display);
}

static void draw(window &w)
{
	oshu_game *game = &w.game;
	SDL_SetRenderDrawColor(w.display->renderer, 0, 0, 0, 255);
	SDL_RenderClear(w.display->renderer);
	w.screen->draw(w);
	SDL_RenderPresent(w.display->renderer);
}

void loop(window &w)
{
	oshu_game *game = &w.game;
	oshu_welcome(game);
	oshu_initialize_clock(game);

	SDL_Event event;
	int missed_frames = 0;

	while (!game->stop) {
		oshu_update_clock(game);
		oshu_reset_view(w.display);
		while (SDL_PollEvent(&event))
			w.screen->on_event(w, &event);
		w.screen->update(w);
		draw(w);

		/* Calling oshu_print_state before draw causes some flickering
		 * on the tty, for some reason. */
		if (w.screen == &oshu_play_screen)
			oshu_print_state(game);

		double advance = w.display->frame_duration - (SDL_GetTicks() / 1000. - game->clock.system);
		if (advance > 0) {
			SDL_Delay(advance * 1000);
		} else {
			missed_frames++;
			if (missed_frames == 1000) {
				oshu_log_warning("your computer is having a hard time keeping up");
				if (w.display->features)
					oshu_log_warning("try running oshu! with OSHU_QUALITY=low (see the man page)");
			}
		}
	}

	if (w.screen != &oshu_score_screen)
		puts("");
		/* write a new line to avoid conflict between the status line
		 * and the shell prompt */
	oshu_log_debug("%d missed frames", missed_frames);
}

}}
