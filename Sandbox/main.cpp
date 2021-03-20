#include <iostream>
#include "TestClass.h"

int main() {
    TestClass c1;
    c1.set_numbers(1, 1);
    c1.sum_numbers(5);
    std::cout << "result: " << c1.get_sum_of_numbers();
    return 0;
}
