#include "thirdperson.h"

bool Settings::ThirdPerson::enabled = false;
ThirdPersonMode Settings::ThirdPerson::mode = ThirdPersonMode::FAKE;
float Settings::ThirdPerson::distance = 30.0f;
ButtonCode_t Settings::ThirdPerson::key = ButtonCode_t::KEY_DELETE;
float Settings::ThirdPerson::transparency = 0.3f;

IMaterial* transparentPlayerMaterial = nullptr;
long millisSinceLastPress = 0;

// Credit to LWSS for making the initial non-bytepatching TP in Fuzion
// I just fixed it so it can be disabled and that it shows fake angles
void ThirdPerson::OverrideView( CViewSetup* pSetup ) {
    long millis = Util::GetEpochTime();

    if ( millis - millisSinceLastPress >= 1000 ) {
        if ( inputSystem->IsButtonDown( Settings::ThirdPerson::key ) && !UI::isVisible ) {
            Settings::ThirdPerson::enabled = !Settings::ThirdPerson::enabled;
            millisSinceLastPress = Util::GetEpochTime();
        }
    }

    if ( !Settings::ThirdPerson::enabled ) {
        input->m_fCameraInThirdPerson = false;
        input->m_vecCameraOffset.z = 150.0f;
        return;
    }

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() )
        return;

    Vector desiredOffset = Vector( cos( DEG2RAD( pSetup->y ) ) * Settings::ThirdPerson::distance,
                                   sin( DEG2RAD( pSetup->y ) ) * Settings::ThirdPerson::distance,
                                   sin( DEG2RAD( -pSetup->x ) ) * Settings::ThirdPerson::distance );

    Ray_t ray;
    ray.Init( pLocal->GetEyePosition(), ( pLocal->GetEyePosition() - desiredOffset ) );

    CTraceFilter filter;
    filter.pSkip = pLocal;

    trace_t tr;
    trace->TraceRay( ray, MASK_SHOT, &filter, &tr );

    Vector difference = pLocal->GetEyePosition() - tr.endpos;
    float distance2D = sqrt( abs( difference.x * difference.x ) + abs( difference.y * difference.y ) ); // Pythagorean
    bool horizontalOk = distance2D > ( Settings::ThirdPerson::distance - 2.0f );
    bool verticalOk = ( abs( difference.z ) - abs( desiredOffset.z ) < 3.0f );

    float cameraDistance;

    if ( horizontalOk && verticalOk ) {
        cameraDistance = Settings::ThirdPerson::distance;
    } else if ( verticalOk ) {
        cameraDistance = distance2D * 0.95f;
    } else {
        cameraDistance = abs( difference.z ) * 0.95f;
    }

    QAngle origin; // Origin angles which we're seeing
    engine->GetViewAngles( origin );

    input->m_fCameraInThirdPerson = true;
    input->m_vecCameraOffset.x = origin.x;
    input->m_vecCameraOffset.y = origin.y;
    input->m_vecCameraOffset.z = cameraDistance;
}

void ThirdPerson::FrameStageNotify( ClientFrameStage_t stage ) {
    if ( !engine->IsInGame() )
        return;

    if ( stage != ClientFrameStage_t::FRAME_RENDER_START )
        return;

    if ( !Settings::ThirdPerson::enabled )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal || !pLocal->GetAlive() )
        return;

    QAngle view;
    engine->GetViewAngles( view );

    switch ( Settings::ThirdPerson::mode ) {
        case ThirdPersonMode::FAKE: {
            view = QAngle( pLocal->GetEyeAngles()->x, AntiAim::lastFakeYaw, 0.0f );
            break;
        }
        case ThirdPersonMode::REAL: {
            if ( ( AntiAim::IsAirborne() && Settings::AntiAim::Airborne::Yaw::enabled ) ||
                 ( AntiAim::IsMoving() && Settings::AntiAim::Moving::Yaw::enabled ) ||
                 ( AntiAim::IsStanding() && Settings::AntiAim::Standing::Yaw::enabled ) ) {
                if ( AntiAim::fakeTp ) {
                    view = QAngle( pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f );
                } else {
                    view = QAngle( pLocal->GetEyeAngles()->x, AntiAim::lastRealYaw, 0.0f );
                }
            } else {
                view = QAngle( pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f );
            }
            break;
        }
        case ThirdPersonMode::LBY: {
            float* lby = pLocal->GetLowerBodyYawTarget();

            view = QAngle( pLocal->GetEyeAngles()->x, *( float* ) &lby, 0.0f );
            break;
        }
        case ThirdPersonMode::GHOST: {
            // Ghost ThirdPerson is work in progress.
            break;
        }
        case ThirdPersonMode::NUMBER_OF_TYPES: {
            // Unused, Ghost will be drawn in DrawModelExecute
            break;
        }
    }

    *pLocal->GetVAngles() = view;
}

void ThirdPerson::DrawModelExecute( void* thisptr, void* context, void* state, const ModelRenderInfo_t& pInfo,
                                    matrix3x4_t* pCustomBoneToWorld ) {
    if ( !engine->IsInGame() )
        return;

    if ( !Settings::ThirdPerson::enabled )
        return;

    if ( !pInfo.pModel )
        return;

    C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !pLocal )
        return;

    C_BasePlayer* entity = ( C_BasePlayer* ) entityList->GetClientEntity( pInfo.entity_index );
    if ( !entity || entity->GetDormant() || !entity->GetAlive() )
        return;

    std::string modelName = modelInfo->GetModelName( pInfo.pModel );
    if ( ( !Settings::ESP::Chams::enabled || !Settings::ESP::Filters::localplayer ) && pLocal->IsScoped() ) {
        if ( modelName.find( XORSTR( "models/player" ) ) != std::string::npos ) {
            if ( transparentPlayerMaterial == nullptr ) {
                IMaterial* studioSkins[32];
                modelInfo->GetModelMaterials( pLocal->GetModel(),
                                              modelInfo->GetStudioModel( pLocal->GetModel() )->numtextures,
                                              studioSkins );
                transparentPlayerMaterial = *studioSkins;
            }

            transparentPlayerMaterial->AlphaModulate( Settings::ThirdPerson::transparency );


            modelRender->ForcedMaterialOverride( transparentPlayerMaterial );
            modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>( 21 )( thisptr, context, state, pInfo,
                                                                         pCustomBoneToWorld );
        }
    } else {
        if ( transparentPlayerMaterial != nullptr ) {
            transparentPlayerMaterial->AlphaModulate( 1.0f );
        }
    }
}

/*void ThirdPerson::BeginFrame() {
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
            angles =  QAngle(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);

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
                angles =  QAngle(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);

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
        if(AntiAim::isAntiAiming)
        *pLocal->GetVAngles() = angles;
        else
            *pLocal->GetVAngles() = CreateMove::lastTickViewAngles;
    }
}*/

