#include <iostream>

struct RealFunction {
    virtual double operator()(unsigned idx) const = 0;
};

unsigned max_at(unsigned begin, unsigned end, RealFunction const &f);
        
double derivative(unsigned at, RealFunction const &f);
        
struct DerivativeFunction: public RealFunction {
    RealFunction const &f;

    DerivativeFunction(RealFunction const &f): f(f) {};

    double operator()(unsigned idx) const override{
        return derivative(idx, f);
    }
};

// int main() {
//     SimpleFunction f;
//     RealFunction const &rf = f;
//     DerivativeFunction g(rf);
//     std::cout << max_at(1, 1024, g);
//     return 0;
// }