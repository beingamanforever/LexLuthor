# Move Semantics
## Lectures Referred
1. Back to Basics: Move Semantics: CppCon 2021: https://youtu.be/Bt3zcJZIalk?si=Vodi0-QHJUGM8NPE
--- Notes Start Here ---
1. Motivation for move semantics
    - Copying large objects is expensive
    - Heap allocations are expensive
2. Avoid Objects with names, but sometimes you need them
    - std::move -> "I dont need this value here anymore, you can steal its resources"
    - Rvalue references (T&&) -> can bind to temporaries (rvalues)
    - Lvalue references (T&) -> can bind to named objects (lvalues)
    - what move does is: casts an lvalue to an rvalue -> T& to T&&
3. 