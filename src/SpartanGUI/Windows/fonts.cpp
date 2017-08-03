#include "fonts.h"

bool Fonts::showWindow = false;

void Fonts::RenderWindow()
{
	if (!Fonts::showWindow)
		return;
/*
	struct FontListVar
	{
		const char* name;
		union
		{
			FontVar* fontVarPtr;
			
		};
		enum
		{
			FONTVAR_TYPE,
			
		} type;

		FontListVar(const char* name, FontVar* ptr)
		{
			this->name = name;
			this->fontVarPtr = ptr;
			this->type = FONTVAR_TYPE;
		}

	};

	FontListVar Fonts[] = {
			{ "1", &Settings::Fonts::1 },
			{ "2", &Settings::Fonts::2 },
			{ "3", &Settings::Fonts::3 },
			{ "4", &Settings::Fonts::4 },
			{ "SegoeUI", &Settings::Fonts::SegoeUI },
		
	};

	const char* fontNames[IM_ARRAYSIZE(fonts)];
	for (int i = 0; i < IM_ARRAYSIZE(fonts); i++)
		fontNames[i] = fonts[i].name;

	static int fontSelected = 0;
*/
	ImGui::SetNextWindowSize(ImVec2(540,325), ImGuiSetCond_Always);
	if (ImGui::Begin("Fonts", &Fonts::showWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize))
	{
		ImGui::Columns(2, NULL, true);
		{
			ImGui::PushItemWidth(-1);
		//	ImGui::ListBox("##COLORSELECTION", &colorSelected, colorNames, IM_ARRAYSIZE(colorNames), 12);
			ImGui::PopItemWidth();
		}
		ImGui::NextColumn();
		{
		
		//		UI::ColorPicker4((float*)colors[colorSelected].colorVarPtr);
			//	ImGui::Checkbox("Rainbow", &colors[colorSelected].colorVarPtr->rainbow);
				ImGui::PushItemWidth(-1);

				ImGui::PopItemWidth();
			
		}
		ImGui::End();
	}
}
