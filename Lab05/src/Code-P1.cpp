#include <iostream>
using namespace std;

const int MAXIN = 2000;

int OPT[MAXIN * 2+1] = {0};
int thick[MAXIN+1];
int width[MAXIN+1];

int main(){
    //fast read and write
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n,sumThick = 0;
    cin>>n;
    for (int i = 1;i <= n;++i){
        cin >> thick[i] >> width[i];
        sumThick += thick[i];
    }
    for(int i = 1;i <= n;++i){
        int wt = thick[i]+width[i],j = sumThick;//wt: width+thick
        while(j >= wt){
            if(OPT[j - wt]+thick[i] >= OPT[j])
                OPT[j] = OPT[j - wt]+thick[i];
            --j;
        }
        
    }
    cout << sumThick - OPT[sumThick];
    return 0;
}