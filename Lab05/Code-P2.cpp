#include <iostream>
#include <stdio.h>
#include <queue>
#include <string>
using namespace std;

//#define ONLY_COST
//#define OUTPUT_HORIZONTAL

const int MAXL = 100001;
char DNA_x[MAXL+5];
char DNA_y[MAXL+5];
int lenx = 1,leny = 1;
int line1[MAXL+1],line2[MAXL+1],linemid[MAXL+1];
#ifndef ONLY_COST
queue<pair<int,int>> seq;
#endif
int cost;

inline int singleCost(char l,char r){
    if(l == r) return 0;
    if(l == '-'){
        if(r == 'A'||r == 'G') return 1;
        if(r == 'T') return 2;
        if(r == 'C') return 3;
    }else if(l == 'A'){
        if(r == 'T'||r == 'C') return 1;
        if(r == 'G')return 5;
    }else if(l == 'T'){
        if(r == 'G')return 9;
        if(r == 'C')return 1;
    }else if(l == 'G'&&r == 'C')return 1;

    if(r == '-'){
        if(l == 'A'||l == 'G') return 1;
        if(l == 'T') return 2;
        if(l == 'C') return 3;
    }else if(r == 'A'){
        if(l == 'T'||l == 'C') return 1;
        if(l == 'G')return 5;
    }else if(r == 'T'){
        if(l == 'G')return 9;
        if(l == 'C')return 1;
    }else if(r == 'G'&&l == 'C')return 1;
    cerr<<"Wrong DNA sequence!";
    exit(1);
}

inline int _min(int a,int b){
    if(a < b)return a;
    else return b;
}

void similarity(int beginx,int beginy,int endx,int endy){
    int* linel = &line1[0],*liner = &line2[0];
    int midx,midy;
    if(beginy + 1== endy){
        if(beginx + 1 == endx){
            int tmprl = singleCost('-',DNA_y[endy])+singleCost(DNA_x[endx],'-');
            int tmpb = singleCost(DNA_x[endx],DNA_y[endy]);
#ifndef ONLY_COST
            if(tmprl < tmpb) seq.push(pair<int,int>(beginx,endy));
#endif
            if(tmprl < tmpb) cost = tmprl;
            else cost = tmpb;
        } else if (beginx == endx){
            cost = singleCost('-',DNA_y[endy]);
        }else{
            midx = (beginx + endx)>>1;
            linel[beginx] = 0,liner[beginx] = singleCost('-',DNA_y[endy]);
            for(int i = beginx+1;i <= midx;++i){
                linel[i] = singleCost(DNA_x[i],'-')+linel[i - 1];
                int tmp = _min(singleCost('-',DNA_y[endy])+linel[i],
                               singleCost(DNA_x[i],'-')+liner[i - 1]);
                liner[i] = _min(singleCost(DNA_x[i],DNA_y[endy])+linel[i - 1],tmp);
            }
            int tmpl = linel[midx],tmpr = liner[midx];
            liner[endx] = 0,linel[endx] = singleCost('-',DNA_y[endy]);
            for(int i = endx - 1;i >= midx;--i){
                liner[i] = singleCost(DNA_x[i+1],'-')+liner[i+1];
                int tmp = _min(singleCost(DNA_x[i+1],'-')+linel[i+1],
                               singleCost('-',DNA_y[endy])+liner[i]);
                linel[i] = _min(singleCost(DNA_x[i+1],DNA_y[endy])+liner[i+1],tmp);
            }
            tmpl += linel[midx],tmpr += liner[midx];
            if(tmpl <= tmpr){
#ifndef ONLY_COST
                int i = beginx+1;
                while (i <= midx){
                    seq.push(pair<int,int>(i,beginy));
                    ++i;
                }
                similarity(midx,beginy,endx,endy);
#endif
                cost = tmpl;
            }else{
#ifndef ONLY_COST
                similarity(beginx,beginy,midx,endy);
                seq.push(pair<int,int>(midx,endy));
                int i = midx + 1;
                while(i < endx){
                    seq.push(pair<int,int>(i,endy));
                    ++i;
                }
#endif
                cost = tmpr;
            }
        }
    }else{
        midy = (beginy + endy)>>1;
        //dp,find the (midi,midy)
        //dp1
        //ini
        linel[beginx] = 0;
        for(int i = beginx+1;i <= endx;++i)
            linel[i] = singleCost(DNA_x[i],'-') + linel[i - 1];
        //begin->mid
        for(int i = beginy + 1;i <= midy;++i){
            liner[beginx] = singleCost('-',DNA_y[i]) + linel[beginx];
            for(int j = beginx+1;j <= endx;++j){
                int tmp = _min(singleCost('-',DNA_y[i])+linel[j],
                               singleCost(DNA_x[j],'-')+liner[j - 1]);
                liner[j] = _min(linel[j - 1]+singleCost(DNA_x[j],DNA_y[i]),tmp);
            }
            int* tmp = linel;
            linel = liner;
            liner = tmp;
        }
        for(int i = beginx;i <= endx;++i) linemid[i] = linel[i];
        //dp2
        //ini
        liner[endx] = 0;
        for(int i = endx - 1;i >= beginx;--i)
            liner[i] = liner[i + 1]+ singleCost(DNA_x[i + 1],'-');
        //end->mid
        for(int i = endy - 1;i >= midy;--i){
            linel[endx] = liner[endx] + singleCost('-',DNA_y[i + 1]);
            for(int j = endx - 1;j >= beginx;--j){
                int tmp = _min(singleCost(DNA_x[j+1],'-')+linel[j + 1],
                               singleCost('-',DNA_y[i+1])+liner[j]);
                linel[j] = _min(singleCost(DNA_x[j+1],DNA_y[i+1])+liner[j + 1],tmp);
            }
            int* tmp = linel;
            linel = liner;
            liner = tmp;
        }
        //find midx
        midx = beginx;
        for(int i = beginx;i <= endx;++i){
            linemid[i] += liner[i];
            if(linemid[i] < linemid[midx]) midx = i;
        }
        int mincost = linemid[midx];
        //recurrence
#ifndef ONLY_COST
        similarity(beginx,beginy,midx,midy);
        seq.push(pair<int,int>(midx,midy));
        similarity(midx,midy,endx,endy);
#endif
        cost = mincost;
    }
}

#ifndef ONLY_COST
void _print(){
//    for(int i = seq.size();i > 0;--i){
//        auto it = seq.front();
//        cout << it.first << "||" << it.second << '\n';
//        seq.pop();
//    }
#ifdef OUTPUT_HORIZONTAL
    string x = "",y = "";
    auto begin = seq.front();
    seq.pop();
    for(int i = seq.size();i > 0;--i){
        auto dst = seq.front();
        if(dst.first == begin.first){
            x+="-",y+=DNA_y[dst.second];
        } else if (dst.second == begin.second){
            x+=DNA_x[dst.first],y+=DNA_y[dst.second];
        }else {
            y+=DNA_y[dst.second],x+=DNA_x[dst.first];
        }
        seq.pop();
        begin = dst;
    }
    cout << x << "||" << y;
#else
    auto begin = seq.front();
    seq.pop();
    for(int i = seq.size();i > 0;--i){
        auto dst = seq.front();
        if(dst.first == begin.first){
            cout << "-||" << DNA_y[dst.second] <<'\n';
        } else if (dst.second == begin.second){
            cout << DNA_x[dst.first] << "||-\n";
        }else cout << DNA_x[dst.first] << "||" << DNA_y[dst.second] << '\n';
        seq.pop();
        begin = dst;
    }
#endif
}
#endif

void getDNA(){
    cout << "Input the data by File or Command line? Enter f(file)/c(Command Line): ";
    char input;
    cin.get(input);
    cin.clear();
    cin.sync();
    if(input == 'f'){
        cout << "Please enter two file names:\n";
        string name1,name2;
        cin >> name1 >> name2;
        FILE *fp;
        if((fp = fopen(name1.c_str(),"r")) == NULL){
            cerr << "Fail to read " << name1;
            exit (1) ;
        }
        fseek (fp , 0 , SEEK_END);
        lenx = ftell(fp) - 1;
        fseek (fp , 0 , SEEK_SET);
        fread(DNA_x+1, sizeof(char),lenx,fp);
        int tmp = fclose(fp);
        if(tmp == EOF){
            cerr << "Fail to close " << name1;
            exit (1) ;
        }

        if((fp = fopen(name2.c_str(),"r")) == NULL){
            cerr << "Fail to read " << name2;
            exit (1) ;
        }
        fseek (fp , 0 , SEEK_END);
        leny = ftell(fp) - 1;
        fseek (fp , 0 , SEEK_SET);
        fread(DNA_y+1, sizeof(char),leny,fp);
        tmp = fclose(fp);
        if(tmp == EOF){
            cerr << "Fail to close " << name2;
            exit (1) ;
        }
    }else if(input == 'c'){
        cout << "Please enter two DNA sequences.\n";
        char tmp;
        cin.get(tmp);
        while (tmp != ' '&&tmp != '\n'){
            DNA_x[lenx] = tmp;
            ++lenx;
            cin.get(tmp);
        }
        --lenx;
        while(tmp == ' '||tmp == '\n')cin.get(tmp);
        while(tmp != ' '&&tmp != '\n'){
            DNA_y[leny] = tmp;
            ++leny;
            cin.get(tmp);
        }
        --leny;
//        cout << lenx << ' ' << leny << '\n';
        cin.clear();
        cin.sync();
    }else{
        cerr << "Wrong Input!";
        exit(1);
    }
}

int main(){
    getDNA();
#ifndef ONLY_COST
    seq.push(pair<int,int>(0,0));
#endif
    similarity(0,0,lenx,leny);
#ifndef ONLY_COST
    seq.push(pair<int,int>(lenx,leny));
    cout << "The paired sequence is:\n";
    _print();
#endif
    cout << "Edit distance = "<<cost;
    return 0;
}