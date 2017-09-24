#include "nosmoke.h"

bool Settings::NoSmoke::enabled = false;

std::vector<const char*> smoke_materials = {
        XORSTR( "particle/vistasmokev1/vistasmokev1_fire" ),
        XORSTR( "particle/vistasmokev1/vistasmokev1_smokegrenade" ),
        XORSTR( "particle/vistasmokev1/vistasmokev1_emods" ),
        XORSTR( "particle/vistasmokev1/vistasmokev1_emods_impactdust" ),
};


void NoSmoke::Cleanup() {
    for ( auto material_name : smoke_materials ) {
        IMaterial* mat = material->FindMaterial( material_name, TEXTURE_GROUP_OTHER );
        mat->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, false );
    }
}


bool NoSmoke::RenderSmokePostViewmodel() {
    return Settings::ESP::enabled && Settings::NoSmoke::enabled;
}

void NoSmoke::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !engine->IsInGame() )
        return;

    if ( stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END )
        return;

    for ( auto material_name : smoke_materials ) {
        IMaterial* mat = material->FindMaterial( material_name, TEXTURE_GROUP_OTHER );
        mat->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, Settings::ESP::enabled && Settings::NoSmoke::enabled );
    }
}
