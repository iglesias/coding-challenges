//TODO CMD macro; param for CPPFLAGS
#define NOBUILD_IMPLEMENTATION
#include "./nob.h"

#define CFLAGS "-Wall", "-Wextra", "-std=c2x", "-pedantic"

void build_kattis_c_file(const char *file)
{
    Cstr tool_path = PATH("kattis", file);
    CMD("cc", CFLAGS, "-o", NOEXT(tool_path), tool_path);
}

void build_kattis_c_files(void)
{
    FOREACH_FILE_IN_DIR(file, "kattis", {
        if (ENDS_WITH(file, ".c")) {
            build_kattis_c_file(file);
        }
    });
}

void run_example(const char *example)
{
    Cstr example_path = PATH("uva", example);
    //TODO: fix below.
    CMD("g++", "-Wall", "-Wextra", "-pedantic", "-Wconversion", "-Wfatal-errors", "-fsanitize=undefined,address", "-o", NOEXT(example_path), example_path, "-lgtest");
    CMD(NOEXT(example_path));
}

void run_examples(void)
{
    FOREACH_FILE_IN_DIR(example, "uva", {
        if (ENDS_WITH(example, "summing_digits.cpp")) {
            run_example(example);
        }
    });
}


void print_chain(const Chain *chain)
{
    INFO("input: %s", chain->input_filepath);
    INFO("output: %s", chain->output_filepath);
    FOREACH_ARRAY(Cmd, cmd, chain->cmds, {
        INFO("cmd: %s", cmd_show(*cmd));
    });
}

int main(int argc, char **argv)
{
    GO_REBUILD_URSELF(argc, argv);

    build_kattis_c_files();
    run_examples();

    return 0;
}
