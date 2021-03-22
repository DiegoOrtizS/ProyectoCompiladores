#include <unordered_map>
#include <iostream>
#include <string>
using namespace std;

struct AFD{

unordered_map<int,State *> stados;

};

struct State{

 vector<pair<string,string>> reglas;

};

int main (){

    unordered_map<int,unordered_map<string,*State>> _transitions;

    State * zero= new State{vecto<string,string> reglas}

    AFD * afd = new ADF();

    adf.stados[0]=zero;

    int nextStAteToCreate=1;



    //mientras no existan valores en el unordermpa que yo ya recorrio no dejes de recorrer.

    //mientras voy recoorriendo el unordered_map voy a ir agregadno estados entiendes?

    for (auto& [id,itstate]: afd.estado){

      //revisar que el siguiente me de el final /// prueben en c++

        auto state = state

        auto idState= id

        for( auto& it: state->reglas){

        auto vec= *(it);
        //necesito el valor despues del punto.

        termTransition= ((vec.second).split(".")[1]).split(" ")[0];

        //S-> a b.z1 z2 z3
        // z1 .z2 z3

        //busco un estado que desde donde estoy transiciona.

        auto nextState=_transitions[idState,termTransition];
        //  0 , S == State S->S.
        //1, S == State S->S.
        //  1, S , S.

        //
//         auto newString=movePunto(vec.second);
//         if(!nextState){
                          
//                 createState(nextStAteToCreate++,vec.firts,newString,idState,termTransition);
//          }
//             else{
//                  if(!existeRule(newString,nextState)){
//                     createState(nextStAteToCreate++,vec.firts,newString,idState,termTransition);
//                  }

//              }

//        }
    }
}


void createState(int idState,string parent,string rule,int fronState,string termTransition){

    auto aux = make_pair(parent,rule);
    vector<pair<string,string>> newVect {aux}; 
    auto *newState= new State(newVect);
    _transitions[fronState][termTransition]=newState;
    adf.stados[idState]=newState;     
}

bool existeRule(string newRule){

    for()
    

}



































