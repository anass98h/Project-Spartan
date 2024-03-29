#pragma once
#include "IMaterial.h"

typedef unsigned short MaterialHandle_t;

class KeyValues;

class ITexture {
public:
    int GetActualWidth( )
    {
        typedef int (* oGetActualWidth)( void* );
        return getvfunc<oGetActualWidth>( this, 3 )( this );
    }
    int GetActualHeight( )
    {
        typedef int (* oGetActualHeight)( void* );
        return getvfunc<oGetActualHeight>( this, 4 )( this );
    }
};

enum RenderTargetSizeMode_t
{
    RT_SIZE_NO_CHANGE=0,			// Only allowed for render targets that don't want a depth buffer
    // (because if they have a depth buffer, the render target must be less than or equal to the size of the framebuffer).
    RT_SIZE_DEFAULT=1,				// Don't play with the specified width and height other than making sure it fits in the framebuffer.
    RT_SIZE_PICMIP=2,				// Apply picmip to the render target's width and height.
    RT_SIZE_HDR=3,					// frame_buffer_width / 4
    RT_SIZE_FULL_FRAME_BUFFER=4,	// Same size as frame buffer, or next lower power of 2 if we can't do that.
    RT_SIZE_OFFSCREEN=5,			// Target of specified size, don't mess with dimensions
    RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP=6, // Same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
    RT_SIZE_REPLAY_SCREENSHOT = 7,	// Rounded down to power of 2, essentially...
    RT_SIZE_LITERAL = 8				// Use the size passed in. Don't clamp it to the frame buffer size. Really.
};
enum CompiledVtfFlags
{
    // flags from the *.txt config file
    TEXTUREFLAGS_POINTSAMPLE	               = 0x00000001,
    TEXTUREFLAGS_TRILINEAR		               = 0x00000002,
    TEXTUREFLAGS_CLAMPS			               = 0x00000004,
    TEXTUREFLAGS_CLAMPT			               = 0x00000008,
    TEXTUREFLAGS_ANISOTROPIC	               = 0x00000010,
    TEXTUREFLAGS_HINT_DXT5		               = 0x00000020,
    TEXTUREFLAGS_SRGB						   = 0x00000040,
    TEXTUREFLAGS_NORMAL			               = 0x00000080,
    TEXTUREFLAGS_NOMIP			               = 0x00000100,
    TEXTUREFLAGS_NOLOD			               = 0x00000200,
    TEXTUREFLAGS_ALL_MIPS			           = 0x00000400,
    TEXTUREFLAGS_PROCEDURAL		               = 0x00000800,

    // These are automatically generated by vtex from the texture data.
    TEXTUREFLAGS_ONEBITALPHA	               = 0x00001000,
    TEXTUREFLAGS_EIGHTBITALPHA	               = 0x00002000,

    // newer flags from the *.txt config file
    TEXTUREFLAGS_ENVMAP			               = 0x00004000,
    TEXTUREFLAGS_RENDERTARGET	               = 0x00008000,
    TEXTUREFLAGS_DEPTHRENDERTARGET	           = 0x00010000,
    TEXTUREFLAGS_NODEBUGOVERRIDE               = 0x00020000,
    TEXTUREFLAGS_SINGLECOPY		               = 0x00040000,

    TEXTUREFLAGS_STAGING_MEMORY                = 0x00080000,
    TEXTUREFLAGS_IMMEDIATE_CLEANUP			   = 0x00100000,
    TEXTUREFLAGS_IGNORE_PICMIP				   = 0x00200000,
    TEXTUREFLAGS_UNUSED_00400000           = 0x00400000,

    TEXTUREFLAGS_NODEPTHBUFFER                 = 0x00800000,

    TEXTUREFLAGS_UNUSED_01000000           = 0x01000000,

    TEXTUREFLAGS_CLAMPU                        = 0x02000000,

    TEXTUREFLAGS_VERTEXTEXTURE                 = 0x04000000,					// Useable as a vertex texture

    TEXTUREFLAGS_SSBUMP                        = 0x08000000,

    TEXTUREFLAGS_UNUSED_10000000           = 0x10000000,

    // Clamp to border color on all texture coordinates
    TEXTUREFLAGS_BORDER						   = 0x20000000,

    TEXTUREFLAGS_UNUSED_40000000		   = 0x40000000,
    TEXTUREFLAGS_UNUSED_80000000		   = 0x80000000,
};

enum MaterialRenderTargetDepth_t
{
    MATERIAL_RT_DEPTH_SHARED   = 0x0,
    MATERIAL_RT_DEPTH_SEPARATE = 0x1,
    MATERIAL_RT_DEPTH_NONE     = 0x2,
    MATERIAL_RT_DEPTH_ONLY	   = 0x3,
};

enum MaterialFogMode_t
{
    MATERIAL_FOG_NONE,
    MATERIAL_FOG_LINEAR,
    MATERIAL_FOG_LINEAR_BELOW_FOG_Z,
};

#define CREATERENDERTARGETFLAGS_HDR				0x00000001
#define CREATERENDERTARGETFLAGS_AUTOMIPMAP		0x00000002
#define CREATERENDERTARGETFLAGS_UNFILTERABLE_OK 0x00000004
// XBOX ONLY:
#define CREATERENDERTARGETFLAGS_NOEDRAM  0x00000008 // inhibit allocation in 360 EDRAM
#define CREATERENDERTARGETFLAGS_TEMP	0x00000010 // only allocates memory upon first resolve, destroyed at level end


class IMatRenderContext {
public:
    void *GetThisPtr( ){
        return this;
    }
    int Release( ) {
        typedef int (* oRelease)( void* );
        return getvfunc<oRelease>( this, 1 )( this );
    }
    void SetRenderTarget( ITexture *pTexture ) {
        typedef void (* oSetRenderTarget)( void*, ITexture* );
        return getvfunc<oSetRenderTarget>( this, 6 )( this, pTexture );
    }
    // 14? - SetAmbientLight()

    void FogMode( MaterialFogMode_t fogMode ){
        typedef void (* oFogMode)( void*, MaterialHandle_t );
        return getvfunc<oFogMode>( this, 49 )( this, fogMode );
    }
    void FogColor( float r, float g, float b ){
        typedef void (* oFogColor)( void*, float ,float ,float );
        return getvfunc<oFogColor>( this, 54 )( this, r, g, b );
    }
    void GetFogColor( unsigned char *rgb ){
        typedef void (* oGetFogColor)( void*, unsigned char * );
        return getvfunc<oGetFogColor>( this, 58 )( this, rgb );
    }

    void PushRenderTargetAndViewport( ) {
        typedef void (* oPushRenderTargetAndViewport)( void* );
        return getvfunc<oPushRenderTargetAndViewport>( this, 115 )( this );
    }
    void PopRenderTargetAndViewport( ) {
        typedef void (* oPopRenderTargetAndViewport)( void* );
        return getvfunc<oPopRenderTargetAndViewport>( this, 119 )( this );
    }

    void DrawScreenSpaceRectangle( IMaterial* pMaterial,
                                   int destX, int destY, int width, int height,
                                   float srcTextureX0, float srcTextureY0, float srcTextureX1, float srcTextureY1,
                                   int srcTextureWidth, int srcTextureHeight,
                                   void* pClientRenderable, int nXDice, int nYDice ) {
        typedef void (* oDrawScreenSpaceRectangle)( void*, IMaterial* ,
                                                    int , int , int , int ,
                                                    float , float , float , float ,
                                                    int , int ,
                                                    void* , int , int  );
        return getvfunc<oDrawScreenSpaceRectangle>( this, 113 )( this, pMaterial, destX, destY, width, height,
                                                                 srcTextureX0, srcTextureY0, srcTextureX1, srcTextureY1,
                                                                 srcTextureWidth, srcTextureHeight, pClientRenderable,
                                                                 nXDice, nYDice );
    }
};

class IMaterialSystem : public IAppSystem {
public:
    ImageFormat  GetBackBufferFormat( ) {
        typedef ImageFormat(* oGetBackBufferFormat)( void* );
        return getvfunc<oGetBackBufferFormat>( this, 36 )( this ); // 33
    }
    IMaterial* CreateMaterial( const char* pMaterialName, KeyValues* pVMTKeyValues ) {
        typedef IMaterial* (* oCreateMaterial)( void*, const char*, KeyValues* );
        return getvfunc<oCreateMaterial>( this, 83 )( this, pMaterialName, pVMTKeyValues );
    }

    IMaterial* FindMaterial( char const* pMaterialName, const char* pTextureGroupName, bool complain = true,
                             const char* pComplainPrefix = NULL ) {
        typedef IMaterial* (* oFindMaterial)( void*, char const*, const char*, bool, const char* );
        return getvfunc<oFindMaterial>( this, 84 )( this, pMaterialName, pTextureGroupName, complain, pComplainPrefix );
    }

    MaterialHandle_t FirstMaterial() {
        typedef MaterialHandle_t (* oFirstMaterial)( void* );
        return getvfunc<oFirstMaterial>( this, 86 )( this );
    }

    MaterialHandle_t NextMaterial( MaterialHandle_t h ) {
        typedef MaterialHandle_t (* oNextMaterial)( void*, MaterialHandle_t );
        return getvfunc<oNextMaterial>( this, 87 )( this, h );
    }

    MaterialHandle_t InvalidMaterial() {
        typedef MaterialHandle_t (* oInvalidMaterial)( void* );
        return getvfunc<oInvalidMaterial>( this, 88 )( this );
    }

    IMaterial* GetMaterial( MaterialHandle_t h ) {
        typedef IMaterial* (* oGetMaterial)( void*, MaterialHandle_t );
        return getvfunc<oGetMaterial>( this, 89 )( this, h );
    }
    /* Look for string "Tried BeginRenderTargetAllocation after game startup" */
    void BeginRenderTargetAllocation() {
        typedef void (* oBeginRenderTargetAlloc)( void* );
        return getvfunc<oBeginRenderTargetAlloc>( this, 94 )( this );
    }
    void EndRenderTargetAllocation() {
        typedef void (* oEndRenderTargetAlloc)( void* );
        return getvfunc<oEndRenderTargetAlloc>( this, 95 )( this );
    }
    void forceBeginRenderTargetAllocation() {
        bool oldState = *GetGameStarted();
        *GetGameStarted() = false;         // Fooling the game that loading is not finished yet
        BeginRenderTargetAllocation();
        *GetGameStarted()  = oldState;
    }
    void forceEndRenderTargetAllocation() {
        bool oldState = *GetGameStarted();
        *GetGameStarted() = false;
        EndRenderTargetAllocation();
        *GetGameStarted() = oldState;
    }
    ITexture* CreateNamedRenderTargetTextureEx( const char* name, int w, int h, RenderTargetSizeMode_t sizeMode,
                                                       ImageFormat format, MaterialRenderTargetDepth_t depth,
                                                       unsigned int textureFlags, unsigned int renderTargetFlags )
    {
        typedef ITexture* (* oCreateNamedRenderTargetTextureEx)( void*, const char*, int, int, RenderTargetSizeMode_t,
                                                                 ImageFormat, MaterialRenderTargetDepth_t,
                                                                 unsigned int, unsigned int );
        return getvfunc<oCreateNamedRenderTargetTextureEx>( this,99 )( this, name, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags );
    }
    ITexture* createFullFrameRenderTarget(const char* name, int width, int height) {
        return CreateNamedRenderTargetTextureEx(
                name, width, height, RT_SIZE_DEFAULT, GetBackBufferFormat(),
                MATERIAL_RT_DEPTH_SHARED, 0, CREATERENDERTARGETFLAGS_HDR
        );
    }
    IMatRenderContext* GetRenderContext() {
        typedef IMatRenderContext* (* oGetRenderContext)( void* );
        return getvfunc<oGetRenderContext>( this, 115 )( this );
    }

    bool* GetGameStarted() {
        return ( bool* ) ( ( uintptr_t ) this + 0x32A8);
    }
};