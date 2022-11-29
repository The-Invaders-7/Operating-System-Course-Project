#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include<fstream>
using namespace std;

vector<vector<char>> mainMemory(300,vector<char> (4));
vector<vector<char>> drum(500,vector<char> (4));
vector<char> IR(4);
vector<int> IC(2);
vector<char> reg(4);
vector<int> pages;
bool toggle=false;
int SI=0;
int TI=0;
int PI=0;
int EM=0;
int IOI=0;
int PTR;
int cards=0;

string error="";


int timer=0;
int ch1_time=0;
int ch2_time=0;
int ch3_time=0;
bool ch1_waiting=false;
bool ch2_waiting=false;
bool ch3_waiting=false;
bool ch2_wait=false;
bool ch3_wait=false;
bool ch3_waitIS=false;

bool ch1_stop=false;
bool PTRInitialize=false;

bool inputSpooling=false;
bool outputSpooling=false;
bool loadMemory=false;
bool i_oloading=false;
bool processing=false;
bool start=true;

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
    int no_pc=0;
    int d_track;
    int no_dc=0;
    int o_track;
    int no_oc=0;
    bool D=false;
    bool P=false;
};

queue<PCB*> loadQueue;
queue<PCB*> readyQueue;
queue<PCB*> i_oQueue;
queue<PCB*> terminateQueue;

struct PCB* PCB1;
struct PCB* PCB2;

ifstream myFile("Input.txt");
ofstream MyFile("TimeOutput.txt");
ofstream OutputFile("MemoryOutput.txt");

void IR1();
void IR2();
void IR3();
void simulation();


void mainMemoryPrint(){
    OutputFile<<"Main Memory\n";
    for(int i=0;i<300;i++){
        if(i%10==0){
            OutputFile<<"Block "<<(i/10+1)<<"\n";
        }
        OutputFile<<"Memory Location "<<(i+1)<<": ";
        for(int j=0;j<4;j++){
            OutputFile<<mainMemory[i][j]<<" ";
        }
        OutputFile<<"\n";
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
void mainMemoryLoad(){
    mainMemory.assign(300,vector<char> (4));
    pages.assign(0,'#');
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    toggle=false;
    PI=0;
    SI=3;
    TI=0;
    EM=0;
    cards=0;
}
void load(){
    mainMemory.assign(300,vector<char> (4));
    drum.assign(500,vector<char> (4));
    pages.assign(0,'#');
    IR.assign(4,'#');
    IC.assign(2,0);
    reg.assign(4,'#');
    toggle=false;
    PI=0;
    SI=3;
    TI=0;
    EM=0;
    IOI=1;
    cards=0;
    vector<char> v(40);
    for(int i=0;i<10;i++){
        emptyBuffer.push(v);
    }

}
void drumPartMemory(int start){
    OutputFile<<"Drum Block "<<start<<"\n";
    for(int i=start*10;i<start*10+10;i++){
        OutputFile<<"Drum Memory Location "<<i<<" ";
        for(int j=0;j<4;j++){
            if(drum[i][j]=='\0'){
                break;
            }
            OutputFile<<drum[i][j]<<" ";
        }
        OutputFile<<"\n";
    }
    OutputFile<<"\n";
}
void printPartMemory(int start){
    OutputFile<<"Block "<<start/10<<"\n";
    for(int i=start;i<start+10;i++){
        OutputFile<<"Memory Location "<<i<<" ";
        for(int j=0;j<4;j++){
            if(mainMemory[i][j]=='\0'){
                break;
            }
            OutputFile<<mainMemory[i][j]<<" ";
        }
        OutputFile<<"\n";
    }
    OutputFile<<"\n";
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
void printLines(){
    cout<<"Print Extra Lines"<<endl;
    if(readyQueue.size()>0){
        i_oQueue.push(readyQueue.front());
        readyQueue.pop();
    }
    if(terminateQueue.size()>0){
        i_oQueue.push(terminateQueue.front());
        terminateQueue.pop();
    }
    if(i_oQueue.front()->no_oc==0){
        i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
        i_oQueue.front()->no_oc+=1;
    }
    else{
        i_oQueue.front()->no_oc+=1;
    }
    int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
    block%=50;
    
    for(int i=0;i<10;i++){
        for(int j=0;j<4;j++){
            drum[block*10+i][j]='\0';
        }
    }
    string status="IR ";
    for(int i=0;i<4;i++){
        status.push_back(IR[i]);
    }
    //status.push_back(IR);
    // status.push_back(' ');
    // status.push_back('I');
    // status.push_back('C');
    // //status.push_back(IC);
    // status.push_back((IC[0]-'0')+'0');
    // status.push_back(IC[1]);
    // status.push_back('\n');
    // status.push_back('\n');
    // MyFile<<"Status "<<status;
    drum[block*10+0][0]='I';
    drum[block*10+0][1]='R';
    drum[block*10+0][2]=' ';
    drum[block*10+0][3]=IR[0];
    drum[block*10+1][0]=IR[1];
    drum[block*10+1][1]=IR[2];
    drum[block*10+1][2]=IR[3];
    drum[block*10+1][3]=' ';
    drum[block*10+2][0]='I';
    drum[block*10+2][1]='C';
    drum[block*10+2][2]=' ';
    drum[block*10+2][3]=IC[0]+'0';
    drum[block*10+3][0]=IC[1]+'0';
    drum[block*10+3][1]='\n';
    drum[block*10+3][2]='\n';
    drum[block*10+3][3]=' ';
    MyFile<<"Data ";
    for(int i=0;i<10;i++){
        for(int j=0;j<4;j++){
            if(drum[block*10+i][j]=='\0'){
                break;
            }
            MyFile<<drum[block*10+i][j];
        }
    }
    MyFile<<"\n";
    terminateQueue.push(i_oQueue.front());
    i_oQueue.pop();
    // drumPartMemory(block);
}
void terminate(int errorCode){
    cout<<"Terminate"<<" Error Code "<<errorCode<<endl;
    if(readyQueue.size()>0){
        i_oQueue.push(readyQueue.front());
        readyQueue.pop();
    }
    if(terminateQueue.size()>0){
        i_oQueue.push(terminateQueue.front());
        terminateQueue.pop();
    }
    //cout<<"Terminate Queue "<<terminateQueue.size()<<endl;
    PCB2=i_oQueue.front();
    while(ch3_waiting){
        ch2_wait=false;
        ch3_wait=false;
        ch3_waitIS=false;
        ch1_time=ch1_time>=5?5:++ch1_time;
        ch2_time=ch2_time>=5?5:++ch2_time;
        ch3_time=ch3_time>=2?2:++ch3_time;
        timer++;
        for(int i=1;i<=3;i++){
            cout<<inputSpooling<<" "<<outputSpooling<<endl;
            if(i==1){
                if(ch1_time>=5 && !ch1_stop){
                    // cout<<ch1_time<<endl;
                    IOI+=1;
                    IR1();
                }
            }
            else if(i==2){
                if(ch2_time>=5){
                    IOI+=2;
                    IR2();
                }
            }
            else{
                if(ch3_time>=2){
                    IOI+=4;
                    IR3();
                }
            }
        }
        cout<<"CH3_Waiting "<<ch3_waiting<<" CH3_wait "<<ch3_wait<<endl;
    }
    cout<<"Queue "<<terminateQueue.size()<<endl;
    if(errorCode==0){
        error="No Error";
        if(i_oQueue.front()->no_oc==0){
            i_oQueue.front()->no_oc=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
            i_oQueue.front()->no_oc+=1;
        }
        else{
            i_oQueue.front()->no_oc+=1;
        }
        int dataIndex=0;
        cout<<"Output LineData ";
        int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
        block%=50;
        for(int i=0;i<10 && dataIndex<error.length();i++){
            for(int j=0;j<4 && dataIndex<error.length();j++){
                drum[block*10+i][j]=error[dataIndex++];
                cout<<error[dataIndex-1];
            }
        }
        cout<<endl;
        // drumPartMemory(block);
        EM=0;
    }
    else if(errorCode==1){
        error="Out of Data";

        if(i_oQueue.front()->no_oc==0){
            i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
            i_oQueue.front()->no_oc+=1;
        }
        else{
            i_oQueue.front()->no_oc+=1;
        }
        int dataIndex=0;
        int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
        block%=50;
        for(int i=0;i<10 && dataIndex<error.length();i++){
            for(int j=0;j<4 && dataIndex<error.length();j++){
                drum[block*10+i][j]=error[dataIndex++];
            }
        }
        EM=1;
        drumPartMemory(block);
    }
    else if(errorCode==2){
        error="Line Limit Exceeded";

        if(i_oQueue.front()->no_oc==0){
            i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
            i_oQueue.front()->no_oc+=1;
        }
        else{
            i_oQueue.front()->no_oc+=1;
        }
        // cout<<"drumBlock "<<PCB2->d_track<<endl;
        int dataIndex=0;
        int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
        block%=50;
        for(int i=0;i<10 && dataIndex<error.length();i++){
            for(int j=0;j<4 && dataIndex<error.length();j++){
                drum[block*10+i][j]=error[dataIndex++];
            }
        }
        EM=2;
        drumPartMemory(block);
    }
    else if(errorCode==3){
        error="Time Limit Exceeded";
        // cout<<"Error "<<error<<" "<<error.size()<<" "<<PCB2->no_oc<<endl;
        if(i_oQueue.front()->no_oc==0){
            i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
            i_oQueue.front()->no_oc+=1;
        }
        else{
            i_oQueue.front()->no_oc+=1;
        }
        int dataIndex=0;
        cout<<"DrumBlock "<<i_oQueue.front()->d_track<<endl;
        cout<<"Output LineData ";
        int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
        block%=50;
        for(int i=0;i<10 && dataIndex<error.length();i++){
            for(int j=0;j<4 && dataIndex<error.length();j++){
                drum[block*10+i][j]=error[dataIndex++];
                cout<<drum[block*10+i][j];
            }
        }
        cout<<endl;
        EM=3;
        drumPartMemory(block);
    }
    else if(errorCode==4){
        error="Operation Code Error";
        if(i_oQueue.front()->no_oc==0){
            i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
            i_oQueue.front()->no_oc+=1;
        }
        else{
            i_oQueue.front()->no_oc+=1;
        }
        int dataIndex=0;
        int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
        block%=50;
        for(int i=0;i<10 && dataIndex<error.length();i++){
            for(int j=0;j<4 && dataIndex<error.length();j++){
                drum[block*10+i][j]=error[dataIndex++];
            }
        }
        EM=4;
        drumPartMemory(block);
    }
    else if(errorCode==5){
        error="Operand Error";
        if(i_oQueue.front()->no_oc==0){
            i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
            i_oQueue.front()->no_oc+=1;
        }
        else{
            i_oQueue.front()->no_oc+=1;
        }
        int dataIndex=0;
        int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
        block%=50;
        for(int i=0;i<10 && dataIndex<error.length();i++){
            for(int j=0;j<4 && dataIndex<error.length();j++){
                drum[block*10+i][j]=error[dataIndex++];
            }
        }
        EM=5;
        drumPartMemory(block);
    }
    else if(errorCode==6){
        error="Invalid Page Fault";
        if(i_oQueue.front()->no_oc==0){
            i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
            i_oQueue.front()->no_oc+=1;
        }
        else{
            i_oQueue.front()->no_oc+=1;
        }
        int dataIndex=0;
        int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
        block%=50;
        for(int i=0;i<10 && dataIndex<error.length();i++){
            for(int j=0;j<4 && dataIndex<error.length();j++){
                drum[block*10+i][j]=error[dataIndex++];
            }
        }
        EM=6;
        drumPartMemory(block);
    }
    MyFile<<"CH3 started terminate Operation using channel 3 at timer "<<timer<<" ";
    int block1=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
    OutputFile<<"CH3 writing data to Drum\n";
    block1%=50;
    drumPartMemory(block1);
    for(int i=0;i<10;i++){
        for(int j=0;j<4;j++){
            if(drum[block1*10+i][j]=='\0'){
                break;
            }
            MyFile<<drum[block1*10+i][j];
        }
    }
    MyFile<<"\n";
    cout<<"PCB-O_Track "<<i_oQueue.front()->o_track<<endl;
}
void readData(int memoryLocation){
    OutputFile<<"Read Data started at time "<<timer<<"\n";
    int dataIndex=0;
    memoryLocation=(memoryLocation/10)*10;
    int row=memoryLocation;
    int col=0;
    while(ch3_waiting || ch3_wait){
        // MyFile<<"Timer ReadData "<<timer<<" CH3_waiting "<<ch3_waiting<<" CH3_wait "<<ch3_wait<<"\n";
        SI=3;
        simulation();
    }
    // MyFile<<"Timer ReadData start "<<timer<<" CH3_waiting "<<ch3_waiting<<" CH3_wait "<<ch3_wait<<"\n";
    SI=1;
    ch3_waiting=true;
    ch3_time=0;
    i_oloading=true;
    cout<<"Instruction: ";
    for(int i=0;i<4;i++){
        cout<<IR[i];
    }
    cout<<"Testing"<<endl;
    cout<<endl;
    if(i_oQueue.front()->no_dc==0){
        terminate(1);
        printLines();
        return;
    }
    else{
        i_oQueue.front()->no_dc-=1;
    }
    ++ch3_time;
    int block=i_oQueue.front()->d_track;
    for(int i=0;i<10;i++){
        for(int j=0;j<4;j++){
            mainMemory[row+i][j]=drum[block*10+i][j];
            drum[block*10+i][j]='\0';
        }
    }
    printPartMemory(row);
    MyFile<<"CH3 started I/O Read Operation using channel 3 at timer "<<timer<<" ";
    //cout<<"Main Memory ";
    for(int i=0;i<10;i++){
        for(int j=0;j<4;j++){
            if(mainMemory[row+i][j]=='\0'){
                break;
            }
            MyFile<<mainMemory[row+i][j];
        }
    }
    MyFile<<"\n";
    i_oQueue.front()->d_track+=1;
    i_oQueue.front()->d_track%=50;
    cout<<endl;
    increment();
    // printPartMemory(memoryLocation);
    
}

void writeData(int memoryLocation){
    OutputFile<<"Write Data started  at time "<<timer<<"\n";
    while(ch3_waiting || ch3_wait){
        SI=3;
        simulation();
    }
    SI=2;
    ch3_waiting=true;
    ch3_time=0;
    i_oloading=true;
    if(i_oQueue.front()->no_oc==0){
        i_oQueue.front()->o_track=i_oQueue.front()->d_track+i_oQueue.front()->no_dc;
        i_oQueue.front()->no_oc+=1;
    }
    else{
        i_oQueue.front()->no_oc+=1;
    }
    ++ch3_time;
    int row=(memoryLocation/10)*10;
    int block=i_oQueue.front()->o_track+i_oQueue.front()->no_oc-1;
    block%=50;
    for(int i=0;i<10;i++){
        for(int j=0;j<4;j++){
            if(drum[block*10+i][j]!='\0'){
                drum[block*10+i][j]='\0';
            }
        }
    }
    MyFile<<"CH3 started I/O Write Operation using channel 3 at timer "<<timer<<" ";
    for(int i=0;i<10;i++){
        for(int j=0;j<4;j++){
            if(mainMemory[row+i][j]=='\0'){
                break;
            }
            drum[block*10+i][j]=mainMemory[row+i][j];
            MyFile<<drum[block*10+i][j];
        }
    }
    MyFile<<"\n";
    drumPartMemory(block);
    increment();
}

void loadReg(int memoryLocation){
    for(int col=0;col<4;col++){         
        reg[col]=mainMemory[memoryLocation][col];
    }
    OutputFile<<"Load register started  at time "<<timer<<"\n";
    OutputFile<<"Register ";
    for(int col=0;col<4;col++){
        if(reg[col]=='\0'){
            break;
        }
        OutputFile<<reg[col]<<" ";
    }
    OutputFile<<"\n";
    OutputFile<<"\n";
    increment();
}
void storeReg(int memoryLocation){
    cout<<"Memory Location "<<memoryLocation<<endl;
    for(int col=0;col<4;col++){         
        mainMemory[memoryLocation][col]=reg[col];
    }
    OutputFile<<"Store Register content started  at time "<<timer<<" at memory Location "<<memoryLocation<<"\n";
    OutputFile<<"Memory Location "<<memoryLocation<<" ";
    for(int col=0;col<4;col++){
        if(mainMemory[memoryLocation][col]=='\0'){
            break;
        }
        OutputFile<<mainMemory[memoryLocation][col]<<" ";
    }
    OutputFile<<"\n";
    OutputFile<<"\n";
    // mainMemoryPrint();
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
    cout<<IOI<<endl;
    if(SI==1 && TI==0){
        readData(memoryLocation);
    }
    else if(SI==1 && TI==2){
        terminate(3);
        SI=3;
        TI=0;
        printLines();
    }
    else if(SI==2 && TI==0){
        writeData(memoryLocation);
    }
    else if(SI==2 && TI==2){
        terminate(3);
        printLines();
        SI=3;
        TI=0;
    }
    // else if(SI==3 && TI==0){
    //     terminate(0);
    // }
    else if(SI==3 && TI==2){
        terminate(2);
        printLines();
        SI=3;
        TI=0;
    }

    if(PI==1 && TI==0){
        terminate(4);
        printLines();
        PI=0;
    }
    else if(PI==1 && TI==2){
        terminate(3);
        terminate(4);
        printLines();
        PI=0;
        TI=0;
    }
    else if(PI==2 && TI==0){
        terminate(5);
        printLines();
        PI=0;
    }
    else if(PI==2 && TI==2){
        terminate(3);
        terminate(5);
        printLines();
        PI=0;
        TI=0;
    }
    else if(PI==3 && TI==0){
        terminate(6);
        printLines();
        PI=0;
    }
    else if(PI==3 && TI==2){
        terminate(3);
        printLines();
        PI=0;
        TI=0;
    }
    // cout<<"IOI "<<IOI<<endl;
    if(IOI==1){
        IR1();
    }
    else if(IOI==2){
        IR2();
    }
    else if(IOI==3){
        IR2();
        IR1();
    }
    else if(IOI==4){
        IR3();
    }
    else if(IOI==5){
        IR1();
        IR3();
    }
    else if(IOI==6){
        IR3();
        IR2();
    }
    else if(IOI==7){
        IR2();
        IR1();
        IR3();
    }
    SI=3;
    if(terminateQueue.size()>0){
        cout<<"terminate PCB-O_Track "<<terminateQueue.front()->o_track<<endl;
    }
    return;
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
    
    // mainMemoryPrint();
    while(true){
        PCB2=readyQueue.front();
        // mainMemoryPrint();
        cout<<"TTC "<<PCB2->TTC<<" TTL "<<PCB2->TTL<<" TLC "<<PCB2->TLC<<" TLL "<<PCB2->TLL<<endl;
        cout<<"Address "<<(IC[0]*10+IC[1])<<endl;
        cout<<getRealAddress(IC[0]*10+IC[1]);
        IR=mainMemory[getRealAddress(IC[0]*10+IC[1])];

        cout<<(IC[0]*10+IC[1])<<endl;
        OutputFile<<"Instruction: ";
        for(int i=0;i<4;i++){
            OutputFile<<IR[i];
        }
        OutputFile<<"\n";
        int memoryLocation=(IR[2]-'0')*10+(IR[3]-'0');
        //cout<<"PCB2->TTC "<<PCB2->TTC<<endl;

        if(IR[0]!='H' && checkPageFault(memoryLocation)){
            PI=3;
            MOS(memoryLocation);
            return;
        }
        if(IR[0]!='H' && (memoryLocation>99 || memoryLocation<0)){
            PI=2;
            MOS(memoryLocation);
            return;
        }
        int RA=getRealAddress(memoryLocation);
        cout<<"Virtual Address: "<<(IC[0]*10+IC[1])<<" Real Address: "<<getRealAddress(IC[0]*10+IC[1])<<" Memory Location: "<<memoryLocation<<" RA: "<<RA<<endl;;
        if(IR[0]=='G' && IR[1]=='D'){
            PCB2->TTC+=1;
            SI=1;
            if(PCB2->TTC>PCB2->TTL){
                TI=2;
                MOS(RA);
                return;
            }
            if(PCB2->no_dc<=0){
                terminate(1);
                printLines();
                return;
            }
            i_oQueue.push(readyQueue.front());
            readyQueue.pop();
            MOS(RA);
            simulation();
            PCB2->TTC+=1;
            readyQueue.push(i_oQueue.front());
            i_oQueue.pop();
            if(EM!=0){
                return;
            }
        }
        else if(IR[0]=='P' && IR[1]=='D'){
            PCB2->TTC+=1;
            PCB2->TLC+=1;
            SI=2;
            if(PCB2->TTC>PCB2->TTL){
                TI=2;
                MOS(RA);
                return;
            }
            if(PCB2->TLC>PCB2->TLL){
                terminate(2);
                printLines();
                return;
            }
            cout<<"Read PD"<<endl;
            i_oQueue.push(readyQueue.front());
            readyQueue.pop();
            MOS(RA);
            readyQueue.push(i_oQueue.front());
            i_oQueue.pop();
            cout<<"Write Completed"<<endl;
            cout<<"PD PCB-O_Track "<<PCB2->o_track<<endl;
        }
        else if(IR[0]=='L' && IR[1]=='R'){
            PCB2->TTC+=1;
            if(PCB2->TTC>PCB2->TTL){
                TI=2;
                MOS(RA);
                return;
            }
            loadReg(RA);
        }
        else if(IR[0]=='S' && IR[1]=='R'){
            PCB2->TTC+=1;
            simulation();
            PCB2->TTC+=1;
            if(PCB2->TTC>PCB2->TTL){
                TI=2;
                MOS(RA);
                return;
            }
            storeReg(RA);
            cout<<"Storing Completed"<<endl;
        }
        else if(IR[0]=='C' && IR[1]=='R'){
            PCB2->TTC+=1;
            if(PCB2->TTC>PCB2->TTL){
                TI=2;
                MOS(RA);
                return;
            }
            compare(RA);
        }
        else if(IR[0]=='B' && IR[1]=='T'){
            cout<<"Toggle: "<<toggle<<endl;
            PCB2->TTC+=1;
            if(PCB2->TTC>PCB2->TTL){
                TI=2;
                MOS(RA);
                return;
            }
            branch(memoryLocation);
        }
        else if(IR[0]=='H'){
            PCB2->TTC+=1;
            if(readyQueue.size()>0){
                i_oQueue.push(readyQueue.front());
                cout<<"Output Lines "<<i_oQueue.front()->no_oc<<endl;
                readyQueue.pop();
            }
            SI=3;
            if(PCB2->TTC>PCB2->TTL){
                TI=2;
                MOS(RA);
                return;
            }
            terminate(0);
            printLines();
            return;
        }
        else{
            PI=1;
            MOS(RA);
            return;
        }
        cout<<"PCB-O_Track "<<PCB2->o_track<<endl;
        simulation();
        cout<<"Instruction: ";
        for(int i=0;i<4;i++){
            cout<<IR[i];
        }
        cout<<endl;
        // printPartMemory(PTR);
        cout<<"Completed Loop"<<endl;
    }
    //bufferPrint();
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

void loadMainMemory(){
    
}

void IR1(){
    string text;
    // cout<<"CH1 "<<emptyBuffer.size()<<ch1_time<<ch1_waiting<<endl;
    if(ch1_time>=5 && ch1_waiting){
        ch1_waiting=false;
        ch3_waitIS=true;
        inputBuffer.push(ch1_temp);
        MyFile<<"CH1 ended added input buffer at time "<<timer<<" ";
        for(int i=0;i<ch1_temp.size();i++){
            MyFile<<ch1_temp[i];
        }
        MyFile<<"\n";
    }
    else if(emptyBuffer.size()>0 && !ch1_waiting){
        emptyBuffer.pop();
        MyFile<<"CH1 started popped empty buffer at time "<<timer<<" ";
        if(!getline(myFile,text)){
            ch1_stop=true;
            vector<char> v(40);
            emptyBuffer.push(v);
            MyFile<<"CH1Stop "<<ch1_stop<<"\n";
        }
        else{
            cout<<text<<endl;
            std::vector<char> buffer(text.begin(),text.end()); 
            ch1_temp=buffer;
            for(int i=0;i<ch1_temp.size();i++){
                MyFile<<ch1_temp[i];
            }
            MyFile<<"\n";
            ch1_time=0;
            ch1_waiting=true;
        }
    }
    ++ch1_time;
    IOI-=1;
}
void IR2(){
    string text;
    if(outputBuffer.size()>0 && !ch2_wait && !ch2_waiting){
        MyFile<<"CH2 started popped output buffer at time "<<timer<<" ";
        ch2_temp=outputBuffer.front();
        for(int i=0;i<ch2_temp.size();i++){
            MyFile<<ch2_temp[i];
        }
        MyFile<<"\n";
        outputBuffer.pop(); 
        // cout<<"InputBuffer "<<inputBuffer.size()<<" OutputBuffer "<<outputBuffer.size()<<" EmptyBuffer "<<emptyBuffer.size()<<endl;
        ch2_time=5;
        ch2_waiting=true;
    }
    else if(ch2_time>=5 && ch2_waiting){
        vector<char> v(40);
        emptyBuffer.push(v);
        MyFile<<"CH2 stopped added empty buffer at time "<<timer<<" ";
        for(int i=0;i<ch2_temp.size();i++){
            MyFile<<ch2_temp[i];
        }
        MyFile<<"\n";
        // cout<<"InputBuffer "<<inputBuffer.size()<<" OutputBuffer "<<outputBuffer.size()<<" EmptyBuffer "<<emptyBuffer.size()<<endl;
        FILE* file = fopen("Output1.txt", "a");
        int dataIndex=0;
        cout<<"File ";
        for(int i=0;i<ch2_temp.size();i++){
            if(ch2_temp[i]=='\0'){
                break;
            }
            cout<<ch2_temp[i];
            fprintf(file,"%c",ch2_temp[i]);
        }
        cout<<endl;
        fprintf(file, "%c",'\n');
        fclose(file);

        ch2_waiting=false;
    }
    ++ch2_time;
    IOI-=2;
}

void IR3(){
    // cout<<"LoadQueue "<<loadQueue.size()<<" ReadyQueue "<<readyQueue.size()<<" I/OQueue "<<i_oQueue.size()<<" Terminate Queue "<<terminateQueue.size()<<endl;
    // cout<<"CH3"<<" "<<ch3_waiting<<" "<<ch3_time<<" "<<inputBuffer.size()<<" "<<emptyBuffer.size()<<" IOI "<<IOI<<endl;
    // cout<<"LoadMemory "<<loadMemory<<" InputSpooling "<<inputSpooling<<" OutputSpooling "<<outputSpooling<<" Input_OutputSpooling "<<i_oloading<<endl;
    if(ch3_time>=2 && i_oloading && ch3_waiting){
        MyFile<<"I_OQueue operation completed at time "<<timer<<"\n";
        i_oloading=false;
        ch3_waiting=false;
        ch3_wait=true;
    }
    else if(ch3_time>=2 && ch3_waiting && loadMemory && !processing){
        MyFile<<"CH3 stopped loading memory at time "<<timer<<"\n";
        ch3_waiting=false;
        loadMemory=false;
        if(loadQueue.front()->no_pc==0){
            readyQueue.push(loadQueue.front());
            loadQueue.pop();
            // mainMemoryPrint();
            IC.assign(2,0);
            int temp_state=IOI;
            IOI=0;
            readyQueue.front()->TTC=0;
            readyQueue.front()->TLC=0;
            processing=true;
            // mainMemoryPrint();
            PCBPrint();
            cout<<"Processing starts Timer "<<timer<<endl;
            // mainMemoryPrint();
            MyFile<<"Execution starts at time "<<timer<<"\n";
            ch3_wait=true;
            mainMemoryPrint();
            execute();
            MyFile<<"Execution ends at time "<<timer<<"\n";
            mainMemoryLoad();
            cout<<"Processing ends Timer "<<timer<<" Output Track "<<terminateQueue.front()->o_track<<" No_OC "<<terminateQueue.front()->no_oc<<endl;
            PTRInitialize=false;
            processing=false;
            start=true;
            IOI=temp_state;
            cout<<"After Execution"<<endl;
            ch3_wait=true;
        }
    }

    else if(ch3_time>=2 && ch3_waiting && inputSpooling){
        int dataIndex=0;
        inputSpooling=false;
        ch3_waiting=false;
        vector<char> v(40);
        emptyBuffer.push(v);
        MyFile<<"CH3 stopped Input Spooling added Empty Buffer at time "<<timer<<" ";
        for(int i=0;i<ch3_temp.size();i++){
            MyFile<<ch3_temp[i];
        }
        MyFile<<"\n";
        if(!(ch3_temp[0]=='$' && ch3_temp[1]=='A' && ch3_temp[2]=='M' && ch3_temp[3]=='J') && !(ch3_temp[0]=='$' && ch3_temp[1]=='D' && ch3_temp[2]=='T' && ch3_temp[3]=='A') && !(ch3_temp[0]=='$' && ch3_temp[1]=='E' && ch3_temp[2]=='N' && ch3_temp[3]=='D')){
            int row=0;
            int col=0;
            if(PCB1->P){
                PCB1->no_pc+=1;
                cout<<"PCB1->No-PC "<<PCB1->no_pc<<endl;
            }
            else if(PCB1->D){
                PCB1->no_dc+=1;
            }
            for(int i=0;i<40 && dataIndex<ch3_temp.size();i++){
                drum[drumBlock*10+row][col++]='\0';
                if(col==4){
                    col=0;
                    row++;
                }
            }
            row=0;
            col=0;
            for(int i=0;i<40 && dataIndex<ch3_temp.size();i++){
                drum[drumBlock*10+row][col++]=ch3_temp[dataIndex++];
                if(col==4){
                    col=0;
                    row++;
                }
            }
            drumBlock++;
            drumBlock%=50;
            drumPartMemory(drumBlock-1);
        }
        if(ch3_temp[0]=='$' && ch3_temp[1]=='E' && ch3_temp[2]=='N' && ch3_temp[3]=='D'){
            PCBPrint();
            drumBlock+=10;
            drumBlock%=50;
            // drumPrint();
            loadQueue.push(PCB1);
            loadMainMemory();

        }
        if(ch3_temp[0]=='$' && ch3_temp[1]=='A' && ch3_temp[2]=='M' && ch3_temp[3]=='J'){
            cout<<"PCB1"<<endl;
            PCB1=(struct PCB*) malloc(sizeof(struct PCB));
            PCB1->jobId=(ch3_temp[4]-'0')*1000+(ch3_temp[5]-'0')*100+(ch3_temp[6]-'0')*10+(ch3_temp[7]-'0');
            PCB1->TTL=(ch3_temp[8]-'0')*1000+(ch3_temp[9]-'0')*100+(ch3_temp[10]-'0')*10+(ch3_temp[11]-'0');
            PCB1->TLL=(ch3_temp[12]-'0')*1000+(ch3_temp[13]-'0')*100+(ch3_temp[14]-'0')*10+(ch3_temp[15]-'0');
            PCB1->P=true;
            PCB1->p_track=drumBlock;
            PCB1->no_pc=0;
            PCB1->no_dc=0;
            PCB1->no_oc=0;
            PCB1->TTC=0;
            PCB1->TLC=0;
            MyFile<<"PCB created"<<"\n";
        }
        if(ch3_temp[0]=='$' && ch3_temp[1]=='D' && ch3_temp[2]=='T' && ch3_temp[3]=='A'){
            PCB1->P=false;
            PCB1->D=true;
            PCB1->d_track=drumBlock;
        }
        OutputFile<<"InputSpooling Completed\n";
        //drumPartMemory((drumBlock-1)*10);
        ch3_wait=true;
    }
    else if(ch3_time>=2 && ch3_waiting && outputSpooling){
        outputBuffer.push(ch3_temp);
        // cout<<"CH3_Temp.size() "<<ch3_temp.size()<<endl;
        MyFile<<"CH3 stopped Output Spooling added Output Buffer at time "<<timer<<" ";
        for(int i=0;i<ch3_temp.size();i++){
            if(ch3_temp[i]=='\0'){
                break;
            }
            MyFile<<ch3_temp[i];
        }
        MyFile<<"\n";
        outputSpooling=false;
        ch3_waiting=false;
        ch2_wait=true;
        ch3_wait=true;
    }
    //Input Spooling
    else if(inputBuffer.size()>0 && !ch3_waitIS){
        inputSpooling=true;
        ch3_temp=inputBuffer.front();
        cout<<"CH3"<<endl;
        vector<char> v;
        for(int i=0;i<ch3_temp.size();i++){
            v.push_back(ch3_temp[i]);
            cout<<v[i];
            if(ch3_temp[i]=='H' && PCB1->P){
                v.push_back('0');
                v.push_back('0');
                v.push_back('0');
            }
        }
        ch3_temp=v;
        cout<<endl;
        inputBuffer.pop();
        MyFile<<"CH3 started popped input buffer started its operation on Input Spooling at time "<<timer<<" ";
        for(int i=0;i<ch3_temp.size();i++){
            MyFile<<ch3_temp[i];
        }
        MyFile<<"\n";
        ch3_time=0;
        ch3_waiting=true;
    }    
    
    //Load Memory
    else if(loadQueue.size()>0 && (readyQueue.size()+i_oQueue.size()+terminateQueue.size())==0 && !ch3_wait && !processing){
        if(start){
            mainMemoryLoad();
            start=false;
        }
        if(!PTRInitialize){
            PTR=generateRandom();
            PTRInitialize=true;
            PTR*=10;
        }
        if(loadQueue.front()->jobId==2){
            cout<<"JobId "<<loadQueue.front()->jobId<<" No PC "<<loadQueue.front()->no_pc<<" P-Track "<<loadQueue.front()->p_track<<endl;
        }
        int row=generateRandom();
        int col=0;
        mainMemory[PTR+cards][0]='0';
        mainMemory[PTR+cards][1]=cards+'0';
        mainMemory[PTR+cards][2]=row/10+'0';
        mainMemory[PTR+cards++][3]=row%10+'0';
        row*=10;
        int row1=0;
        row1=0;
        cout<<"Loading Data ";
        for(int i=0;i<10;i++){
            for(int j=0;j<4;j++){
                if(drum[loadQueue.front()->p_track*10+i][j]=='\0'){
                    break;
                }
                mainMemory[row+i][j]=drum[loadQueue.front()->p_track*10+i][j];
                cout<<mainMemory[row+i][j];
                drum[loadQueue.front()->p_track*10+i][j]='\0';
            }
        }
        cout<<endl;
        loadQueue.front()->no_pc=loadQueue.front()->no_pc-1;
        cout<<"P_Track "<<loadQueue.front()->p_track<<" No-PC "<<PCB1->no_pc<<endl;
        if(loadQueue.front()->no_pc!=0){
            loadQueue.front()->p_track+=1;
            loadQueue.front()->p_track%=50;
        }
        ch3_time=0;
        MyFile<<"CH3 started loading memory at time "<<timer<<" ";
        for(int i=0;i<10;i++){
            for(int j=0;j<4;j++){
                if(mainMemory[row+i][j]=='\0'){
                    break;
                }
                MyFile<<mainMemory[row+i][j];
            }
        }
        MyFile<<"\n";
        ch3_waiting=true;
        loadMemory=true;
    }
    //Output Spooling
    else if(terminateQueue.size()>0 && emptyBuffer.size()>0 && !ch3_wait){
        PCB2=terminateQueue.front();
        if(PCB2->no_oc==0){
            terminateQueue.pop();
        }
        else{
            MyFile<<"CH3 started popped empty buffer started its operation on Output Spooling at time "<<timer<<" ";
            vector<char> vect;
            emptyBuffer.pop();
            int dataIndex=0;
            int block=PCB2->o_track;
            cout<<"PCB2-no_oc "<<PCB2->no_oc<<endl;
            cout<<"Block "<<block<<endl;
            PCB2->o_track++;
            PCB2->o_track%=50;
            cout<<"PCB2-O_Track "<<PCB2->o_track<<endl;
            PCB2->no_oc-=1;
            cout<<"Data ";
            for(int i=0;i<10;i++){
                for(int j=0;j<4;j++){
                    if(drum[block*10+i][j]=='\0'){
                        break;
                    }
                    MyFile<<drum[block*10+i][j];
                }
            }
            MyFile<<"\n";
            bool flag=false;
            for(int i=0;i<10;i++){
                for(int j=0;j<4;j++){
                    if(drum[block*10+i][j]!='\0'){
                        vect.push_back(drum[block*10+i][j]);
                        drum[block*10+i][j]='\0';
                    }
                    else{
                        break;
                    }
                }
            }
            ch3_temp=vect;
            cout<<"CH3_Temp ";
            for(int i=0;i<ch3_temp.size();i++){
                cout<<ch3_temp[i];
            }
            cout<<endl;
            ch3_time=0;
            ch3_waiting=true;
            outputSpooling=true;
        }
    }
    // cout<<"LoadMemory "<<loadMemory<<" InputSpooling "<<inputSpooling<<" OutputSpooling "<<outputSpooling<<endl;
    ++ch3_time;
    IOI-=4;
}

void simulation(){
    ch2_wait=false;
    ch3_wait=false;
    ch3_waitIS=false;
    ch1_time=ch1_time>=5?5:++ch1_time;
    ch2_time=ch2_time>=5?5:++ch2_time;
    ch3_time=ch3_time>=2?2:++ch3_time;
    // cout<<"CH1_Time "<<ch1_time<<" CH1_Waiting "<<ch1_waiting<<" CH1_Stop "<<ch1_stop<<" CH2_Time "<<ch2_time<<" CH2_Waiting "<<ch2_waiting<<" CH3_Time "<<ch3_time<<" CH3_Waiting "<<ch3_waiting<<endl;
    // cout<<"IOT "<<IOI<<endl;
    // cout<<"Timer "<<timer<<endl;
    // cout<<"CH1Waiting "<<ch1_waiting<<" CH2Waiting "<<ch2_waiting<<" CH3Waiting "<<ch3_waiting<<" EmptyBuffer "<<emptyBuffer.size()<<" IOI "<<IOI<<endl;
    // cout<<"LoadMemory "<<loadMemory<<" InputSpooling "<<inputSpooling<<" OutputSpooling "<<outputSpooling<<" Input_OutputSpooling "<<i_oloading<<endl;
    for(int i=1;i<=3;i++){
        if(i==1){
            if(ch1_time>=5 && !ch1_stop){
                // cout<<ch1_time<<endl;
                IOI+=1;
            }
        }
        else if(i==2){
            if(ch2_time>=5){
                IOI+=2;
            }
        }
        else{
            if(ch3_time>=2){
                IOI+=4;
            }
        }
    }
    // cout<<"IOI "<<IOI<<endl;
    timer++;
    // cout<<"IOI start "<<IOI<<endl;
    // MyFile<<"Timer "<<timer<<" CH3_waiting "<<ch3_waiting<<" CH3_wait "<<ch3_wait<<"\n";
    if(SI!=0 || PI!=0 || TI!=0 || IOI!=0){
        MOS(0);
    }
    // MyFile<<"Timer "<<timer<<" CH3_waiting "<<ch3_waiting<<" CH3_wait "<<ch3_wait<<"\n";
    // cout<<"IOI end "<<IOI<<endl;
}

int main()
{
    load();
    int count=0;
    while(true){
        simulation();
        if(ch1_stop && loadQueue.size()==0 && readyQueue.size()==0 && i_oQueue.size()==0 && terminateQueue.size()==0 && outputBuffer.size()==0 && inputBuffer.size()==0 && emptyBuffer.size()==10){
            break;
        }
        ++count;
    }
    MyFile.close();
    cout<<ch1_stop<<endl;
    // mainMemoryPrint();
    drumPrint();
    return 0;
}