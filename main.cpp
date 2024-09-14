#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "main.hh"

using String = std::string;
template <class T> using Vec = std::vector<T>;

static inline void trim(String& s);

Vec<String> split(String s, const String &delimiter);

std::ostream &operator<<(std::ostream& op, Fraction fraction);

void simplify();
void expand();

Surd parseSurd(String &temp, size_t index);

Fraction calcAdds(String str);
Fraction calcMins(String str);
Fraction calcMuls(String str);
Fraction calcDivs(String str);

/**
The main function, asks whether the user wants to
simplify or expand surds, or if they want to exit.
 */
int main() {
    std::cout << "The following functions are available:" << std::endl
              << " (1) Simplify" << std::endl
              << " (2) Expand [not implemented yet]" << std::endl
              << " (3) Exit" << std::endl
              << std::endl
              << "Choose an option: ";

    short option;
    std::cin >> option;

    {
        String temp;
        std::getline(std::cin, temp);
    }
    
    std::cout << std::endl;

    switch (option) {
        case 1:
            simplify();
            break;
        case 2:
            //expand();
            break;
        case 3: return 0;
    }
}

/**
The function called when the user chooses the option
for simplifying surds. Asks for surd(s) to simplify
and then goes through each one and passes them to
the `parseSurd` function before displaying them to
the user.
 */
void simplify() {
    String surd;
    std::cout << "Enter (a) surd(s) to simplify (use 'sqrt' in place of the surd sign): ";
    std::getline(std::cin, surd);
    std::cout << std::endl;

    if ((int)surd.find("sqrt") != -1) {
        Vec<String> vars = split(surd, ",");
        String temp = surd;
        size_t index;

        for (int i = 0; i < vars.size(); i++) {
            Vec<Number> surds;
            Vec<String> ops;
            Vec<Operator> op;

            int c = 0;
            
            while((int)(index = vars[i].find("sqrt")) != -1) {
                c++;
                bool a = false;
                ops.push_back(vars[i].substr(0, index));

                Surd s = parseSurd(vars[i], index);
                Vec<Fraction> x = s.extractSquares();

                double b = c - 10 * std::floor(std::log10(c));
                std::cout << c << (b == 1 ? "st" : (b == 2 ? "nd" : (b == 3 ? "rd" : "th"))) << " item: ";
                
                if (x[0].numerator != 1 || x[0].denominator != 1) {
                    std::cout << x[0] << ((x[1].numerator == 1 && x[1].denominator == 1) ? "" : " * ");
                    a = true;
                }

                if (x[1].numerator != 1 || x[1].denominator != 1) {
                    if (s.root != 2)
                        std::cout << s.root;

                    std::cout << "sqrt(" << x[1] << ")";
                } else if (!a) {
                    std::cout << x[1];
                }
                std::cout << std::endl;
                surds.push_back(s);
            }

            if (!vars[i].empty()) {
                ops.push_back(vars[i]);
            }

            int j;

            for (j = 0; j < (ops.size() - (vars[i].empty() ? 0 : 1)); j++) {
                String x = ops[j];
                trim(x);
                if (!x.empty()) {
                    if (x[0] == '+') {
                        op.push_back(Add);
                    } else if (x[0] == '-') {
                        op.push_back(Min);
                    } else if (x[0] == '*') {
                        op.push_back(Mul);
                    } else if (x[0] == '/') {
                        op.push_back(Div);
                    } else {
                        std::cout << "Missing operator!" << std::endl;
                        return;
                    }
                    String z = x.substr(1);
                    trim(z);
                    if (!z.empty()) {
                        std::cout << calcAdds(z) << std::endl;
                    }
                }
            }

            if (!vars[i].empty()) {
                String f = ops[ops.size() - 1];

                if (!f.empty()) {
                    trim(f);
                    if (f[0] == '+') {
                        op.push_back(Add);
                    } else if (f[0] == '-') {
                        op.push_back(Min);
                    } else if (f[0] == '*') {
                        op.push_back(Mul);
                    } else if (f[0] == '/') {
                        op.push_back(Div);
                    } else {
                        std::cout << "Missing operator!" << std::endl;
                        return;
                    }

                    String a = f.substr(1);
                    trim(a);

                    if (!a.empty()) {
                        std::cout << calcAdds(a) << std::endl;
                        
                        surds.push_back(calcAdds(a));
                    }
                }
            }

            std::cout << std::endl;
            
            for(int j = 0; j < op.size(); j++) {
                loop_inner:
                if (op[j] == Mul) {
                    surds[j] = surds[j].surd * surds[j + 1].surd;
                    op.erase(op.begin() + j);
                    surds.erase(surds.begin() + j + 1);
                    if (j < op.size()) goto loop_inner;
                } else if (op[j] == Div) {
                    surds[j] = surds[j].surd / surds[j + 1].surd;
                    surds.erase(surds.begin() + j + 1);
                    goto loop_inner;
                } else if (op[j] == Add) {
                    if (j == op.size() - 1 || (op[j + 1] != Mul && op[j + 1] != Div)) {
                        surds[j] = Number::fromExpression(surds[j].surd + surds[j + 1].surd);
                    }
                } else if (op[j] == Min) {
                    if (j == op.size() - 1 || (op[j + 1] != Mul && op[j + 1] != Div)) {
                        surds[j] = Number::fromExpression(surds[j].surd - surds[j + 1].surd);
                    }
                }

                if(surds[j].x == 0) {
                    Vec<Fraction> x = surds[j].surd.extractSquares();
                    bool a = false;

                    if (x[0].numerator != 1 || x[0].denominator != 1) {
                        std::cout << x[0] << ((x[1].numerator == 1 && x[1].denominator == 1) ? "" : " * ");
                        a = true;
                    }

                    if (x[1].numerator != 1 || x[1].denominator != 1) {
                        if (surds[j].surd.root != 2)
                            std::cout << surds[j - 1].surd.root;
                        std::cout << "sqrt(" << x[1] << ")";
                    } else if (!a) {
                        std::cout << x[1];
                    }
                    std::cout << std::endl;
                } else if (surds[j].x == 2) {
                    surds[j].expression->display();
                }
            }
        }
    }
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
static inline void trim(String &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
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

/**
Takes a string and an index at which the word `sqrt` is,
then checks for `n` before it to use as a root, before
passing finding the section of the string in which the
inner expression of the surd is located and parsing it,
before finally erasing that section of the string for
future function calls.
 */
Surd parseSurd(String& temp, size_t index) {
    int root = 2;

    if (index > 0 && !std::isspace(temp[index - 1])) {
        int x = temp.rfind(' ', index);
        String a = temp.substr(x == -1 ? 0 : x, index);

        if (!a.empty() && (int)a.find(')') == -1) {
            root = std::stoi(a);
        }
    }

    if (index == temp.npos) {
        std::cout << "No surds found to simplify!" << std::endl;
        return Surd(Fraction(1.0));
    }

    String current_sqrt;
    String a = temp.substr(index + 4);

    int f;

    if (a[0] == '(') {
        f = a.find(")") - 1;
        current_sqrt = a.substr(1, f);
    } else {
        f = a.find(" ", 1);
        current_sqrt = a.substr(0, f);
    }

    Surd s = Surd(calcAdds(current_sqrt), root);

    temp.erase(0, f + index + 6);
    return s;
}

