#define NOBUILD_IMPLEMENTATION
#include "./nob.h"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <memory>
#include <queue>
#include <string_view>
#include <vector>

#define CFLAGS    "-std=c2x",   "-Wall", "-Wextra", "-pedantic"
#define CPPFLAGS  "-std=c++23", "-Wall", "-Wconversion", "-Wextra", "-pedantic", "-fsanitize=address,pointer-overflow,signed-integer-overflow,undefined"

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)


#define MAKE_CMD(...)                                 \
      ({Cmd cmd = {                                   \
          .line = cstr_array_make(__VA_ARGS__, NULL)  \
      };                                              \
      Cstr cmd_to_show = cmd_show(cmd);               \
      INFO("MAKE_CMD: %s", cmd_to_show);              \
      std::free(reinterpret_cast<void*>(              \
          const_cast<char*>(cmd_to_show)));           \
      cmd;})                                          \


template<size_t N> auto make_cmd(std::array<Cstr, N> strings) -> std::unique_ptr<Cmd>
{
    std::unique_ptr<Cmd> cmd_ptr = std::make_unique<Cmd>();
    cmd_ptr->line.count = strings.size();
    cmd_ptr->line.elems = strings.data();
    return cmd_ptr;
}

template<size_t N> void make_and_run_cmd(std::array<Cstr, N> strings)
{
    const std::unique_ptr<const Cmd> cmd_ptr{make_cmd(strings)};
    //TODO FIX leaks.
    //FIXME stack-use-after-return?
    INFO("make_and_run_cmd: %s", cmd_show(*cmd_ptr));
    cmd_run_sync(*cmd_ptr);
}


namespace fs = std::filesystem;

using namespace std::string_literals;  // for operator""s
using namespace std::literals;         // for operator""sv

void build_kattis_c_file(std::string_view path) {
  Cstr noextpath = NOEXT(path.data());
  CMD("cc", CFLAGS, "-o", noextpath, path.data());
  std::free(reinterpret_cast<void*>(const_cast<char *>(noextpath))); // :-O
}

void build_kattis_c_files() {
  for (auto const& entry : fs::directory_iterator("kattis"))
    if (fs::is_regular_file(entry.path())) {
      const std::string& path = entry.path().string();
      if (path.ends_with(".c")) build_kattis_c_file(path);
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

Pid build_gtest_file_async(std::string_view path)
{
    Cstr noextpath = NOEXT(path.data());
    Cmd cmd = MAKE_CMD("g++", CPPFLAGS, "-o", noextpath, path.data(), "-lgtest");
    Pid pid = cmd_run_async(cmd, NULL, NULL);
    std::free(reinterpret_cast<void*>(const_cast<char**>(cmd.line.elems)));
    std::free(reinterpret_cast<void*>(const_cast<char *>(noextpath))); // :-O
    return pid;
}

Pid run_gtest_file_async(std::string_view filename)
{
    return cmd_run_async(MAKE_CMD(NOEXT(PATH(filename.data()))), NULL, NULL);
}


std::map<std::string_view, std::vector<std::unique_ptr<Cmd>>> specifics;

void init_specifics()
{
    using CmdPtr = std::unique_ptr<Cmd>;
    std::vector<CmdPtr> cmds;
    cmds.push_back(make_cmd(std::array{"g++ -std=c++20 -fmodules-ts -x c++-system-header array"}));
    cmds.push_back(make_cmd(std::array{"g++ -std=c++20 -fmodules-ts -x c++-system-header algorithm"}));
    //TODO add last command that will be modified later, when the complete path is available,
    // appending "-o NOEXT(path)" and "path"
    specifics.emplace("3012.cpp", std::move(cmds));
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

    build_kattis_c_files();
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
