#include <iostream>
#include <bits/stdc++.h>
#include<fstream>
using namespace std;

vector<vector<char>> mainMemory(300,vector<char> (4));
vector<char> IR(4);
vector<int> IC(2, 0);
vector<char> reg(4);
vector<char> buffer(40);
bool toggle=false;
int SI=0;
int PI=0;
int TI=0;
int PTR=0;
set<int> st;
int EM;

ifstream myFile("Input.txt");

void mainMemoryPrint(){
    for(int i=0;i<300;i++){
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
    mainMemory.assign(300,vector<char> (4));
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    buffer.assign(40,'#');
    toggle=false;
    PI=0;
    SI=0;
    TI=0;
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

int generateRandom(){
    srand((unsigned)time(0));
    int random = rand()%30;
    while(st.find(random)!=st.end()){
        srand((unsigned)time(0));
        random = rand()%30;
    }
    st.insert(random);
    return random;
}

void allocate(int virtualMemory){
    int newBlock = generateRandom();
    string s = to_string(newBlock);
    cout<<"new block: "<<newBlock<<"\n";
    if(s.length()==2){
        mainMemory[PTR+virtualMemory/10][2] = s[0];
        mainMemory[PTR+virtualMemory/10][3] = s[1];
    }else{
        mainMemory[PTR+virtualMemory/10][2] = '0';
        mainMemory[PTR+virtualMemory/10][3] = s[0];
    }
}

int addressMap(int virtualMemory){
    int PTE = PTR+virtualMemory/10;
    cout<<"PTE: "<<PTE<<"\n";
    int realAddress = ((mainMemory[PTE][2]-'0')*10 + (mainMemory[PTE][3]-'0'))*10 + virtualMemory%10;
    return (realAddress > 299 or realAddress < 0) ? -1:realAddress;
}

bool pageFault(int memoryLocation){
    if((IR[0]=='G' and IR[1]=='D') or (IR[0]=='S' and IR[1]=='R')){
        cout<<"Valid Page Fault\n";
        allocate(memoryLocation);
        printPartMemory(PTR);
        return true;
    }
    cout<<"Invalid Page Fault\n";
    return false;
}

void MOS(int memoryLocation){
    if(PI==3){
        bool valid = pageFault(memoryLocation);
        if(!valid) EM=6;
        PI = 0;
    }
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
    while(true){
        cout<<endl;
        int virtualAddress = IC[0]*10+IC[1];
        int realAddress = addressMap(virtualAddress);
        cout<<"RA: "<<realAddress<<", VA: "<<virtualAddress<<"\n";
        cout<<"IC: "<<IC[0]<<IC[1]<<"\n";
        IR=mainMemory[realAddress];
        cout<<"Instruction: ";
        for(int i=0;i<4;i++){
            cout<<IR[i];
        }
        cout<<endl;
        int memoryLocation=(IR[2]-'0')*10+(IR[3]-'0');
        if(addressMap(memoryLocation)==-1){
            PI = 3;
            MOS(memoryLocation);
            if(EM==6){
                break;
            }
            PI=0;
        }
        cout<<memoryLocation<<" ,";
        memoryLocation = addressMap(memoryLocation);
        cout<<"Address Map: "<<memoryLocation<<"\n";
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
            PTR = generateRandom();
            PTR*=10;
            cout<<"PTR: "<<PTR<<"\n";
        }
        else if(str=="$DTA"){
            mainMemoryPrint();
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
                bool appendBlock=true;
                int currentBlock;
                while(dataIndex<text.size()){
                    if(appendBlock){
                        int newBlock = generateRandom();
                        string s = to_string(newBlock);
                        cout<<"new block: "<<newBlock<<"\n";
                        if(s.length()==2){
                            mainMemory[PTR+row/10][2] = s[0];
                            mainMemory[PTR+row/10][3] = s[1];
                        }else{
                            mainMemory[PTR+row/10][3] = s[0];
                        }
                        currentBlock = newBlock*10;
                        appendBlock = false;
                    }
                    mainMemory[currentBlock+row%10][col++]=text[dataIndex++];
                    if(text[dataIndex-1]=='H'){
                        mainMemory[currentBlock+row%10][col++]='0';
                        mainMemory[currentBlock+row%10][col++]='0';
                        mainMemory[currentBlock+row%10][col++]='0';
                    }
                    if(col==4){
                        col=0;
                        row++;
                        if(row%10==0){
                            appendBlock = true;
                        }
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