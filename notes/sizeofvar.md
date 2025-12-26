## Why Sizeof Variable Matters?

[Video](https://youtu.be/hwyRnHA54lI?si=D5QBjPgSuC1H_6TC)

1. **Fixed-size types** exist in C++, Rust (Systems Programming Languages), as compared to Python and JavaScript (Scripting Languages).
2. In SPLs, you have explicit control over how much memory is used to represent data.
3. Fixed types tell the compiler how much space is needed in memory.
4. Computers operate in **bytes (8 bits)**; hence sizes always appear in multiples of 8 (not arbitrary values like 3, 4, or 5).
5. We might allocate more space to something that requires a smaller range by definition.

![Interpreted language allocation](assets/interpretedLangvariablealloc.png)

6. Hence, we end up using more memory than necessary in scripting languages (since resource allocation is handled by the interpreter).
7. Enforcing a fixed type avoids mis-allocations (e.g., rejecting a negative value for an `age` variable).

---

### Performance Implications

1. In scripting languages, once the interpreter assigns memory to variables, how does it later know whether those bits represent a string, number, or something else (since everything is machine code)?
2. This is solved by attaching **runtime type metadata (tags)** to values.  
   This enables runtime type checks and errors (Python throws errors; JavaScript often doesn’t).

![JavaScript type coercion](assets/javascriptHavocks.png)

3. These tags require extra memory.
4. Tags must be:
   - initialized,
   - read,
   - compared,
   - written at runtime.

   All of this adds runtime cost. As a result, the CPU spends time performing comparisons instead of arithmetic.

![Tag comparison function](assets/tagCompFunction.png)

![Tag comparison](assets/tagComparison.png)

5. In compiled languages, this overhead is removed.  
   The compiler directly generates assembly that performs arithmetic without runtime type checks.

![Runtime overhead](assets/runtimeOverhead.png)

---

### What if size is unknown at compile time?

1. We keep references to memory regions that can grow or shrink at runtime (stack or heap).
2. **Stack**: fixed-size, fast, scoped  
   **Heap**: dynamic-size, flexible, slower
