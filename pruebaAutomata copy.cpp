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

struct State
{
    vector<pair<string, string>> reglas;
    bool isTerminal;
    unordered_map<string, State*> transiciones;
    unordered_map<string, State*> llegada;
};

struct AFD
{   
    int cont  = 0;
    int cont2 = 0;
    unordered_map<int, State*> states;
    unordered_map<string,vector<State*>> _allTransition;

    void printAFD(ofstream &out)
    {
        for (auto it2 : states)
        {
            out << "Estado: " << it2.first << endl;
            for (auto it : it2.second->reglas)
            {
                out << it.first << " -> " << it.second << endl;
            }
            out << endl << endl;
        }        
    }
};


void swap(vector<string> &aux, int i)
{
    string temp = aux[i];
    aux[i] = aux[i+1];
    aux[i+1] = temp;    
}


class ReadGrammar
{
    private:
        ifstream file;
        ofstream outputFile;
        set<string> nonTerminals;
        set<string> terminals;
        set<string> universe;
        set<string> rightToPoint;
        set<string> newRightToPoint;
        // set<pair<string, string>> pointsLeftAndRight;

        AFD* afd = new AFD();

        void fillPointRights(vector<pair<string, string>> reglas, set<string> &puntoDer)
        {
            for (int i = 0; i < reglas.size(); ++i)
            {
                vector<string> tempE = split(reglas[i].second, " ");
                for (int j = 0; j < tempE.size(); ++j)
                {
                    if (tempE[j] == "." && j+1 < tempE.size())
                    {
                        puntoDer.emplace(tempE[j+1]);
                    }
                }
            }            
        }

        // void fillPointLeftAndRight(vector<pair<string, string>> reglas)
        // {
        //     for (int i = 0; i < reglas.size(); ++i)
        //     {
        //         // cout << reglas[i].second << endl;
        //         vector<string> tempE = split(reglas[i].second, " ");
        //         for (int j = 0; j < tempE.size(); ++j)
        //         {
        //             if (tempE[j] == ".")
        //             {
        //                 if (j - 1 > 0)
        //                 {
        //                     if (j + 1 < tempE.size())
        //                     {
        //                         pointsLeftAndRight.emplace(make_pair(tempE[j-1], tempE[j+1]));
        //                     }
        //                     else
        //                     {
        //                         pointsLeftAndRight.emplace(make_pair(tempE[j-1], ""));
        //                     }
        //                 }

        //                 else
        //                 {
        //                     pointsLeftAndRight.emplace("", tempE[j+1]);
        //                 }
        //             }
        //         }
        //     }            
        // }

        pair<string, string> movePoint(int state, int rule)
        {
            auto aux = split(afd->states[state]->reglas[rule].second, " ");
            string cadena = "";

            for (int i = 0; i < aux.size() - 1; ++i)
            {
                if (aux[i] == ".")
                {
                    swap(aux, i);
                    break;
                }
            }

            for (int i = 0; i < aux.size(); ++i)
            {
                cadena = cadena + aux[i] + " ";
            }

            return make_pair(afd->states[state]->reglas[rule].first, cadena);
        }

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
            outputFile.open("afd.txt");

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
            // SE CREA EL ESTADO 0
            afd->states[afd->cont++] = new State{startRules, false, unordered_map<string, State*>(), unordered_map<string, State*>() };
            
            // auto nextRule = movePoint(0, 40);

            // cout << nextRule.first << " -> "<< nextRule.second << endl;
            // cout << endl;

            // vector<pair<string, string>> reglas
            // .first antes de la flecha
            // .second después de la flecha (ejemplo . Z1 Z3)
            for (int z = 0; z < afd->cont; ++z)
            {
                unordered_map<int, State*> newStates;

                // cout << afd->cont << endl;
                if (afd->states[z]->isTerminal)
                {
                    continue;
                }

                auto currentState = afd->states[z];
                rightToPoint.clear();
                // cout << "ENTER fillPointRights\n";
                fillPointRights(currentState->reglas, rightToPoint);
                // cout << "FINISH fillPointRights\n";
                // TRANSICIÓN CON LO QUE ESTÁ A LA DERECHA DEL PUNTO
                for (auto it : rightToPoint)
                {
                    bool estadoCreado = true;
                    vector<pair<string, string>> nuevasReglas;
                    for (int i = 0; i < currentState->reglas.size(); ++i)
                    {
                        vector<string> tempE = split(currentState->reglas[i].second, " ");

                        for (int j = 0; j < tempE.size(); ++j)
                        {
                            if (tempE[j] == "." && j + 1 < tempE.size())
                            {
                                if (tempE[j + 1] == it)
                                {
                                    nuevasReglas.push_back(movePoint(z, i));
                                }
                            }
                        }
                    }
                    // SI ES QUE A LA DERECHA DEL PUNTO DESPUÉS DE MOVERLO
                    // HAY UN NO TERMINAL Y ESTE SE ENCUENTRA
                    // PRESENTE EN LAS REGLAS DEL ESTADO POR DONDE VINE
                    // TENEMOS QUE AGREGARLE SUS REGLAS A NUEVAS REGLAS
                    bool _isTerminal = true;  

                    for (int i = 0; i < nuevasReglas.size(); ++i)
                    {
                        if (nuevasReglas[i].second.back() != '.')
                        {
                            _isTerminal = false;
                        }
                    }                    
                    // Llenar right to points después de la movida.
                    State* nuevoEstado = new State{nuevasReglas, _isTerminal, 
                    unordered_map<string, State*>(), unordered_map<string, State*>() };  
                    newRightToPoint.clear();
                    fillPointRights(nuevoEstado->reglas, newRightToPoint);
                    // vector<pair<string, string>> reglasDelEstadoAnterior;

                    for (auto it2 : newRightToPoint)
                    {
                        // Si el nuevo que está a la derecha del punto 
                        // es un no terminal.
                        if (nonTerminals.find(it2) != nonTerminals.end())
                        {
                            // Z3 -> Z8 . Z6
                            // it.first = Z8, it.second = Z6
                            // auto stateFrom = currentState->llegada[it.first];
                            // cout << stateFrom << endl;
                            
                            // if (currentState != nullptr)
                            for (int j = 0; j < currentState->reglas.size(); ++j)
                            {
                                // Encuentro las reglas que estaba buscando
                                // en el estado de donde vine
                                if (currentState->reglas[j].first == it2)
                                {
                                    // reglasDelEstadoAnterior.push_back(currentState->reglas[j]);
                                    // Añado esas reglas al currentState
                                    nuevoEstado->reglas.push_back(currentState->reglas[j]);
                                }
                            }
                        }
                    }


                    // do
                    // {
                    //     contAux = 0;
                    //     newRightToPoint.clear();

                    //     if (firstTry)
                    //     {
                    //         fillPointRights(nuevoEstado->reglas, newRightToPoint);
                    //     }
                    //     else
                    //     {
                    //         // cout << "ENTRA A LLENAR LEFT AND RIGHT\n";
                    //         fillPointRights(reglasDelEstadoAnterior, newRightToPoint);
                    //         // cout << "SALE DE LLENAR LEFT AND RIGHT\n";
                    //     }

                    //     reglasDelEstadoAnterior.clear();
                    //     // Itero sobre los nuevos que están a la derecha del punto
                    //     for (auto it2 : newRightToPoint)
                    //     {
                    //         // Si el nuevo que está a la derecha del punto 
                    //         // es un no terminal.
                    //         if (nonTerminals.find(it2) != nonTerminals.end())
                    //         {
                    //             ++contAux;
                    //             // Z3 -> Z8 . Z6
                    //             // it.first = Z8, it.second = Z6
                    //             // auto stateFrom = currentState->llegada[it.first];
                    //             // cout << stateFrom << endl;
                                
                    //             // if (currentState != nullptr)
                    //             for (int j = 0; j < currentState->reglas.size(); ++j)
                    //             {
                    //                 // Encuentro las reglas que estaba buscando
                    //                 // en el estado de donde vine
                    //                 if (currentState->reglas[j].first == it2)
                    //                 {
                    //                     reglasDelEstadoAnterior.push_back(currentState->reglas[j]);
                    //                     // Añado esas reglas al currentState
                    //                     nuevoEstado->reglas.push_back(currentState->reglas[j]);
                    //                     firstTry = false;
                    //                 }
                    //             }
                    //         }
                    //     }
                        
                    //     // cout << contAux << " " << reglasDelEstadoAnterior.size() << (contAux != 0 && reglasDelEstadoAnterior.size() != 0) << endl;
                    // }while(contAux != 0 && reglasDelEstadoAnterior.size() != 0);


                    // CUANDO YA EXISTE UN ESTADO CON ESAS REGLAS
                    for (auto estadoIterado : afd->_allTransition[it])
                    {
                        if (estadoIterado->reglas.size() == nuevoEstado->reglas.size())
                        {
                            int compareRules = 0;
                            for (int i = 0; i < nuevoEstado->reglas.size(); ++i)
                            {
                                for (int j = 0; j < nuevoEstado->reglas.size(); ++j)
                                {
                                    if (nuevoEstado->reglas[i] == estadoIterado->reglas[i])
                                    {
                                        ++compareRules;
                                        break;
                                    }
                                }
                            }

                            if (compareRules == nuevoEstado->reglas.size())
                            {
                                // Transición hacia el estadoIterado desde e
                                // afd->states[afd->cont]->llegada[it] = afd->states[z];
                                // afd->states[z]->transiciones[it] = afd->states[afd->cont];
                                estadoCreado = false;
                                estadoIterado->llegada[it] = afd->states[z];
                                afd->states[z]->transiciones[it] = estadoIterado;
                            }                            
                        }
                    }

                    if (estadoCreado)
                    {
                        afd->states[afd->cont] = nuevoEstado;
                        newStates[afd->cont] = nuevoEstado;
                        afd->_allTransition[it].push_back(nuevoEstado);

                        // cout << it << endl;
                        afd->states[afd->cont]->llegada[it] = afd->states[z];
                        afd->states[z]->transiciones[it] = afd->states[afd->cont];
                        afd->cont++;
                    }
                }

                // Iterar sobre los nuevos estados
                // for (auto F : newStates)
                // {
                //     currentState = F.second;
                //     pointsLeftAndRight.clear();
                //     // Llenar right to points después de la movida.
                //     fillPointLeftAndRight(currentState);

                //     // Itero sobre los nuevos que están a la derecha del punto
                //     for (auto it : pointsLeftAndRight)
                //     {
                //         // Si el nuevo que está a la derecha del punto 
                //         // es un no terminal.
                //         if (it.second != "")
                //         {
                //             if (nonTerminals.find(it.second) != nonTerminals.end())
                //             {
                //                 // Z3 -> Z8 . Z6
                //                 // it.first = Z8, it.second = Z6
                //                 auto stateFrom = currentState->llegada[it.first];
                //                 // cout << stateFrom << endl;

                //                 if (stateFrom != nullptr)
                //                 {
                //                     for (int j = 0; j < stateFrom->reglas.size(); ++j)
                //                     {
                //                         // Encuentro las reglas que estaba buscando
                //                         // en el estado de donde vine
                //                         if (stateFrom->reglas[j].first == it.second)
                //                         {
                //                             // Añado esas reglas al currentState
                //                             currentState->reglas.push_back(stateFrom->reglas[j]);
                //                         }
                //                     }
                //                 }
                //             }
                //         }
                //     }
                // }
                
            }
            
            afd->printAFD(outputFile);

            /*
            // for (int i = 0; i < afd->states[0]->reglas.size(); ++i)
            // {
            //     afd->state[i].second;
            // }
            // afd->states[afd->cont++]

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
            // }*/

            file.close();
            outputFile.close();
        };
};


int main()
{
    ReadGrammar rg("leerGramatica.txt");
};