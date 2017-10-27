#include "settings.h"

void GetVal( Json::Value& config, int* setting ) {
    if ( config.isNull() )
        return;

    *setting = config.asInt();
}

void GetVal( Json::Value& config, bool* setting ) {
    if ( config.isNull() )
        return;

    *setting = config.asBool();
}

void GetVal( Json::Value& config, float* setting ) {
    if ( config.isNull() )
        return;

    *setting = config.asFloat();
}

void GetVal( Json::Value& config, ImColor* setting ) {
    if ( config.isNull() )
        return;

    GetVal( config[XORSTR( "r" )], &setting->Value.x );
    GetVal( config[XORSTR( "g" )], &setting->Value.y );
    GetVal( config[XORSTR( "b" )], &setting->Value.z );
    GetVal( config[XORSTR( "a" )], &setting->Value.w );
}

void GetVal( Json::Value& config, char** setting ) {
    if ( config.isNull() )
        return;

    *setting = strdup( config.asCString() );
}

void GetVal( Json::Value& config, char* setting ) {
    if ( config.isNull() )
        return;

    strcpy( setting, config.asCString() );
}

void GetVal( Json::Value& config, ColorVar* setting ) {
    if ( config.isNull() )
        return;

    GetVal( config[XORSTR( "r" )], &setting->color.Value.x );
    GetVal( config[XORSTR( "g" )], &setting->color.Value.y );
    GetVal( config[XORSTR( "b" )], &setting->color.Value.z );
    GetVal( config[XORSTR( "a" )], &setting->color.Value.w );
    GetVal( config[XORSTR( "rainbow" )], &setting->rainbow );
    GetVal( config[XORSTR( "rainbowSpeed" )], &setting->rainbowSpeed );
}

void GetVal( Json::Value& config, HealthColorVar* setting ) {
    if ( config.isNull() )
        return;

    GetVal( config[XORSTR( "r" )], &setting->color.Value.x );
    GetVal( config[XORSTR( "g" )], &setting->color.Value.y );
    GetVal( config[XORSTR( "b" )], &setting->color.Value.z );
    GetVal( config[XORSTR( "a" )], &setting->color.Value.w );
    GetVal( config[XORSTR( "rainbow" )], &setting->rainbow );
    GetVal( config[XORSTR( "rainbowSpeed" )], &setting->rainbowSpeed );
    GetVal( config[XORSTR( "hp" )], &setting->hp );
}

template< typename Ord, Ord(* lookupFunction)( std::string ) >
void GetOrdinal( Json::Value& config, Ord* setting ) {
    if ( config.isNull() )
        return;

    Ord value;
    if ( config.isString() )
        value = lookupFunction( config.asString() );
    else
        value = ( Ord ) config.asInt();

    *setting = value;
}

void GetButtonCode( Json::Value& config, enum ButtonCode_t* setting ) {
    GetOrdinal<enum ButtonCode_t, Util::GetButtonCode>( config, setting );
}

void LoadColor( Json::Value& config, ImColor color ) {
    config[XORSTR( "r" )] = color.Value.x;
    config[XORSTR( "g" )] = color.Value.y;
    config[XORSTR( "b" )] = color.Value.z;
    config[XORSTR( "a" )] = color.Value.w;
}

void LoadColor( Json::Value& config, ColorVar color ) {
    config[XORSTR( "r" )] = color.color.Value.x;
    config[XORSTR( "g" )] = color.color.Value.y;
    config[XORSTR( "b" )] = color.color.Value.z;
    config[XORSTR( "a" )] = color.color.Value.w;
    config[XORSTR( "rainbow" )] = color.rainbow;
    config[XORSTR( "rainbowSpeed" )] = color.rainbowSpeed;
}

void LoadColor( Json::Value& config, HealthColorVar color ) {
    config[XORSTR( "r" )] = color.color.Value.x;
    config[XORSTR( "g" )] = color.color.Value.y;
    config[XORSTR( "b" )] = color.color.Value.z;
    config[XORSTR( "a" )] = color.color.Value.w;
    config[XORSTR( "rainbow" )] = color.rainbow;
    config[XORSTR( "rainbowSpeed" )] = color.rainbowSpeed;
    config[XORSTR( "hp" )] = color.hp;
}

void Settings::LoadDefaultsOrSave( std::string path ) {
    Json::Value settings;
    Json::StyledWriter styledWriter;

    LoadColor( settings[XORSTR( "UI" )][XORSTR( "mainColor" )], Settings::UI::mainColor );
    LoadColor( settings[XORSTR( "UI" )][XORSTR( "bodyColor" )], Settings::UI::bodyColor );
    LoadColor( settings[XORSTR( "UI" )][XORSTR( "fontColor" )], Settings::UI::fontColor );
    LoadColor( settings[XORSTR( "UI" )][XORSTR( "accentColor" )], Settings::UI::accentColor );
    settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "ESP" )][XORSTR( "family" )] = Settings::UI::Fonts::ESP::family;
    settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "ESP" )][XORSTR( "size" )] = Settings::UI::Fonts::ESP::size;
    settings[XORSTR( "UI" )][XORSTR( "Pie" )] = Settings::UI::Pie;
    settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "font" )] = ( int ) Settings::UI::Fonts::font;
    settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "fontsize" )] = Settings::UI::Fonts::fontsize;
    settings[XORSTR( "UI" )][XORSTR( "middle" )] = Settings::UI::middle;
    settings[XORSTR( "UI" )][XORSTR( "right" )] = Settings::UI::right;


    for ( auto i : Settings::Aimbot::weapons ) {
        // TODO this is kind of a hack and i'm too tired to find a better way to do this
        // yes i tried defining a variable, skinSetting, and giving it the same value but woooooo operator overloading
        // in C++ and weird shit
#define weaponSetting settings[XORSTR("Aimbot")][XORSTR("weapons")][Util::Items::GetItemName((enum ItemDefinitionIndex) i.first)]
        weaponSetting[XORSTR( "Enabled" )] = i.second.enabled;
        weaponSetting[XORSTR( "Silent" )] = i.second.silent;
        weaponSetting["pSilent"] = i.second.pSilent;
        weaponSetting["backtrack"] = i.second.backtrack;
        weaponSetting[XORSTR( "Friendly" )] = i.second.friendly;
        weaponSetting[XORSTR( "ClosestBone" )] = i.second.closestBone;
        weaponSetting[XORSTR( "engageLock" )] = i.second.engageLock;
        weaponSetting[XORSTR( "engageLockTR" )] = i.second.engageLockTR;
        weaponSetting[XORSTR( "engageLockTTR" )] = i.second.engageLockTTR;
        weaponSetting[XORSTR( "TargetBone" )] = ( int ) i.second.bone;
        weaponSetting[XORSTR( "AimKey" )] = Util::GetButtonName( i.second.aimkey );
        weaponSetting[XORSTR( "AimKeyOnly" )] = i.second.aimkeyOnly;
        weaponSetting[XORSTR( "Smooth" )][XORSTR( "Enabled" )] = i.second.smoothEnabled;
        weaponSetting[XORSTR( "Smooth" )][XORSTR( "Amount" )] = i.second.smoothAmount;
        weaponSetting[XORSTR( "Smooth" )][XORSTR( "Type" )] = ( int ) i.second.smoothType;
        weaponSetting[XORSTR( "Smooth" )][XORSTR( "Salting" )][XORSTR( "Enabled" )] = i.second.smoothSaltEnabled;
        weaponSetting[XORSTR( "Smooth" )][XORSTR( "Salting" )][XORSTR( "Multiplier" )] = i.second.smoothSaltMultiplier;
        weaponSetting[XORSTR( "ErrorMargin" )][XORSTR( "Enabled" )] = i.second.errorMarginEnabled;
        weaponSetting[XORSTR( "ErrorMargin" )][XORSTR( "Value" )] = i.second.errorMarginValue;
        weaponSetting[XORSTR( "AutoAim" )][XORSTR( "Enabled" )] = i.second.autoAimEnabled;
        weaponSetting[XORSTR( "AutoAim" )][XORSTR( "FOV" )] = i.second.autoAimFov;
        weaponSetting[XORSTR( "AimStep" )][XORSTR( "Enabled" )] = i.second.aimStepEnabled;
        weaponSetting[XORSTR( "AimStep" )][XORSTR( "min" )] = i.second.aimStepMin;
        weaponSetting[XORSTR( "AimStep" )][XORSTR( "max" )] = i.second.aimStepMax;
        weaponSetting[XORSTR( "RCS" )][XORSTR( "Enabled" )] = i.second.rcsEnabled;
        weaponSetting[XORSTR( "RCS" )][XORSTR( "AlwaysOn" )] = i.second.rcsAlwaysOn;
        weaponSetting[XORSTR( "RCS" )][XORSTR( "AmountX" )] = i.second.rcsAmountX;
        weaponSetting[XORSTR( "RCS" )][XORSTR( "AmountY" )] = i.second.rcsAmountY;
        weaponSetting[XORSTR( "AutoPistol" )][XORSTR( "Enabled" )] = i.second.autoPistolEnabled;
        weaponSetting[XORSTR( "AutoShoot" )][XORSTR( "Enabled" )] = i.second.autoShootEnabled;
        weaponSetting[XORSTR( "AutoScope" )][XORSTR( "Enabled" )] = i.second.autoScopeEnabled;
        weaponSetting[XORSTR( "NoShoot" )][XORSTR( "Enabled" )] = i.second.noShootEnabled;
        weaponSetting[XORSTR( "IgnoreJump" )][XORSTR( "Enabled" )] = i.second.ignoreJumpEnabled;
        weaponSetting[XORSTR( "SmokeCheck" )][XORSTR( "Enabled" )] = i.second.smokeCheck;
        weaponSetting[XORSTR( "FlashCheck" )][XORSTR( "Enabled" )] = i.second.flashCheck;
        weaponSetting[XORSTR( "SpreadLimit" )][XORSTR( "Enabled" )] = i.second.spreadLimitEnabled;
        weaponSetting[XORSTR( "SpreadLimit" )][XORSTR( "Value" )] = i.second.spreadLimit;
        weaponSetting[XORSTR( "AutoWall" )][XORSTR( "Enabled" )] = i.second.autoWallEnabled;
        weaponSetting[XORSTR( "AutoWall" )][XORSTR( "Value" )] = i.second.autoWallValue;
        weaponSetting[XORSTR( "AutoSlow" )][XORSTR( "enabled" )] = i.second.autoSlow;
        weaponSetting[XORSTR( "Prediction" )][XORSTR( "enabled" )] = i.second.predEnabled;
        weaponSetting[XORSTR( "Aimbot" )][XORSTR( "velocityCheck" )] = Settings::Aimbot::velocityCheck::enabled;
        weaponSetting[XORSTR( "legitMode" )] = i.second.legitMode;


        for ( int bone = ( int ) DesiredBones::BONE_PELVIS; bone <= ( int ) DesiredBones::BONE_RIGHT_SOLE; bone++ )
            weaponSetting[XORSTR( "DesiredBones" )][XORSTR( "Bones" )][bone] = i.second.desiredBones[bone];

        weaponSetting[XORSTR( "AutoAim" )][XORSTR( "RealDistance" )] = i.second.autoAimRealDistance;
        weaponSetting[XORSTR( "moveMouse" )] = i.second.moveMouse;
        weaponSetting[XORSTR( "HitChance" )][XORSTR( "enabled" )] = i.second.hitChanceEnabled;
        weaponSetting[XORSTR( "HitChance" )][XORSTR( "hitRays" )] = i.second.hitChanceRays;
        weaponSetting[XORSTR( "HitChance" )][XORSTR( "value" )] = i.second.hitChanceValue;
        weaponSetting[XORSTR( "AutoCockRevolver" )][XORSTR( "enabled" )] = i.second.autoCockRevolver;

#undef weaponSetting
    }

    settings[XORSTR( "Aimbot" )][XORSTR( "AutoCrouch" )][XORSTR( "enabled" )] = Settings::Aimbot::AutoCrouch::enabled;
    //settings[XORSTR( "Aimbot" )][XORSTR( "legitMode" )][XORSTR( "enabled" )] = Settings::Aimbot::legitMode::enabled;
    //settings[XORSTR("Aimbot")][XORSTR("AutoShoot")][XORSTR("velocityCheck")] = Settings::Aimbot::AutoShoot::velocityCheck;
    settings[XORSTR( "LBYIndicator" )][XORSTR( "enabled" )] = Settings::lbyindicator::enabled;
    settings[XORSTR( "Resolver" )][XORSTR( "enabled" )] =  Settings::Resolver::enabled;
    settings[XORSTR( "Resolver" )][XORSTR( "ticks" )] = Settings::Resolver::ticks;
    settings[XORSTR( "Resolver" )][XORSTR( "modulo" )] = Settings::Resolver::modulo;
    settings[XORSTR( "Resolver" )][XORSTR( "pitch" )] =  Settings::Resolver::pitch;
    settings[XORSTR( "Resolver" )][XORSTR( "LagComp" )] =  Settings::Resolver::LagComp;
    settings[XORSTR( "Resolver" )][XORSTR( "angleFlipEnabled" )] = Settings::Resolver::angleFlipEnabled;
    settings[XORSTR( "Resolver" )][XORSTR( "angleFlip" )] = Util::GetButtonName( Settings::Resolver::angleFlip );
    settings[XORSTR( "Resolver" )][XORSTR( "baimAfter" )] =  Settings::Resolver::baimAfter;
    settings[XORSTR( "Resolver" )][XORSTR( "lbyOnly" )] = Settings::Resolver::lbyOnly;
    settings[XORSTR( "Triggerbot" )][XORSTR( "enabled" )] = Settings::Triggerbot::enabled;
    settings[XORSTR( "Triggerbot" )][XORSTR( "key" )] = Util::GetButtonName( Settings::Triggerbot::key );
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "enemies" )] = Settings::Triggerbot::Filters::enemies;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "allies" )] = Settings::Triggerbot::Filters::allies;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "walls" )] = Settings::Triggerbot::Filters::walls;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR(
            "smoke_check" )] = Settings::Triggerbot::Filters::smokeCheck;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR(
            "flash_check" )] = Settings::Triggerbot::Filters::flashCheck;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "head" )] = Settings::Triggerbot::Filters::head;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "chest" )] = Settings::Triggerbot::Filters::chest;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "stomach" )] = Settings::Triggerbot::Filters::stomach;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "arms" )] = Settings::Triggerbot::Filters::arms;
    settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "legs" )] = Settings::Triggerbot::Filters::legs;
    settings[XORSTR( "Triggerbot" )][XORSTR( "RandomDelay" )][XORSTR(
            "enabled" )] = Settings::Triggerbot::RandomDelay::enabled;
    settings[XORSTR( "Triggerbot" )][XORSTR( "RandomDelay" )][XORSTR(
            "lowBound" )] = Settings::Triggerbot::RandomDelay::lowBound;
    settings[XORSTR( "Triggerbot" )][XORSTR( "RandomDelay" )][XORSTR(
            "highBound" )] = Settings::Triggerbot::RandomDelay::highBound;

    settings[XORSTR( "AngleIndicator" )][XORSTR( "enabled" )] = Settings::AngleIndicator::enabled;
    settings[XORSTR( "AngleIndicator" )][XORSTR( "Veloc" )] = Settings::AngleIndicator::Veloc;

    settings[XORSTR( "EventLogger" )][XORSTR( "enabled" )] = Settings::EventLogger::enabled;

    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Standing::Yaw::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Standing::Yaw::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR(
            "typeFake" )] = ( int ) Settings::AntiAim::Standing::Yaw::typeFake;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR(
            "typeAdd" )] = Settings::AntiAim::Standing::Yaw::typeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR(
            "typeFakeAdd" )] = Settings::AntiAim::Standing::Yaw::typeFakeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Pitch" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Standing::Pitch::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Pitch" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Standing::Pitch::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Pitch" )][XORSTR(
            "custom" )] = Settings::AntiAim::Standing::Pitch::custom;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Roll" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Standing::Roll::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Roll" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Standing::Roll::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "SwitchAA" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Standing::SwitchAA::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "SwitchAA" )][XORSTR(
            "key" )] = Settings::AntiAim::Standing::SwitchAA::key;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "LBY" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Standing::LBY::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "LBY" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Standing::LBY::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Standing::HeadEdge::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR(
            "distance" )] = Settings::AntiAim::Standing::HeadEdge::distance;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR(
            "realAdd" )] = Settings::AntiAim::Standing::HeadEdge::realAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR(
            "fakeAdd" )] = Settings::AntiAim::Standing::HeadEdge::fakeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR(
            "antiResolver" )] = Settings::AntiAim::Standing::antiResolver;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "dynamicAA" )] = Settings::AntiAim::Standing::dynamicAA;
    settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR(
            "untrustedAngles" )] = Settings::AntiAim::Standing::untrustedAngles;

    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Moving::Yaw::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Moving::Yaw::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR(
            "typeFake" )] = ( int ) Settings::AntiAim::Moving::Yaw::typeFake;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR(
            "typeAdd" )] = Settings::AntiAim::Moving::Yaw::typeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR(
            "typeFakeAdd" )] = Settings::AntiAim::Moving::Yaw::typeFakeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Pitch" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Moving::Pitch::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Pitch" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Moving::Pitch::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Pitch" )][XORSTR(
            "custom" )] = Settings::AntiAim::Moving::Pitch::custom;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Roll" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Moving::Roll::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Roll" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Moving::Roll::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "SwitchAA" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Moving::SwitchAA::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "SwitchAA" )][XORSTR(
            "key" )] = Settings::AntiAim::Moving::SwitchAA::key;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "LBY" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Moving::LBY::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "LBY" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Moving::LBY::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Moving::HeadEdge::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR(
            "distance" )] = Settings::AntiAim::Moving::HeadEdge::distance;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR(
            "realAdd" )] = Settings::AntiAim::Moving::HeadEdge::realAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR(
            "fakeAdd" )] = Settings::AntiAim::Moving::HeadEdge::fakeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR(
            "antiResolver" )] = Settings::AntiAim::Moving::antiResolver;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "dynamicAA" )] = Settings::AntiAim::Moving::dynamicAA;
    settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR(
            "untrustedAngles" )] = Settings::AntiAim::Moving::untrustedAngles;

    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Airborne::Yaw::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Airborne::Yaw::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR(
            "typeFake" )] = ( int ) Settings::AntiAim::Airborne::Yaw::typeFake;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR(
            "typeAdd" )] = Settings::AntiAim::Airborne::Yaw::typeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR(
            "typeFakeAdd" )] = Settings::AntiAim::Airborne::Yaw::typeFakeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Pitch" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Airborne::Pitch::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Pitch" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Airborne::Pitch::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Pitch" )][XORSTR(
            "custom" )] = Settings::AntiAim::Airborne::Pitch::custom;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Roll" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Airborne::Roll::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Roll" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Airborne::Roll::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "SwitchAA" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Airborne::SwitchAA::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "SwitchAA" )][XORSTR(
            "key" )] = Settings::AntiAim::Airborne::SwitchAA::key;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "LBY" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Airborne::LBY::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "LBY" )][XORSTR(
            "type" )] = ( int ) Settings::AntiAim::Airborne::LBY::type;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR(
            "enabled" )] = Settings::AntiAim::Airborne::HeadEdge::enabled;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR(
            "distance" )] = Settings::AntiAim::Airborne::HeadEdge::distance;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR(
            "fakeAdd" )] = Settings::AntiAim::Airborne::HeadEdge::fakeAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR(
            "realAdd" )] = Settings::AntiAim::Airborne::HeadEdge::realAdd;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR(
            "antiResolver" )] = Settings::AntiAim::Airborne::antiResolver;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "dynamicAA" )] = Settings::AntiAim::Airborne::dynamicAA;
    settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR(
            "untrustedAngles" )] = Settings::AntiAim::Airborne::untrustedAngles;

    settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR(
            "noEnemy" )] = Settings::AntiAim::Misc::AutoDisable::noEnemy;
    settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR(
            "knifeHeld" )] = Settings::AntiAim::Misc::AutoDisable::knifeHeld;
    settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR(
            "bombHeld" )] = Settings::AntiAim::Misc::AutoDisable::bombHeld;
    settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR(
            "freezeTime" )] = Settings::AntiAim::Misc::AutoDisable::freezeTime;

    settings[XORSTR( "ESP" )][XORSTR( "enabled" )] = Settings::ESP::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "key" )] = Util::GetButtonName( Settings::ESP::key );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "enemy_color" )], Settings::ESP::enemyColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "enemy_visible_color" )], Settings::ESP::enemyVisibleColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "ally_color" )], Settings::ESP::allyColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "ally_visible_color" )], Settings::ESP::allyVisibleColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "t_color" )], Settings::ESP::tColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "t_visible_color" )], Settings::ESP::tVisibleColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "ct_color" )], Settings::ESP::ctColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "ct_visible_color" )], Settings::ESP::ctVisibleColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "localplayer_color" )], Settings::ESP::localplayerColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "bomb_color" )], Settings::ESP::bombColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "bomb_defusing_color" )], Settings::ESP::bombDefusingColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "hostage_color" )], Settings::ESP::hostageColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "defuser_color" )], Settings::ESP::defuserColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "weapon_color" )], Settings::ESP::weaponColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "chicken_color" )], Settings::ESP::chickenColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "fish_color" )], Settings::ESP::fishColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "smoke_color" )], Settings::ESP::smokeColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "decoy_color" )], Settings::ESP::decoyColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "flashbang_color" )], Settings::ESP::flashbangColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "grenade_color" )], Settings::ESP::grenadeColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "molotov_color" )], Settings::ESP::molotovColor );
    settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "enabled" )] = Settings::ESP::Glow::enabled;
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "ally_color" )], Settings::ESP::Glow::allyColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "enemy_color" )], Settings::ESP::Glow::enemyColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "enemy_visible_color" )],
               Settings::ESP::Glow::enemyVisibleColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "localplayer_color" )],
               Settings::ESP::Glow::localplayerColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "weapon_color" )],
               Settings::ESP::Glow::weaponColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "grenade_color" )],
               Settings::ESP::Glow::grenadeColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "defuser_color" )],
               Settings::ESP::Glow::defuserColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "chicken_color" )],
               Settings::ESP::Glow::chickenColor );
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "legit" )] = Settings::ESP::Filters::legit;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR(
            "visibility_check" )] = Settings::ESP::Filters::visibilityCheck;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "smoke_check" )] = Settings::ESP::Filters::smokeCheck;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "enemies" )] = Settings::ESP::Filters::enemies;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "allies" )] = Settings::ESP::Filters::allies;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "bomb" )] = Settings::ESP::Filters::bomb;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "hostages" )] = Settings::ESP::Filters::hostages;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "defusers" )] = Settings::ESP::Filters::defusers;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "weapons" )] = Settings::ESP::Filters::weapons;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "chickens" )] = Settings::ESP::Filters::chickens;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "fishes" )] = Settings::ESP::Filters::fishes;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "throwables" )] = Settings::ESP::Filters::throwables;
    settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "localplayer" )] = Settings::ESP::Filters::localplayer;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "name" )] = Settings::ESP::Info::name;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "clan" )] = Settings::ESP::Info::clan;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "steam_id" )] = Settings::ESP::Info::steamId;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "rank" )] = Settings::ESP::Info::rank;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "health" )] = Settings::ESP::Info::health;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "weapon" )] = Settings::ESP::Info::weapon;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "scoped" )] = Settings::ESP::Info::scoped;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "reloading" )] = Settings::ESP::Info::reloading;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "flashed" )] = Settings::ESP::Info::flashed;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "planting" )] = Settings::ESP::Info::planting;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "has_defuser" )] = Settings::ESP::Info::hasDefuser;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "defusing" )] = Settings::ESP::Info::defusing;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "grabbing_hostage" )] = Settings::ESP::Info::grabbingHostage;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "rescuing" )] = Settings::ESP::Info::rescuing;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "location" )] = Settings::ESP::Info::location;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "lby" )] = Settings::ESP::Info::lby;
    settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "resolveInfo" )] = Settings::ESP::Info::resolveInfo;
    settings[XORSTR( "ESP" )][XORSTR( "Boxes" )][XORSTR( "enabled" )] = Settings::ESP::Boxes::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Boxes" )][XORSTR( "type" )] = ( int ) Settings::ESP::Boxes::type;
    settings[XORSTR( "ESP" )][XORSTR( "Skeleton" )][XORSTR( "enabled" )] = Settings::ESP::Skeleton::enabled;
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Skeleton" )][XORSTR( "color" )], Settings::ESP::Skeleton::color );
    settings[XORSTR( "ESP" )][XORSTR( "Bars" )][XORSTR( "enabled" )] = Settings::ESP::Bars::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Bars" )][XORSTR( "color_type" )] = ( int ) Settings::ESP::Bars::colorType;
    settings[XORSTR( "ESP" )][XORSTR( "Bars" )][XORSTR( "type" )] = ( int ) Settings::ESP::Bars::type;
    settings[XORSTR( "ESP" )][XORSTR( "ArmorBars" )][XORSTR( "enabled" )] = Settings::ESP::ArmorBars::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "ArmorBars" )][XORSTR( "type" )] = ( int ) Settings::ESP::ArmorBars::type;
    settings[XORSTR( "ESP" )][XORSTR( "Tracers" )][XORSTR( "enabled" )] = Settings::ESP::Tracers::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Tracers" )][XORSTR( "type" )] = ( int ) Settings::ESP::Tracers::type;
    settings[XORSTR( "ESP" )][XORSTR( "BulletTracers" )][XORSTR( "enabled" )] = Settings::ESP::BulletTracers::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "FOVCrosshair" )][XORSTR( "enabled" )] = Settings::ESP::FOVCrosshair::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "FOVCrosshair" )][XORSTR( "filled" )] = Settings::ESP::FOVCrosshair::filled;
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "FOVCrosshair" )][XORSTR( "color" )],
               Settings::ESP::FOVCrosshair::color );
    settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Arms" )][XORSTR(
            "enabled" )] = Settings::ESP::Chams::Arms::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Arms" )][XORSTR(
            "type" )] = ( int ) Settings::ESP::Chams::Arms::type;
    settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Weapon" )][XORSTR(
            "enabled" )] = Settings::ESP::Chams::Weapon::enabled;
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Weapon" )][XORSTR( "color" )],
               Settings::ESP::Chams::Weapon::color );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Arms" )][XORSTR( "color" )],
               Settings::ESP::Chams::Arms::color );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_ally_color" )],
               Settings::ESP::Chams::allyColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_ally_visible_color" )],
               Settings::ESP::Chams::allyVisibleColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_enemy_color" )],
               Settings::ESP::Chams::enemyColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_enemy_visible_color" )],
               Settings::ESP::Chams::enemyVisibleColor );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "localplayer_color" )],
               Settings::ESP::Chams::localplayerColor );
    settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "type" )] = ( int ) Settings::ESP::Chams::type;
    settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "enabled" )] = Settings::ESP::Chams::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Sounds" )][XORSTR( "enabled" )] = Settings::ESP::Sounds::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Sounds" )][XORSTR( "time" )] = Settings::ESP::Sounds::time;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "enabled" )] = Settings::ESP::Hitmarker::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "enemies" )] = Settings::ESP::Hitmarker::enemies;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "allies" )] = Settings::ESP::Hitmarker::allies;
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "color" )], Settings::ESP::Hitmarker::color );
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "duration" )] = Settings::ESP::Hitmarker::duration;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "size" )] = Settings::ESP::Hitmarker::size;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "inner_gap" )] = Settings::ESP::Hitmarker::innerGap;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "Damage" )][XORSTR(
            "enabled" )] = Settings::ESP::Hitmarker::Damage::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "Sounds" )][XORSTR(
            "enabled" )] = Settings::ESP::Hitmarker::Sounds::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "Sounds" )][XORSTR(
            "sound" )] = (int) Settings::ESP::Hitmarker::Sounds::sound;
    settings[XORSTR( "ESP" )][XORSTR( "HeadDot" )][XORSTR( "enabled" )] = Settings::ESP::HeadDot::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "HeadDot" )][XORSTR( "size" )] = Settings::ESP::HeadDot::size;
    settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "enabled" )] = Settings::ESP::Spread::enabled;
    settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "spreadLimit" )] = Settings::ESP::Spread::spreadLimit;
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "color" )], Settings::ESP::Spread::color );
    LoadColor( settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "spreadLimitColor" )],
               Settings::ESP::Spread::spreadLimitColor );
    settings[XORSTR( "TracerEffects" )][XORSTR( "enabled" )] = Settings::TracerEffects::enabled;
    settings[XORSTR( "TracerEffects" )][XORSTR( "serverSide" )] = Settings::TracerEffects::serverSide;
    settings[XORSTR( "TracerEffects" )][XORSTR( "effect" )] = ( int ) Settings::TracerEffects::effect;
    settings[XORSTR( "TracerEffects" )][XORSTR( "frequency" )] = Settings::TracerEffects::frequency;


    settings[XORSTR( "Dlights" )][XORSTR( "enabled" )] = Settings::Dlights::enabled;
    settings[XORSTR( "Dlights" )][XORSTR( "radius" )] = Settings::Dlights::radius;

    settings[XORSTR( "Spammer" )][XORSTR( "spammer_type" )] = ( int ) Settings::Spammer::type;
    settings[XORSTR( "Spammer" )][XORSTR( "say_team" )] = Settings::Spammer::say_team;

    settings[XORSTR( "Spammer" )][XORSTR( "KillSpammer" )][XORSTR(
            "enabled" )] = Settings::Spammer::KillSpammer::enabled;
    settings[XORSTR( "Spammer" )][XORSTR( "KillSpammer" )][XORSTR(
            "say_team" )] = Settings::Spammer::KillSpammer::sayTeam;
    Json::Value killSpammerMessages;
    for ( auto it : Settings::Spammer::KillSpammer::messages )
        killSpammerMessages.append( it );
    settings[XORSTR( "Spammer" )][XORSTR( "KillSpammer" )][XORSTR( "messages" )] = killSpammerMessages;

    Json::Value normalSpammerMessages;
    for ( auto it : Settings::Spammer::NormalSpammer::messages )
        normalSpammerMessages.append( it );
    settings[XORSTR( "Spammer" )][XORSTR( "NormalSpammer" )][XORSTR( "messages" )] = normalSpammerMessages;

    settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR(
            "show_name" )] = Settings::Spammer::PositionSpammer::showName;
    settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR(
            "show_weapon" )] = Settings::Spammer::PositionSpammer::showWeapon;
    settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR(
            "show_rank" )] = Settings::Spammer::PositionSpammer::showRank;
    settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR(
            "show_wins" )] = Settings::Spammer::PositionSpammer::showWins;
    settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR(
            "show_health" )] = Settings::Spammer::PositionSpammer::showHealth;
    settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR(
            "show_money" )] = Settings::Spammer::PositionSpammer::showMoney;
    settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR(
            "show_lastplace" )] = Settings::Spammer::PositionSpammer::showLastplace;

    settings[XORSTR( "BHop" )][XORSTR( "enabled" )] = Settings::BHop::enabled;

    settings[XORSTR( "AutoStrafe" )][XORSTR( "enabled" )] = Settings::AutoStrafe::enabled;
    settings[XORSTR( "AutoStrafe" )][XORSTR( "type" )] = ( int ) Settings::AutoStrafe::type;
    settings[XORSTR( "AutoStrafe" )][XORSTR( "silent" )] = Settings::AutoStrafe::silent;

    settings[XORSTR( "Noflash" )][XORSTR( "enabled" )] = Settings::Noflash::enabled;
    settings[XORSTR( "Noflash" )][XORSTR( "value" )] = Settings::Noflash::value;

    settings[XORSTR( "Radar" )][XORSTR( "enabled" )] = Settings::Radar::enabled;
    settings[XORSTR( "Radar" )][XORSTR( "zoom" )] = Settings::Radar::zoom;
    settings[XORSTR( "Radar" )][XORSTR( "enemies" )] = Settings::Radar::enemies;
    settings[XORSTR( "Radar" )][XORSTR( "allies" )] = Settings::Radar::allies;
    settings[XORSTR( "Radar" )][XORSTR( "legit" )] = Settings::Radar::legit;
    settings[XORSTR( "Radar" )][XORSTR( "visibility_check" )] = Settings::Radar::visibilityCheck;
    settings[XORSTR( "Radar" )][XORSTR( "smoke_check" )] = Settings::Radar::smokeCheck;
    settings[XORSTR( "Radar" )][XORSTR( "InGame" )][XORSTR( "enabled" )] = Settings::Radar::InGame::enabled;
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "enemy_color" )], Settings::Radar::enemyColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "enemy_visible_color" )], Settings::Radar::enemyVisibleColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "ally_color" )], Settings::Radar::allyColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "ally_visible_color" )], Settings::Radar::allyVisibleColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "t_color" )], Settings::Radar::tColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "t_visible_color" )], Settings::Radar::tVisibleColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "ct_color" )], Settings::Radar::ctColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "ct_visible_color" )], Settings::Radar::ctVisibleColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "bomb_color" )], Settings::Radar::bombColor );
    LoadColor( settings[XORSTR( "Radar" )][XORSTR( "bomb_defusing_color" )], Settings::Radar::bombDefusingColor );
    settings[XORSTR( "Radar" )][XORSTR( "icons_scale" )] = Settings::Radar::iconsScale;

    settings[XORSTR( "Recoilcrosshair" )][XORSTR( "enabled" )] = Settings::Recoilcrosshair::enabled;

    settings[XORSTR( "Recoilcrosshair" )][XORSTR(
            "showOnlyWhenShooting" )] = Settings::Recoilcrosshair::showOnlyWhenShooting;

    settings[XORSTR( "FOVChanger" )][XORSTR( "enabled" )] = Settings::FOVChanger::enabled;
    settings[XORSTR( "FOVChanger" )][XORSTR( "value" )] = Settings::FOVChanger::value;
    settings[XORSTR( "FOVChanger" )][XORSTR( "viewmodel_enabled" )] = Settings::FOVChanger::viewmodelEnabled;
    settings[XORSTR( "FOVChanger" )][XORSTR( "viewmodel_value" )] = Settings::FOVChanger::viewmodelValue;
    settings[XORSTR( "FOVChanger" )][XORSTR( "ignore_scope" )] = Settings::FOVChanger::ignoreScope;

    settings[XORSTR( "Airstuck" )][XORSTR( "enabled" )] = Settings::Airstuck::enabled;
    settings[XORSTR( "Airstuck" )][XORSTR( "key" )] = Util::GetButtonName( Settings::Airstuck::key );
    settings[XORSTR( "FakewalkAW" )][XORSTR( "enabled" )] = Settings::FakewalkAW::enabled;
    settings[XORSTR( "FakewalkAW" )][XORSTR( "key" )] = Util::GetButtonName( Settings::FakewalkAW::key );
    settings[XORSTR( "Fakewalk" )][XORSTR( "enabled" )] = Settings::Fakewalk::enabled;
    settings[XORSTR( "Fakewalk" )][XORSTR( "key" )] = Util::GetButtonName( Settings::Fakewalk::key );
    settings[XORSTR( "ChokePackets" )][XORSTR( "enabled" )] = Settings::ChokePackets::enabled;
    settings[XORSTR( "ChokePackets" )][XORSTR( "key" )] = Util::GetButtonName( Settings::ChokePackets::key );
    settings[XORSTR( "SlowMo" )][XORSTR( "enabled" )] = Settings::SlowMo::enabled;
    settings[XORSTR( "SlowMo" )][XORSTR( "key" )] = Util::GetButtonName( Settings::SlowMo::key );
    settings[XORSTR( "CircleStrafe" )][XORSTR( "enabled" )] = Settings::CircleStrafe::enabled;
    settings[XORSTR( "CircleStrafe" )][XORSTR( "key" )] = Util::GetButtonName( Settings::CircleStrafe::key );
    //settings[XORSTR("anglechange")][XORSTR("enabled")] = Settings::anglechange::enabled;
    //settings[XORSTR("anglechange")][XORSTR("key")] = Util::GetButtonName(Settings::anglechange::key);

    settings[XORSTR( "SkinChanger" )][XORSTR( "Skins" )][XORSTR( "enabled" )] = Settings::Skinchanger::Skins::enabled;
    settings[XORSTR( "SkinChanger" )][XORSTR( "Models" )][XORSTR( "enabled" )] = Settings::Skinchanger::Models::enabled;
    settings[XORSTR( "SkinChanger" )][XORSTR( "Skins" )][XORSTR( "perTeam" )] = Settings::Skinchanger::Skins::perTeam;

    for ( const auto& item : Settings::Skinchanger::skinsCT ) {
        const AttribItem_t& skin = item.second;

#define skinSetting settings[XORSTR("SkinChanger")][XORSTR("skinsCT")][Util::Items::GetItemConfigEntityName(item.first)]
        skinSetting[XORSTR( "ItemDefinitionIndex" )] = Util::Items::GetItemConfigEntityName( skin.itemDefinitionIndex );
        skinSetting[XORSTR( "PaintKit" )] = skin.fallbackPaintKit;
        skinSetting[XORSTR( "Wear" )] = skin.fallbackWear;
        skinSetting[XORSTR( "Seed" )] = skin.fallbackSeed;
        skinSetting[XORSTR( "StatTrak" )] = skin.fallbackStatTrak;
        skinSetting[XORSTR( "CustomName" )] = skin.customName;
#undef skinSetting
    }

    for ( const auto& item : Settings::Skinchanger::skinsT ) {
        const AttribItem_t& skin = item.second;

#define skinSetting settings[XORSTR("SkinChanger")][XORSTR("skinsT")][Util::Items::GetItemConfigEntityName(item.first)]
        skinSetting[XORSTR( "ItemDefinitionIndex" )] = Util::Items::GetItemConfigEntityName( skin.itemDefinitionIndex );
        skinSetting[XORSTR( "PaintKit" )] = skin.fallbackPaintKit;
        skinSetting[XORSTR( "Wear" )] = skin.fallbackWear;
        skinSetting[XORSTR( "Seed" )] = skin.fallbackSeed;
        skinSetting[XORSTR( "StatTrak" )] = skin.fallbackStatTrak;
        skinSetting[XORSTR( "CustomName" )] = skin.customName;
#undef skinSetting
    }

    settings[XORSTR( "ShowRanks" )][XORSTR( "enabled" )] = Settings::ShowRanks::enabled;
    settings[XORSTR( "SmartAim" )][XORSTR( "enabled" )] = Settings::SmartAim::enabled;

    settings[XORSTR( "ShowSpectators" )][XORSTR( "enabled" )] = Settings::ShowSpectators::enabled;

    settings[XORSTR( "ClanTagChanger" )][XORSTR( "value" )] = Settings::ClanTagChanger::value;
    settings[XORSTR( "ClanTagChanger" )][XORSTR( "enabled" )] = Settings::ClanTagChanger::enabled;
    settings[XORSTR( "ClanTagChanger" )][XORSTR( "animation" )] = Settings::ClanTagChanger::animation;
    settings[XORSTR( "ClanTagChanger" )][XORSTR( "animation_speed" )] = Settings::ClanTagChanger::animationSpeed;
    settings[XORSTR( "ClanTagChanger" )][XORSTR( "type" )] = ( int ) Settings::ClanTagChanger::type;

    settings[XORSTR( "View" )][XORSTR( "NoViewPunch" )][XORSTR( "enabled" )] = Settings::View::NoViewPunch::enabled;
    settings[XORSTR( "View" )][XORSTR( "NoAimPunch" )][XORSTR( "enabled" )] = Settings::View::NoAimPunch::enabled;

    settings[XORSTR( "FakeLag" )][XORSTR( "type" )] = ( int ) Settings::FakeLag::type;
    settings[XORSTR( "FakeLag" )][XORSTR( "value" )] = Settings::FakeLag::value;
    settings[XORSTR( "spinFactor" )][XORSTR( "value" )] = Settings::spinFactor::value;
    settings[XORSTR( "customYaw" )][XORSTR( "value" )] = Settings::customYaw::value;
    settings[XORSTR( "customYaw2" )][XORSTR( "value" )] = Settings::customYaw2::value;
    settings[XORSTR( "customYawLby" )][XORSTR( "enabled" )] = Settings::customYaw::lby;
    settings[XORSTR( "customYaw2Lby" )][XORSTR( "enabled" )] = Settings::customYaw2::lby;

    settings[XORSTR( "AutoAccept" )][XORSTR( "enabled" )] = Settings::AutoAccept::enabled;

    settings[XORSTR( "NoFog" )][XORSTR( "enabled" )] = Settings::NoFog::enabled;
    settings[XORSTR( "NoSky" )][XORSTR( "enabled" )] = Settings::NoSky::enabled;
    LoadColor( settings[XORSTR( "NoSky" )][XORSTR( "color" )], Settings::NoSky::color );

    settings[XORSTR( "SkyBox" )][XORSTR( "enabled" )] = Settings::SkyBox::enabled;
    settings[XORSTR( "SkyBox" )][XORSTR( "skyBoxNumber" )] = Settings::SkyBox::skyBoxNumber;

    settings[XORSTR( "ASUSWalls" )][XORSTR( "enabled" )] = Settings::ASUSWalls::enabled;
    LoadColor( settings[XORSTR( "ASUSWalls" )][XORSTR( "color" )], Settings::ASUSWalls::color );

    settings[XORSTR( "NoScopeBorder" )][XORSTR( "enabled" )] = Settings::NoScopeBorder::enabled;

    settings[XORSTR( "SniperCrosshair" )][XORSTR( "enabled" )] = Settings::SniperCrosshair::enabled;

    settings[XORSTR( "Autoblock" )][XORSTR( "enabled" )] = Settings::Autoblock::enabled;
    settings[XORSTR( "Autoblock" )][XORSTR( "key" )] = Settings::Autoblock::key;

    settings[XORSTR( "AutoDefuse" )][XORSTR( "enabled" )] = Settings::AutoDefuse::enabled;
    settings[XORSTR( "AutoDefuse" )][XORSTR( "silent" )] = Settings::AutoDefuse::silent;

    settings[XORSTR( "NoSmoke" )][XORSTR( "enabled" )] = Settings::NoSmoke::enabled;

    settings[XORSTR( "ScreenshotCleaner" )][XORSTR( "enabled" )] = Settings::ScreenshotCleaner::enabled;

    settings[XORSTR( "EdgeJump" )][XORSTR( "enabled" )] = Settings::EdgeJump::enabled;
    settings[XORSTR( "EdgeJump" )][XORSTR( "key" )] = Util::GetButtonName( Settings::EdgeJump::key );

    settings[XORSTR( "NameStealer" )][XORSTR( "enabled" )] = Settings::NameStealer::enabled;
    settings[XORSTR( "NameStealer" )][XORSTR( "team" )] = Settings::NameStealer::team;

    settings[XORSTR( "ThirdPerson" )][XORSTR( "enabled" )] = Settings::ThirdPerson::enabled;
    settings[XORSTR( "ThirdPerson" )][XORSTR( "realAngles" )] = Settings::ThirdPerson::realAngles;
    settings[XORSTR( "ThirdPerson" )][XORSTR( "distance" )] = Settings::ThirdPerson::distance;
    settings[XORSTR( "ThirdPerson" )][XORSTR( "key" )] = Settings::ThirdPerson::key;

    settings[XORSTR( "JumpThrow" )][XORSTR( "enabled" )] = Settings::JumpThrow::enabled;
    settings[XORSTR( "JumpThrow" )][XORSTR( "key" )] = Util::GetButtonName( Settings::JumpThrow::key );

    settings[XORSTR( "DisablePostProcessing" )][XORSTR( "enabled" )] = Settings::DisablePostProcessing::enabled;

    // settings[XORSTR("WalkBot")][XORSTR("enabled")] = Settings::WalkBot::enabled;
    //settings[XORSTR("WalkBot")][XORSTR("autobuy")] = Settings::WalkBot::autobuy;
    //settings[XORSTR("WalkBot")][XORSTR("autobuyAt")] = Settings::WalkBot::autobuyAt;

    settings[XORSTR( "GrenadeHelper" )][XORSTR( "enabled" )] = Settings::GrenadeHelper::enabled;
    settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimAssist" )] = Settings::GrenadeHelper::aimAssist;
    settings[XORSTR( "GrenadeHelper" )][XORSTR( "OnlyMatching" )] = Settings::GrenadeHelper::onlyMatchingInfos;
    settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimStep" )] = Settings::GrenadeHelper::aimStep;
    settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimDistance" )] = Settings::GrenadeHelper::aimDistance;
    settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimFov" )] = Settings::GrenadeHelper::aimFov;
    LoadColor( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimDot" )], Settings::GrenadeHelper::aimDot );
    LoadColor( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimLine" )], Settings::GrenadeHelper::aimLine );
    LoadColor( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoHe" )], Settings::GrenadeHelper::infoHE );
    LoadColor( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoSmoke" )], Settings::GrenadeHelper::infoSmoke );
    LoadColor( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoMolotov" )], Settings::GrenadeHelper::infoMolotov );
    LoadColor( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoFlash" )], Settings::GrenadeHelper::infoFlash );
    settings[XORSTR( "AutoKnife" )][XORSTR( "enabled" )] = Settings::AutoKnife::enabled;
    settings[XORSTR( "AutoKnife" )][XORSTR( "Filters" )][XORSTR( "enemies" )] = Settings::AutoKnife::Filters::enemies;
    settings[XORSTR( "AutoKnife" )][XORSTR( "Filters" )][XORSTR( "allies" )] = Settings::AutoKnife::Filters::allies;
    settings[XORSTR( "AutoKnife" )][XORSTR( "onKey" )] = Settings::AutoKnife::onKey;

    std::ofstream( path ) << styledWriter.write( settings );
}

void Settings::LoadConfig( std::string path ) {
    if ( !std::ifstream( path ).good() ) {
        Settings::LoadDefaultsOrSave( path );
        TracerEffect::RestoreTracers();
        return;
    }

    Json::Value settings;
    std::ifstream configDoc( path, std::ifstream::binary );
    configDoc >> settings;

    GetVal( settings[XORSTR( "UI" )][XORSTR( "mainColor" )], &Settings::UI::mainColor );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "bodyColor" )], &Settings::UI::bodyColor );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "fontColor" )], &Settings::UI::fontColor );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "accentColor" )], &Settings::UI::accentColor );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "ESP" )][XORSTR( "family" )],
            &Settings::UI::Fonts::ESP::family );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "ESP" )][XORSTR( "size" )],
            &Settings::UI::Fonts::ESP::size );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "ESP" )][XORSTR( "flags" )],
            &Settings::UI::Fonts::ESP::flags );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "font" )], ( int* ) &Settings::UI::Fonts::font );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "Fonts" )][XORSTR( "fontsize" )], &Settings::UI::Fonts::fontsize );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "middle" )], &Settings::UI::middle );
    GetVal( settings[XORSTR( "UI" )][XORSTR( "right" )], &Settings::UI::right );

    Fonts::SetupFonts();

    Settings::Aimbot::weapons = {
            { ItemDefinitionIndex::INVALID, { false, false, false, false, false,
                                              false, false, 700, Bone::BONE_HEAD, ButtonCode_t::MOUSE_MIDDLE,
                                              false, false, 1.0f, SmoothType::SLOW_END, false,
                                              0.0f, false, 0.0f, true, 180.0f,
                                              false, 25.0f, 35.0f, false, false,
                                              2.0f, 2.0f, false, false, false,
                                              false, false, false, false, false,
                                              0.1f, false, 10.0f, false, false,
                                              false, false, false, 100, 0.5f,
                                              false, false, false, false
                                            }
            },
    };

    for ( Json::ValueIterator itr = settings[XORSTR( "Aimbot" )][XORSTR( "weapons" )].begin();
          itr != settings[XORSTR( "Aimbot" )][XORSTR( "weapons" )].end(); itr++ ) {
        std::string weaponDataKey = itr.key().asString();
        auto weaponSetting = settings[XORSTR( "Aimbot" )][XORSTR( "weapons" )][weaponDataKey];

        // XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
        // XXX We can't use GetOrdinal() since the key type is a string...
        ItemDefinitionIndex weaponID;
        try {
            weaponID = ( ItemDefinitionIndex ) std::stoi( weaponDataKey );
        } catch ( std::invalid_argument ) // Not a number
        {
            weaponID = Util::Items::GetItemIndex( weaponDataKey );
        }

        if ( Settings::Aimbot::weapons.find( weaponID ) == Settings::Aimbot::weapons.end() )
            Settings::Aimbot::weapons[weaponID] = AimbotWeapon_t();

        AimbotWeapon_t weapon = {
                weaponSetting[XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "Silent" )].asBool(),
                weaponSetting[XORSTR( "pSilent" )].asBool(),
                weaponSetting[XORSTR( "Friendly" )].asBool(),
                weaponSetting[XORSTR( "ClosestBone" )].asBool(),
                weaponSetting[XORSTR( "engageLock" )].asBool(),
                weaponSetting[XORSTR( "engageLockTR" )].asBool(),
                weaponSetting[XORSTR( "engageLockTTR" )].asInt(),
                ( Bone ) weaponSetting[XORSTR( "TargetBone" )].asInt(),
                Util::GetButtonCode( weaponSetting[XORSTR( "AimKey" )].asCString() ),
                weaponSetting[XORSTR( "AimKeyOnly" )].asBool(),
                weaponSetting[XORSTR( "Smooth" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "Smooth" )][XORSTR( "Amount" )].asFloat(),
                ( SmoothType ) weaponSetting[XORSTR( "Smooth" )][XORSTR( "Type" )].asInt(),
                weaponSetting[XORSTR( "Smooth" )][XORSTR( "Salting" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "Smooth" )][XORSTR( "Salting" )][XORSTR( "Multiplier" )].asFloat(),
                weaponSetting[XORSTR( "ErrorMargin" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "ErrorMargin" )][XORSTR( "Value" )].asFloat(),
                weaponSetting[XORSTR( "AutoAim" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "AutoAim" )][XORSTR( "FOV" )].asFloat(),
                weaponSetting[XORSTR( "AimStep" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "AimStep" )][XORSTR( "min" )].asFloat(),
                weaponSetting[XORSTR( "AimStep" )][XORSTR( "max" )].asFloat(),
                weaponSetting[XORSTR( "RCS" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "RCS" )][XORSTR( "AlwaysOn" )].asBool(),
                weaponSetting[XORSTR( "RCS" )][XORSTR( "AmountX" )].asFloat(),
                weaponSetting[XORSTR( "RCS" )][XORSTR( "AmountY" )].asFloat(),
                weaponSetting[XORSTR( "AutoPistol" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "AutoShoot" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "AutoScope" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "NoShoot" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "IgnoreJump" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "SmokeCheck" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "FlashCheck" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "SpreadLimit" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "SpreadLimit" )][XORSTR( "Value" )].asFloat(),
                weaponSetting[XORSTR( "AutoWall" )][XORSTR( "Enabled" )].asBool(),
                weaponSetting[XORSTR( "AutoWall" )][XORSTR( "Value" )].asFloat(),
                weaponSetting[XORSTR( "AutoAim" )][XORSTR( "RealDistance" )].asBool(),
                weaponSetting[XORSTR( "AutoSlow" )][XORSTR( "enabled" )].asBool(),
                weaponSetting[XORSTR( "Prediction" )][XORSTR( "enabled" )].asBool(),
                weaponSetting[XORSTR( "moveMouse" )].asBool(),
                weaponSetting[XORSTR( "HitChance" )][XORSTR( "enabled" )].asBool(),
                weaponSetting[XORSTR( "HitChance" )][XORSTR( "hitRays" )].asInt(),
                weaponSetting[XORSTR( "HitChance" )][XORSTR( "value" )].asFloat(),
                weaponSetting[XORSTR( "AutoCockRevolver" )][XORSTR( "enabled" )].asBool(),
                weaponSetting[XORSTR( "velocityCheck" )][XORSTR( "enabled" )].asBool(),
                weaponSetting[XORSTR( "backtrack" )].asBool(),
                weaponSetting[XORSTR( "legitMode" )].asBool()

        };

        for ( int bone = ( int ) DesiredBones::BONE_PELVIS; bone <= ( int ) DesiredBones::BONE_RIGHT_SOLE; bone++ )
            weapon.desiredBones[bone] = weaponSetting[XORSTR( "DesiredBones" )][XORSTR( "Bones" )][bone].asBool();
        Settings::Aimbot::weapons.at( weaponID ) = weapon;
    }

    GetVal( settings[XORSTR( "Aimbot" )][XORSTR( "AutoCrouch" )][XORSTR( "enabled" )],
            &Settings::Aimbot::AutoCrouch::enabled );
   // GetVal( settings[XORSTR( "Aimbot" )][XORSTR( "legitMode" )][XORSTR( "enabled" )],
     //       &Settings::Aimbot::legitMode::enabled );
    GetVal( settings[XORSTR( "Aimbot" )][XORSTR( "velocityCheck" )], &Settings::Aimbot::velocityCheck::enabled );

    GetVal( settings[XORSTR( "Resolver" )][XORSTR( "enabled" )],  &Settings::Resolver::enabled );
    GetVal( settings[XORSTR( "Resolver" )][XORSTR( "ticks" )], &Settings::Resolver::ticks );
    GetVal( settings[XORSTR( "Resolver" )][XORSTR( "modulo" )], &Settings::Resolver::modulo );
    GetVal( settings[XORSTR( "Resolver" )][XORSTR( "pitch" )],  &Settings::Resolver::pitch );
    GetVal( settings[XORSTR( "Resolver" )][XORSTR( "LagComp" )], &Settings::Resolver::LagComp );
    GetVal( settings[XORSTR( "Resolver" )][XORSTR( "angleFlipEnabled" )], &Settings::Resolver::angleFlipEnabled );
    GetButtonCode( settings[XORSTR( "Resolver" )][XORSTR( "angleFlip" )], &Settings::Resolver::angleFlip );
    GetVal( settings[XORSTR( "Resolver" )][XORSTR( "baimAfter" )],  &Settings::Resolver::baimAfter );
    GetVal( settings[XORSTR( "Resolver")][ "lbyOnly" ], &Settings::Resolver::lbyOnly);
    GetVal( settings[XORSTR( "AngleIndicator" )][XORSTR( "enabled" )], &Settings::AngleIndicator::enabled );
    GetVal( settings[XORSTR( "AngleIndicator" )][XORSTR( "Veloc" )], &Settings::AngleIndicator::Veloc );
    GetVal( settings[XORSTR( "EventLogger" )][XORSTR( "enabled" )], &Settings::EventLogger::enabled );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "enabled" )], &Settings::Triggerbot::enabled );
    GetButtonCode( settings[XORSTR( "Triggerbot" )][XORSTR( "key" )], &Settings::Triggerbot::key );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "enemies" )],
            &Settings::Triggerbot::Filters::enemies );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "allies" )],
            &Settings::Triggerbot::Filters::allies );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "walls" )],
            &Settings::Triggerbot::Filters::walls );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "smoke_check" )],
            &Settings::Triggerbot::Filters::smokeCheck );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "flash_check" )],
            &Settings::Triggerbot::Filters::flashCheck );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "head" )],
            &Settings::Triggerbot::Filters::head );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "chest" )],
            &Settings::Triggerbot::Filters::chest );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "stomach" )],
            &Settings::Triggerbot::Filters::stomach );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "arms" )],
            &Settings::Triggerbot::Filters::arms );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "Filters" )][XORSTR( "legs" )],
            &Settings::Triggerbot::Filters::legs );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "RandomDelay" )][XORSTR( "enabled" )],
            &Settings::Triggerbot::RandomDelay::enabled );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "RandomDelay" )][XORSTR( "lowBound" )],
            &Settings::Triggerbot::RandomDelay::lowBound );
    GetVal( settings[XORSTR( "Triggerbot" )][XORSTR( "RandomDelay" )][XORSTR( "highBound" )],
            &Settings::Triggerbot::RandomDelay::highBound );

    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Standing::Yaw::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Standing::Yaw::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR( "typeFake" )],
            ( int* ) &Settings::AntiAim::Standing::Yaw::typeFake );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR( "typeAdd" )],
            &Settings::AntiAim::Standing::Yaw::typeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Yaw" )][XORSTR( "typeFakeAdd" )],
            &Settings::AntiAim::Standing::Yaw::typeFakeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Pitch" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Standing::Pitch::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Pitch" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Standing::Pitch::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Pitch" )][XORSTR( "custom" )],
            &Settings::AntiAim::Standing::Pitch::custom );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Roll" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Standing::Roll::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "Roll" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Standing::Roll::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "SwitchAA" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Standing::SwitchAA::enabled );
    GetButtonCode( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "SwitchAA" )][XORSTR( "key" )],
                   &Settings::AntiAim::Standing::SwitchAA::key );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "LBY" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Standing::LBY::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "LBY" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Standing::LBY::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Standing::HeadEdge::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR( "distance" )],
            &Settings::AntiAim::Standing::HeadEdge::distance );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR( "realAdd" )],
            &Settings::AntiAim::Standing::HeadEdge::realAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "HeadEdge" )][XORSTR( "fakeAdd" )],
            &Settings::AntiAim::Standing::HeadEdge::fakeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "antiResolver" )],
            &Settings::AntiAim::Standing::antiResolver );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "dynamicAA" )],
            &Settings::AntiAim::Standing::dynamicAA );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Standing" )][XORSTR( "untrustedAngles" )],
            &Settings::AntiAim::Standing::untrustedAngles );

    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Moving::Yaw::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Moving::Yaw::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR( "typeFake" )],
            ( int* ) &Settings::AntiAim::Moving::Yaw::typeFake );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR( "typeAdd" )],
            &Settings::AntiAim::Moving::Yaw::typeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Yaw" )][XORSTR( "typeFakeAdd" )],
            &Settings::AntiAim::Moving::Yaw::typeFakeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Pitch" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Moving::Pitch::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Pitch" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Moving::Pitch::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Pitch" )][XORSTR( "custom" )],
            &Settings::AntiAim::Moving::Pitch::custom );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Roll" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Moving::Roll::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "Roll" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Moving::Roll::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "SwitchAA" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Moving::SwitchAA::enabled );
    GetButtonCode( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "SwitchAA" )][XORSTR( "key" )],
                   &Settings::AntiAim::Moving::SwitchAA::key );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "LBY" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Moving::LBY::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "LBY" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Moving::LBY::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Moving::HeadEdge::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR( "distance" )],
            &Settings::AntiAim::Moving::HeadEdge::distance );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR( "realAdd" )],
            &Settings::AntiAim::Moving::HeadEdge::realAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "HeadEdge" )][XORSTR( "fakeAdd" )],
            &Settings::AntiAim::Moving::HeadEdge::fakeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "antiResolver" )],
            &Settings::AntiAim::Moving::antiResolver );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "dynamicAA" )],
            &Settings::AntiAim::Moving::dynamicAA );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Moving" )][XORSTR( "untrustedAngles" )],
            &Settings::AntiAim::Moving::untrustedAngles );

    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Airborne::Yaw::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Airborne::Yaw::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR( "typeFake" )],
            ( int* ) &Settings::AntiAim::Airborne::Yaw::typeFake );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR( "typeAdd" )],
            &Settings::AntiAim::Airborne::Yaw::typeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Yaw" )][XORSTR( "typeFakeAdd" )],
            &Settings::AntiAim::Airborne::Yaw::typeFakeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Pitch" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Airborne::Pitch::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Pitch" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Airborne::Pitch::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Pitch" )][XORSTR( "custom" )],
            &Settings::AntiAim::Airborne::Pitch::custom );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Roll" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Airborne::Roll::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "Roll" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Airborne::Roll::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "SwitchAA" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Airborne::SwitchAA::enabled );
    GetButtonCode( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "SwitchAA" )][XORSTR( "key" )],
                   &Settings::AntiAim::Airborne::SwitchAA::key );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "LBY" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Airborne::LBY::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "LBY" )][XORSTR( "type" )],
            ( int* ) &Settings::AntiAim::Airborne::LBY::type );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR( "enabled" )],
            &Settings::AntiAim::Airborne::HeadEdge::enabled );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR( "distance" )],
            &Settings::AntiAim::Airborne::HeadEdge::distance );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR( "fakeAdd" )],
            &Settings::AntiAim::Airborne::HeadEdge::fakeAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "HeadEdge" )][XORSTR( "realAdd" )],
            &Settings::AntiAim::Airborne::HeadEdge::realAdd );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "antiResolver" )],
            &Settings::AntiAim::Airborne::antiResolver );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "dynamicAA" )],
            &Settings::AntiAim::Airborne::dynamicAA );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Airborne" )][XORSTR( "untrustedAngles" )],
            &Settings::AntiAim::Airborne::untrustedAngles );

    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR( "noEnemy" )],
            &Settings::AntiAim::Misc::AutoDisable::noEnemy );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR( "knifeHeld" )],
            &Settings::AntiAim::Misc::AutoDisable::knifeHeld );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR( "bombHeld" )],
            &Settings::AntiAim::Misc::AutoDisable::bombHeld );
    GetVal( settings[XORSTR( "AntiAim" )][XORSTR( "Misc" )][XORSTR( "AutoDisable" )][XORSTR( "freezeTime" )],
            &Settings::AntiAim::Misc::AutoDisable::freezeTime );

    GetVal( settings[XORSTR( "ESP" )][XORSTR( "enabled" )], &Settings::ESP::enabled );
    GetButtonCode( settings[XORSTR( "ESP" )][XORSTR( "key" )], &Settings::ESP::key );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "enemy_color" )], &Settings::ESP::enemyColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "enemy_visible_color" )], &Settings::ESP::enemyVisibleColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "ally_color" )], &Settings::ESP::allyColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "ally_visible_color" )], &Settings::ESP::allyVisibleColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "t_color" )], &Settings::ESP::tColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "t_visible_color" )], &Settings::ESP::tVisibleColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "ct_color" )], &Settings::ESP::ctColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "ct_visible_color" )], &Settings::ESP::ctVisibleColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "localplayer_color" )], &Settings::ESP::localplayerColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "bomb_color" )], &Settings::ESP::bombColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "bomb_defusing_color" )], &Settings::ESP::bombDefusingColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "hostage_color" )], &Settings::ESP::hostageColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "defuser_color" )], &Settings::ESP::defuserColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "weapon_color" )], &Settings::ESP::weaponColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "chicken_color" )], &Settings::ESP::chickenColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "fish_color" )], &Settings::ESP::fishColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "smoke_color" )], &Settings::ESP::smokeColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "decoy_color" )], &Settings::ESP::decoyColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "flashbang_color" )], &Settings::ESP::flashbangColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "grenade_color" )], &Settings::ESP::grenadeColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "molotov_color" )], &Settings::ESP::molotovColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "enabled" )], &Settings::ESP::Glow::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "ally_color" )], &Settings::ESP::Glow::allyColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "enemy_color" )], &Settings::ESP::Glow::enemyColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "enemy_visible_color" )],
            &Settings::ESP::Glow::enemyVisibleColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "localplayer_color" )],
            &Settings::ESP::Glow::localplayerColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "weapon_color" )], &Settings::ESP::Glow::weaponColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "grenade_color" )],
            &Settings::ESP::Glow::grenadeColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "defuser_color" )],
            &Settings::ESP::Glow::defuserColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Glow" )][XORSTR( "chicken_color" )],
            &Settings::ESP::Glow::chickenColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "legit" )], &Settings::ESP::Filters::legit );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "visibility_check" )],
            &Settings::ESP::Filters::visibilityCheck );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "smoke_check" )],
            &Settings::ESP::Filters::smokeCheck );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "enemies" )], &Settings::ESP::Filters::enemies );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "allies" )], &Settings::ESP::Filters::allies );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "bomb" )], &Settings::ESP::Filters::bomb );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "hostages" )], &Settings::ESP::Filters::hostages );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "defusers" )], &Settings::ESP::Filters::defusers );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "weapons" )], &Settings::ESP::Filters::weapons );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "chickens" )], &Settings::ESP::Filters::chickens );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "fishes" )], &Settings::ESP::Filters::fishes );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "throwables" )],
            &Settings::ESP::Filters::throwables );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Filters" )][XORSTR( "localplayer" )],
            &Settings::ESP::Filters::localplayer );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "name" )], &Settings::ESP::Info::name );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "clan" )], &Settings::ESP::Info::clan );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "steam_id" )], &Settings::ESP::Info::steamId );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "rank" )], &Settings::ESP::Info::rank );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "health" )], &Settings::ESP::Info::health );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "weapon" )], &Settings::ESP::Info::weapon );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "scoped" )], &Settings::ESP::Info::scoped );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "reloading" )], &Settings::ESP::Info::reloading );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "flashed" )], &Settings::ESP::Info::flashed );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "planting" )], &Settings::ESP::Info::planting );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "has_defuser" )], &Settings::ESP::Info::hasDefuser );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "defusing" )], &Settings::ESP::Info::defusing );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "grabbing_hostage" )],
            &Settings::ESP::Info::grabbingHostage );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "rescuing" )], &Settings::ESP::Info::rescuing );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "location" )], &Settings::ESP::Info::location );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "lby" )], &Settings::ESP::Info::lby );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Info" )][XORSTR( "resolveInfo" )], &Settings::ESP::Info::resolveInfo );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Boxes" )][XORSTR( "enabled" )], &Settings::ESP::Boxes::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Boxes" )][XORSTR( "type" )], ( int* ) &Settings::ESP::Boxes::type );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Skeleton" )][XORSTR( "enabled" )], &Settings::ESP::Skeleton::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Skeleton" )][XORSTR( "color" )], &Settings::ESP::Skeleton::color );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Bars" )][XORSTR( "enabled" )], &Settings::ESP::Bars::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Bars" )][XORSTR( "color_type" )],
            ( int* ) &Settings::ESP::Bars::colorType );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Bars" )][XORSTR( "type" )], ( int* ) &Settings::ESP::Bars::type );

    GetVal( settings[XORSTR( "ESP" )][XORSTR( "ArmorBars" )][XORSTR( "enabled" )], &Settings::ESP::ArmorBars::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "ArmorBars" )][XORSTR( "type" )],
            ( int* ) &Settings::ESP::ArmorBars::type );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Tracers" )][XORSTR( "enabled" )], &Settings::ESP::Tracers::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Tracers" )][XORSTR( "type" )], ( int* ) &Settings::ESP::Tracers::type );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "BulletTracers" )][XORSTR( "enabled" )],
            &Settings::ESP::BulletTracers::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "FOVCrosshair" )][XORSTR( "enabled" )],
            &Settings::ESP::FOVCrosshair::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "FOVCrosshair" )][XORSTR( "filled" )],
            &Settings::ESP::FOVCrosshair::filled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "FOVCrosshair" )][XORSTR( "color" )],
            &Settings::ESP::FOVCrosshair::color );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Arms" )][XORSTR( "enabled" )],
            &Settings::ESP::Chams::Arms::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Arms" )][XORSTR( "type" )],
            ( int* ) &Settings::ESP::Chams::Arms::type );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Arms" )][XORSTR( "color" )],
            &Settings::ESP::Chams::Arms::color );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Weapon" )][XORSTR( "enabled" )],
            &Settings::ESP::Chams::Weapon::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "Weapon" )][XORSTR( "color" )],
            &Settings::ESP::Chams::Weapon::color );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_ally_color" )],
            &Settings::ESP::Chams::allyColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_ally_visible_color" )],
            &Settings::ESP::Chams::allyVisibleColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_enemy_color" )],
            &Settings::ESP::Chams::enemyColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "players_enemy_visible_color" )],
            &Settings::ESP::Chams::enemyVisibleColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "localplayer_color" )],
            &Settings::ESP::Chams::localplayerColor );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "type" )], ( int* ) &Settings::ESP::Chams::type );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Chams" )][XORSTR( "enabled" )], &Settings::ESP::Chams::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Sounds" )][XORSTR( "enabled" )], &Settings::ESP::Sounds::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Sounds" )][XORSTR( "time" )], &Settings::ESP::Sounds::time );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "enabled" )], &Settings::ESP::Hitmarker::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "enemies" )], &Settings::ESP::Hitmarker::enemies );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "allies" )], &Settings::ESP::Hitmarker::allies );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "color" )], &Settings::ESP::Hitmarker::color );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "duration" )],
            &Settings::ESP::Hitmarker::duration );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "size" )], &Settings::ESP::Hitmarker::size );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "inner_gap" )],
            &Settings::ESP::Hitmarker::innerGap );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "Damage" )][XORSTR( "enabled" )],
            &Settings::ESP::Hitmarker::Damage::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "Sounds" )][XORSTR( "enabled" )],
            &Settings::ESP::Hitmarker::Sounds::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Hitmarker" )][XORSTR( "Sounds" )][XORSTR( "sound" )],
            (int*)  &Settings::ESP::Hitmarker::Sounds::sound );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "HeadDot" )][XORSTR( "enabled" )], &Settings::ESP::HeadDot::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "HeadDot" )][XORSTR( "size" )], &Settings::ESP::HeadDot::size );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "enabled" )], &Settings::ESP::Spread::enabled );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "spreadLimit" )],
            &Settings::ESP::Spread::spreadLimit );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "color" )], &Settings::ESP::Spread::color );
    GetVal( settings[XORSTR( "ESP" )][XORSTR( "Spread" )][XORSTR( "spreadLimitColor" )],
            &Settings::ESP::Spread::spreadLimitColor );

    GetVal( settings[XORSTR( "LBYIndicator" )][XORSTR( "enabled" )], &Settings::lbyindicator::enabled );
    GetVal( settings[XORSTR( "TracerEffects" )][XORSTR( "enabled" )], &Settings::TracerEffects::enabled );
    GetVal( settings[XORSTR( "TracerEffects" )][XORSTR( "serverSide" )], &Settings::TracerEffects::serverSide );
    GetVal( settings[XORSTR( "TracerEffects" )][XORSTR( "effect" )], ( int* ) &Settings::TracerEffects::effect );
    GetVal( settings[XORSTR( "TracerEffects" )][XORSTR( "frequency" )], &Settings::TracerEffects::frequency );
    GetVal( settings[XORSTR( "lbyindicator" )][XORSTR( "enabled" )], &Settings::lbyindicator::enabled );

    GetVal( settings[XORSTR( "Dlights" )][XORSTR( "enabled" )], &Settings::Dlights::enabled );
    GetVal( settings[XORSTR( "Dlights" )][XORSTR( "radius" )], &Settings::Dlights::radius );

    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "spammer_type" )], ( int* ) &Settings::Spammer::type );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "say_team" )], &Settings::Spammer::say_team );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "KillSpammer" )][XORSTR( "enabled" )],
            &Settings::Spammer::KillSpammer::enabled );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "KillSpammer" )][XORSTR( "say_team" )],
            &Settings::Spammer::KillSpammer::sayTeam );
    if ( !settings[XORSTR( "Spammer" )][XORSTR( "KillSpammer" )][XORSTR( "messages" )].isNull() ) {
        Settings::Spammer::KillSpammer::messages.clear();
        for ( const Json::Value& message : settings[XORSTR( "Spammer" )][XORSTR( "KillSpammer" )][XORSTR(
                "messages" )] )
            Settings::Spammer::KillSpammer::messages.push_back( message.asString() );
    }
    if ( !settings[XORSTR( "Spammer" )][XORSTR( "NormalSpammer" )][XORSTR( "messages" )].isNull() ) {
        Settings::Spammer::NormalSpammer::messages.clear();
        for ( const Json::Value& message : settings[XORSTR( "Spammer" )][XORSTR( "NormalSpammer" )][XORSTR(
                "messages" )] )
            Settings::Spammer::NormalSpammer::messages.push_back( message.asString() );
    }
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR( "show_name" )],
            &Settings::Spammer::PositionSpammer::showName );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR( "show_weapon" )],
            &Settings::Spammer::PositionSpammer::showWeapon );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR( "show_rank" )],
            &Settings::Spammer::PositionSpammer::showRank );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR( "show_wins" )],
            &Settings::Spammer::PositionSpammer::showWins );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR( "show_health" )],
            &Settings::Spammer::PositionSpammer::showHealth );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR( "show_money" )],
            &Settings::Spammer::PositionSpammer::showMoney );
    GetVal( settings[XORSTR( "Spammer" )][XORSTR( "PositionSpammer" )][XORSTR( "show_lastplace" )],
            &Settings::Spammer::PositionSpammer::showLastplace );

    GetVal( settings[XORSTR( "BHop" )][XORSTR( "enabled" )], &Settings::BHop::enabled );

    GetVal( settings[XORSTR( "AutoStrafe" )][XORSTR( "enabled" )], &Settings::AutoStrafe::enabled );
    GetVal( settings[XORSTR( "AutoStrafe" )][XORSTR( "type" )], ( int* ) &Settings::AutoStrafe::type );
    GetVal( settings[XORSTR( "AutoStrafe" )][XORSTR( "silent" )], &Settings::AutoStrafe::silent );

    GetVal( settings[XORSTR( "Noflash" )][XORSTR( "enabled" )], &Settings::Noflash::enabled );
    GetVal( settings[XORSTR( "Noflash" )][XORSTR( "value" )], &Settings::Noflash::value );

    GetVal( settings[XORSTR( "Radar" )][XORSTR( "enabled" )], &Settings::Radar::enabled );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "zoom" )], &Settings::Radar::zoom );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "enemies" )], &Settings::Radar::enemies );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "allies" )], &Settings::Radar::allies );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "legit" )], &Settings::Radar::legit );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "visibility_check" )], &Settings::Radar::visibilityCheck );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "smoke_check" )], &Settings::Radar::smokeCheck );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "InGame" )][XORSTR( "enabled" )], &Settings::Radar::InGame::enabled );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "enemy_color" )], &Settings::Radar::enemyColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "enemy_visible_color" )], &Settings::Radar::enemyVisibleColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "ally_color" )], &Settings::Radar::allyColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "ally_visible_color" )], &Settings::Radar::allyVisibleColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "t_color" )], &Settings::Radar::tColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "t_visible_color" )], &Settings::Radar::tVisibleColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "ct_color" )], &Settings::Radar::ctColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "ct_visible_color" )], &Settings::Radar::ctVisibleColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "bomb_color" )], &Settings::Radar::bombColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "bomb_defusing_color" )], &Settings::Radar::bombDefusingColor );
    GetVal( settings[XORSTR( "Radar" )][XORSTR( "icons_scale" )], &Settings::Radar::iconsScale );


    GetVal( settings[XORSTR( "Recoilcrosshair" )][XORSTR( "enabled" )], &Settings::Recoilcrosshair::enabled );
    GetVal( settings[XORSTR( "Recoilcrosshair" )][XORSTR( "showOnlyWhenShooting" )],
            &Settings::Recoilcrosshair::showOnlyWhenShooting );

    GetVal( settings[XORSTR( "FOVChanger" )][XORSTR( "enabled" )], &Settings::FOVChanger::enabled );
    GetVal( settings[XORSTR( "FOVChanger" )][XORSTR( "value" )], &Settings::FOVChanger::value );
    GetVal( settings[XORSTR( "FOVChanger" )][XORSTR( "viewmodel_enabled" )], &Settings::FOVChanger::viewmodelEnabled );
    GetVal( settings[XORSTR( "FOVChanger" )][XORSTR( "viewmodel_value" )], &Settings::FOVChanger::viewmodelValue );
    GetVal( settings[XORSTR( "FOVChanger" )][XORSTR( "ignore_scope" )], &Settings::FOVChanger::ignoreScope );

    GetVal( settings[XORSTR( "Airstuck" )][XORSTR( "enabled" )], &Settings::Airstuck::enabled );
    GetButtonCode( settings[XORSTR( "Airstuck" )][XORSTR( "key" )], &Settings::Airstuck::key );
    GetVal( settings[XORSTR( "FakewalkAW" )][XORSTR( "enabled" )], &Settings::FakewalkAW::enabled );
    GetButtonCode( settings[XORSTR( "FakewalkAW" )][XORSTR( "key" )], &Settings::FakewalkAW::key );
    GetVal( settings[XORSTR( "Fakewalk" )][XORSTR( "enabled" )], &Settings::Fakewalk::enabled );
    GetButtonCode( settings[XORSTR( "Fakewalk" )][XORSTR( "key" )], &Settings::Fakewalk::key );
    GetVal( settings[XORSTR( "ChokePackets" )][XORSTR( "enabled" )], &Settings::ChokePackets::enabled );
    GetButtonCode( settings[XORSTR( "ChokePackets" )][XORSTR( "key" )], &Settings::ChokePackets::key );
    GetVal( settings[XORSTR( "SlowMo" )][XORSTR( "enabled" )], &Settings::SlowMo::enabled );
    GetButtonCode( settings[XORSTR( "SlowMo" )][XORSTR( "key" )], &Settings::SlowMo::key );
    GetVal( settings[XORSTR( "CircleStrafe" )][XORSTR( "enabled" )], &Settings::CircleStrafe::enabled );
    GetButtonCode( settings[XORSTR( "CircleStrafe" )][XORSTR( "key" )], &Settings::CircleStrafe::key );

    Settings::Skinchanger::Skins::enabled = false;
    Settings::Skinchanger::skinsCT.clear();
    Settings::Skinchanger::skinsT.clear();

    for ( Json::ValueIterator itr = settings[XORSTR( "SkinChanger" )][XORSTR( "skinsCT" )].begin();
          itr != settings[XORSTR( "SkinChanger" )][XORSTR( "skinsCT" )].end(); itr++ ) {
        std::string skinDataKey = itr.key().asString();
        auto skinSetting = settings[XORSTR( "SkinChanger" )][XORSTR( "skinsCT" )][skinDataKey];

        // XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
        // XXX We can't use GetOrdinal() since the key type is a string...
        unsigned int weaponID;

        try {
            weaponID = std::stoi( skinDataKey );
        } catch ( std::invalid_argument ) {
            weaponID = ( int ) Util::Items::GetItemIndex( skinDataKey );
        }

        ItemDefinitionIndex defIndex;
        GetOrdinal<ItemDefinitionIndex, Util::Items::GetItemIndex>( skinSetting[XORSTR( "ItemDefinitionIndex" )],
                                                                    &defIndex );

        if ( Settings::Skinchanger::skinsCT.find( ( ItemDefinitionIndex ) weaponID ) ==
             Settings::Skinchanger::skinsCT.end() )
            Settings::Skinchanger::skinsCT[( ItemDefinitionIndex ) weaponID] = AttribItem_t();

        AttribItem_t skin = {
                defIndex,
                skinSetting[XORSTR( "PaintKit" )].asInt(),
                skinSetting[XORSTR( "Wear" )].asFloat(),
                skinSetting[XORSTR( "Seed" )].asInt(),
                skinSetting[XORSTR( "StatTrak" )].asInt(),
                -1,
                skinSetting[XORSTR( "CustomName" )].asString(),
        };

        Settings::Skinchanger::skinsCT.at( ( ItemDefinitionIndex ) weaponID ) = skin;
    }

    for ( Json::ValueIterator itr = settings[XORSTR( "SkinChanger" )][XORSTR( "skinsT" )].begin();
          itr != settings[XORSTR( "SkinChanger" )][XORSTR( "skinsT" )].end(); itr++ ) {
        std::string skinDataKey = itr.key().asString();
        auto skinSetting = settings[XORSTR( "SkinChanger" )][XORSTR( "skinsT" )][skinDataKey];

        // XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
        // XXX We can't use GetOrdinal() since the key type is a string...
        unsigned int weaponID;

        try {
            weaponID = std::stoi( skinDataKey );
        } catch ( std::invalid_argument ) {
            weaponID = ( int ) Util::Items::GetItemIndex( skinDataKey );
        }

        ItemDefinitionIndex defIndex;
        GetOrdinal<ItemDefinitionIndex, Util::Items::GetItemIndex>( skinSetting[XORSTR( "ItemDefinitionIndex" )],
                                                                    &defIndex );

        if ( Settings::Skinchanger::skinsT.find( ( ItemDefinitionIndex ) weaponID ) ==
             Settings::Skinchanger::skinsT.end() )
            Settings::Skinchanger::skinsT[( ItemDefinitionIndex ) weaponID] = AttribItem_t();

        AttribItem_t skin = {
                defIndex,
                skinSetting[XORSTR( "PaintKit" )].asInt(),
                skinSetting[XORSTR( "Wear" )].asFloat(),
                skinSetting[XORSTR( "Seed" )].asInt(),
                skinSetting[XORSTR( "StatTrak" )].asInt(),
                -1,
                skinSetting[XORSTR( "CustomName" )].asString(),
        };

        Settings::Skinchanger::skinsT.at( ( ItemDefinitionIndex ) weaponID ) = skin;
    }

    SkinChanger::forceFullUpdate = true;

    GetVal( settings[XORSTR( "SkinChanger" )][XORSTR( "Skins" )][XORSTR( "enabled" )],
            &Settings::Skinchanger::Skins::enabled );
    GetVal( settings[XORSTR( "SkinChanger" )][XORSTR( "Models" )][XORSTR( "enabled" )],
            &Settings::Skinchanger::Models::enabled );
    GetVal( settings[XORSTR( "SkinChanger" )][XORSTR( "Skins" )][XORSTR( "perTeam" )],
            &Settings::Skinchanger::Skins::perTeam );

    GetVal( settings[XORSTR( "ShowRanks" )][XORSTR( "enabled" )], &Settings::ShowRanks::enabled );
    GetVal( settings[XORSTR( "SmartAim" )][XORSTR( "enabled" )], &Settings::SmartAim::enabled );

    GetVal( settings[XORSTR( "ShowSpectators" )][XORSTR( "enabled" )], &Settings::ShowSpectators::enabled );

    GetVal( settings[XORSTR( "ClanTagChanger" )][XORSTR( "value" )], ( char* ) &Settings::ClanTagChanger::value );
    GetVal( settings[XORSTR( "ClanTagChanger" )][XORSTR( "enabled" )], &Settings::ClanTagChanger::enabled );
    GetVal( settings[XORSTR( "ClanTagChanger" )][XORSTR( "animation" )], &Settings::ClanTagChanger::animation );
    GetVal( settings[XORSTR( "ClanTagChanger" )][XORSTR( "animation_speed" )],
            &Settings::ClanTagChanger::animationSpeed );
    GetVal( settings[XORSTR( "ClanTagChanger" )][XORSTR( "type" )], ( int* ) &Settings::ClanTagChanger::type );
    ::ClanTagChanger::UpdateClanTagCallback();

    GetVal( settings[XORSTR( "View" )][XORSTR( "NoViewPunch" )][XORSTR( "enabled" )],
            &Settings::View::NoViewPunch::enabled );
    GetVal( settings[XORSTR( "View" )][XORSTR( "NoAimPunch" )][XORSTR( "enabled" )],
            &Settings::View::NoAimPunch::enabled );

    GetVal( settings[XORSTR( "FakeLag" )][XORSTR( "type" )], ( int* ) &Settings::FakeLag::type );
    GetVal( settings[XORSTR( "FakeLag" )][XORSTR( "value" )], &Settings::FakeLag::value );
    GetVal( settings[XORSTR( "spinFactor" )][XORSTR( "value" )], &Settings::spinFactor::value );

    GetVal( settings[XORSTR( "AutoAccept" )][XORSTR( "enabled" )], &Settings::AutoAccept::enabled );

    GetVal( settings[XORSTR( "NoFog" )][XORSTR( "enabled" )], &Settings::NoFog::enabled);

    GetVal( settings[XORSTR( "NoSky" )][XORSTR( "enabled" )], &Settings::NoSky::enabled );
    GetVal( settings[XORSTR( "NoSky" )][XORSTR( "color" )], &Settings::NoSky::color );

    GetVal( settings[XORSTR( "SkyBox" )][XORSTR( "enabled" )], &Settings::SkyBox::enabled );
    GetVal( settings[XORSTR( "SkyBox" )][XORSTR( "skyBoxNumber" )], &Settings::SkyBox::skyBoxNumber );

    GetVal( settings[XORSTR( "ASUSWalls" )][XORSTR( "enabled" )], &Settings::ASUSWalls::enabled );
    GetVal( settings[XORSTR( "ASUSWalls" )][XORSTR( "color" )], &Settings::ASUSWalls::color );

    GetVal( settings[XORSTR( "NoScopeBorder" )][XORSTR( "enabled" )], &Settings::NoScopeBorder::enabled );

    GetVal( settings[XORSTR( "SniperCrosshair" )][XORSTR( "enabled" )], &Settings::SniperCrosshair::enabled );

    GetVal( settings[XORSTR( "Autoblock" )][XORSTR( "enabled" )], &Settings::Autoblock::enabled );
    GetButtonCode( settings[XORSTR( "Autoblock" )][XORSTR( "key" )], &Settings::Autoblock::key );

    GetVal( settings[XORSTR( "AutoDefuse" )][XORSTR( "enabled" )], &Settings::AutoDefuse::enabled );
    GetVal( settings[XORSTR( "AutoDefuse" )][XORSTR( "silent" )], &Settings::AutoDefuse::silent );

    GetVal( settings[XORSTR( "NoSmoke" )][XORSTR( "enabled" )], &Settings::NoSmoke::enabled );

    GetVal( settings[XORSTR( "ScreenshotCleaner" )][XORSTR( "enabled" )], &Settings::ScreenshotCleaner::enabled );

    GetVal( settings[XORSTR( "EdgeJump" )][XORSTR( "enabled" )], &Settings::EdgeJump::enabled );
    GetButtonCode( settings[XORSTR( "EdgeJump" )][XORSTR( "key" )], &Settings::EdgeJump::key );

    GetVal( settings[XORSTR( "NameStealer" )][XORSTR( "enabled" )], &Settings::NameStealer::enabled );
    GetVal( settings[XORSTR( "NameStealer" )][XORSTR( "team" )], &Settings::NameStealer::team );

    GetVal( settings[XORSTR( "ThirdPerson" )][XORSTR( "enabled" )], &Settings::ThirdPerson::enabled );
    GetVal( settings[XORSTR( "ThirdPerson" )][XORSTR( "realAngles" )], &Settings::ThirdPerson::realAngles );
    GetVal( settings[XORSTR( "ThirdPerson" )][XORSTR( "distance" )], &Settings::ThirdPerson::distance );
    GetButtonCode( settings[XORSTR( "ThirdPerson" )][XORSTR( "key" )], &Settings::ThirdPerson::key );

    GetVal( settings[XORSTR( "JumpThrow" )][XORSTR( "enabled" )], &Settings::JumpThrow::enabled );
    GetButtonCode( settings[XORSTR( "JumpThrow" )][XORSTR( "key" )], &Settings::JumpThrow::key );

    GetVal( settings[XORSTR( "DisablePostProcessing" )][XORSTR( "enabled" )],
            &Settings::DisablePostProcessing::enabled );

    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "enabled" )], &Settings::GrenadeHelper::enabled );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimAssist" )], &Settings::GrenadeHelper::aimAssist );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "OnlyMatching" )],
            &Settings::GrenadeHelper::onlyMatchingInfos );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimStep" )], &Settings::GrenadeHelper::aimStep );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimDistance" )], &Settings::GrenadeHelper::aimDistance );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimFov" )], &Settings::GrenadeHelper::aimFov );

    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimDot" )], &Settings::GrenadeHelper::aimDot );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "aimLine" )], &Settings::GrenadeHelper::aimLine );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoHE" )], &Settings::GrenadeHelper::infoHE );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoSmoke" )], &Settings::GrenadeHelper::infoSmoke );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoFlash" )], &Settings::GrenadeHelper::infoFlash );
    GetVal( settings[XORSTR( "GrenadeHelper" )][XORSTR( "infoMolotov" )], &Settings::GrenadeHelper::infoMolotov );
    GetVal( settings[XORSTR( "AutoKnife" )][XORSTR( "enabled" )], &Settings::AutoKnife::enabled );
    GetVal( settings[XORSTR( "AutoKnife" )][XORSTR( "Filters" )][XORSTR( "enemies" )],
            &Settings::AutoKnife::Filters::enemies );
    GetVal( settings[XORSTR( "AutoKnife" )][XORSTR( "Filters" )][XORSTR( "allies" )],
            &Settings::AutoKnife::Filters::allies );
    GetVal( settings[XORSTR( "AutoKnife" )][XORSTR( "onKey" )], &Settings::AutoKnife::onKey );

    // GetVal(settings[XORSTR("WalkBot")][XORSTR("enabled")], &Settings::WalkBot::enabled);
    //GetVal(settings[XORSTR("WalkBot")][XORSTR("autobuy")], &Settings::WalkBot::autobuy);
    //GetVal(settings[XORSTR("WalkBot")][XORSTR("autobuyAt")], &Settings::WalkBot::autobuyAt);


}

/*
void Settings::LoadSettings()
{
        pstring directory = getenv("HOME");
        directory << "/.config";

        if (!DoesDirectoryExist(directory.c_str()))
                mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        directory << "/Project Spartan/";

        if (!DoesDirectoryExist(directory.c_str()))
                mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
 */

void Settings::SaveGrenadeInfo( std::string path ) {
    Json::Value grenadeInfos;
    for ( auto grenadeInfo = GrenadeHelper::grenadeInfos.begin();
          grenadeInfo != GrenadeHelper::grenadeInfos.end(); grenadeInfo++ ) {
        Json::Value act;
        act[XORSTR( "name" )] = grenadeInfo->name.c_str();
        act[XORSTR( "gType" )] = grenadeInfo->gType;
        act[XORSTR( "tType" )] = grenadeInfo->tType;
        act[XORSTR( "pos" )][XORSTR( "x" )] = grenadeInfo->pos.x;
        act[XORSTR( "pos" )][XORSTR( "y" )] = grenadeInfo->pos.y;
        act[XORSTR( "pos" )][XORSTR( "z" )] = grenadeInfo->pos.z;

        act[XORSTR( "angle" )][XORSTR( "x" )] = grenadeInfo->angle.x;
        act[XORSTR( "angle" )][XORSTR( "y" )] = grenadeInfo->angle.y;

        grenadeInfos.append( act );
    }

    Json::Value data;
    Json::StyledWriter styledWriter;

    data[XORSTR( "smokeinfos" )] = grenadeInfos;

    std::ofstream( path ) << styledWriter.write( data );
}

void Settings::LoadGrenadeInfo( std::string path ) {
    if ( !std::ifstream( path ).good() || !DoesFileExist( path.c_str() ) )
        return;
    Json::Value data;
    std::ifstream configDoc( path, std::ifstream::binary );
    try {
        configDoc >> data;
    } catch ( ... ) {
        cvar->ConsoleDPrintf( XORSTR( "Error parsing the config file.\n" ) );
        return;
    }

    Json::Value array = data[XORSTR( "smokeinfos" )];
    Settings::GrenadeHelper::grenadeInfos = {};

    for ( Json::Value::iterator it = array.begin(); it != array.end(); ++it ) {
        Json::Value act = *it;
        const char* name = act[XORSTR( "name" )].asCString();
        GrenadeType gType = ( GrenadeType ) act[XORSTR( "gType" )].asInt();
        ThrowType tType = ( ThrowType ) act[XORSTR( "tType" )].asInt();
        Json::Value pos = act[XORSTR( "pos" )];
        Vector posVec = Vector( pos[XORSTR( "x" )].asFloat(), pos[XORSTR( "y" )].asFloat(),
                                pos[XORSTR( "z" )].asFloat() );
        Json::Value angle = act[XORSTR( "angle" )];
        QAngle vAngle = QAngle( angle[XORSTR( "x" )].asFloat(), angle[XORSTR( "y" )].asFloat(), 0 );
        Settings::GrenadeHelper::grenadeInfos.push_back( GrenadeInfo( gType, posVec, vAngle, tType, pstring( name ) ) );
    }
}

void remove_directory( const char* path ) {
    DIR* dir;
    dirent* pdir;

    dir = opendir( path );

    while ( ( pdir = readdir( dir ) ) ) {
        if ( strcmp( pdir->d_name, "." ) == 0 || strcmp( pdir->d_name, ".." ) == 0 )
            continue;

        if ( pdir->d_type == DT_DIR ) {
            pstring _dir;
            _dir << path << "/" << pdir->d_name;

            remove_directory( _dir.c_str() );
        } else if ( pdir->d_type == DT_REG ) {
            pstring file;
            file << path << "/" << pdir->d_name;

            unlink( file.c_str() );
        }
    }

    rmdir( path );
}

void Settings::DeleteConfig( std::string path ) {
    remove_directory( path.c_str() );
}
