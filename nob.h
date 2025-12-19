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


using Cstr_Array = std::vector<std::string>;

bool cstr_ends_with(const char* cstr, const char* postfix);
#define ENDS_WITH(cstr, postfix) cstr_ends_with(cstr, postfix)

std::string cstr_no_ext(const char* path);
#define NOEXT(path) cstr_no_ext(path)

template<typename... Args>
Cstr_Array cstr_array_make(Args... args)
{
    Cstr_Array result;
    (result.emplace_back(args), ...);
    return result;
}
Cstr_Array cstr_array_from_int_char(int argc, char** argv);
std::string cstr_array_join(const char* sep, const Cstr_Array &cstrs);

#define JOIN(sep, ...) cstr_array_join(sep, cstr_array_make(__VA_ARGS__))
#define CONCAT(...) JOIN("", __VA_ARGS__)
#define PATH(...) JOIN(PATH_SEP, __VA_ARGS__)
#define GETCWD() path_get_current_dir()
#define SETCWD(path) path_set_current_dir(path)

typedef struct {
    Fd read;
    Fd write;
} Pipe;

Pipe pipe_make(void);

typedef struct {
    Cstr_Array line;
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

typedef enum {
    CHAIN_TOKEN_END = 0,
    CHAIN_TOKEN_IN,
    CHAIN_TOKEN_OUT,
    CHAIN_TOKEN_CMD
} Chain_Token_Type;

// A single token for the CHAIN(...) DSL syntax
typedef struct {
    Chain_Token_Type type;
    Cstr_Array args;
} Chain_Token;

// TODO(#17): IN and OUT are already taken by WinAPI
#define IN(path) \
    (Chain_Token) { \
        .type = CHAIN_TOKEN_IN, \
        .args = cstr_array_make(path) \
    }

#define OUT(path) \
    (Chain_Token) { \
        .type = CHAIN_TOKEN_OUT, \
        .args = cstr_array_make(path) \
    }

#define CHAIN_CMD(...) \
    (Chain_Token) { \
        .type = CHAIN_TOKEN_CMD, \
        .args = cstr_array_make(__VA_ARGS__) \
    }

// TODO(#20): pipes do not allow redirecting stderr
struct Chain {
    std::optional<std::string> input_filepath;
    Cmd_Array cmds;
    std::optional<std::string> output_filepath;
};

Chain chain_build_from_tokens(Chain_Token first, ...);
void chain_run_sync(const Chain &chain);
void chain_echo(const Chain &chain);

// TODO(#15): PIPE does not report where exactly a syntactic error has happened
#define CHAIN(...)                                                      \
    do {                                                                \
        Chain chain = chain_build_from_tokens(__VA_ARGS__, (Chain_Token) {0}); \
        chain_echo(chain);                                              \
        chain_run_sync(chain);                                          \
    } while(0)

#ifndef REBUILD_URSELF
#  define REBUILD_URSELF(binary_path, source_path) CMD("g++", "-std=c++20", "-o", binary_path, source_path)
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

bool path_is_dir(const char* path);
#define IS_DIR(path) path_is_dir(path)

bool path_exists(const char* path);
#define PATH_EXISTS(path) path_exists(path)

void path_mkdirs(const Cstr_Array &path);
#define MKDIRS(...)                                             \
    do {                                                        \
        Cstr_Array path = cstr_array_make(__VA_ARGS__);   \
        INFO("MKDIRS: %s", cstr_array_join(PATH_SEP, path).c_str());    \
        path_mkdirs(path);                                      \
    } while (0)

void path_rename(const char* old_path, const char* new_path);
#define RENAME(old_path, new_path)                    \
    do {                                              \
        INFO("RENAME: %s -> %s", old_path, new_path); \
        path_rename(old_path, new_path);              \
    } while (0)

void path_rm(const char* path);
#define RM(path)                                \
    do {                                        \
        INFO("RM: %s", path);                   \
        path_rm(path);                          \
    } while(0)



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

Cstr_Array cstr_array_from_int_char(int argc, char** argv)
{
    Cstr_Array result;
    for (int i = 0; i < argc; ++i) {
        result.push_back(argv[i]);
    }
    return result;
}

bool cstr_ends_with(const char* cstr, const char* postfix)
{
    std::string_view s(cstr);
    std::string_view p(postfix);
    if (s.length() < p.length()) {
        return false;
    }
    return s.substr(s.length() - p.length()) == p;
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

std::string cstr_array_join(const char* sep, const Cstr_Array &cstrs)
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

Pipe pipe_make(void)
{
    Pipe pip = {0};

    Fd pipefd[2];
    if (pipe(pipefd) < 0) {
        PANIC("Could not create pipe: %s", strerror(errno));
    }

    pip.read = pipefd[0];
    pip.write = pipefd[1];

    return pip;
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

Chain chain_build_from_tokens(Chain_Token first, ...)
{
    Chain result;

    auto process_token = [&](Chain_Token token) {
        switch (token.type) {
        case CHAIN_TOKEN_CMD: {
            result.cmds.push_back({.line = token.args});
        }
        break;

        case CHAIN_TOKEN_IN: {
            if (result.input_filepath) {
                PANIC("Input file path was already set");
            }
            result.input_filepath = token.args.front();
        }
        break;

        case CHAIN_TOKEN_OUT: {
            if (result.output_filepath) {
                PANIC("Output file path was already set");
            }
            result.output_filepath = token.args.front();
        }
        break;

        case CHAIN_TOKEN_END:
        default: {
            assert(0 && "unreachable");
            exit(1);
        }
        }
    };

    process_token(first);

    va_list args;
    va_start(args, first);
    Chain_Token next = va_arg(args, Chain_Token);
    while (next.type != CHAIN_TOKEN_END) {
        process_token(next);
        next = va_arg(args, Chain_Token);
    }
    va_end(args);

    return result;
}

void chain_run_sync(const Chain &chain)
{
    if (chain.cmds.empty()) {
        return;
    }

    std::vector<Pid> cpids;
    cpids.reserve(chain.cmds.size());

    Fd fdin = -1;
    if (chain.input_filepath) {
        fdin = fd_open_for_read(chain.input_filepath->c_str());
    }

    for (size_t i = 0; i < chain.cmds.size(); ++i) {
        Fd* fdin_ptr = (fdin != -1) ? &fdin : nullptr;
        bool is_last = (i == chain.cmds.size() - 1);

        if (is_last) {
            Fd fdout = -1;
            if (chain.output_filepath) {
                fdout = fd_open_for_write(chain.output_filepath->c_str());
            }
            Fd* fdout_ptr = (fdout != -1) ? &fdout : nullptr;

            cpids.push_back(cmd_run_async(chain.cmds[i], fdin_ptr, fdout_ptr));

            if (fdin != -1) close(fdin);
            if (fdout != -1) close(fdout);
        } else {
            int pipefd[2];
            if (pipe(pipefd) < 0) {
                PANIC("Could not create pipe: %s", strerror(errno));
            }
            Fd read_end = pipefd[0];
            Fd write_end = pipefd[1];

            cpids.push_back(cmd_run_async(chain.cmds[i], fdin_ptr, &write_end));

            if (fdin != -1) close(fdin);
            close(write_end);
            fdin = read_end;
        }
    }

    for (const auto &cpid : cpids) {
        pid_wait(cpid);
    }
}

void chain_echo(const Chain &chain)
{
    std::cout << "[INFO] CHAIN:";
    if (chain.input_filepath) {
        std::cout << " " << *chain.input_filepath;
    }

    for (const auto &cmd : chain.cmds) {
        std::cout << " |> " << cmd_show(cmd);
    }

    if (chain.output_filepath) {
        std::cout << " |> " << *chain.output_filepath;
    }

    std::cout << std::endl;
}

std::string path_get_current_dir()
{
    std::error_code ec;
    auto path = std::filesystem::current_path(ec);
    if (ec) {
        PANIC("could not get current directory: %s", ec.message().c_str());
    }
    return path.string();
}

void path_set_current_dir(const char* path)
{
    std::error_code ec;
    std::filesystem::current_path(path, ec);
    if (ec) {
        PANIC("could not set current directory to %s: %s",
              path, ec.message().c_str());
    }
}

bool path_exists(const char* path)
{
    std::error_code ec;
    bool result = std::filesystem::exists(path, ec);
    if (ec) {
        PANIC("could not check existence of path %s: %s", path, ec.message().c_str());
    }
    return result;
}

bool path_is_dir(const char* path)
{
    std::error_code ec;
    bool result = std::filesystem::is_directory(path, ec);
    if (ec) {
        if (ec.value() == ENOENT) {
            return false;
        }
        PANIC("could not check if path %s is a directory: %s", path, ec.message().c_str());
    }
    return result;
}

void path_rename(const char* old_path, const char* new_path)
{
    std::error_code ec;
    std::filesystem::rename(old_path, new_path, ec);
    if (ec) {
        PANIC("could not rename %s to %s: %s", old_path, new_path, ec.message().c_str());
    }
}

void path_mkdirs(const Cstr_Array &path)
{
    std::filesystem::path p;
    for(const auto& part : path) {
        p /= part;
    }

    if (p.empty()) {
        return;
    }

    std::error_code ec;
    std::filesystem::create_directories(p, ec);
    if (ec) {
        PANIC("could not create directories %s: %s", p.c_str(), ec.message().c_str());
    }
}

void path_rm(const char* path)
{
    std::error_code ec;
    std::filesystem::remove_all(path, ec);
    if (ec) {
        PANIC("could not remove %s: %s", path, ec.message().c_str());
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
