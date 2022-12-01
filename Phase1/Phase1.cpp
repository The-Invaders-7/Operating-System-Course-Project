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
int SI=0;

ifstream myFile("Input.txt");


void mainMemoryPrint(){
    for(int i=0;i<100;i++){
        if(i%10==0){
            cout<<"Block "<<(i/10+1)<<": ";
            cout<<endl;
        }
        cout<<"Memory Location "<<(i+1)<<": ";
        for(int j=0;j<4;j++){
            cout<<mainMemory[i][j]<<" ";
        }
        cout<<endl;
    }
}
void load(){
    mainMemory.assign(100,vector<char> (4));
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    buffer.assign(40,'#');
    toggle=false;
    mainMemoryPrint();
}
void printPartMemory(int start){
    cout<<"Block "<<start/10<<endl;
    for(int i=start;i<start+10;i++){
        cout<<"Memory Location "<<i<<" ";
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
    memoryLocation=(memoryLocation/10)*10;
    int row=memoryLocation;
    int col=0;
    string text;
    getline(myFile,text);
    cout<<text<<endl;
    std::vector<char> buffer(text.begin(),text.end()); 
    while(dataIndex<40 && dataIndex<buffer.size()){
        mainMemory[row][col++]=buffer[dataIndex++];
        if(col==4){
            col=0;
            row++;
        }
    }
    increment();
    printPartMemory(memoryLocation);
    
}

void writeData(int memoryLocation){
    FILE* file = fopen("Output.txt", "a");
    int row=(memoryLocation/10)*10;
    int col=0;
    for(int i=0;i<40;i++){
        if(mainMemory[row][col]=='\0'){
            int a=6;   
        }
        else{
            fprintf(file, "%c", mainMemory[row][col]);
            cout<<mainMemory[row][col];   
        }
        col++;
        if(col==4){
            col=0;
            row++;
        }   
    }
    cout<<endl;
    fprintf(file, "%c",'\n');
    fclose(file);
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
void bufferPrint(){
    for(int i=0;i<40;i++){
        cout<<buffer[i];
    }
    cout<<endl;
}
void execute(){
    
    mainMemoryPrint();
    while(true){
        cout<<endl;
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
    string text;
    int col=0;
    int row=0;
    while (getline(myFile,text)){
        cout<<"Text:"<<text<<endl;
        string str=text.substr(0,4);
        if(str=="$AMJ"){
            prog=true;
            col=0;
            row=0;
        }
        else if(str=="$DTA"){
            execute();
            prog=false;
            
        }
        else if(str=="$END"){
            FILE* file = fopen("Output.txt", "a");
            fprintf(file, "%c",'\n');
            fprintf(file, "%c",'\n');
            fclose(file);
            load();
        }
        else{
            if(prog){
                int dataIndex=0;
                while(dataIndex<text.size()){
                    mainMemory[row][col++]=text[dataIndex++];
                    if(text[dataIndex-1]=='H'){
                        mainMemory[row][col++]='0';
                        mainMemory[row][col++]='0';
                        mainMemory[row][col++]='0';
                    }
                    if(col==4){
                        col=0;
                        row++;
                    }
                }
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