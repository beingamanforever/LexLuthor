# Importance of Being Const
## Lecture Referred
1. Importance of being const - CppCon 2015: https://youtu.be/Y1KOuFYtTF4?si=8NJhbsMbnvHEeiVu

--- Notes Start Here ---
1. Taken from google style guide
    - ![alt text](image16.png)
2. const T t (T const t) // same thing
    - const go to interview question
    - ![alt text](image17.png)
    - Valid lines: 1, 2, 3, 6 (Read from right to left)
3. Questions on const correctness:
    - Once objects are const, they stay const (except when using const_cast)
    - compiler is free to add constness to objects but cannot take it away
    - ![alt text](image18.png)
    - compiler cannot remove constness, it can only add constness
    - ![alt text](image19.png)
    - Valid lines: 1, 5, 7, 8
    - ![alt text](image20.png)
    -  Valid lines: 1, 3, 5, 7
    - ![alt text](image21.png)
    - Valid lines: 1, 3, 4, 5, 6
    - Member functions can be marked const to promise not to modify the object
    - ```cpp
        class T{
            returnValue FunctionName(args) CV-qualifiers; // CV-qualifiers: const, volatile, const volatile

        };```

    - How the compiler sees them?
        - Step 1: Original member function
        ```cpp
        int Foo::GetValue() const
        {
            return mValue;
        }
        ```
        
        - Step 2: `this` pointer added as first hidden argument
        ```cpp
        int Foo::GetValue(Foo const* const this)
        {
            return mValue;
        }
        ```
        
        - Step 3: CV qualifiers on `this` pointer
        ```cpp
        // const member function → this is pointer to const
        int Foo::GetValue(Foo const* const this)
        {
            return this->mValue;
        }
        ```
        
        - Step 4: Non-const member function for comparison
        ```cpp
        // non-const member function → this is pointer to non-const
        int Foo::GetValue(Foo* const this)
        {
            return this->mValue;
        }
        ```
        
        - Step 5: Function invocation translated
        ```cpp
        // What you write:
        Foo f;
        auto v = f.GetValue();
        
        // How compiler sees it:
        Foo f;
        auto v = GetValue(&f);
        ```
        
        - Step 6: Name mangling (implementation-defined)
        ```cpp
        // Compiler generates mangled name
        int __ZNK3Foo8GetValueEv(Foo const* const this)
        {
            return this->mValue;
        }
        
        // Function call becomes:
        Foo f;
        auto v = __ZNK3Foo8GetValueEv(&f);
        ```
        
        - Key insight: const member functions take `Foo const*`, non-const take `Foo*`
        - This is why you can't call non-const methods on const objects
    - ![alt text](image27.png)
    - Function overloading on const:
        - Non-const object: calls non-const version (exact match)
        - Const object: calls const version (can only call const methods)
        - Non-const object → can call const methods (safe, adds constness)
        - Const object → cannot call non-const methods (unsafe, would remove constness)
        - This is why const-correctness matters: const objects are restricted to const member functions only.
    - ![alt text](image28.png)