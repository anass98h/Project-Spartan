#include "fakewalk.h"

bool Settings::Fakewalk::enabled;
ButtonCode_t Settings::Fakewalk::key = ButtonCode_t::KEY_Y;

static int choked = -1;

void Fakewalk::CreateMove( CUserCmd* cmd ) {
    if ( Settings::Fakewalk::enabled && inputSystem->IsButtonDown( Settings::Fakewalk::key ) ) {
        C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
        if ( !pLocal || !pLocal->GetAlive() )
            return;

        if ( !( pLocal->GetFlags() & FL_ONGROUND ) ) {
            return;
        }

        if ( cmd->command_number % 7 == 0 ) {
            return;
        }

        cmd->buttons &= ~IN_SPEED;

        choked++;

        CreateMove::sendPacket = choked >= 1;

        cmd->tick_count += 10;
        cmd->command_number += 7 + cmd->tick_count % 2 ? 0 : 1;

        cmd->buttons |= pLocal->GetMoveType() == IN_BACK;
        cmd->forwardmove = cmd->sidemove = 0.0f;
    } else {
        choked = -1;
    }
}
