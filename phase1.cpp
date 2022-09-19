#include <iostream>
#include <bits/stdc++.h>
#include<fstream>
using namespace std;

vector<vector<char>> mainMemory(100,vector<char> (4));
vector<char> IR(4);
vector<int> IC(2);
vector<char> reg(4);
vector<char> buffer(40);
bool toggle=false;

void bufferPrint(){
    for(int i=0;i<40;i++){
        cout<<buffer[i];
    }
    cout<<endl;
}
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
void execute(){
    
    mainMemoryPrint();
    while(true){
    
    }
    bufferPrint();
}


void load(){
    
    mainMemory.assign(100,vector<char> (4,'#'));
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
    ifstream myFile("Input.txt");
    while (getline(myFile,text)){
        cout<<text<<endl;
        string str=text.substr(0,4);
        if(str=="$AMJ"){
            prog=true;
        }
        else if(str=="$DTA"){
            prog=false;
            data=true;
            
        }
        else if(str=="$END"){
            
        }
        else{
            if(prog){
                int j=0;
                int i=0;
                int a=0;
                while(a<text.size()){
                    mainMemory[i][j++]=text[a++];
                    if(j==4){
                        j=0;
                        i++;
                    }
                }
            }
            else if(data){
                for(int i=0;i<text.size();i++){
                    buffer[i]=text[i];
                }
                execute();
            }
             
        }
    }
}


int main()
{
    load();
    input();

    return 0;
}