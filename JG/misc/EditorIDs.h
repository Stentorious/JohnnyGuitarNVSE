#include <unordered_map>

const UInt32 TESForm_Vtables[] =
{
	0x103168C,	//	TESWeather
	0x103140C,	//	TESWaterForm
	0x104D5B4,	//	TESTopicInfo
	0x104CC0C,	//	TESSkill
	0x104BA24,	//	TESReputation
	0x102397C,	//	TESRegion
	0x10369DC,	//	TESRecipeCategory
	0x1036B2C,	//	TESRecipe
	0x106847C,	//	TESPackage
	0x102C51C,	//	TESObjectWEAP
	0x102BC94,	//	TESObjectTREE
	0x102BA2C,	//	TESObjectSTAT
	0x102B844,	//	TESObjectMISC
	0x1028EE4,	//	TESObjectLIGH
	0x102DCD4,	//	TESObjectLAND
	0x102B5AC,	//	TESObjectIMOD
	0x102B1FC,	//	TESObjectDOOR
	0x102AEB4,	//	TESObjectCONT
	0x102A9C4,	//	TESObjectBOOK
	0x102A62C,	//	TESObjectARMO
	0x102A31C,	//	TESObjectARMA
	0x102A0A4,	//	TESObjectANIO
	0x1029D5C,	//	TESObjectACTI
	0x104A2F4,	//	TESNPC
	0x1036854,	//	TESLoadScreenType
	0x10366CC,	//	TESLoadScreen
	0x1028C5C,	//	TESLevSpell
	0x1028A64,	//	TESLevItem
	0x102886C,	//	TESLevCreature
	0x102864C,	//	TESLevCharacter
	0x102E6C4,	//	TESLandTexture
	0x1028444,	//	TESKey
	0x102D97C,	//	TESImageSpaceModifier
	0x102D7F4,	//	TESImageSpace
	0x1049B9C,	//	TESHair
	0x102814C,	//	TESGrass
	0x1026D0C,	//	TESFurniture
	0x10498DC,	//	TESFaction
	0x104973C,	//	TESEyes
	0x102685C,	//	TESEffectShader
	0x1048F5C,	//	TESCreature
	0x10266E4,	//	TESCombatStyle
	0x102D5C4,	//	TESClimate
	0x1048BB4,	//	TESClass
	0x104891C,	//	TESChallenge
	0x10263DC,	//	TESCasinoChips
	0x1026574,	//	TESCasino
	0x10349B4,	//	TESCaravanMoney
	0x1034B4C,	//	TESCaravanDeck
	0x103478C,	//	TESCaravanCard
	0x103449C,	//	TESAmmoEffect
	0x1026064,	//	TESAmmo
	0x1013F8C,	//	SpellItem
	0x1037094,	//	Script
	0x10342EC,	//	MediaSet
	0x10340C4,	//	MediaLocationController
	0x1012EA4,	//	EnchantmentItem
	0x1012834,	//	EffectSetting
	0x1033D1C,	//	BGSTextureSet
	0x1025914,	//	BGSTerminal
	0x1025594,	//	BGSTalkingActivator
	0x102535C,	//	BGSStaticCollection
	0x10116FC,	//	BGSSleepDeprevationStage
	0x10470EC,	//	BGSRagdoll
	0x10251AC,	//	BGSProjectile
	0x1024F4C,	//	BGSPlaceableWater
	0x1046EC4,	//	BGSPerk
	0x1046874,	//	BGSNote
	0x103397C,	//	BGSMusicType
	0x10337C4,	//	BGSMessage
	0x1033654,	//	BGSMenuIcon
	0x10334B4,	//	BGSListForm
	0x102CD94,	//	BGSLightingTemplate
	0x103323C,	//	BGSImpactDataSet
	0x1032F6C,	//	BGSImpactData
	0x104664C,	//	BGSIdleMarker
	0x10464B4,	//	BGSHeadPart
	0x1024A94,	//	BGSExplosion
	0x102CBBC,	//	BGSEncounterZone
	0x1024834,	//	BGSDebris
	0x10327F4,	//	BGSCameraShot
	0x103245C,	//	BGSCameraPath
	0x1045504,	//	BGSBodyPartData
	0x1024214,	//	BGSAddonNode
	0x10320FC,	//	BGSAcousticSpace
	0x1011964,	//	AlchemyItem
	0x1024CEC,  //  BGSMovableStatic
	0x1067A2C,  //  ActorValueInfo
	0x10115B4,  //	BGSHungerStage
	0x101144C,	//	BGSDehydrationStage
	0x1033B34,	//	BGSRadiationStage
};

//special for references, so it only uses persistent ones
const UInt32 TESObjectREFR_Vtables[] =
{
	0x102F55C,    //    TESObjectREFR
	0x1086A6C,    //    Character
	0x10870AC,    //    Creature
	0x108AA3C,    //    PlayerCharacter
	0x108F674,    //    GrenadeProjectile
};

extern NiTMap<const char*, TESForm*>** g_gameFormEditorIDsMap;

std::unordered_map<uint32_t, const char**> g_EditorNameMap;
std::mutex g_NameMapLock;

bool IsInserted(uint32_t id, const char** name) {
	auto itr = g_EditorNameMap.find(id);
	if (itr != g_EditorNameMap.end()) {
		if (strcmp(*itr->second, *name)) {
			PrintDebug("%08X - Tried to replace EDID \"%s\" with \"%s\"", id, *itr->second, *name);
			return true;
		}
	}
	else if (id) {
		PrintDebug("%08X - Inserted EDID \"%s\"", id, *name);
	}
	else {
		PrintDebug("We have a FormID of 0, this shouldn't happen.");
	}
	return false;
}

__declspec(naked) void GetNameHook() {
	__asm jmp TESForm::hk_GetName
}
__declspec(naked) void SetEditorIdHook() {
	__asm jmp TESForm::hk_SetEditorId
}
__declspec(naked) void REFRSetEditorIdHook() {
	__asm jmp TESForm::hk_REFRSetEditorID
}

void __fastcall AVInfoSetEditorIDHook(TESForm* form, UInt32 EDX, char* name) {
	form->SetEditorID(name);
}

const char** AddToGameMap(const char* name, TESForm* form) {
	ThisCall<NiTMap<const char*, TESForm*>::Entry*>(0x470200, *g_gameFormEditorIDsMap, name, form); // adds it to the game map
	auto* entry = (*g_gameFormEditorIDsMap)->LookupEntry(name);
	if (!entry) // shouldn't happen
		return nullptr;
	return &entry->key;
}

bool __fastcall TESQuestSetEditorIdHook(TESQuest* Form, UInt32 EDX, const char* Name) {
	if (!(((bool(__thiscall*)(TESQuest*, const char*))(0x60DAB0))(Form, Name))) return false;
	if (strcmp(Name, "SysWindowCompileAndRun")) {
		std::lock_guard<std::mutex> lock(g_NameMapLock);
		auto** name = AddToGameMap(Name, Form);
		if (name) {
#if _DEBUG
			IsInserted(Form->GetId(), name);
#endif
			g_EditorNameMap.insert(std::make_pair(Form->GetId(), name));
		}
			
	}
	return true;
}

// exported
UInt32 __cdecl JGNVSE_GetFormIDFromEDID(char* edid) {
	TESForm* form = ((TESForm * (__cdecl*)(char*))(0x483A00))(edid); //LookupEditorID
	if (form) {
		return form->refID;
	}
	return 0;
}

// vftable + 0x130
const char* TESForm::hk_GetName() {
	std::lock_guard<std::mutex> lock(g_NameMapLock);
	auto itr = g_EditorNameMap.find(GetId());

	if (itr != g_EditorNameMap.end() && *itr->second) //add failsafe
		return *itr->second;

	// By default the game returns an empty string
	return "";
}

// vftable + 0x134
bool TESForm::hk_SetEditorId(const char* Name) {
	if (strcmp(Name, "SysWindowCompileAndRun")) {
		std::lock_guard<std::mutex> lock(g_NameMapLock);
		auto** name = AddToGameMap(Name, this);
		if (name) {
#if _DEBUG
			IsInserted(GetId(), name);
#endif
			g_EditorNameMap.insert(std::make_pair(GetId(), name));
		}
	}
	return true;
}

bool TESForm::hk_REFRSetEditorID(const char* Name) {
	std::lock_guard<std::mutex> lock(g_NameMapLock);
	if ((refID < 0xFF000000) && ((flags & 0x420) == 0x400)) {
		auto** name = AddToGameMap(Name, this);
		if (name) {
#if _DEBUG
			IsInserted(GetId(), name);
#endif
			g_EditorNameMap.insert(std::make_pair(GetId(), name));
		}
	}
	return true;
}
const char* __fastcall ConsoleNameHook(TESObjectREFR* ref) {
	__try {
		const char* name = ref->baseForm->GetTheName();
		if (!strlen(name)) name = ref->baseForm->GetName();
		return name;
	}
	__except (EXCEPTION_ACCESS_VIOLATION) {
		return "";
	}
	return "";
}

void RemoveEDID(uint32_t id, bool removeFromGame) {
	auto itr = g_EditorNameMap.find(id);
	if (itr != g_EditorNameMap.end()) {
		if (removeFromGame) {
#if _DEBUG
			PrintDebug("%08X - Completely Removed EDID \"%s\"", id, *itr->second);
#endif
			ThisCall(0xE91FD0, *g_gameFormEditorIDsMap, itr->second);
		}
#if _DEBUG
		else {
			PrintDebug("%08X - Removed EDID \"%s\"", id, *itr->second);
		}
#endif
		g_EditorNameMap.erase(itr);
	}
}

void __fastcall TESDataHandler__RemoveIDFromDataHandler(void* apThis, void*, unsigned int aiID) {
	ThisCall(0x4696F0, apThis, aiID);

	std::lock_guard<std::mutex> lock(g_NameMapLock);
	RemoveEDID(aiID, true);
}

void __fastcall NiTMapBase_DWORD_DWORD___SetAt(void* apThis, void*, UInt32 key, TESForm* val) {
	ThisCall(0x844700, apThis, key, val);
	UInt32 newID = key;
	UInt32 oldID = val->GetId();
	std::lock_guard<std::mutex> lock(g_NameMapLock);
	auto itr = g_EditorNameMap.find(val->GetId());
	if (itr != g_EditorNameMap.end()) {
		// No removal from game map here because it's not being deleted
		const char** name = itr->second;
#if _DEBUG
		PrintDebug("%08X -> %08X - Removing stale FormID entry for \"%s\"", oldID, newID, *name);
#endif
		g_EditorNameMap.erase(itr);

		RemoveEDID(newID, false);

#if _DEBUG
		IsInserted(newID, name);
#endif
		g_EditorNameMap.insert(std::make_pair(newID, name));
	}
}

void LoadEditorIDs() {
	WriteRelCall(0x486903, (UInt32(GetNameHook))); // replaces empty string with editor id in TESForm::GetDebugName
	WriteRelCall(0x71B748, UInt32(ConsoleNameHook)); // replaces empty string with editor id in selected ref name in console
	WriteRelCall(0x710BFC, UInt32(ConsoleNameHook));
	WriteRelCall(0x55D498, (UInt32(GetNameHook))); // replaces empty string with editor id in TESObjectREFR::GetDebugName
	SafeWrite16(0x467A12, 0x3AEB); // loads more types in game's editor:form map

	WriteRelCall(0x483D12, UInt32(TESDataHandler__RemoveIDFromDataHandler)); // removes editor id in TESForm's destructor
	WriteRelCall(0x485D0B, UInt32(TESDataHandler__RemoveIDFromDataHandler)); // removes editor id in TESForm::SetFormID (only if the ID is being freed)
	WriteRelCall(0x485D24, UInt32(NiTMapBase_DWORD_DWORD___SetAt));			 // replaces editor id in TESForm::SetFormID

	for (uint32_t i = 0; i < ARRAYSIZE(TESForm_Vtables); i++) {
		if (*(uintptr_t*)(TESForm_Vtables[i] + 0x130) == 0x00401280)
			SafeWrite32(TESForm_Vtables[i] + 0x130, (UInt32)GetNameHook);

		if (*(uintptr_t*)(TESForm_Vtables[i] + 0x134) == 0x00401290)
			SafeWrite32(TESForm_Vtables[i] + 0x134, (UInt32)SetEditorIdHook);
	}
	for (uint32_t i = 0; i < ARRAYSIZE(TESObjectREFR_Vtables); i++) {
		if (*(uintptr_t*)(TESObjectREFR_Vtables[i] + 0x130) == 0x00401280)
			SafeWrite32(TESObjectREFR_Vtables[i] + 0x130, (UInt32)GetNameHook);

		if (*(uintptr_t*)(TESObjectREFR_Vtables[i] + 0x134) == 0x00401290)
			SafeWrite32(TESObjectREFR_Vtables[i] + 0x134, (UInt32)REFRSetEditorIdHook);
	}

	// TESQuest
	SafeWrite32(0x104AC44 + 0x130, (UInt32)GetNameHook);
	SafeWrite32(0x104AC44 + 0x134, (UInt32)(TESQuestSetEditorIdHook));

	// ActorValueInfo
	WriteRelCall(0x66FF57, reinterpret_cast<UInt32>(AVInfoSetEditorIDHook));
}
