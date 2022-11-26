#include <iostream>
#include <bits/stdc++.h>
#include<fstream>
using namespace std;

vector<vector<char>> mainMemory(100,vector<char> (4));
vector<vector<char>> drum(500,vector<char> (4));
vector<char> IR(4);
vector<int> IC(2);
vector<char> reg(4);
vector<char> buffer(40);
bool toggle=false;
int SI=0;
int IOI=0;

int timer=0;
int ch1_time=0;
int ch2_time=0;
int ch3_time=0;
bool ch1_waiting=false;
bool ch2_waiting=false;
bool ch3_waiting=false;

int drumBlock=0; 


vector<char> ch1_temp;
vector<char> ch2_temp;
vector<char> ch3_temp;

queue<vector<char> > emptyBuffer;
queue<vector<char> > inputBuffer;
queue<vector<char> > outputBuffer;

struct PCB{   
    int jobId;          
    int TTL;         
    int TLL;   
    int TTC=0;
    int TLC=0;
    int p_track;
    int no_pc;
    int d_track;
    int no_dc;
    int o_track;
    int no_oc;
    bool D=false;
    bool P=false;
};

queue<PCB*> loadQueue;
queue<PCB*> readyQueue;
queue<PCB*> i_oQueue;
queue<PCB*> terminateQueue;

struct PCB* PCB1;

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
void drumPrint(){
    for(int i=0;i<500;i++){
        if(i%10==0){
            cout<<"Block "<<(i/10+1)<<": ";
            cout<<endl;
        }
        cout<<"Memory Location "<<(i+1)<<": ";
        for(int j=0;j<4;j++){
            cout<<drum[i][j]<<" ";
        }
        cout<<endl;
    }
}
void load(){
    mainMemory.assign(100,vector<char> (4));
    drum.assign(500,vector<char> (4));
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    buffer.assign(40,'#');
    toggle=false;
    vector<char> v(40);
    for(int i=0;i<10;i++){
        emptyBuffer.push(v);
    }

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
void PCBPrint(){
     cout<<"PCB"<<endl;
    cout<<"jobId:"<<PCB1->jobId<<endl;   
    cout<<"TTL:"<<PCB1->TTL<<endl;          
    cout<<"TLL:"<<PCB1->TLL<<endl;   
    cout<<"TTC:"<<PCB1->TTC<<endl;   
    cout<<"TLC:"<<PCB1->TLC<<endl;   
    cout<<"P_Track:"<<PCB1->p_track<<endl;   
    cout<<"No-PC:"<<PCB1->no_pc<<endl;          
    cout<<"D_Track:"<<PCB1->d_track<<endl;   
    cout<<"No-DC:"<<PCB1->no_dc<<endl;   
    cout<<"O_Track:"<<PCB1->o_track<<endl;   
    cout<<"No-OC:"<<PCB1->no_oc<<endl;   
    cout<<"D:"<<PCB1->o_track<<endl;   
    cout<<"P:"<<PCB1->no_oc<<endl;   
}

void IR1(){
    string text;
    if(ch1_time==0 && ch1_waiting){
        ch1_waiting=false;
        inputBuffer.push(ch1_temp);
    }
    if(emptyBuffer.size()>0 && ch1_time<1){
        emptyBuffer.pop();
        getline(myFile,text);
        cout<<text<<endl;
        std::vector<char> buffer(text.begin(),text.end()); 
        ch1_temp=buffer;
        ch1_time=5;
        ch1_waiting=true;
    }
    --ch1_time;
}
void IR3(){
    string text;
    if(ch3_time==0 && ch3_waiting){
        int dataIndex=0;
        ch3_waiting=false;
        vector<char> v(40);
        emptyBuffer.push(v);
        if(!(ch3_temp[0]=='$' && ch3_temp[1]=='A' && ch3_temp[2]=='M' && ch3_temp[3]=='J') && !(ch3_temp[0]=='$' && ch3_temp[1]=='D' && ch3_temp[2]=='T' && ch3_temp[3]=='A') !(ch3_temp[0]=='$' && ch3_temp[1]=='E' && ch3_temp[2]=='N' && ch3_temp[3]=='D')){
            int row=0;
            int col=0;
            for(int i=0;i<40 && dataIndex<ch3_temp.size();i++){
                drum[drumBlock*10+row][col++]=ch3_temp[dataIndex++];
                if(col==4){
                    col=0;
                    row++;
                }
            }
            drumBlock++;
        }
        if(ch3_temp[0]=='$' && ch3_temp[1]=='E' && ch3_temp[2]=='N' && ch3_temp[3]=='D'){
            PCBPrint();
            loadQueue.push(PCB1);
            //execution();
            break;

        }
        if(ch3_temp[0]=='$' && ch3_temp[1]=='A' && ch3_temp[2]=='M' && ch3_temp[3]=='J'){
            PCB1=(struct PCB*) malloc(sizeof(struct PCB));
            PCB1->jobId=(ch3_temp[4]-'0')*1000+(ch3_temp[5]-'0')*100+(ch3_temp[6]-'0')*10+(ch3_temp[7]-'0');
            PCB1->TTL=(ch3_temp[8]-'0')*1000+(ch3_temp[9]-'0')*100+(ch3_temp[10]-'0')*10+(ch3_temp[11]-'0');
            PCB1->TLL=(ch3_temp[12]-'0')*1000+(ch3_temp[13]-'0')*100+(ch3_temp[14]-'0')*10+(ch3_temp[15]-'0');
            PCB1->P=true;
            PCB1->p_track=drumBlock;
        }
        if(ch3_temp[0]=='$' && ch3_temp[1]=='D' && ch3_temp[2]=='T' && ch3_temp[3]=='A'){
            PCB1->P=false;
            PCB1->D=true;
            PCB1->d_track=drumBlock;
        }
    }
    if(inputBuffer.size()>0 && ch2_time<1){
        ch3_temp=inputBuffer.front();
        inputBuffer.pop();
        ch3_time=2;
        ch3_waiting=true;
    }    
     --ch3_time;
}

void simulatio(){
    for(int i=1;i<=3;i++){
        if(i==1){
            if(ch1_waiting && ch1_time==0){
                ch1_waiting=false;
                IOI+=1;
            }
        }
        else if(i==2){
            if(ch2_waiting && ch2_time==0){
                ch2_waiting=false;
                IOI+=2;
            }
        }
        else{
            if(ch3_waiting && ch3_time==0){
                ch3_waiting=false;
                IOI+=4;
            }
        }
        if(SI!=0 || PI!=0 || TI!=0 || IOI!=0){
            MOS(0);
        }
    }
}


int main()
{
    load();
    //input();
    input1();

    return 0;
}