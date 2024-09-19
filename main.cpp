#include <cctype>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "main.hh"
#include "fractions.hh"

void simplify(String msg);
void expand();

Surd parseSurd(String &temp, size_t index);

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
            simplify("Enter (a) surd(s) to simplify (use 'sqrt' in place of the surd sign): ");
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
void simplify(String msg) {
    String surd;
    std::cout << msg;
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
                std::cout << c << (b == 1 ? "st" : (b == 2 ? "nd" : (b == 3 ? "rd" : "th"))) << " surd: ";
                
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

            if (ops.size() >= 1) {
                trim(ops[i]);
                String z = ops[i];
                if (!ops[i].empty()) {
                    Fraction f = calcAdds(z.substr(0, z.length() - 1));
                    f = f * f;
                    surds.push_back(Surd(f));
                    char c = z[z.length() - 1];
                    if (c == '+') {
                        op.push_back(Add);
                    } else if (c == '-') {
                        op.push_back(Min);
                    } else if (c == '*') {
                        op.push_back(Mul);
                    } else if (c == '/') {
                        op.push_back(Div);
                    } else {
                        std::cout << "Missing operator!" << std::endl;
                        return;
                    }
                }
            }

            int j;
            
            for (j = 1; j < (ops.size() - (vars[i].empty() ? 0 : 1)); j++) {
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
                        Fraction f = calcAdds(z.substr(0, z.length() - 1));
                        f = f * f;
                        surds.push_back(Surd(f));
                        char c = z[z.length() - 1];
                        if (c == '+') {
                            op.push_back(Add);
                        } else if (c == '-') {
                            op.push_back(Min);
                        } else if (c == '*') {
                            op.push_back(Mul);
                        } else if (c == '/') {
                            op.push_back(Div);
                        } else {
                            std::cout << "Missing operator!" << std::endl;
                            return;
                        }
                    }
                }
            }

            if (!vars[i].empty()) {
                String f = ops[ops.size() - 1];
                trim(f);

                if (!f.empty()) {
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
                        Fraction f = calcAdds(a);
                        f = f * f;
                        surds.push_back(Surd(f));
                    }
                }
            }

            std::cout << std::endl;

            if (surds.size() == 1) {
                if(surds[0].x == 0) {
                    Vec<Fraction> x = surds[0].surd.extractSquares();
                    bool a = false;

                    if (x[0].numerator != 1 || x[0].denominator != 1) {
                        std::cout << x[0] << ((x[1].numerator == 1 && x[1].denominator == 1) ? "" : " * ");
                        a = true;
                    }

                    if (x[1].numerator != 1 || x[1].denominator != 1) {
                        if (surds[0].surd.root != 2)
                            std::cout << surds[0].surd.root;
                        std::cout << "sqrt(" << x[1] << ")";
                    } else if (!a) {
                        std::cout << x[1];
                    }
                    std::cout << std::endl;
                } else if (surds[0].x == 2) {
                    surds[0].expression->display();
                }
            }
            
            for(int j = 0; j < op.size(); j++) {
                loop_inner:
                if (op[j] == Mul) {
                    surds[j] = surds[j].surd * surds[j + 1].surd;
                    op.erase(op.begin() + j);
                    surds.erase(surds.begin() + j + 1);
                    if (j < op.size()) goto loop_inner;
                } else if (op[j] == Div) {
                    surds[j] = surds[j].surd / surds[j + 1].surd;
                    op.erase(op.begin() + j);
                    surds.erase(surds.begin() + j + 1);
                    if (j < op.size()) goto loop_inner;
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

