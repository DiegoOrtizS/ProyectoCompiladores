#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string> 
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <regex>
#include <set>

using namespace std;

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

struct Transitions;

struct State
{
    vector<pair<string, string>> reglas;
    vector<Transitions> trans;
};

struct AFD
{
    int cont = 0;
    unordered_map<int, State*> states;
};

struct Transitions
{
    unordered_map<int, unordered_map<string, State*>> umapTrans;
};


class ReadGrammar
{
    private:
        ifstream file;
        set<string> nonTerminals;
        set<string> terminals;
        set<string> universe;
        AFD* afd = new AFD();

    public:       
        ReadGrammar() {};
        ReadGrammar(string nombre) 
        {
            readGrammar(nombre);
        };

        void readGrammar(string nombre) 
        {
            vector<pair<string, string>> vec;
            vector<pair<string, string>> vecFinal;
            file.open(nombre);

            if (file.is_open())
            {
                while(!file.eof())
                {
                    string line;
                    getline(file,line);
                    if (line == "X") break;
                    line = line + "|";
                    auto sepFlecha = split(line, " -> ");
                    auto sepOr = split(sepFlecha[1], "|");
                    for (int i = 0; i < sepOr.size() - 1; ++i)
                    {
                        vec.push_back(make_pair(sepFlecha[0], sepOr[i]));
                        nonTerminals.emplace(sepFlecha[0]);

                        auto sepEspacio = split(sepOr[i], " ");
                        for (int j = 0; j < sepEspacio.size(); ++j)
                        {
                            universe.emplace(sepEspacio[j]);
                        }
                    }
                }

                set_difference(universe.begin(), universe.end(), nonTerminals.begin(), nonTerminals.end(), 
                inserter(terminals, terminals.end()));
            }

            for (int i = 0; i < vec.size(); ++i)
            {
                auto sepEspacio = split(vec[i].second, " ");
   
                for (int j = 0; j < sepEspacio.size(); ++j)
                {
                    string aux = "";
                    for (int k = 0; k < j; ++k)
                    {
                        aux = aux + " " + sepEspacio[k];
                    }
                    aux = aux + ". " + sepEspacio[j];
                    for (int k = j + 1; k < sepEspacio.size(); ++k)
                    {
                        aux = aux + " " + sepEspacio[k];
                    }
                    // Puntos intercalados
                    vecFinal.push_back(make_pair(vec[i].first,aux));
                 }
                 // Punto final
                 vecFinal.push_back(make_pair(vec[i].first, vec[i].second + "."));
            }

            // cout << "RULES\n";
            vector<pair<string, string>> startRules;
            for (size_t i = 0; i < vecFinal.size(); i++)
            {
                if (vecFinal[i].second[0] == '.')
                {
                    startRules.push_back(make_pair(vecFinal[i].first, vecFinal[i].second));
                }
                // cout<<vecFinal[i].first<<" -> "<<vecFinal[i].second<<endl;
            }
            afd->states[afd->cont++] = new State{startRules, vector<Transitions>()};

            // cout << "START RULES\n";
            // for (auto& it: afd->states[0]->reglas) 
            // {
            //     cout << it.first << " -> " << it.second << endl;
            // }

            // cout << "NON TERMINALS\n";
            // for (auto it : nonTerminals)
            // {
            //     cout << it << endl;
            // }
            
            // cout << "TERMINALS\n";
            // for (auto it : terminals)
            // {
            //     cout << it << endl;
            // }

            file.close();
        };
};


int main()
{
    ReadGrammar rg("leerGramatica.txt");
}