/*
伙伴系统数据生成：
第一行N，M，代表N次操作,内存大小为M(M为2的整数次幂)
接下来N行，每行两或三个整数flag,processing,(size)
flag=1时，代表插入进程，进程编号为processing,大小size
flag=2时，代表删除进程，进程编号为processing
*/

#include <bits/stdc++.h>
using namespace std;


int random(int a,int b){
    return (rand() % (b-a+1))+ a;
}

struct processing{
    int num, order;
    processing(int num,int order):num(num),order(order){}
    processing(){}
    bool operator<(const processing&other)const{
        return order<other.order;
    }
};

int main()
{
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    //freopen("data.txt","w",stdout);
    ofstream fout("data.txt");
    srand((unsigned)time(0));
    int n=1e3;
    int m=1<<25;
    int k=(int)(floor(log(m)/log(2)));
    fout<<n<<' '<<m<<endl;
    set <processing> s;//随机化进程序列
    for(int i=0;i<n;++i){
        int flag;
        if(i==0||s.empty())flag=1;
        else flag=random(0,1)%2+1;
        if(flag==1){
            fout<<flag<<' '<<i<<' '<<(int)pow(2,double(rand())/RAND_MAX*k)<<endl;//2^k上非均匀分布
            s.insert(processing(i,rand()));
        }
        else{
            fout<<flag<<' '<<s.begin()->num<<endl;
            s.erase(s.begin());
        }
    }
    return 0;
}
