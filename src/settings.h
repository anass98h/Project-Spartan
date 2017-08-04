#pragma once

#include <unordered_map>
#include <zconf.h>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "json/json.h"
#include "SDK/SDK.h"
#include "fonts.h"
#include "Utils/draw.h"
#include "Hacks/skinchanger.h"
#include "Utils/util.h"
#include "Utils/util_items.h"
#include "Utils/util_sdk.h"
#include "config.h"
#include "SpartanGUI/Spartangui.h"
#include "Hacks/esp.h"
#include "Hacks/lbyindicator.h"

enum class SmoothType : int {
    SLOW_END,
    CONSTANT,
    FAST_END,
};

enum class TracerType : int {
    BOTTOM,
    CURSOR,
};

enum class ClanTagType : int {
    STATIC,
    CURTIME,
    MARQUEE,
    WORDS,
    LETTERS,
};

enum class AutostrafeType : int {
    AS_FORWARDS,
    AS_BACKWARDS,
    AS_LEFTSIDEWAYS,
    AS_RIGHTSIDEWAYS,
    AS_RAGE,
};

enum class AntiAimType_Y : int {
    LEGITTROLLING,
    LEGITTROLLING2,
    NOAA,
    SPIN,
    APOSTROPHE,
    Tank,
    TANK2,
    TANK3,
    LBYBREAK,
    LBYSPIN,
    RANDOMBACKJITTER,
    CASUALJITTER,
    LBYJITTER,
    JITTER,
    BACKJITTER,
    SIDEWAYSRIGHT,
    SIDEWAYSLEFT,
    FAKESIDEWAYS,
    BACKWARDS,
    FORWARDS,
    STATICAA,
    STATICJITTER,
    STATICSMALLJITTER,
    LUA1, // ImGui stole the name of LUA
    LUA2, // Alternate LUA if you want a separate one for Fake.
    CASUAL,
    LISP,
    LISP_SIDE,
    LISP_JITTER,
    ANGEL_BACKWARD,
    ANGEL_INVERSE,
    ANGEL_SPIN,
    LOWERBODY,
    LBYONGROUND,
    LUA_UNCLAMPED,
    LUA_UNCLAMPED2
};

enum class AntiAimType_LBY : int {
    ONE,
    TWO,
    THREE,
    NONE,
};

enum class AntiAimType_Z : int {
    REVERSE,
    AUTISMFLIP,
    TEST,
};

enum class AntiAimType_X : int {
    STATIC_UP,
    FLIP,
    STATIC_DOWN,
    DANCE,
    FRONT,
    LUA1,
    STATIC_UP_FAKE,
    STATIC_DOWN_FAKE,
    LISP_DOWN,
    ANGEL_DOWN,
    ANGEL_UP,
    LUA_UNCLAMPED,
};

enum class ChamsType : int {
    CHAMS,
    CHAMS_XQZ,
    CHAMS_FLAT,
    CHAMS_FLAT_XQZ,
    CHAMS_GLASS,
    CHAMS_CRYSTAL,
    CHAMS_CRYSTALBLUE,
    CHAMS_GOLD,
    CHAMS_VELVET,
    CHAMS_TREE,
    CHAMS_SPEECHINFO,
    CHAMS_FISHNET,
    CHAMS_LETTERS,
    CHAMS_GLOSS,
};

enum class BoxType : int {
    FLAT_2D,
    FRAME_2D,
    BOX_3D,
    HITBOXES,
};

enum class BarType : int {
    VERTICAL,
    VERTICAL_RIGHT,
    HORIZONTAL,
    HORIZONTAL_UP,
    INTERWEBZ,
};

enum class BarColorType : int {
    STATIC_COLOR,
    HEALTH_BASED,
};

enum class TeamColorType : int {
    ABSOLUTE,
    RELATIVE,
};

enum class ArmsType : int {
    DEFAULT,
    GLASS,
    CRYSTAL,
    CRYSTALBLUE,
    GOLD,
    VELVET,
    TREE,
    SPEECHINFO,
    FISHNET,
    LETTERS,
    GLOSS,
    WIREFRAME,
    NONE,
};

enum class AimTargetType : int {
    FOV,
    DISTANCE,
    REAL_DISTANCE,
    HP,
};

enum class SpammerType : int {
    SPAMMER_NONE,
    SPAMMER_NORMAL,
    SPAMMER_POSITIONS,
};

enum class ResolverHugtype : int
{
	OFF,
	AIMTUX,
	PLUSDELTA,
	APOSTROPHE,
    BRUTE1,
	AUTISM,
};
struct AimbotWeapon_t
{
	bool enabled, silent, pSilent, friendly, closestBone, desiredBones[31], engageLock, engageLockTR;
	int engageLockTTR, hitChanceRays;
	Bone bone;
	SmoothType smoothType;
	ButtonCode_t aimkey;
	bool aimkeyOnly, smoothEnabled, smoothSaltEnabled, errorMarginEnabled, autoAimEnabled, aimStepEnabled, rcsEnabled, rcsAlwaysOn, spreadLimitEnabled;
	float smoothAmount, smoothSaltMultiplier, errorMarginValue, autoAimFov, aimStepMin, aimStepMax, rcsAmountX, rcsAmountY, autoWallValue, spreadLimit, hitChanceValue;
	bool autoPistolEnabled, autoShootEnabled, autoScopeEnabled, noShootEnabled, ignoreJumpEnabled, smokeCheck, flashCheck, autoWallEnabled, autoAimRealDistance, autoSlow, predEnabled, moveMouse, hitChanceEnabled;

	AimbotWeapon_t(bool enabled, bool silent, bool pSilent, bool friendly, bool closestBone, bool engageLock, bool engageLockTR, int engageLockTTR, Bone bone, ButtonCode_t aimkey, bool aimkeyOnly,
		   bool smoothEnabled, float smoothValue, SmoothType smoothType, bool smoothSaltEnabled, float smoothSaltMultiplier,
		   bool errorMarginEnabled, float errorMarginValue,
		   bool autoAimEnabled, float autoAimValue, bool aimStepEnabled, float aimStepMin, float aimStepMax,
		   bool rcsEnabled, bool rcsAlwaysOn, float rcsAmountX, float rcsAmountY,
		   bool autoPistolEnabled, bool autoShootEnabled, bool autoScopeEnabled,
		   bool noShootEnabled, bool ignoreJumpEnabled, bool smokeCheck, bool flashCheck,
		   bool spreadLimitEnabled, float spreadLimit,
		   bool autoWallEnabled, float autoWallValue, bool autoAimRealDistance, bool autoSlow,
		   bool predEnabled, bool moveMouse, bool hitChanceEnabled, int hitChanceRays, float hitChanceValue)
	{
		this->enabled = enabled;
		this->silent = silent;
		this->pSilent = pSilent;
		this->friendly = friendly;
		this->closestBone = closestBone;
		this->engageLock = engageLock;
		this->engageLockTR = engageLockTR;
		this->engageLockTTR = engageLockTTR;
		this->bone = bone;
		this->aimkey = aimkey;
		this->aimkeyOnly = aimkeyOnly;
		this->smoothEnabled = smoothEnabled;
		this->smoothAmount = smoothValue;
		this->smoothType = smoothType;
		this->smoothSaltEnabled = smoothSaltEnabled;
		this->smoothSaltMultiplier = smoothSaltMultiplier;
		this->errorMarginEnabled = errorMarginEnabled;
		this->errorMarginValue = errorMarginValue;
		this->autoAimEnabled = autoAimEnabled;
		this->autoAimFov = autoAimValue;
		this->aimStepEnabled = aimStepEnabled;
		this->aimStepMin = aimStepMin;
		this->aimStepMax = aimStepMax;
		this->rcsEnabled = rcsEnabled;
		this->rcsAlwaysOn = rcsAlwaysOn;
		this->rcsAmountX = rcsAmountX;
		this->rcsAmountY = rcsAmountY;
		this->autoPistolEnabled = autoPistolEnabled;
		this->autoShootEnabled = autoShootEnabled;
		this->autoScopeEnabled = autoScopeEnabled;
		this->noShootEnabled = noShootEnabled;
		this->ignoreJumpEnabled = ignoreJumpEnabled;
		this->smokeCheck = smokeCheck;
		this->flashCheck = flashCheck;
		this->spreadLimitEnabled = spreadLimitEnabled;
		this->spreadLimit = spreadLimit;
		this->autoWallEnabled = autoWallEnabled;
		this->autoWallValue = autoWallValue;
		this->autoSlow = autoSlow;
		this->predEnabled = predEnabled;
		this->moveMouse = moveMouse;
		this->hitChanceEnabled = hitChanceEnabled;

		for (int bone = (int) DesiredBones::BONE_PELVIS; bone <= (int) DesiredBones::BONE_RIGHT_SOLE; bone++)
			this->desiredBones[bone] = (desiredBones != nullptr ) ? desiredBones[bone] : false;

		this->autoAimRealDistance = autoAimRealDistance;
	}

	AimbotWeapon_t() {};

	bool operator == (const AimbotWeapon_t& another) const
	{
		for (int bone = (int) DesiredBones::BONE_PELVIS; bone <= (int) DesiredBones::BONE_RIGHT_SOLE; bone++)
		{
			if( this->desiredBones[bone] != another.desiredBones[bone] )
				return false;
		}

		return this->enabled == another.enabled &&
			this->silent == another.silent &&
			this->pSilent == another.pSilent &&
			this->friendly == another.friendly &&
			this->closestBone == another.closestBone &&
			this->engageLock == another.engageLock &&
			this->engageLockTR == another.engageLockTR &&
			this->engageLockTTR == another.engageLockTTR &&
			this->bone == another.bone &&
			this->aimkey == another.aimkey &&
			this->aimkeyOnly == another.aimkeyOnly &&
			this->smoothEnabled == another.smoothEnabled &&
			this->smoothAmount == another.smoothAmount &&
			this->smoothType == another.smoothType &&
			this->smoothSaltEnabled == another.smoothSaltEnabled &&
			this->smoothSaltMultiplier == another.smoothSaltMultiplier &&
			this->errorMarginEnabled == another.errorMarginEnabled &&
			this->errorMarginValue == another.errorMarginValue &&
			this->autoAimEnabled == another.autoAimEnabled &&
			this->autoAimFov == another.autoAimFov &&
			this->aimStepEnabled == another.aimStepEnabled &&
			this->aimStepMin == another.aimStepMin &&
			this->aimStepMax == another.aimStepMax &&
			this->rcsEnabled == another.rcsEnabled &&
			this->rcsAlwaysOn == another.rcsAlwaysOn &&
			this->rcsAmountX == another.rcsAmountX &&
			this->rcsAmountY == another.rcsAmountY &&
			this->autoPistolEnabled == another.autoPistolEnabled &&
			this->autoShootEnabled == another.autoShootEnabled &&
			this->autoScopeEnabled == another.autoScopeEnabled &&
			this->noShootEnabled == another.noShootEnabled &&
			this->ignoreJumpEnabled == another.ignoreJumpEnabled &&
			this->smokeCheck == another.smokeCheck &&
			this->flashCheck == another.flashCheck &&
			this->spreadLimitEnabled == another.spreadLimitEnabled &&
			this->spreadLimit == another.spreadLimit &&
			this->autoWallEnabled == another.autoWallEnabled &&
			this->autoWallValue == another.autoWallValue &&
			this->autoSlow == another.autoSlow &&
			this->predEnabled == another.predEnabled &&
			this->autoAimRealDistance == another.autoAimRealDistance &&
			this->moveMouse == another.moveMouse;
	}

};

class ColorVar {
public:
    ImColor color;
    bool rainbow;
    float rainbowSpeed;

    ColorVar() {
    }

    ColorVar(ImColor color) {
        this->color = color;
        this->rainbow = false;
        this->rainbowSpeed = 0.5f;
    }

    ImColor Color() {
        ImColor result = this->rainbow ? Util::GetRainbowColor(this->rainbowSpeed) : this->color;
        result.Value.w = this->color.Value.w;
        return result;
    }
};

class HealthColorVar : public ColorVar {
public:
    bool hp;

    HealthColorVar(ImColor color) {
        this->color = color;
        this->rainbow = false;
        this->rainbowSpeed = 0.5f;
        this->hp = false;
    }

    ImColor Color(C_BasePlayer* player) {
        ImColor result = this->rainbow ? Util::GetRainbowColor(this->rainbowSpeed) : (this->hp ? Color::ToImColor(Util::GetHealthColor(player)) : this->color);
        result.Value.w = this->color.Value.w;
        return result;
    }
};

namespace Settings {
    namespace UI {
        extern ColorVar mainColor;
        extern ColorVar bodyColor;
        extern ColorVar fontColor;
        extern ColorVar accentColor;
        extern bool oldMenu;
        extern bool otherMenu;
        extern bool combinedMenu;
        namespace Fonts {
            namespace ESP {
                extern char* family;
                extern int size;
                extern int flags;
            }
        }
    }

    namespace Aimbot {
        extern bool enabled;
        extern bool silent;
        extern bool pSilent;
        extern bool friendly;
        extern Bone bone;
        extern ButtonCode_t aimkey;
        extern bool aimkeyOnly;
        extern bool moveMouse;

        namespace Smooth {
            extern bool enabled;
            extern float value;
            extern SmoothType type;

            namespace Salting {
                extern bool enabled;
                extern float multiplier;
            }
        }

        namespace ErrorMargin {
            extern bool enabled;
            extern float value;
        }

        namespace AutoAim {
            extern bool enabled;
            extern float fov;
            extern bool realDistance;
            extern bool closestBone;
            extern bool desiredBones[];
            extern bool engageLock;
            extern bool engageLockTR;
            extern int engageLockTTR;
        }

        namespace AutoWall {
            extern bool enabled;
            extern float value;
        }

        namespace AimStep {
            extern bool enabled;
            extern float min;
            extern float max;
        }

        namespace RCS {
            extern bool enabled;
            extern bool always_on;
            extern float valueX;
            extern float valueY;
        }

        namespace AutoPistol {
            extern bool enabled;
        }

        namespace AutoShoot {
            extern bool enabled;
            extern bool autoscope;

        }

        namespace velocityCheck {
            extern bool enabled;



        }


        namespace AutoCrouch {
            extern bool enabled;
        }

        namespace AutoSlow {
            extern bool enabled;
            extern bool goingToSlow;
        }

        namespace NoShoot {
            extern bool enabled;
        }

        namespace IgnoreJump {
            extern bool enabled;
        }

        namespace SmokeCheck {
            extern bool enabled;
        }

        namespace FlashCheck {
            extern bool enabled;
        }

        namespace SpreadLimit {
            extern bool enabled;
            extern float value;
        }

        namespace HitChance {
            extern bool enabled;
            extern int hitRays;
            extern float value;
        }

        namespace Prediction {
            extern bool enabled;
        }

        extern std::unordered_map<ItemDefinitionIndex, AimbotWeapon_t, Util::IntHash<ItemDefinitionIndex>> weapons;
    }

    namespace Triggerbot {
        extern bool enabled;
        extern ButtonCode_t key;

        namespace Filters {
            extern bool enemies;
            extern bool allies;
            extern bool walls;
            extern bool smokeCheck;
            extern bool flashCheck;
            extern bool head;
            extern bool chest;
            extern bool stomach;
            extern bool arms;
            extern bool legs;
        }

        namespace RandomDelay {
            extern bool enabled;
            extern int lowBound; // in ms
            extern int highBound; // in ms
            extern int lastRoll;
        }
    }

    namespace AntiAim {
        namespace AutoDisable {
            extern bool noEnemy;
            extern bool knifeHeld;
            extern bool bombHeld;
        }

        namespace Yaw {
            extern bool enabled;
            extern AntiAimType_Y type;
            extern AntiAimType_Y typeFake;
            extern bool antiResolver;
            extern bool dynamicAA;
        }

		namespace HeadEdge
		{
			extern bool enabled;
			extern float distance;
		}

		namespace Lua
		{
			extern bool debugMode; // turns on/off error checking. Can be turned off after your script is working for speed.
			extern char scriptX[512];
			extern char scriptY[512];
			extern char scriptY2[512];
		}
	}

	namespace Resolver
	{	
		
		extern bool resolveAll;
		extern float ticks;
		extern float modulo;
		extern ResolverHugtype Hugtype;
		extern bool pitch;
	}
      namespace lbyindicator
		{
			extern bool enabled;

        }
    }

    namespace FakeLag {
        extern bool enabled;
        extern int value;
        extern bool adaptive;
    }

    namespace spinFactor {
        extern int value;
    }

    namespace AutoAccept {
        extern bool enabled;
    }

    namespace NoSky {
        extern bool enabled;
        extern ColorVar color;
    }

    namespace SkyBox {
        extern bool enabled;
        extern int skyBoxNumber; // number in skyBoxNames
    }

    namespace ASUSWalls {
        extern bool enabled;
        extern ColorVar color;
    }

    namespace NoScopeBorder {
        extern bool enabled;
    }

    namespace SniperCrosshair {
        extern bool enabled;
    }

    namespace AutoDefuse {
        extern bool enabled;
        extern bool silent;
    }

    namespace NoSmoke {
        extern bool enabled;
    }

    namespace ScreenshotCleaner {
        extern bool enabled;
    }

    namespace EdgeJump {
        extern bool enabled;
        extern ButtonCode_t key;
    }

    namespace NameStealer {
        extern bool enabled;
        extern int team;
    }

    namespace ThirdPerson {
        extern bool enabled;
        extern float distance;
        extern ButtonCode_t key;
    }

    namespace JumpThrow {
        extern bool enabled;
        extern ButtonCode_t key;
    }

    namespace DisablePostProcessing {
        extern bool enabled;
    }

    namespace GrenadeHelper {
        extern std::vector<GrenadeInfo> grenadeInfos;
        extern bool enabled;
        extern bool onlyMatchingInfos;
        extern bool aimAssist;
        extern float aimStep;
        extern float aimDistance;
        extern float aimFov;
        extern ColorVar aimDot;
        extern ColorVar aimLine;
        extern ColorVar infoHE;
        extern ColorVar infoSmoke;
        extern ColorVar infoFlash;
        extern ColorVar infoMolotov;
        extern pstring actMapName;
    }

    void LoadDefaultsOrSave(std::string path);
    void LoadConfig(std::string path);
    void LoadSettings();
    void DeleteConfig(std::string path);
    void SaveGrenadeInfo(std::string path);
    void LoadGrenadeInfo(std::string path);
}
