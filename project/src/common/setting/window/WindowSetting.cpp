#include "src/common/setting/window/WindowSetting.h"
#include "src/util/file/ini/settings_ini.h"

float WindowSetting::width = 100.0f;
float WindowSetting::width_half = 50.0f;
float WindowSetting::height = 100.0f;
float WindowSetting::height_half = 50.0f;
float WindowSetting::extend = 1.0f;
std::string WindowSetting::name = "No Setting";
bool WindowSetting::isFull = false;

void WindowSetting::Load(const std::string& file)
{
    Settings_ini::Inst().Load(file, "window", true);
    extend = Settings_ini::Inst().Get<float>("Main_Window.Extend", "window");

    width = Settings_ini::Inst().Get<float>("Main_Window.Width", "window") * extend;
    height = Settings_ini::Inst().Get<float>("Main_Window.Height", "window") * extend;
    
    width_half = width / 2.0f;
    height_half = height / 2.0f;
    
    name = Settings_ini::Inst().Get<std::string>("Main_Window.Name", "window");
    isFull = Settings_ini::Inst().Get<bool>("Main_Window.Full", "window");
}
