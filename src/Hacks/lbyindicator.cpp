#include "lbyindicator.h"


bool Settings::lbyindicator::enabled=false;
static bool cLowerBodyIsUpdated;
static float* storedYaw;
 static Color col;
static  float Weeb;
 

void lbyindicator::CreateMove(CUserCmd* cmd){
    
    
    
    
  if (!Settings::lbyindicator::enabled)
		return;

	if (!engine->IsInGame())
		return;
 C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	
	if (!pLocal)
		return;

	if (!pLocal->GetAlive())
		return;
       
    
            
        Weeb = (cmd->viewangles.y);

        if(std::abs(cmd->viewangles.y - *pLocal->GetLowerBodyYawTarget()) >= -35 && std::abs(cmd->viewangles.y - *pLocal->GetLowerBodyYawTarget()) <= 35)  {
          
            cLowerBodyIsUpdated =true;
           col = Color(255, 0, 0);
           storedYaw = pLocal->GetLowerBodyYawTarget();
        }
    
        else  {
            

              
            cLowerBodyIsUpdated = false;
            col = Color(0, 255, 0);
           storedYaw = pLocal->GetLowerBodyYawTarget();
        }
     
  
}

    
    
    

 void lbyindicator::Paint()
{
   
  
	 if (!Settings::lbyindicator::enabled)
		return;

	if (!engine->IsInGame())
		return;
   
 C_BasePlayer* pLocal = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

    	if (!pLocal)
		return;

	if (!pLocal->GetAlive())
		return;
       
              int width;
      int height;
	engine->GetScreenSize(width, height);
    
    int w = width - 150;
    int h = height - (height -(height*55/100));
     int w2 = width - 150;
    int h2 = height - (height -(height*51/100));
     int w3 = width - 150;
    int h3 = height - (height -(height*53/100));

    
             Draw::Text(w,  h, "LBY updated",lby_font,col);
        float Lobby = (*pLocal->GetLowerBodyYawTarget());
		
        std::ostringstream ss;
        ss <<"LBYValue = " <<Lobby;
        std::string s(ss.str());
		Draw::Text(w2,h2, s.c_str(), lby_font, Color(225,205,0));
       
        std::ostringstream ss2;
        ss2 <<"RealAngle = " <<Weeb;
        std::string s2(ss2.str());
		Draw::Text(w3,h3, s2.c_str(), lby_font, Color(225,205,0));
 
 
 
}

    


