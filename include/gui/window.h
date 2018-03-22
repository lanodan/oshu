/**
 * \file include/gui/window.h
 * \ingroup gui_window
 */

#pragma once

#include "gui/audio.h"
#include "gui/background.h"
#include "gui/metadata.h"
#include "gui/score.h"

struct oshu_game;
struct oshu_game_screen;

namespace oshu {
namespace gui {

/**
 * \defgroup gui_window Window
 * \ingroup gui
 *
 * \brief
 * Manage the game's main window.
 *
 * \{
 */

/**
 * One game window.
 *
 * Multiple game windows are currently not supported, and probably never will.
 */
struct window {
	window(oshu_game&);
	~window();
	/**
	 * A reference a game object. It is not owned by the window and must
	 * live longer than the window.
	 */
	oshu_game &game;
	oshu_game_screen *screen;
	oshu_background background {};
	oshu_metadata_frame metadata {};
	oshu_score_frame score {};
	oshu_audio_progress_bar audio_progress_bar {};
};

/**
 * Start the main loop. Stops when the user closes the window, or when the game
 * exits.
 */
void loop(window&);

/** \} */

}}