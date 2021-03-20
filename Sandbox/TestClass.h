//
// Created by eger on 06.03.2021.
//

#ifndef SANDBOX_TESTCLASS_H
#define SANDBOX_TESTCLASS_H

#include <iostream>

class TestClass {
private:
    double firstNumber;
    double secondNumber;
    double sumOfNumbers;
public:
    TestClass() : firstNumber(0), secondNumber(0)
    { }
    void set_numbers(double a, double b)
    {
        firstNumber = a;
        secondNumber = b;
    }

    void sum_numbers(unsigned int howManyTimes)
    {
        unsigned int increment = 0;
        for(unsigned int i = 0; i < howManyTimes; i++)
        {
            increment++;
        }
        sumOfNumbers = increment * (firstNumber + secondNumber);
    }

    double get_sum_of_numbers()
    {
        return sumOfNumbers;
    }
};


#endif //SANDBOX_TESTCLASS_H
