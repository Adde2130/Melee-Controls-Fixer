#include <windows.h>
#include <ShlObj.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <filesystem>

std::string path;
std::string wide_to_narrow(const std::wstring& wide_string)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, NULL, 0, NULL, NULL);
    std::string narrow_string(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wide_string.c_str(), -1, &narrow_string[0], len, NULL, NULL);
    return narrow_string;
}

bool file_exists(const std::string& filename) {
    std::ifstream ifile(filename);
    return ifile.good();
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
"Device = DInput/0/Keyboard Mouse\n"
"Buttons/A = `NUMPAD5`\n"
"Buttons/B = `NUMPAD4`\n"
"Buttons/X = `NUMPAD6`\n"
"Buttons/Start = RETURN\n"
"Main Stick/Up = W\n"
"Main Stick/Up/Range = 70.000000000000000\n"
"Main Stick/Down = S\n"
"Main Stick/Left = A\n"
"Main Stick/Right = D\n"
"Main Stick/Modifier = LSHIFT\n"
"Main Stick/Modifier/Range = 50.000000000000000\n"
"C-Stick/Up = `NUMPAD7`\n"
"C-Stick/Down = `NUMPAD8`\n"
"C-Stick/Left = DOWN\n"
"C-Stick/Right = RIGHT\n"
"C-Stick/Modifier = LCONTROL\n"
"C-Stick/Modifier/Range = 50.000000000000000\n"
"Triggers/L = SPACE\n"
"Triggers/R = ADD\n"
"D-Pad/Up = `1`\n"
"D-Pad/Down = `2`\n"
"D-Pad/Left = `3`\n"
"D-Pad/Right = `4`\n"
"Buttons/Z = `NUMPAD0`\n";

void edit_controls(){
    std::string current_filename = path + "Config\\GCPadNew.ini";
    std::string profile_filename = path + "Config\\Profiles\\GCPad\\KEYBOARD.ini";

    std::ofstream current_settings("current_temp");
    std::ofstream profile_settings("profile_temp");

    current_settings << "[GCPad1]\n" << controls_content;
    profile_settings << "[Profile]\n" <<controls_content;

    current_settings.close();
    profile_settings.close();

    if(std::remove(current_filename.c_str()) != 0){
        std::perror("Error removing current controls file");
    }

    else if(std::rename("current_temp", current_filename.c_str()) != 0) {
        std::perror("Error replacing current controls file");
    }

    std::remove(profile_filename.c_str());
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
    
    edit_settings();
    edit_controls();
    edit_gecko();
}