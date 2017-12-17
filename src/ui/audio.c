/**
 * \file ui/audio.c
 * \ingroup ui_audio
 */

#include "ui/audio.h"

#include "audio/stream.h"
#include "graphics/display.h"

#include <assert.h>
#include <SDL2/SDL.h>

int oshu_create_audio_progress_bar(struct oshu_display *display, struct oshu_stream *stream, struct oshu_audio_progress_bar_widget *bar)
{
	bar->display = display;
	bar->stream = stream;
	return 0;
}

void oshu_show_audio_progress_bar(struct oshu_audio_progress_bar_widget *bar)
{
	assert (bar->stream->duration != 0);
	double progression = bar->stream->current_timestamp / bar->stream->duration;
	if (progression < 0)
		progression = 0;
	else if (progression > 1)
		progression = 1;

	double height = 4;
	SDL_Rect shape = {
		.x = 0,
		.y = cimag(bar->display->view.size) - height,
		.w = progression * creal(bar->display->view.size),
		.h = height,
	};
	SDL_SetRenderDrawColor(bar->display->renderer, 255, 255, 255, 48);
	SDL_SetRenderDrawBlendMode(bar->display->renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(bar->display->renderer, &shape);
}

void oshu_destroy_audio_progress_bar(struct oshu_audio_progress_bar_widget *bar)
{
}