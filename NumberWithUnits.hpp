#include <string>
#include <map>
using namespace std;

namespace ariel
{
    class NumberWithUnits
    {
    string unit;
    double number;

    public:
        NumberWithUnits(double, const string&);
        static void read_units(ifstream&);

        friend int comparator (const NumberWithUnits& a, const NumberWithUnits& b);

        bool operator>(const NumberWithUnits& a) const;
        bool operator>=(const NumberWithUnits& a) const;
        bool operator<(const NumberWithUnits& a) const;
        bool operator<=(const NumberWithUnits& a) const;
        bool operator!=(const NumberWithUnits& a) const;
        bool operator==(const NumberWithUnits& a) const;
        
        friend ostream &operator<<(ostream& stream, const NumberWithUnits& a);
        friend istream &operator>>(istream& stream, NumberWithUnits& a);
        
        NumberWithUnits operator+() const;
        NumberWithUnits operator+(const NumberWithUnits&) const; 
        NumberWithUnits &operator+=(const NumberWithUnits&); 
        NumberWithUnits operator-() const; 
        NumberWithUnits operator-(const NumberWithUnits&) const; 
        NumberWithUnits &operator-=(const NumberWithUnits&); 

        NumberWithUnits &operator++();
        NumberWithUnits operator++(int); 
        NumberWithUnits &operator--();
        NumberWithUnits operator--(int); 
        
        friend NumberWithUnits operator*(double a, const NumberWithUnits& b);
        NumberWithUnits operator*(double a) const;
    };
}