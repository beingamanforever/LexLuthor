# Your new mental model for constexpr
CppCon 2021 - https://youtu.be/MdrfPSUtMVM?si=nAFzN2AWLx3aAzdG

---

1. What is constexpr?
    - Moving computation from runtime to compile time
    - constexpr is NOT metaprogramming
        - Metaprogramming: programs that manipulate other programs as data
        - constexpr: regular code, just executed at compile time

2. What can be done at compile time?
    - Anything that can be done at runtime can be done at compile time
    - C++20 consteval: Turing complete at compile time
    - C++11/14/17 constexpr: not Turing complete at compile time

3. The constexpr continuum:
    - Deciding how much work you do at compile time vs runtime
    - 0% (all runtime) ... constexpr ... 100% (all compile time)

4. Practical use cases for speed:
    - Precomputed lookup tables:
        - CRC32, hash functions, trigonometry tables
        - Compute once at compile time, look up at runtime
        ```cpp
        constexpr std::array<uint32_t, 256> make_crc_table() {
            std::array<uint32_t, 256> table{};
            for (uint32_t i = 0; i < 256; i++) {
                uint32_t crc = i;
                for (int j = 0; j < 8; j++) {
                    crc = (crc >> 1) ^ ((crc & 1) ? 0xEDB88320 : 0);
                }
                table[i] = crc;
            }
            return table;
        }
        constexpr auto CRC_TABLE = make_crc_table();
        ```
    
    - String processing:
        - Convert string encodings at compile time (ASCII to PETSCII, etc.)
        - Useful for embedded systems, game dev on retro hardware
        ```cpp
        constexpr auto PETSCII(const char* str) {
            // conversion logic
        }
        static constexpr auto greeting = PETSCII("Hello World");
        ```
    
    - String tables:
        - Store multiple strings in one contiguous memory block
        - Better for cache, better for embedded systems with limited RAM
        ```cpp
        // instead of this (scattered memory)
        const char* s1 = "Error"; const char* s2 = "Warning";
        
        // do this (one block, built at compile time)
        constexpr auto LOG_LEVELS = MAKE_STRING_TABLE("Error", "Warning", "Info");
        // memory layout: "Error\0Warning\0Info\0" + index array
        // access: LOG_LEVELS[1] returns "Warning"
        ```
    
    - Complex math at compile time:
        - Matrix operations, projection matrices for graphics
        - All calculations done by compiler, executable has final values
        ```cpp
        constexpr Matrix4x4 computeProjectionMatrix(float fov, float aspect) {
            // trig calculations
            return {/* ... */};
        }
        constexpr auto PROJ_MATRIX = computeProjectionMatrix(90.0f, 16.0f/9.0f);
        ```

5. The three keywords:
    - constexpr: may evaluate at compile time
        - Compiler can choose to evaluate at compile time or runtime
        ```cpp
        constexpr int factorial(int n) {
            return (n <= 1) ? 1 : n * factorial(n - 1);
        }
        constexpr int fact5 = factorial(5);  // guaranteed compile time
        int x = 6;
        int factX = factorial(x);            // runtime (x is not constexpr)
        ```
    
    - consteval (C++20): must evaluate at compile time
        - Forces compile-time evaluation
        - Compile error if called with runtime values
        ```cpp
        consteval int factorial(int n) {
            return (n <= 1) ? 1 : n * factorial(n - 1);
        }
        constexpr int x = factorial(5);  // OK
        int n = 5;
        int y = factorial(n);            // compile error
        ```
    
    - constinit (C++20): compile-time initialization, runtime mutability
        - For globals/statics that need zero-cost initialization but can change
        ```cpp
        constinit int counter = 0;  // initialized at compile time
        counter++;                  // can modify at runtime
        ```

6. When compiler evaluates at compile time:
    - Using in constexpr/consteval context: guaranteed compile time
    - Using with static_assert: guaranteed compile time
    - Using with const: maybe compile time (compiler decides)
    - Using with non-const runtime variables: runtime
    ```cpp
    constexpr int factorial(int n) {
        return (n <= 1) ? 1 : n * factorial(n - 1);
    }
    constexpr int fact5 = factorial(5);     // compile time
    static_assert(factorial(4) == 24);      // compile time
    const int fact6 = factorial(6);         // maybe compile time
    int x = 7;
    int factX = factorial(x);               // runtime
    ```

7. Best practices:
    - When to use:
        - Math constants: PI, E, conversion factors
        - Lookup tables: CRC, trigonometry, color palettes
        - Configuration data that doesn't change
        - Powers of 2: use bit shifts computed at compile time
        ```cpp
        constexpr int MAX_PLAYERS = 64;
        constexpr float PI = 3.14159265359f;
        constexpr int KB = 1 << 10;  // instead of std::pow(2, 10)
        constexpr int MB = 1 << 20;
        ```
    
    - Use std::array for constexpr containers:
        ```cpp
        constexpr std::array<int, 5> nums = {1, 2, 3, 4, 5};  // good
        constexpr int nums[] = {1, 2, 3, 4, 5};               // old style
        ```
    
    - Trade-offs:
        - Compilation time increases with complex constexpr
        - Binary size grows (precomputed data lives in executable)
        - Debugging compile-time code is harder
        - Sweet spot: things that truly don't change and are expensive to compute

8. Examples of runtime speed-ups:
    - Color parsing in games:
        ```cpp
        // runtime: parse hex color every frame
        Color parseHex(const char* hex) { /* parsing */ }
        draw(parseHex("#FF5733"));  // wasteful
        
        // compile time: color ready to use
        constexpr auto FIRE_COLOR = parseHex("#FF5733");
        draw(FIRE_COLOR);  // just loads the value
        ```
    
    - Configuration baking:
        ```cpp
        // runtime: parse config file at startup
        Config loadConfig("settings.json");
        
        // compile time: config already in the binary
        constexpr auto CONFIG = parseConfigFile("settings.json");
        ```

Key takeaways:
- Compile time = prep work, Runtime = serving results
- Move expensive work to compile time if value doesn't change
- Trade-off: slower compilation for faster execution
- constexpr (flexible), consteval (strict), constinit (globals)
- Best for: lookup tables, math constants, string processing, config data

---
