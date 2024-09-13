#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "fractions.hh"
#include "surds.hh"

static inline void ltrim(std::string& s);
static inline void rtrim(std::string& s);
static inline void trim(std::string& s);

std::vector<std::string> split(std::string s, const std::string &delimiter);

std::ostream &operator<<(std::ostream& op, Fraction fraction);

void simplify();
void expand();

Fraction calcInner(std::string inner);

Fraction calcAdds(std::string str);
Fraction calcMins(std::string str);
Fraction calcMuls(std::string str);
Fraction calcDivs(std::string str);

// TODO: Implement simplification of multi-surd expressions
Surd surdAdds(std::vector<Surd> surds);
Surd surdMins(std::vector<Surd> surds);
Surd surdMuls(std::vector<Surd> surds);
Surd surdDivs(std::vector<Surd> surds);

Surd parseSurd(std::string &temp, size_t index);

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
        std::string temp;
        std::getline(std::cin, temp);
    }

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

void simplify() {
    std::string surd;
    
    std::cout << "Enter (a) surd(s) to simplify (use 'sqrt' in place of the surd sign): ";
    std::getline(std::cin, surd);

    if (surd.find("sqrt") != -1) {
        std::vector<std::string> vars = split(surd, ",");
        std::string temp = surd;
        size_t index;

        for (int i = 0; i < vars.size(); i++) {
            while((int)(index = vars[i].find("sqrt")) != -1) {
                Surd s = parseSurd(vars[i], index);
                std::vector<Fraction> x = s.extractSquares();
                bool a = false;
                if (x[0].numerator != 1 || x[0].denominator != 1) {
                    std::cout << x[0] << ((x[1].numerator == 1 && x[1].denominator == 1) ? "" : " * ");
                    a = true;
                }
                if (x[1].numerator != 1 || x[1].denominator != 1) {
                    if (s.root != 2) std::cout << s.root;
                    std::cout << "sqrt(" << x[1] << ")";
                }
                else if(!a) std::cout << x[1];
                std::cout << std::endl;
            }
        }
    }
}

// delimiter is a reference to reduce allocations
std::vector<std::string> split(std::string s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

Fraction calcInner(std::string inner) {
    return calcAdds(inner);
}

Fraction calcAdds(std::string str) {
    std::vector<Fraction> addVals;
    std::vector<std::string> x = split(str, "+");
    
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

Fraction calcMins(std::string str) {
    std::vector<Fraction> minVals;
    std::vector<std::string> x = split(str, "-");
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

Fraction calcMuls(std::string str) {
    std::vector<Fraction> mulVals;
    std::vector<std::string> x = split(str, "*");
    
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

Fraction calcDivs(std::string str) {
    std::vector<double> divVals;
    std::vector<std::string> x = split(str, "/");
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

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::ostream &operator<<(std::ostream& op, Fraction fraction) {
    if (fraction.denominator != 1) {
        return op << fraction.numerator << "/" << fraction.denominator;
    } else {
        return op << fraction.numerator;
    }
}

Surd parseSurd(std::string& temp, size_t index) {
    int root = 2;
    
    if (index > 0 && !std::isspace(temp[index - 1])) {
        int x = temp.rfind(' ', index);
        std::string a;
        if ((int)x == -1) {
            a = temp.substr(0, index);
        } else {
            a = temp.substr(x, index);
        }
        if (!a.empty() && (int)a.find(')') == -1) {
            root = std::stoi(a);
        }
    }

    if (index == temp.npos) {
        std::cout << "No surds found to simplify!" << std::endl;
        return (Surd)NULL;
    }

    std::string current_sqrt;

    std::string a = temp.substr(index + 4);
    int f;
    if (a[0] == '(') {
        f = a.find(")") - 1;
        current_sqrt = a.substr(1, f);
    } else {
        f = a.find(" ", 1);
        current_sqrt = a.substr(0, f);
    }

    Surd s = Surd(calcInner(current_sqrt), root);

    temp.erase(0, f + index + 6);
    return s;
}

