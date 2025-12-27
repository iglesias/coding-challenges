/*
 *
        Copyright 2020 Alexey Kutepov <reximkut@gmail.com>

        Permission is hereby granted, free of charge, to any person obtaining
        a copy of this software and associated documentation files (the
        "Software"), to deal in the Software without restriction, including
        without limitation the rights to use, copy, modify, merge, publish,
        distribute, sublicense, and/or sell copies of the Software, and to
        permit persons to whom the Software is furnished to do so, subject to
        the following conditions:

        The above copyright notice and this permission notice shall be
        included in all copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
        EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
        MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
        NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
        LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
        OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
        WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#define PATH_SEP "/"
typedef pid_t Pid;
typedef int Fd;

#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstring>

#include <iostream>
#include <filesystem>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

using std::string;
using std::vector;

std::string cstr_no_ext(const char* path);
#define NOEXT(path) cstr_no_ext(path)

template<typename... Args>
vector<string> cstr_array_make(Args... args)
{
    vector<string> result;
    (result.emplace_back(args), ...);
    return result;
}
vector<string> cstr_array_from_int_char(int argc, char** argv);
std::string cstr_array_join(const char* sep, const vector<string> &cstrs);

#define JOIN(sep, ...) cstr_array_join(sep, cstr_array_make(__VA_ARGS__))
#define CONCAT(...) JOIN("", __VA_ARGS__)
#define PATH(...) JOIN(PATH_SEP, __VA_ARGS__)

typedef struct {
    vector<string> line;
} Cmd;

std::string cmd_show(const Cmd &cmd);
Pid cmd_run_async(Cmd cmd, Fd *fdin, Fd *fdout);
void cmd_run_sync(Cmd cmd);

typedef std::vector<Cmd> Cmd_Array;

// TODO(#1): no way to disable echo in nobuild scripts
// TODO(#2): no way to ignore fails
#define CMD(...)                                        \
    do {                                                \
        Cmd cmd = {                                     \
            .line = cstr_array_make(__VA_ARGS__)  \
        };                                              \
        std::string cmd_to_show = cmd_show(cmd);        \
        INFO("CMD: %s", cmd_to_show.c_str());           \
        cmd_run_sync(cmd);                              \
    } while (0)

// TODO: make common with nob workflow
#ifndef REBUILD_URSELF
#  define REBUILD_URSELF(binary_path, source_path) CMD("g++", "-std=c++23", "-fsanitize=address,undefined", "-fsanitize-address-use-after-scope -D_GLIBCXX_DEBUG",  "-o", binary_path, source_path)
#endif

// Go Rebuild Urself™ Technology
//
//   How to use it:
//     int main(int argc, char** argv) {
//         GO_REBUILD_URSELF(argc, argv);
//         // actual work
//         return 0;
//     }
//
//   After your added this macro every time you run ./nobuild it will detect
//   that you modified its original source code and will try to rebuild itself
//   before doing any actual work. So you only need to bootstrap your build system
//   once.
//
//   The modification is detected by comparing the last modified times of the executable
//   and its source code. The same way the make utility usually does it.
//
//   The rebuilding is done by using the REBUILD_URSELF macro which you can redefine
//   if you need a special way of bootstraping your build system. (which I personally
//   do not recommend since the whole idea of nobuild is to keep the process of bootstrapping
//   as simple as possible and doing all of the actual work inside of the nobuild)
//
#define GO_REBUILD_URSELF(argc, argv)                                  \
    do {                                                               \
        const char *source_path = __FILE__;                            \
        assert(argc >= 1);                                             \
        const char *binary_path = argv[0];                             \
                                                                       \
        if (is_path1_modified_after_path2(source_path, binary_path)) { \
            RENAME(binary_path, CONCAT(binary_path, ".old").data());   \
            REBUILD_URSELF(binary_path, source_path);                  \
            Cmd cmd = {                                                \
                .line = cstr_array_from_int_char(argc, argv),           \
            };                                                         \
            std::string cmd_to_show = cmd_show(cmd);                   \
            INFO("CMD: %s", cmd_to_show.c_str());                      \
            cmd_run_sync(cmd);                                         \
            exit(0);                                                   \
        }                                                              \
    } while(0)
// The implementation idea is stolen from https://github.com/zhiayang/nabs

void rebuild_urself(const char *binary_path, const char *source_path);

void path_rename(const char* old_path, const char* new_path);
#define RENAME(old_path, new_path)                    \
    do {                                              \
        INFO("RENAME: %s -> %s", old_path, new_path); \
        path_rename(old_path, new_path);              \
    } while (0)

#if defined(__GNUC__) || defined(__clang__)
// https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html
#define NOBUILD_PRINTF_FORMAT(STRING_INDEX, FIRST_TO_CHECK) __attribute__ ((format (printf, STRING_INDEX, FIRST_TO_CHECK)))
#else
#define NOBUILD_PRINTF_FORMAT(STRING_INDEX, FIRST_TO_CHECK)
#endif

void VLOG(FILE *stream, const char* tag, const char* fmt, va_list args);
void INFO(const char* fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void WARN(const char* fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void ERRO(const char* fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void PANIC(const char* fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);

////////////////////////////////////////////////////////////////////////////////

#ifdef NOBUILD_IMPLEMENTATION

vector<string> cstr_array_from_int_char(int argc, char** argv)
{
    vector<string> result;
    for (int i = 0; i < argc; ++i) {
        result.push_back(argv[i]);
    }
    return result;
}

std::string cstr_no_ext(const char* path)
{
    std::string_view p(path);
    size_t dot_pos = p.find_last_of('.');
    if (dot_pos != std::string_view::npos) {
        return std::string(p.substr(0, dot_pos));
    }
    return std::string(p);
}

std::string cstr_array_join(const char* sep, const vector<string> &cstrs)
{
    std::stringstream ss;
    for (size_t i = 0; i < cstrs.size(); ++i) {
        if (i > 0) {
            ss << sep;
        }
        ss << cstrs[i];
    }
    return ss.str();
}

Fd fd_open_for_read(const char* path)
{
    Fd result = open(path, O_RDONLY);
    if (result < 0) {
        PANIC("Could not open file %s: %s", path, strerror(errno));
    }
    return result;
}

Fd fd_open_for_write(const char* path)
{
    Fd result = open(path,
                     O_WRONLY | O_CREAT | O_TRUNC,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (result < 0) {
        PANIC("could not open file %s: %s", path, strerror(errno));
    }
    return result;
}

void fd_close(Fd fd)
{
    close(fd);
}

void pid_wait(Pid pid)
{
    for (;;) {
        int wstatus = 0;
        if (waitpid(pid, &wstatus, 0) < 0) {
            PANIC("could not wait on command (pid %d): %s", pid, strerror(errno));
        }

        if (WIFEXITED(wstatus)) {
            int exit_status = WEXITSTATUS(wstatus);
            if (exit_status != 0) {
                PANIC("command exited with exit code %d", exit_status);
            }

            break;
        }

        if (WIFSIGNALED(wstatus)) {
            PANIC("command process was terminated by %s", strsignal(WTERMSIG(wstatus)));
        }
    }
}

std::string cmd_show(const Cmd &cmd)
{
    // TODO(#31): cmd_show does not render the command line properly
    // - No string literals when arguments contains space
    // - No escaping of special characters
    // - Etc.
    return cstr_array_join(" ", cmd.line);
}

Pid cmd_run_async(Cmd cmd, Fd *fdin, Fd *fdout)
{
    pid_t cpid = fork();
    if (cpid < 0) {
        std::string cmd_str = cmd_show(cmd);
        PANIC("Could not fork child process: %s: %s",
              cmd_str.c_str(), strerror(errno));
    }

    if (cpid == 0) {
        std::vector<const char*> args;
        for (const auto& s : cmd.line) {
            args.push_back(s.c_str());
        }
        args.push_back(nullptr);

        if (fdin) {
            if (dup2(*fdin, STDIN_FILENO) < 0) {
                PANIC("Could not setup stdin for child process: %s", strerror(errno));
            }
        }

        if (fdout) {
            if (dup2(*fdout, STDOUT_FILENO) < 0) {
                PANIC("Could not setup stdout for child process: %s", strerror(errno));
            }
        }

        if (execvp(args[0], (char * const*) args.data()) < 0) {
            std::string cmd_str = cmd_show(cmd);
            PANIC("Could not exec child process: %s: %s",
                  cmd_str.c_str(), strerror(errno));
        }
    }

    return cpid;
}

void cmd_run_sync(Cmd cmd)
{
    pid_wait(cmd_run_async(cmd, NULL, NULL));
}

void path_rename(const char* old_path, const char* new_path)
{
    std::error_code ec;
    std::filesystem::rename(old_path, new_path, ec);
    if (ec) {
        PANIC("could not rename %s to %s: %s", old_path, new_path, ec.message().c_str());
    }
}

bool is_path1_modified_after_path2(const char *path1, const char *path2)
{
    std::error_code ec;
    auto time1 = std::filesystem::last_write_time(path1, ec);
    if (ec) {
        PANIC("could not stat %s: %s", path1, ec.message().c_str());
    }

    auto time2 = std::filesystem::last_write_time(path2, ec);
    if (ec) {
        PANIC("could not stat %s: %s", path2, ec.message().c_str());
    }

    return time1 > time2;
}

void VLOG(FILE *stream, const char* tag, const char* fmt, va_list args)
{
    fprintf(stream, "[%s] ", tag);
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}

void INFO(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "INFO", fmt, args);
    va_end(args);
}

void WARN(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "WARN", fmt, args);
    va_end(args);
}

void ERRO(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "ERRO", fmt, args);
    va_end(args);
}

void PANIC(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "ERRO", fmt, args);
    va_end(args);
    exit(1);
}

#endif // NOBUILD_IMPLEMENTATION
