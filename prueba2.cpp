#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <regex>

using namespace std;

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

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
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

int main()
{
    ifstream inputFile;
    inputFile.open("tabla.csv");
    vector<vector<string>> result;
    unordered_map<int, unordered_map<string, ParseCell*>> slr1;

    while(!inputFile.eof())
    {
        string line;
        getline(inputFile,line);
        std::istringstream ss(line);
        std::string token;
        vector<string> playerInfoVector;
        while(std::getline(ss, token, ',')) {
            playerInfoVector.push_back(token);
            // cout<<token<<" ";
    	}
        // cout<<endl;
        result.push_back(playerInfoVector);
    }

    result.erase(result.begin());
    //result[0] = ESTADO arch drei,etct // vector<string>
    //tratar el U\r
    //INICIO --> result[1] = 0 s32 s18 

    for (auto itrow=result.begin()+1;itrow!=result.end();itrow++){
        auto col = *(itrow);
        auto estado = stoi(col[0]);
        // cout<<col[0]<<endl;
        int countCell=1;
        for (auto itcol =col.begin()+1;itcol!=col.end();itcol++){
                auto celda= *(itcol);
                celda= regex_replace(celda, regex("\r"), "");
                auto action=regex_replace(result[0][countCell],regex("\r"), "");
                // if(celda!=""){
                  
                //     //cout<<estado<<","<<action<<"="<<celda<<endl;
                // }
                if(celda[0]=='s'){
                    //reduceTo, isReduce,estado,stack,boolAcepted
                    auto p = new ParseCell{"", false, estado, stack<string>(), false};
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
        // cout<<endl;
    }
    auto x =slr1[31]["tausend"]->reduce;
   
    while (!x.empty()) { auto top_ = x.top(); cout << top_ << endl; x.pop(); } 
}