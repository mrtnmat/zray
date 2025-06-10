# -o game.html    // Output file, the .html extension determines the files that need to be generated: `.wasm`, `.js` (glue code) and `.html` (optional: `.data`). All files are already configured to just work.
# game.c          // The input files for compilation, in this case just one but it could be multiple code files: `game.c screen_logo.c screen_title.c screen_gameplay.c`
# -Os -Wall       // Some config parameters for the compiler, optimize code for small size and show all warnings generated
# ./path-to/libraylib.a   // This is the libraylib.a generated, it's recommended to provide it directly, with the path to it: i.e. `./raylib/src/libraylib.a`
# -Ipath          // Include path to look for additional #include .h files (if required)
# -Lpath          // Library path to look for additional library .a files (if required)
# -s USE_GLFW=3   // We tell the linker that the game/library uses GLFW3 library internally, it must be linked automatically (emscripten provides the implementation)
# -s ASYNCIFY     // Add this flag ONLY in case we are using ASYNCIFY code
# --shell-file path-to/shell.html  // All webs need a "shell" structure to load and run the game, by default emscripten has a `shell.html` but we can provide our own

emcc -o web/index.html main.c -Os -Wall ./wasm/libraylib.a -I. -I../raylib/src -L./web -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --shell-file ./shell_minimal.html