#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define ASCII_RESET "\033[0;0;0m"
#define ASCII_GREEN "\033[32;0;0m"

#define HOME std::string(getenv("HOME"))
#define THEME_LOCATION HOME + "/.config/hypr/.current-theme"
#define THEMES_DIRECTORY HOME + "/.config/hypr/themes"

std::string removeAfterLast(std::string s, char delimeter) {
  return s.substr(0, s.find_last_of(delimeter));
}

void printUsage(char *name) {
  std::cout << '\n';
  std::cout << '\t' << name << '\n';
  std::cout << '\n';
  std::cout << "\tHyprland theme swapping utility.\n\n";
  std::cout << ASCII_GREEN;
  std::cout << "\t - get: return the name of the currently used theme.\n\n";
  std::cout << "\t - list: list all available themes.\n\n";
  std::cout << "\t - set <theme name>: set the current theme to the specified "
               "one.\n\n";
  std::cout
      << "\t - create <theme name>: creates a new theme with the specified "
         "name, copying neccessary files from the config directory.\n\n";
  std::cout << ASCII_RESET;
}

std::string getCurrentTheme() {
  std::ifstream themeFile(THEME_LOCATION);
  if (!themeFile.is_open()) {
    return "none";
  }

  std::string line;
  std::getline(themeFile, line);
  return line;
}

std::vector<std::string> themes() {
  std::vector<std::string> themes;
  if (!std::filesystem::directory_entry(THEMES_DIRECTORY).exists()) {
    return themes;
  }

  for (const auto &entry :
       std::filesystem::directory_iterator(THEMES_DIRECTORY)) {
    if (entry.is_directory()) {
      themes.push_back(entry.path().filename().string());
    }
  }

  return themes;
}

std::vector<std::string> filesToExport() {
  std::vector<std::string> files;

  std::ifstream fileList(HOME + "/.config/hypr/themes/.themefiles");
  if (!fileList.is_open()) {
    return files;
  }

  std::string line;
  while (std::getline(fileList, line)) {
    if (line.length() > 0) {
      files.push_back(line);
    }
  }

  return files;
}

bool themeExists(std::string theme) {
  std::vector<std::string> themeList = themes();
  return std::find(themeList.begin(), themeList.end(), theme) !=
         themeList.end();
}

bool setTheme(std::string theme) {
  if (!themeExists(theme)) {
    std::cout << "The theme \"" << theme << "\" doesn't exist.\n";
    return false;
  }

  std::ofstream currentTheme(THEME_LOCATION);
  currentTheme << theme;
  currentTheme.close();

  std::system(std::string("cp -r " + THEMES_DIRECTORY + "/" + theme + "/* " +
                          HOME + "/.config")
                  .c_str());

  std::cout << "Theme set to " << theme << '\n';
  return true;
}

bool createTheme(std::string themeName) {
  if (themeExists(themeName)) {
    std::cout << "Theme with this name already exists. Do you want to "
                 "overwrite it? [y/n]\n";
    std::string answer;
    std::cin >> answer;
    if (strcmp(answer.c_str(), "y")) {
      return false;
    }
  }

  system(std::string("mkdir " + THEMES_DIRECTORY + '/' + themeName).c_str());
  for (std::string fileToSave : filesToExport()) {
    system(std::string("mkdir -p " + THEMES_DIRECTORY + '/' + themeName + '/' +
                       removeAfterLast(fileToSave, '/'))
               .c_str());
    system(std::string("cp -r " + HOME + "/.config/" + fileToSave + ' ' +
                       THEMES_DIRECTORY + '/' + themeName + '/' +
                       removeAfterLast(fileToSave, '/') + '/')
               .c_str());
  }
  std::cout << "Created new theme.\n";
  return true;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printUsage(argv[0]);
    return 1;
  }

  if (!strcmp(argv[1], "get")) {
    std::cout << getCurrentTheme() << '\n';
  } else if (!strcmp(argv[1], "list")) {
    int any = 0;
    for (std::string theme : themes()) {
      std::cout << theme << '\n';
      ++any;
    }
    if (!any) {
      std::cout << "No themes found.\n";
    }
  } else if (!strcmp(argv[1], "set")) {
    if (argc != 3) {
      printUsage(argv[0]);
      return 1;
    }

    return setTheme(argv[2]);
  } else if (!strcmp(argv[1], "create")) {
    if (argc != 3) {
      printUsage(argv[0]);
      return 1;
    }

    return createTheme(argv[2]);
  }

  return 0;
}
