#include "oshu.h"

int main(int argc, char **argv)
{
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		return 1;
	}
	oshu_log_debug("successfully initialized");

	oshu_audio_init();
	oshu_log_debug("initialized the audio module");

	struct oshu_audio_stream *stream;
	if (oshu_audio_open("test.ogg", &stream)) {
		oshu_log_error("no audio, aborting");
		return 2;
	}
	oshu_log_debug("audio opened");

	oshu_log_info("starting the playback");
	oshu_audio_play(stream);
	SDL_Delay(10000);

	oshu_audio_close(&stream);
	SDL_Quit();
	return 0;
}
