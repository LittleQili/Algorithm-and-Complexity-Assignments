#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAXIN = 2000;

int OPT[MAXIN * 2+1] = {0};
int thick[MAXIN+1];
int width[MAXIN+1];
int n,sumThick = 0;

void getBook(){
    cout << "Input the data by File or Command line? Enter f(file)/c(Command Line): ";
    char input;
    cin.get(input);
    cin.clear();
    cin.sync();
    if(input == 'f'){
        cout << "Please enter the file name:\n";
        string name;
        cin >> name;
        ifstream data(name);
        if(!data.is_open()){
            cerr << "Failed to open " << name;
            exit(1);
        } 
        data>>n;
        for (int i = 1;i <= n;++i){
            data >> thick[i] >> width[i];
            sumThick += thick[i];
        }
        data.close();
    }else if (input == 'c'){
        cin>>n;
        for (int i = 1;i <= n;++i){
            cin >> thick[i] >> width[i];
            sumThick += thick[i];
        }
    }else{
        cerr << "Wrong Input!";
        exit(1);
    }
    
}

int main(){
    getBook();

    for(int i = 1;i <= n;++i){
        int wt = thick[i]+width[i],j = sumThick;//wt: width+thick
        while(j >= wt){
            if(OPT[j - wt]+thick[i] >= OPT[j])
                OPT[j] = OPT[j - wt]+thick[i];
            --j;
        }
        
    }
    cout << "The minimum total thickness of the vertical books = " << sumThick - OPT[sumThick];
    return 0;
}