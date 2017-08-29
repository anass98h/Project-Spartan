#include "grenadeprediction.h"

bool Settings::GrenadePrediction::enabled = false;

void GrenadePrediction::Paint()
{
    if(!Settings::GrenadePrediction::enabled || !engine->IsInGame())
        return;

    C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if(!pLocal)
        return;

    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(pLocal->GetActiveWeapon());
    if (!activeWeapon || activeWeapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_GRENADE)
        return;

    GrenadePrediction::DrawPrediction(pLocal, activeWeapon);
}

void GrenadePrediction::DrawPrediction(C_BasePlayer* pLocal, C_BaseCombatWeapon* grenade)
{
    // Give paste pl0x i'm bad codenz
}
