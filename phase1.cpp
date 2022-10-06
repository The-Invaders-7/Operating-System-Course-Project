#include <iostream>
#include <bits/stdc++.h>
#include<fstream>
using namespace std;

vector<vector<char>> mainMemory(100,vector<char> (4));
vector<char> IR(4);
vector<int> IC(2);
vector<char> reg(4);
vector<string> buffer(10);
bool toggle=false;
int bufferData=0;
int SI=0;

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
    mainMemory.assign(100,vector<char> (4,'#'));
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    buffer.assign(10,"#");
    toggle=false;
    mainMemoryPrint();
}
void printPartMemory(int start,int end){
    for(int i=start;i<=end;i++){
        cout<<"Memory Location "<<(i+1)<<": ";
        for(int j=0;j<4;j++){
            cout<<mainMemory[i][j]<<" ";
        }
        cout<<endl;
    }
}
void increment(){
    if(IC[1]<9){
        IC[1]=IC[1]+1;
    }
    else if(IC[0]<9){
        IC[0]=IC[0]+1;
        IC[1]=0;
    }
    else{
        cout<<"Memory Overflow";
    }
    cout<<"Instruction Counter: "<<IC[0]<<" "<<IC[1]<<endl;
}
void readData(int memoryLocation){
    
    int dataIndex=0;
    int row=memoryLocation;
    int col=0;
    string data=buffer[bufferData++];
    while(dataIndex<40 && dataIndex<data.size()){
        mainMemory[row][col++]=data[dataIndex++];
        if(col==4){
            col=0;
            row++;
        }
    }
    increment();
    printPartMemory(memoryLocation,row);
    
}

void writeData(int memoryLocation){
    freopen("Output.txt","w",stdout);
    int row=memoryLocation;
    int col=0;

    while(mainMemory[row][col]!='#'){
        cout<<(mainMemory[row][col])<<" ";
        col++;
        if(col==4){
            col=0;
            row++;
        }
    }
    
    fclose(stdout);
    increment();
    
}

void loadReg(int memoryLocation){
    for(int col=0;col<4;col++){         
        reg[col]=mainMemory[memoryLocation][col];
    }
    
    for(int col=0;col<4;col++){
        cout<<reg[col]<<" ";
    }
    cout<<endl;
    increment();
}
void storeReg(int memoryLocation){
    for(int col=0;col<4;col++){         
        mainMemory[memoryLocation][col]=reg[col];
    }
    
    for(int col=0;col<4;col++){
        cout<<mainMemory[memoryLocation][col]<<" ";
    }
    cout<<endl;
    increment();
}
void compare(int memoryLocation){
    increment();
    for(int col=0;col<4;col++){
        if(reg[col]!=mainMemory[memoryLocation][col]){
            return;
        }
    }
    toggle=true;
}
void branch(int memoryLocation){
    if(toggle){
        IC[0]=memoryLocation/10;
        IC[1]=memoryLocation%10;
    }
    else{
        increment();   
    }
}
void halt(){
    load();
}
void bufferPrint(){
    for(int i=0;i<40;i++){
        cout<<buffer[i];
    }
    cout<<endl;
}

void MOS(int memoryLocation){
    if(SI==1){
        readData(memoryLocation);
    }
    else if(SI==2){
        writeData(memoryLocation);
    }
    else if(SI==3){
        halt();
    }

    SI=0;
    return;
}

void execute(){
    
    mainMemoryPrint();
    while(true){
        IR=mainMemory[IC[0]*10+IC[1]];
        cout<<"Instruction: ";
        for(int i=0;i<4;i++){
            cout<<IR[i];
        }
        cout<<endl;
        int memoryLocation=(IR[2]-'0')*10+(IR[3]-'0');
        if(IR[0]=='G' && IR[1]=='D'){
            SI=1;
            MOS(memoryLocation);
        }
        else if(IR[0]=='P' && IR[1]=='D'){
            SI=2;
            MOS(memoryLocation);
        }
        else if(IR[0]=='L' && IR[1]=='R'){
            loadReg(memoryLocation);
        }
        else if(IR[0]=='S' && IR[1]=='R'){
            storeReg(memoryLocation);
        }
        else if(IR[0]=='C' && IR[1]=='R'){
            compare(memoryLocation);
        }
        else if(IR[0]=='B' && IR[1]=='T'){
            branch(memoryLocation);
        }
        else{
            SI=3;
            MOS(memoryLocation);
            break;
        }
        
    }
    //bufferPrint();
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
            bufferData=0;
            execute();
        }
        else{
            if(prog){
                int col=0;
                int row=0;
                int dataIndex=0;
                while(dataIndex<text.size()){
                    mainMemory[row][col++]=text[dataIndex++];
                    if(text[dataIndex-1]=='H'){
                        col+=3;
                    }
                    if(col==4){
                        col=0;
                        row++;
                    }
                }
            }
            else if(data){
                buffer[bufferData++]=text;
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