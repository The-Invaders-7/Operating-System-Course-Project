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
  for(int i=0;i<10;i++){
    cout<<"Block "<<i+1<<": ";
    for(int j=0;j<40;j++){
      cout<<mainMemory[i][j]<<" ";
    }
    cout<<endl;
  }
  
}

void BufferPrint(){
  for(int i=0;i<40;i++){
    cout<<buffer[i]<<" ";
  }
  cout<<endl;
  
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

void input(){
    bool prog=false;
    bool data=false;
    string text;
    ifstream myFile("input.txt");
    while(getline(myFile,text)){
        cout<<text<<endl;
        
        string str=text.substr(0,4);
        //cout<<str<<endl;
        if(str=="$AMJ"){
            prog=true;
        }
        else if(str=="$DTA"){
            prog=false;
            data=true;
        }
        else if(str=="$END"){
            
        }
       
        
    }
    
    
}


int main()
{
    load();
    input();
    BufferPrint();
    mainMemoryPrint();

    return 0;
}