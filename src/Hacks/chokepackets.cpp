#include "chokepackets.h"

bool Settings::ChokePackets::enabled = false;
ButtonCode_t Settings::ChokePackets::key = ButtonCode_t::KEY_X;

static int chokedPackets = 0;

void ChokePackets::CreateMove( CUserCmd* cmd ) {
    if ( Settings::ChokePackets::enabled && inputSystem->IsButtonDown( Settings::ChokePackets::key ) ) {
        if ( chokedPackets > 1000 ) {
            CreateMove::sendPacket = true;
            chokedPackets = 0;
        } else {
            CreateMove::sendPacket = false;
            chokedPackets++;
        }
    }
}
