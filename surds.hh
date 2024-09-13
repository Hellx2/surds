#ifndef _SURDS_HH_
    #define _SURDS_HH_

    #include "fractions.hh"
    
    #include <cmath>
    #include <cstdlib>
    #include <iostream>
    #include <vector>

    class Surd {
        public:
            Fraction inner = Fraction(0.0);
            int root;

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
#endif

