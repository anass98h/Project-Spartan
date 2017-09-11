
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
float Settings::AntiAim::Yaw::customType = 0.0f;
AntiAimType_Y Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Yaw::customTypeFake = 0.0f;
bool Settings::AntiAim::Yaw::antiResolver = false;
AntiAimType_X Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Pitch::customType = 0.0f;
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
char luaLastX[sizeof( Settings::AntiAim::Lua::scriptX )];
char luaLastY[sizeof( Settings::AntiAim::Lua::scriptY )];
char luaLastY2[sizeof( Settings::AntiAim::Lua::scriptY2 )];
float luaRetX, luaRetY, luaRetY2; // Pop the Lua stack off into these and then return them.
lua_State* LuaX, * LuaY, * LuaY2; // 1 instance of Lua for each Script.

static float Distance( Vector a, Vector b ) {
    return ( sqrt( pow( a.x - b.x, 2 ) + pow( a.y - b.y, 2 ) + pow( a.z - b.z, 2 ) ) );
}

static bool GetBestHeadAngle( QAngle& angle ) {
    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    Vector position = localplayer->GetVecOrigin() + localplayer->GetVecViewOffset();

    float closest_distance = 100.0f;

    float radius = Settings::AntiAim::HeadEdge::distance + 0.1f;
    float step = M_PI * 2.0 / 8;

    for ( float a = 0; a < ( M_PI * 2.0 ); a += step ) {
        Vector location( radius * cos( a ) + position.x, radius * sin( a ) + position.y, position.z );

        Ray_t ray;
        trace_t tr;
        ray.Init( position, location );
        CTraceFilter traceFilter;
        traceFilter.pSkip = localplayer;
        trace->TraceRay( ray, 0x4600400B, &traceFilter, &tr );

        float distance = Distance( position, tr.endpos );

        if ( distance < closest_distance ) {
            closest_distance = distance;
            angle.y = RAD2DEG( a );
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
    Settings::AntiAim::Lua::scriptX[sizeof( Settings::AntiAim::Lua::scriptX ) - 1] = '\0';

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
    Settings::AntiAim::Lua::scriptY[sizeof( Settings::AntiAim::Lua::scriptY ) - 1] = '\0';

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
    Settings::AntiAim::Lua::scriptY2[sizeof( Settings::AntiAim::Lua::scriptY2 ) - 1] = '\0';

    luaLastX[sizeof( luaLastX ) - 1] = '\0';
    luaLastY[sizeof( luaLastY ) - 1] = '\0';
    luaLastY2[sizeof( luaLastY2 ) - 1] = '\0';

    LuaX = luaL_newstate();
    luaL_openlibs( LuaX );

    LuaY = luaL_newstate();
    luaL_openlibs( LuaY );

    LuaY2 = luaL_newstate();
    luaL_openlibs( LuaY2 );

}

void AntiAim::LuaCleanup() {
    lua_close( LuaX );
    lua_close( LuaY );
    lua_close( LuaY2 );
}

static void LuaError( int errorCode, lua_State* luaInstance ) {
    switch ( errorCode ) { // defined in lua.h
        case 2: // Runtime Error.
            cvar->ConsoleDPrintf( XORSTR( "LUA: Runtime Error: %s\n" ), lua_tostring( luaInstance, -1 ) );
            break;
        case 3: // Syntax error.
            cvar->ConsoleDPrintf( XORSTR( "LUA: Syntax Error: %s\n" ), lua_tostring( luaInstance, -1 ) );
            break;
        case 4: // Memory Allocation error.
            cvar->ConsoleDPrintf( XORSTR( "LUA: Memory Alloc Error: %s\n" ), lua_tostring( luaInstance, -1 ) );
            break;
        case 6: // Error while returning Error Code
            cvar->ConsoleDPrintf( XORSTR( "LUA: Error returning Error: %s\n" ), lua_tostring( luaInstance, -1 ) );
            break;
        default: // Unknown.
            cvar->ConsoleDPrintf( XORSTR( "LUA: Unknown Error: %s\n" ), lua_tostring( luaInstance, -1 ) );
            break;
    }
}

static inline float LuaScriptX( const float lastAngle, const float angle ) {
    if ( Settings::AntiAim::Lua::debugMode ) {
        if ( strcmp( Settings::AntiAim::Lua::scriptX, luaLastX ) != 0 ) {
            int load_status = luaL_loadbuffer( LuaX, Settings::AntiAim::Lua::scriptX,
                                               strlen( Settings::AntiAim::Lua::scriptX ),
                                               Settings::AntiAim::Lua::scriptX );
            if ( load_status != 0 ) {
                cvar->ConsoleDPrintf( XORSTR( "LUA: Error Loading Buffer\n" ) );
                LuaError( load_status, LuaX );
                return angle;
            }
            lua_pcall( LuaX, 0, 0, 0 ); // load the script with no args for function setup.
            cvar->ConsoleDPrintf( XORSTR( "Updating ScriptX\n" ) );
            strncpy( luaLastX, Settings::AntiAim::Lua::scriptX, sizeof( luaLastX ) );
        }
        lua_getglobal( LuaX, XORSTR( "angleX" ) );
        lua_pushnumber( LuaX, lastAngle ); // give Angle from last tick to Lua.
        lua_pushnumber( LuaX, angle ); // give current Angle to Lua.
        int run_status = lua_pcall( LuaX, 2, 1, 0 ); // pcall :^)
        if ( run_status != 0 ) {
            LuaError( run_status, LuaX );
            return angle;
        }
        if ( !lua_isnumber( LuaX, -1 ) ) {
            cvar->ConsoleDPrintf( XORSTR( "LUA: Your LUA script must return a Number!\n" ) );
            return angle;
        }
        luaRetX = ( float ) lua_tonumber( LuaX, -1 ); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop( LuaX, 1 ); // pop the returned value off of the stack.
        return luaRetX;
    } else {
        if ( strcmp( Settings::AntiAim::Lua::scriptX, luaLastX ) != 0 ) {
            luaL_loadbuffer( LuaX, Settings::AntiAim::Lua::scriptX, strlen( Settings::AntiAim::Lua::scriptX ),
                             Settings::AntiAim::Lua::scriptX );
            lua_pcall( LuaX, 0, 0, 0 );
            strncpy( luaLastX, Settings::AntiAim::Lua::scriptX, sizeof( luaLastX ) );
        }
        lua_getglobal( LuaX, XORSTR( "angleX" ) );
        lua_pushnumber( LuaX, lastAngle ); // give Angle from last tick to Lua.
        lua_pushnumber( LuaX, angle ); // give current Angle to Lua.
        lua_pcall( LuaX, 2, 1, 0 );
        luaRetX = ( float ) lua_tonumber( LuaX, -1 ); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop( LuaX, 1 ); // pop the returned value off of the stack.
        return luaRetX;
    }

}

static inline float LuaScriptY( const float lastAngle, const float angle ) {
    if ( Settings::AntiAim::Lua::debugMode ) {
        if ( strcmp( Settings::AntiAim::Lua::scriptY, luaLastY ) != 0 ) {
            int load_status = luaL_loadbuffer( LuaY, Settings::AntiAim::Lua::scriptY,
                                               strlen( Settings::AntiAim::Lua::scriptY ),
                                               Settings::AntiAim::Lua::scriptY );
            if ( load_status != 0 ) {
                cvar->ConsoleDPrintf( XORSTR( "LUA: Error Loading Buffer\n" ) );
                LuaError( load_status, LuaY );
                return angle;
            }
            lua_pcall( LuaY, 0, 0, 0 ); // load the script with no args for function setup.
            cvar->ConsoleDPrintf( XORSTR( "Updating ScriptY\n" ) );
            strncpy( luaLastY, Settings::AntiAim::Lua::scriptY, sizeof( luaLastY ) );
        }
        lua_getglobal( LuaY, XORSTR( "angleY" ) );
        lua_pushnumber( LuaY, lastAngle ); // give Angle from last tick to Lua.
        lua_pushnumber( LuaY, angle ); // give current Angle to Lua.
        int run_status = lua_pcall( LuaY, 2, 1, 0 ); // pcall :^)
        if ( run_status != 0 ) {
            LuaError( run_status, LuaY );
            return angle;
        }
        if ( !lua_isnumber( LuaY, -1 ) ) {
            cvar->ConsoleDPrintf( XORSTR( "LUA: Your LUA script must return a Number!\n" ) );
            return angle;
        }
        luaRetY = ( float ) lua_tonumber( LuaY, -1 ); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop( LuaY, 1 ); // pop the returned value off of the stack.
        return luaRetY;
    } else {
        if ( strcmp( Settings::AntiAim::Lua::scriptY, luaLastY ) != 0 ) {
            luaL_loadbuffer( LuaY, Settings::AntiAim::Lua::scriptY, strlen( Settings::AntiAim::Lua::scriptY ),
                             Settings::AntiAim::Lua::scriptY );
            lua_pcall( LuaY, 0, 0, 0 );
            strncpy( luaLastY, Settings::AntiAim::Lua::scriptY, sizeof( luaLastY ) );
        }
        lua_getglobal( LuaY, XORSTR( "angleY" ) );
        lua_pushnumber( LuaY, lastAngle ); // give Angle from last tick to Lua.
        lua_pushnumber( LuaY, angle ); // give current Angle to Lua.
        lua_pcall( LuaY, 2, 1, 0 );
        luaRetY = ( float ) lua_tonumber( LuaY, -1 ); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop( LuaY, 1 ); // pop the returned value off of the stack.
        return luaRetY;
    }
}

static inline float LuaScriptY2( const float lastAngle, const float angle ) {
    if ( Settings::AntiAim::Lua::debugMode ) {
        if ( strcmp( Settings::AntiAim::Lua::scriptY2, luaLastY2 ) != 0 ) {
            int load_status = luaL_loadbuffer( LuaY2, Settings::AntiAim::Lua::scriptY2,
                                               strlen( Settings::AntiAim::Lua::scriptY2 ),
                                               Settings::AntiAim::Lua::scriptY2 );
            if ( load_status != 0 ) {
                cvar->ConsoleDPrintf( XORSTR( "LUA: Error Loading Buffer\n" ) );
                LuaError( load_status, LuaY2 );
                return angle;
            }
            lua_pcall( LuaY2, 0, 0, 0 ); // load the script with no args for function setup.
            cvar->ConsoleDPrintf( XORSTR( "Updating ScriptY2\n" ) );
            strncpy( luaLastY2, Settings::AntiAim::Lua::scriptY2, sizeof( luaLastY2 ) );
        }
        lua_getglobal( LuaY2, XORSTR( "angleY2" ) );
        lua_pushnumber( LuaY2, lastAngle ); // give Angle from last tick to Lua.
        lua_pushnumber( LuaY2, angle ); // give current Angle to Lua.
        int run_status = lua_pcall( LuaY2, 2, 1, 0 ); // pcall :^)
        if ( run_status != 0 ) {
            LuaError( run_status, LuaY2 );
            return angle;
        }
        if ( !lua_isnumber( LuaY2, -1 ) ) {
            cvar->ConsoleDPrintf( XORSTR( "LUA: Your LUA script must return a Number!\n" ) );
            return angle;
        }
        luaRetY2 = ( float ) lua_tonumber( LuaY2,
                                           -1 ); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop( LuaY2, 1 ); // pop the returned value off of the stack.
        return luaRetY2;
    } else {
        if ( strcmp( Settings::AntiAim::Lua::scriptY2, luaLastY2 ) != 0 ) {
            luaL_loadbuffer( LuaY2, Settings::AntiAim::Lua::scriptY2, strlen( Settings::AntiAim::Lua::scriptY2 ),
                             Settings::AntiAim::Lua::scriptY2 );
            lua_pcall( LuaY2, 0, 0, 0 );
            strncpy( luaLastY2, Settings::AntiAim::Lua::scriptY2, sizeof( luaLastY2 ) );
        }
        lua_getglobal( LuaY2, XORSTR( "angleY2" ) );
        lua_pushnumber( LuaY2, lastAngle ); // give Angle from last tick to Lua.
        lua_pushnumber( LuaY2, angle ); // give current Angle to Lua.
        lua_pcall( LuaY2, 2, 1, 0 );
        float temp = ( float ) lua_tonumber( LuaY2,
                                             -1 ); // By default lua_number is a double, can be changed in luaconf.h
        lua_pop( LuaY2, 1 ); // pop the returned value off of the stack.
        return temp;
    }
}

static bool HasViableEnemy() {
    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    for ( int i = 1; i < engine->GetMaxClients(); ++i ) {
        C_BasePlayer* entity = ( C_BasePlayer* ) entityList->GetClientEntity( i );

        if ( !entity
             || entity == localplayer
             || entity->GetDormant()
             || !entity->GetAlive()
             || entity->GetImmune() )
            continue;

        if ( !Aimbot::friends.empty() ) // check for friends, if any
        {
            IEngineClient::player_info_t entityInformation;
            engine->GetPlayerInfo( i, &entityInformation );

            if ( std::find( Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid ) !=
                 Aimbot::friends.end() )
                continue;
        }

        if ( Settings::Aimbot::friendly || entity->GetTeam() != localplayer->GetTeam() )
            return true;
    }

    return false;
}

static float DoAAatTarget() {
    static C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    static float Angle = 0.0f;

    if ( Settings::AntiAim::Yaw::enabled ) {
        if ( Settings::AntiAim::Yaw::dynamicAA ) {
            float bestDist = 999999999.f; // easy cuz im retarded
            for ( int i = 1; i < engine->GetMaxClients(); ++i ) {
                C_BasePlayer* target = ( C_BasePlayer* ) entityList->GetClientEntity( i );

                if ( !target
                     || target == pLocal
                     || target->GetDormant()
                     || !target->GetAlive()
                     || target->GetImmune()
                     || target->GetTeam() == pLocal->GetTeam() )
                    continue;

                Vector eye_pos = pLocal->GetEyePosition();
                Vector target_pos = target->GetEyePosition();

                float tempDist = eye_pos.DistTo( target_pos );

                if ( bestDist > tempDist ) {
                    bestDist = tempDist;
                    Angle = Math::CalcAngle( eye_pos, target_pos ).y;
                    return Angle;

                }
            }
        }

    }
}

static void SwapAA() {
    int count;

    if ( inputSystem->IsButtonDown( Settings::AntiAim::SwitchAA::key ) ) {
        count++;
        //cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "Swap AA Count: %i\n", count);
    } else {
        if ( count > 0 ) {
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

float GetLatency() {
    INetChannelInfo* nci = engine->GetNetChannelInfo();
    if ( nci ) {
        float Latency = nci->GetAvgLatency( FLOW_OUTGOING ) + nci->GetAvgLatency( FLOW_INCOMING );
        return Latency;
    } else {
        return 0.0f;
    }
}

float GetOutgoingLatency() {
    INetChannelInfo* nci = engine->GetNetChannelInfo();
    if ( nci ) {
        float OutgoingLatency = nci->GetAvgLatency( FLOW_OUTGOING );
        return OutgoingLatency;
    } else {
        return 0.0f;
    }
}

float GetIncomingLatency() {
    INetChannelInfo* nci = engine->GetNetChannelInfo();
    if ( nci ) {
        float IncomingLatency = nci->GetAvgLatency( FLOW_INCOMING );
        return IncomingLatency;
    } else {
        return 0.0f;
    }
}

float OldLBY;
float LBYBreakerTimer;
float LastLBYUpdateTime;
bool bSwitch;

float CurrentVelocity( C_BasePlayer* LocalPlayer ) {
    int vel = LocalPlayer->GetVelocity().Length2D();
    return vel;
}

bool NextLBYUpdate() {
    C_BasePlayer* LocalPlayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    float flServerTime = ( float ) ( LocalPlayer->GetTickBase() * globalVars->interval_per_tick );


    if ( OldLBY != *LocalPlayer->GetLowerBodyYawTarget() ) {
        LBYBreakerTimer++;
        OldLBY = *LocalPlayer->GetLowerBodyYawTarget();
        bSwitch = !bSwitch;
        LastLBYUpdateTime = flServerTime;
    }

    if ( CurrentVelocity( LocalPlayer ) > 0.5 ) {
        LastLBYUpdateTime = flServerTime;
        return false;
    }

    if ( ( LastLBYUpdateTime + 1 - ( GetLatency() * 2 ) < flServerTime ) &&
         ( LocalPlayer->GetFlags() & FL_ONGROUND ) ) {
        if ( LastLBYUpdateTime + 1.1 - ( GetLatency() * 2 ) < flServerTime ) {
            LastLBYUpdateTime += 1.1;
        }
        return true;
    }
    return false;
}


static void DoAntiAimY( QAngle& angle, int command_number, bool bFlip, bool& clamp ) {
    AntiAimType_Y aa_type = bFlip ? Settings::AntiAim::Yaw::typeFake : Settings::AntiAim::Yaw::type;

    static bool yFlip;
    double factor;
    static bool flip1 = false;
    static bool back = false;
    static float prevLBY1 = *( ( C_BasePlayer* ) entityList->GetClientEntity(
            engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget();
    static bool lolgay;
    int random;
    int ticks = 0;
    int jitterticks = 0;
    static C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );


    yFlip = bFlip != yFlip;

    float baseYaw = angle.y;
    baseYaw += bFlip ? Settings::AntiAim::Yaw::customTypeFake : Settings::AntiAim::Yaw::customType;

    switch ( aa_type ) {
        case AntiAimType_Y::LEGITTROLLING:
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
            baseYaw -= 180.f;
            break;
        case AntiAimType_Y::LEGITTROLLING2:
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::NOAA;
            baseYaw -= 90.0f;
            break;
        case AntiAimType_Y::SPIN:
            factor = 360.0 / M_PHI;
            factor *= Settings::spinFactor::value;
            baseYaw = fmodf( globalVars->curtime * factor, 360.0 );
            break;
        case AntiAimType_Y::CUSTOM:
            Math::ClampY( Settings::customYaw::value );
            Math::ClampY( Settings::customYaw::value );

            if ( Settings::customYaw::lby ) {
                if ( Settings::customYaw::value > 0 )
                    baseYaw += *pLocal->GetLowerBodyYawTarget() + ( Settings::customYaw::value );
                else
                    baseYaw -= *pLocal->GetLowerBodyYawTarget() - ( Settings::customYaw::value );

            } else {
                baseYaw += Settings::customYaw::value;
            }
            break;
        case AntiAimType_Y::CUSTOM2:
            Math::ClampY( Settings::customYaw2::value );
            Math::ClampY( Settings::customYaw2::value );

            if ( Settings::customYaw2::lby ) {
                if ( Settings::customYaw2::value > 0 )
                    baseYaw += *pLocal->GetLowerBodyYawTarget() + ( Settings::customYaw2::value );
                else
                    baseYaw -= *pLocal->GetLowerBodyYawTarget() - ( Settings::customYaw2::value );
            } else {
                baseYaw += Settings::customYaw2::value;
            }
            break;

        case AntiAimType_Y::TANK1:
            if ( !( pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f ) )
                yFlip ? baseYaw -= 165 : baseYaw += 195;
            if ( !( pLocal->GetFlags() & FL_ONGROUND ) )
                yFlip ? baseYaw += 90 : baseYaw -= 90;

            if ( CreateMove::sendPacket ) {
                random = rand() % 4;
                switch ( random ) {
                    case 0:
                        baseYaw = *pLocal->GetLowerBodyYawTarget() + rand() % 220;
                        break;
                    case 1:
                        baseYaw = *pLocal->GetLowerBodyYawTarget() - 97;
                        break;
                    case 2:
                        yFlip ? baseYaw += 97 : baseYaw -= 97;
                        break;
                    case 3:
                        baseYaw = *pLocal->GetLowerBodyYawTarget() - rand() + 97;
                        break;
                }
                CreateMove::sendPacket = false;

            } else {
                static int psilent;
                psilent = rand() % 6;
                switch ( psilent ) {
                    case 0:
                        baseYaw = *pLocal->GetLowerBodyYawTarget() + 88;
                        break;
                    case 1:
                        baseYaw = bSendPacket ? 118 : 270;
                        break;
                    case 2:
                        baseYaw = yFlip ? *pLocal->GetLowerBodyYawTarget() + 97 : *pLocal->GetLowerBodyYawTarget() - rand() % 97;
                        break;
                    case 3:
                        baseYaw = *pLocal->GetLowerBodyYawTarget() - rand();
                        break;
                    case 4:
                        baseYaw -= 97;
                        break;
                    case 5:
                        baseYaw = yFlip ? *pLocal->GetLowerBodyYawTarget() + rand() % 112 :
                                          *pLocal->GetLowerBodyYawTarget() - rand() % 66;
                        break;


                }
                CreateMove::sendPacket = false;

            }
            break;
        case AntiAimType_Y::TANK2:
            if ( bSendPacket ) {
                random = rand() % 4;
                switch ( random ) {
                    case 1:
                        baseYaw = *pLocal->GetLowerBodyYawTarget() + rand() % 35 + 165;
                        break;
                    case 2:
                        yFlip ? baseYaw -= 160 : baseYaw += 160;
                        break;
                    case 3:
                        factor = 360.0 / M_PHI;
                        factor *= rand() % 25;
                        float x = fmodf( globalVars->curtime * factor, 360.0 );
                        factor *= 5;
                        float y = fmodf( globalVars->curtime * factor, 360.0f );
                        if ( y >= 100.0f ) {
                            y -= 170.0f;
                        }
                        if ( y <= 200.0f ) {
                            y += 305.00f;
                        }
                        baseYaw = ( ( x / y ) + 60.2f ) * M_PI;
                }
            } else {
                random = rand() % 4;
                switch ( random ) {
                    case 1:
                        baseYaw -= 180.0f;
                        break;
                    case 2:
                        yFlip ? baseYaw += 90.f : baseYaw -= 90.0f;
                        break;
                    case 3:
                        factor = 360.0 / M_PHI;
                        baseYaw = fmodf( globalVars->curtime * factor, 360.0 );

                }
            }
            if ( baseYaw ==
                 *pLocal->GetLowerBodyYawTarget() ) {
                baseYaw = *pLocal->GetLowerBodyYawTarget() + 90;

            }

            break;
        case AntiAimType_Y::TANK3:
            static bool uff = false;
            static bool uff2 = false;
            static bool uff3 = false;
            static int uff4 = 0;

            if ( pLocal->GetVelocity().Length2D() < 0.1f ) {
                if ( CreateMove::sendPacket ) {

                    if ( pLocal->GetFlags() & FL_ONGROUND )
                        baseYaw = *pLocal->GetLowerBodyYawTarget() + rand() % 90 + 99;
                    else {
                        random = rand() % 4;
                        switch ( random ) {
                            case 1:
                                yFlip ? baseYaw += 90.f : baseYaw -= 90.0f;
                                break;
                            case 2:
                                baseYaw -= 120.0f;
                                break;
                            case 3:
                                factor = 360.0 / M_PHI;
                                factor *= 25;
                                baseYaw = fmodf( globalVars->curtime * factor, 360.0 );
                                break;
                            default:
                                baseYaw -= 180.0f;
                        }
                    }
                    CreateMove::sendPacket = false;
                } else {
                    {
                        baseYaw = *pLocal->GetLowerBodyYawTarget() + 97.0f;

                        if ( uff2 ) {

                            if ( uff4 > 0 ) {

                                uff4 = 1000;
                                baseYaw += 1888 + uff4 / rand();
                                uff4 = 88000;
                            } else {
                                baseYaw = *pLocal->GetLowerBodyYawTarget() + uff4;
                                uff4 = rand();
                            }

                            uff2 = false;
                        } else {

                            baseYaw = rand() * rand();
                            uff2 = true;

                        }


                        if ( uff3 ) {
                            baseYaw = -97.f;
                            uff3 = false;
                        } else {
                            baseYaw = *pLocal->GetLowerBodyYawTarget() + 97.f;
                            uff3 = true;


                        }
                    }

                    CreateMove::sendPacket = true;

                }
            } else {

                yFlip ? baseYaw -= 170 : baseYaw += 170;

            }
            break;
        case AntiAimType_Y::LBYBREAK:
            //Placeholder
            break;
        case AntiAimType_Y::FAKELBY:
            clamp = false;
            if ( fabsf( pLocal->GetVelocity().x ) != 0 ) {
                baseYaw -= 180.f;
            } else {
                if ( lolgay ) {
                    yFlip ? baseYaw -= *pLocal->GetLowerBodyYawTarget() - 97 : baseYaw -=
                                                                                       *pLocal->GetLowerBodyYawTarget() +
                                                                                       97;
                } else {
                    if ( prevLBY1 != *pLocal->GetLowerBodyYawTarget() )
                        flip1 = false;
                    if ( prevLBY1 != *pLocal->GetLowerBodyYawTarget() )
                        flip1 = true;
                    if ( flip1 )
                        baseYaw += 120.f;
                    else
                        baseYaw -= 120.f;

                    prevLBY1 = *pLocal->GetLowerBodyYawTarget();
                }
                break;
                case AntiAimType_Y::LBYSPIN:
                    factor = 360.0 / M_PHI;
                factor *= Settings::spinFactor::value;
                baseYaw = *pLocal->GetLowerBodyYawTarget() +
                          fmodf( globalVars->curtime * factor, 360.0 );
                break;
                case AntiAimType_Y::LBYJITTER:

                    if ( pLocal->GetFlags() & FL_ONGROUND )
                        baseYaw = *pLocal->GetLowerBodyYawTarget() + rand() % 35 + 165;
                    else {
                        random = rand() % 4;
                        switch ( random ) {
                            case 1:
                                yFlip ? baseYaw += 90.f : baseYaw -= 90.0f;
                                break;
                            case 2:
                                yFlip ? baseYaw -= 120.0f : baseYaw -= 210.0f;
                                break;
                            case 3:
                                factor = 360.0 / M_PHI;
                                factor *= 25;
                                baseYaw = fmodf( globalVars->curtime * factor, 360.0 );
                                break;
                            default:
                                baseYaw -= 180.0f;
                        }
                    }
                break;
                case AntiAimType_Y::NOAA:
                    break;
                case AntiAimType_Y::BACKJITTER:
                    static int random = rand() % 16;
                yFlip ? baseYaw -= 180.f + random : baseYaw += 180.f + random;
                break;
                case AntiAimType_Y::FAKESIDEWAYS:

                    if ( CreateMove::sendPacket ) {
                        baseYaw -= 90.f;
                        CreateMove::sendPacket = false;
                    } else {
                        baseYaw = 120.0f;
                        CreateMove::sendPacket = true;
                    }
                break;
                case AntiAimType_Y::BACKWARDS:
                    baseYaw -= 180.0f;
                break;

                case AntiAimType_Y::FORWARDS:
                    baseYaw -= 0.0f;
                break;
                case AntiAimType_Y::LOWERBODY:
                    if ( CreateMove::sendPacket ) {
                        baseYaw = *pLocal->GetLowerBodyYawTarget();
                        CreateMove::sendPacket = false;
                    } else {
                        baseYaw = *pLocal->GetLowerBodyYawTarget();
                        CreateMove::sendPacket = true;
                    }
                break;
                case AntiAimType_Y::FJITTER: {
                    if ( CreateMove::sendPacket ) {
                        if ( jitterticks > 0 )
                            jitterticks = -1;
                        jitterticks++;
                    }
                    int add = 0;
                    if ( jitterticks == 0 )
                        add += 130;
                    if ( jitterticks == 1 )
                        add -= 145;
                    if ( ticks > 0 || !CreateMove::sendPacket ) {
                        add = -add;
                        CreateMove::sendPacket = false;
                    }
                    baseYaw += add;
                }
                break;
                case AntiAimType_Y::richieap:
                    static bool besteap = false;
                if ( fabsf( pLocal->GetVelocity().x ) < 0.1 ) {
                    besteap = !besteap;
                    CreateMove::sendPacket = besteap;
                    baseYaw -= besteap ? 180 : *pLocal->GetLowerBodyYawTarget() + 90.f;

                } else {
                    besteap = !besteap;
                    CreateMove::sendPacket = besteap;
                    baseYaw -= besteap ? 0 : 180;
                }
                break;
                case AntiAimType_Y::MYRRIB:
                    if ( CreateMove::sendPacket ) {
                        if ( fabsf( pLocal->GetVelocity().x ) != 0 ) {
                            baseYaw -= 180.f;
                            CreateMove::sendPacket = false;
                        } else {
                            baseYaw -= *pLocal->GetLowerBodyYawTarget() + 45.f;
                            CreateMove::sendPacket = false;
                        }
                    } else {
                        if ( fabsf( pLocal->GetVelocity().x ) != 0 ) {
                            baseYaw -= 180; //pMemez here plz fix me
                            CreateMove::sendPacket = true;
                        } else {
                            baseYaw -= *pLocal->GetLowerBodyYawTarget() - 180.f;
                            CreateMove::sendPacket = true;
                        }
                    }
                break;
                case AntiAimType_Y::LBYONGROUND:
                    if ( pLocal->GetFlags() & FL_ONGROUND )
                        baseYaw = *pLocal->GetLowerBodyYawTarget() + rand() % 35 + 165;
                    else {
                        static int aimType = rand() % 4;
                        switch ( aimType ) {
                            case 1:
                                yFlip ? baseYaw += 90.f : baseYaw -= 90.0f;
                                break;
                            case 2:
                                yFlip ? baseYaw -= 120.0f : baseYaw -= 210.0f;
                                break;
                            case 3:
                                factor = 360.0 / M_PHI;
                                factor *= 25;
                                baseYaw = fmodf( globalVars->curtime * factor, 360.0 );
                                break;
                            default:
                                baseYaw -= 180.0f;
                        }
                    }
            }
    }

    angle.y = baseYaw;

    if ( !Settings::AntiAim::allowUntrustedAngles ) {
        Math::ClampAngles(angle);
    }
}

static void DoAntiAimX( QAngle& angle, bool bFlip, bool& clamp ) {
    static C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );

    AntiAimType_X aa_type = Settings::AntiAim::Pitch::type;

    static bool fakezeroS = false;
    static bool fakeupS = false;

    switch ( aa_type ) {
        case AntiAimType_X::STATIC_UP:
            angle.x = -89.0f;
            break;
        case AntiAimType_X::FLIP:
            if ( fabsf( pLocal->GetVelocity().x ) != 0 ) {
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
}

static void DoAntiAimZ( QAngle& angle, int command_number, bool& clamp ) {
    AntiAimType_Z aa_type = Settings::AntiAim::Roll::type;

    switch ( aa_type ) {
        case AntiAimType_Z::TEST:
            angle.z = 180.0f;
            break;
    }
}

static void DoAntiAimLBY( QAngle& angle, int command_number, bool bFlip, bool& clamp ) {
    static float pDance = 0.0f;
    static bool xFlip;

    AntiAimType_LBY aa_type = Settings::AntiAim::Lby::type;
    static C_BasePlayer* pLocal = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    xFlip = bFlip != xFlip;

    switch ( aa_type ) {

        case AntiAimType_LBY::ONE: {
            static bool flip1 = false;
            static float prevLBY1 = *( ( C_BasePlayer* ) entityList->GetClientEntity(
                    engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget();

            if ( pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f ) {

                if ( prevLBY1 !=
                     *( ( C_BasePlayer* ) entityList->GetClientEntity(
                             engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget() )
                    flip1 = false;
                if ( prevLBY1 !=
                     *( ( C_BasePlayer* ) entityList->GetClientEntity(
                             engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget() )
                    flip1 = true;
                if ( flip1 )
                    angle.y += 108.f;
                else
                    angle.y -= 109.f;

                prevLBY1 = *( ( C_BasePlayer* ) entityList->GetClientEntity(
                        engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget();
            } else {
                angle.y -= 0;
            }
        }
            break;
        case AntiAimType_LBY::TWO: {
            static bool flip2 = false;
            static float prevLBY2 = *( ( C_BasePlayer* ) entityList->GetClientEntity(
                    engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget();

            if ( pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f ) {

                if ( prevLBY2 !=
                     *( ( C_BasePlayer* ) entityList->GetClientEntity(
                             engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget() )
                    flip2 = !flip2;

                if ( flip2 )
                    angle.y += 90.f;
                else
                    angle.y -= 90.f;

                prevLBY2 = *( ( C_BasePlayer* ) entityList->GetClientEntity(
                        engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget();
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
            if ( LBYflip ) {
                if ( fabsf( pLocal->GetVelocity().x ) != 0 ) {
                    angle.y += 180.f;
                    CreateMove::sendPacket = false;
                } else {
                    if ( globalVars->curtime > nextUpdate ) {

                        angle.y += cuck;
                        CreateMove::sendPacket = false;
                        nextUpdate = globalVars->curtime + 1.1f;
                    }
                }
            } else {
                angle.y += *( ( C_BasePlayer* ) entityList->GetClientEntity(
                        engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget() + 90.f;
                CreateMove::sendPacket = true;
            }
        }
            break;
        case AntiAimType_LBY::MYRRIB: {
            static bool LBYflipM = false;
            LBYflipM = !LBYflipM;
            if ( LBYflipM ) {
                angle.y = *( ( C_BasePlayer* ) entityList->GetClientEntity(
                        engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget();
                CreateMove::sendPacket = true;
            } else {
                angle.y = *( ( C_BasePlayer* ) entityList->GetClientEntity(
                        engine->GetLocalPlayer() ) )->GetLowerBodyYawTarget() + 180.f;
                CreateMove::sendPacket = false;
            }
        }
            break;
        case AntiAimType_LBY::FOUR:
            // TODO
            break;
        case AntiAimType_LBY::NONE:
            //Settings::AntiAim::Lby::enabled = false;
            break;
    }
}


void AntiAim::CreateMove( CUserCmd* cmd ) {
    isAntiAiming = false;
    if ( !Settings::AntiAim::Yaw::enabled && !Settings::AntiAim::Pitch::enabled ) {
        if ( Settings::FakeLag::enabled )
            FakeLag::bFlipping = true;
        return;
    }

    if ( Settings::Aimbot::AimStep::enabled && Aimbot::aimStepInProgress )
        return;

    QAngle oldAngle = cmd->viewangles;
    float oldForward = cmd->forwardmove;
    float oldSideMove = cmd->sidemove;

    QAngle angle = cmd->viewangles;

    C_BasePlayer* localplayer = ( C_BasePlayer* ) entityList->GetClientEntity( engine->GetLocalPlayer() );
    if ( !localplayer )
        return;

    C_BaseCombatWeapon* activeWeapon = ( C_BaseCombatWeapon* ) entityList->GetClientEntityFromHandle(
            localplayer->GetActiveWeapon() );
    if ( !activeWeapon )
        return;

    if ( activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE ) {
        C_BaseCSGrenade* csGrenade = ( C_BaseCSGrenade* ) activeWeapon;

        if ( csGrenade->GetThrowTime() > 0.0f )
            return;
    }

    if ( cmd->buttons & IN_USE || cmd->buttons & IN_ATTACK || ( cmd->buttons & IN_ATTACK2 &&
                                                                *activeWeapon->GetItemDefinitionIndex() ==
                                                                ItemDefinitionIndex::WEAPON_REVOLVER ) )
        return;
    if ( localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP )
        return;

    // AutoDisable checks

    // Knife
    if ( Settings::AntiAim::AutoDisable::knifeHeld && localplayer->GetAlive() &&
         activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE )
        return;
    // Bomb
    if ( Settings::AntiAim::AutoDisable::bombHeld && localplayer->GetAlive() &&
         activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_C4 )
        return;
    // No Enemy
    if ( Settings::AntiAim::AutoDisable::noEnemy && localplayer->GetAlive() && !HasViableEnemy() )
        return;

    isAntiAiming = true;
    QAngle edge_angle = angle;
    bool edging_head = Settings::AntiAim::HeadEdge::enabled && GetBestHeadAngle( edge_angle );
    static bool bFlip;

    bFlip = !bFlip;
    FakeLag::bFlipping = bFlip;

    bool should_clamp = !Settings::AntiAim::allowUntrustedAngles;

    if ( !ValveDSCheck::forceUT && ( *csGameRules ) && ( *csGameRules )->IsValveDS() && !should_clamp ) {
        /*if (Settings::AntiAim::Yaw::type > AntiAimType_Y::CUSTOM)
            Settings::AntiAim::Yaw::type = AntiAimType_Y::CUSTOM;

        if (Settings::AntiAim::Yaw::typeFake >= AntiAimType_Y::CUSTOM)
            Settings::AntiAim::Yaw::typeFake = AntiAimType_Y::CUSTOM;

            As far as my information goes, we no longer have any untrusted Yaw AA's in Spartan.
        */

        if ( Settings::AntiAim::Pitch::type > AntiAimType_X::STATIC_DOWN )
            Settings::AntiAim::Pitch::type = AntiAimType_X::STATIC_DOWN;
    }

    if ( Settings::AntiAim::Yaw::enabled ) {
        if ( Settings::AntiAim::Lby::enabled && !bFlip ) {
            DoAntiAimLBY( angle, cmd->command_number, bFlip, should_clamp );
            Math::NormalizeAngles( angle );
            if ( !Settings::FakeLag::enabled )
                CreateMove::sendPacket = bFlip;
            if ( Settings::AntiAim::HeadEdge::enabled && edging_head && !bFlip )
                angle.y = edge_angle.y;
        } else {
            DoAntiAimY( angle, cmd->command_number, bFlip, should_clamp );
            Math::NormalizeAngles( angle );
            if ( !Settings::FakeLag::enabled )
                CreateMove::sendPacket = bFlip;
            if ( Settings::AntiAim::HeadEdge::enabled && edging_head && !bFlip )
                angle.y = edge_angle.y;
        }
        if ( Settings::AntiAim::SwitchAA::enabled )
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

    if ( Settings::AntiAim::Roll::enabled && Settings::AntiAim::allowUntrustedAngles )
        DoAntiAimZ( angle, cmd->command_number, should_clamp );
    if ( Settings::AntiAim::Pitch::enabled )
        DoAntiAimX( angle, bFlip, should_clamp );
    Math::NormalizeAngles( angle );
    if ( should_clamp ) {
        Math::ClampAngles( angle );
    }

    cmd->viewangles = angle;

    if ( Settings::AntiAim::Yaw::antiResolver ) {
        static bool antiResolverFlip = false;


        if ( cmd->viewangles.y == *localplayer->GetLowerBodyYawTarget() ) {
            if ( antiResolverFlip )
                cmd->viewangles.y += +90;
            else
                cmd->viewangles.y -= +90;

            antiResolverFlip = !antiResolverFlip;

            Math::NormalizeAngles( cmd->viewangles );

            if ( should_clamp ) {
                Math::ClampAngles( cmd->viewangles );
            }
        }
    }

    if ( bFlip ) {
        AntiAim::lastFakeYaw = cmd->viewangles.y;
    } else {
        AntiAim::lastRealYaw = cmd->viewangles.y;
    }

    Math::CorrectMovement( oldAngle, cmd, oldForward, oldSideMove );

}
