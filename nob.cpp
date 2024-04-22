#define NOBUILD_IMPLEMENTATION
#include "./nob.h"

#include <filesystem>
#include <queue>
#include <string_view>

#define CFLAGS    "-Wall", "-Wextra", "-std=c2x", "-pedantic"
#define CPPFLAGS  "-Wall", "-Wextra", "-std=c++23", "-pedantic", "-Wconversion"

namespace fs = std::filesystem;

using namespace std::string_literals;  // for operator""s

void build_kattis_c_file(std::string_view filename) {
  Cstr tool_path = PATH("kattis", filename.data());
  CMD("cc", CFLAGS, "-o", NOEXT(tool_path), tool_path);
}

void build_kattis_c_files() {
  for (auto const& entry : fs::directory_iterator("kattis"))
    if (fs::is_regular_file(entry.path())) {
      auto const filename = entry.path().filename().string();
      if (filename.ends_with(".c"))
        build_kattis_c_file(filename);
    }
}

void build_custom_cpp_file(std::string_view filename) {
  Cstr tool_path = PATH("custom", filename.data());
  CMD("g++", CPPFLAGS, "-o", NOEXT(tool_path), tool_path);
}

void build_custom_cpp_files() {
  for (auto const& entry : fs::directory_iterator("custom"))
    if (fs::is_regular_file(entry.path())) {
      auto const filename = entry.path().filename().string();
      if (filename.ends_with(".cpp") or filename.ends_with("cc"))
        build_custom_cpp_file(filename);
    }
}

void build_cpp_file(std::string_view filename) {
  Cstr tool_path = PATH(filename.data());
  CMD("g++", CPPFLAGS, "-o", NOEXT(tool_path), tool_path, "-lgtest");
}

void build_leetcode_cpp_files() {
  for (auto const& entry : fs::directory_iterator("leetcode"))
    if (fs::is_regular_file(entry.path())) {
      auto const filename = entry.path().filename().string();
      if (filename == "3012.cpp") {
        Cstr tool_path = PATH(("leetcode/" + filename).c_str());
        CMD("g++", "-std=c++20", "-fmodules-ts", "-x", "c++-system-header", "array");
        CMD("g++", "-std=c++20", "-fmodules-ts", "-x", "c++-system-header", "algorithm");
        CMD("g++", "-std=c++20", "-fmodules-ts", "-Wall", "-Wextra", "-pedantic", "-Wconversion", "-o", NOEXT(tool_path), tool_path);
        continue;
      }
      if (filename.ends_with(".cpp"))
        build_cpp_file("leetcode/" + filename);
    }
}

void build_codeforces_cpp_files() {
  std::queue<std::string> directoriesQ;
  directoriesQ.push("codeforces"s);
  while (!directoriesQ.empty()) {
    for (auto const& entry : fs::directory_iterator(directoriesQ.front()))
      if (fs::is_regular_file(entry.path())) {
        auto const filename = entry.path().filename().string();
        // FIXME: auto-detect when compilation fails with modern standards and if desired
        // to keep the old file as it is, revert here to using older -std= option.
        if (filename == "pocketbook.cc" or filename == "steps.cc" or filename == "phonenumbers.cc") {
          Cstr tool_path = PATH(std::string_view(directoriesQ.front() + "/" + filename).data());
          CMD("g++", "-Wall", "-Wextra", "-std=gnu++17", "-pedantic", "-Wconversion", "-o", NOEXT(tool_path), tool_path);
          continue;
        } else if (filename == "marks.cc") {
          Cstr tool_path = PATH(std::string_view(directoriesQ.front() + "/" + filename).data());
          CMD("g++", "-Wall", "-Wextra", "-std=gnu++11", "-pedantic", "-Wconversion", "-o", NOEXT(tool_path), tool_path);
          continue;
        }
        //
        if (filename.ends_with(".cc") or filename.ends_with(".cpp")) {
          build_cpp_file(directoriesQ.front() + "/" + filename);
        }
      } else if (fs::is_directory(entry.path())) {
        directoriesQ.push(directoriesQ.front() + "/" + entry.path().filename().string());
      }
    directoriesQ.pop();
  }
}

void build_directory_cpp_files(std::string const& root_directory) {
  std::queue<std::string> directoriesQ;
  directoriesQ.push(root_directory);
  while (!directoriesQ.empty()) {
    for (auto const& entry : fs::directory_iterator(directoriesQ.front()))
      if (fs::is_regular_file(entry.path())) {
        auto const filename = entry.path().filename().string();
        if (filename.ends_with(".cc") or filename.ends_with(".cpp"))
          build_cpp_file(directoriesQ.front() + "/" + filename);
      } else if (fs::is_directory(entry.path())) {
        directoriesQ.push(directoriesQ.front() + "/" + entry.path().filename().string());
      }
    directoriesQ.pop();
  }
}

void run_gtest_file(std::string_view filename) {
  Cstr path = PATH(filename.data());
  CMD("g++", CPPFLAGS, "-o", NOEXT(path), path, "-lgtest");
  CMD(NOEXT(path));
}

int main(int argc, char* argv[]) {
  GO_REBUILD_URSELF(argc, argv);

  //build_kattis_c_files();
  //build_custom_cpp_files();
  //build_directory_cpp_files("adventofcode");
  //build_leetcode_cpp_files();
  //build_codeforces_cpp_files();
  //run_examples();
  run_gtest_file("uva/summing_digits.cpp");
  run_gtest_file("leetcode/752.cpp");
}
