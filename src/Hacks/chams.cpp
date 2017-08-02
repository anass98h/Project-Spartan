#include "chams.h"

bool Settings::ESP::Chams::enabled = false;
bool Settings::ESP::Chams::Arms::enabled = false;
bool Settings::ESP::Chams::Weapon::enabled = false;
ArmsType Settings::ESP::Chams::Arms::type = ArmsType::DEFAULT;
HealthColorVar Settings::ESP::Chams::allyColor = ImColor(0, 0, 255, 255);
HealthColorVar Settings::ESP::Chams::allyVisibleColor = ImColor(0, 255, 0, 255);
HealthColorVar Settings::ESP::Chams::enemyColor = ImColor(255, 0, 0, 255);
HealthColorVar Settings::ESP::Chams::enemyVisibleColor = ImColor(255, 255, 0, 255);
HealthColorVar Settings::ESP::Chams::localplayerColor = ImColor(0, 255, 255, 255);
ColorVar Settings::ESP::Chams::Arms::color = ImColor(255, 255, 255, 255);
ColorVar Settings::ESP::Chams::Weapon::color = ImColor(255, 255, 255, 255);
ChamsType Settings::ESP::Chams::type = ChamsType::CHAMS;

IMaterial* materialChams;
IMaterial* materialChamsIgnorez;
IMaterial* materialChamsFlat;
IMaterial* materialChamsFlatIgnorez;
IMaterial* materialChamsGlass;
IMaterial* materialChamsCrystal;
IMaterial* materialChamsBlueCrystal;
IMaterial* materialChamsGold;
IMaterial* materialChamsVelvet;
IMaterial* materialChamsTree;
IMaterial* materialChamsSpeechInfo;
IMaterial* materialChamsFishNet;
IMaterial* materialChamsLetters;
IMaterial* materialChamsGloss;
IMaterial* materialChamsArms;
IMaterial* materialChamsWeapons;

static void DrawPlayer(void* thisptr, void* context, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!Settings::ESP::Chams::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer)
		return;

	C_BasePlayer* entity = (C_BasePlayer*) entityList->GetClientEntity(pInfo.entity_index);
	if (!entity
		|| entity->GetDormant()
		|| !entity->GetAlive())
		return;

	if (entity == localplayer && !Settings::ESP::Filters::localplayer)
		return;

	if (entity->GetTeam() != localplayer->GetTeam() && !Settings::ESP::Filters::enemies)
		return;

	if (entity != localplayer && entity->GetTeam() == localplayer->GetTeam() && !Settings::ESP::Filters::allies)
		return;

	IMaterial* visible_material;
	IMaterial* hidden_material;

	switch (Settings::ESP::Chams::type)
	{
		case ChamsType::CHAMS:
		case ChamsType::CHAMS_XQZ:
			visible_material = materialChams;
			hidden_material = materialChamsIgnorez;
			break;
		case ChamsType::CHAMS_FLAT:
		case ChamsType::CHAMS_FLAT_XQZ:
			visible_material = materialChamsFlat;
			hidden_material = materialChamsFlatIgnorez;
			break;
			case ChamsType::CHAMS_GLASS:
				visible_material = materialChamsGlass;
				hidden_material = materialChamsGlass;
		break;
		case ChamsType::CHAMS_CRYSTAL:
			visible_material = materialChamsCrystal;
			hidden_material = materialChamsCrystal;
			break;
	    	case ChamsType::CHAMS_CRYSTALBLUE:
			visible_material = materialChamsBlueCrystal;
			hidden_material = materialChamsBlueCrystal;
			break;
			case ChamsType::CHAMS_GOLD:
			visible_material = materialChamsGold;
			hidden_material = materialChamsGold;
			break;
			case ChamsType::CHAMS_VELVET:
			visible_material = materialChamsVelvet;
			hidden_material = materialChamsVelvet;
			break;	
            case ChamsType::CHAMS_TREE:
			visible_material = materialChamsTree;
			hidden_material = materialChamsTree;
			break;	
            case ChamsType::CHAMS_SPEECHINFO:
			visible_material = materialChamsSpeechInfo;
			hidden_material = materialChamsSpeechInfo;
			break;	
            case ChamsType::CHAMS_FISHNET:
			visible_material = materialChamsFishNet;
			hidden_material = materialChamsFishNet;
			break;                
            case ChamsType::CHAMS_LETTERS:
			visible_material = materialChamsLetters;
			hidden_material = materialChamsLetters;
			break;            
            case ChamsType::CHAMS_GLOSS:
			visible_material = materialChamsGloss;
			hidden_material = materialChamsGloss;
			break;  
	}

	visible_material->AlphaModulate(1.0f);
	hidden_material->AlphaModulate(1.0f);

	if (entity == localplayer)
	{
		Color visColor = Color::FromImColor(Settings::ESP::Chams::localplayerColor.Color(entity));
		Color color = visColor;
		color *= 0.45f;

		visible_material->ColorModulate(visColor);
		hidden_material->ColorModulate(color);
	}
	else if (entity->GetTeam() == localplayer->GetTeam())
	{
		Color visColor = Color::FromImColor(Settings::ESP::Chams::allyVisibleColor.Color(entity));
		Color color = Color::FromImColor(Settings::ESP::Chams::allyColor.Color(entity));

		visible_material->ColorModulate(visColor);
		hidden_material->ColorModulate(color);
	}
	else if (entity->GetTeam() != localplayer->GetTeam())
	{
		Color visColor = Color::FromImColor(Settings::ESP::Chams::enemyVisibleColor.Color(entity));
		Color color = Color::FromImColor(Settings::ESP::Chams::enemyColor.Color(entity));

		visible_material->ColorModulate(visColor);
		hidden_material->ColorModulate(color);
	}
	else
	{
		return;
	}

	if (entity->GetImmune())
	{
		visible_material->AlphaModulate(0.5f);
		hidden_material->AlphaModulate(0.5f);
	}

	if (!Settings::ESP::Filters::legit && (Settings::ESP::Chams::type == ChamsType::CHAMS_XQZ || Settings::ESP::Chams::type == ChamsType::CHAMS_FLAT_XQZ))
	{
		modelRender->ForcedMaterialOverride(hidden_material);
		modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(thisptr, context, state, pInfo, pCustomBoneToWorld);
	}

	modelRender->ForcedMaterialOverride(visible_material);
	modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(21)(thisptr, context, state, pInfo, pCustomBoneToWorld);
}

static void DrawWeapon(const ModelRenderInfo_t& pInfo)
{
	if (!Settings::ESP::Chams::Weapon::enabled)
		return;

	std::string modelName = modelInfo->GetModelName(pInfo.pModel);
	IMaterial* mat = materialChamsWeapons;

	if (!Settings::ESP::Chams::Weapon::enabled)
		mat = material->FindMaterial(modelName.c_str(), TEXTURE_GROUP_MODEL);

	mat->AlphaModulate(1.0f);
	mat->ColorModulate(Settings::ESP::Chams::Weapon::color.Color());

	modelRender->ForcedMaterialOverride(mat);
}

static void DrawArms(const ModelRenderInfo_t& pInfo)
{
	if (!Settings::ESP::Chams::Arms::enabled)
		return;

	std::string modelName = modelInfo->GetModelName(pInfo.pModel);
	IMaterial* mat = materialChamsArms;

	if (!Settings::ESP::Chams::Arms::enabled)
		mat = material->FindMaterial(modelName.c_str(), TEXTURE_GROUP_MODEL);

	switch (Settings::ESP::Chams::Arms::type)
	{
		default:
			break;
		case ArmsType::DEFAULT:
			mat->AlphaModulate(1.0f);
			mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
			break;
			case ArmsType::GLASS:
			mat = material->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
			mat->AlphaModulate(1.0f);
			mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
			break;
		case ArmsType::CRYSTAL:
			mat = material->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
			mat->AlphaModulate(1.0f);
			mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
			break;
			case ArmsType::CRYSTALBLUE:
			mat = material->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER);
			mat->AlphaModulate(1.0f);
			mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
			break;
			case ArmsType::GOLD:
			mat = material->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail", TEXTURE_GROUP_OTHER);
			mat->AlphaModulate(1.0f);
			mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
			break;
			case ArmsType::VELVET:
			mat = material->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER);
			mat->AlphaModulate(1.0f);
			mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
			break;	
            case ArmsType::TREE:
                mat = material->FindMaterial("models/props_foliage/urban_tree03_branches",TEXTURE_GROUP_OTHER);
                mat->AlphaModulate(1.0f);
                mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
                break;
            case ArmsType::SPEECHINFO:
                mat = material->FindMaterial("models/extras/speech_info",TEXTURE_GROUP_OTHER);
                mat->AlphaModulate(1.0f);
                mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
                break;
                case ArmsType::FISHNET:
                mat = material->FindMaterial("models/props_shacks/fishing_net01",TEXTURE_GROUP_OTHER);
                mat->AlphaModulate(1.0f);
                mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
                break;
                case ArmsType::LETTERS:
                mat = material->FindMaterial("models/inventory_items/contributor_map_tokens/contributor_charset_color",TEXTURE_GROUP_OTHER);
                mat->AlphaModulate(1.0f);
                mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
                break;
                case ArmsType::GLOSS:
                mat =  material->FindMaterial("models/inventory_items/trophy_majors/gloss",TEXTURE_GROUP_OTHER);
                mat->AlphaModulate(1.0f);
                mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color());
                break;
        
    }

	mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::ESP::Chams::Arms::type == ArmsType::WIREFRAME);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Settings::ESP::Chams::Arms::type == ArmsType::NONE);
	modelRender->ForcedMaterialOverride(mat);
}

void Chams::DrawModelExecute(void* thisptr, void* context, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!engine->IsInGame())
		return;

	if (!Settings::ESP::enabled)
		return;

	if (!pInfo.pModel)
		return;

	static bool materialsCreated = false;
	if (!materialsCreated)
	{
		materialChams = Util::CreateMaterial(XORSTR("VertexLitGeneric"), XORSTR("VGUI/white_additive"), false, true, true, true, true);
		materialChamsIgnorez = Util::CreateMaterial(XORSTR("VertexLitGeneric"), XORSTR("VGUI/white_additive"), true, true, true, true, true);
		materialChamsFlat = Util::CreateMaterial(XORSTR("UnlitGeneric"), XORSTR("VGUI/white_additive"), false, true, true, true, true);
		materialChamsFlatIgnorez = Util::CreateMaterial(XORSTR("UnlitGeneric"), XORSTR("VGUI/white_additive"), true, true, true, true, true);
		materialChamsArms = Util::CreateMaterial(XORSTR("VertexLitGeneric"), XORSTR("VGUI/white_additive"), false, true, true, true, true);
		materialChamsWeapons = Util::CreateMaterial(XORSTR("VertexLitGeneric"), XORSTR("VGUI/white_additive"), false, true, true, true, true);
		materialChamsGlass = material->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
		materialChamsCrystal = material->FindMaterial("models/inventory_items/trophy_majors/crystal_clear",TEXTURE_GROUP_OTHER);
		materialChamsBlueCrystal = material->FindMaterial("models/inventory_items/trophy_majors/crystal_blue",TEXTURE_GROUP_OTHER);
		materialChamsGold = material->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail",TEXTURE_GROUP_OTHER);
		materialChamsVelvet = material->FindMaterial("models/inventory_items/trophy_majors/velvet",TEXTURE_GROUP_OTHER);
        materialChamsTree = material->FindMaterial("models/props_foliage/urban_tree03_branches",TEXTURE_GROUP_OTHER);
		materialChamsSpeechInfo = material->FindMaterial("models/extras/speech_info",TEXTURE_GROUP_OTHER);
		materialChamsFishNet = material->FindMaterial("models/props_shacks/fishing_net01",TEXTURE_GROUP_OTHER);
        materialChamsLetters = material->FindMaterial("models/inventory_items/contributor_map_tokens/contributor_charset_color",TEXTURE_GROUP_OTHER);
        materialChamsGloss = material->FindMaterial("models/inventory_items/trophy_majors/gloss",TEXTURE_GROUP_OTHER);
        
        materialsCreated = true;
	}

	std::string modelName = modelInfo->GetModelName(pInfo.pModel);

	if (modelName.find(XORSTR("models/player")) != std::string::npos)
		DrawPlayer(thisptr, context, state, pInfo, pCustomBoneToWorld);
	else if (modelName.find(XORSTR("arms")) != std::string::npos)
		DrawArms(pInfo);
	else if (modelName.find(XORSTR("weapon")) != std::string::npos)
		DrawWeapon(pInfo);
}
