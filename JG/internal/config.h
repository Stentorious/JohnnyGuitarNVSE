#pragma once
#pragma once
#include <string>

namespace config {

    struct Settings {
        bool loadEditorIDs = 0;
        bool fixHighNoon = 0;
        bool fixFleeing = 0;
        bool fixItemStacks = 0;
        bool resetVanityCam = 0;
        bool fixNPCShootingAngle = 0;
        bool noMuzzleFlashCooldown = 0;
        bool enableRadioSubtitles = 0;
        bool removeMainMenuMusic = 0;
        bool fixDeathSounds = 1;
        bool patchPainedPlayer = 0;
        bool bDisableDeathResponses = 0;
        unsigned int iFPSCapLoadScreen = 0;
        float iDeathSoundMAXTimer = 10;
        bool bDisableDLLCompatibilityRoutines = 0;
    };
    const Settings& Get(); 

    void ReadIni(const char* filename); 
}