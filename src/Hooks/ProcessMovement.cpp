#include "hooks.h"


void Hooks::ProcessMovement( void* thisptr, C_BasePlayer *player, CMoveData *moveData ){
    gameMovementVMT->GetOriginalMethod<ProcessMovementFn>(2)( thisptr, player, moveData );

    // I was playing with some stuff here. Not fully explored if you want to fuzz.
    // NOTE: Might get called by us later by our engine prediction.
}