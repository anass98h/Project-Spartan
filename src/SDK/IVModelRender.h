#pragma once

typedef unsigned short ModelInstanceHandle_t;

struct model_t {
    char name[255];
};

struct ModelRenderInfo_t {
    Vector origin;
    QAngle angles;
    void* pRenderable;
    const model_t* pModel;
    const matrix3x4_t* pModelToWorld;
    const matrix3x4_t* pLightingOffset;
    const Vector* pLightingOrigin;
    int flags;
    int entity_index;
    int skin;
    int body;
    int hitboxset;
    ModelInstanceHandle_t instance;

    ModelRenderInfo_t() {
        pModelToWorld = NULL;
        pLightingOffset = NULL;
        pLightingOrigin = NULL;
    }
};

enum DrawModelFlags_t {
    STUDIO_NONE = 0x00000000,
    STUDIO_RENDER = 0x00000001,
    STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
    STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
    STUDIO_TWOPASS = 0x00000008,
    STUDIO_STATIC_LIGHTING = 0x00000010,
    STUDIO_WIREFRAME = 0x00000020,
    STUDIO_ITEM_BLINK = 0x00000040,
    STUDIO_NOSHADOWS = 0x00000080,
    STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
    STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
    STUDIO_SKIP_FLEXES = 0x00000400,
    STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
    // Not a studio flag, but used to flag model as a non-sorting brush model
            STUDIO_TRANSPARENCY = 0x80000000,
    // Not a studio flag, but used to flag model as using shadow depth material override
            STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
    // Not a studio flag, but used to flag model as doing custom rendering into shadow texture
            STUDIO_SHADOWTEXTURE = 0x20000000,
    STUDIO_SKIP_DECALS = 0x10000000,
};

class IVModelRender {
public:
    void ForcedMaterialOverride( IMaterial* mat ) {
        typedef void (* oForcedMaterialOverride)( void*, IMaterial*, int, int );
        return getvfunc<oForcedMaterialOverride>( this, 1 )( this, mat, 0, 0 );
    }

    void
    DrawModelExecute( void* ctx, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL ) {
        typedef void (* oDrawModelExecute)( void*, void* ctx, void* state, const ModelRenderInfo_t& pInfo,
                                            matrix3x4_t* pCustomBoneToWorld );
        return getvfunc<oDrawModelExecute>( this, 21 )( this, ctx, state, pInfo, pCustomBoneToWorld );
    }
};
