#ifndef _FRACTIONS_HH_ 
    #define _FRACTIONS_HH_
    #include <cmath>
    #include <string>
    #include <vector>

    using String = std::string;
    template <class T> using Vec = std::vector<T>;
    
    class Fraction {
        public:
            int numerator;
            int denominator;

            explicit Fraction(int numerator, int denominator);

            Fraction(double x);

            Fraction simplify();

            Fraction operator+(Fraction x);
            Fraction operator-(Fraction x);
            Fraction operator*(Fraction x);
            Fraction operator/(Fraction x);

            Fraction operator+(double x);
    };

    inline void trim(String& s);
    Vec<String> split(String s, const String &delimiter);

    std::ostream &operator<<(std::ostream& op, Fraction fraction);

    Fraction calcAdds(String str);
    Fraction calcMins(String str);
    Fraction calcMuls(String str);
    Fraction calcDivs(String str);
#endif

