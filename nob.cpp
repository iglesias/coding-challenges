#define NOBUILD_IMPLEMENTATION
#include "./nob.h"

#include <array>
#include <filesystem>
#include <queue>
#include <string_view>
#include <vector>

#define CFLAGS    "-Wall", "-Wextra", "-std=c2x", "-pedantic"
#define CPPFLAGS  "-Wall", "-Wextra", "-std=c++23", "-pedantic", "-Wconversion"

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

namespace fs = std::filesystem;

using namespace std::string_literals;  // for operator""s
using namespace std::literals;         // for operator""sv

void build_kattis_c_file(std::string_view filename) {
  Cstr path = PATH("kattis", filename.data());
  CMD("cc", CFLAGS, "-o", NOEXT(path), path);
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
  Cstr path = PATH("custom", filename.data());
  CMD("g++", CPPFLAGS, "-o", NOEXT(path), path);
}

void build_custom_cpp_files() {
  for (auto const& entry : fs::directory_iterator("custom"))
    if (fs::is_regular_file(entry.path())) {
      auto const filename = entry.path().filename().string();
      if (filename.ends_with(".cpp") or filename.ends_with("cc"))
        build_custom_cpp_file(filename);
    }
}

template<size_t N>
void make_and_run_cmd(std::array<Cstr, N> strings)
{
    Cmd cmd;
    cmd.line.count = strings.size();
    cmd.line.elems = strings.data();
    INFO("make_and_run_cmd: %s", cmd_show(cmd));
    cmd_run_sync(cmd);
}

void build_cpp_file(std::string_view filename)
{
    Cstr path = PATH(filename.data());
    make_and_run_cmd(std::array{"g++", CPPFLAGS, "-o", NOEXT(path), path});
}

void build_and_run_gtest_file(std::string_view filename)
{
    Cstr path = PATH(filename.data());
    CMD("g++", CPPFLAGS, "-o", NOEXT(path), path, "-lgtest");
    CMD(NOEXT(path));
}

Pid build_gtest_file_async(std::string_view filename)
{
    Cstr path = PATH(filename.data());
    return cmd_run_async(MAKE_CMD("g++", CPPFLAGS, "-o", NOEXT(path), path, "-lgtest"), NULL, NULL);
}

Pid run_gtest_file_async(std::string_view filename)
{
    return cmd_run_async(MAKE_CMD(NOEXT(PATH(filename.data()))), NULL, NULL);
}

void work_out_leetcode()
{
    std::vector<Pid> pids;
    for (const auto& entry : fs::directory_iterator("leetcode")) if (fs::is_regular_file(entry.path())) {
        const std::string& filename = entry.path().filename().string();
        if (filename == "3012.cpp") {
            Cstr path = PATH(("leetcode/" + filename).c_str());
            CMD("g++", "-std=c++20", "-fmodules-ts", "-x", "c++-system-header", "array");
            CMD("g++", "-std=c++20", "-fmodules-ts", "-x", "c++-system-header", "algorithm");
            CMD("g++", "-std=c++20", "-fmodules-ts", "-Wall", "-Wextra", "-pedantic", "-Wconversion", "-o", NOEXT(path), path);
            continue; }
        if (filename.ends_with(".cpp")) pids.push_back(build_gtest_file_async("leetcode/" + filename)); }

    for (const Pid& pid : pids) pid_wait(pid);

    pids.clear();

    for (const auto& entry : fs::directory_iterator("leetcode")) if (fs::is_regular_file(entry.path())) {
        const std::string& filename = entry.path().filename().string();
        if (filename == "3012.cpp") continue;
        if (filename.ends_with(".cpp")) pids.push_back(run_gtest_file_async("leetcode/" + filename)); }

    for (const Pid& pid : pids) pid_wait(pid);
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
          Cstr path = PATH(std::string_view(directoriesQ.front() + "/" + filename).data());
          CMD("g++", "-Wall", "-Wextra", "-std=gnu++17", "-pedantic", "-Wconversion", "-o", NOEXT(path), path);
          continue;
        } else if (filename == "marks.cc") {
          Cstr path = PATH(std::string_view(directoriesQ.front() + "/" + filename).data());
          CMD("g++", "-Wall", "-Wextra", "-std=gnu++11", "-pedantic", "-Wconversion", "-o", NOEXT(path), path);
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

int main(int argc, char* argv[])
{
    GO_REBUILD_URSELF(argc, argv);

  //build_kattis_c_files();
  //build_custom_cpp_files();
  //build_directory_cpp_files("adventofcode");
  //build_codeforces_cpp_files();
    work_out_leetcode();
    build_and_run_gtest_file("uva/summing_digits.cpp");
#if GCC_VERSION > 120000
    build_cpp_file("adventofcode/2023/day/01/trebuchet.cpp");
#endif
    build_cpp_file("adventofcode/2023/day/02/cube_conundrum.cpp");
    build_cpp_file("adventofcode/2022/day/25/snafu.cpp");
}
