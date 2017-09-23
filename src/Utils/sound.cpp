#include "sound.h"

static const unsigned char hitsound_mp3[21175] = {
#include "../assets/sound/hitsound.txt"
};

void Sound::InitSounds() {
    char currentDir[PATH_MAX];
    getcwd(currentDir, sizeof(currentDir));

    std::string sound(currentDir);
    sound.append(XORSTR("/csgo/sound/hitsound.mp3"));

    FILE* hitsound = fopen(sound.c_str(), XORSTR("wb"));
    if(hitsound == NULL) {
        cvar->ConsoleDPrintf( XORSTR( "Error opening ./csgo/sound/hitsound.mp3\n" ) );
        return;
    }
    fwrite( hitsound_mp3, sizeof( char ), 21175, hitsound );
    fclose(hitsound);
}
