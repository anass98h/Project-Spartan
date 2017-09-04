#include "antiaim.h"

bool Settings::AntiAim::allowUntrustedAngles = false;
bool Settings::AntiAim::Yaw::dynamicAA = false;
bool Settings::AntiAim::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Roll::type = AntiAimType_Z::TEST; // Dank Roll
bool Settings::AntiAim::Yaw::enabled = false;
bool Settings::AntiAim::Pitch::enabled = false;
bool Settings::AntiAim::Lby::enabled = false;
AntiAimType_LBY Settings::AntiAim::Lby::type = AntiAimType_LBY::ONE;
AntiAimType_Y Settings::AntiAim::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
bool Settings::AntiAim::Yaw::antiResolver = false;
AntiAimType_X Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_DOWN;
bool Settings::AntiAim::HeadEdge::enabled = false;
float Settings::AntiAim::HeadEdge::distance = 25.0f;
bool Settings::AntiAim::AutoDisable::noEnemy = false;
bool Settings::AntiAim::AutoDisable::knifeHeld = false;
bool Settings::AntiAim::AutoDisable::bombHeld = false;
bool Settings::AntiAim::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::SwitchAA::key = ButtonCode_t::KEY_DOWN;
float AntiAim::lastRealYaw = 0.0f;
float AntiAim::lastFakeYaw = 0.0f;
bool AntiAim::isAntiAiming = false;


static float Distance(Vector a, Vector b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

static bool GetBestHeadAngle(QAngle& angle) {
    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

    Vector position = localplayer->GetVecOrigin() + localplayer->GetVecViewOffset();

    float closest_distance = 100.0f;

    float radius = Settings::AntiAim::HeadEdge::distance + 0.1f;
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

    return closest_distance < Settings::AntiAim::HeadEdge::distance;
}



static bool HasViableEnemy() {
    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

    for (int i = 1; i < engine->GetMaxClients(); ++i) {
        C_BasePlayer* entity = (C_BasePlayer*) entityList->GetClientEntity(i);

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

            if (std::find(Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid) != Aimbot::friends.end())
                continue;
        }

        if (Settings::Aimbot::friendly || entity->GetTeam() != localplayer->GetTeam())
            return true;
    }

    return false;
}

static float DoAAatTarget()
 {

    static C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    static float Angle = 0.0f;

    if (Settings::AntiAim::Yaw::enabled) {
        if (Settings::AntiAim::Yaw::dynamicAA) {
            float bestDist = 999999999.f; // easy cuz im retarded
            for (int i = 1; i < engine->GetMaxClients(); ++i) {
                C_BasePlayer* target = (C_BasePlayer*) entityList->GetClientEntity(i);

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

static void SwapAA()
{
    int count;

    if(inputSystem->IsButtonDown(KEY_DOWN)) {
        count++;
        //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Swap AA Count: %i\n", count);
    } else{ 
    if (count > 0) {
        static AntiAimType_Y fake = Settings::AntiAim::Yaw::typeFake;
        static AntiAimType_Y real = Settings::AntiAim::Yaw::type;
        fake = Settings::AntiAim::Yaw::typeFake;
        real = Settings::AntiAim::Yaw::type;
        Settings::AntiAim::Yaw::type = fake;
        Settings::AntiAim::Yaw::typeFake = real;
        count = 0;
    }
}
}

float GetLatency()
{
	INetChannelInfo *nci = engine->GetNetChannelInfo();
	if (nci)
	{
		float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
		return Latency;
	}
	else
	{
		return 0.0f;
	}
}
float GetOutgoingLatency()
{
	INetChannelInfo *nci = engine->GetNetChannelInfo();
	if (nci)
	{
		float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else
	{
		return 0.0f;
	}
}
float GetIncomingLatency()
{
	INetChannelInfo *nci = engine->GetNetChannelInfo();
	if (nci)
	{
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		return IncomingLatency;
	}
	else
	{
		return 0.0f;
	}
}
float OldLBY;
float LBYBreakerTimer;
float LastLBYUpdateTime;
bool bSwitch;
float CurrentVelocity(C_BasePlayer* LocalPlayer)
{
	int vel = LocalPlayer->GetVelocity().Length2D();
	return vel;
}
bool NextLBYUpdate()
{
	C_BasePlayer* LocalPlayer =  (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	float flServerTime = (float)(LocalPlayer->GetTickBase()  * globalVars->interval_per_tick);


	if (OldLBY != *LocalPlayer->GetLowerBodyYawTarget())
	{
		LBYBreakerTimer++;
		OldLBY = *LocalPlayer->GetLowerBodyYawTarget();
		bSwitch = !bSwitch;
		LastLBYUpdateTime = flServerTime;
	}

	if (CurrentVelocity(LocalPlayer) > 0.5)
	{
		LastLBYUpdateTime = flServerTime;
		return false;
	}

	if ((LastLBYUpdateTime + 1 - (GetLatency() * 2) < flServerTime) && (LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (LastLBYUpdateTime + 1.1 - (GetLatency() * 2) < flServerTime)
		{
			LastLBYUpdateTime += 1.1;
		}
		return true;
	}
	return false;
}


static void DoAntiAimY(QAngle& angle, int command_number, bool bFlip, bool& clamp) {
    AntiAimType_Y aa_type = bFlip ? Settings::AntiAim::Yaw::typeFake : Settings::AntiAim::Yaw::type;

    static bool yFlip;
    double factor;

    int random;
    int ticks = 0;
    int jitterticks = 0;
    static C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());


    yFlip = bFlip != yFlip;

    switch (aa_type) {
        case AntiAimType_Y::LEGITTROLLING:
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
            angle.y -= 180.f;
            //Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
            break;
        case AntiAimType_Y::LEGITTROLLING2:
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
            angle.y -= 90.0f;
            //	Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
            break;
        case AntiAimType_Y::SPIN:
            factor = 360.0 / M_PHI;
            factor *= Settings::spinFactor::value;
            angle.y = fmodf(globalVars->curtime * factor, 360.0);
            break;
        case AntiAimType_Y::CUSTOM:
            Math::ClampY(Settings::customYaw::value);
            Math::ClampY(Settings::customYaw::value);

            if(Settings::customYaw::lby)
            {
                if(Settings::customYaw::value > 0)
                    angle.y += *pLocal->GetLowerBodyYawTarget() + (Settings::customYaw::value);
                else
                    angle.y -= *pLocal->GetLowerBodyYawTarget() - (Settings::customYaw::value);

            }
            else
            {
                angle.y += Settings::customYaw::value;
            }
            break;
        case AntiAimType_Y::CUSTOM2:
            Math::ClampY(Settings::customYaw2::value);
            Math::ClampY(Settings::customYaw2::value);

            if(Settings::customYaw2::lby)
            {
                if(Settings::customYaw2::value > 0)
                    angle.y += *pLocal->GetLowerBodyYawTarget() + (Settings::customYaw2::value);
                else
                    angle.y -= *pLocal->GetLowerBodyYawTarget() - (Settings::customYaw2::value);
            }
            else
            {
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
                        angle.y = *pLocal->GetLowerBodyYawTarget() + rand() % 220;
                        break;
                    case 1:
                        angle.y = *pLocal->GetLowerBodyYawTarget() - 97;
                        break;
                    case 2:
                        yFlip ? angle.y += 97 : angle.y -= 97;
                        break;
                    case 3:
                        angle.y = *pLocal->GetLowerBodyYawTarget() - rand() + 97;
                        break;
                }
                CreateMove::sendPacket = false;

            } else {
                static int psilent;
                psilent = rand() % 6;
                switch (psilent) {
                    case 0:
                        angle.y = *pLocal->GetLowerBodyYawTarget() + 88;
                        break;
                    case 1:
                        angle.y = bSendPacket ? 118 : 270;
                        break;
                    case 2:
                        yFlip ? angle.y = *pLocal->GetLowerBodyYawTarget() + 97 : angle.y =
                                                                                          *pLocal->GetLowerBodyYawTarget() -
                                                                                          rand() % 97;
                        break;
                    case 3:
                        angle.y = *pLocal->GetLowerBodyYawTarget() - rand();
                        break;
                    case 4:
                        angle.y -= 97;
                        break;
                    case 5:
                        yFlip ? angle.y = *pLocal->GetLowerBodyYawTarget() + rand() % 112 : angle.y =
                                                                                                    *pLocal->GetLowerBodyYawTarget() -
                                                                                                    rand() % 66;
                        break;


                }
                CreateMove::sendPacket = false;

            }
            break;
        case AntiAimType_Y::TANK2: 
            
            if(bSendPacket) {
                random = rand() % 4; 
                switch (random) { 
                    case 1: angle.y = *pLocal->GetLowerBodyYawTarget() + rand() % 35 + 165;
                    break; 
                    case 2: yFlip ? angle.y -= 160 : angle.y += 160;
                    break; 
                    case 3: factor = 360.0 / M_PHI; factor *= rand() % 25; float x = fmodf(globalVars->curtime * factor, 360.0); factor *= 5; float y = fmodf(globalVars->curtime * factor, 360.0f);
                    if (y >= 100.0f) 
                    { 
                            y -= 170.0f; } if (y <= 200.0f) 
                    { 
                            y += 305.00f;
                }
                angle.y=((x/y)+60.2f)*M_PI;
                } 
                } 
                else 
                {
                random = rand() % 4; 
                switch (random) { 
                case 1: angle.y -= 180.0f; 
                break; 
                case 2: yFlip ? angle.y += 90.f : angle.y -= 90.0f;
                break; 
                case 3: factor = 360.0 / M_PHI; angle.y = fmodf(globalVars->curtime * factor, 360.0);

                } 
            }
            if (angle.y == *pLocal->GetLowerBodyYawTarget()){
                angle.y = *pLocal->GetLowerBodyYawTarget() + 90;

            }
            
            break;
        case AntiAimType_Y::LBYBREAK:
           //Placeholder
            break;
        case AntiAimType_Y::FAKELBY:
            static bool lbyflip = false;
            static bool lbyflip2 = false;
            static bool lbyflip3 = false;
            clamp = false;
            if (!(pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f))
                
                yFlip ? angle.y -= 170.f : angle.y -= 190.f;
            else
            {
              if(CreateMove::sendPacket)
              {
                yFlip ? angle.y = *pLocal->GetLowerBodyYawTarget() + 97 : angle.y -= *pLocal->GetLowerBodyYawTarget() + 97;

                CreateMove::sendPacket = false;
              }
              else
              {
                if (lbyflip)
                {
                  angle.y = *pLocal->GetLowerBodyYawTarget() + 120;
                }
                else
                {
                  yFlip ? angle.y = *pLocal->GetLowerBodyYawTarget() + 88 : angle.y -= *pLocal->GetLowerBodyYawTarget() + 88;
                }
                if(lbyflip2)
                  angle.y = *pLocal->GetLowerBodyYawTarget() + 45;
                else
                  *pLocal->GetLowerBodyYawTarget() + 180;
                if(lbyflip3)
                  angle.y = *pLocal->GetLowerBodyYawTarget() + rand() + 360;
                else
                  angle.y = *pLocal->GetLowerBodyYawTarget() + 97;

              }


            }
            lbyflip = !lbyflip;
            lbyflip2 = !lbyflip2;
            lbyflip3 = !lbyflip3;
            break;
        case AntiAimType_Y::LBYSPIN:
            factor = 360.0 / M_PHI;
            factor *= Settings::spinFactor::value;
            angle.y = *pLocal->GetLowerBodyYawTarget() + fmodf(globalVars->curtime * factor, 360.0);
            break;
        case AntiAimType_Y::LBYJITTER:

            if (pLocal->GetFlags() & FL_ONGROUND)
                angle.y = *pLocal->GetLowerBodyYawTarget() + rand() % 35 + 165;
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
            angle.y = *pLocal->GetLowerBodyYawTarget();
            break;
        case AntiAimType_Y::FJITTER:
        {
        if (CreateMove::sendPacket)
                        {
                            if (jitterticks > 0)
                                jitterticks = -1;
                                jitterticks++;
                        }
                    int add = 0;
                    if (jitterticks == 0)
                        add += 130;
                    if (jitterticks == 1)
                        add -= 145;
                    if (ticks > 0 || !CreateMove::sendPacket)
                        {
                            add = -add;
                            CreateMove::sendPacket = false;
                        }
                    angle.y += add;
                        }
            break;
        case AntiAimType_Y::richieap:
        static bool besteap = false;
        if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) 
        {
            besteap = !besteap;
            if (besteap)
                {
                    angle.y -= *pLocal->GetLowerBodyYawTarget() - 180.f;
                    CreateMove::sendPacket = false;
                }
                else
                {
                    angle.y = *pLocal->GetLowerBodyYawTarget();
                }
        }
        else
        {
            angle.y -= 180.f;
        }
	case AntiAimType_Y::MYRRIB:
            if (CreateMove::sendPacket)
            {
                if (fabsf(pLocal->GetVelocity().x) != 0)
                {
                    angle.y -= 180.f;
                    CreateMove::sendPacket = false;
                }
                else {
                    angle.y -= *pLocal->GetLowerBodyYawTarget() + 45.f;
                    CreateMove::sendPacket = false;
                }
            }
            else
            {
               if (fabsf(pLocal->GetVelocity().x) != 0)
                {
                    angle.y -= 0; //pMemez here plz fix me
                    CreateMove::sendPacket = true;
                }
                else {
                    angle.y -= *pLocal->GetLowerBodyYawTarget() - 180.f;
                    CreateMove::sendPacket = true;
                }
            }
            break;
    }
}

static void DoAntiAimX(QAngle& angle, bool bFlip, bool& clamp) {



    static C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

    AntiAimType_X aa_type = Settings::AntiAim::Pitch::type;





    switch (aa_type) {
        case AntiAimType_X::STATIC_UP:
            angle.x = -89.0f;
            break;
        case AntiAimType_X::FLIP:
            if (fabsf(pLocal->GetVelocity().x) != 0)
            {
                angle.x = bFlip ? -55.0f : 40.0f;
            }
            else
            {
                angle.x = 50.0f;
            }
            break;
        case AntiAimType_X::STATIC_DOWN:
            angle.x = 89.0f;
            break;
    }
}

static void DoAntiAimZ(QAngle& angle, int command_number, bool& clamp) {
    AntiAimType_Z aa_type = Settings::AntiAim::Roll::type;

    switch (aa_type) {
        case AntiAimType_Z::TEST:

            angle.z = 180.0f;
            break;

    }
}

static void DoAntiAimLBY(QAngle& angle, int command_number, bool bFlip, bool& clamp) {
    static float pDance = 0.0f;
    static bool xFlip;

    AntiAimType_LBY aa_type = Settings::AntiAim::Lby::type;
    static C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    xFlip = bFlip != xFlip;

    switch (aa_type) {

        case AntiAimType_LBY::ONE:
            static bool flip1 = false;
            static float prevLBY1 = *pLocal->GetLowerBodyYawTarget();

            if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) 
            {

                if (prevLBY1 != *pLocal->GetLowerBodyYawTarget())
                    flip1 = false;
                if (prevLBY1 != *pLocal->GetLowerBodyYawTarget())
                    flip1 = true;
                if (flip1)
                    angle.y += 120.f;
                else
                    angle.y -= 120.f;

                prevLBY1 = *pLocal->GetLowerBodyYawTarget();
            }
            else
            {
                angle.y -= 0;
            }
            break;
        case AntiAimType_LBY::TWO:
            static bool flip2 = false;
            static float prevLBY2 = *pLocal->GetLowerBodyYawTarget();

            if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {

                if (prevLBY2 != *pLocal->GetLowerBodyYawTarget())
                    flip2 = !flip2;

                if (flip2)
                    angle.y += 90.f;
                else
                    angle.y -= 90.f;

                prevLBY2 = *pLocal->GetLowerBodyYawTarget();
            } else   
                angle.y -= 0.0f;
            break;
        case AntiAimType_LBY::THREE:
            static int flip3 = (int) (floorf(globalVars->curtime) / 1.1) % 2;

            if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f)
                angle.y += 225.f + flip3 * 145.0f;
            else
                angle.y += 0.f;
            break;
        case AntiAimType_LBY::FOUR:

		if (NextLBYUpdate())
			angle.y -= 90;
		else
			angle.y += 90;
                break;
        case AntiAimType_LBY::NONE:
            Settings::AntiAim::Lby::enabled = false;
            break;
    }
}


void AntiAim::CreateMove(CUserCmd* cmd) {


    isAntiAiming = false;
    if (!Settings::AntiAim::Yaw::enabled && !Settings::AntiAim::Pitch::enabled)
	{
		if(Settings::FakeLag::enabled)
			FakeLag::bFlipping = true;
		return;
	}


    if (Settings::Aimbot::AimStep::enabled && Aimbot::aimStepInProgress)
        return;

    QAngle oldAngle = cmd->viewangles;
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;

    QAngle angle = cmd->viewangles;

    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer)
        return;

    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    if (!activeWeapon)
        return;

    if (activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE) {
        C_BaseCSGrenade* csGrenade = (C_BaseCSGrenade*) activeWeapon;

        if (csGrenade->GetThrowTime() > 0.f)
            return;
    }

    if (cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || (cmd->buttons & IN_ATTACK2 && *activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER))
        return;
    if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
        return;

    // AutoDisable checks

    // Knife
    if (Settings::AntiAim::AutoDisable::knifeHeld && localplayer->GetAlive() && activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE)
        return;
    // Bomb
    if (Settings::AntiAim::AutoDisable::bombHeld && localplayer->GetAlive() && activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_C4)
        return;
    // No Enemy
    if (Settings::AntiAim::AutoDisable::noEnemy && localplayer->GetAlive() && !HasViableEnemy())
        return;

	isAntiAiming = true;
    QAngle edge_angle = angle;
    bool edging_head = Settings::AntiAim::HeadEdge::enabled && GetBestHeadAngle(edge_angle);
    static bool bFlip;

    bFlip = !bFlip;
    FakeLag::bFlipping = bFlip;

    bool should_clamp = !Settings::AntiAim::allowUntrustedAngles;

    if (!ValveDSCheck::forceUT && (*csGameRules) && (*csGameRules)->IsValveDS() && should_clamp) {
        if (Settings::AntiAim::Yaw::type >= AntiAimType_Y::CUSTOM)
            Settings::AntiAim::Yaw::type = AntiAimType_Y::CUSTOM;

        if (Settings::AntiAim::Yaw::typeFake >= AntiAimType_Y::CUSTOM)
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::CUSTOM;

        if (Settings::AntiAim::Pitch::type >= AntiAimType_X::STATIC_UP)
            Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_UP;
    }

    if (Settings::AntiAim::Yaw::enabled) {
        if (Settings::AntiAim::Lby::enabled && !bFlip) {
            DoAntiAimLBY(angle, cmd->command_number, bFlip, should_clamp);
            Math::NormalizeAngles(angle);
            if (!Settings::FakeLag::enabled)
                CreateMove::sendPacket = bFlip;
            if (Settings::AntiAim::HeadEdge::enabled && edging_head && !bFlip)
                angle.y = edge_angle.y;
        } else {
            DoAntiAimY(angle, cmd->command_number, bFlip, should_clamp);
            Math::NormalizeAngles(angle);
            if (!Settings::FakeLag::enabled)
                CreateMove::sendPacket = bFlip;
            if (Settings::AntiAim::HeadEdge::enabled && edging_head && !bFlip)
                angle.y = edge_angle.y;
        }
        if (Settings::AntiAim::SwitchAA::enabled)
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

    if (Settings::AntiAim::Roll::enabled && Settings::AntiAim::allowUntrustedAngles)
        DoAntiAimZ(angle, cmd->command_number, should_clamp);
    if (Settings::AntiAim::Pitch::enabled)
        DoAntiAimX(angle, bFlip, should_clamp);
    Math::NormalizeAngles(angle);
    if (should_clamp) {
        Math::ClampAngles(angle);
    }

    cmd->viewangles = angle;

    if (Settings::AntiAim::Yaw::antiResolver)
    {
        static bool antiResolverFlip = false;

        static C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

        if (cmd->viewangles.y == *localplayer->GetLowerBodyYawTarget())
        {
            if (antiResolverFlip)
                cmd->viewangles.y += *pLocal->GetLowerBodyYawTarget() + 90;
            else
                cmd->viewangles.y -= *pLocal->GetLowerBodyYawTarget() + 90;

            antiResolverFlip = !antiResolverFlip;

            Math::NormalizeAngles(cmd->viewangles);

            if (should_clamp)
            {
                Math::ClampAngles(cmd->viewangles);
            }
        }
    }

	if (bFlip)
	{
		AntiAim::lastFakeYaw = cmd->viewangles.y;
	}
	else
	{
		AntiAim::lastRealYaw = cmd->viewangles.y;
	}

    Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);

}
