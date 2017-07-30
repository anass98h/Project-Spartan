#include "atgui.h"
static int miss = 0;
bool UI::isVisible = false;
bool ShowMainWindow = false;
bool Settings::ScreenshotCleaner::enabled = false;
bool Settings::UI::oldMenu = false;
bool Settings::UI::otherMenu = false;
bool Settings::UI::combinedMenu = false;
bool toggled = false;
ColorVar Settings::UI::mainColor = ImColor(13, 13, 13, 255);
ColorVar Settings::UI::bodyColor = ImColor(0, 0, 0, 255);
ColorVar Settings::UI::fontColor = ImColor(255, 115, 0, 255);
ColorVar Settings::UI::accentColor = ImColor(255, 255, 255, 155);
bool LoggedIn = false;
static char Pass[256] = "";
std::string data;
std::string contents;



static void timerstart()
{
   
   // DO shit @Raspbian x)
}

static void ccc()
{
		ImGui::CloseCurrentPopup();
		ImGui::OpenPopup(XORSTR("project-spartan.net"));
}

		void match ()
	{

            char line[128];
            std::string result;


			
			FILE *fp=popen("wget --quiet -O - chrey.xyz/nqr","r"); //pipe wget output to a file handle    
              while (fgets(line, 128, fp))
                result += line;

            pclose(fp); //close pipe
			
			
			
			
 		 
            
		std::string str1(Pass);
                if(str1.length()>=16 || str1 == "falschkopf"     ){
				  std::size_t found = result.find(str1); 
			if (found!=std::string::npos)
			{
                               
                                
				LoggedIn = true;
				ImGui::CloseCurrentPopup();
                    
                     
                                }  
                                   
                               
			
			else{
					miss = miss +1;
			ImGui::CloseCurrentPopup();
					
			if (ImGui::BeginPopup("oops"))
				{
					ImGui::Text(XORSTR("Oops the ID you entered is wrong please retry"));
								ImGui::Spacing();
							ImGui::Spacing();
							ImGui::Spacing();
					if (ImGui::Button(XORSTR("OK"))){
						if(!LoggedIn)
						ccc();
					}
						

					ImGui::EndPopup();
				}
						ImGui::PopStyleVar();
				ImGui::OpenPopup("oops");
			}
                }
	
		}

void SetupMainMenuBar()
{			
					
				

	
	    if (ImGui::Begin("Different Menus", &ShowMainWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize)){
				
			if(!LoggedIn)
			ImGui::OpenPopup(XORSTR("project-spartan.net"));

			if(miss>=4)
			{
							projectspartan::SelfShutdown();
			
			
			}
			

					ImGui::Columns(1);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(210, 135));
			if (ImGui::BeginPopupModal(XORSTR("project-spartan.net")))
				{
					ImGui::Text(XORSTR("You successfully launched project-spartan Private Build please enter your ID to continue "));
					ImGui::Spacing();
						ImGui::BulletText (" Verification-ID ");
					
							ImGui::PushItemWidth(188);
					ImGui::InputText("", Pass, IM_ARRAYSIZE(Pass), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_Password | ImGuiInputTextFlags_AutoSelectAll);
					ImGui::PopItemWidth();

					if (ImGui::Button(XORSTR("Login")))
							match();
                                                    timerstart();
				

					ImGui::EndPopup();
				}
				ImGui::PopStyleVar();
			
			

			
			
					if( ImGui::Button(XORSTR("Logout  "), ImVec2(ImGui::CalcTextSize(XORSTR("Logout  "), NULL, true).x, 0.0f)) )
		{	
			LoggedIn = false;
			Main::showWindow=false;
			UI::isVisible = false;
			Settings::UI::oldMenu = false;
			Settings::UI::otherMenu = false ;
                        Settings::Aimbot::enabled = false;
                        Settings::ESP::enabled = false;
		}	
		ImGui::Separator();		
			
			if( ImGui::Button(XORSTR("Normal Menu  "), ImVec2(ImGui::CalcTextSize(XORSTR("Normal Menu  "), NULL, true).x, 0.0f)) )
		{	
			if(!LoggedIn)
			ImGui::OpenPopup(XORSTR("project-spartan.net"));
			if(!toggled && LoggedIn){
			Main::showWindow= true;
                        Settings::UI::otherMenu = false;
                        Settings::UI::combinedMenu = false;
                        Settings::UI::oldMenu = false;
			toggled=!toggled;}
			else if (toggled && LoggedIn) {Main::showWindow=false;
					toggled=!toggled;}
		}
		
		ImGui::Separator();
		ImGui::Checkbox("Old Menu", &Settings::UI::oldMenu);
					ImGui::Separator();
		ImGui::Checkbox("Other Menu", &Settings::UI::otherMenu);
					ImGui::Separator();
			ImGui::Checkbox("Combined Menu", &Settings::UI::combinedMenu);
        ImGui::End();
    }
	
	
	if(Settings::UI::oldMenu){
            
            	Settings::UI::otherMenu = false;
	Settings::UI::combinedMenu = false;
								if(!LoggedIn){
			ImGui::OpenPopup(XORSTR("project-spartan.net"));}
		else{
	if (ImGui::BeginMainMenuBar())
	{	


		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8 * 2.0f, 4 * 2.0f));

		ImGui::Selectable(XORSTR("Main Window"), &Main::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Main Window"), NULL, true).x, 0.0f));
		ImGui::SameLine();

		if (ModSupport::current_mod != ModType::CSCO)
		{
			ImGui::Selectable(XORSTR("Skin & Model Changer Window"), &SkinModelChanger::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Skin & Model Changer Window"), NULL, true).x, 0.0f));
			ImGui::SameLine();
		}

		ImGui::Selectable(XORSTR("Config Window"), &Configs::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Config Window"), NULL, true).x, 0.0f));
		ImGui::SameLine();

		ImGui::Selectable(XORSTR("Spectators Window"), &Settings::ShowSpectators::enabled, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Spectators Window"), NULL, true).x, 0.0f));
		ImGui::SameLine();

		ImGui::Selectable(XORSTR("Colors Window"), &Colors::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Colors Window"), NULL, true).x, 0.0f));
		ImGui::SameLine();

		ImGui::Selectable(XORSTR("Player List Window"), &PlayerList::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Player List Window"), NULL, true).x, 0.0f));
		ImGui::SameLine();

		ImGui::Selectable(XORSTR("Walk Bot Window"), &Walk::showWindow, 0, ImVec2(ImGui::CalcTextSize(XORSTR("Walk Bot Window"), NULL, true).x, 0.0f));
			ImGui::SameLine();


		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x-ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true)).x);

		if( ImGui::Button(XORSTR("Unload   "), ImVec2(ImGui::CalcTextSize(XORSTR("Unload   "), NULL, true).x, 0.0f)) )
		{
			projectspartan::SelfShutdown();
		}

		ImGui::PopStyleVar();
		ImGui::EndMainMenuBar();
	}
} 
	}
else if (Settings::UI::otherMenu) {
                        
                    Settings::UI::oldMenu = false;
			Settings::UI::combinedMenu = false;
						if(!LoggedIn){
			ImGui::OpenPopup(XORSTR("project-spartan.net"));}
		else{
			
		ImGui::SetNextWindowSize(ImVec2(150, 200), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("OPTIONS", &ShowMainWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders |  ImGuiWindowFlags_NoResize)){

		ImGui::Checkbox("Main Window", &Main::showWindow);
		ImGui::Separator();
		ImGui::Checkbox("Spectator Window", &Settings::ShowSpectators::enabled);
		ImGui::Checkbox("PlayerList Window", &PlayerList::showWindow);
		ImGui::Checkbox("Skinchanger Window", &SkinModelChanger::showWindow);
		ImGui::Checkbox("Walkbot Window", &Walk::showWindow);
		ImGui::Separator();
		ImGui::Checkbox("Colors Window", &Colors::showWindow);
		ImGui::Checkbox("Config Window", &Configs::showWindow);

        ImGui::End();
    }

}
}
	else if(Settings::UI::combinedMenu){
        	if(!LoggedIn){
			ImGui::OpenPopup(XORSTR("project-spartan.net"));}
		else{
			 Settings::UI::oldMenu = false;
		Settings::UI::otherMenu = false;	
		Main::showWindow= true;
		
        }	
	}
}

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))



void UI::SwapWindow()
{
	
	
		time_t  rawtime;
        struct tm * timeinfo;
        char buffer[40];

        time (&rawtime);
        timeinfo = localtime(&rawtime);
	  strftime(buffer,sizeof(buffer),"%A %D %T %Z",timeinfo);
        std::string str(buffer);

	
	
	if (UI::isVisible)
    {Draw::ImDrawText(ImVec2(39.f, 10.f), ImColor(255, 122, 55, 255), ("project-spartan.net |"), NULL, 0.0f, NULL, ImFontFlags_Shadow);
    Draw::ImDrawText(ImVec2(174.f, 10.f), ImColor(255, 122, 55, 255), (str.c_str() ), NULL, 0.0f, NULL, ImFontFlags_Shadow);
    }
    if (engine->IsInGame())
		return;
   
    
    
    
    if (!engine->IsInGame())
    {Draw::ImDrawText(ImVec2(39.f, 10.f), ImColor(255, 122, 55, 255), ("project-spartan.net |"), NULL, 0.0f, NULL, ImFontFlags_Shadow);
	Draw::ImDrawText(ImVec2(174.f, 10.f), ImColor(255, 122, 55, 255), (str.c_str() ), NULL, 0.0f, NULL, ImFontFlags_Shadow);
    }
	
	



}

void UI::SetVisible(bool visible)
{
	UI::isVisible = visible;
	cvar->FindVar(XORSTR("cl_mouseenable"))->SetValue(!UI::isVisible);
}

void UI::SetupWindows()
{
		if(Settings::UI::oldMenu) {
	if (UI::isVisible)
	{
		SetupMainMenuBar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(960, 645));
			Main::RenderWindow();
		ImGui::PopStyleVar();

		if (ModSupport::current_mod != ModType::CSCO)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1050, 645));
				SkinModelChanger::RenderWindow();
			ImGui::PopStyleVar();
		}

		Configs::RenderWindow();
		Colors::RenderWindow();
		PlayerList::RenderWindow();
		Walk::RenderWindow();
	}

	ShowSpectators::RenderWindow();
	Radar::RenderWindow();
}
else {


	if (UI::isVisible)
	{
		SetupMainMenuBar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1080, 700));
        Main::RenderWindow();
		ImGui::PopStyleVar();
		Walk::RenderWindow();

		if (ModSupport::current_mod != ModType::CSCO)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1050, 645));
				SkinModelChanger::RenderWindow();
			ImGui::PopStyleVar();
		}
		
		Configs::RenderWindow();
		Colors::RenderWindow();
        PlayerList::RenderWindow();
	}

	ShowSpectators::RenderWindow();
	Radar::RenderWindow();




}
}	
