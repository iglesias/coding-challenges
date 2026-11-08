objects=""

for file in *.cpp; do
    file_with_ext=$(basename "$file")

    # Strip the file extension using parameter expansion.
    name="${file_with_ext%.*}"

    # Compile, run, and rename the profile output.
    clang++ -std=c++23 "$file_with_ext" \
        -lgtest \
        -fprofile-instr-generate -fcoverage-mapping -O0 -g \
         -o "$name" \
        || continue

    "./$name"
    mv default.profraw "$name.profraw"

    # Build the objects argument for llvm-cov.
    if [ -n "$objects" ]; then
        objects="$objects -object $name"
    else
        objects="$name"
    fi
done

llvm-profdata merge -sparse -o default.profdata *.profraw
llvm-cov report $objects -instr-profile=default.profdata -ignore-filename-regex="googletest|gtest"
