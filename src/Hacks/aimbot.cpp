#include "aimbot.h"
#include "autowall.h"

bool Settings::Aimbot::enabled = false;
bool Settings::Aimbot::pSilent = false;
bool Settings::Aimbot::silent = false;
bool Settings::Aimbot::friendly = false;
Bone Settings::Aimbot::bone = Bone::BONE_HEAD;
ButtonCode_t Settings::Aimbot::aimkey = ButtonCode_t::MOUSE_MIDDLE;
bool Settings::Aimbot::aimkeyOnly = false;
bool Settings::Aimbot::Smooth::enabled = false;
float Settings::Aimbot::Smooth::value = 0.5f;
SmoothType Settings::Aimbot::Smooth::type = SmoothType::SLOW_END;
bool Settings::SmartAim::enabled = false;
bool Settings::Aimbot::ErrorMargin::enabled = false;
float Settings::Aimbot::ErrorMargin::value = 0.0f;
bool Settings::Aimbot::AutoAim::enabled = false;
float Settings::Aimbot::AutoAim::fov = 180.0f;
bool Settings::Aimbot::AutoAim::realDistance = false;
bool Settings::Aimbot::AutoAim::closestBone = false;
bool Settings::Aimbot::AutoAim::desiredBones[] =
        {
                true, true, true, true, true, true, true, // center mass
                false, false, false, false, false, false, false, // left arm
                false, false, false, false, false, false, false, // right arm
                false, false, false, false, false, // left leg
                false, false, false, false, false // right leg
        };
bool Settings::Aimbot::AutoAim::engageLock = false;
bool Settings::Aimbot::AutoAim::engageLockTR = false; // engage lock Target Reacquisition ( re-target after getting a kill when spraying ).
int Settings::Aimbot::AutoAim::engageLockTTR = 700; // Time to Target Reacquisition in ms
bool Settings::Aimbot::AutoWall::enabled = false;
float Settings::Aimbot::AutoWall::value = 10.0f;
bool Settings::Aimbot::AimStep::enabled = false;
float Settings::Aimbot::AimStep::min = 25.0f;
float Settings::Aimbot::AimStep::max = 35.0f;
bool Settings::Aimbot::AutoPistol::enabled = false;
bool Settings::Aimbot::AutoShoot::enabled = false;
bool Settings::Aimbot::AutoShoot::autoscope = false;
bool Settings::Aimbot::RCS::enabled = false;
bool Settings::Aimbot::RCS::always_on = false;
float Settings::Aimbot::RCS::valueX = 2.0f;
float Settings::Aimbot::RCS::valueY = 2.0f;
bool Settings::Aimbot::AutoCrouch::enabled = false;
bool Settings::Aimbot::NoShoot::enabled = false;
bool Settings::Aimbot::IgnoreJump::enabled = false;
bool Settings::Aimbot::SmokeCheck::enabled = false;
bool Settings::Aimbot::FlashCheck::enabled = false;
bool Settings::Aimbot::SpreadLimit::enabled = false;
float Settings::Aimbot::SpreadLimit::value = 0.1f;
bool Settings::Aimbot::Smooth::Salting::enabled = false;
float Settings::Aimbot::Smooth::Salting::multiplier = 0.0f;
bool Settings::Aimbot::AutoSlow::enabled = false;
bool Settings::Aimbot::AutoSlow::goingToSlow = false;
bool Settings::Aimbot::Prediction::enabled = false;
bool Settings::Aimbot::moveMouse = false;
bool Settings::Aimbot::HitChance::enabled = false;
int Settings::Aimbot::HitChance::hitRays = 100;
float Settings::Aimbot::HitChance::value = 0.5f;
bool Settings::Aimbot::AutoCockRevolver::enabled = false;
bool Settings::Aimbot::velocityCheck::enabled = false;
bool Settings::Aimbot::backtrack = false;


bool Aimbot::aimStepInProgress = false;
std::vector<int64_t> Aimbot::friends = {};
std::vector<int64_t> Aimbot::baim = {};
std::vector<long> killTimes = {0}; // the Epoch time from when we kill someone

int missedShots = 0;
bool sentShotToTarget;

bool shouldAim;
QAngle AimStepLastAngle;
QAngle RCSLastPunch;

int Aimbot::targetAimbot = -1;
const int HEAD_VECTORS = 11;

static xdo_t *xdo = xdo_new(NULL);

std::unordered_map<ItemDefinitionIndex, AimbotWeapon_t, Util::IntHash<ItemDefinitionIndex>> Settings::Aimbot::weapons =
        {
                {ItemDefinitionIndex::INVALID, {false, false, false, false, false, false, false, 700, Bone::BONE_HEAD, ButtonCode_t::MOUSE_MIDDLE, false, false, 1.0f,
                                                       SmoothType::SLOW_END, false, 0.0f, false, 0.0f, true, 180.0f, false, 25.0f, 35.0f, false, false, 2.0f, 2.0f,
                                                       false, false, false, false, false, false, false, false, 0.1f, false, 10.0f, false, false, 5.0f, false, false, 100, 0.5f, false, false, false}},
        };

static QAngle ApplyErrorToAngle(QAngle *angles, float margin) {
    QAngle error;
    error.Random(-1.0f, 1.0f);
    error *= margin;
    angles->operator+=(error);
    return error;
}

static inline void ApplyOffsetToAngle(QAngle *angles, QAngle *offset) {
    angles->operator+=(*offset);
}

void Aimbot::XDOCleanup() {
    xdo_free(xdo);
}

bool Aimbot::HitChance(const Vector &point, bool teamCheck, C_BasePlayer *localplayer) {
    if (!Settings::Aimbot::HitChance::enabled)
        return true;


    int hitCount = 0;
    for (int i = 0; i < Settings::Aimbot::HitChance::hitRays; i++) {
        Vector dst = point;


        C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *) entityList->GetClientEntityFromHandle(
                localplayer->GetActiveWeapon());
        if (!activeWeapon)
            return false;


        float a = ( float ) M_PI * 2.0f * ( ( float ) ( rand() % 1000 ) / 1000.0f ) - 500;
        float b = activeWeapon->GetSpread() * ( ( float ) ( rand() % 1000 ) / 1000.0f ) * 90.0f - ( float ) M_PI;
        float c = ( float ) M_PI * 2.0f * ( ( float ) ( rand() % 1000 ) / 1000.0f ) - 500;
        float d = activeWeapon->GetInaccuracy() * ( ( float ) ( rand() % 1000 ) / 1000.0f ) * 90.0f - ( float ) M_PI;

        // if you think this is wrong enable console spam and test your new math before pushing an updated -.-
        float range = activeWeapon->GetCSWpnData()->GetRange();
        float a = (float)M_PI * 2.0f * ((float)(rand() % 1000)/1000.0f) ;
  		float b = activeWeapon->GetSpread() * ((float)(rand() % 1000)/1000.0f) * 90.0f; 
  		float c = (float)M_PI * 2.0f * ((float)(rand() % 1000)/1000.0f) ;
  		float d = activeWeapon->GetInaccuracy() * ((float)(rand() % 1000)/1000.0f) * 90.0f; 


        Vector dir, src, dest;
        trace_t tr;
        Ray_t ray;
        CTraceFilterEntitiesOnly filter;

        src = localplayer->GetEyePosition();
        QAngle angles = Math::CalcAngle(src, dst);
        angles.x += (cos(a) * b) + (cos(c) * d);
        angles.y += (sin(a) * b) + (sin(c) * d);
        Math::AngleVectors(angles, dir);

        dest = src + (dir * range); // not sure what this magic 8192 was replaced it with range :eyo:

        ray.Init(src, dest);
        filter.pSkip = localplayer;
        trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

        C_BasePlayer *player = (C_BasePlayer *) tr.m_pEntityHit;
        if (player && player->GetClientClass()->m_ClassID == EClassIds::CCSPlayer && player != localplayer &&
            !player->GetDormant() && player->GetAlive() && !player->GetImmune() &&
            (player->GetTeam() != localplayer->GetTeam() || Settings::Aimbot::friendly))
            hitCount++;
    }

    //cvar->ConsoleDPrintf("HitCount: %d/%d - %f\n", hitCount, Settings::Aimbot::HitChance::hitRays, Settings::Aimbot::HitChance::value);


    return ((float) hitCount / (float) Settings::Aimbot::HitChance::hitRays > Settings::Aimbot::HitChance::value);
}

/* Fills points Vector. True if successful. False if not.  Credits for Original method - ReactiioN */
static bool HeadMultiPoint(C_BasePlayer *player, Vector points[]) {
    matrix3x4_t matrix[128];

    if (!player->SetupBones(matrix, 128, 0x100, 0.f))
        return false;
    model_t *pModel = player->GetModel();
    if (!pModel)
        return false;

    studiohdr_t *hdr = modelInfo->GetStudioModel(pModel);
    if (!hdr)
        return false;
    mstudiobbox_t *bbox = hdr->pHitbox((int) Hitbox::HITBOX_HEAD, 0);
    if (!bbox)
        return false;

    Vector mins, maxs;
    Math::VectorTransform(bbox->bbmin, matrix[bbox->bone], mins);
    Math::VectorTransform(bbox->bbmax, matrix[bbox->bone], maxs);

    Vector center = (mins + maxs) * 0.5f;
    // 0 - center, 1 - forehead, 2 - skullcap, 3 - upperleftear, 4 - upperrightear, 5 - uppernose, 6 - upperbackofhead
    // 7 - leftear, 8 - rightear, 9 - nose, 10 - backofhead
    for (int i = 0; i < HEAD_VECTORS; i++) // set all points initially to center mass of head.
        points[i] = center;
    points[1].z += bbox->radius * 0.60f; // morph each point.
    points[2].z += bbox->radius * 1.25f; // ...
    points[3].x += bbox->radius * 0.80f;
    points[3].z += bbox->radius * 0.60f;
    points[4].x -= bbox->radius * 0.80f;
    points[4].z += bbox->radius * 0.90f;
    points[5].y += bbox->radius * 0.80f;
    points[5].z += bbox->radius * 0.90f;
    points[6].y -= bbox->radius * 0.80f;
    points[6].z += bbox->radius * 0.90f;
    points[7].x += bbox->radius * 0.80f;
    points[8].x -= bbox->radius * 0.80f;
    points[9].y += bbox->radius * 0.80f;
    points[10].y -= bbox->radius * 0.80f;

    return true;
}

static float AutoWallBestSpot(C_BasePlayer *player, Vector &bestSpot) {
    float bestDamage = Settings::Aimbot::AutoWall::value;
    const std::map<int, int> *modelType = Util::GetModelTypeBoneMap(player);

    static int len =
            sizeof(Settings::Aimbot::AutoAim::desiredBones) / sizeof(Settings::Aimbot::AutoAim::desiredBones[0]);

    for (int i = 0; i < len; i++) {
        if (!Settings::Aimbot::AutoAim::desiredBones[i])
            continue;
        if (i == (int) DesiredBones::BONE_HEAD) // head multipoint
        {
            Vector headPoints[HEAD_VECTORS];
            if (!HeadMultiPoint(player, headPoints))
                continue;
            for (int j = 0; j < HEAD_VECTORS; j++) {
                Autowall::FireBulletData data;
                float spotDamage = Autowall::GetDamage(headPoints[j], !Settings::Aimbot::friendly, data);
                if (spotDamage > bestDamage) {
                    bestSpot = headPoints[j];
                    if (spotDamage > player->GetHealth())
                        return spotDamage;
                    bestDamage = spotDamage;
                }
            }
        }
        int boneID = (*modelType).at(i);
        if (boneID == (int) Bone::INVALID) // bone not available on this modeltype.
            continue;

        Vector bone3D = player->GetBonePosition(boneID);

        Autowall::FireBulletData data;
        float boneDamage = Autowall::GetDamage(bone3D, !Settings::Aimbot::friendly, data);
        if (boneDamage > bestDamage) {
            bestSpot = bone3D;
            if (boneDamage > player->GetHealth())
                return boneDamage;

            bestDamage = boneDamage;
        }
    }
    return bestDamage;
}

static float GetRealDistanceFOV(float distance, QAngle angle, CUserCmd *cmd) {
    /*    n
        w + e
          s        'real distance'
                          |
       a point -> x --..  v
                  |     ''-- x <- a guy
                  |          /
                 |         /
                 |       /
                | <------------ both of these lines are the same length
                |    /      /
               |   / <-----'
               | /
              o
         localplayer
     */

    Vector aimingAt;
    Math::AngleVectors(cmd->viewangles, aimingAt);
    aimingAt *= distance;

    Vector aimAt;
    Math::AngleVectors(angle, aimAt);
    aimAt *= distance;

    return aimingAt.DistTo(aimAt);
}

static Vector VelocityExtrapolate(C_BasePlayer *player, Vector aimPos) {
    return aimPos + (player->GetVelocity() * globalVars->interval_per_tick);
}

/* Original Credits to: https://github.com/goldenguy00 ( study! study! study! :^) ) */
static Vector GetClosestSpot(CUserCmd *cmd, C_BasePlayer *localPlayer, C_BasePlayer *enemy,
                             AimTargetType aimTargetType = AimTargetType::FOV) {
    QAngle viewAngles;
    engine->GetViewAngles(viewAngles);

    float tempFov = Settings::Aimbot::AutoAim::fov;
    float tempDistance = Settings::Aimbot::AutoAim::fov * 5.f;

    Vector pVecTarget = localPlayer->GetEyePosition();

    Vector tempSpot = {0, 0, 0};

    const std::map<int, int> *modelType = Util::GetModelTypeBoneMap(enemy);

    static int len =
            sizeof(Settings::Aimbot::AutoAim::desiredBones) / sizeof(Settings::Aimbot::AutoAim::desiredBones[0]);
    for (int i = 0; i < len; i++) {
        if (!Settings::Aimbot::AutoAim::desiredBones[i])
            continue;

        int boneID = (*modelType).at(i);
        if (boneID == (int) Bone::INVALID)
            continue;

        Vector cbVecTarget = enemy->GetBonePosition(boneID);

        if (aimTargetType == AimTargetType::FOV) {
            float cbFov = Math::GetFov(viewAngles, Math::CalcAngle(pVecTarget, cbVecTarget));

            if (cbFov < tempFov) {
                if (Entity::IsVisibleThroughEnemies(enemy, boneID)) {
                    tempFov = cbFov;
                    tempSpot = cbVecTarget;
                }
            }
        } else if (aimTargetType == AimTargetType::REAL_DISTANCE) {
            float cbDistance = pVecTarget.DistTo(cbVecTarget);
            float cbRealDistance = GetRealDistanceFOV(cbDistance, Math::CalcAngle(pVecTarget, cbVecTarget), cmd);

            if (cbRealDistance < tempDistance) {
                if (Entity::IsVisibleThroughEnemies(enemy, boneID)) {
                    tempDistance = cbRealDistance;
                    tempSpot = cbVecTarget;
                }
            }
        }
    }
    return tempSpot;
}

static Vector GetClosestBacktrack(CUserCmd *cmd, C_BasePlayer *localPlayer, C_BasePlayer *enemy, int *tickDiff) {
    QAngle viewAngles;
    engine->GetViewAngles(viewAngles);
    // FOV Style Closest
    float tempFov = Settings::Aimbot::AutoAim::fov;
    Vector bestSpot = {0, 0, 0};
    Vector pVecTarget = localPlayer->GetEyePosition();
    for (int i = 1; i < Backtracking::lagRecords[enemy->GetIndex()].size(); i++) {
        float cbFov = Math::GetFov(viewAngles, Math::CalcAngle(pVecTarget,
                                                               Backtracking::lagRecords[enemy->GetIndex()][i].headPos));
        if (cbFov < tempFov) {
            tempFov = cbFov;
            bestSpot = Backtracking::lagRecords[enemy->GetIndex()][i].headPos;
            *tickDiff = i;
        }
    }
    return bestSpot;
}

static C_BasePlayer *
GetClosestPlayer(CUserCmd *cmd, bool visibleCheck, Vector &bestSpot, float &bestDamage, int *tickDiff,
                 AimTargetType aimTargetType = AimTargetType::FOV) {
    if (Settings::Aimbot::AutoAim::realDistance)
        aimTargetType = AimTargetType::REAL_DISTANCE;

    static C_BasePlayer *lockedOn = NULL;
    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    C_BasePlayer *closestEntity = NULL;

    float bestFov = Settings::Aimbot::AutoAim::fov;
    float bestRealDistance = Settings::Aimbot::AutoAim::fov * 5.f;

    if (lockedOn) {
        if (lockedOn->GetAlive() && !Settings::Aimbot::AutoAim::closestBone &&
            !Entity::IsSpotVisibleThroughEnemies(lockedOn,
                                                 lockedOn->GetBonePosition((int) Settings::Aimbot::bone))) {
            lockedOn = NULL;
            return NULL;
        }
        if (!(cmd->buttons & IN_ATTACK || inputSystem->IsButtonDown(Settings::Aimbot::aimkey)) ||
            lockedOn->GetDormant())//|| !Entity::IsVisible(lockedOn, bestBone, 180.f, Settings::ESP::Filters::smokeCheck))
        {
            lockedOn = NULL;
        } else {
            if (!lockedOn->GetAlive()) {
                if (Settings::Aimbot::AutoAim::engageLockTR) {
                    if (Util::GetEpochTime() - killTimes.back() >
                        Settings::Aimbot::AutoAim::engageLockTTR) // if we got the kill over the TTR time, engage another foe.
                    {
                        lockedOn = NULL;
                    }
                }
                return NULL;
            }

            if (Settings::Aimbot::AutoAim::closestBone) {
                Vector tempSpot = GetClosestSpot(cmd, localplayer, lockedOn, aimTargetType);
                if (tempSpot.x == 0 && tempSpot.y == 0 && tempSpot.z == 0) {
                    return NULL;
                }
                bestSpot = tempSpot;
            } else {
                bestSpot = lockedOn->GetBonePosition((int) Settings::Aimbot::bone);
            }

            return lockedOn;
        }
    }

    for (int i = 1; i < engine->GetMaxClients(); ++i) {
        C_BasePlayer *player = (C_BasePlayer *) entityList->GetClientEntity(i);

        if (!player
            || player == localplayer
            || player->GetDormant()
            || !player->GetAlive()
            || player->GetImmune())
            continue;

        if (!Settings::Aimbot::friendly && player->GetTeam() == localplayer->GetTeam())
            continue;

        if (!Aimbot::friends.empty()) // check for friends, if any
        {
            IEngineClient::player_info_t entityInformation;
            engine->GetPlayerInfo(i, &entityInformation);

            if (std::find(Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid) !=
                Aimbot::friends.end())
                continue;
        }

        Aimbot::targetAimbot = i;

        Vector eVecTarget = player->GetBonePosition((int) Settings::Aimbot::bone);

        if (Settings::Aimbot::AutoAim::closestBone) {
            Vector tempSpot = GetClosestSpot(cmd, localplayer, player, aimTargetType);
            if ((tempSpot.x == 0 && tempSpot.y == 0 && tempSpot.z == 0) ||
                !Entity::IsSpotVisibleThroughEnemies(player, tempSpot))
                continue;
            eVecTarget = tempSpot;
        } else if (Settings::Aimbot::backtrack) {
            int tempTickDiff = -1;
            Vector spot = GetClosestBacktrack(cmd, localplayer, player, &tempTickDiff);
            if ((spot.x == 0 && spot.y == 0 && spot.z == 0) || tempTickDiff == -1)
                continue;
            eVecTarget = spot;
            *tickDiff = tempTickDiff;
        }

        if (Settings::SmartAim::enabled) {
            if (missedShots <= 5) {
                eVecTarget = player->GetBonePosition((int) Bone::BONE_MIDDLE_SPINAL_COLUMN);
                //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "BAIM TIME! We missed %i shots.\n", missedShots);

                // Maybe introduce a SmartAim counting damage and not just shots
            } else {
                //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Missed Shots: %i\n", missedShots);
            }
        }

        if (!Aimbot::baim.empty()) {
            //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Prefer Baim List contains xUIDs:\n");
            /*for(int64_t &xuid : Aimbot::baim) {
                cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "- %i\n", xuid);
            }*/

            IEngineClient::player_info_t entityInformation;
            engine->GetPlayerInfo(i, &entityInformation);

            if (std::find(Aimbot::baim.begin(), Aimbot::baim.end(), entityInformation.xuid) != Aimbot::baim.end()) {
                //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Baiming current target because its in the Baim List.\n", missedShots);
                eVecTarget = player->GetBonePosition((int) Bone::BONE_MIDDLE_SPINAL_COLUMN);

            }
        } else {
            //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Prefer Baim List is empty.\n");
        }


        Vector pVecTarget = localplayer->GetEyePosition();

        QAngle viewAngles;
        engine->GetViewAngles(viewAngles);

        float distance = pVecTarget.DistTo(eVecTarget);
        float fov = Math::GetFov(viewAngles, Math::CalcAngle(pVecTarget, eVecTarget));

        if (aimTargetType == AimTargetType::FOV && fov > bestFov)
            continue;

        float realDistance = GetRealDistanceFOV(distance, Math::CalcAngle(pVecTarget, eVecTarget), cmd);

        if (aimTargetType == AimTargetType::REAL_DISTANCE && realDistance > bestRealDistance)
            continue;

        if (visibleCheck && !Settings::Aimbot::AutoWall::enabled && !Settings::Aimbot::backtrack &&
            !Entity::IsSpotVisible(player, eVecTarget))
            continue;

        if (Settings::Aimbot::AutoWall::enabled) {
            Vector wallBangSpot = {0, 0, 0};
            float damage = AutoWallBestSpot(player,
                                            wallBangSpot); // sets Vector Angle, returns damage of hitting that spot.

            if (wallBangSpot.x != 0 && wallBangSpot.y != 0 && wallBangSpot.z != 0) {
                bestDamage = damage;
                bestSpot = wallBangSpot;
                closestEntity = player;
            }
        } else {
            closestEntity = player;
            bestSpot = eVecTarget;
            bestFov = fov;
            bestRealDistance = realDistance;
        }
    }
    if (Settings::Aimbot::AutoAim::engageLock) {
        if (!lockedOn) {
            if ((cmd->buttons & IN_ATTACK) || inputSystem->IsButtonDown(Settings::Aimbot::aimkey)) {
                if (Util::GetEpochTime() - killTimes.back() > 100) // if we haven't gotten a kill in under 100ms.
                {
                    lockedOn = closestEntity; // This is to prevent a Rare condition when you one-tap someone without the aimbot, it will lock on to another target.
                }
            } else {
                return NULL;
            }
        }
    }
    if (bestSpot.x == 0 && bestSpot.y == 0 && bestSpot.z == 0)
        return NULL;

    /*
    if( closestEntity )
    {
            IEngineClient::player_info_t playerInfo;
            engine->GetPlayerInfo(closestEntity->GetIndex(), &playerInfo);
            cvar->ConsoleDPrintf("%s is Closest.\n", playerInfo.name);
    }
    */

    return closestEntity;
}

static void RCS(QAngle &angle, C_BasePlayer *player, CUserCmd *cmd) {
    if (!Settings::Aimbot::RCS::enabled)
        return;

    if (!(cmd->buttons & IN_ATTACK))
        return;

    bool hasTarget = Settings::Aimbot::AutoAim::enabled && shouldAim && player;

    if (!Settings::Aimbot::RCS::always_on && !hasTarget)
        return;

    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    QAngle CurrentPunch = *localplayer->GetAimPunchAngle();

    if (Settings::Aimbot::silent || hasTarget) {
        angle.x -= CurrentPunch.x * Settings::Aimbot::RCS::valueX;
        angle.y -= CurrentPunch.y * Settings::Aimbot::RCS::valueY;
    } else if (localplayer->GetShotsFired() > 1) {
        QAngle NewPunch = {CurrentPunch.x - RCSLastPunch.x, CurrentPunch.y - RCSLastPunch.y, 0};

        angle.x -= NewPunch.x * Settings::Aimbot::RCS::valueX;
        angle.y -= NewPunch.y * Settings::Aimbot::RCS::valueY;
    }

    RCSLastPunch = CurrentPunch;
}

void Aimbot::AutoCockRevolver(C_BaseCombatWeapon *activeWeapon, C_BasePlayer *localplayer, CUserCmd *cmd) {
    if (!Settings::Aimbot::AutoCockRevolver::enabled)
        return;

    if (cmd->buttons & IN_RELOAD)
        return;

    if (*activeWeapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER)
        return;
      static int timer = 0;
      timer++;

      if ( timer <= 15 )
          cmd->buttons |= IN_ATTACK;

      else
          timer = 0;


}


static void AimStep(C_BasePlayer *player, QAngle &angle, CUserCmd *cmd) {
    if (!Settings::Aimbot::AimStep::enabled)
        return;

    if (!Settings::Aimbot::AutoAim::enabled)
        return;

    if (Settings::Aimbot::Smooth::enabled)
        return;

    if (!shouldAim)
        return;

    if (!Aimbot::aimStepInProgress)
        AimStepLastAngle = cmd->viewangles;

    if (!player)
        return;

    float fov = Math::GetFov(AimStepLastAngle, angle);

    Aimbot::aimStepInProgress = (fov > (Math::float_rand(Settings::Aimbot::AimStep::min,
                                                         Settings::Aimbot::AimStep::max)));

    if (!Aimbot::aimStepInProgress)
        return;

    QAngle deltaAngle = AimStepLastAngle - angle;

    Math::NormalizeAngles(deltaAngle);
    float randX = Math::float_rand(Settings::Aimbot::AimStep::min, std::min(Settings::Aimbot::AimStep::max, fov));
    float randY = Math::float_rand(Settings::Aimbot::AimStep::min, std::min(Settings::Aimbot::AimStep::max, fov));
    if (deltaAngle.y < 0)
        AimStepLastAngle.y += randY;
    else
        AimStepLastAngle.y -= randY;

    if (deltaAngle.x < 0)
        AimStepLastAngle.x += randX;
    else
        AimStepLastAngle.x -= randX;

    angle = AimStepLastAngle;
}

static void Salt(float &smooth) {
    float sine = sin(globalVars->tickcount);
    float salt = sine * Settings::Aimbot::Smooth::Salting::multiplier;
    float oval = smooth + salt;
    smooth *= oval;
}

static void Smooth(C_BasePlayer *player, QAngle &angle, CUserCmd *cmd) {
    if (!Settings::Aimbot::Smooth::enabled)
        return;

    if (Settings::AntiAim::Pitch::enabled || Settings::AntiAim::Yaw::enabled)
        return;

    if (!shouldAim || !player)
        return;

    if (Settings::Aimbot::silent)
        return;

    QAngle viewAngles;
    engine->GetViewAngles(viewAngles);

    QAngle delta = angle - viewAngles;
    Math::NormalizeAngles(delta);

    float smooth = powf(Settings::Aimbot::Smooth::value, 0.4f); // Makes more slider space for actual useful values

    smooth = std::min(0.99f, smooth);

    if (Settings::Aimbot::Smooth::Salting::enabled)
        Salt(smooth);

    QAngle toChange = QAngle();

    SmoothType type = Settings::Aimbot::Smooth::type;

    if (type == SmoothType::SLOW_END)
        toChange = delta - (delta * smooth);
    else if (type == SmoothType::CONSTANT || type == SmoothType::FAST_END) {
        float coeff = (1.0f - smooth) / delta.Length() * 4.f;

        if (type == SmoothType::FAST_END)
            coeff = powf(coeff, 2.f) * 10.f;

        coeff = std::min(1.f, coeff);
        toChange = delta * coeff;
    }

    angle = viewAngles + toChange;
}

static void AutoCrouch(C_BasePlayer *player, CUserCmd *cmd) {
    if (!Settings::Aimbot::AutoCrouch::enabled)
        return;

    if (!player)
        return;

    cmd->buttons |= IN_DUCK;
}

static void
AutoSlow(C_BasePlayer *player, float &forward, float &sideMove, float &bestDamage, C_BaseCombatWeapon *active_weapon,
         CUserCmd *cmd) {

    if (!Settings::Aimbot::AutoSlow::enabled) {
        Settings::Aimbot::AutoSlow::goingToSlow = false;
        return;
    }

    if (!player) {
        Settings::Aimbot::AutoSlow::goingToSlow = false;
        return;
    }

    float nextPrimaryAttack = active_weapon->GetNextPrimaryAttack();

    if (nextPrimaryAttack > globalVars->curtime) {
        Settings::Aimbot::AutoSlow::goingToSlow = false;
        return;
    }

    Settings::Aimbot::AutoSlow::goingToSlow = true;

    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *) entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon());
    if (!activeWeapon || activeWeapon->GetAmmo() == 0)
        return;

    if (Settings::Aimbot::SpreadLimit::enabled) {
        if ((activeWeapon->GetSpread() + activeWeapon->GetInaccuracy()) > Settings::Aimbot::SpreadLimit::value) {
            cmd->buttons |= IN_WALK;
            forward = -forward;
            sideMove = -sideMove;
            cmd->upmove = 0;
        }
    } else if (localplayer->GetVelocity().Length() >
               (activeWeapon->GetCSWpnData()->GetMaxPlayerSpeed() / 3)) // https://youtu.be/ZgjYxBRuagA
    {
        cmd->buttons |= IN_WALK;
        forward = -forward;
        sideMove = -sideMove;
        cmd->upmove = 0;
    }
}

static void AutoPistol(C_BaseCombatWeapon *activeWeapon, CUserCmd *cmd) {
    if (!Settings::Aimbot::AutoPistol::enabled)
        return;

    if (!activeWeapon || activeWeapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_PISTOL)
        return;

    if (activeWeapon->GetNextPrimaryAttack() < globalVars->curtime)
        return;

    if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
        cmd->buttons &= ~IN_ATTACK2;
    else
        cmd->buttons &= ~IN_ATTACK;
}

static void AutoShoot(C_BasePlayer *player, Vector spot, C_BaseCombatWeapon *activeWeapon, CUserCmd *cmd) {
    if (!Settings::Aimbot::AutoShoot::enabled)
        return;

    if (Settings::Aimbot::AimStep::enabled && Aimbot::aimStepInProgress)
        return;

    if (!player || activeWeapon->GetAmmo() == 0)
        return;

    CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
    if (weaponType == CSWeaponType::WEAPONTYPE_KNIFE || weaponType == CSWeaponType::WEAPONTYPE_C4 ||
        weaponType == CSWeaponType::WEAPONTYPE_GRENADE)
        return;

    if (cmd->buttons & IN_USE)
        return;

    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());

    if (Settings::Aimbot::AutoShoot::autoscope && activeWeapon->GetCSWpnData()->GetZoomLevels() > 0 && !localplayer->IsScoped())

        cmd->buttons |= IN_ATTACK2;

    if (Settings::Aimbot::velocityCheck::enabled &&
        localplayer->GetVelocity().Length() > (activeWeapon->GetCSWpnData()->GetMaxPlayerSpeed() / 3))
        return;
    if (Settings::Aimbot::SpreadLimit::enabled &&
        ((activeWeapon->GetSpread() + activeWeapon->GetInaccuracy()) > Settings::Aimbot::SpreadLimit::value))
        return;

    if (Settings::Aimbot::HitChance::enabled && !Aimbot::HitChance(spot, !Settings::Aimbot::friendly, localplayer))
        return;

    float nextPrimaryAttack = activeWeapon->GetNextPrimaryAttack();

    if (nextPrimaryAttack > globalVars->curtime) {
        if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
            cmd->buttons &= ~IN_ATTACK2;
        else

            cmd->buttons &= ~IN_ATTACK;
    } else {
        if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
            cmd->buttons |= IN_ATTACK2;
        else if (Settings::Aimbot::AutoShoot::autoscope && activeWeapon->GetCSWpnData()->GetZoomLevels() > 0 && !localplayer->IsScoped()) {
            cmd->buttons &= ~IN_ATTACK;
        } else {
            cmd->buttons |= IN_ATTACK;
        }
    }

    if (Settings::SmartAim::enabled) {
        if (sentShotToTarget) {
            missedShots++;
            cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "WE MISSED A SHOT! Missed Shots: " + missedShots);
            sentShotToTarget = false;
        } else {
            cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Hitted Shots. Missed Shots: " + missedShots);
        }

        sentShotToTarget = true;
    }
}

static void ShootCheck(C_BaseCombatWeapon *activeWeapon, CUserCmd *cmd) {
    if (!Settings::AntiAim::Pitch::enabled && !Settings::AntiAim::Yaw::enabled)
        return;

    if (!Settings::Aimbot::silent)
        return;

    if (!(cmd->buttons & IN_ATTACK))
        return;

    if (activeWeapon->GetNextPrimaryAttack() < globalVars->curtime)
        return;

    if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_C4)
        return;

    if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
        cmd->buttons &= ~IN_ATTACK2;
    else
        cmd->buttons &= ~IN_ATTACK;
}

static void NoShoot(C_BaseCombatWeapon *activeWeapon, C_BasePlayer *player, CUserCmd *cmd) {
    if (player && Settings::Aimbot::NoShoot::enabled) {
        if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_C4)
            return;

        if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
            cmd->buttons &= ~IN_ATTACK2;
        else
            cmd->buttons &= ~IN_ATTACK;
    }
}

void Aimbot::CreateMove(CUserCmd *cmd) {
    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer || !localplayer->GetAlive())
        return;

    Aimbot::UpdateValues();

    if (!Settings::Aimbot::enabled)
        return;

    QAngle oldAngle;
    engine->GetViewAngles(oldAngle);
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;

    QAngle angle = cmd->viewangles;
    static bool newTarget = true;
    static QAngle lastRandom = {0, 0, 0};

    shouldAim = Settings::Aimbot::AutoShoot::enabled;

    if (Settings::Aimbot::IgnoreJump::enabled && !(localplayer->GetFlags() & FL_ONGROUND))
        return;

    C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *) entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon());
    if (!activeWeapon || activeWeapon->GetInReload())
        return;

    CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
    if (weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE ||
        weaponType == CSWeaponType::WEAPONTYPE_KNIFE)
        return;

    Vector aimSpot = {0, 0, 0};
    float bestDamage = 0.0f;
    int tickDiff = -1;
    C_BasePlayer *player = GetClosestPlayer(cmd, true, aimSpot, bestDamage, &tickDiff);

    if (player) {
        bool skipPlayer = false;

        Vector eVecTarget = aimSpot;
        Vector pVecTarget = localplayer->GetEyePosition();

        if (Settings::Aimbot::SmokeCheck::enabled && LineGoesThroughSmoke(pVecTarget, eVecTarget, true))
            skipPlayer = true;

        if (Settings::Aimbot::FlashCheck::enabled && localplayer->GetFlashBangTime() - globalVars->curtime > 2.0f)
            skipPlayer = true;

        if (skipPlayer)
            player = nullptr;

        if (Settings::Aimbot::AutoAim::enabled && !skipPlayer) {
            if (cmd->buttons & IN_ATTACK && !Settings::Aimbot::aimkeyOnly)
                shouldAim = true;

            if (inputSystem->IsButtonDown(Settings::Aimbot::aimkey))
                shouldAim = true;

            if (shouldAim) {
                //IEngineClient::player_info_t playerInfo;
                //engine->GetPlayerInfo(player->GetIndex(), &playerInfo);
                //studiohdr_t* pStudioModel = modelInfo->GetStudioModel(player->GetModel());
                //cvar->ConsoleDPrintf("Model ID: %d, Name: %s, NumOfBones: %d, len %d\n", pStudioModel->id, pStudioModel->name, pStudioModel->numbones);
                //cvar->ConsoleDPrintf("Aiming at: %s, he is a %s \n", playerInfo.name, Util::ModelTypeToString(Util::GetModelTypeID(player)).c_str());

                if (Settings::Aimbot::backtrack && tickDiff != -1) {
                    cmd->tick_count -= (16 - tickDiff);
                }
                if (Settings::Aimbot::Prediction::enabled) {
                    pVecTarget = VelocityExtrapolate(localplayer, pVecTarget); // get eye pos next tick
                    eVecTarget = VelocityExtrapolate(player, eVecTarget); // get target pos next tick
                }
                angle = Math::CalcAngle(pVecTarget, eVecTarget);

                //cvar->ConsoleDPrintf("Raw Angle = (%.2f, %.2f, %.2f)\n", angle.x, angle.y, angle.z);

                if (Settings::Aimbot::ErrorMargin::enabled) {
                    static int lastShotFired = 0;
                    if ((localplayer->GetShotsFired() > lastShotFired) ||
                        newTarget)//get new random spot when firing a shot or when aiming at a new target
                    {
                        lastRandom = ApplyErrorToAngle(&angle, Settings::Aimbot::ErrorMargin::value);
                    }

                    if (lastRandom.x != 0 && lastRandom.y != 0 && lastRandom.z != 0) {
                        ApplyOffsetToAngle(&angle, &lastRandom);
                    }

                    lastShotFired = localplayer->GetShotsFired();
                }
                newTarget = false;
            } else {
                newTarget = true;
                lastRandom = {0, 0, 0};
            }
        }
    } else {
        newTarget = true;
        lastRandom = {0, 0, 0};
    }

    AimStep(player, angle, cmd);
    AutoCrouch(player, cmd);
    AutoSlow(player, oldForward, oldSideMove, bestDamage, activeWeapon, cmd);
    AutoPistol(activeWeapon, cmd);
    AutoShoot(player, aimSpot, activeWeapon, cmd);
    RCS(angle, player, cmd);
    Smooth(player, angle, cmd);
    ShootCheck(activeWeapon, cmd);
    NoShoot(activeWeapon, player, cmd);
    AutoCockRevolver(activeWeapon, player, cmd);

    Math::NormalizeAngles(angle);
    Math::ClampAngles(angle);

    if (angle != cmd->viewangles) {
        if (Settings::Aimbot::moveMouse && !Settings::Aimbot::silent &&
            !Settings::Aimbot::pSilent) // not a good idea to use mouse-based aimbot and silent aim at the same time
        {
            float sensitivity = cvar->FindVar(XORSTR("sensitivity"))->GetFloat();
            float m_pitch = cvar->FindVar(XORSTR("m_pitch"))->GetFloat();
            float m_yaw = cvar->FindVar(XORSTR("m_yaw"))->GetFloat();

            xdo_move_mouse_relative(xdo, (int) -((angle.y - oldAngle.y) / (m_pitch * sensitivity)),
                                    (int) ((angle.x - oldAngle.x) / (m_yaw * sensitivity))
            );
        } else {
            cmd->viewangles = angle;
        }
    }

    Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);

    bool bulletTime = true;

    if (activeWeapon->GetNextPrimaryAttack() > globalVars->curtime)
        bulletTime = false;

    FakeLag::pSilent = false;

    if (Settings::Aimbot::pSilent) {
        if (cmd->buttons & IN_ATTACK && bulletTime) {
            FakeLag::pSilent = true;
            CreateMove::sendPacket = false;
        } else {
            cmd->buttons &= ~IN_ATTACK; // hands up, don't shoot.
        }
    }

    if (!Settings::Aimbot::silent)
        engine->SetViewAngles(cmd->viewangles);


}

void Aimbot::FireGameEvent(IGameEvent *event) {
    if (!event)
        return;

    if (strcmp(event->GetName(), XORSTR("player_connect_full")) == 0 ||
        strcmp(event->GetName(), XORSTR("cs_game_disconnected")) == 0) {
        if (event->GetInt(XORSTR("userid")) &&
            engine->GetPlayerForUserID(event->GetInt(XORSTR("userid"))) != engine->GetLocalPlayer())
            return;
        Aimbot::friends.clear();
    }
    if (strcmp(event->GetName(), XORSTR("player_death")) == 0) {
        int attacker_id = engine->GetPlayerForUserID(event->GetInt(XORSTR("attacker")));
        int deadPlayer_id = engine->GetPlayerForUserID(event->GetInt(XORSTR("userid")));

        if (deadPlayer_id == engine->GetLocalPlayer())
            missedShots = 0;

        if (attacker_id == deadPlayer_id) // suicide
            return;

        if (attacker_id != engine->GetLocalPlayer())
            return;

        killTimes.push_back(Util::GetEpochTime());
    }

    if (Settings::SmartAim::enabled) {
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

            sentShotToTarget = false;

            IEngineClient::player_info_t localPlayerInfo;
            engine->GetPlayerInfo(localplayer->GetIndex(), &localPlayerInfo);

            IEngineClient::player_info_t hurtPlayerInfo;
            engine->GetPlayerInfo(hurt_player->GetIndex(), &hurtPlayerInfo);

            missedShots = 0;

            cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Received hurt event. Attacker: %s HurtPlayer: %s\n",
                                     localPlayerInfo.name, hurtPlayerInfo.name);
        }
    }
}

void Aimbot::UpdateValues() {
    C_BasePlayer *localplayer = (C_BasePlayer *) entityList->GetClientEntity(engine->GetLocalPlayer());
    C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *) entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon());
    if (!activeWeapon)
        return;

    ItemDefinitionIndex index = ItemDefinitionIndex::INVALID;
    if (Settings::Aimbot::weapons.find(*activeWeapon->GetItemDefinitionIndex()) != Settings::Aimbot::weapons.end())
        index = *activeWeapon->GetItemDefinitionIndex();

    const AimbotWeapon_t &currentWeaponSetting = Settings::Aimbot::weapons.at(index);

    Settings::Aimbot::enabled = currentWeaponSetting.enabled;
    Settings::Aimbot::silent = currentWeaponSetting.silent;
    Settings::Aimbot::pSilent = currentWeaponSetting.pSilent;
    Settings::Aimbot::backtrack = currentWeaponSetting.backtrack;
    Settings::Aimbot::friendly = currentWeaponSetting.friendly;
    Settings::Aimbot::bone = currentWeaponSetting.bone;
    Settings::Aimbot::aimkey = currentWeaponSetting.aimkey;
    Settings::Aimbot::aimkeyOnly = currentWeaponSetting.aimkeyOnly;
    Settings::Aimbot::Smooth::enabled = currentWeaponSetting.smoothEnabled;
    Settings::Aimbot::Smooth::value = currentWeaponSetting.smoothAmount;
    Settings::Aimbot::Smooth::type = currentWeaponSetting.smoothType;
    Settings::Aimbot::ErrorMargin::enabled = currentWeaponSetting.errorMarginEnabled;
    Settings::Aimbot::ErrorMargin::value = currentWeaponSetting.errorMarginValue;
    Settings::Aimbot::AutoAim::enabled = currentWeaponSetting.autoAimEnabled;
    Settings::Aimbot::AutoAim::fov = currentWeaponSetting.autoAimFov;
    Settings::Aimbot::AutoAim::closestBone = currentWeaponSetting.closestBone;
    Settings::Aimbot::AutoAim::engageLock = currentWeaponSetting.engageLock;
    Settings::Aimbot::AutoAim::engageLockTR = currentWeaponSetting.engageLockTR;
    Settings::Aimbot::AutoAim::engageLockTTR = currentWeaponSetting.engageLockTTR;
    Settings::Aimbot::AimStep::enabled = currentWeaponSetting.aimStepEnabled;
    Settings::Aimbot::AimStep::min = currentWeaponSetting.aimStepMin;
    Settings::Aimbot::AimStep::max = currentWeaponSetting.aimStepMax;
    Settings::Aimbot::AutoPistol::enabled = currentWeaponSetting.autoPistolEnabled;
    Settings::Aimbot::AutoShoot::enabled = currentWeaponSetting.autoShootEnabled;
    Settings::Aimbot::AutoShoot::autoscope = currentWeaponSetting.autoScopeEnabled;
    Settings::Aimbot::RCS::enabled = currentWeaponSetting.rcsEnabled;
    Settings::Aimbot::RCS::always_on = currentWeaponSetting.rcsAlwaysOn;
    Settings::Aimbot::RCS::valueX = currentWeaponSetting.rcsAmountX;
    Settings::Aimbot::RCS::valueY = currentWeaponSetting.rcsAmountY;
    Settings::Aimbot::NoShoot::enabled = currentWeaponSetting.noShootEnabled;
    Settings::Aimbot::IgnoreJump::enabled = currentWeaponSetting.ignoreJumpEnabled;
    Settings::Aimbot::Smooth::Salting::enabled = currentWeaponSetting.smoothSaltEnabled;
    Settings::Aimbot::Smooth::Salting::multiplier = currentWeaponSetting.smoothSaltMultiplier;
    Settings::Aimbot::SmokeCheck::enabled = currentWeaponSetting.smokeCheck;
    Settings::Aimbot::FlashCheck::enabled = currentWeaponSetting.flashCheck;
    Settings::Aimbot::SpreadLimit::enabled = currentWeaponSetting.spreadLimitEnabled;
    Settings::Aimbot::SpreadLimit::value = currentWeaponSetting.spreadLimit;
    Settings::Aimbot::AutoWall::enabled = currentWeaponSetting.autoWallEnabled;
    Settings::Aimbot::AutoWall::value = currentWeaponSetting.autoWallValue;
    Settings::Aimbot::AutoSlow::enabled = currentWeaponSetting.autoSlow;
    Settings::Aimbot::HitChance::value = currentWeaponSetting.hitChanceValue;
    Settings::Aimbot::HitChance::hitRays = currentWeaponSetting.hitChanceRays;
    Settings::Aimbot::HitChance::enabled = currentWeaponSetting.hitChanceEnabled;
    Settings::Aimbot::AutoCockRevolver::enabled = currentWeaponSetting.autoCockRevolver;
    Settings::Aimbot::velocityCheck::enabled = currentWeaponSetting.velocityCheck;
    for (int bone = (int) DesiredBones::BONE_PELVIS; bone <= (int) DesiredBones::BONE_RIGHT_SOLE; bone++)
        Settings::Aimbot::AutoAim::desiredBones[bone] = currentWeaponSetting.desiredBones[bone];

    Settings::Aimbot::AutoAim::realDistance = currentWeaponSetting.autoAimRealDistance;
    Settings::Aimbot::moveMouse = currentWeaponSetting.moveMouse;
}
