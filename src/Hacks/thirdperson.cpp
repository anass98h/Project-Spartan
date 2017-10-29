#include "thirdperson.h"

bool Settings::ThirdPerson::enabled = false;
ThirdPersonMode Settings::ThirdPerson::mode = ThirdPersonMode::FAKE;
float Settings::ThirdPerson::distance = 30.f;
ButtonCode_t Settings::ThirdPerson::key = ButtonCode_t::KEY_DELETE;
long millisSinceLastPress = 0;

IMaterial* seeThoughtChams = NULL;

// Predeclare
static void DrawScopedPlayer( void* thisptr, void* context, void* state, const ModelRenderInfo_t& pInfo,
                              matrix3x4_t* pCustomBoneToWorld );

void ThirdPerson::BeginFrame() {
    if ( !engine->IsInGame() )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() )
        return;

    long millis = Util::GetEpochTime();

    if ( millis - millisSinceLastPress >= 1000 ) {
        if ( inputSystem->IsButtonDown( Settings::ThirdPerson::key ) ) {
            Settings::ThirdPerson::enabled = !Settings::ThirdPerson::enabled;
            millisSinceLastPress = Util::GetEpochTime();
        }
    }

    if ( Settings::ThirdPerson::enabled ) {
        for ( ptrdiff_t off = 0; off < 0x2; off++ )
            *( CamThinkSvCheatsCheck + off ) = 0x90;
    } else {
        *CamThinkSvCheatsCheck = 0x74;
        *( CamThinkSvCheatsCheck + 0x1 ) = 0x64;
    }
}

void ThirdPerson::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !engine->IsInGame() )
        return;

    if ( stage != ClientFrameStage_t::FRAME_RENDER_START )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal )
        return;

    input->m_fCameraInThirdPerson = Settings::ThirdPerson::enabled && pLocal->GetAlive();
    input->m_vecCameraOffset.z = Settings::ThirdPerson::enabled ? Settings::ThirdPerson::distance : 150.0f;

    QAngle angles = QAngle(0, 0, 0);

    switch(Settings::ThirdPerson::mode) {
        case ThirdPersonMode::FAKE: {
            angles = QAngle(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);
            break;
        }
        case ThirdPersonMode::REAL: {
            if ( ( AntiAim::IsAirborne() && Settings::AntiAim::Airborne::Yaw::enabled ) ||
                 ( AntiAim::IsMoving() && Settings::AntiAim::Moving::Yaw::enabled ) ||
                 ( AntiAim::IsStanding() && Settings::AntiAim::Standing::Yaw::enabled ) ) {
                if(AntiAim::fakeTp) {
                    angles = QAngle(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);
                } else {
                    angles = QAngle(pLocal->GetEyeAngles()->x, AntiAim::lastRealYaw, 0.0f);
                }
            } else {
                angles = QAngle(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);
            }
            break;
        }
        case ThirdPersonMode::LBY: {
            float* lby = pLocal->GetLowerBodyYawTarget();

            angles = QAngle(pLocal->GetEyeAngles()->x, *(float*) &lby, 0.0f);
            break;
        }
        case ThirdPersonMode::GHOST: {
            // TODO: #250 (https://www.unknowncheats.me/forum/counterstrike-global-offensive/238768-fake-angle-src-friendly.html)

            IMaterial* material = Chams::chamsMaterial;
            if(material) {
                Vector originAngle;
                Math::AngleVectors(*pLocal->GetEyeAngles(), originAngle);
                //pLocal->SetAngle2(Vector(0, AntiAim::lastFakeYaw, 0)); - Need sig
                material->ColorModulate(Color(255, 255, 255));
                material->AlphaModulate(255);
                modelRender->ForcedMaterialOverride(material);
                //pLocal->DrawModel(0x1, 255); - Need sig aswell
                modelRender->ForcedMaterialOverride(nullptr);
                //pLocal->SetAngle2(originAngle); - Need sig again
            }

            if ( ( AntiAim::IsAirborne() && Settings::AntiAim::Airborne::Yaw::enabled ) ||
                 ( AntiAim::IsMoving() && Settings::AntiAim::Moving::Yaw::enabled ) ||
                 ( AntiAim::IsStanding() && Settings::AntiAim::Standing::Yaw::enabled ) ) {
                if(AntiAim::fakeTp) {
                    angles = QAngle(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);
                } else {
                    angles = QAngle(pLocal->GetEyeAngles()->x, AntiAim::lastRealYaw, 0.0f);
                }
            } else {
                angles = QAngle(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);
            }

            break;
        }
        case ThirdPersonMode::NUMBER_OF_TYPES: {
            break;
        }
    }

    if ( Settings::ThirdPerson::enabled ) {
        *pLocal->GetVAngles() = angles;
    }
}

void ThirdPerson::DrawModelExecute( void* thisptr, void* context, void* state, const ModelRenderInfo_t& pInfo,
                                    matrix3x4_t* pCustomBoneToWorld ) {
    if ( !engine->IsInGame() )
        return;

    if ( !Settings::ThirdPerson::enabled )
        return;

    if ( !pInfo.pModel )
        return;

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer )
        return;

    C_BasePlayer* entity = ( C_BasePlayer* ) entityList->GetClientEntity( pInfo.entity_index );
    if ( !entity
         || entity->GetDormant()
         || !entity->GetAlive() )
        return;

    std::string modelName = modelInfo->GetModelName( pInfo.pModel );

    if ( modelName.find( XORSTR( "models/player" ) ) != std::string::npos ) {
        DrawScopedPlayer( thisptr, context, state, pInfo, pCustomBoneToWorld );
    }

}

// Make the player a bit see thought to make thirdperson while scoped less cancerous
static void DrawScopedPlayer( void* thisptr, void* context, void* state, const ModelRenderInfo_t& pInfo,
                              matrix3x4_t* pCustomBoneToWorld ) {
    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer || !localplayer->GetAlive() )
        return;

    C_BasePlayer* entity = ( C_BasePlayer* ) entityList->GetClientEntity( pInfo.entity_index );
    if ( !entity || entity->GetDormant() || !entity->GetAlive() )
        return;

    if ( localplayer == entity && localplayer->IsScoped() ) {
        if ( seeThoughtChams == NULL ) {
            seeThoughtChams = Util::CreateMaterial( XORSTR( "VertexLitGeneric" ), XORSTR( "VGUI/white_additive" ),
                                                    false,
                                                    true, true, true, true );
        }

        seeThoughtChams->ColorModulate( Color::FromImColor( Settings::ESP::Chams::localplayerColor.Color( entity ) ) );
        seeThoughtChams->AlphaModulate( 0.5f );

        modelRender->ForcedMaterialOverride( seeThoughtChams );
        modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>( 21 )( thisptr, context, state, pInfo,
                                                                     pCustomBoneToWorld );
    }
}
