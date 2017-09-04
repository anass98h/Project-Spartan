#include "image.h"

static const unsigned char speedometer_png[68895] = {
#include "../assets/img/speedometer.txt"
};

static const unsigned char gas_png[15621] = {
#include "../assets/img/gas.txt"
};

static const unsigned char needle_png[8071] = {
#include "../assets/img/needle.txt"
};

static const unsigned char wheel_png[321023] = {
#include "../assets/img/wheel.txt"
};

static const unsigned char oil_png[15710] = {
#include "../assets/img/oil.txt"
};

GLuint Image::LoadImage( const char * const path )
{
    SDL_Surface *surface = IMG_Load(path);
    if( !surface )
    {
        cvar->ConsoleDPrintf(XORSTR("(Image::LoadImage): Error: Could not load Image \"%s\" Error: \"%s\"\n"), path, IMG_GetError());
        return EXIT_FAILURE;
    }


    int mode = GL_RGB;
    switch (surface->format->BytesPerPixel)
    {
        case 4:
            if( surface->format->Rmask == 0x000000ff)
                mode = GL_RGBA;
            else
                mode = GL_BGRA;
            break;
        case 3:
            if( surface->format->Rmask == 0x000000ff)
                mode = GL_RGB;
            else
                mode = GL_BGR;
            break;
        default:
            cvar->ConsoleDPrintf(XORSTR("(Image::LoadImage): Error: Image \"%s\" is not truecolor\n"), path);
            return EXIT_FAILURE;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D,
                             0,
                             mode,
                             surface->w,
                             surface->h,
                             0,
                             mode,
                             GL_UNSIGNED_BYTE,
                             surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // I think IMG_Quit() handles these.
    //SDL_DestroyTexture(texture);
    //SDL_FreeSurface(surface);

    return texture;
}


void Image::InitImages( )
{

    FILE *needle = fopen(XORSTR("/tmp/needle.png"), "wb");
    if( needle == NULL )
    {
        cvar->ConsoleDPrintf("Error opening /tmp/needle.png\n");
        return;
    }
    fwrite(needle_png, sizeof(unsigned char), 8071, needle);
    fclose(needle);

    FILE *speedo = fopen(XORSTR("/tmp/speedometer.png"), "wb");
    if( speedo == NULL )
    {
        cvar->ConsoleDPrintf(XORSTR("Error opening /tmp/speedometer.png\n"));
        return;
    }
    fwrite(speedometer_png, sizeof(char), 68895, speedo);
    fclose(speedo);

    FILE *gas = fopen(XORSTR("/tmp/gas.png"), "wb");
    if( gas == NULL )
    {
        cvar->ConsoleDPrintf(XORSTR("Error opening /tmp/gas.png\n"));
        return;
    }
    fwrite(gas_png, sizeof(char), 15621, gas);
    fclose(gas);

    FILE *wheel = fopen(XORSTR("/tmp/wheel.png"), "wb");
    if( wheel == NULL )
    {
        cvar->ConsoleDPrintf(XORSTR("Error opening /tmp/wheel.png\n"));
        return;
    }
    fwrite(wheel_png, sizeof(char), 321023, wheel);
    fclose(wheel);

    FILE *oil = fopen(XORSTR("/tmp/oil.png"), "wb");
    if( oil == NULL )
    {
        cvar->ConsoleDPrintf(XORSTR("Error opening /tmp/oil.png\n"));
        return;
    }
    fwrite(oil_png, sizeof(char), 15710, oil);
    fclose(oil);
}
