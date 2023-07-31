#include <windows.h>
#include <ShlObj.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <filesystem>

const GUID FOLDERID_RoamingAppData = {0x3EB685DB, 0x65F9, 0x4CF6, {0xA0, 0x3A, 0xE3, 0xEF, 0x65, 0x72, 0x9F, 0x3D}};
std::string path;

std::string wide_to_narrow(const std::wstring& wide_string)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, NULL, 0, NULL, NULL);
    std::string narrow_string(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, &narrow_string[0], len, NULL, NULL);
    return narrow_string;
}

/* Goofy ah ChatGPT code */
void edit_settings(){
    std::string filename = path + "Config\\GFX.ini";
    std::ifstream infile( filename );
    if(!infile.good()){
        std::perror("Error opening file");
        return;
    }
    std::ofstream outfile("temp");

    std::string line;
    for(int i = 1; std::getline(infile, line); i++){
        if(i == 5)
            outfile << "AspectRatio = 1\n";
        else
            outfile << line << "\n";
    }

    infile.close();
    outfile.close();
    
    if(std::remove(filename.c_str()) != 0) {
        std::perror("Error removing settings file");
        return;
    }
    if(std::rename("temp", filename.c_str()) != 0) {
        std::perror("Error replacing settings file");
        return;
    }
}

const char* controls_content = 
"[GCPad1]\n"
"Device = DInput/0/Keyboard Mouse\n"
"Buttons/A = `NUMPAD7`\n"
"Buttons/B = `NUMPAD9`\n"
"Buttons/X = SPACE\n"
"Buttons/Start = H\n"
"Main Stick/Up = W\n"
"Main Stick/Up/Range = 70.000000000000000\n"
"Main Stick/Down = S\n"
"Main Stick/Left = A\n"
"Main Stick/Right = D\n"
"Main Stick/Modifier = LSHIFT\n"
"Main Stick/Modifier/Range = 50.000000000000000\n"
"C-Stick/Up = `NUMPAD8`\n"
"C-Stick/Down = `NUMPAD5`\n"
"C-Stick/Left = `NUMPAD4`\n"
"C-Stick/Right = `NUMPAD6`\n"
"C-Stick/Modifier = LCONTROL\n"
"C-Stick/Modifier/Range = 50.000000000000000\n"
"Triggers/L = Q\n"
"Triggers/R = E\n"
"D-Pad/Up = I\n"
"D-Pad/Down = K\n"
"D-Pad/Left = J\n"
"D-Pad/Right = L\n";

void edit_controls(){
    std::string current_filename = path + "Config\\GCPadNew.ini";
    std::string profile_filename = path + "Config\\Profiles\\GCPad\\KEYBOARD.ini";

    std::ofstream current_settings("current_temp");
    std::ofstream profile_settings("profile_temp");

    current_settings << controls_content;
    profile_settings << controls_content;

    current_settings.close();
    profile_settings.close();

    if(std::remove(current_filename.c_str()) != 0){
        std::perror("Error removing current controls file");
    }

    else if(std::rename("current_temp", current_filename.c_str()) != 0) {
        std::perror("Error replacing current controls file");
    }

    if(std::rename("profile_temp", profile_filename.c_str()) !=0) {
        std::perror("Error renaming controls profile");
    }
}

const char* gecko_content = 
"[Gecko]\n"
"$Boot to CSS [Dan Salvato, Achilles]\n"
"041BFA20 38600002\n"
"[Gecko_Enabled]\n"
"$Optional: Widescreen 16:9\n";

void edit_gecko(){
    std::string filename = path + "GameSettings\\GALE01.ini";
    std::ofstream outfile("temp");
    outfile << gecko_content;
    outfile.close();

    std::remove(filename.c_str());

    if(std::rename("temp", filename.c_str()) != 0) {
        std::perror("Error replacing gecko file");
        return;
    }
}

int main(){
    path = getenv("appdata");
    path += "\\Slippi Launcher\\netplay\\User\\";
    
    std::ofstream gecko_file(path + "User\\GALE01.ini");
    std::ofstream controls_file(path + "Config\\GCPadNew.ini");

    edit_settings();
    edit_controls();
    edit_gecko();
}