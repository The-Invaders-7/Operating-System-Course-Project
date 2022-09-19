#include <iostream>
#include <bits/stdc++.h>
#include<fstream>
using namespace std;

vector<vector<char>> mainMemory(10,vector<char> (40));
vector<char> IR(4);
vector<int> IC(2);
vector<char> reg(4);
vector<char> buffer(40);
bool toggle=false;
string contain;


void mainMemoryPrint(){
    for(int i=0;i<100;i++){
        if(i%10==0){
            cout<<"Block "<<(i/10+1)<<": "<<endl;
        }
        cout<<"Memory Location "<<(i+1)<<": ";
        for(int j=0;j<4;j++){
            cout<<mainMemory[i][j]<<" ";
        }
        cout<<endl;
    }
}

void load(){
    
    mainMemory.assign(10,vector<char> (40,'#'));
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    buffer.assign(40,'#');
    toggle=false;
    mainMemoryPrint();

    
}


int main()
{
    load();
    input();

    return 0;
}