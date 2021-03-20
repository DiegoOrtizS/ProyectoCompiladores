#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string> 
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <regex>

#define DEBUG false

using namespace std;

enum class Kind {
	Z1 = 1,
	Z2 = 2,
	//Z3 = 3,
	//Z4 = 4,
    Z34 = 34,
	Z5 = 5,
	Z6 = 6,
    /*
	drei = 7,
	zehn = 8,*/
	zig = 9,
	ssig = 10,
	und = 11,
    ZS = 12,
    zwan = 13
};

ostream& operator<<(ostream& os, const Kind& obj)
{
	os << static_cast<underlying_type<Kind>::type>(obj);
	return os;
}

// only remove first element found consecutively by value of a vector
template <typename T>
void removeByValue(vector<T> &v, T value)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if (v[i] == value)
        {
            v.erase(v.begin() + i);
            return;
        }
    }
}

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos)
		{
			pos = str.length();
		}
		string token = str.substr(prev, pos - prev);
		if (!token.empty())
		{
			tokens.push_back(token);
		}
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());

	return tokens;
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
    string getCadena() { return cadena; }
};

class Lexer
{
    protected:
        vector<Token> tokens;

    private:
        unordered_map<int, char> errors;
        int contErrors = 0;
        //vector<pair<char, int>> errors;

        bool is_Z1(string subStr)
        {
            return (subStr == "ein" || subStr == "zwei" || subStr == "drei"
            || subStr == "sechs" || subStr == "sieben" || is_Z34(subStr));
        }

        bool is_Z2(string subStr)
        {
            // DUDA
            /*return (subStr == "dreizehn" || subStr == "vierzehn" || subStr == "fünfzehn" 
            || subStr == "achtzehn" || subStr == "neunzehn" || subStr == "sechzehn" 
            || subStr == "siebzehn" || subStr == "zehn" || subStr == "elf" || subStr == "zwölf");*/
            return (subStr == "zehn" || subStr == "elf" || subStr == "zwölf");
        }

        /*
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
        }*/


        bool is_Z34(string subStr)
        {
            return (subStr == "vier" || subStr == "fünf" || subStr == "acht" 
            || subStr == "neun");
        }

        bool is_ZS(string subStr)
        {
            return (subStr == "sech" || subStr == "sieb");            
        }

        bool is_Z5(string subStr)
        {
            return (subStr == "hundert");
        }

        bool is_Z6(string subStr)
        {
            return (subStr == "tausend");
        }

        /*
        bool is_drei(string subStr)
        {
            return (subStr == "drei");
        }

        bool is_zehn(string subStr)
        {
            return (subStr == "zehn");
        }*/
        
        bool is_zig(string subStr)
        {
            return (subStr == "zig");
        }

        bool is_ssig(string subStr)
        {
            return (subStr == "ssig");
        }

        bool is_und(string subStr)
        {
            return (subStr == "und");
        }

        bool is_zwan(string subStr)
        {
            return (subStr == "zwan");
        }

    public:
        Lexer() {}
        Lexer(string cadena) 
        {
            tokenizeString(cadena);
        }
        
        ~Lexer() {}
        
        void tokenizeString(string cadena)
        {
            //cadena=funftausendzweihundertneunundfunfzig
            for (int i = 0; i < cadena.size(); ++i)
            {
                string subCadena = "";
                bool z1, z2, z34, zS, z5, z6, zig, ssig, und, zwan;
                
                for (int j = i; j < cadena.size(); ++j)
                {
                    z1 = z2 = z34 = zS = z5 = z6 = zig = ssig = und = zwan = false;
                    vector<Kind> kinds_;
                    subCadena += cadena[j];

                    z1   = is_Z1(subCadena);
                    z2   = is_Z2(subCadena);
                    //z3   = is_Z3(subCadena);
                    //z4   = is_Z4(subCadena);
                    z34  = is_Z34(subCadena);
                    zS   = is_ZS(subCadena);
                    z5   = is_Z5(subCadena);
                    z6   = is_Z6(subCadena);
                    //drei = is_drei(subCadena);
                    //zehn = is_zehn(subCadena);
                    zig  = is_zig(subCadena);
                    ssig = is_ssig(subCadena);
                    und  = is_und(subCadena);
                    zwan = is_zwan(subCadena);
                    
                    if (z1)
                    {
                        kinds_.push_back(Kind::Z1);
                    }
                    if (z2)
                    {
                        kinds_.push_back(Kind::Z2);
                    }
                    if (z34)
                    {    
                        kinds_.push_back(Kind::Z34);
                    }
                    if (zS)
                    {    
                        kinds_.push_back(Kind::ZS);
                    }
                    if (z5)
                    {    
                        kinds_.push_back(Kind::Z5);
                    }
                    if (z6)
                    {    
                        kinds_.push_back(Kind::Z6);
                    }
                    /*
                    if (drei)
                    {    
                        kinds_.push_back(Kind::drei);
                    }
                    if (zehn)
                    {    
                        kinds_.push_back(Kind::zehn);
                    }*/
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
                    if (zwan)
                    {
                        kinds_.push_back(Kind::zwan);
                    }

                    if (z1 || z2 || z34 || zS || z5 || z6 || zig || ssig || und || zwan)
                    {
                        Token t(subCadena, kinds_);
                        tokens.push_back(t);
                        /*
                        if (DEBUG)
                        {
                            cout << subCadena << " con tipos: " << endl;
                            for (int k = 0; k < kinds_.size(); ++k)
                            {
                                cout << kinds_[k] << " ";
                            }
                            cout << endl;
                        }*/
                        i += subCadena.size();
                        subCadena = "";                        
                    }
                }
                if (!(z1 || z2 || z34 || zS || z5 || z6 || zig || ssig || und || zwan))
                {
                    //cout << cadena[i] << endl;
                    //errors.push_back(make_pair(cadena[i], i));
                    if (cadena[i] != '\0')
                    {
                        errors[i] = cadena[i];
                        ++contErrors;
                    }
                }                
            }
            // END OF FOR i
            // DETECTAR SECHS y SIEBEN en caso haya alguna s o en detectados como errores.
            int cont = 0;
            for (int i = 0; i < tokens.size(); ++i)
            {
                string cad = tokens[i].getCadena();
                cont += cad.size();

                //cout << "!" << cont << errors[cont] << endl;
                
                if (cad == "sech" && errors[cont] == 's')
                {
                    vector<Kind> v;
                    v.push_back(Kind::Z1);
                    Token t("sechs", v);
                    tokens[i] = t;
                    errors.erase(cont);
                    --contErrors;
                    ++cont;
                    //removeByValue(errors, make_pair('s', cont));
                }
                
                else if (cad == "sieb" && errors[cont] == 'e' && errors[cont + 1] == 'n')
                {
                    vector<Kind> v;
                    v.push_back(Kind::Z1);
                    Token t("sieben", v);
                    tokens[i] = t;
                    errors.erase(cont);
                    errors.erase(cont + 1);
                    cont += 2;
                    contErrors -= 2;
                    //removeByValue(errors, make_pair('e', cont));
                    //removeByValue(errors, make_pair('n', cont + 1));            
                }
            }
            
            if (DEBUG)
            {
                if (contErrors == 0)
                {
                    cout << "Cadena válida\n";
                }
                else
                {
                    if (contErrors == 1) cout << "Cadena NO válida porque tiene " << contErrors << " error, el cual es \n";
                    else cout << "Cadena NO válida porque tiene " << contErrors << " errores, los cuales son:\n";
                    
                    for (auto it = errors.begin(); it != errors.end(); ++it)
                    {
                        //if (it->second != '\0')
                        {
                            cout << "Error pos " << it->first << ": " << it->second << endl;
                        }
                    }
                }
            }
        }
        bool isValid() { return (contErrors == 0); }
        vector<Token> getTokens() { return tokens; }
        void printTokens()
        {

            for (int i = 0; i < tokens.size(); ++i)
            {
                cout << tokens[i].getCadena() << endl;
                for (int j = 0; j < tokens[i].getKinds().size(); ++j)
                {
                    cout << tokens[i].getKinds()[j] << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
};


struct ParseCell {    
    string reduceTo;
    bool isReduce;
    int estado;
    stack<string> reduce; // Z1-> Z2 Z3  FIFO
    bool isAcepted;

    void printReduce(){
        cout<<"Estado: "<<estado<<endl;
        cout<<"isReduce: "<<isReduce<<endl;
        if(isReduce){
            cout<<reduceTo<<"-->";
            while(!reduce.empty()) {
                cout << reduce.top()<<" ";
                reduce.pop();
            }
            cout<<endl;
        }
    }
};

class ReadFile
{
    private:
        ifstream file;

        bool is_number(const std::string& s)
        {
            std::string::const_iterator it = s.begin();
            while (it != s.end() && std::isdigit(*it)) ++it;
            return !s.empty() && it == s.end();
        }

    public:
        unordered_map<int, unordered_map<string, ParseCell*>> slr1;
       

        ReadFile() {};
        void readTable(string nombre) 
        { 
            file.open(nombre);
            vector<vector<string>> result;
            while(!file.eof())
                {
                    string line;
                    getline(file,line);
                    std::istringstream ss(line);
                    std::string token;
                    vector<string> playerInfoVector;
                    while(std::getline(ss, token, ',')) {
                        playerInfoVector.push_back(token);
                    };
                    result.push_back(playerInfoVector);
                }
            result.erase(result.begin());

            for (auto itrow=result.begin()+1;itrow!=result.end();itrow++){
                auto col = *(itrow);
                auto estado = stoi(col[0]);
                // cout<<col[0]<<endl;
                int countCell=1;
                for (auto itcol =col.begin()+1;itcol!=col.end();itcol++){
                        auto celda= *(itcol);
                        celda= regex_replace(celda, regex("\r"), "");
                        auto action=regex_replace(result[0][countCell],regex("\r"), "");
                        if(celda[0]=='s'){
                            //reduceTo, isReduce,estado,stack,boolAcepted
                            auto goTo = stoi(celda.substr(1,celda.size() - 1));
                            auto p = new ParseCell{"", false, goTo, stack<string>(), false};
                            // //int, strign , parcell
                            slr1[estado][action] = p;
                        }
                        else if (celda[0]=='r'){
                            vector<string> vecAux = split(celda, " ");
                            stack<string> stack_;
                            for (int j = 3; j < vecAux.size() - 1; ++j)
                                {
                                    // cout<<vecAux[j]<<" ";
                                    stack_.push(vecAux[j]);
                                }
                            auto p = new ParseCell{vecAux[1], true, -1, stack_, false};
                            slr1[estado][action] = p;
                        }
                        else if (is_number(celda))
                        {
                            auto p = new ParseCell{"", false,stoi(celda), stack<string>(), false};
                            slr1[estado][action] = p;
                        }
                        else if (celda == "acc")
                        {
                        auto p = new ParseCell{"", false, -1, stack<string>(), true};
                        slr1[estado][action] = p;
                        }

                    countCell++;
                }
            }
        };

        ~ReadFile() {};

};

class Parser : public Lexer
{
private:
	int state;
	int nextState;
    ReadFile rf;
public:

	Parser(string cadena) 
	{
	    tokenizeString(cadena);
	    if (isValid())
	    {
            rf.readTable("tabla.csv");
            cout << "Sin errores léxicos\n";
            processParse();
	    }
	    else
	    {
	        cout << "Con errores léxicos\n";
	    }
	};

	~Parser() {};

    void processParse(){

        stack< pair<string,int>> pila;
        pila.push(make_pair("$",0));
        // //zweitausendneunhundertsechsundsiebzig
        
        stack<string> cadenaPila;
        cadenaPila.push("$");
        for (int i = tokens.size()-1; i >= 0; --i)
        {
            cadenaPila.push(tokens[i].getCadena());
        }

        while (!pila.empty())
        {
            auto top = pila.top();
            auto tmpTerminal=cadenaPila.top();
            
            ParseCell* obj = rf.slr1[top.second][tmpTerminal];    
            cout<<top.second<<" "<<tmpTerminal<<endl;
            //muere
            if(!obj) 
            {
                cout<<"La cadena No es Aceptada"<<endl;
                break;
            }
            else{
                // obj->printReduce();
                if(obj->isAcepted){
                    cout<<"La cadena Es aceptada"<<endl;
                    break;
                }
                else if(!obj->isReduce)
                {
                    //$0zwie0
                    pila.push(make_pair(tmpTerminal,obj->estado));
                    cadenaPila.pop();
                }
                else{
                    if(reducePila(pila,obj)) {
                        cout<<"La cadena No es Aceptada no se puede reducir";
                        break;
                    }
                }    
            }
    
        }
    }

    bool reducePila(stack<pair<string,int>>&pila, ParseCell* obj){

            while (!obj->reduce.empty()){
                auto toDelete = obj->reduce.top();
                if(toDelete == (pila.top().first))
                {
                    pila.pop();
                    obj->reduce.pop();
                }
                else
                {
                    return true;
                }
            }
            auto current = (pila.top()).second;
            auto goTo = rf.slr1[current][obj->reduceTo];
            if(!goTo) return true;
            pila.push(make_pair(obj->reduceTo, goTo->estado));
            return false;
    };
	
    /*
	// FINAL: SIN CON CONFLICTOS (81 ESTADOS)
        S -> Z1 | Z2 | zwan | ZS | Z5 | Z6 | Z7 | Z8 | Z9 | Z11 | Z12 | Z13 | Z14 | U
        Z1 -> ein | zwei | drei | sechs | sieben | Z34
        Z2 -> drei zehn | Z34 zehn | ZS zehn | zehn | elf | zwölf
        Z34 -> vier | fünf | acht | neun
        ZS -> sech | sieb
        Z5 -> hundert
        Z6 -> tausend
        Z7 -> zwan zig | Z34 zig | ZS zig | drei ssig
        Z8 -> U Z7
        Z9 -> Z1 Z5
        Z10 -> Z2 Z5
        Z11 -> Z5 Z1 | Z5 Z2 | Z5 Z7 | Z5 Z8 | Z9 Z1 | Z9 Z2 | Z9 Z7 | Z9 Z8
        Z12 -> Z10 Z1 | Z10 Z2 | Z10 Z7 | Z10 Z8
        Z13 -> Z1 Z6 | Z2 Z6 | Z7 Z6 | Z8 Z6 | Z9 Z6 | Z11 Z6
        Z14 -> Z6 Z1 | Z6 Z2 | Z6 Z7 | Z6 Z8 | Z6 Z9 | Z6 Z11 | Z13 Z1 | Z13 Z2 | Z13 Z7 | Z13 Z8 | Z13 Z9 | Z13 Z11
        U -> Z1 und
    */

	//fünftausendzweihundertneunundfünfzig // ES ACEPTADA LÉXICAMENTE
	//zweitausendneunhundertsechsundsiebzig // ES ACEPTADA LÉXICAMENTE
	//zweihundertzweiundzwanzigtausendvierhundertsiebzehn // ES ACEPTADA LÉXICAMENTE
	/*
	S -> Z13 ->
	*/
};


int main()
{
    // ReadFile rf;
    // rf.readTable("tablaSlr.csv");
    // auto a =rf.slr1[31]["tausend"];
    // if(a) a->printReduce();
    //rf.getUMap()
	//("azweitb")s32
	// Lexer
	//Lexer lex("zweitausendneunhundertsechsundsiebzig");
    Parser par("zweihundertzweiundzwanzigtausendvierhundertsiebzehn");
    //par.processParse();
    //Lexer lex("sechssig");
    //Lexer lex("
    //sechssig");
    //Lexer lex("sechssigw");ovector<strin g tokens >={"zweit","tausend"};r 
    // TESTS CON SECHS y SIEBEN
    // Lexer lex1("sechsssig");
    // lex1.printTokens();
    // Lexer lex2("sechssechs");
    // lex2.printTokens();
    // Lexer lex3("sechssssig");
    // lex3.printTokens();    
    // Lexer lex4("sechssieb");
    // lex4.printTokens(); 
    // Lexer lex5("sechssieben");
    // lex5.printTokens();
    // Lexer lex6("siebensieben");
    // lex6.printTokens();
    // // Cadenas del proyecto
    // Parser parserP1("fünftausendzweihundertneunundfünfzig");
    // parserP1.printTokens();
    // Parser parserP2("zweitausendneunhundertsechsundsiebzig");
    // parserP2.printTokens();
    // Parser parserP3("zweihundertzweiundzwanzigtausendvierhundertsiebzehn");
    // parserP3.printTokens();

    // t b und 

	// a zwei tb und
	// - T1 - T2
	return 0;
}
