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
#include <limits.h>
#define PATH_SEP "/"
typedef pid_t Pid;
typedef int Fd;

#include <assert.h>
#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#define FOREACH_ARRAY(type, elem, array, body)  \
    for (size_t elem_##index = 0;                           \
         elem_##index < array.count;                        \
         ++elem_##index)                                    \
    {                                                       \
        type *elem = &array.elems[elem_##index];            \
        body;                                               \
    }

typedef const char * Cstr;

int cstr_ends_with(Cstr cstr, Cstr postfix);
#define ENDS_WITH(cstr, postfix) cstr_ends_with(cstr, postfix)

Cstr cstr_no_ext(Cstr path);
#define NOEXT(path) cstr_no_ext(path)

typedef struct {
    Cstr *elems;
    size_t count;
} Cstr_Array;

Cstr_Array cstr_array_make(Cstr first, ...);
Cstr_Array cstr_array_append(Cstr_Array cstrs, Cstr cstr);
Cstr cstr_array_join(Cstr sep, Cstr_Array cstrs);

#define JOIN(sep, ...) cstr_array_join(sep, cstr_array_make(__VA_ARGS__, NULL))
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

Fd fd_open_for_read(Cstr path);
Fd fd_open_for_write(Cstr path);
void fd_close(Fd fd);
void pid_wait(Pid pid);
Cstr cmd_show(Cmd cmd);
Pid cmd_run_async(Cmd cmd, Fd *fdin, Fd *fdout);
void cmd_run_sync(Cmd cmd);

typedef struct {
    Cmd *elems;
    size_t count;
} Cmd_Array;

// TODO(#1): no way to disable echo in nobuild scripts
// TODO(#2): no way to ignore fails
#define CMD(...)                                        \
    do {                                                \
        Cmd cmd = {                                     \
            .line = cstr_array_make(__VA_ARGS__, NULL)  \
        };                                              \
        INFO("CMD: %s", cmd_show(cmd));                 \
        cmd_run_sync(cmd);                              \
    } while (0)

#define MAKE_CMD(...)                                 \
      ({Cmd cmd = {                                   \
          .line = cstr_array_make(__VA_ARGS__, NULL)  \
      };                                              \
      INFO("MAKE_CMD: %s", cmd_show(cmd));            \
      cmd;})                                          \

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
        .args = cstr_array_make(path, NULL) \
    }

#define OUT(path) \
    (Chain_Token) { \
        .type = CHAIN_TOKEN_OUT, \
        .args = cstr_array_make(path, NULL) \
    }

#define CHAIN_CMD(...) \
    (Chain_Token) { \
        .type = CHAIN_TOKEN_CMD, \
        .args = cstr_array_make(__VA_ARGS__, NULL) \
    }

// TODO(#20): pipes do not allow redirecting stderr
typedef struct {
    Cstr input_filepath;
    Cmd_Array cmds;
    Cstr output_filepath;
} Chain;

Chain chain_build_from_tokens(Chain_Token first, ...);
void chain_run_sync(Chain chain);
void chain_echo(Chain chain);

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
            RENAME(binary_path, CONCAT(binary_path, ".old"));          \
            REBUILD_URSELF(binary_path, source_path);                  \
            Cmd cmd = {                                                \
                .line = {                                              \
                    .elems = (Cstr*) argv,                             \
                    .count = static_cast<size_t>(argc),                                     \
                },                                                     \
            };                                                         \
            INFO("CMD: %s", cmd_show(cmd));                            \
            cmd_run_sync(cmd);                                         \
            exit(0);                                                   \
        }                                                              \
    } while(0)
// The implementation idea is stolen from https://github.com/zhiayang/nabs

void rebuild_urself(const char *binary_path, const char *source_path);

int path_is_dir(Cstr path);
#define IS_DIR(path) path_is_dir(path)

int path_exists(Cstr path);
#define PATH_EXISTS(path) path_exists(path)

void path_mkdirs(Cstr_Array path);
#define MKDIRS(...)                                             \
    do {                                                        \
        Cstr_Array path = cstr_array_make(__VA_ARGS__, NULL);   \
        INFO("MKDIRS: %s", cstr_array_join(PATH_SEP, path));    \
        path_mkdirs(path);                                      \
    } while (0)

void path_rename(Cstr old_path, Cstr new_path);
#define RENAME(old_path, new_path)                    \
    do {                                              \
        INFO("RENAME: %s -> %s", old_path, new_path); \
        path_rename(old_path, new_path);              \
    } while (0)

void path_rm(Cstr path);
#define RM(path)                                \
    do {                                        \
        INFO("RM: %s", path);                   \
        path_rm(path);                          \
    } while(0)

#define FOREACH_FILE_IN_DIR(file, dirpath, body)        \
    do {                                                \
        struct dirent *dp = NULL;                       \
        DIR *dir = opendir(dirpath);                    \
        if (dir == NULL) {                              \
            PANIC("could not open directory %s: %s",    \
                  dirpath, strerror(errno));            \
        }                                               \
        errno = 0;                                      \
        while ((dp = readdir(dir))) {                   \
            const char *file = dp->d_name;              \
            body;                                       \
        }                                               \
                                                        \
        if (errno > 0) {                                \
            PANIC("could not read directory %s: %s",    \
                  dirpath, strerror(errno));            \
        }                                               \
                                                        \
        closedir(dir);                                  \
    } while(0)

#if defined(__GNUC__) || defined(__clang__)
// https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html
#define NOBUILD_PRINTF_FORMAT(STRING_INDEX, FIRST_TO_CHECK) __attribute__ ((format (printf, STRING_INDEX, FIRST_TO_CHECK)))
#else
#define NOBUILD_PRINTF_FORMAT(STRING_INDEX, FIRST_TO_CHECK)
#endif

void VLOG(FILE *stream, Cstr tag, Cstr fmt, va_list args);
void INFO(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void WARN(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void ERRO(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void PANIC(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);

char *shift_args(int *argc, char ***argv);

////////////////////////////////////////////////////////////////////////////////

#ifdef NOBUILD_IMPLEMENTATION

Cstr_Array cstr_array_append(Cstr_Array cstrs, Cstr cstr)
{
    Cstr_Array result = {
        .count = cstrs.count + 1
    };
    result.elems = reinterpret_cast<const char**>(std::malloc(sizeof(result.elems[0]) * result.count));
    memcpy(result.elems, cstrs.elems, cstrs.count * sizeof(result.elems[0]));
    result.elems[cstrs.count] = cstr;
    return result;
}

int cstr_ends_with(Cstr cstr, Cstr postfix)
{
    const size_t cstr_len = strlen(cstr);
    const size_t postfix_len = strlen(postfix);
    return postfix_len <= cstr_len
           && strcmp(cstr + cstr_len - postfix_len, postfix) == 0;
}

Cstr cstr_no_ext(Cstr path)
{
    size_t n = strlen(path);
    while (n > 0 && path[n - 1] != '.') {
        n -= 1;
    }

    if (n > 0) {
        char *result = reinterpret_cast<char*>(std::malloc(n));
        memcpy(result, path, n);
        result[n - 1] = '\0';

        return result;
    } else {
        return path;
    }
}

Cstr_Array cstr_array_make(Cstr first, ...)
{
    Cstr_Array result = {0};

    if (first == NULL) {
        return result;
    }

    result.count += 1;

    va_list args;
    va_start(args, first);
    for (Cstr next = va_arg(args, Cstr);
            next != NULL;
            next = va_arg(args, Cstr)) {
        result.count += 1;
    }
    va_end(args);

    result.elems = reinterpret_cast<const char**>(std::malloc(sizeof(result.elems[0]) * result.count));
    if (result.elems == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    result.count = 0;

    result.elems[result.count++] = first;

    va_start(args, first);
    for (Cstr next = va_arg(args, Cstr);
            next != NULL;
            next = va_arg(args, Cstr)) {
        result.elems[result.count++] = next;
    }
    va_end(args);

    return result;
}

Cstr cstr_array_join(Cstr sep, Cstr_Array cstrs)
{
    if (cstrs.count == 0) {
        return "";
    }

    const size_t sep_len = strlen(sep);
    size_t len = 0;
    for (size_t i = 0; i < cstrs.count; ++i) {
        len += strlen(cstrs.elems[i]);
    }

    const size_t result_len = (cstrs.count - 1) * sep_len + len + 1;
    char *result = reinterpret_cast<char*>(std::malloc(sizeof(char) * result_len));
    if (result == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }

    len = 0;
    for (size_t i = 0; i < cstrs.count; ++i) {
        if (i > 0) {
            memcpy(result + len, sep, sep_len);
            len += sep_len;
        }

        size_t elem_len = strlen(cstrs.elems[i]);
        memcpy(result + len, cstrs.elems[i], elem_len);
        len += elem_len;
    }
    result[len] = '\0';

    return result;
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

Fd fd_open_for_read(Cstr path)
{
    Fd result = open(path, O_RDONLY);
    if (result < 0) {
        PANIC("Could not open file %s: %s", path, strerror(errno));
    }
    return result;
}

Fd fd_open_for_write(Cstr path)
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

Cstr cmd_show(Cmd cmd)
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
        PANIC("Could not fork child process: %s: %s",
              cmd_show(cmd), strerror(errno));
    }

    if (cpid == 0) {
        Cstr_Array args = cstr_array_append(cmd.line, NULL);

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

        if (execvp(args.elems[0], (char * const*) args.elems) < 0) {
            PANIC("Could not exec child process: %s: %s",
                  cmd_show(cmd), strerror(errno));
        }
    }

    return cpid;
}

void cmd_run_sync(Cmd cmd)
{
    pid_wait(cmd_run_async(cmd, NULL, NULL));
}

static void chain_set_input_output_files_or_count_cmds(Chain *chain, Chain_Token token)
{
    switch (token.type) {
    case CHAIN_TOKEN_CMD: {
        chain->cmds.count += 1;
    }
    break;

    case CHAIN_TOKEN_IN: {
        if (chain->input_filepath) {
            PANIC("Input file path was already set");
        }

        chain->input_filepath = token.args.elems[0];
    }
    break;

    case CHAIN_TOKEN_OUT: {
        if (chain->output_filepath) {
            PANIC("Output file path was already set");
        }

        chain->output_filepath = token.args.elems[0];
    }
    break;

    case CHAIN_TOKEN_END:
    default: {
        assert(0 && "unreachable");
        exit(1);
    }
    }
}

static void chain_push_cmd(Chain *chain, Chain_Token token)
{
    if (token.type == CHAIN_TOKEN_CMD) {
        chain->cmds.elems[chain->cmds.count++] = (Cmd) {
            .line = token.args
        };
    }
}

Chain chain_build_from_tokens(Chain_Token first, ...)
{
    Chain result = {0};

    chain_set_input_output_files_or_count_cmds(&result, first);
    va_list args;
    va_start(args, first);
    Chain_Token next = va_arg(args, Chain_Token);
    while (next.type != CHAIN_TOKEN_END) {
        chain_set_input_output_files_or_count_cmds(&result, next);
        next = va_arg(args, Chain_Token);
    }
    va_end(args);

    result.cmds.elems = reinterpret_cast<Cmd*>(malloc(sizeof(result.cmds.elems[0]) * result.cmds.count));
    if (result.cmds.elems == NULL) {
        PANIC("could not allocate memory: %s", strerror(errno));
    }
    result.cmds.count = 0;

    chain_push_cmd(&result, first);

    va_start(args, first);
    next = va_arg(args, Chain_Token);
    while (next.type != CHAIN_TOKEN_END) {
        chain_push_cmd(&result, next);
        next = va_arg(args, Chain_Token);
    }
    va_end(args);

    return result;
}

void chain_run_sync(Chain chain)
{
    if (chain.cmds.count == 0) {
        return;
    }

    Pid *cpids = reinterpret_cast<Pid*>(malloc(sizeof(Pid) * chain.cmds.count));

    Pipe pip = {0};
    Fd fdin = 0;
    Fd *fdprev = NULL;

    if (chain.input_filepath) {
        fdin = fd_open_for_read(chain.input_filepath);
        if (fdin < 0) {
            PANIC("could not open file %s: %s", chain.input_filepath, strerror(errno));
        }
        fdprev = &fdin;
    }

    for (size_t i = 0; i < chain.cmds.count - 1; ++i) {
        pip = pipe_make();

        cpids[i] = cmd_run_async(
                       chain.cmds.elems[i],
                       fdprev,
                       &pip.write);

        if (fdprev) fd_close(*fdprev);
        fd_close(pip.write);
        fdprev = &fdin;
        fdin = pip.read;
    }

    {
        Fd fdout = 0;
        Fd *fdnext = NULL;

        if (chain.output_filepath) {
            fdout = fd_open_for_write(chain.output_filepath);
            if (fdout < 0) {
                PANIC("could not open file %s: %s",
                      chain.output_filepath,
                      strerror(errno));
            }
            fdnext = &fdout;
        }

        const size_t last = chain.cmds.count - 1;
        cpids[last] =
            cmd_run_async(
                chain.cmds.elems[last],
                fdprev,
                fdnext);

        if (fdprev) fd_close(*fdprev);
        if (fdnext) fd_close(*fdnext);
    }

    for (size_t i = 0; i < chain.cmds.count; ++i) {
        pid_wait(cpids[i]);
    }
}

void chain_echo(Chain chain)
{
    printf("[INFO] CHAIN:");
    if (chain.input_filepath) {
        printf(" %s", chain.input_filepath);
    }

    FOREACH_ARRAY(Cmd, cmd, chain.cmds, {
        printf(" |> %s", cmd_show(*cmd));
    });

    if (chain.output_filepath) {
        printf(" |> %s", chain.output_filepath);
    }

    printf("\n");
}

Cstr path_get_current_dir()
{
    char *buffer = reinterpret_cast<char*>(std::malloc(PATH_MAX));
    if (getcwd(buffer, PATH_MAX) == NULL) {
        PANIC("could not get current directory: %s", strerror(errno));
    }

    return buffer;
}

void path_set_current_dir(Cstr path)
{
    if (chdir(path) < 0) {
        PANIC("could not set current directory to %s: %s",
              path, strerror(errno));
    }
}

int path_exists(Cstr path)
{
    struct stat statbuf = {0};
    if (stat(path, &statbuf) < 0) {
        if (errno == ENOENT) {
            errno = 0;
            return 0;
        }

        PANIC("could not retrieve information about file %s: %s",
              path, strerror(errno));
    }

    return 1;
}

int path_is_dir(Cstr path)
{
    struct stat statbuf = {0};
    if (stat(path, &statbuf) < 0) {
        if (errno == ENOENT) {
            errno = 0;
            return 0;
        }

        PANIC("could not retrieve information about file %s: %s",
              path, strerror(errno));
    }

    return S_ISDIR(statbuf.st_mode);
}

void path_rename(const char *old_path, const char *new_path)
{
    if (rename(old_path, new_path) < 0) {
        PANIC("could not rename %s to %s: %s", old_path, new_path,
              strerror(errno));
    }
}

void path_mkdirs(Cstr_Array path)
{
    if (path.count == 0) {
        return;
    }

    size_t len = 0;
    for (size_t i = 0; i < path.count; ++i) {
        len += strlen(path.elems[i]);
    }

    size_t seps_count = path.count - 1;
    const size_t sep_len = strlen(PATH_SEP);

    char *result = reinterpret_cast<char*>(std::malloc(len + seps_count * sep_len + 1));

    len = 0;
    for (size_t i = 0; i < path.count; ++i) {
        size_t n = strlen(path.elems[i]);
        memcpy(result + len, path.elems[i], n);
        len += n;

        if (seps_count > 0) {
            memcpy(result + len, PATH_SEP, sep_len);
            len += sep_len;
            seps_count -= 1;
        }

        result[len] = '\0';

        if (mkdir(result, 0755) < 0) {
            if (errno == EEXIST) {
                errno = 0;
                WARN("directory %s already exists", result);
            } else {
                PANIC("could not create directory %s: %s", result, strerror(errno));
            }
        }
    }
}

void path_rm(Cstr path)
{
    if (IS_DIR(path)) {
        FOREACH_FILE_IN_DIR(file, path, {
            if (strcmp(file, ".") != 0 && strcmp(file, "..") != 0)
            {
                path_rm(PATH(path, file));
            }
        });

        if (rmdir(path) < 0) {
            if (errno == ENOENT) {
                errno = 0;
                WARN("directory %s does not exist", path);
            } else {
                PANIC("could not remove directory %s: %s", path, strerror(errno));
            }
        }
    } else {
        if (unlink(path) < 0) {
            if (errno == ENOENT) {
                errno = 0;
                WARN("file %s does not exist", path);
            } else {
                PANIC("could not remove file %s: %s", path, strerror(errno));
            }
        }
    }
}

int is_path1_modified_after_path2(const char *path1, const char *path2)
{
    struct stat statbuf = {0};

    if (stat(path1, &statbuf) < 0) {
        PANIC("could not stat %s: %s\n", path1, strerror(errno));
    }
    int path1_time = statbuf.st_mtime;

    if (stat(path2, &statbuf) < 0) {
        PANIC("could not stat %s: %s\n", path2, strerror(errno));
    }
    int path2_time = statbuf.st_mtime;

    return path1_time > path2_time;
}

void VLOG(FILE *stream, Cstr tag, Cstr fmt, va_list args)
{
    fprintf(stream, "[%s] ", tag);
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}

void INFO(Cstr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "INFO", fmt, args);
    va_end(args);
}

void WARN(Cstr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "WARN", fmt, args);
    va_end(args);
}

void ERRO(Cstr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "ERRO", fmt, args);
    va_end(args);
}

void PANIC(Cstr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    VLOG(stderr, "ERRO", fmt, args);
    va_end(args);
    exit(1);
}

char *shift_args(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argc -= 1;
    *argv += 1;
    return result;
}

#endif // NOBUILD_IMPLEMENTATION
