#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include<fstream>
using namespace std;

vector<vector<char>> mainMemory(300,vector<char> (4));
vector<char> IR(4);
vector<int> IC(2);
vector<char> reg(4);
vector<char> buffer(40);
vector<int> pages;
bool toggle=false;
int SI=0;
int TI=0;
int PI=0;
int EM=0;
int PTR;
int cards=0;


struct {             
  int TTL;         
  int TLL;   
  int TTC;
  int TLC;
} PCB; 

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
    pages.assign(0,'#');
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    buffer.assign(40,'#');
    toggle=false;
    PI=0;
    SI=3;
    TI=0;
    EM=0;
    cards=0;
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
void terminate(int errorCode){
    if(errorCode==0){
        cout<<"No Error"<<endl;
        EM=0;
    }
    else if(errorCode==1){
        cout<<"Out of Data"<<endl;
        EM=1;
    }
    else if(errorCode==2){
        cout<<"Line Limit Exceeded"<<endl;
        EM=2;
    }
    else if(errorCode==3){
        cout<<"Time Limit Exceeded"<<endl;
        EM=3;
    }
    else if(errorCode==4){
        cout<<"Operation Code Error"<<endl;
        EM=4;
    }
    else if(errorCode==5){
        cout<<"Operand Error"<<endl;
        EM=5;
    }
    else if(errorCode==6){
        cout<<"Invalid Page Fault"<<endl;
        EM=6;
    }
}
void readData(int memoryLocation){
    
    int dataIndex=0;
    memoryLocation=(memoryLocation/10)*10;
    int row=memoryLocation;
    int col=0;
    string text;
    getline(myFile,text);
    cout<<text<<endl;
    if(text.substr(0,4)=="$END"){
        terminate(1);
        return;
    }
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
    cout<<memoryLocation<<endl;
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
    cout<<"SI "<<SI<<" TI "<<TI<<" PI "<<PI<<endl;
    if(SI==1 && TI==0){
        readData(memoryLocation);
    }
    else if(SI==1 && TI==2){
        terminate(3);
    }
    else if(SI==2 && TI==0){
        writeData(memoryLocation);
    }
    else if(SI==2 && TI==2){
        terminate(3);
    }
    else if(SI==3 && TI==0){
        terminate(0);
    }
    else if(SI==3 && TI==2){
        terminate(0);
    }

    if(PI==1 && TI==0){
        terminate(4);
    }
    else if(PI==1 && TI==2){
        terminate(3);
        terminate(4);
    }
    else if(PI==2 && TI==0){
        terminate(5);
    }
    else if(PI==2 && TI==2){
        terminate(3);
        terminate(5);
    }
    else if(SI==3 && TI==2){
        terminate(3);
    }
    else if(SI==3 && TI==2){
        terminate(0);
    }
    SI=3;
    return;
}
void bufferPrint(){
    for(int i=0;i<40;i++){
        cout<<buffer[i];
    }
    cout<<endl;
}
int generateRandom(){
    int random=100;
    bool flag;
    while(true){
        flag=true;
        random=rand();
        random%=30;
        for(int num:pages){
            if(num==random){
                flag=false;
                break;
            }
        }
        if(flag){
            pages.push_back(random);
            return random; 
        }
    }
}
int getRealAddress(int virtualAddress){
    int PTE=PTR+virtualAddress/10;
    if(mainMemory[PTE][0]=='\0'){
        return -1;
    }
    int RA=((mainMemory[PTE][2]-'0')*10+(mainMemory[PTE][3]-'0'))*10+virtualAddress%10;
    return RA;
}
void allocateMemory(int virtualAddress){
    int PTE=PTR+virtualAddress/10;
    mainMemory[PTE][0]='0';
    mainMemory[PTE][1]=virtualAddress/10+'0';
    int page=generateRandom();
    mainMemory[PTE][2]=page/10+'0';
    mainMemory[PTE][3]=page%10+'0';
}
bool checkPageFault(int virtualAddress){
    int realAddress=getRealAddress(virtualAddress);
    if(realAddress==-1){
        //Page Fault
        if((IR[0]=='G' && IR[1]=='D') || (IR[0]=='S' && IR[1]=='R')){
            cout<<"Valid Page Fault"<<endl;
            allocateMemory(virtualAddress);
            return false;
        }
        else{
            cout<<"Invalid Page Fault"<<endl;
            return true;
        }
    }
    return false;
}
void execute(){
    
    mainMemoryPrint();
    while(true){
        IR=mainMemory[getRealAddress(IC[0]*10+IC[1])];
        cout<<(IC[0]*10+IC[1])<<endl;
        cout<<"Instruction: ";
        for(int i=0;i<4;i++){
            cout<<IR[i];
        }
        cout<<endl;
        int memoryLocation=(IR[2]-'0')*10+(IR[3]-'0');
        cout<<"PCB.TTC "<<PCB.TTC<<endl;
        if(IR[0]!='H' && checkPageFault(memoryLocation)){
            PI=3;
            MOS(memoryLocation);
            return;
        }
        if(memoryLocation>99 || memoryLocation<0){
            PI=2;
            MOS(memoryLocation);
            return;
        }
        int RA=getRealAddress(memoryLocation);
        cout<<"Virtual Address: "<<(IC[0]*10+IC[1])<<" Real Address: "<<getRealAddress(IC[0]*10+IC[1])<<" Memory Location: "<<memoryLocation<<" RA: "<<RA<<endl;;
        if(IR[0]=='G' && IR[1]=='D'){
            PCB.TTC+=2;
            SI=1;
            if(PCB.TTC>PCB.TTL){
                TI=2;
                MOS(RA);
                return;
            }
            MOS(RA);
            if(EM!=0){
                return;
            }
        }
        else if(IR[0]=='P' && IR[1]=='D'){
            PCB.TTC+=1;
            PCB.TLC+=1;
            SI=2;
            if(PCB.TTC>PCB.TTL){
                TI=2;
                MOS(RA);
                return;
            }
            if(PCB.TLC>PCB.TLL){
                terminate(2);
                return;
            }
            MOS(RA);
        }
        else if(IR[0]=='L' && IR[1]=='R'){
            PCB.TTC+=1;
            if(PCB.TTC>PCB.TTL){
                TI=2;
                MOS(RA);
                return;
            }
            loadReg(RA);
        }
        else if(IR[0]=='S' && IR[1]=='R'){
            PCB.TTC+=2;
            if(PCB.TTC>PCB.TTL){
                TI=2;
                MOS(RA);
                return;
            }
            storeReg(RA);
        }
        else if(IR[0]=='C' && IR[1]=='R'){
            PCB.TTC+=1;
            if(PCB.TTC>PCB.TTL){
                TI=2;
                MOS(RA);
                return;
            }
            compare(RA);
        }
        else if(IR[0]=='B' && IR[1]=='T'){
            cout<<"Toggle: "<<toggle<<endl;
            PCB.TTC+=1;
            if(PCB.TTC>PCB.TTL){
                TI=2;
                MOS(RA);
                return;
            }
            branch(memoryLocation);
        }
        else if(IR[0]=='H'){
            PCB.TTC+=1;
            SI=3;
            if(PCB.TTC>PCB.TTL){
                TI=2;
                MOS(RA);
                return;
            }
            MOS(RA);
            return;
        }
        else{
            PI=1;
            MOS(RA);
            return;
        }
        cout<<"Instruction: ";
        for(int i=0;i<4;i++){
            cout<<IR[i];
        }
        cout<<endl;
        printPartMemory(PTR);
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
            PTR=generateRandom();
            PTR*=10;
            PCB.TTL=stoi(text.substr(8,4));
            cout<<"TTL: "<<text.substr(8,4)<<endl;
            PCB.TLL=stoi(text.substr(12,4));
            cout<<"TLL: "<<text.substr(12,4)<<endl;
            PCB.TTC=0;
            PCB.TLC=0;
            cout<<PCB.TTC<<" "<<PCB.TLC<<" "<<PCB.TTC<<" "<<PCB.TTL<<endl;
            prog=true;
            col=0;
            row=0;
        }
        else if(str=="$DTA"){
            cout<<"PTR: "<<PTR<<endl;
            execute();
            prog=false;
        }
        else if(str=="$END"){
            cout<<PCB.TTC<<" "<<PCB.TLC<<" "<<PCB.TTL<<" "<<PCB.TLL<<endl;
            FILE* file = fopen("Output.txt", "a");
            fprintf(file, "%c",'\n');
            fprintf(file, "%c",'\n');
            fclose(file);
            load();
        }
        else{
            if(prog){
                int dataIndex=0;
                row=generateRandom();
                cout<<"Row: "<<row<<" Cards: "<<cards<<endl;
                mainMemory[PTR+cards][0]='0';
                mainMemory[PTR+cards][1]=cards+'0';
                mainMemory[PTR+cards][2]=row/10+'0';
                mainMemory[PTR+cards][3]=row%10+'0';
                //cout<<"Logical Address: "<<mainMemory[PTR+cards][0]+" "<<mainMemory[PTR+cards][1]+" Real Address: "<<mainMemory[PTR+cards][2]+" "<<mainMemory[PTR+cards][3]<<endl;
                row*=10;
                ++cards;
                while(dataIndex<text.size()){
                    mainMemory[row][col++]=text[dataIndex++];
                    if(text[dataIndex-1]=='H' && col==1){
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