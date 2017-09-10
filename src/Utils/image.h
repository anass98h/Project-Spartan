#pragma once

#include "../interfaces.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>

namespace Image {
    void InitImages();

    GLuint LoadImage( const char* const path );
}
