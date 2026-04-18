#include "../inc/main.hpp"
#include "../inc/D1Q2.hpp"
#include "../inc/D1Q3.hpp"
#include "../inc/D2Q5.hpp"
#include "../inc/D2Q9.hpp"

int main()
{

#ifdef use_D1Q2

    D1Q2 test(100.0, 101, 50000, 1, 0.25, 1.0, 0.0);
    test.D1Q2_t();
    D1Q2 test(100.0, 101, 50000, 1, 0.25, 0.01, 0.0);
    test.D1Q2_f();

#endif

#ifdef use_D1Q3

    D1Q3 test(100.0, 101, 0.0001, 50000, 1, 0.25, 1.0, 0.0);
    test.D1Q3_t();
    D1Q3 test(100.0, 101, 0.0001, 50000, 1, 0.25, 0.01, 0.0);
    test.D1Q3_f();
    D1Q3 test(100.0, 101, 0.0001, 50000, 1, 0.25, 1.0, 0.0);
    test.D1Q3_alpha();
    D1Q3 test(100.0, 101, 0.0001, 50000, 1, 0.25, 0.0, 0.0);
    test.D1Q3_s();
    D1Q3 test(100.0, 101, 0.0001, 50000, 1, 0.25, 0.0, 0.0);
    test.D1Q3_v();

#endif

#ifdef use_D2Q5

    D2Q5 test(100, 100, 1.0, 1.0, 50000, 1.0 / 3.0, 1.0, 0.15);
    test.D2Q5_F();

#endif

#ifdef use_D2Q9

    D2Q9 test(100, 100, 1, 0.5, 50000, 1.0 / 3.0, 1.0, 0.1);
    test.D2Q9_F();

#endif

    return 0;
}