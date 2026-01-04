#include <iostream>

// SAFE: object is originally non-const, but we only have a const view.
void safe_mutate_through_const_view(const int *p)
{
    // Removing const is allowed; mutating is only defined if *p was originally non-const.
    int *q = const_cast<int *>(p);
    *q = 42;
}

// UB: object is originally const.
void ub_mutate_const_object(const int *p)
{
    int *q = const_cast<int *>(p);
    *q = 99; // undefined behavior if *p refers to a truly-const object
}

int main()
{
    {
        int nonConstInt = 10;
        const int *pToNonConstInt = &nonConstInt; // const view of a non-const object
        safe_mutate_through_const_view(pToNonConstInt);
        std::cout << "SAFE nonConstInt: " << nonConstInt << '\n';
    }

#ifdef RUN_UB_DEMO
    {
        const int constInt = 10;
        const int *pToConstInt = &constInt;
        ub_mutate_const_object(pToConstInt); // UB: may crash, may print 10, may print 99...
        std::cout << "UB constInt: " << constInt << '\n';
    }
#endif
}
