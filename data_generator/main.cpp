/*
���ϵͳ�������ɣ�
��һ��N��M������N�β���,�ڴ��СΪM(MΪ2����������)
������N�У�ÿ��������������flag,processing,(size)
flag=1ʱ�����������̣����̱��Ϊprocessing,��Сsize
flag=2ʱ������ɾ�����̣����̱��Ϊprocessing
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
    set <processing> s;//�������������
    for(int i=0;i<n;++i){
        int flag;
        if(i==0||s.empty())flag=1;
        else flag=random(0,1)%2+1;
        if(flag==1){
            fout<<flag<<' '<<i<<' '<<(int)pow(2,double(rand())/RAND_MAX*k)<<endl;//2^k�ϷǾ��ȷֲ�
            s.insert(processing(i,rand()));
        }
        else{
            fout<<flag<<' '<<s.begin()->num<<endl;
            s.erase(s.begin());
        }
    }
    return 0;
}
