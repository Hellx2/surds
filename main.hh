#include <cmath>
#include <numeric>
#include <iostream>
#include <cstdlib>
#include <vector>

#ifndef _MAIN_HH_
    #define _MAIN_HH_

    class Surd;
    class Fraction;
    
    class Number;

    class Expression;
    
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
                return Fraction(x.numerator * numerator, x.denominator * denominator).simplify();
            }

            Fraction operator/(Fraction x) {
                return Fraction(x.denominator * numerator, x.numerator * denominator).simplify();
            }

            Fraction operator+(double x) {
                return Fraction(x * numerator, denominator);
            }
    };

    /**
    A class for representing a surd, contains a fraction to
    represent the inner expression, and a root to use as the
    n-th root for simplification.
     */
    class Surd {
        public:
            Fraction inner = Fraction(1.0);
            int root;

            Expression operator+(Surd other);
            Expression operator-(Surd other);

            explicit Surd(Fraction inner, int root) {
                this->inner = inner;
                this->root = root;
            }

            explicit Surd(Fraction inner) {
                this->inner = inner;
                this->root = 2;
            }

            Surd operator/(Surd other) {
                if (other.root != root) {
                    std::cerr << "Unable to divide with different roots!" << std::endl;
                    exit(1);
                }

                return Surd(inner / other.inner, root);
            }

            Surd operator*(Surd other) {
                if (other.root != root) {
                    std::cerr << "Unable to multiply with different roots!" << std::endl;
                    exit(1);
                }

                return Surd(inner * other.inner, root);
            }

            /**
            Takes the current Surd and simplifies it, extracting
            all of the squares and placing them into the first
            fraction returned, before placing the inside of the
            root into the second fraction.
             */
            std::vector<Fraction> extractSquares() {
                Fraction frac = inner;
                Fraction a(1.0);
                
                numerator:
                double x = std::sqrt(frac.numerator);

                if (std::floor(x) == x && root == 2) {
                    a.numerator = x;
                    frac.numerator = 1;
                } else {
                    for (int b = std::floor(x); b >= 2; b--) {
                        double y = (double)frac.numerator / std::pow(b, root);
                        if (y == std::floor(y)) {
                            a.numerator *= b;
                            frac.numerator /= std::pow(b, root);
                            goto numerator;
                        }
                    }
                } 

                denominator:
                x = std::sqrt(frac.denominator);

                if (std::floor(x) == x && root == 2) {
                    a.denominator = x;
                    frac.denominator = 1;
                } else {
                    for (int b = std::floor(x); b >= 2; b--) {
                        double y = (double)frac.denominator / std::pow(b, root);
                        if (y == std::floor(y)) {
                            a.denominator *= b;
                            frac.denominator /= std::pow(b, root);
                            goto denominator;
                        }
                    }
                }

                return {a, frac};
            }
    };

    class Number {
        public:
            Surd surd = Surd(1.0);
            Fraction fraction = Fraction(1.0);
            Expression *expression;
            short x;

            Number(Surd surd) {
                this->surd = surd;
                this->x = 0;
            }

            Number(Fraction fraction) {
                this->fraction = fraction;
                this->x = 1;
            }

            static Number fromExpression(Expression expr);
    };
    
    enum Operator {
        Add,
        Min,
        Div,
        Mul
    };

    class Expression {
        public:
            bool add = true;
            // Vector of numbers added together
            std::vector<Number> coefficient;
            Surd base = Surd(Fraction(1.0));
            
            explicit Expression(std::vector<Number> coefficient, Surd base) {
                this->coefficient = coefficient;
                this->base = base;
            }

            explicit Expression(bool add, std::vector<Number> coefficient, Surd base) {
                this->add = add;
                this->coefficient = coefficient;
                this->base = base;
            }
            
            void simplifyCoefficient() {
                Fraction fractional(0.0);
                bool g = false;
                std::vector<Number> surds;
                for (int i = 0; i < coefficient.size(); i++) {
                    if (coefficient[i].x == 1) {
                        if(g && !add) fractional = fractional - coefficient[i].fraction;
                        else if(g && add) fractional = fractional + coefficient[i].fraction;
                        else fractional = coefficient[i].fraction;

                        g = true;
                    } else {
                        std::vector<Fraction> f = coefficient[i].surd.extractSquares();

                        if (f[1].numerator == 1) {
                            if(g) fractional = fractional + Fraction(add ? 1 : -1) * f[0];
                            else fractional = f[0];
                            g = true;
                        } else {
                            surds.push_back(coefficient[i].surd);
                        }
                    }
                }
                if (fractional.numerator != 0) {
                    if(add) surds.insert(surds.begin(), fractional.simplify());
                    else surds.push_back(fractional.simplify());
                }
                coefficient = surds;
            }
            
            void display() {
                bool a = true;
                simplifyCoefficient();

                if(coefficient.empty()) {
                    a = false;
                } else if(coefficient.size() == 1) {
                    if (coefficient[0].x) {
                        std::cout << coefficient[0].fraction.numerator;
                        if(coefficient[0].fraction.denominator != 1) std::cout << "/" << coefficient[0].fraction.denominator;
                    } else {
                        Surd s = coefficient[0].surd;
                        if (s.root != 2) {
                            std::cout << s.root;
                        }
                        std::cout << "sqrt(" << s.inner.numerator;
                        if(s.inner.denominator != 1)
                            std::cout << "/" << s.inner.denominator;
                        std::cout << ")" << std::endl;
                    }
                } else {
                    std::cout << "(";
                    int i;
                    for(i = 0; i < coefficient.size() - 1; i++) {
                        if (coefficient[i].x) {
                            std::cout << coefficient[i].fraction.numerator;
                            if (coefficient[i].fraction.denominator != 1) std::cout << "/" << coefficient[i].fraction.denominator;
                        } else {
                            Surd s = coefficient[i].surd;
                            if (s.root != 2) {
                                std::cout << s.root;
                            }
                            std::cout << "sqrt(" << s.inner.numerator;
                            if(s.inner.denominator != 1)
                                std::cout << "/" << s.inner.denominator;
                            std::cout << ")";
                        }
                        std::cout << (add ? " + " : " - ");
                    }

                    if (coefficient[i].x == 1) {
                        std::cout << coefficient[i].fraction.numerator;
                        if (coefficient[i].fraction.denominator != 1) std::cout << "/" << coefficient[i].fraction.denominator;
                    } else {
                        Surd s = coefficient[i].surd;
                        if (s.root != 2) {
                            std::cout << s.root;
                        }
                        std::cout << "sqrt(" << s.inner.numerator;
                        if(s.inner.denominator != 1)
                            std::cout << "/" << s.inner.denominator;
                        std::cout << ")";
                    }
                    std::cout << ")";
                    // loop through and print a plus after each except the last, using the print algorithm for the single item from right before
                }
                if(base.inner.numerator != 1 || base.inner.denominator != 1) {
                    std::cout << " * ";
                    std::vector<Fraction> f = base.extractSquares();

                    if (f[0].numerator != 1 || f[0].denominator != 1) {
                        std::cout << f[0].numerator;
                        if (f[0].denominator != 1) {
                            std::cout << "/" << f[0].denominator;
                        }
                        std::cout << " * ";
                    }
                    
                    if (base.root != 2) {
                        std::cout << base.root;
                    }
                    std::cout << "sqrt(" << f[1].numerator;
                    if(f[1].denominator != 1)
                        std::cout << "/" << f[1].denominator;
                    std::cout << ")";
                } else if(!a) {
                    std::cout << 1;
                }
                std::cout << std::endl;
            }
    };

    inline Expression Surd::operator+(Surd other) {
        Surd s = *this;
        
        Fraction gcd = Fraction(
            std::gcd(s.inner.numerator, other.inner.numerator),
            std::gcd(s.inner.denominator, other.inner.denominator)
        );

        s.inner.numerator /= gcd.numerator;
        other.inner.numerator /= gcd.numerator;
        
        s.inner.denominator /= gcd.denominator;
        other.inner.denominator /= gcd.denominator;

        return Expression({s, other}, Surd(gcd));
    }

    inline Expression Surd::operator-(Surd other) {
        Surd s = *this;
        
        Fraction gcd = Fraction(
            std::gcd(s.inner.numerator, other.inner.numerator),
            std::gcd(s.inner.denominator, other.inner.denominator)
        );

        s.inner.numerator /= gcd.numerator;
        other.inner.numerator /= gcd.numerator;
        
        s.inner.denominator /= gcd.denominator;
        other.inner.denominator /= gcd.denominator;

        return Expression(false, {s, other}, Surd(gcd));
    }

    inline Number Number::fromExpression(Expression expr) {
        Number num(Fraction(1.0));
        num.expression = new Expression(expr);
        num.x = 2;
        return num;
    }
#endif

