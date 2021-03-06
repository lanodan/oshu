/**
 * \file game/helpers.cc
 * \ingroup game_helpers
 */

#include "game/game.h"

struct oshu_hit* oshu_look_hit_back(struct oshu_game *game, double offset)
{
	struct oshu_hit *hit = game->hit_cursor;
	double target = game->clock.now - offset;
	/* seek backward */
	while (oshu_hit_end_time(hit) > target)
		hit = hit->previous;
	/* seek forward */
	while (oshu_hit_end_time(hit) < target)
		hit = hit->next;
	/* here we have the guarantee that hit->time >= target */
	return hit;
}

struct oshu_hit* oshu_look_hit_up(struct oshu_game *game, double offset)
{
	struct oshu_hit *hit = game->hit_cursor;
	double target = game->clock.now + offset;
	/* seek forward */
	while (hit->time < target)
		hit = hit->next;
	/* seek backward */
	while (hit->time > target)
		hit = hit->previous;
	/* here we have the guarantee that hit->time <= target */
	return hit;
}

struct oshu_hit* oshu_next_hit(struct oshu_game *game)
{
	struct oshu_hit *hit = game->hit_cursor;
	for (; hit->next; hit = hit->next) {
		if (hit->type & (OSHU_CIRCLE_HIT | OSHU_SLIDER_HIT))
			break;
	}
	return hit;
}

struct oshu_hit* oshu_previous_hit(struct oshu_game *game)
{
	struct oshu_hit *hit = game->hit_cursor;
	if (!hit->previous)
		return hit;
	for (hit = hit->previous; hit->previous; hit = hit->previous) {
		if (hit->type & (OSHU_CIRCLE_HIT | OSHU_SLIDER_HIT))
			break;
	}
	return hit;
}
