#include "grenadeprediction.h"

#define TIMEALIVE 5.0f
#define GRENADE_COEFFICIENT_OF_RESTITUTION 0.4f

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

    float fStep = 0.1f;
    float fGravity = 800.0f / 8.f;

    Vector vPos;
    Vector vStart;
    Vector vThrow;
    Vector vThrow2;
    int iCollisions = 0;

    QAngle vViewAngles;
    engine->GetViewAngles(vViewAngles);

    vThrow[0] = vViewAngles[0];
    vThrow[1] = vViewAngles[1];
    vThrow[2] = vViewAngles[2];

    if (vThrow[0] < 0)
        vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
    else
        vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

    float fVel = (90 - vThrow[0]) * 4;
    if (fVel > 500)
        fVel = 500;

    Math::AngleVectors(vThrow, &vThrow2);

    Vector vEye = pLocal->GetEyePosition();
    vStart[0] = vEye[0] + vThrow2[0] * 16;
    vStart[1] = vEye[1] + vThrow2[1] * 16;
    vStart[2] = vEye[2] + vThrow2[2] * 16;

    vThrow2[0] = (vThrow2[0] * fVel) + pLocal->GetVelocity()[0];
    vThrow2[1] = (vThrow2[1] * fVel) + pLocal->GetVelocity()[1];
    vThrow2[2] = (vThrow2[2] * fVel) + pLocal->GetVelocity()[2]; // casualhacker for da magic calc help

    for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
    {
        vPos = vStart + vThrow2 * fStep;

        Ray_t ray;
        trace_t tr;

        CTraceFilterWorldOnly worldFilter;

        ray.Init(vStart, vPos);
        trace->TraceRay(ray, MASK_SOLID, &worldFilter, &tr);

        if (tr.m_pEntityHit) // if the trace hits a surface
        {
            //float proj = DotProduct(throwvec, tr.plane.normal);
            vThrow2 = tr.plane.normal * -2.0f * Math::DotProduct(vThrow2, tr.plane.normal) + vThrow2;
            vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

            iCollisions++;
            if (iCollisions > 2)
                break;

            vPos = vStart + vThrow2*tr.fraction*fStep;

            fTime += (fStep * (1 - tr.fraction));
        } // end of bounce

        int w, h;
        engine->GetScreenSize(w, h);

        // TODO: Fix this
        Draw::Line(Vector2D(0, 0), Vector2D(vPos.x, vPos.y), Color::FromImColor(Settings::GrenadeHelper::aimLine.Color()));

        cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XORSTR("w / 2: %f - h / 2: %f - start x: %f - start y: %f - vPos x: %f - vPos y: %f\n"),
            w / 2, h / 2, vStart.x, vStart.y, vPos.x, vPos.y);

        vStart = vPos;
        vThrow2.z -= fGravity * tr.fraction * fStep;
    }
}
