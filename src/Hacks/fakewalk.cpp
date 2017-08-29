#include "fakewalk.h"

bool Settings::Fakewalk::enabled;
ButtonCode_t Settings::Fakewalk::key = ButtonCode_t::KEY_C;
bool Settings::SlowMo::enabled;
ButtonCode_t Settings::SlowMo::key = ButtonCode_t::KEY_C;



// I should rename this to pMemes


void Fakewalk::CreateMove(CUserCmd* cmd)
{
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	if (Settings::Fakewalk::enabled){
		

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon || activeWeapon->GetInReload())
		return;

	CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
	if (weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE || weaponType == CSWeaponType::WEAPONTYPE_KNIFE)
		return;

        ConVar* stopme = cvar->FindVar(XORSTR("sv_stopspeed"));
        ConVar* runsme = cvar->FindVar(XORSTR("sv_accelerate"));
        
        float stopfact = stopme->fValue;
        float stopspeed = runsme->fValue;
    	if (inputSystem->IsButtonDown(Settings::Fakewalk::key))
    	{
    		static int iChoked = -1;
    		iChoked++;
     
    		if (iChoked < 1)
    		{
    			CreateMove::sendPacket = false;
     
    			cmd->buttons |= localplayer->GetMoveType() == IN_BACK;
    			cmd->forwardmove = cmd->sidemove = 0.f;
                        
    		}
             
    		else
    		{       
                    if (localplayer->GetVelocity().Length2D()> 0.1f)
                    {
                     cmd->upmove = -localplayer->GetVelocity().Length2D();
                     
                     
                    }
                    
                    CreateMove::sendPacket = true;
    			iChoked = -1;
                        
    			
    			
                      cmd->buttons |= localplayer->GetMoveType() == IN_WALK;
                     // ik ik this still isnt even close to fakewalk but old meme fucked some shit up :eyo:
    		}
            }
        else if(Settings::SlowMo::enabled)
        {   
            
            	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
                if (!activeWeapon || activeWeapon->GetInReload())
		return;

            CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
            if (weaponType == CSWeaponType::WEAPONTYPE_C4 || weaponType == CSWeaponType::WEAPONTYPE_GRENADE || weaponType == CSWeaponType::WEAPONTYPE_KNIFE)
		return;

            
            if (inputSystem->IsButtonDown(Settings::SlowMo::key))
	{
		static bool slowmo;
		slowmo = !slowmo;
		if (slowmo)
		{
			cmd->tick_count = INT_MAX;
		}
	}
        } 
        
        else  
            return;
	
	
 }
 







