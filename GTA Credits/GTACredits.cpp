#include "pch.h"

#include "iniconfig/INIFile.h"

using namespace plugin;

static float GetFullScreenSizeX() { return 1024.0f; }
static float GetFullScreenSizeY() { return 768.0f; }

static float ScreenX(float x) { return (x * (SCREEN_COORD_CENTER_X * 2)) / GetFullScreenSizeX(); }
static float ScreenY(float y) { return (y * (SCREEN_COORD_CENTER_Y * 2)) / GetFullScreenSizeY(); }

static std::string MessageText = "Text";
static float FontSize = 2.0f;

class GTACredits {
public:
    GTACredits()
    {
        char path[256];
        sprintf(path, "\\GTACredits.ini");
        std::string settingsFileDir = PLUGIN_PATH(path);

        INIFile file;
        if (!file.Read(settingsFileDir))
        {
            return;
        }

        auto generalSections = file.GetSections("GENERAL");
        if (generalSections.size() > 0)
        {
            auto generalSection = generalSections[0];

            MessageText = generalSection->GetString("text");
            FontSize = generalSection->GetFloat("size", FontSize);
        }

        Events::drawHudEvent += []()
        {
            CVector2D position = CVector2D(
                GetFullScreenSizeX() / 2.0f,
                40.0f
            );

            char buffer[256];
            sprintf_s(buffer, "%s", MessageText.c_str());

            CFont::m_fWrapx = 100000.0f;
            CFont::SetOrientation(eFontAlignment::ALIGN_CENTER);
            CFont::SetDropShadowPosition(0);
            CFont::SetRightJustifyWrap(false);
            CFont::SetJustify(true);
            CFont::SetBackground(false, false);
            CFont::SetScale(ScreenX(0.35f * FontSize), ScreenY(0.95f * FontSize));
            CFont::SetFontStyle(eFontStyle::FONT_SUBTITLES);
            CFont::SetProportional(true);
            CFont::SetColor(CRGBA(255, 255, 255));
            CFont::PrintString(ScreenX(position.x), ScreenY(position.y), buffer);
        };
    }
} gtaCredits;
