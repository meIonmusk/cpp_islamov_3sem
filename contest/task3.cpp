#include <iostream>
#include <cstdint>


class Fraction final {
private:
// public:
    // Do NOT rename
    int64_t numerator;
    uint64_t denominator;

    // Do NOT delete
    template <class T>
    friend bool operator==(const Fraction& lhs, const T& rhs);

public:
    Fraction() = delete;
    Fraction(const Fraction& rhs) = default;
    Fraction& operator=(const Fraction& rhs) = default;
    Fraction(int64_t numerator, uint64_t denominator): numerator(numerator), denominator(denominator) { simplify(); };
    // Your code goes below!
    // Add operators overload here
    
    Fraction operator +(const Fraction &f) const {
        uint64_t d = lcm(denominator, f.denominator);
        int64_t n = d / denominator * numerator + d / f.denominator * f.numerator;

        return Fraction(n, d);
    }

    Fraction operator -(const Fraction &f) const {
        uint64_t d = lcm(denominator, f.denominator);
        int64_t n = d / denominator * numerator - d / f.denominator * f.numerator;

        return Fraction(n, d);
    }

    Fraction operator *(const Fraction &f) const{
        return Fraction(numerator * f.numerator, denominator * f.denominator);
    }

    Fraction operator -() const{
        return Fraction(-numerator, denominator);
    }

    Fraction operator +=(const Fraction &f)
    {
        *this = *this + f;
        return *this;
    }

    Fraction operator -=(const Fraction &f)
    {
        *this = *this - f;
        return *this;
    }

    Fraction operator *=(const Fraction &f)
    {
        *this = *this * f;
        return *this;
    }


    uint64_t gcd(uint64_t a, uint64_t b) const { return b ? gcd(b, a % b) : a; }

    uint64_t lcm(uint64_t a, uint64_t b) const { return  a * b ? a / gcd(a, b) * b : 0 ; }

    void simplify() {
        uint64_t k = gcd(uint_abs(numerator), denominator);

        numerator = (int64_t) (numerator / k);
        denominator = (uint64_t) (denominator / k);
    }

    uint64_t uint_abs(int64_t a) const { return a >= 0? a : -a; }
};


template <typename T>
bool operator==(const Fraction& lhs, const T& rhs){
    return (long double)lhs.numerator/(long double)lhs.denominator == (long double)rhs;
}

int main(){
    // Fraction a(2, 5);
    // Fraction b(-4, 9);
    // std::cout << b.numerator << ' ' << b.denominator << std::endl;

    // Fraction c(a);
    // c *= b;
    // c +=Fraction(12, 7);
    // std::cout << c.numerator << ' ' << c.denominator;
    // std::cout << ((Fraction(2, 7) + Fraction(12, 7)) == (int64_t)2) << " ";
    std::cout << (Fraction(2, 3) == 2/3) << " ";
    return 0;
}