/**
 * \file graphics/paint.c
 * \ingroup graphics_paint
 */

#include "graphics/paint.h"

#include "graphics/display.h"
#include "log.h"

#include <SDL2/SDL.h>

static void destroy_painter(struct oshu_painter *painter)
{
	if (painter->cr) {
		cairo_destroy(painter->cr);
		painter->cr = NULL;
	}
	if (painter->surface) {
		cairo_surface_destroy(painter->surface);
		painter->surface = NULL;
	}
	if (painter->destination) {
		SDL_FreeSurface(painter->destination);
		painter->destination = NULL;
	}
}

int oshu_start_painting(oshu_size size, oshu_point origin, struct oshu_painter *painter)
{
	memset(painter, 0, sizeof(*painter));
	painter->size = size;
	painter->origin = origin;

	/* 1. SDL */
	painter->destination = SDL_CreateRGBSurface(
		0, creal(size), cimag(size), 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (!painter->destination) {
		oshu_log_error("could not create a painting surface: %s", SDL_GetError());
		goto fail;
	}
	if (SDL_LockSurface(painter->destination) < 0) {
		oshu_log_error("could not lock the surface: %s", SDL_GetError());
		goto fail;
	}

	/* 2. Cairo surface */
	painter->surface = cairo_image_surface_create_for_data(
		painter->destination->pixels, CAIRO_FORMAT_ARGB32,
		creal(size), cimag(size), painter->destination->pitch);
	cairo_status_t s = cairo_surface_status(painter->surface);
	if (s != CAIRO_STATUS_SUCCESS) {
		oshu_log_error("cairo surface error: %s", cairo_status_to_string(s));
		goto fail;
	}

	/* 3. Cairo context */
	painter->cr = cairo_create(painter->surface);
	s = cairo_status(painter->cr);
	if (s != CAIRO_STATUS_SUCCESS) {
		oshu_log_error("cairo error: %s", cairo_status_to_string(s));
		goto fail;
	}

	cairo_translate(painter->cr, creal(origin), cimag(origin));
	return 0;

fail:
	destroy_painter(painter);
	return -1;
}

int oshu_finish_painting(struct oshu_painter *painter, struct oshu_display *display, struct oshu_texture *texture)
{
	int rc = 0;
	SDL_UnlockSurface(painter->destination);
	texture->size = painter->size;
	texture->origin = painter->origin;
	texture->texture = SDL_CreateTextureFromSurface(display->renderer, painter->destination);
	if (!texture->texture) {
		oshu_log_error("error uploading texture: %s", SDL_GetError());
		rc = -1;
	}
	destroy_painter(painter);
	return rc;
}