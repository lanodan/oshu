/**
 * \file lib/ui/screens/screens.h
 * \ingroup ui_screens
 */

#pragma once

union SDL_Event;

namespace oshu {
namespace ui {
struct window;
}}

/**
 * \defgroup ui_screens Screens
 * \ingroup ui
 *
 * \brief
 * Game states.
 *
 * The main idea is that there is the main loop iterating at a constant 60 FPS
 * rate, and handling the common events, like window closing.
 *
 * Then, there are the more specific states. You could imagine a welcome
 * screen, a song selection screen, the actual in-game screen, the pause
 * screen, and score screen.
 *
 * Handling all these various screens in the same loop quickly becomes
 * unreadable. Instead, we'll define a screen by a few entry points, and each
 * screen will be its own master. Most screens would share a more or less
 * common structure, and would share code when appropriate. The main point is
 * that event handlers should never cascade the if-else conditionals depending
 * on the state.
 *
 * \dot
 * digraph modules {
 * 	rankdir=LR;
 * 	node [shape=rect];
 * 	subgraph {
 * 		rank=same;
 * 		Pause -> Play;
 * 		Play -> Pause;
 * 	}
 * 	Start -> Play;
 * 	Play -> Finish;
 * 	Finish -> Exit;
 * 	Pause -> Exit;
 * 	Start [shape=none];
 * 	Exit [shape=none];
 * }
 * \enddot
 *
 * \{
 */

/**
 * Define a game screen by its behavior.
 */
struct oshu_game_screen {
	/**
	 * \todo
	 * Drop it if it's not used.
	 */
	const char *name;
	/**
	 * Called every time an event happens.
	 *
	 * The main game module may preprocess the event, or even filter it.
	 * For instance, when the window is closed.
	 */
	int (*on_event)(oshu::ui::window&, union SDL_Event *event);
	/**
	 * The update function is run at every iteration of the main loop.
	 */
	int (*update)(oshu::ui::window&);
	/**
	 * Draw the screen.
	 *
	 * The screen is cleared before this function is called, and the
	 * display is refreshed after.
	 *
	 * Beside that, no implicit drawing is done for you.
	 */
	int (*draw)(oshu::ui::window&);
};

/* Defined in play.c */
extern struct oshu_game_screen oshu_play_screen;

/* Defined in pause.c */
extern struct oshu_game_screen oshu_pause_screen;

/* Defined in score.c */
extern struct oshu_game_screen oshu_score_screen;

/** \} */
