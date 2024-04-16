#define NOBUILD_IMPLEMENTATION
#include "./nob.h"

#include <filesystem>
#include <queue>
#include <string_view>

#define CFLAGS    "-Wall", "-Wextra", "-std=c2x", "-pedantic"
#define CPPFLAGS  "-Wall", "-Wextra", "-std=c++23", "-pedantic", "-Wconversion", "-Wfatal-errors", "-fsanitize=undefined,address"

namespace fs = std::filesystem;

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
  CMD("g++", CPPFLAGS, "-o", NOEXT(tool_path), tool_path);
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

void run_example(const char *example) {
  Cstr example_path = PATH("uva", example);
  CMD("g++", CPPFLAGS, "-o", NOEXT(example_path), example_path, "-lgtest");
  CMD(NOEXT(example_path));
}

void run_examples() {
  for (const auto& entry : fs::directory_iterator("uva"))
    if (fs::is_regular_file(entry.path())) {
      auto filename = entry.path().filename().string();
      if (filename == "summing_digits.cpp")
        run_example(filename.c_str());
    }
}

int main(int argc, char* argv[]) {
  GO_REBUILD_URSELF(argc, argv);

  build_kattis_c_files();
  build_custom_cpp_files();
  //build_directory_cpp_files("adventofcode");
  //build_directory_cpp_files("leetcode");
  run_examples();
}
