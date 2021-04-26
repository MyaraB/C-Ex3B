#include <iostream>
#include <fstream>
#include <cassert>
#include <stdio.h>
#include <stdbool.h>
#include <map>
#include "NumberWithUnits.hpp"
using namespace std;
namespace ariel
{

    static map<string, map<string, double>> units;

    NumberWithUnits::NumberWithUnits(double a, const string &unit) {

        if( units.find(unit)==units.end()){
            throw invalid_argument("The unit" +unit+ "is not registered in the database");
        }
        this->unit = unit;
        this->number = a;
    }


    void UnitSync(const string& base, const string& dst)
    {
        for( const auto &conversion1 : units[base]){
            for(const auto &conversion2 : units[dst]){
                double ratio = units[conversion1.first][base] * units[base][dst] * units[dst][conversion2.first];
                units[conversion1.first][conversion2.first]=ratio;
                units[conversion2.first][conversion1.first] = 1/ratio;
            }
        }
    }

    void NumberWithUnits::read_units(ifstream &f)
    {
        string firstU;
        string secondU;
        string blank;
        double v1 = 0;
        double v2 = 0;

        while(f >> v1 >> firstU >> blank >> v2 >> secondU){
            units[firstU][secondU] = v2;
            units[secondU][firstU] = 1/v2;
            units[firstU][firstU] = 1;
            units[secondU][secondU] = 1;
            
            UnitSync(firstU,secondU);
            

        }
    }



    double baseConver(double value, const string& base, const string& dst)
    {   
        if(units[base].find(dst) == units[base].end()){
            throw invalid_argument("No valid conversion between " +base+ " and " +dst+ " sorry");
        }
        
        return value * units[base][dst];
    }





    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &a) const { 

        double conver = baseConver(a.number, a.unit, this->unit);
        return NumberWithUnits(this->number + conver, this->unit);

        }


    NumberWithUnits NumberWithUnits::operator+() const { 

        return NumberWithUnits{this->number,this->unit};
    }


    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &a) const { 

        double conver = baseConver(a.number, a.unit, this->unit);
        return NumberWithUnits(this->number - conver, this-> unit);
    }


    NumberWithUnits NumberWithUnits::operator-() const { 

        double inverse = -(this->number);
        return NumberWithUnits{inverse,this->unit};
     }


    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &a) { 
        double conver = baseConver(a.number, a.unit, this->unit);
        this->number += conver;
        return *this;
     }


    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &a) { 
        double conver = baseConver(a.number, a.unit, this->unit);
        this->number -= conver;
        return *this;
     }


    NumberWithUnits &NumberWithUnits::operator++() { 
        this->number+=1;
        return *this;
     }

    NumberWithUnits NumberWithUnits::operator++(int a) { 
        
        NumberWithUnits before = *this;
        this->number+=1;
        return before;
        }


    NumberWithUnits &NumberWithUnits::operator--() { 

        this->number-=1;
        return *this;

     }
    NumberWithUnits NumberWithUnits::operator--(int a) { 
        
        NumberWithUnits before = *this;
        this->number-=1;
        return before;

     }


    NumberWithUnits NumberWithUnits::operator*(double a) const { 

        double calc = this->number * a; 
        return NumberWithUnits{calc, this->unit};

     }


    NumberWithUnits operator*(double a, const NumberWithUnits &b)
    {
        double calc = a*b.number;
        return NumberWithUnits{calc, b.unit};
    }

    ostream &operator<<(ostream &stream, const NumberWithUnits &a) { 

         return stream << a.number << "[" << a.unit << "]";
     }


    istream &operator>>(istream &stream, NumberWithUnits &a) {

        double num = 0;
        string word;

        stream >> num;

        char c = ' ';
        while (stream >> c)
        {
            if (c != '[' && c != ']')
            {
                word += c;
            }
            else if (c == ']')
            {
                break;
            }
        }

        if (units.count(word) == 0)
        {
            throw invalid_argument("There is no unit names " +word+ " sorry :(");
        }
        a.unit = word;
        a.number = num;
        return stream;

     }

    double absolute(double number)
    {
        if(number > 0)
        {
            return number;
        }
        
        return -number;
        
    }
    int comparator (const NumberWithUnits& a, const NumberWithUnits& b){
        const double TOLLERANCE = 0.001;

        double bval = a.number - baseConver(b.number, b.unit, a.unit);
        
        if(absolute(bval) < TOLLERANCE){
            return 0;
        }
        
        if (bval < -TOLLERANCE){
            return -1;
        }
        
        return 1;


    }

    bool NumberWithUnits::operator>(const NumberWithUnits &a) const { 
        return (comparator(*this,a) == 1);
     }

    bool NumberWithUnits::operator>=(const NumberWithUnits &a) const {
       
        return (comparator(*this,a) == 1 || comparator(*this,a) ==0);
    
  }

    bool NumberWithUnits::operator<(const NumberWithUnits &a) const { 
        return (comparator(*this,a) == -1);
   }

    bool NumberWithUnits::operator<=(const NumberWithUnits &a) const { 
        return (comparator(*this,a) == -1 || comparator(*this,a) ==0);
   }

    bool NumberWithUnits::operator==(const NumberWithUnits &a) const { 

        return (comparator(*this,a)==0);

    }

    bool NumberWithUnits::operator!=(const NumberWithUnits &a) const { 
        return (comparator(*this,a)!=0);
        
    }

}