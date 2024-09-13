#ifndef _FRACTIONS_HH_
    #define _FRACTIONS_HH_

    #include <cmath>
    #include <numeric>
    #include <iostream>
    
    class Fraction {
        public:
            int numerator;
            int denominator;

            explicit Fraction(int numerator, int denominator) {
                this->numerator = numerator;
                this->denominator = denominator;
            }

            Fraction(double x) {
                if (x == std::floor(x)) {
                    this->numerator = x;
                    this->denominator = 1;
                } else {
                    double b, c;
                    
                    if (x < 1) {
                        b = 1 / x;
                        c = x * b;
                    } else {
                        b = 1 / (x - std::floor(x));
                        c = x * b;
                    }
                    std::cout << b << ", " << c << std::endl;

                    this->numerator = c;
                    this->denominator = b;
                }
            }

            Fraction simplify() {
                int x = std::gcd(numerator, denominator);
                return Fraction(numerator / x, denominator / x);
            }

            Fraction operator+(Fraction x) {
                int denom = x.denominator * denominator;
                return Fraction(x.numerator * denom / x.denominator + numerator * denom / denominator, denom).simplify();
            }

            Fraction operator-(Fraction x) {
                int denom = x.denominator * denominator;
                return Fraction(x.numerator * denom / x.denominator - numerator * denom / denominator, denom).simplify();
            }

            Fraction operator*(Fraction x) {
                std::cout << numerator << "/" << denominator << ", " << x.numerator << "/" << x.denominator << std::endl;
                return Fraction(x.numerator * numerator, x.denominator * denominator).simplify();
            }

            Fraction operator/(Fraction x) {
                return Fraction(x.denominator * numerator, x.numerator * denominator).simplify();
            }

            Fraction operator+(double x) {
                return Fraction(x * numerator, denominator);
            }
    };

    class Number {
        public:
            double number = 0.0;
            Fraction fraction = NULL;
            bool x;

            Number(double number) {
                this->number = number;
                this->x = false;
            }

            Number(Fraction fraction) {
                this->fraction = fraction;
                this->x = true;
            }
    };
#endif

