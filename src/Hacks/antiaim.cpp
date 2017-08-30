#include "antiaim.h"

bool Settings::AntiAim::allowUntrustedAngles = false;
bool Settings::AntiAim::Yaw::dynamicAA = false;
bool Settings::AntiAim::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Roll::type = AntiAimType_Z::REVERSE; // Dank Roll
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
bool Settings::AntiAim::Lua::debugMode = true;
char Settings::AntiAim::Lua::scriptX[512];
char Settings::AntiAim::Lua::scriptY[512];
char Settings::AntiAim::Lua::scriptY2[512];
bool Settings::AntiAim::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::SwitchAA::key = ButtonCode_t::KEY_DOWN;
float AntiAim::lastRealYaw = 0.0f;
float AntiAim::lastFakeYaw = 0.0f;
bool AntiAim::isAntiAiming = false;


// if the script is the same, we can skip some initialization.
char luaLastX[sizeof (Settings::AntiAim::Lua::scriptX)];
char luaLastY[sizeof (Settings::AntiAim::Lua::scriptY)];
char luaLastY2[sizeof (Settings::AntiAim::Lua::scriptY2)];
float luaRetX, luaRetY, luaRetY2; // Pop the Lua stack off into these and then return them.

lua_State *LuaX, *LuaY, *LuaY2; // 1 instance of Lua for each Script.

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

void AntiAim::LuaInit() // commence nigg riggin'
{
    // Fill out the structure of the Lua scripts
    Settings::AntiAim::Lua::scriptX[0] = 'f';
    Settings::AntiAim::Lua::scriptX[1] = 'u';
    Settings::AntiAim::Lua::scriptX[2] = 'n';
    Settings::AntiAim::Lua::scriptX[3] = 'c';
    Settings::AntiAim::Lua::scriptX[4] = 't';
    Settings::AntiAim::Lua::scriptX[5] = 'i';
    Settings::AntiAim::Lua::scriptX[6] = 'o';
    Settings::AntiAim::Lua::scriptX[7] = 'n';
    Settings::AntiAim::Lua::scriptX[8] = ' ';
    Settings::AntiAim::Lua::scriptX[9] = 'a';
    Settings::AntiAim::Lua::scriptX[10] = 'n';
    Settings::AntiAim::Lua::scriptX[11] = 'g';
    Settings::AntiAim::Lua::scriptX[12] = 'l';
    Settings::AntiAim::Lua::scriptX[13] = 'e';
    Settings::AntiAim::Lua::scriptX[14] = 'X';
    Settings::AntiAim::Lua::scriptX[15] = '(';
    Settings::AntiAim::Lua::scriptX[16] = 'l';
    Settings::AntiAim::Lua::scriptX[17] = 'a';
    Settings::AntiAim::Lua::scriptX[18] = 's';
    Settings::AntiAim::Lua::scriptX[19] = 't';
    Settings::AntiAim::Lua::scriptX[20] = 'A';
    Settings::AntiAim::Lua::scriptX[21] = 'n';
    Settings::AntiAim::Lua::scriptX[22] = 'g';
    Settings::AntiAim::Lua::scriptX[23] = 'l';
    Settings::AntiAim::Lua::scriptX[24] = 'e';
    Settings::AntiAim::Lua::scriptX[25] = ',';
    Settings::AntiAim::Lua::scriptX[26] = ' ';
    Settings::AntiAim::Lua::scriptX[27] = 'a';
    Settings::AntiAim::Lua::scriptX[28] = 'n';
    Settings::AntiAim::Lua::scriptX[29] = 'g';
    Settings::AntiAim::Lua::scriptX[30] = 'l';
    Settings::AntiAim::Lua::scriptX[31] = 'e';
    Settings::AntiAim::Lua::scriptX[32] = ')';
    Settings::AntiAim::Lua::scriptX[33] = '\n';
    Settings::AntiAim::Lua::scriptX[34] = '\n';
    Settings::AntiAim::Lua::scriptX[35] = 'r';
    Settings::AntiAim::Lua::scriptX[36] = 'e';
    Settings::AntiAim::Lua::scriptX[37] = 't';
    Settings::AntiAim::Lua::scriptX[38] = 'u';
    Settings::AntiAim::Lua::scriptX[39] = 'r';
    Settings::AntiAim::Lua::scriptX[40] = 'n';
    Settings::AntiAim::Lua::scriptX[41] = ' ';
    Settings::AntiAim::Lua::scriptX[42] = 'a';
    Settings::AntiAim::Lua::scriptX[43] = 'n';
    Settings::AntiAim::Lua::scriptX[44] = 'g';
    Settings::AntiAim::Lua::scriptX[45] = 'l';
    Settings::AntiAim::Lua::scriptX[46] = 'e';
    Settings::AntiAim::Lua::scriptX[47] = ';';
    Settings::AntiAim::Lua::scriptX[48] = '\n';
    Settings::AntiAim::Lua::scriptX[49] = 'e';
    Settings::AntiAim::Lua::scriptX[50] = 'n';
    Settings::AntiAim::Lua::scriptX[51] = 'd';
    Settings::AntiAim::Lua::scriptX[sizeof (Settings::AntiAim::Lua::scriptX) - 1] = '\0';

    Settings::AntiAim::Lua::scriptY[0] = 'f';
    Settings::AntiAim::Lua::scriptY[1] = 'u';
    Settings::AntiAim::Lua::scriptY[2] = 'n';
    Settings::AntiAim::Lua::scriptY[3] = 'c';
    Settings::AntiAim::Lua::scriptY[4] = 't';
    Settings::AntiAim::Lua::scriptY[5] = 'i';
    Settings::AntiAim::Lua::scriptY[6] = 'o';
    Settings::AntiAim::Lua::scriptY[7] = 'n';
    Settings::AntiAim::Lua::scriptY[8] = ' ';
    Settings::AntiAim::Lua::scriptY[9] = 'a';
    Settings::AntiAim::Lua::scriptY[10] = 'n';
    Settings::AntiAim::Lua::scriptY[11] = 'g';
    Settings::AntiAim::Lua::scriptY[12] = 'l';
    Settings::AntiAim::Lua::scriptY[13] = 'e';
    Settings::AntiAim::Lua::scriptY[14] = 'Y';
    Settings::AntiAim::Lua::scriptY[15] = '(';
    Settings::AntiAim::Lua::scriptY[16] = 'l';
    Settings::AntiAim::Lua::scriptY[17] = 'a';
    Settings::AntiAim::Lua::scriptY[18] = 's';
    Settings::AntiAim::Lua::scriptY[19] = 't';
    Settings::AntiAim::Lua::scriptY[20] = 'A';
    Settings::AntiAim::Lua::scriptY[21] = 'n';
    Settings::AntiAim::Lua::scriptY[22] = 'g';
    Settings::AntiAim::Lua::scriptY[23] = 'l';
    Settings::AntiAim::Lua::scriptY[24] = 'e';
    Settings::AntiAim::Lua::scriptY[25] = ',';
    Settings::AntiAim::Lua::scriptY[26] = ' ';
    Settings::AntiAim::Lua::scriptY[27] = 'a';
    Settings::AntiAim::Lua::scriptY[28] = 'n';
    Settings::AntiAim::Lua::scriptY[29] = 'g';
    Settings::AntiAim::Lua::scriptY[30] = 'l';
    Settings::AntiAim::Lua::scriptY[31] = 'e';
    Settings::AntiAim::Lua::scriptY[32] = ')';
    Settings::AntiAim::Lua::scriptY[33] = '\n';
    Settings::AntiAim::Lua::scriptY[34] = '\n';
    Settings::AntiAim::Lua::scriptY[35] = 'r';
    Settings::AntiAim::Lua::scriptY[36] = 'e';
    Settings::AntiAim::Lua::scriptY[37] = 't';
    Settings::AntiAim::Lua::scriptY[38] = 'u';
    Settings::AntiAim::Lua::scriptY[39] = 'r';
    Settings::AntiAim::Lua::scriptY[40] = 'n';
    Settings::AntiAim::Lua::scriptY[41] = ' ';
    Settings::AntiAim::Lua::scriptY[42] = 'a';
    Settings::AntiAim::Lua::scriptY[43] = 'n';
    Settings::AntiAim::Lua::scriptY[44] = 'g';
    Settings::AntiAim::Lua::scriptY[45] = 'l';
    Settings::AntiAim::Lua::scriptY[46] = 'e';
    Settings::AntiAim::Lua::scriptY[47] = ';';
    Settings::AntiAim::Lua::scriptY[48] = '\n';
    Settings::AntiAim::Lua::scriptY[49] = 'e';
    Settings::AntiAim::Lua::scriptY[50] = 'n';
    Settings::AntiAim::Lua::scriptY[51] = 'd';
    Settings::AntiAim::Lua::scriptY[sizeof (Settings::AntiAim::Lua::scriptY) - 1] = '\0';

    Settings::AntiAim::Lua::scriptY2[0] = 'f';
    Settings::AntiAim::Lua::scriptY2[1] = 'u';
    Settings::AntiAim::Lua::scriptY2[2] = 'n';
    Settings::AntiAim::Lua::scriptY2[3] = 'c';
    Settings::AntiAim::Lua::scriptY2[4] = 't';
    Settings::AntiAim::Lua::scriptY2[5] = 'i';
    Settings::AntiAim::Lua::scriptY2[6] = 'o';
    Settings::AntiAim::Lua::scriptY2[7] = 'n';
    Settings::AntiAim::Lua::scriptY2[8] = ' ';
    Settings::AntiAim::Lua::scriptY2[9] = 'a';
    Settings::AntiAim::Lua::scriptY2[10] = 'n';
    Settings::AntiAim::Lua::scriptY2[11] = 'g';
    Settings::AntiAim::Lua::scriptY2[12] = 'l';
    Settings::AntiAim::Lua::scriptY2[13] = 'e';
    Settings::AntiAim::Lua::scriptY2[14] = 'Y';
    Settings::AntiAim::Lua::scriptY2[15] = '2';
    Settings::AntiAim::Lua::scriptY2[16] = '(';
    Settings::AntiAim::Lua::scriptY2[17] = 'l';
    Settings::AntiAim::Lua::scriptY2[18] = 'a';
    Settings::AntiAim::Lua::scriptY2[19] = 's';
    Settings::AntiAim::Lua::scriptY2[20] = 't';
    Settings::AntiAim::Lua::scriptY2[21] = 'A';
    Settings::AntiAim::Lua::scriptY2[22] = 'n';
    Settings::AntiAim::Lua::scriptY2[23] = 'g';
    Settings::AntiAim::Lua::scriptY2[24] = 'l';
    Settings::AntiAim::Lua::scriptY2[25] = 'e';
    Settings::AntiAim::Lua::scriptY2[26] = ',';
    Settings::AntiAim::Lua::scriptY2[27] = ' ';
    Settings::AntiAim::Lua::scriptY2[28] = 'a';
    Settings::AntiAim::Lua::scriptY2[29] = 'n';
    Settings::AntiAim::Lua::scriptY2[30] = 'g';
    Settings::AntiAim::Lua::scriptY2[31] = 'l';
    Settings::AntiAim::Lua::scriptY2[32] = 'e';
    Settings::AntiAim::Lua::scriptY2[33] = ')';
    Settings::AntiAim::Lua::scriptY2[34] = '\n';
    Settings::AntiAim::Lua::scriptY2[35] = '\n';
    Settings::AntiAim::Lua::scriptY2[36] = 'r';
    Settings::AntiAim::Lua::scriptY2[37] = 'e';
    Settings::AntiAim::Lua::scriptY2[38] = 't';
    Settings::AntiAim::Lua::scriptY2[39] = 'u';
    Settings::AntiAim::Lua::scriptY2[40] = 'r';
    Settings::AntiAim::Lua::scriptY2[41] = 'n';
    Settings::AntiAim::Lua::scriptY2[42] = ' ';
    Settings::AntiAim::Lua::scriptY2[43] = 'a';
    Settings::AntiAim::Lua::scriptY2[44] = 'n';
    Settings::AntiAim::Lua::scriptY2[45] = 'g';
    Settings::AntiAim::Lua::scriptY2[46] = 'l';
    Settings::AntiAim::Lua::scriptY2[47] = 'e';
    Settings::AntiAim::Lua::scriptY2[48] = ';';
    Settings::AntiAim::Lua::scriptY2[49] = '\n';
    Settings::AntiAim::Lua::scriptY2[50] = 'e';
    Settings::AntiAim::Lua::scriptY2[51] = 'n';
    Settings::AntiAim::Lua::scriptY2[52] = 'd';
    Settings::AntiAim::Lua::scriptY2[sizeof (Settings::AntiAim::Lua::scriptY2) - 1] = '\0';

    luaLastX[sizeof (luaLastX) - 1] = '\0';
    luaLastY[sizeof (luaLastY) - 1] = '\0';
    luaLastY2[sizeof (luaLastY2) - 1] = '\0';

    LuaX = luaL_newstate();
    luaL_openlibs(LuaX);

    LuaY = luaL_newstate();
    luaL_openlibs(LuaY);

    LuaY2 = luaL_newstate();
    luaL_openlibs(LuaY2);

}

void AntiAim::LuaCleanup() {
    lua_close(LuaX);
    lua_close(LuaY);
    lua_close(LuaY2);
}

static void LuaError(int errorCode, lua_State *luaInstance) {
    switch (errorCode) { // defined in lua.h
        case 2: // Runtime Error.
            cvar->ConsoleDPrintf(XORSTR("LUA: Runtime Error: %s\n"), lua_tostring(luaInstance, -1));
            break;
        case 3: // Syntax error.
            cvar->ConsoleDPrintf(XORSTR("LUA: Syntax Error: %s\n"), lua_tostring(luaInstance, -1));
            break;
        case 4: // Memory Allocation error.
            cvar->ConsoleDPrintf(XORSTR("LUA: Memory Alloc Error: %s\n"), lua_tostring(luaInstance, -1));
            break;
        case 6: // Error while returning Error Code
            cvar->ConsoleDPrintf(XORSTR("LUA: Error returning Error: %s\n"), lua_tostring(luaInstance, -1));
            break;
        default: // Unknown.
            cvar->ConsoleDPrintf(XORSTR("LUA: Unknown Error: %s\n"), lua_tostring(luaInstance, -1));
            break;
    }
}

static inline float LuaScriptX(const float lastAngle, const float angle) {
    if (Settings::AntiAim::Lua::debugMode) {
        if (strcmp(Settings::AntiAim::Lua::scriptX, luaLastX) != 0) {
            int load_status = luaL_loadbuffer(LuaX, Settings::AntiAim::Lua::scriptX, strlen(Settings::AntiAim::Lua::scriptX), Settings::AntiAim::Lua::scriptX);
            if (load_status != 0) {
                cvar->ConsoleDPrintf(XORSTR("LUA: Error Loading Buffer\n"));
                LuaError(load_status, LuaX);
                return angle;
            }
            lua_pcall(LuaX, 0, 0, 0); // load the script with no args for function setup.
            cvar->ConsoleDPrintf(XORSTR("Updating ScriptX\n"));
            strncpy(luaLastX, Settings::AntiAim::Lua::scriptX, sizeof (luaLastX));
        }
        lua_getglobal(LuaX, XORSTR("angleX"));
        lua_pushnumber(LuaX, lastAngle); // give Angle from last tick to Lua.
        lua_pushnumber(LuaX, angle); // give current Angle to Lua.
        int run_status = lua_pcall(LuaX, 2, 1, 0); // pcall :^)
        if (run_status != 0) {
            LuaError(run_status, LuaX);
            return angle;
        }
        if (!lua_isnumber(LuaX, -1)) {
            cvar->ConsoleDPrintf(XORSTR("LUA: Your LUA script must return a Number!\n"));
            return angle;
        }
        luaRetX = (float) lua_tonumber(LuaX, -1); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop(LuaX, 1); // pop the returned value off of the stack.
        return luaRetX;
    } else {
        if (strcmp(Settings::AntiAim::Lua::scriptX, luaLastX) != 0) {
            luaL_loadbuffer(LuaX, Settings::AntiAim::Lua::scriptX, strlen(Settings::AntiAim::Lua::scriptX), Settings::AntiAim::Lua::scriptX);
            lua_pcall(LuaX, 0, 0, 0);
            strncpy(luaLastX, Settings::AntiAim::Lua::scriptX, sizeof (luaLastX));
        }
        lua_getglobal(LuaX, XORSTR("angleX"));
        lua_pushnumber(LuaX, lastAngle); // give Angle from last tick to Lua.
        lua_pushnumber(LuaX, angle); // give current Angle to Lua.
        lua_pcall(LuaX, 2, 1, 0);
        luaRetX = (float) lua_tonumber(LuaX, -1); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop(LuaX, 1); // pop the returned value off of the stack.
        return luaRetX;
    }

}

static inline float LuaScriptY(const float lastAngle, const float angle) {
    if (Settings::AntiAim::Lua::debugMode) {
        if (strcmp(Settings::AntiAim::Lua::scriptY, luaLastY) != 0) {
            int load_status = luaL_loadbuffer(LuaY, Settings::AntiAim::Lua::scriptY, strlen(Settings::AntiAim::Lua::scriptY), Settings::AntiAim::Lua::scriptY);
            if (load_status != 0) {
                cvar->ConsoleDPrintf(XORSTR("LUA: Error Loading Buffer\n"));
                LuaError(load_status, LuaY);
                return angle;
            }
            lua_pcall(LuaY, 0, 0, 0); // load the script with no args for function setup.
            cvar->ConsoleDPrintf(XORSTR("Updating ScriptY\n"));
            strncpy(luaLastY, Settings::AntiAim::Lua::scriptY, sizeof (luaLastY));
        }
        lua_getglobal(LuaY, XORSTR("angleY"));
        lua_pushnumber(LuaY, lastAngle); // give Angle from last tick to Lua.
        lua_pushnumber(LuaY, angle); // give current Angle to Lua.
        int run_status = lua_pcall(LuaY, 2, 1, 0); // pcall :^)
        if (run_status != 0) {
            LuaError(run_status, LuaY);
            return angle;
        }
        if (!lua_isnumber(LuaY, -1)) {
            cvar->ConsoleDPrintf(XORSTR("LUA: Your LUA script must return a Number!\n"));
            return angle;
        }
        luaRetY = (float) lua_tonumber(LuaY, -1); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop(LuaY, 1); // pop the returned value off of the stack.
        return luaRetY;
    } else {
        if (strcmp(Settings::AntiAim::Lua::scriptY, luaLastY) != 0) {
            luaL_loadbuffer(LuaY, Settings::AntiAim::Lua::scriptY, strlen(Settings::AntiAim::Lua::scriptY), Settings::AntiAim::Lua::scriptY);
            lua_pcall(LuaY, 0, 0, 0);
            strncpy(luaLastY, Settings::AntiAim::Lua::scriptY, sizeof (luaLastY));
        }
        lua_getglobal(LuaY, XORSTR("angleY"));
        lua_pushnumber(LuaY, lastAngle); // give Angle from last tick to Lua.
        lua_pushnumber(LuaY, angle); // give current Angle to Lua.
        lua_pcall(LuaY, 2, 1, 0);
        luaRetY = (float) lua_tonumber(LuaY, -1); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop(LuaY, 1); // pop the returned value off of the stack.
        return luaRetY;
    }
}

static inline float LuaScriptY2(const float lastAngle, const float angle) {
    if (Settings::AntiAim::Lua::debugMode) {
        if (strcmp(Settings::AntiAim::Lua::scriptY2, luaLastY2) != 0) {
            int load_status = luaL_loadbuffer(LuaY2, Settings::AntiAim::Lua::scriptY2, strlen(Settings::AntiAim::Lua::scriptY2), Settings::AntiAim::Lua::scriptY2);
            if (load_status != 0) {
                cvar->ConsoleDPrintf(XORSTR("LUA: Error Loading Buffer\n"));
                LuaError(load_status, LuaY2);
                return angle;
            }
            lua_pcall(LuaY2, 0, 0, 0); // load the script with no args for function setup.
            cvar->ConsoleDPrintf(XORSTR("Updating ScriptY2\n"));
            strncpy(luaLastY2, Settings::AntiAim::Lua::scriptY2, sizeof (luaLastY2));
        }
        lua_getglobal(LuaY2, XORSTR("angleY2"));
        lua_pushnumber(LuaY2, lastAngle); // give Angle from last tick to Lua.
        lua_pushnumber(LuaY2, angle); // give current Angle to Lua.
        int run_status = lua_pcall(LuaY2, 2, 1, 0); // pcall :^)
        if (run_status != 0) {
            LuaError(run_status, LuaY2);
            return angle;
        }
        if (!lua_isnumber(LuaY2, -1)) {
            cvar->ConsoleDPrintf(XORSTR("LUA: Your LUA script must return a Number!\n"));
            return angle;
        }
        luaRetY2 = (float) lua_tonumber(LuaY2, -1); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop(LuaY2, 1); // pop the returned value off of the stack.
        return luaRetY2;
    } else {
        if (strcmp(Settings::AntiAim::Lua::scriptY2, luaLastY2) != 0) {
            luaL_loadbuffer(LuaY2, Settings::AntiAim::Lua::scriptY2, strlen(Settings::AntiAim::Lua::scriptY2), Settings::AntiAim::Lua::scriptY2);
            lua_pcall(LuaY2, 0, 0, 0);
            strncpy(luaLastY2, Settings::AntiAim::Lua::scriptY2, sizeof (luaLastY2));
        }
        lua_getglobal(LuaY2, XORSTR("angleY2"));
        lua_pushnumber(LuaY2, lastAngle); // give Angle from last tick to Lua.
        lua_pushnumber(LuaY2, angle); // give current Angle to Lua.
        lua_pcall(LuaY2, 2, 1, 0);
        float temp = (float) lua_tonumber(LuaY2, -1); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop(LuaY2, 1); // pop the returned value off of the stack.
        return temp;
    }
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
    int count = 0;

    if(inputSystem->IsButtonDown(Settings::AntiAim::SwitchAA::key)) {
        count++;
        cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Swap AA Count: %i\n", count);
    } else if (count > 0) {
        static AntiAimType_Y fake = Settings::AntiAim::Yaw::typeFake;
        static AntiAimType_Y real = Settings::AntiAim::Yaw::type;
        fake = Settings::AntiAim::Yaw::typeFake;
        real = Settings::AntiAim::Yaw::type;
        Settings::AntiAim::Yaw::type = fake;
        Settings::AntiAim::Yaw::typeFake = real;
        count = 0;
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
    float temp;
    double factor;
    static float trigger;
    QAngle temp_qangle;
    int random;
    int maxJitter;
    int spinval = rand() % 80;
    int ticks = 0;
    int jitterticks = 0;
  	float Base;

    static C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

    static float lastAngleY, lastAngleY2; // angle we had last frame

    yFlip = bFlip != yFlip;

    switch (aa_type) {
        case AntiAimType_Y::LEGITTROLLING:
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
            angle.y -= 180.0f;
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
        case AntiAimType_Y::APOSTROPHE:
        {
            if (CreateMove::sendPacket) {
              int n = 0;
              if (n == 0) {
                  factor = 360.0 / M_PHI;
                  factor *= 25;
                  angle.y = fmodf(globalVars->curtime * factor, 360.0);
                  n = 1;

              } else if (n == 1) {
                  factor = 660.0 / M_PHI;
                  factor *= 65;
                  angle.y = fmodf(globalVars->curtime * factor, 660.0);
                  n = 2;
              } else if (n == 2) {
                  factor = 1360.0 / M_PHI;
                  factor *= 125;
                  angle.y = fmodf(globalVars->curtime * factor, 1360.0);
                  n = 3;
              } else if (n == 3) {

                  factor = 187.0 / M_PHI;
                  factor *= 88;
                  angle.y = fmodf(globalVars->curtime * factor, 187.0);
                  n = 4;
              } else if (n == 4) {


                  factor = 107.0 / M_PHI;
                  factor *= 108;
                  angle.y = fmodf(globalVars->curtime * factor, 107.0);
                  n = 5;

              } else if (n == 5) {


                  factor = 687.0 / M_PHI;
                  factor *= 188;
                  angle.y = fmodf(globalVars->curtime * factor, 687.0);
                  n = 6;
              } else if (n == 6) {


                  factor = 387.0 / M_PHI;
                  factor *= 288;
                  angle.y = fmodf(globalVars->curtime * factor, 387.0);
                  n = 7;
              } else if (n == 7) {


                  factor = 487.0 / M_PHI;
                  factor *= 198;
                  angle.y = fmodf(globalVars->curtime * factor, 487.0);
                  n = 8;
              } else if (n == 8) {


                  factor = 287.0 / M_PHI;
                  factor *= 388;
                  angle.y = fmodf(globalVars->curtime * factor, 287.0);
                  n = 9;
              } else if (n == 9) {


                  factor = 1187.0 / M_PHI;
                  factor *= 588;
                  angle.y = fmodf(globalVars->curtime * factor, 1187.0);
                  n = 10;
              } else if (n == 10) {


                  factor = 1687.0 / M_PHI;
                  factor *= 888;
                  angle.y = fmodf(globalVars->curtime * factor, 1687.0);
                  n = 11;
              } else if (n == 11) {


                  factor = 987.0 / M_PHI;
                  factor *= 248;
                  angle.y = fmodf(globalVars->curtime * factor, 987.0);
                  n = 12;
              } else if (n == 12) {


                  factor = 687.0 / M_PHI;
                  factor *= 188;
                  angle.y = fmodf(globalVars->curtime * factor, 687.0);
                  n = 13;
              } else if (n == 13) {


                  factor = 487.0 / M_PHI;
                  factor *= 168;
                  angle.y = fmodf(globalVars->curtime * factor, 487.0);
                  n = 14;
              } else if (n == 14) {


                  factor = 687.0 / M_PHI;
                  factor *= 188;
                  angle.y = fmodf(globalVars->curtime * factor, 687.0);
                  n = 15;
              } else if (n == 15) {


                  factor = 687.0 / M_PHI;
                  factor *= 1868;
                  angle.y = fmodf(globalVars->curtime * factor, 687.0);
                  n = 16;
              } else if (n == 16) {


                  factor = 6287.0 / M_PHI;
                  factor *= 188;
                  angle.y = fmodf(globalVars->curtime * factor, 6287.0);
                  n = 17;
              } else if (n == 17) {


                  factor = 587.0 / M_PHI;
                  factor *= 168;
                  angle.y = fmodf(globalVars->curtime * factor, 587.0);
                  n = 18;
              } else if (n == 18) {


                  factor = 687.0 / M_PHI;
                  factor *= 1868;
                  angle.y = fmodf(globalVars->curtime * factor, 687.0);
                  n = 19;
              }
              else {
                  factor = 420.0;
                  factor *= 50;
                  angle.y = fmodf(globalVars->curtime * factor, 420);
                  n = 0;

              }
              CreateMove::sendPacket = false;
            } else {
                angle.y -= 180;
                CreateMove::sendPacket = true;

            }
            break;
        }
       
       case AntiAimType_Y::Tank:
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
        case AntiAimType_Y::TANK3:
            static bool uff = false;
            static bool uff2 = false;
            static bool uff3 = false;
            static int uff4 = 0;

            if (pLocal->GetVelocity().Length2D() < 0.1f) {
                if (CreateMove::sendPacket) {

                    if (pLocal->GetFlags() & FL_ONGROUND)
                        angle.y = *pLocal->GetLowerBodyYawTarget() + rand() % 90 + 99;
                    else {
                        random = rand() % 4;
                        switch (random) {
                            case 1:
                                yFlip ? angle.y += 90.f : angle.y -= 90.0f;
                                break;
                            case 2:
                                yFlip ? angle.y -= 120.0f : angle.y -= 120.0f;
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

                        if (uff) {

                            angle.y = *pLocal->GetLowerBodyYawTarget() + 97.0f;
                            uff = false;

                        } else {
                            angle.y -= *pLocal->GetLowerBodyYawTarget() + 97.f;
                            uff = true;
                        }

                        if (uff2) {

                            if (uff4 > 0) {

                                uff4 = 1000;
                                angle.y += 1888 + uff4 / rand();
                                uff4 = 88000;
                            } else {
                                angle.y = *pLocal->GetLowerBodyYawTarget() + uff4;
                                uff4 = rand();
                            }

                            uff2 = false;
                        }
                        else {

                            angle.y = rand() * rand();
                            uff2 = true;

                        }


                        if (uff3) {
                            angle.y = -97.f;
                            uff3 = false;
                        } else {
                            angle.y = *pLocal->GetLowerBodyYawTarget() + 97.f;
                            uff3 = true;


                        }
                    }

                    CreateMove::sendPacket = true;

                }
            } else {

                yFlip ? angle.y -= 170 : angle.y += 170;

            }
            break;
        case AntiAimType_Y::LBYBREAK:
           //Placeholder :/
            // Someone should fix this probably  ._.
            break;
        case AntiAimType_Y::FAKELBY:
            static bool lbyflip = false;
            static bool lbyflip2 = false;
            static bool lbyflip3 = false;
            clamp = false;
            if (!(pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f)) {
              if(CreateMove::sendPacket)
              {
                
                yFlip ? angle.y -= 170.f : angle.y -= 190.f;
                
              }
              else
                { 
                    CreateMove::sendPacket = true;
                    yFlip ? angle.y -= 120 : angle.y += 120;
                    CreateMove::sendPacket = false;
                }
            }
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
                  angle.y = *pLocal->GetLowerBodyYawTarget() + 1337;
                else
                  *pLocal->GetLowerBodyYawTarget() + 180;
                if(lbyflip3)
                  angle.y = *pLocal->GetLowerBodyYawTarget() + rand() + 1080;
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
        case AntiAimType_Y::RANDOMBACKJITTER:
            angle.y -= 180;
            random = rand() % 100;
            maxJitter = rand() % (85 - 70 + 1) + 70;
            temp = maxJitter - (rand() % maxJitter);
            if (random < 35 + (rand() % 15))
                angle.y -= temp;
            else if (random < 85 + (rand() % 15))
                angle.y += temp;
            break;



        case AntiAimType_Y::CASUALJITTER:
            yFlip ? angle.y -= 35.0f : angle.y += 35.0f;
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
        case AntiAimType_Y::JITTER:
            yFlip ? angle.y -= 90.0f : angle.y -= 270.0f;
            break;
        case AntiAimType_Y::BACKJITTER:
            angle.y -= 180;
            random = rand() % 100;
            maxJitter = rand() % (85 - 70 + 1) + 70;
            temp = maxJitter - (rand() % maxJitter);
            if (random < 35 + (rand() % 15))
                angle.y -= temp;
            else if (random < 85 + (rand() % 15))
                angle.y += temp;
            break;

        case AntiAimType_Y::SIDEWAYSRIGHT:
        angle.y += 90;
            break;
        case AntiAimType_Y::SIDEWAYSLEFT:
	        if(Settings::AntiAim::Yaw::dynamicAA)
	        {

		         Base = DoAAatTarget();
            	if(Base > 0)
            		angle.y = Base + 90;
            	else
            		angle.y = Base - 90;

	        }
	        else
           	 angle.y -= 90.0f;
            break;
        case AntiAimType_Y::FAKESIDEWAYS:

                      if (CreateMove::sendPacket) {
                angle.y += 90.0f;
                CreateMove::sendPacket = false;
            } else {
                angle.y = 190.0f;
                CreateMove::sendPacket = true;
            }

            break;
        case AntiAimType_Y::BACKWARDS:
            angle.y -= 180.0f;
            break;

        case AntiAimType_Y::FORWARDS:
            angle.y -= 0.0f;
            break;

        case AntiAimType_Y::STATICAA:
            angle.y = 0.0f;
            break;
        case AntiAimType_Y::STATICJITTER:
            trigger += 15.0f;
            angle.y = trigger > 50.0f ? 150.0f : -150.0f;

            if (trigger > 100.0f)
                trigger = 0.0f;
            break;
        case AntiAimType_Y::STATICSMALLJITTER:
            trigger += 15.0f;
            angle.y = trigger > 50.0f ? -30.0f : 30.0f;

            if (trigger > 100.0f)
                trigger = 0.0f;
            break;
        case AntiAimType_Y::LUA1:
            angle.y = LuaScriptY(lastAngleY, angle.y);
            break;
        case AntiAimType_Y::LUA2:
            angle.y = LuaScriptY2(lastAngleY2, angle.y);
            break;
        case AntiAimType_Y::LISP:
            clamp = false;

            yFlip ? angle.y += 323210000.0f : angle.y -= 323210000.0f;


            break;
        case AntiAimType_Y::LISP_SIDE:
            clamp = false;
            temp = angle.y + 90.0f;
            temp_qangle = QAngle(0.0f, temp, 0.0f);
            if(!Settings::AntiAim::allowUntrustedAngles)
                Math::NormalizeAngles(temp_qangle);
            temp = temp_qangle.y;

            if (temp > -45.0f)
                temp < 0.0f ? temp = -90.0f : temp < 45.0f ? temp = 90.0f : temp = temp;

            temp += 1800000.0f;
            angle.y = temp;
            break;
        case AntiAimType_Y::LISP_JITTER:
            clamp = false;
            static int jittertimer = -1;
            temp = angle.y - 155.0f;

            if (jittertimer == 1) {
                temp = angle.y + 58.0f;
                temp += 1800000.0f;
            }

            if (bSendPacket) {
                if (jittertimer >= 1)
                    jittertimer = -1;
                jittertimer++;
            }
            angle.y = temp;
            break;
        case AntiAimType_Y::ANGEL_BACKWARD:
            clamp = false;
            angle.y += 36000180.0f;
            break;
        case AntiAimType_Y::ANGEL_INVERSE:
            clamp = false;
            angle.y = 36000180.0f;
            break;
        case AntiAimType_Y::LOWERBODY:
            angle.y = *pLocal->GetLowerBodyYawTarget() + rand() % 35 + 165;
            break;

        case AntiAimType_Y::ANGEL_SPIN:
            clamp = false;
            factor = (globalVars->curtime * 5000.0f);
            angle.y = factor + 36000000.0f;
            break;
        case AntiAimType_Y::CASUAL:
            yFlip ? angle.y -= 35.0f : angle.y += 35.0f;
            break;
        case AntiAimType_Y::LUA_UNCLAMPED:
            clamp = false;
            angle.y = LuaScriptY(lastAngleY, angle.y);
            break;
        case AntiAimType_Y::LUA_UNCLAMPED2:
            clamp = false;
            angle.y = LuaScriptY2(lastAngleY2, angle.y);
            break;
        case AntiAimType_Y::LBYONGROUND:
            if (pLocal->GetFlags() & FL_ONGROUND)
                angle.y = *pLocal->GetLowerBodyYawTarget() + rand() % 35 + 165;
            else {
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
                        add = 130;
                    if (jitterticks == 1)
                        add = -145;
                    if (ticks > 0 || !CreateMove::sendPacket)
                        {
                            add = -add;
                            CreateMove::sendPacket = false;
                        }
                    angle.y = + add;
                        }
            break;
        case AntiAimType_Y::EXPERIMENTAL:
        if (!(pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f)) 
        {
            if (CreateMove::sendPacket)
            {
                yFlip ? angle.y = 35.0f : angle.y -= 35.0f;
                CreateMove::sendPacket = false;
            }
            else
            {
                angle.y = 180.f;
                CreateMove::sendPacket = true;
            }
        }
        else
        {
            if (CreateMove::sendPacket)
            {
                yFlip ? angle.y = *pLocal->GetLowerBodyYawTarget() - 170.f : angle.y = *pLocal->GetLowerBodyYawTarget() - 190.f;
                CreateMove::sendPacket = false;
            }
            else
            {
                if (fakeantiaim) 
                {
                    CreateMove::sendPacket = false;
                    angle.y -= *pLocal->GetLowerBodyYawTarget() - 180.f;
                }
                else 
                {
                    CreateMove::sendPacket = true;
                    angle.y -= *pLocal->GetLowerBodyYawTarget() * 18.f;
                }
            }
        }
        default:
            angle.y -= 0.0f;
            break;
    }
    lastAngleY = angle.y;
    lastAngleY2 = angle.y;
}

static void DoAntiAimX(QAngle& angle, bool bFlip, bool& clamp) {
    static float pDance = 0.0f;
    static float lastAngleX;

    static float prape = 0.0f;




    AntiAimType_X aa_type = Settings::AntiAim::Pitch::type;





    switch (aa_type) {
        case AntiAimType_X::STATIC_UP:
            angle.x = -89.0f;
            break;
        case AntiAimType_X::FLIP:
            angle.x = bFlip ? -55.0f : 40.0f;
            break;
        case AntiAimType_X::STATIC_DOWN:
            angle.x = 89.0f;
            break;

        case AntiAimType_X::DANCE:
            pDance += 45.0f;
            if (pDance > 100)
                pDance = 0.0f;
            else if (pDance > 75.f)
                angle.x = -89.f;
            else if (pDance < 75.f)
                angle.x = 89.f;
            break;
        case AntiAimType_X::FRONT:
            angle.x = 0.0f;
            break;
        case AntiAimType_X::STATIC_UP_FAKE:
            if (CreateMove::sendPacket) 
            {
                angle.x -= 89.0f;
                CreateMove::sendPacket = false;
            } else 
            {
                angle.x = 89.0f;
                CreateMove::sendPacket = true;
            }
            break;
        case AntiAimType_X::STATIC_DOWN_FAKE:
            if (CreateMove::sendPacket) {
                angle.x = -89.0f;
                CreateMove::sendPacket = false;
            } else {
                angle.x = 89.0f;
                CreateMove::sendPacket = true;
            }

            break;
        case AntiAimType_X::LUA1:
            angle.x = LuaScriptX(lastAngleX, angle.x);
            break;
        case AntiAimType_X::LISP_DOWN:
            clamp = false;
            angle.x = 1800089.0f;
            break;
        case AntiAimType_X::ANGEL_DOWN:
            clamp = false;
            angle.x = 36000088.0f;
            break;
        case AntiAimType_X::ANGEL_UP:
            clamp = false;
            angle.x = 35999912.0f;
            break;
        case AntiAimType_X::LUA_UNCLAMPED:
            clamp = false;
            angle.x = LuaScriptX(lastAngleX, angle.x);
            break;
        default:
            angle.x -= 0.0f;
            break;
    }
    lastAngleX = angle.x;
}

static void DoAntiAimZ(QAngle& angle, int command_number, bool& clamp) {
    static float pDance = 0.0f;
    AntiAimType_Z aa_type = Settings::AntiAim::Roll::type;

    switch (aa_type) {
        case AntiAimType_Z::REVERSE:
            angle.z = -50.0f;
            break;
        case AntiAimType_Z::AUTISMFLIP:
            angle.z = bSendPacket ? -50.0f : 45.0f;
            break;
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

            if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {

                if (prevLBY1 != *pLocal->GetLowerBodyYawTarget())
                    flip1 = !flip1;

                if (flip1)
                    angle.y += 110.f;
                else
                    angle.y -= 110.f;

                prevLBY1 = *pLocal->GetLowerBodyYawTarget();
            } else
                angle.y -= 0;
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
        if (Settings::AntiAim::Yaw::type >= AntiAimType_Y::LISP)
            Settings::AntiAim::Yaw::type = AntiAimType_Y::NOAA;

        if (Settings::AntiAim::Yaw::typeFake >= AntiAimType_Y::LISP)
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;

        if (Settings::AntiAim::Pitch::type >= AntiAimType_X::STATIC_UP_FAKE)
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
        if (cmd->viewangles.y == *localplayer->GetLowerBodyYawTarget())
        {
            if (antiResolverFlip)
                cmd->viewangles.y += 60.f;
            else
                cmd->viewangles.y -= 60.f;

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
