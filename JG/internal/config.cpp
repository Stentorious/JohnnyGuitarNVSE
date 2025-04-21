#include "Config.h"
#include <stdexcept>

namespace {
    config::Settings settings; 
    bool initialized = false;
}

namespace config {

    void ReadIni(const char* filename) {
		settings.loadEditorIDs = 1;
		settings.fixHighNoon = 0;
		settings.fixFleeing = GetPrivateProfileInt("MAIN", "bFixFleeing", 1, filename);
		settings.fixItemStacks = GetPrivateProfileInt("MAIN", "bFixItemStackCount", 1, filename);
		settings.fixNPCShootingAngle = GetPrivateProfileInt("MAIN", "bFixNPCShootingAngle", 1, filename);
		settings.iFPSCapLoadScreen = GetPrivateProfileInt("MAIN", "iFPSLimitLoadScreen", 0, filename);
		settings.noMuzzleFlashCooldown = GetPrivateProfileInt("MAIN", "bNoMuzzleFlashCooldown", 0, filename);
		settings.resetVanityCam = GetPrivateProfileInt("MAIN", "bReset3rdPersonCamera", 0, filename);
		settings.enableRadioSubtitles = GetPrivateProfileInt("MAIN", "bEnableRadioSubtitles", 0, filename);
		settings.removeMainMenuMusic = GetPrivateProfileInt("MAIN", "bRemoveMainMenuMusic", 0, filename);
		settings.fixDeathSounds = GetPrivateProfileInt("MAIN", "bFixDeathVoicelines", 1, filename);
		settings.patchPainedPlayer = GetPrivateProfileInt("MAIN", "bRemovePlayerPainExpression", 0, filename);
		settings.iDeathSoundMAXTimer = GetPrivateProfileInt("DeathResponses", "iDeathSoundMAXTimer", 10, filename); //Hidden, don't actually expose it in the INI
		settings.bDisableDLLCompatibilityRoutines = GetPrivateProfileInt("Misc", "bDisableDLLCompatibilityRoutines", 0, filename); //Hidden
		//settings.bDisableDeathResponses = GetPrivateProfileInt("DeathResponses", "bDisableDeathResponses", 0, filename);
        initialized = true;
    }

    const Settings& Get() {
        if (!initialized)
            throw std::runtime_error("Settings accessed before initialization!");
        return settings;
    }

}