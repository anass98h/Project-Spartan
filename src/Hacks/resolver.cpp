#include "resolver.h"

bool Settings::Resolver::resolveAll = false;
std::vector<int64_t> Resolver::Players = {};
static float slby;
std::vector<std::pair<C_BasePlayer *, QAngle>> player_data;

void Resolver::FrameStageNotify(ClientFrameStage_t stage) {
    if (!engine->IsInGame())
        return;

    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer)
        return;

    if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
        for (int i = 1; i < engine->GetMaxClients(); ++i) {
            C_BasePlayer *player = (C_BasePlayer *) entityList->GetClientEntity(i);

            if (!player
                || player == localplayer
                || player->GetDormant()
                || !player->GetAlive()
                || player->GetImmune()
                || player->GetTeam() == localplayer->GetTeam())
                continue;

            IEngineClient::player_info_t entityInformation;
            engine->GetPlayerInfo(i, &entityInformation);

            if (!Settings::Resolver::resolveAll &&
                std::find(Resolver::Players.begin(), Resolver::Players.end(), entityInformation.xuid) ==
                Resolver::Players.end())
                continue;

            player_data.push_back(std::pair<C_BasePlayer *, QAngle>(player, *player->GetEyeAngles()));


            static bool bFlip = true;
            static float flYaw = *player->GetLowerBodyYawTarget();

            if (player->isMoving()) {
                float flCurTime = globalVars->curtime;
                static float flTimeUpdate = 0.5f;
                static float flNextTimeUpdate = flCurTime + flTimeUpdate;

                if (flCurTime >= flNextTimeUpdate) {
                    bFlip = !bFlip;
                }

                if (flNextTimeUpdate < flCurTime || flNextTimeUpdate - flCurTime > 10.f)
                    flNextTimeUpdate = flCurTime + flTimeUpdate;

                if (bFlip) {
                    flYaw += 35.f;
                } else {
                    flYaw -= 35.f;
                }
                if (slby != *player->GetLowerBodyYawTarget()) {
                    player->GetEyeAngles()->y = flYaw;
                    slby = flYaw;
                } else {

                    player->GetEyeAngles()->y = (*player->GetLowerBodyYawTarget());
                }

            } else {

                // Add some more  awesome Bruteforce stuff and LbyBreaker prevention here x)
                float flCurTime = globalVars->curtime;
                static float Lbyupdi = 1.1f;
                static float nxtLbyup = flCurTime + Lbyupdi;
                bool hasUpdated;
                if (nxtLbyup < flCurTime) { nxtLbyup = flCurTime + Lbyupdi; }


                hasUpdated = !(flCurTime >= nxtLbyup && slby == *player->GetLowerBodyYawTarget());


                if (hasUpdated) {

                    int num = localplayer->GetShotsFired() % 6;
                    switch (num) {
                        case 0:
                            player->GetEyeAngles()->y -= 0.0f;
                            break;
                        case 1:
                            player->GetEyeAngles()->y = 35.0f;
                            break;
                        case 2:
                            player->GetEyeAngles()->y = -70.0f;
                            break;
                        case 3:
                            player->GetEyeAngles()->y = 55.0f;
                            break;
                        case 4:
                            player->GetEyeAngles()->y = -180.0f;
                            break;
                        case 5:
                            player->GetEyeAngles()->y = -30.0f;
                            break;


                    }

                } else {

                    player->GetEyeAngles()->y = *player->GetLowerBodyYawTarget();
                    slby = *player->GetLowerBodyYawTarget();

                }
            }
        }
    } else if (stage == ClientFrameStage_t::FRAME_RENDER_END) {
        for (unsigned long i = 0; i < player_data.size(); i++) {
            std::pair<C_BasePlayer *, QAngle> player_aa_data = player_data[i];
            *player_aa_data.first->GetEyeAngles() = player_aa_data.second;
        }

        player_data.clear();
    }
}

void Resolver::PostFrameStageNotify(ClientFrameStage_t stage) {
}

void Resolver::FireGameEvent(IGameEvent *event) {
    if (!event)
        return;

    if (strcmp(event->GetName(), XORSTR("player_connect_full")) != 0 &&
        strcmp(event->GetName(), XORSTR("cs_game_disconnected")) != 0)
        return;

    if (event->GetInt(XORSTR("userid")) &&
        engine->GetPlayerForUserID(event->GetInt(XORSTR("userid"))) != engine->GetLocalPlayer())
        return;

    Resolver::Players.clear();
}
