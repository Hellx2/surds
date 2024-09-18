#include "fractions.hh"

#include <iostream>
#include <algorithm>
#include <numeric>

Fraction Fraction::simplify() {
    int x = std::gcd(numerator, denominator);
    return Fraction(numerator / x, denominator / x);
}

Fraction Fraction::operator+(Fraction x) {
    int denom = x.denominator * denominator;
    return Fraction(x.numerator * denom / x.denominator + numerator * denom / denominator, denom).simplify();
}

Fraction Fraction::operator-(Fraction x)  {
    int denom = x.denominator * denominator;
    return Fraction(x.numerator * denom / x.denominator - numerator * denom / denominator, denom).simplify();
}

Fraction Fraction::operator*(Fraction x) {
    return Fraction(x.numerator * numerator, x.denominator * denominator).simplify();
}

Fraction Fraction::operator/(Fraction x) {
    return Fraction(x.denominator * numerator, x.numerator * denominator).simplify();
}

Fraction Fraction::operator+(double x) {
    return Fraction(x * numerator, denominator);
}

inline Fraction::Fraction(double x) {
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

Fraction::Fraction(int numerator, int denominator) {
    this->numerator = numerator;
    this->denominator = denominator;
}
/**
Takes a string containing only values separated
by `/` and performs division between them all as
fractions.
 */
Fraction calcDivs(String str) {
    Vec<double> divVals;
    Vec<String> x = split(str, "/");

    for (int i = 0; i < x.size(); ++i) {
        trim(x[i]);
        
        divVals.push_back(std::stod(x[i]));
    }

    Fraction retval(divVals[0]);
    for (int i = 1; i < divVals.size(); ++i) {
        retval = retval / Fraction(divVals[i]);
    }

    return retval;
}

/**
Takes a string containing expressions separated
by `*` and performs multiplication between the
parsed expressions as fractions.
 */
Fraction calcMuls(String str) {
    Vec<Fraction> mulVals;
    Vec<String> x = split(str, "*");

    for (int i = 0; i < x.size(); ++i) {
        trim(x[i]);

        if ((int)x[i].find('/') != -1) {
            mulVals.push_back(calcDivs(x[i]));
        } else {
            mulVals.push_back(Fraction(std::stod(x[i])));
        }
    }

    Fraction retval(1.0);

    for (int i = 0; i < mulVals.size(); i++) {
        retval = retval * mulVals[i];
    }

    return retval;
}

/**
Takes a string containing expressions separated
by `-` and performs subtraction between the parsed
expressions as fractions.
 */
Fraction calcMins(String str) {
    Vec<Fraction> minVals;
    Vec<String> x = split(str, "-");
    for (int i = 0; i < x.size(); ++i) {
        trim(x[i]);
        if ((int)x[i].find('*') != -1) {
            minVals.push_back(calcDivs(x[i]));
        } else if ((int)x[i].find('/') != -1) {
            minVals.push_back(calcDivs(x[i]));
        } else {
            minVals.push_back(Fraction(std::stod(x[i])));
        }
    }

    Fraction retval = minVals[0];

    for (int i = 1; i < minVals.size(); i++) {
        retval = retval - minVals[i];
    }

    return retval;
}

/**
Takes a string containing expressions separated
by `+` and performs addition between the parsed
expressions as fractions.
 */
Fraction calcAdds(String str) {
    Vec<Fraction> addVals;
    Vec<String> x = split(str, "+");

    for (int i = 0; i < x.size(); ++i) {
        trim(x[i]);
        if ((int)x[i].find('-') != -1) {
            addVals.push_back(calcMins(x[i]));
        } else if ((int)x[i].find('*') != -1) {
            addVals.push_back(calcMuls(x[i]));
        } else if ((int)x[i].find('/') != -1) {
            addVals.push_back(calcDivs(x[i]));
        } else {
            addVals.push_back(Fraction(std::stod(x[i])));
        }
    }

    Fraction retval(0.0);
    for (int i = 0; i < addVals.size(); i++) {
        retval = retval + addVals[i];
    }

    return retval;
}

/**
Trims a string at both ends to remove whitespace.

Notes:
 - Not my code, came from StackOverflow.
 - Performs trimming in place.
 */
inline void trim(String &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

/**
Splits a string into multiple substrings by the
given delimiter.

Notes:
 - Delimiter is a reference to reduce allocations.
 - Not my code, came from StackOverflow
 */
Vec<String> split(String s, const String& delimiter) {
    Vec<String> tokens;
    size_t pos = 0;
    String token;

    while ((int)(pos = s.find(delimiter)) != -1) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    tokens.push_back(s);

    return tokens;
}

/**
Operator implementation to allow printing the Fraction
class for the user to see, shown as `numerator / denominator`
unless `denominator` is `1`, in which case it's shown
as `numerator`.
 */
std::ostream &operator<<(std::ostream& op, Fraction fraction) {
    if (fraction.denominator != 1) {
        return op << fraction.numerator << "/" << fraction.denominator;
    } else {
        return op << fraction.numerator;
    }
}

