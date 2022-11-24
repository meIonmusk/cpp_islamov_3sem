#include <functional>
#include <utility>

template<typename T, typename U>
class FunctionComposition {
private:
    const T& outer;
    const U& inner;
public:
    FunctionComposition(const T& outer, const U& inner) : inner(inner) , outer(outer) {  }

    template<typename Argument>
    using RetType = decltype(std::declval<T>()(std::declval<U>()(std::declval<Argument>())));

    template<typename Argument>
    RetType<Argument> operator()(Argument argument) const{
        return outer(inner(argument));
    }
};

template<typename T, typename U>
FunctionComposition<T, U> make_function_composition(const T& f, const U& g) {
    return FunctionComposition<T, U>(f, g);
}

double f(double x) {
    return x + x;
}

struct NonCopyableFunction {
    NonCopyableFunction() = default;
    NonCopyableFunction(const NonCopyableFunction&) = delete;
    double operator()(double x) const {
        return x * x;
    }
};

#include <iostream>
int main() {
    NonCopyableFunction g;
    auto h1 = make_function_composition(f, g); // f(g())
    auto res1 = h1(2.0); // res1 <- 8.0

    auto h2 = make_function_composition(g, f); // g(f())
    auto res2 = h2(2.0); // res2 <- 16.0
    std::cout << res1 << ' ' << res2 << std::endl;
    return 0;
}