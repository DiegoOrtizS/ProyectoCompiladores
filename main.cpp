#include <iostream>
#include <vector>
#include <string>

#define DEBUG true

using namespace std;

enum class Kind {
    Z1   = 1,
    Z2   = 2,
    Z3   = 3,
    Z4   = 4,
    Z5   = 5,
    Z6   = 6,
    drei = 7,
    zehn = 8,
    zig  = 9,
    ssig = 10,
    und  = 11
};

ostream& operator<<(ostream& os, const Kind& obj)
{
   os << static_cast<underlying_type<Kind>::type>(obj);
   return os;
}

class Token
{
    private:
        string cadena;
        vector<Kind> kinds;

    public:
        Token() {};
        Token(string c, vector<Kind> k) 
        {
            cadena = c; kinds = k;
        };
        ~Token() {};

        vector<Kind> getKinds() { return kinds; }
};

class Lexer
{
    private:
        vector<Token> tokens;
        vector<char> errors;

        bool is_Z1(string subStr)
        {
            return (subStr == "ein" || subStr == "zwei" || subStr == "drei"
            || subStr == "vier" || subStr == "fünf" || subStr == "sechs"
            || subStr == "sieben" || subStr == "acht" || subStr == "neun");
        }

        bool is_Z2(string subStr)
        {
            return (subStr == "zehn" || subStr == "elf" || subStr == "zwülf");
        }

        bool is_Z3(string subStr)
        {
            return (subStr == "drei" || subStr == "vier" || subStr == "fünf" 
            || subStr == "sech" || subStr == "sieb" || subStr == "acht" 
            || subStr == "neun");
        }

        bool is_Z4(string subStr)
        {
            return (subStr == "zwan" || subStr == "vier" || subStr == "fünf" 
            || subStr == "sech" || subStr == "sieb" || subStr == "acht" 
            || subStr == "neun");
        }

        bool is_Z5(string subStr)
        {
            return (subStr == "hundert");
        }

        bool is_Z6(string subStr)
        {
            return (subStr == "tausend");
        }

        bool is_drei(string subStr)
        {
            return (subStr == "drei");
        }

        bool is_zhen(string subStr)
        {
            return (subStr == "zhen");
        }
        
        bool is_zig(string subStr){
            return (subStr ==  "zig");
        }

        bool is_ssig(string subStr)
        {
            return (subStr == "ssig");
        }

        bool is_und(string subStr)
        {
            return (subStr == "und");
        }

    public:
        Lexer() {};
        Lexer(string cadena) 
        {
            //cadena=funftausendzweihundertneunundfunfzig
            for (int i = 0; i < cadena.size(); ++i)
            {
                string subCadena = "";
                // char tmp=cadena[i];
                
                bool z1, z2, z3, z4, z5, z6, drei, zehn, zig, ssig, und;
                for (int j = i; j < cadena.size(); ++j)
                {
                    
                    
                    vector<Kind> kinds_;
                    //asdxasdsadasdsa
                
                    subCadena += cadena[j];

                    z1   = is_Z1(subCadena);
                    z2   = is_Z2(subCadena);
                    z3   = is_Z3(subCadena);
                    z4   = is_Z4(subCadena);
                    z5   = is_Z5(subCadena);
                    z6   = is_Z6(subCadena);
                    drei = is_drei(subCadena);
                    zehn = is_zhen(subCadena);
                    zig  = is_zig(subCadena);
                    ssig = is_ssig(subCadena);
                    und  = is_und(subCadena);
                    
                    if (z1)
                    {
                        kinds_.push_back(Kind::Z1);
                    }
                    if (z2)
                    {
                        kinds_.push_back(Kind::Z2);
                    }
                    if (z3)
                    {    
                        kinds_.push_back(Kind::Z3);
                    }
                    if (z4)
                    {    
                        kinds_.push_back(Kind::Z4);
                    }
                    if (z5)
                    {    
                        kinds_.push_back(Kind::Z5);
                    }
                    if (z6)
                    {    
                        kinds_.push_back(Kind::Z6);
                    }
                    if (drei)
                    {    
                        kinds_.push_back(Kind::drei);
                    }
                    if (zehn)
                    {    
                        kinds_.push_back(Kind::zehn);
                    }
                    if (zig)
                    {    
                        kinds_.push_back(Kind::zig);
                    }
                    if (ssig)
                    {    
                        kinds_.push_back(Kind::ssig);
                    }
                    if (und)
                    {    
                        kinds_.push_back(Kind::und);
                    }

                    if (z1 || z2 || z3 || z4 || z5 || z6 || drei || zehn || zig || ssig || und)
                    {
                        Token t(subCadena, kinds_);
                        
                        if (DEBUG)
                        {
                            cout << subCadena << " con tipos: " << endl;
                            for (int i = 0; i < kinds_.size(); ++i)
                            {
                                cout << kinds_[i] << " ";
                            }
                            cout << endl;
                        }
                        i += subCadena.size();
                        subCadena = "";
                    }
                }
                if (!(z1 || z2 || z3 || z4 || z5 || z6 || drei || zehn || zig || ssig || und))
                {
                    errors.push_back(cadena[i]);
                }                
            }

            if (errors.size() == 0)
            {
                cout << "Cadena válida";
            }
            else
            {
                cout << "Cadena NO válida, por los siguientes errores\n";
                for (int i = 0; i < errors.size(); ++i)
                {
                    cout << "Error: " << cadena[i] << endl;
                }
            }
        };

        ~Lexer() {};

        bool isValid()
        {
            return (errors.size() == 0);
        }
};

int main()
{
    //("azweitb")
    Lexer lex("aazweitb");
    // a
    // z w e i
    // t b und 

    // a zwei tb und
    // - T1 - T2
    return 0;
}