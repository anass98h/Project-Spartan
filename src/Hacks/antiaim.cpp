#include "antiaim.h"

#ifndef REGION_ANTIAIM_STANDING_SETTINGS
bool Settings::AntiAim::Standing::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Standing::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Standing::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Standing::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Standing::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Standing::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Standing::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Standing::Pitch::custom = 0.0f;

bool Settings::AntiAim::Standing::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Standing::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Standing::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::Standing::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Standing::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Standing::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Standing::HeadEdge::enabled = false;
float Settings::AntiAim::Standing::HeadEdge::distance = 25.0f;

bool Settings::AntiAim::Standing::antiResolver = false;
bool Settings::AntiAim::Standing::dynamicAA = false;
bool Settings::AntiAim::Standing::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_MOVING_SETTINGS
bool Settings::AntiAim::Moving::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Moving::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Moving::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Moving::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Moving::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Moving::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Moving::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Moving::Pitch::custom = 0.0f;

bool Settings::AntiAim::Moving::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Moving::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Moving::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::Moving::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Moving::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Moving::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Moving::HeadEdge::enabled = false;
float Settings::AntiAim::Moving::HeadEdge::distance = 25.0f;

bool Settings::AntiAim::Moving::antiResolver = false;
bool Settings::AntiAim::Moving::dynamicAA = false;
bool Settings::AntiAim::Moving::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_AIRBORNE_SETTINGS
bool Settings::AntiAim::Airborne::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Airborne::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Airborne::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Airborne::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Airborne::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Airborne::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Airborne::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Airborne::Pitch::custom = 0.0f;

bool Settings::AntiAim::Airborne::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Airborne::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Airborne::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::Airborne::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Airborne::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Airborne::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Airborne::HeadEdge::enabled = false;
float Settings::AntiAim::Airborne::HeadEdge::distance = 25.0f;

bool Settings::AntiAim::Airborne::antiResolver = false;
bool Settings::AntiAim::Airborne::dynamicAA = false;
bool Settings::AntiAim::Airborne::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_MISC_SETTINGS
bool Settings::AntiAim::Misc::AutoDisable::noEnemy = false;
bool Settings::AntiAim::Misc::AutoDisable::knifeHeld = true;
bool Settings::AntiAim::Misc::AutoDisable::bombHeld = false;
bool Settings::AntiAim::Misc::AutoDisable::freezeTime = true;
#endif
float AntiAim::lastRealYaw = 0.0f;
float AntiAim::lastFakeYaw = 0.0f;
bool AntiAim::isAntiAiming = false;

static float Distance(Vector a, Vector b) {
    return (sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)));
}

static bool LBYUpdated() {
    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    float bodyEyeDelta = AntiAim::lastRealYaw - *localplayer->GetLowerBodyYawTarget();

    static bool LBYUpdated = false;

    bool moving = (localplayer->GetVelocity().x != 0);
    bool onGround = (localplayer->GetFlags() & FL_ONGROUND);

    if (AntiAim::lastRealYaw == *localplayer->GetLowerBodyYawTarget() || moving && onGround ||
        fabsf(bodyEyeDelta) < 35.f)
        LBYUpdated = true;
    else
        LBYUpdated = false;

    return LBYUpdated;
}

static bool GetBestHeadAngle(QAngle &angle) {
    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    Vector position = localplayer->GetVecOrigin() + localplayer->GetVecViewOffset();

    float closest_distance = 100.0f;

    float radius = Settings::AntiAim::Standing::HeadEdge::distance + 0.1f;
    if (AntiAim::IsMoving()) {
        radius = Settings::AntiAim::Moving::HeadEdge::distance + 0.1f;
    }
    if (AntiAim::IsAirborne()) {
        radius = Settings::AntiAim::Airborne::HeadEdge::distance = 0.1;
    }

    float step = M_PI * 2.0 / 8;

    for (float a = 0; a < (M_PI * 2.0); a += step) {
        Vector location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

        Ray_t ray;
        trace_t tr;
        ray.Init(position, location);
        CTraceFilter traceFilter;
        traceFilter.pSkip = localplayer;
        trace->TraceRay(ray, 0x4600400B, &traceFilter, &tr);

        float distance = Distance(position, tr.endpos);

        if (distance < closest_distance) {
            closest_distance = distance;
            angle.y = RAD2DEG(a);
        }
    }

    return closest_distance < (radius - 0.1f);
}

static bool HasViableEnemy() {
    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    for (int i = 1; i < engine->GetMaxClients(); ++i) {
        C_BasePlayer *entity = (C_BasePlayer *) entityList->GetClientEntity(i);

        if (!entity
            || entity == localplayer
            || entity->GetDormant()
            || !entity->GetAlive()
            || entity->GetImmune())
            continue;

        if (!Aimbot::friends.empty()) // check for friends, if any
        {
            IEngineClient::player_info_t entityInformation;
            engine->GetPlayerInfo(i, &entityInformation);

            if (std::find(Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid) !=
                Aimbot::friends.end())
                continue;
        }

        if (Settings::Aimbot::friendly || entity->GetTeam() != localplayer->GetTeam())
            return true;
    }

    return false;
}

static float DoAAatTarget() {
    static C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    static float Angle = 0.0f;

    if ((AntiAim::IsStanding() && Settings::AntiAim::Standing::Yaw::enabled) ||
        (AntiAim::IsMoving() && Settings::AntiAim::Moving::Yaw::enabled) ||
        (AntiAim::IsAirborne() && Settings::AntiAim::Airborne::dynamicAA)) {
        if ((AntiAim::IsStanding() && Settings::AntiAim::Standing::dynamicAA) ||
            (AntiAim::IsMoving() && Settings::AntiAim::Moving::dynamicAA) ||
            (AntiAim::IsAirborne() && Settings::AntiAim::Airborne::dynamicAA)) {
            float bestDist = 999999999.f; // easy cuz im retarded
            for (int i = 1; i < engine->GetMaxClients(); ++i) {
                C_BasePlayer *target = (C_BasePlayer *) entityList->GetClientEntity(i);

                if (!target
                    || target == pLocal
                    || target->GetDormant()
                    || !target->GetAlive()
                    || target->GetImmune()
                    || target->GetTeam() == pLocal->GetTeam())
                    continue;

                Vector eye_pos = pLocal->GetEyePosition();
                Vector target_pos = target->GetEyePosition();

                float tempDist = eye_pos.DistTo(target_pos);

                if (bestDist > tempDist) {
                    bestDist = tempDist;
                    Angle = Math::CalcAngle(eye_pos, target_pos).y;
                    return Angle;
                }
            }
        }

    }
}

static void SwapAA() {
    int count = 0;

    if (inputSystem->IsButtonDown(AntiAim::IsAirborne() ? Settings::AntiAim::Airborne::SwitchAA::key :
                                  AntiAim::IsMoving() ? Settings::AntiAim::Moving::SwitchAA::key :
                                  Settings::AntiAim::Standing::SwitchAA::key)) {
        count++;
        //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Switch AA Count: %i\n", count);
    } else {
        if (count > 0) {
            if (AntiAim::IsStanding()) {
                AntiAimType_Y fake = Settings::AntiAim::Standing::Yaw::typeFake;
                AntiAimType_Y real = Settings::AntiAim::Standing::Yaw::type;
                float fakeAdd = Settings::AntiAim::Standing::Yaw::typeFakeAdd;
                float realAdd = Settings::AntiAim::Standing::Yaw::typeAdd;

                Settings::AntiAim::Standing::Yaw::typeFake = real;
                Settings::AntiAim::Standing::Yaw::type = fake;
                Settings::AntiAim::Standing::Yaw::typeFakeAdd = realAdd;
                Settings::AntiAim::Standing::Yaw::typeAdd = fakeAdd;
            }
            if (AntiAim::IsMoving()) {
                AntiAimType_Y fake = Settings::AntiAim::Moving::Yaw::typeFake;
                AntiAimType_Y real = Settings::AntiAim::Moving::Yaw::type;
                float fakeAdd = Settings::AntiAim::Moving::Yaw::typeFakeAdd;
                float realAdd = Settings::AntiAim::Moving::Yaw::typeAdd;

                Settings::AntiAim::Moving::Yaw::typeFake = real;
                Settings::AntiAim::Moving::Yaw::type = fake;
                Settings::AntiAim::Moving::Yaw::typeFakeAdd = realAdd;
                Settings::AntiAim::Moving::Yaw::typeAdd = fakeAdd;
            }
            if (AntiAim::IsAirborne()) {
                AntiAimType_Y fake = Settings::AntiAim::Airborne::Yaw::typeFake;
                AntiAimType_Y real = Settings::AntiAim::Airborne::Yaw::type;
                float fakeAdd = Settings::AntiAim::Airborne::Yaw::typeFakeAdd;
                float realAdd = Settings::AntiAim::Airborne::Yaw::typeAdd;

                Settings::AntiAim::Airborne::Yaw::typeFake = real;
                Settings::AntiAim::Airborne::Yaw::type = fake;
                Settings::AntiAim::Airborne::Yaw::typeFakeAdd = realAdd;
                Settings::AntiAim::Airborne::Yaw::typeAdd = fakeAdd;
            }
            count = 0;
        }
    }
}

float GetLatency() {
    INetChannelInfo *nci = engine->GetNetChannelInfo();
    if (nci) {
        float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
        return Latency;
    } else {
        return 0.0f;
    }
}

float GetOutgoingLatency() {
    INetChannelInfo *nci = engine->GetNetChannelInfo();
    if (nci) {
        float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
        return OutgoingLatency;
    } else {
        return 0.0f;
    }
}

float GetIncomingLatency() {
    INetChannelInfo *nci = engine->GetNetChannelInfo();
    if (nci) {
        float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
        return IncomingLatency;
    } else {
        return 0.0f;
    }
}

float OldLBY;
float LBYBreakerTimer;
float LastLBYUpdateTime;
bool bSwitch;

float CurrentVelocity(C_BasePlayer *LocalPlayer) {
    float vel = LocalPlayer->GetVelocity().Length2D();
    return vel;
}

bool NextLBYUpdate() {
    C_BasePlayer *LocalPlayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    float flServerTime = (float) (LocalPlayer->GetTickBase() * globalVars->interval_per_tick);


    if (OldLBY != *LocalPlayer->GetLowerBodyYawTarget()) {
        LBYBreakerTimer++;
        OldLBY = *LocalPlayer->GetLowerBodyYawTarget();
        bSwitch = !bSwitch;
        LastLBYUpdateTime = flServerTime;
    }

    if (CurrentVelocity(LocalPlayer) > 0.5) {
        LastLBYUpdateTime = flServerTime;
        return false;
    }

    if ((LastLBYUpdateTime + 1 - (GetLatency() * 2) < flServerTime) &&
        (LocalPlayer->GetFlags() & FL_ONGROUND)) {
        if (LastLBYUpdateTime + 1.1 - (GetLatency() * 2) < flServerTime) {
            LastLBYUpdateTime += 1.1;
        }
        return true;
    }
    return false;
}


static void DoAntiAimY(QAngle &angle, int command_number, bool bFlip, bool &clamp) {
    AntiAimType_Y antiAim = bFlip ? Settings::AntiAim::Standing::Yaw::typeFake : Settings::AntiAim::Standing::Yaw::type;
    if (AntiAim::IsMoving()) {
        antiAim = bFlip ? Settings::AntiAim::Moving::Yaw::typeFake : Settings::AntiAim::Moving::Yaw::type;
    }
    if (AntiAim::IsAirborne()) {
        antiAim = bFlip ? Settings::AntiAim::Airborne::Yaw::typeFake : Settings::AntiAim::Airborne::Yaw::type;
    }

    static bool yFlip;
    double factor;
    static bool flip1 = false;
    static bool back = false;
    static float prevLBY1 = *((C_BasePlayer *) entityList->GetClientEntity(
            engine->GetLocalPlayer()))->GetLowerBodyYawTarget();
    static bool lolgay;
    int random;
    int ticks = 0;
    int jitterticks = 0;
    static C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());


    yFlip = bFlip != yFlip;

    switch (antiAim) {
        case AntiAimType_Y::LEGITTROLLING:
            if (AntiAim::IsStanding()) {
                Settings::AntiAim::Standing::Yaw::typeFake = AntiAimType_Y::NOAA;
                Settings::AntiAim::Standing::Yaw::typeFakeAdd = 0.0f;
                Settings::AntiAim::Standing::Yaw::typeAdd = 0.0f;
            }
            if (AntiAim::IsMoving()) {
                Settings::AntiAim::Moving::Yaw::typeFake = AntiAimType_Y::NOAA;
                Settings::AntiAim::Moving::Yaw::typeFakeAdd = 0.0f;
                Settings::AntiAim::Moving::Yaw::typeAdd = 0.0f;
            }
            if (AntiAim::IsAirborne()) {
                Settings::AntiAim::Airborne::Yaw::typeFake = AntiAimType_Y::NOAA;
                Settings::AntiAim::Airborne::Yaw::typeFakeAdd = 0.0f;
                Settings::AntiAim::Airborne::Yaw::typeAdd = 0.0f;
            }

            angle.y -= 180.f;
            break;
        case AntiAimType_Y::LEGITTROLLING2:
            if (AntiAim::IsStanding()) {
                Settings::AntiAim::Standing::Yaw::typeFake = AntiAimType_Y::NOAA;
                Settings::AntiAim::Standing::Yaw::typeFakeAdd = 0.0f;
                Settings::AntiAim::Standing::Yaw::typeAdd = 0.0f;
            }
            if (AntiAim::IsMoving()) {
                Settings::AntiAim::Moving::Yaw::typeFake = AntiAimType_Y::NOAA;
                Settings::AntiAim::Moving::Yaw::typeFakeAdd = 0.0f;
                Settings::AntiAim::Moving::Yaw::typeAdd = 0.0f;
            }
            if (AntiAim::IsAirborne()) {
                Settings::AntiAim::Airborne::Yaw::typeFake = AntiAimType_Y::NOAA;
                Settings::AntiAim::Airborne::Yaw::typeFakeAdd = 0.0f;
                Settings::AntiAim::Airborne::Yaw::typeAdd = 0.0f;
            }

            angle.y -= 90.0f;
            break;
        case AntiAimType_Y::SPIN:
            factor = 360.0 / M_PHI;
            factor *= Settings::spinFactor::value;
            angle.y = fmodf(globalVars->curtime * factor, 360.0);
            break;
        case AntiAimType_Y::CUSTOM:
            Math::ClampY(Settings::customYaw::value);
            Math::ClampY(Settings::customYaw::value);

            if (Settings::customYaw::lby) {
                if (Settings::customYaw::value > 0)
                    angle.y += *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + (Settings::customYaw::value);
                else
                    angle.y -= *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - (Settings::customYaw::value);

            } else {
                angle.y += Settings::customYaw::value;
            }
            break;
        case AntiAimType_Y::CUSTOM2:
            Math::ClampY(Settings::customYaw2::value);
            Math::ClampY(Settings::customYaw2::value);

            if (Settings::customYaw2::lby) {
                if (Settings::customYaw2::value > 0)
                    angle.y += *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + (Settings::customYaw2::value);
                else
                    angle.y -= *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - (Settings::customYaw2::value);
            } else {
                angle.y += Settings::customYaw2::value;
            }
            break;

        case AntiAimType_Y::TANK1:
            if (!(pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f))
                yFlip ? angle.y -= 165 : angle.y += 195;
            if (!(pLocal->GetFlags() & FL_ONGROUND))
                yFlip ? angle.y += 90 : angle.y -= 90;

            if (CreateMove::sendPacket) {
                random = rand() % 4;
                switch (random) {
                    case 0:
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 220;
                        break;
                    case 1:
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - 97;
                        break;
                    case 2:
                        yFlip ? angle.y += 97 : angle.y -= 97;
                        break;
                    case 3:
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - rand() + 97;
                        break;
                }
                CreateMove::sendPacket = false;

            } else {
                static int psilent;
                psilent = rand() % 6;
                switch (psilent) {
                    case 0:
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 88;
                        break;
                    case 1:
                        angle.y = bSendPacket ? 118 : 270;
                        break;
                    case 2:
                        angle.y = yFlip ? *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 97 : *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() -
                                                                                  rand() % 97;
                        break;
                    case 3:
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - rand();
                        break;
                    case 4:
                        angle.y -= 97;
                        break;
                    case 5:
                        angle.y = yFlip ? *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 112 :
                                  *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - rand() % 66;
                        break;


                }
                CreateMove::sendPacket = false;

            }
            break;
        case AntiAimType_Y::TANK2:
            if (bSendPacket) {
                random = rand() % 4;
                switch (random) {
                    case 1:
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 35 + 165;
                        break;
                    case 2:
                        yFlip ? angle.y -= 160 : angle.y += 160;
                        break;
                    case 3:
                        factor = 360.0 / M_PHI;
                        factor *= rand() % 25;
                        float x = fmodf(globalVars->curtime * factor, 360.0);
                        factor *= 5;
                        float y = fmodf(globalVars->curtime * factor, 360.0f);
                        if (y >= 100.0f) {
                            y -= 170.0f;
                        }
                        if (y <= 200.0f) {
                            y += 305.00f;
                        }
                        angle.y = ((x / y) + 60.2f) * M_PI;
                }
            } else {
                random = rand() % 4;
                switch (random) {
                    case 1:
                        angle.y -= 180.0f;
                        break;
                    case 2:
                        yFlip ? angle.y += 90.f : angle.y -= 90.0f;
                        break;
                    case 3:
                        factor = 360.0 / M_PHI;
                        angle.y = fmodf(globalVars->curtime * factor, 360.0);

                }
            }
            if (angle.y ==
                *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget()) {
                angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 90;

            }

            break;
        case AntiAimType_Y::TANK3:
            static bool uff = false; // NICE VARIABLE NAMING
            static bool uff2 = false;
            static bool uff3 = false;
            static int uff4 = 0;

            if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {
                if (CreateMove::sendPacket) {
                    if (pLocal->GetFlags() & FL_ONGROUND)
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 90 + 99;
                    else {
                        random = rand() % 4;
                        switch (random) {
                            case 1:
                                yFlip ? angle.y += 90.f : angle.y -= 90.0f;
                                break;
                            case 2:
                                angle.y -= 120.0f;
                                break;
                            case 3:
                                factor = 360.0 / M_PHI;
                                factor *= 25;
                                angle.y = fmodf(globalVars->curtime * factor, 360.0);
                                break;
                            default:
                                angle.y -= 180.0f;
                        }
                    }
                    CreateMove::sendPacket = false;
                } else {
                    {
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 97.0f;

                        if (uff2) {

                            if (uff4 > 0) {

                                uff4 = 1000;
                                angle.y += 1888 + uff4 / rand();
                                uff4 = 88000;
                            } else {
                                angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + uff4;
                                uff4 = rand();
                            }

                            uff2 = false;
                        } else {

                            angle.y = rand() * rand();
                            uff2 = true;

                        }


                        if (uff3) {
                            angle.y = -97.f;
                            uff3 = false;
                        } else {
                            angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 97.f;
                            uff3 = true;


                        }
                    }

                    CreateMove::sendPacket = true;

                }
            } else {

                yFlip ? angle.y -= 170 : angle.y += 170;

            }
            break;
        case AntiAimType_Y::RASP:
            static float realAngle = AntiAim::lastRealYaw;
            //float realAngle = AntiAim::lastRealYaw;
            realAngle = AntiAim::lastRealYaw;
            //realAngle = AntiAim::lastRealYaw;
            if (AntiAim::lastRealYaw == realAngle) {
                angle.y = realAngle + rand() % 110 + 70;
                realAngle = AntiAim::lastRealYaw;
            } else {
                angle.y = realAngle + rand() % 110 + 70;
                realAngle = AntiAim::lastRealYaw;
            }
            break;
        case AntiAimType_Y::LBYBREAK:
            //Placeholder
            break;
        case AntiAimType_Y::FAKELBY:
            //clamp = false;
            if (!(pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f)) {
                angle.y -= 180.f;
            } else {
                if (lolgay) {
                    yFlip ? angle.y -= *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - 97 : angle.y -=
                                                                                       *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() +
                                                                                       97;
                } else {
                    if (prevLBY1 != *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget())
                        flip1 = false;
                    if (prevLBY1 != *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget())
                        flip1 = true;
                    if (flip1)
                        angle.y += 120.f;
                    else
                        angle.y -= 120.f;

                    prevLBY1 = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget();
                }
                break;
                case AntiAimType_Y::LBYSPIN:
                    factor = 360.0 / M_PHI;
                factor *= Settings::spinFactor::value;
                angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() +
                          fmodf(globalVars->curtime * factor, 360.0);
                break;
                case AntiAimType_Y::LBYJITTER:

                    if (pLocal->GetFlags() & FL_ONGROUND)
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 35 + 165;
                    else {
                        random = rand() % 4;
                        switch (random) {
                            case 1:
                                yFlip ? angle.y += 90.f : angle.y -= 90.0f;
                                break;
                            case 2:
                                yFlip ? angle.y -= 120.0f : angle.y -= 210.0f;
                                break;
                            case 3:
                                factor = 360.0 / M_PHI;
                                factor *= 25;
                                angle.y = fmodf(globalVars->curtime * factor, 360.0);
                                break;
                            default:
                                angle.y -= 180.0f;
                        }
                    }
                break;
                case AntiAimType_Y::NOAA:
                    break;
                case AntiAimType_Y::BACKJITTER:
                    static int random = rand() % 16;
                yFlip ? angle.y -= 180.f + random : angle.y += 180.f + random;
                break;
                case AntiAimType_Y::FAKESIDEWAYS:

                    if (CreateMove::sendPacket) {
                        angle.y -= 90.f;
                        CreateMove::sendPacket = false;
                    } else {
                        angle.y = 120.0f;
                        CreateMove::sendPacket = true;
                    }
                break;
                case AntiAimType_Y::BACKWARDS:
                    angle.y -= 180.0f;
                break;

                case AntiAimType_Y::FORWARDS:
                    angle.y -= 0.0f;
                break;
                case AntiAimType_Y::LOWERBODY:
                    angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget();
                break;
                case AntiAimType_Y::FJITTER: {
                    if (CreateMove::sendPacket) {
                        if (jitterticks > 0)
                            jitterticks = -1;
                        jitterticks++;
                    }
                    int add = 0;
                    if (jitterticks == 0)
                        add += 130;
                    if (jitterticks == 1)
                        add -= 145;
                    if (ticks > 0 || !CreateMove::sendPacket) {
                        add = -add;
                        CreateMove::sendPacket = false;
                    }
                    angle.y += add;
                }
                break;
                case AntiAimType_Y::richieap:
                    static bool besteap = false;
                if (fabsf(pLocal->GetVelocity().x) < 0.1) {
                    besteap = !besteap;
                    CreateMove::sendPacket = besteap;
                    angle.y -= besteap ? 180 : *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 90.f;

                } else {
                    besteap = !besteap;
                    CreateMove::sendPacket = besteap;
                    angle.y -= besteap ? 0 : 180;
                }
                break;
                case AntiAimType_Y::MYRRIB:
                    if (CreateMove::sendPacket) {
                        if (fabsf(pLocal->GetVelocity().x) != 0) {
                            angle.y -= 180.f;
                            CreateMove::sendPacket = false;
                        } else {
                            angle.y -= *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 45.f;
                            CreateMove::sendPacket = false;
                        }
                    } else {
                        if (fabsf(pLocal->GetVelocity().x) != 0) {
                            angle.y -= 180; //pMemez here plz fix me
                            CreateMove::sendPacket = true;
                        } else {
                            angle.y -= *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() - 180.f;
                            CreateMove::sendPacket = true;
                        }
                    }
                break;
                case AntiAimType_Y::LBYONGROUND:
                    if (pLocal->GetFlags() & FL_ONGROUND)
                        angle.y = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 35 + 165;
                    else {
                        cvar->ConsoleColorPrintf(ColorRGBA(255,0,0,255), "%f\n",  *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget());
                        static int aimType = rand() % 4;
                        switch (aimType) {
                            case 1:
                                yFlip ? angle.y += 90.f : angle.y -= 90.0f;
                                break;
                            case 2:
                                yFlip ? angle.y -= 120.0f : angle.y -= 210.0f;
                                break;
                            case 3:
                                factor = 360.0 / M_PHI;
                                factor *= 25;
                                angle.y = fmodf(globalVars->curtime * factor, 360.0);
                                break;
                            default:
                                angle.y -= 180.0f;
                        }
                    }
                break;
                case AntiAimType_Y::RASP2:

                    bool MarcisAWeeb;
                float oldyaw = *((C_BasePlayer *) entityList->GetClientEntity(
                        engine->GetLocalPlayer()))->GetLowerBodyYawTarget();
                cvar->ConsoleColorPrintf(ColorRGBA(255,0,0,255), "%f\n", oldyaw);
                if (MarcisAWeeb) {
                    angle.y -= 180.0f;

                    *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() += 45.0f;
                    cvar->ConsoleColorPrintf(ColorRGBA(255,0,0,255), "%f\n",  *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget());
                    MarcisAWeeb = false;
                } else {
                    angle.y -= 180.0f;

                    *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() -= 45.0f;
                    cvar->ConsoleColorPrintf(ColorRGBA(255,0,0,255), "%f\n",  *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget());


                    MarcisAWeeb = true;
                }

                if (oldyaw == *((C_BasePlayer *) entityList->GetClientEntity(
                        engine->GetLocalPlayer()))->GetLowerBodyYawTarget()) {

                    angle.y += oldyaw / 2 + 133;
                    *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget() -= oldyaw / 3 + 133;
                }

                break;


            }
    }

    if (AntiAim::IsAirborne()) {
        angle.y += bFlip ? Settings::AntiAim::Airborne::Yaw::typeFakeAdd : Settings::AntiAim::Airborne::Yaw::typeAdd;
    } else if (AntiAim::IsMoving()) {
        angle.y += bFlip ? Settings::AntiAim::Moving::Yaw::typeFakeAdd : Settings::AntiAim::Moving::Yaw::typeAdd;
    } else {
        angle.y += bFlip ? Settings::AntiAim::Standing::Yaw::typeFakeAdd : Settings::AntiAim::Standing::Yaw::typeAdd;
    }
}

static void DoAntiAimX(QAngle &angle, bool bFlip, bool &clamp) {
    static C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    AntiAimType_X antiAim = Settings::AntiAim::Standing::Pitch::type;
    if (AntiAim::IsMoving()) {
        antiAim = Settings::AntiAim::Moving::Pitch::type;
    }
    if (AntiAim::IsAirborne()) {
        antiAim = Settings::AntiAim::Airborne::Pitch::type;
    }

    static bool fakezeroS = false;
    static bool fakeupS = false;

    switch (antiAim) {
        case AntiAimType_X::STATIC_UP:
            angle.x = -89.0f;
            break;
        case AntiAimType_X::FLIP:
            if (fabsf(pLocal->GetVelocity().x) != 0) {
                angle.x = bFlip ? -55.0f : 40.0f;
            } else {
                angle.x = 50.0f;
            }
            break;
        case AntiAimType_X::STATIC_DOWN:
            angle.x = 89.0f;
            break;
        case AntiAimType_X::FAKEZERO:
            fakezeroS = !fakezeroS;
            CreateMove::sendPacket = fakezeroS;
            angle.x = fakezeroS ? 0 : 89;
            break;
        case AntiAimType_X::FAKEUP:
            fakeupS = !fakeupS;
            CreateMove::sendPacket = fakeupS;
            angle.x = fakeupS ? -89 : 89;
            break;
    }

    if (AntiAim::IsAirborne()) {
        angle.x += Settings::AntiAim::Airborne::Pitch::custom;
    } else if (AntiAim::IsMoving()) {
        angle.x += Settings::AntiAim::Moving::Pitch::custom;
    } else {
        angle.x += Settings::AntiAim::Standing::Pitch::custom;
    }
}

static void DoAntiAimZ(QAngle &angle, int command_number, bool &clamp) {
    AntiAimType_Z antiAim = Settings::AntiAim::Standing::Roll::type;
    if (AntiAim::IsMoving()) {
        antiAim = Settings::AntiAim::Moving::Roll::type;
    }
    if (AntiAim::IsAirborne()) {
        antiAim = Settings::AntiAim::Airborne::Roll::type;
    }

    switch (antiAim) {
        case AntiAimType_Z::TEST:
            angle.z = 180.0f;
            break;
    }
}

static void DoAntiAimLBY(QAngle &angle, int command_number, bool bFlip, bool &clamp) {
    static float pDance = 0.0f;
    static bool xFlip;

    AntiAimType_LBY antiAim = Settings::AntiAim::Standing::LBY::type;
    if (AntiAim::IsMoving()) {
        antiAim = Settings::AntiAim::Moving::LBY::type;
    }
    if (AntiAim::IsAirborne()) {
        antiAim = Settings::AntiAim::Airborne::LBY::type;
    }

    static C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    xFlip = bFlip != xFlip;

    switch (antiAim) {
        case AntiAimType_LBY::ONE: {
            static bool flip1 = false;
            static float prevLBY1 = *((C_BasePlayer *) entityList->GetClientEntity(
                    engine->GetLocalPlayer()))->GetLowerBodyYawTarget();

            if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {

                if (prevLBY1 !=
                    *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget())
                    flip1 = false;
                else
                    flip1 = true;
                if (flip1)
                    angle.y += 108.f;
                else
                    angle.y -= 109.f;

                prevLBY1 = *((C_BasePlayer *) entityList->GetClientEntity(
                        engine->GetLocalPlayer()))->GetLowerBodyYawTarget();
            } else {
                angle.y -= 180;
            }
        }
            break;
        case AntiAimType_LBY::TWO: {
            static bool flip2 = false;
            static float prevLBY2 = *((C_BasePlayer *) entityList->GetClientEntity(
                    engine->GetLocalPlayer()))->GetLowerBodyYawTarget();

            if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {

                if (prevLBY2 !=
                    *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget())
                    flip2 = !flip2;

                if (flip2)
                    angle.y += 90.f;
                else
                    angle.y -= 90.f;

                prevLBY2 = *((C_BasePlayer *) entityList->GetClientEntity(
                        engine->GetLocalPlayer()))->GetLowerBodyYawTarget();
            } else
                angle.y -= 0.0f;
        }
            break;
        case AntiAimType_LBY::THREE: {
            static bool LBYflip = true;
            static float prevLBY1;
            static int cuck = 0;
            static float nextUpdate = globalVars->curtime + 1.1f;
            cuck = prevLBY1 + 108;
            LBYflip = !LBYflip;
            if (LBYflip) {
                if (fabsf(pLocal->GetVelocity().x) != 0) {
                    angle.y += 180.f;
                    CreateMove::sendPacket = false;
                } else {
                    if (globalVars->curtime > nextUpdate) {

                        angle.y += cuck;
                        CreateMove::sendPacket = false;
                        nextUpdate = globalVars->curtime + 1.1f;
                    }
                }
            } else {
                angle.y += *((C_BasePlayer *) entityList->GetClientEntity(
                        engine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 90.f;
                CreateMove::sendPacket = true;
            }
        }
            break;
        case AntiAimType_LBY::MYRRIB: {
            static float LBYsave = 0;
            float LBY = *((C_BasePlayer *) entityList->GetClientEntity(
                    engine->GetLocalPlayer()))->GetLowerBodyYawTarget();

            if (LBY != LBYsave) {
                LBYsave = LBY;
            }

            static bool LBYflipM = false;
            LBYflipM = !LBYflipM;
            if (LBYflipM) {
                angle.y = LBYsave;
                CreateMove::sendPacket = true;
            } else {
                angle.y = LBYsave + 180.f;
                CreateMove::sendPacket = false;
            }
        }
            break;
        case AntiAimType_LBY::FOUR:
            // TODO
            break;
        case AntiAimType_LBY::MYRRIB2: {


            bool onGround = (pLocal->GetFlags() & FL_ONGROUND);
            bool moving = (fabsf(pLocal->GetVelocity().x) != 0);
            bool onGroundMoving = (onGround && moving);

            float lby = *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget();
            float realAngle = AntiAim::lastRealYaw;
            float fakeAngle = AntiAim::lastFakeYaw;

            static bool switch1 = false;
            static bool switch2 = false;
            static bool switch3 = false;

            if (onGroundMoving) {
                angle.y -= 180;
            } else {
                if (realAngle == lby) {
                    switch1 = !switch1;
                    if (switch1) {
                        angle.y -= 180;
                        CreateMove::sendPacket = false;
                    } else {
                        angle.y += 90;
                        CreateMove::sendPacket = true;
                    }
                } else if (realAngle == fakeAngle) {
                    switch2 = !switch2;
                    if (switch2) {
                        angle.y -= 180;
                        CreateMove::sendPacket = false;
                    } else {
                        angle.y += 45;
                        CreateMove::sendPacket = true;
                    }
                } else {
                    switch3 = !switch3;
                    if (switch3) {
                        int rng = rand() % 3 + 1;
                        if (rng == 1) {
                            angle.y = lby + 90;
                            CreateMove::sendPacket = false;
                        } else if (rng == 2) {
                            angle.y = lby + 180;
                            CreateMove::sendPacket = false;
                        } else {
                            angle.y = lby - 90;
                            CreateMove::sendPacket = false;
                        }
                    } else {
                        angle.y = lby;
                        CreateMove::sendPacket = true;
                    }
                }
            }

        }
            break;
        case AntiAimType_LBY::NONE:
            //Settings::AntiAim::Lby::enabled = false;
            break;
    }
}


void AntiAim::CreateMove(CUserCmd *cmd) {
    isAntiAiming = false;

    if ((AntiAim::IsStanding() && !Settings::AntiAim::Standing::Yaw::enabled &&
         !Settings::AntiAim::Standing::Pitch::enabled) ||
        (AntiAim::IsMoving() && !Settings::AntiAim::Moving::Yaw::enabled &&
         !Settings::AntiAim::Moving::Pitch::enabled) ||
        (AntiAim::IsAirborne() && !Settings::AntiAim::Airborne::Yaw::enabled &&
         !Settings::AntiAim::Airborne::Pitch::enabled)) {
        if (Settings::FakeLag::enabled) {
            FakeLag::bFlipping = true;
        }
        return;
    }

    if (Settings::Aimbot::AimStep::enabled && Aimbot::aimStepInProgress)
        return;

    QAngle oldAngle = cmd->viewangles;
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;

    QAngle angle = cmd->viewangles;

    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!pLocal)
        return;

    C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *) entityList->GetClientEntityFromHandle(
            pLocal->GetActiveWeapon());
    if (!activeWeapon)
        return;

    if (activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE) {
        C_BaseCSGrenade *csGrenade = (C_BaseCSGrenade *) activeWeapon;

        if (csGrenade->GetThrowTime() > 0.0f)
            return;
    }

    if (cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || (cmd->buttons & IN_ATTACK2 &&
                                                              *activeWeapon->GetItemDefinitionIndex() ==
                                                              ItemDefinitionIndex::WEAPON_REVOLVER))
        return;
    // ^ This requires a rework for auto cock ^

    if (pLocal->GetMoveType() == MOVETYPE_LADDER || pLocal->GetMoveType() == MOVETYPE_NOCLIP)
        return;

    // AutoDisable checks

    // Knife
    if (Settings::AntiAim::Misc::AutoDisable::knifeHeld && pLocal->GetAlive() &&
        activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE) {
        return;
    }

    // Bomb
    if (Settings::AntiAim::Misc::AutoDisable::bombHeld && pLocal->GetAlive() &&
        activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_C4) {
        return;
    }

    // No Enemy
    if (Settings::AntiAim::Misc::AutoDisable::noEnemy && pLocal->GetAlive() && !HasViableEnemy()) {
        return;
    }

    // Freezetime
    if (Settings::AntiAim::Misc::AutoDisable::freezeTime &&
        ((*csGameRules) && (*csGameRules)->IsFreezeTime())) {
        return;
    }

    isAntiAiming = true;
    QAngle edge_angle = angle;

    bool shouldEdge = AntiAim::IsAirborne() ? Settings::AntiAim::Airborne::HeadEdge::enabled :
                      AntiAim::IsMoving() ? Settings::AntiAim::Moving::HeadEdge::enabled :
                      Settings::AntiAim::Standing::HeadEdge::enabled;
    shouldEdge = shouldEdge && GetBestHeadAngle(edge_angle);

    static bool bFlip;

    bFlip = !bFlip;
    FakeLag::bFlipping = bFlip;

    bool shouldClamp = (IsStanding() && !Settings::AntiAim::Standing::untrustedAngles) ||
                       (IsMoving() && !Settings::AntiAim::Moving::untrustedAngles) ||
                       (IsAirborne() && !Settings::AntiAim::Airborne::untrustedAngles);

    if (!Settings::AntiAim::Standing::untrustedAngles) {
        if (Settings::AntiAim::Standing::Pitch::type > AntiAimType_X::STATIC_DOWN) {
            Settings::AntiAim::Standing::Pitch::type = AntiAimType_X::STATIC_DOWN;
        }
        if (Settings::AntiAim::Standing::Roll::enabled) {
            Settings::AntiAim::Standing::Roll::enabled = false;
        }
    }

    if (!Settings::AntiAim::Moving::untrustedAngles) {
        if (Settings::AntiAim::Moving::Pitch::type > AntiAimType_X::STATIC_DOWN) {
            Settings::AntiAim::Moving::Pitch::type = AntiAimType_X::STATIC_DOWN;
        }
        if (Settings::AntiAim::Moving::Roll::enabled) {
            Settings::AntiAim::Moving::Roll::enabled = false;
        }
    }

    if (!Settings::AntiAim::Airborne::untrustedAngles) {
        if (Settings::AntiAim::Airborne::Pitch::type > AntiAimType_X::STATIC_DOWN) {
            Settings::AntiAim::Airborne::Pitch::type = AntiAimType_X::STATIC_DOWN;
        }
        if (Settings::AntiAim::Airborne::Roll::enabled) {
            Settings::AntiAim::Airborne::Roll::enabled = false;
        }
    }

    if (IsAirborne() ? Settings::AntiAim::Airborne::Yaw::enabled :
        IsMoving() ? Settings::AntiAim::Moving::Yaw::enabled :
        Settings::AntiAim::Standing::Yaw::enabled) {

        if ((IsAirborne() ? Settings::AntiAim::Airborne::LBY::enabled :
             IsMoving() ? Settings::AntiAim::Moving::LBY::enabled :
             Settings::AntiAim::Standing::LBY::enabled) && !bFlip) {
            DoAntiAimLBY(angle, cmd->command_number, bFlip, shouldClamp);
            Math::NormalizeAngles(angle);

        } else {
            DoAntiAimY(angle, cmd->command_number, bFlip, shouldClamp);
            Math::NormalizeAngles(angle);
        }
        if (!Settings::FakeLag::enabled)
            CreateMove::sendPacket = bFlip;
        if ((IsAirborne() ? Settings::AntiAim::Airborne::HeadEdge::enabled :
             IsMoving() ? Settings::AntiAim::Moving::HeadEdge::enabled :
             Settings::AntiAim::Standing::HeadEdge::enabled) && shouldEdge && !bFlip)
            angle.y = edge_angle.y;

        if ((IsAirborne() ? Settings::AntiAim::Airborne::SwitchAA::enabled :
             IsMoving() ? Settings::AntiAim::Moving::SwitchAA::enabled :
             Settings::AntiAim::Standing::SwitchAA::enabled))
            SwapAA();
    }
    /*if (Settings::AntiAim::Yaw::dynamicAA) {

        DoAAatTarget(angle, cmd->command_number, bFlip, should_clamp);
        Math::NormalizeAngles(angle);
        if (!Settings::FakeLag::enabled)
            CreateMove::sendPacket = bFlip;
        if (Settings::AntiAim::HeadEdge::enabled && edging_head && !bFlip)
            angle.y = edge_angle.y;
    }*/

    if ((IsAirborne() ? Settings::AntiAim::Airborne::SwitchAA::enabled && Settings::AntiAim::Airborne::untrustedAngles
                      :
         IsMoving() ? Settings::AntiAim::Moving::SwitchAA::enabled && Settings::AntiAim::Moving::untrustedAngles :
         Settings::AntiAim::Standing::SwitchAA::enabled && Settings::AntiAim::Standing::untrustedAngles)) {
        DoAntiAimZ(angle, cmd->command_number, shouldClamp);
    }
    if ((IsAirborne() ? Settings::AntiAim::Airborne::Pitch::enabled :
         IsMoving() ? Settings::AntiAim::Moving::Pitch::enabled :
         Settings::AntiAim::Standing::Pitch::enabled)) {
        DoAntiAimX(angle, bFlip, shouldClamp);
    }

    Math::NormalizeAngles(angle);

    if (shouldClamp) {
        Math::ClampAngles(angle);
    }

    cmd->viewangles = angle;

    if ((IsAirborne() ? Settings::AntiAim::Airborne::antiResolver :
         IsMoving() ? Settings::AntiAim::Moving::antiResolver :
         Settings::AntiAim::Standing::antiResolver)) {
        static bool antiResolverFlip = false;

        if (cmd->viewangles.y == *((C_BasePlayer *) entityList->GetClientEntity(
                            engine->GetLocalPlayer()))->GetLowerBodyYawTarget()) {
            if (antiResolverFlip) {
                cmd->viewangles.y += +90;
            } else {
                cmd->viewangles.y -= +90;
            }

            antiResolverFlip = !antiResolverFlip;

            Math::NormalizeAngles(cmd->viewangles);

            if (shouldClamp) {
                Math::ClampAngles(cmd->viewangles);
            }
        }
    }

    if (bFlip) {
        AntiAim::lastFakeYaw = cmd->viewangles.y;
    } else {
        AntiAim::lastRealYaw = cmd->viewangles.y;
    }

    Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
}

bool AntiAim::IsStanding() {
    return !IsMoving() && !IsAirborne();
}

bool AntiAim::IsMoving() {
    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!pLocal || !pLocal->GetAlive())
        return false;

    return pLocal->GetVelocity().x != 0;
}

bool AntiAim::IsAirborne() {
    C_BasePlayer *pLocal = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!pLocal || !pLocal->GetAlive() || !pLocal->GetFlags())
        return false;

    return !(pLocal->GetFlags() & FL_ONGROUND);
}
