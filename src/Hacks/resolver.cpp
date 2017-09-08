#include "resolver.h"

bool Settings::Resolver::enabled = false;
float Settings::Resolver::ticks = 2;
float Settings::Resolver::modulo = 2;
bool Settings::Resolver::LagComp = false;
ResolverType Settings::Resolver::Type = ResolverType::OFF;

std::vector<int64_t> Resolver::playerAngleLogs = {};
std::array<CResolveInfo, 32> Resolver::m_arrInfos;

int Shotsmissed = 0;
bool shotATT;
std::vector<std::pair<C_BasePlayer*, QAngle>> player_data;
std::random_device rd;
 static float rSimTime[65]; 


static void StartLagComp(C_BasePlayer* player, CUserCmd* cmd)
{   
        ConVar* cvar_cl_interp = cvar->FindVar(XORSTR("cl_interp"));
	ConVar* cvar_cl_updaterate = cvar->FindVar(XORSTR("cl_updaterate"));
	ConVar* cvar_cl_interp_ratio = cvar->FindVar(XORSTR("cl_interp_ratio"));
        // is this lagcomp ? :/
	float cl_interp = cvar_cl_interp->fValue;
	float cl_updaterate = cvar_cl_updaterate->fValue;
	float cl_interp_ratio = cvar_cl_interp_ratio->fValue;
        float tick = (player->GetSimulationTime() + std::max(cl_interp, cl_interp_ratio / cl_updaterate));
        cmd->tick_count = TIME_TO_TICKS(tick);
}

void Resolver::Resolve(C_BasePlayer* target) {
    auto cur = m_arrInfos.at(target->GetIndex()).m_sRecords;
   
      //  INetChannelInfo* nci = engine->GetNetChannelInfo();
    static float bodyeyedelta = target->GetEyeAngles()->y - cur.front().m_flLowerBodyYawTarget;


    switch (Settings::Resolver::Type) {
       
	case ResolverType::ON:
        {
            // 1. Check if LBY is updated
            // 2. If yes -> LBY, If not -> 3.
            // 3. If Static angle -> If Miss on LBY -> brute
            // 4. Else Brute

            static bool isLBYupdated = false;
            static bool isMoving = false;
            static bool staticAngle = false;
            static bool firstLBYupdate = false;

            bool onGround = target->GetFlags() & FL_ONGROUND;
            float curTime = globalVars->curtime;
            float LBY = *target->GetLowerBodyYawTarget();

            static float nextUpdate = 0.f;

            if (onGround && fabsf(target->GetVelocity().x) > 1)
            {
                isMoving = true;
            }
            else
            {
                isMoving = false;
            }

            if (firstLBYupdate && curTime > nextUpdate && onGround && fabsf(bodyeyedelta) > 35.f)
            {
                isLBYupdated = true;
                nextUpdate = curTime + 1.1f;
            }
            else
            {
                isLBYupdated = false;
            }

            if (HasStaticRealAngle(cur))
            {
                staticAngle = true;
            }
            else
            {
                staticAngle = false;
            }

            if (isMoving || isLBYupdated || fabsf(bodyeyedelta) < 35.f)
            {
                firstLBYupdate = true;
                target->GetEyeAngles()->y = LBY;
            }
            else
            {
                // LBY is NOT updated
                if (staticAngle)
                {
                    if (target->GetEyeAngles()->y == LBY){
                        int a = Shotsmissed % 10;
                        switch (a)
                        {
                            case 0: target->GetEyeAngles()->y = LBY; break;
                            case 1: target->GetEyeAngles()->y = LBY + 180; break;
                            case 2: target->GetEyeAngles()->y = LBY + 90; break;
                            case 3: target->GetEyeAngles()->y = LBY + 135; break;
                            case 4: target->GetEyeAngles()->y = LBY - 90; break;
                            case 5: target->GetEyeAngles()->y = LBY + 45; break;
                            case 6: target->GetEyeAngles()->y = LBY - 45; break;
                            case 7: target->GetEyeAngles()->y = LBY - 135; break;
                            case 8: target->GetEyeAngles()->y = LBY + 100; break;
                            case 9: target->GetEyeAngles()->y = LBY - 100; break;
                            default: target->GetEyeAngles()->y = LBY; break;
                        }
                    }
                    else {
                        int b = Shotsmissed % 10;
                        switch (b)
                        {
                            case 0: target->GetEyeAngles()->y = LBY; break;
                            case 1: target->GetEyeAngles()->y += 180; break;
                            case 2: target->GetEyeAngles()->y -= 90; break;
                            case 3: target->GetEyeAngles()->y += 135; break;
                            case 4: target->GetEyeAngles()->y += 90; break;
                            case 5: target->GetEyeAngles()->y -= 45; break;
                            case 6: target->GetEyeAngles()->y += 45; break;
                            case 7: target->GetEyeAngles()->y -= 135; break;
                            case 8: target->GetEyeAngles()->y -= 100; break;
                            case 9: target->GetEyeAngles()->y += 100; break;
                            default: target->GetEyeAngles()->y = LBY; break;
                        }
                    }
                }
                else
                {
                    int c = Shotsmissed % 11;
                    int rng = rand() % 180 + 1;
                    switch (c)
                    {
                        case 0: target->GetEyeAngles()->y = LBY; break; // This should not even be used
                        case 1: target->GetEyeAngles()->y += 180.f; break;
                        case 2: target->GetEyeAngles()->y = LBY + 180.f; break; // This is where it will start
                        case 3: target->GetEyeAngles()->y = LBY + 90.f; break;
                        case 4: target->GetEyeAngles()->y = LBY + 135.f; break;
                        case 5: target->GetEyeAngles()->y = LBY - 90.f; break;
                        case 6: target->GetEyeAngles()->y = LBY + 45.f; break;
                        case 7: target->GetEyeAngles()->y = LBY - 135.f; break;
                        case 8: target->GetEyeAngles()->y = LBY - 45.f; break;
                        case 9: target->GetEyeAngles()->y = LBY + rng; break;
                        case 10: target->GetEyeAngles()->y = LBY - rng; break;
                        default: target->GetEyeAngles()->y = LBY; break;
                    }
                }
	        }
	    }
        case ResolverType::OFF:break;
    }
}

void Resolver::FrameStageNotify(ClientFrameStage_t stage) {
    if (!Settings::Resolver::enabled || !engine->IsInGame())
        return;

    C_BasePlayer* me = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!me || !me->GetAlive())
        return;

    if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
        for (int i = 1; i < engine->GetMaxClients(); ++i) {
            C_BasePlayer* target = (C_BasePlayer*) entityList->GetClientEntity(i);

            if (!target
                    || target == me
                    || target->GetDormant()
                    || !target->GetAlive()
                    || target->GetImmune()
                    || target->GetTeam() == me->GetTeam())
                continue;

            IEngineClient::player_info_t entityInformation;
            engine->GetPlayerInfo(i, &entityInformation);

            if (std::find(Resolver::playerAngleLogs.begin(), Resolver::playerAngleLogs.end(), entityInformation.xuid) == Resolver::playerAngleLogs.end())
            {
                player_data.push_back(std::pair<C_BasePlayer*, QAngle>(target, *target->GetEyeAngles()));
            }

            if (shotATT) {
                Shotsmissed++;
                shotATT = false;
            }

           
                Resolve(target);
                shotATT = true;
            
        }
    } else if (stage == ClientFrameStage_t::FRAME_RENDER_END) {
        for (unsigned long i = 0; i < player_data.size(); i++) {
            std::pair<C_BasePlayer*, QAngle> player_aa_data = player_data[i];
            *player_aa_data.first->GetEyeAngles() = player_aa_data.second;
        }

        player_data.clear();
    }
}

void Resolver::PostFrameStageNotify(ClientFrameStage_t stage) {
}

CTickRecord Resolver::GetShotRecord(C_BasePlayer* target) {
    for (auto cur : m_arrInfos[target->GetIndex()].m_sRecords) {
        if (cur.validtick)
            return CTickRecord(cur);
    }
    return CTickRecord();
}

bool& Resolver::LowerBodyYawChanged(C_BasePlayer* target) {
    return m_arrInfos.at(target->GetIndex()).m_bLowerBodyYawChanged;
}

void Resolver::StoreVars(C_BasePlayer* target) {
    if (m_arrInfos.at(target->GetIndex()).m_sRecords.size() >= Settings::Resolver::ticks) {
        m_arrInfos.at(target->GetIndex()).m_sRecords.pop_back();
    }
    m_arrInfos.at(target->GetIndex()).m_sRecords.push_front(CTickRecord(target));
}

void Resolver::StoreVars(C_BasePlayer* target, QAngle ang, float lby, float simtime, float tick) {
    if (m_arrInfos.at(target->GetIndex()).m_sRecords.size() >= Settings::Resolver::ticks)
        m_arrInfos.at(target->GetIndex()).m_sRecords.pop_back();
    m_arrInfos.at(target->GetIndex()).m_sRecords.push_front(CTickRecord(target));
}

bool& Resolver::BacktrackThisTick(C_BasePlayer* target) {
    return m_arrInfos.at(target->GetIndex()).m_bBacktrackThisTick;
}

bool Resolver::HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance) {
    auto minmax = std::minmax_element(std::begin(l), std::end(l), [](const CTickRecord& t1, const CTickRecord & t2) {
        return t1.m_flLowerBodyYawTarget < t2.m_flLowerBodyYawTarget; });
    return (fabs(minmax.first->m_flLowerBodyYawTarget - minmax.second->m_flLowerBodyYawTarget) <= tolerance);
}

bool Resolver::HasStaticRealAngle(int index, float tolerance) {
    return HasStaticRealAngle(m_arrInfos[index].m_sRecords, tolerance);
}

bool Resolver::HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance) {
    for (auto i = l.begin(); i < l.end() - 1;) {
        if (GetDelta(LBYDelta(*i), LBYDelta(*++i)) > tolerance)
            return false;
    }
    return true;
}

bool Resolver::HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance) {
    size_t misses = 0;
    for (size_t i = 0; i < l.size() - 1; i++) {
        float tickdif = static_cast<float> (l.at(i).m_flSimulationTime - l.at(i + 1).tickcount);
        float lbydif = GetDelta(l.at(i).m_flLowerBodyYawTarget, l.at(i + 1).m_flLowerBodyYawTarget);
        float ntickdif = static_cast<float> (globalVars->tickcount - l.at(i).tickcount);
        if (((lbydif / tickdif) * ntickdif) > tolerance) misses++;
    }
    return (misses <= (l.size() / 3));
}

int Resolver::GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance) {
    std::vector<float> vec;
    for (auto var : l) {
        float curdelta = LBYDelta(var);
        bool add = true;
        for (auto fl : vec) {
            if (!IsDifferent(curdelta, fl, tolerance))
                add = false;
        }
        if (add)
            vec.push_back(curdelta);
    }
    return vec.size();
}

int Resolver::GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance) {
    std::vector<float> vec;
    for (auto var : l) {
        float curyaw = var.m_flLowerBodyYawTarget;
        bool add = true;
        for (auto fl : vec) {
            if (!IsDifferent(curyaw, fl, tolerance))
                add = false;
        }
        if (add)
            vec.push_back(curyaw);
    }
    return vec.size();
}

bool Resolver::DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
    return (GetDifferentDeltas(cur) > (int) cur.size() / 2);
}

bool Resolver::LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
    return (GetDifferentLBYs(cur, tolerance) > (int) cur.size() / 2);
}

float Resolver::GetLBYByComparingTicks(const std::deque<CTickRecord>& l) {
    int modulo = Settings::Resolver::modulo;
    //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "\nmodulo " + modulo);
    int difangles = GetDifferentLBYs(l);
    int inc = modulo * difangles;
    for (auto var : l) {
        for (int lasttick = var.tickcount; lasttick < globalVars->tickcount; lasttick += inc) {
            if (lasttick == globalVars->tickcount)
                return var.m_flLowerBodyYawTarget;
        }
    }
    return 0.f;
}

float Resolver::GetDeltaByComparingTicks(const std::deque<CTickRecord>& l) {
    int modulo = Settings::Resolver::modulo;
    int difangles = GetDifferentDeltas(l);
    int inc = modulo * difangles;
    for (auto var : l) {
        for (int lasttick = var.tickcount; lasttick < globalVars->tickcount; lasttick += inc) {
            if (lasttick == globalVars->tickcount)
                return LBYDelta(var);
        }
    }
    return 0.f;
}

void Resolver::FireGameEvent(IGameEvent* event) {
    if (!event)
        return;

    if (strcmp(event->GetName(), XORSTR("player_hurt")) == 0) {
        int hurt_player_id = event->GetInt(XORSTR("userid"));
        int attacker_id = event->GetInt(XORSTR("attacker"));

        if (engine->GetPlayerForUserID(hurt_player_id) == engine->GetLocalPlayer())
            return;

        if (engine->GetPlayerForUserID(attacker_id) != engine->GetLocalPlayer())
            return;

        C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
        if (!localplayer)
            return;

        C_BasePlayer *hurt_player = (C_BasePlayer *) entityList->GetClientEntity(
                engine->GetPlayerForUserID(hurt_player_id));
        if (!hurt_player)
            return;

        shotATT = false;

        IEngineClient::player_info_t localPlayerInfo;
        engine->GetPlayerInfo(localplayer->GetIndex(), &localPlayerInfo);

        IEngineClient::player_info_t hurtPlayerInfo;
        engine->GetPlayerInfo(hurt_player->GetIndex(), &hurtPlayerInfo);

        Shotsmissed = 0;
    }

    if (strcmp(event->GetName(), "player_connect_full") != 0 && strcmp(event->GetName(), "cs_game_disconnected") != 0)
        return;

    if (event->GetInt("userid") && engine->GetPlayerForUserID(event->GetInt("userid")) != engine->GetLocalPlayer())
        return;

    Resolver::playerAngleLogs.clear();
}
void Resolver::CreateMove(CUserCmd *cmd)
{
    for (int x = 1; x < engine->GetMaxClients(); ++x)
    {
        C_BasePlayer* target = (C_BasePlayer*) entityList->GetClientEntity(x);

        if (!target
                || target == entityList->GetClientEntity(engine->GetLocalPlayer())
                || target->GetDormant()
                || !target->GetAlive()
                || target->GetImmune()
                || target->GetTeam() == entityList->GetClientEntity(engine->GetLocalPlayer())->GetTeam())
                continue;

        Resolver::StoreVars(target);
        
        if(Settings::Resolver::LagComp)
            StartLagComp(target, cmd);
     
    }
}
