#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string> 
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <regex>
#include <iomanip>

#define DEBUG true

using namespace std;

template <typename T>
void printStack(stack<T> s)
{
    while (!s.empty())
    {
        T x = s.top();
        cout << x << " ";
        s.pop();
    }
}

void printStack(stack<pair<string, int>> s)
{
    while (!s.empty())
    {
        pair<string, int> x = s.top();
        cout << x.first << x.second << " ";
        s.pop();
    }
}

template <typename T>
void reverseAndPrintStack(stack<T> s)
{
    stack<T> revStack;

    while (!s.empty())
    {
        T x = s.top();
        revStack.push(x);
        s.pop();
    }
    printStack(revStack);
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

class Lexer
{
    protected:
        vector<string> tokens;

    private:
        unordered_map<int, char> errors;
        vector<int> errorsToDelete;
        map<int, string> aux;
        int contErrors = 0;

    public:
        Lexer() {}
        Lexer(string cadena) 
        {
            tokenizeString(cadena);
        }
        
        ~Lexer() {}
        
        void tokenizeString(string cadena)
        {
            for (int i = 0; i < cadena.size(); ++i)
            {
                string subCadena = "";
                bool Ac, C, N, Sv, Sh;

                for (int j = i; j < cadena.size(); ++j)
                {
                    Ac = C = N = Sv = Sh = false;
                    subCadena += cadena[j];

                    Ac = subCadena == "Ac";
                    C = subCadena == "C";
                    N = subCadena == "N";
                    Sv = subCadena == "Sv";
                    Sh = subCadena == "Sh";

                    if (Ac || C || N || Sh || Sv)
                    {
                        aux[i] = subCadena;
                        i += subCadena.size();
                        subCadena = "";                        
                    }
                }
                if (!(Ac || C || N || Sh || Sv))
                {
                    if (cadena[i] != '\0')
                    {
                        errors[i] = cadena[i];
                        ++contErrors;
                    }
                }                
            }
            // END OF FOR i
            // DETECTAR A sobrantes y también los números que van seguidos de Sh, Sv, Ac, N y C
            for (auto it : errors)
            {
                if (it.second == 'A')
                {
                  aux[it.first] = it.second;
                  --contErrors;
                  errorsToDelete.push_back(it.first);
                }

                else if (it.second == '1' || it.second == '2'
                || it.second == '3' || it.second == '4' 
                || it.second == '5')
                {
                  // Sh, Sv y Ac
                  if (aux.find(it.first - 2) != aux.end())
                  {
                    string token_ = aux[it.first-2];
                    if ((token_ == "Sh" || token_ == "Sv") || (token_ == "Ac" && it.second != '5'))
                    {
                      aux[it.first - 2] = aux[it.first - 2] + it.second;
                      --contErrors;
                      errorsToDelete.push_back(it.first);
                    }
                  }

                  // N y C
                  else if (aux.find(it.first - 1) != aux.end())
                  {
                    string token_ = aux[it.first-1];
                    if (token_ == "N" || (token_ == "C" && it.second != '5'))
                    {
                      aux[it.first - 1] = aux[it.first - 1] + it.second;
                      --contErrors;
                      errorsToDelete.push_back(it.first);
                    }
                  }
                }
            }

            for (auto it : errorsToDelete)
            {
                errors.erase(it);
            }

            for (auto it : aux)
            {
              tokens.push_back(it.second);
              cout << it.first << " " << it.second << endl;
            }

            if (DEBUG)
            {
                if (contErrors != 0)
                {
                    if (contErrors == 1) cout << "Cadena " << cadena <<  " NO es válida porque tiene " << contErrors << " error léxico, el cual es \n";
                    else cout << "Cadena " << cadena <<  " NO es válida porque tiene " << contErrors << " errores léxicos, los cuales son:\n";
                    
                    for (auto it = errors.begin(); it != errors.end(); ++it)
                    {
                        cout << "Error pos " << it->first << ": " << it->second << endl;
                    }
                }
            }
        }

        bool isValid() { return (contErrors == 0); }

        vector<string> getTokens() { return tokens; }

        void printTokens()
        {
            for (int i = 0; i < tokens.size(); ++i)
            {
                cout << tokens[i] << " ";
            }
            cout << endl;
        }
};


struct ParseCell {    
    string reduceTo;
    bool isReduce;
    int estado;
    stack<string> reduce; 
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
                    }

                    result.push_back(playerInfoVector);
                }
            result.erase(result.begin());

            for (auto itrow=result.begin()+1;itrow!=result.end();itrow++){
                auto col = *(itrow);
                auto estado = stoi(col[0]);
                int countCell=1;
                for (auto itcol =col.begin()+1;itcol!=col.end();itcol++){
                        auto celda= *(itcol);
                        celda= regex_replace(celda, regex("\r"), "");
                        auto action=regex_replace(result[0][countCell],regex("\r"), "");
                        if(celda[0]=='s'){
                            auto goTo = stoi(celda.substr(1,celda.size() - 1));
                            auto p = new ParseCell{"", false, goTo, stack<string>(), false};
                            slr1[estado][action] = p;
                        }
                        else if (celda[0]=='r'){
                            vector<string> vecAux = split(celda, " ");
                            stack<string> stack_;
                            for (int j = 3; j < vecAux.size() - 1; ++j)
                                {
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
    ReadFile rf;
public:

	Parser(string cadena, string nombre) 
	{
	    tokenizeString(cadena);
	    if (isValid())
	    {
            rf.readTable(nombre);
            cout << "Sin errores léxicos\n";
            processParse();
	    }
	    else
	    {
	        cout << "Con errores léxicos\n";
	    }
	};

	~Parser() {};

    void processParse()
    {
        cout << "Pila de análisis sintáctico" << setw(50) << setfill(' ') << "Entrada" << setw(50) << setfill(' ') << "Acción\n";

        stack<pair<string, int>> pila;
        pila.push(make_pair("$",0));
        
        stack<string> cadenaPila;
        cadenaPila.push("$");
        for (int i = tokens.size()-1; i >= 0; --i)
        {
            cadenaPila.push(tokens[i]);
        }

        while (!pila.empty())
        {
            auto top = pila.top();
            auto tmpTerminal = cadenaPila.top();
            
            ParseCell* obj = rf.slr1[top.second][tmpTerminal];
            reverseAndPrintStack(pila);
            cout << setw(50) << setfill(' ');
            printStack(cadenaPila);
            cout << setw(50) << setfill(' ');

            if(!obj) 
            {
                cout << "No existe regla " << top.second << " con " << tmpTerminal << ", por lo que"
                << "la cadena no es aceptada" << endl;
                break;
            }
            else
            {
                if(obj->isAcepted)
                {
                    cout << "acc";
                    //cout<<"La cadena es aceptada"<<endl;
                    break;
                }
                else if(!obj->isReduce)
                {
                    cout << "s" << obj->estado;
                    pila.push(make_pair(tmpTerminal,obj->estado));
                    cadenaPila.pop();
                }
                else
                {
                    if (reducePila(pila, obj)) 
                    {
                        cout<<"La cadena no es aceptada, no se puede realizar esa reducción";
                        break;
                    }
                }    
            }
            cout << endl;    
        }
    }

    bool reducePila(stack<pair<string,int>> &pila, ParseCell* obj){
            cout << "r( " << obj->reduceTo << " -> ";
            stack<string> pilaAux;

            while (!obj->reduce.empty()){
                auto toDelete = obj->reduce.top();
                if(toDelete == (pila.top().first))
                {
                    pila.pop();
                    obj->reduce.pop();
                    pilaAux.push(toDelete);
                }
                else
                {
                    return true;
                }
            }
            printStack(pilaAux);
            cout << " )";
            auto current = (pila.top()).second;
            auto goTo = rf.slr1[current][obj->reduceTo];
            if(!goTo) return true;
            pila.push(make_pair(obj->reduceTo, goTo->estado));
            return false;
    };
};


int main()
{
    // Parser par1("ASh1Sv3N5Ac4C4A", "TablaSlrTextilOp.csv");
    // Parser par2("ASh1Sv3N5Ac2C4A", "TablaSlrTextilOp.csv");
    // Parser par3("A4Sh1Sv3N5Ac4C5A", "TablaSlrTextilOp.csv");
    Parser par4("ASh3Sv2N1Ac1C1C2C4A", "TablaSlrTextilOp.csv");

    return 0;
}