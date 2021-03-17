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
                            }http://sangucheriaelperuanito.com/ 
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

        bool isValid() { return (errors.size() == 0); };
        vector<Token> getTokens() { return tokens; };
};

// SLR(1)
class Parser
{
    private:
        int state;
        int nextState;
        Lexer lex;
    public:
        Parser() {};
        ~Parser() {};
        /*
        S -> Z1 | Z2 | Z3 | Z4 | Z5 | Z6 | Z7 | Z8 | Z9 | Z11 | Z12 | Z13 | Z14 | U
        Z1 -> ein | zwei | drei | vier | fünf | sechs | sieben | acht | neun
        Z2 -> Z3 zhen
        Z3 -> drei | vier | fünf | sech | sieb | acht | neun
        Z4 -> zwan | vier | fünf | sech | sieb | acht | neun
        Z5 -> hundert
        Z6 -> tausend
        Z7 -> Z4 zig | drei ssig
        Z8 -> U Z7
        Z9 -> Z1 Z5
        Z10 -> Z2 Z5
        Z11 -> Z5 Z1 | Z5 Z2 | Z5 Z7 | Z5 Z8 | Z9 Z1 | Z9 Z2 | Z9 Z7 | Z9 Z8 
        Z12 -> Z10 Z1 | Z10 Z2 | Z10 Z7 | Z10 Z8
        Z13 -> Z1 Z6 | Z2 Z6 | Z7 Z6 | Z8 Z6 | Z9 Z6 | Z11 Z6
        Z14 -> Z6 Z1 | Z6 Z2 | Z6 Z7 | Z6 Z8 | Z6 Z9 | Z6 Z11 | Z13 Z1 | Z13 Z2 | Z13 Z7 | Z13 Z8 | Z13 Z9 | Z13 Z11
        U -> Z1 und
        */
        /*fünf + tausend + zwei + hundert + neun + und+ fünf + zig
        
        S -> Z1 
        S -> Z2
        S -> zwan 
        S -> Z5
        S -> Z6 
        S -> Z7 
        S -> Z8 
        S -> Z9 
        S -> Z11 
        S -> Z12 
        S -> Z13 
        S -> Z14 
        S -> ​U
        Z1 -> ein 
        Z1 -> zwei 
        Z1 -> drei 
        Z1 -> vier 
        Z1 -> fünf 
        Z1 -> sechs
        Z1 -> sieben 
        Z1 -> acht 
        Z1 -> neun
        Z2 -> dreizhen
        Z2 -> Z34 zhen
        Z34 -> vier
        Z34 -> fünf
        Z34 -> sech
        Z34 -> sieb
        Z34 -> acht
        Z34 -> neun
        Z4 -> zwan
        Z4 -> vier
        Z4 -> fünf
        Z4 -> sech
        Z4 -> sieb
        Z4 -> acht
        Z4 -> neun
        Z5 -> hundert
        Z6 -> tausend
        Z7 -> zwanzig
        Z7 -> Z34 zig
        Z7 -> dreissig
        Z8 -> U Z7
        Z9 -> Z1 Z5
        Z10 -> Z2 Z5
        Z11 -> Z5 Z1
        Z11 -> Z5 Z2
        Z11 -> Z5 Z7
        Z11 -> Z5 Z8
        Z11 -> Z9 Z1
        Z11 -> Z9 Z2
        Z11 -> Z9 Z7
        Z11 -> Z9 Z8
        Z12 -> Z10 Z1
        Z12 -> Z10 Z2
        Z12 -> Z10 Z7
        Z12 -> Z10 Z8
        Z13 -> Z1 Z6
        Z13 -> Z2 Z6
        Z13 -> Z7 Z6
        Z13 -> Z8 Z6
        Z13 -> Z9 Z6
        Z13 -> Z11 Z6
        Z14 -> Z6 Z1
        Z14 -> Z6 Z2
        Z14 -> Z6 Z7
        Z14 -> Z6 Z8
        Z14 -> Z6 Z9
        Z14 -> Z6 Z11
        Z14 -> Z13 Z1
        Z14 -> Z13 Z2
        Z14 -> Z13 Z7
        Z14 -> Z13 Z8
        Z14 -> Z13 Z9
        Z14 -> Z13 Z11
        U -> Z1 und


        */
        /*
        // SIN CONFLICTOS (75 ESTADOS)
        S -> Z1 | Z2 | zwan | Z34 | Z5 | Z6 | Z7 | Z8 | Z9 | Z11 | Z12 | Z13 | Z14 | U
        Z1 -> ein | zwei | drei | vier | fünf | sechs | sieben | acht | neun
        Z2 -> dreizhen | Z34 zhen
        Z34 -> vier | fünf | sech | sieb | acht | neun
        Z5 -> hundert
        Z6 -> tausend
        Z7 -> zwanzig | Z34 zig | dreissig
        Z8 -> U Z7
        Z9 -> Z1 Z5
        Z10 -> Z2 Z5
        Z11 -> Z5 Z1 | Z5 Z2 | Z5 Z7 | Z5 Z8 | Z9 Z1 | Z9 Z2 | Z9 Z7 | Z9 Z8 
        Z12 -> Z10 Z1 | Z10 Z2 | Z10 Z7 | Z10 Z8
        Z13 -> Z1 Z6 | Z2 Z6 | Z7 Z6 | Z8 Z6 | Z9 Z6 | Z11 Z6
        Z14 -> Z6 Z1 | Z6 Z2 | Z6 Z7 | Z6 Z8 | Z6 Z9 | Z6 Z11 | Z13 Z1 | Z13 Z2 | Z13 Z7 | Z13 Z8 | Z13 Z9 | Z13 Z11
        U -> Z1 und     
        */
        //fünftausendzweihundertneunundfünfzig NO ES ACEPTADA, porque tiene un tausend que no está al final
        // zweitausendneunhundertsechsundsiebzig
        //zweihundertzweiundzwanzigtausendvierhundertsiebzehn
        /*
        S -> Z13 -> 
        */
};

int main()
{
    //("azweitb")
    // Lexer
    Lexer lex("aazweitb");
    // a
    // z w e i
    // t b und 

    // a zwei tb und
    // - T1 - T2
    return 0;
}