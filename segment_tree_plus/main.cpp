#include <bits/stdc++.h>
#include <windows.h>
#define endl '\n'
using namespace std;

ifstream fin("data.txt");
ofstream fout("result5.txt");

struct node{
    int des_state;//子孙状态，每一位代表该层的子孙是否有空闲且未被拆分的区间
    int level;//结点在树中的高度
    int processing_num; //进程编号
    node(){
        des_state=0;
        processing_num=-1;
    }
};

class FriendSystem{
protected:
    int storage;
    map <int, pair<int,int> > pross2node;   //进程映射区间
    node *region;
public:
    FriendSystem(int storage):storage(storage){
        region=new node[4*storage];
        build_node(1,0,storage-1,1);//建立线段树管理区间
        pross2node.clear();
    }
    int add_processing(int processing_num,int distribution_size){
        if(distribution_size>storage||distribution_size<=0)return 0;
        int need_size=storage;
        while(distribution_size<=need_size>>1)need_size>>=1;

        int insert_level=levelAtTree(need_size);
        bool success=insert_node(1,0,storage-1,insert_level,processing_num);
        if(success)return need_size;
        else return 0;
    }
    bool remove_processing(int processing_num){
        map <int,pair<int,int> >::iterator it=pross2node.find(processing_num);
        if(it==pross2node.end())
            return 0;//内存中不存在该进程
        int lx=it->second.first;
        int rx=it->second.second;
        remove_node(1,0,storage-1,lx,rx);
        pross2node.erase(it);
        return 1;
    }
    void show_processing(){
        fout<<"当前内存状态为:"<<endl;
        print_node(1,0,storage-1);
    }
    ~FriendSystem(){
        delete[]region;
        region=NULL;
    }
private:
    const int INF=0x7FFFFFFF;
    int valueAtBit(int num,int x){//一个数num第x位二进制为多少，从右第一位开始
        return (num>>(x-1))&1;
    }
    int levelAtTree(int num){  //宽为num的区间在第几层
        int s=storage, level=1;
        while(s>num){
            s>>=1;
            level++;
        }
        return level;
    }
    void setAtBit(int &num,int x,int b){    //将num第x位置为b
        num=b==0?(b<<(x-1))&num:(b<<(x-1))|num;
    }
    int getFullBit(int x){  //获得一个前x位均为1的32位二进制数
        if(x==32)return -1;
        int num=1<<(x-1);
        return (num-1)|num;
    }
    void fillBit(int &num,int x,int b){   //将num二进制第x位及x位以上所有位置为b
        int cp=getFullBit(x-1);
        num=b==0?num&cp:(~cp)|num;
    }
    int levelAtRegion(int num,int lx,int rx,int b=1){
        for(int i=rx;i>=lx;i--){
            if(valueAtBit(num,i)==b)return i;
        }
        return -1;
    }
    void build_node(int n,int l,int r,int lv){
        region[n].level=lv;
        if(l==r){
            region[n].des_state=0;
            return;
        }
        int ln=n<<1;    //左子节点编号
        int rn=n<<1|1;  //右子节点编号
        int mid=(l+r)>>1;   //分隔中点
        int nsize=r-l+1;
        build_node(ln,l,mid,lv+1);
        build_node(rn,mid+1,r,lv+1);
        update(n,ln,rn);//区间状态更新
        if(lv==1)setAtBit(region[n].des_state,1,1);
    }
    bool insert_node(int n,int l,int r,int insert_level,int num){
        int nsize=r-l+1;
        int lv=region[n].level;

        if(lv==insert_level){//查询到满足要求的块

             if(valueAtBit(region[n].des_state,lv)){
                region[n].processing_num=num;
                setAtBit(region[n].des_state,lv,0);

                pross2node[num].first=l;
                pross2node[num].second=r;
                return 1;
            }
            else
                return 0;
        }

        int mid=(l+r)>>1;   //分隔中点
        int ln=n<<1;
        int rn=n<<1|1;

        push_down(n,ln,rn);

        int lv_left=levelAtRegion(region[ln].des_state,lv+1,insert_level);
        int lv_right=levelAtRegion(region[rn].des_state,lv+1,insert_level);

        if(lv_right==-1&&lv_left==-1)return 0;
        if(lv_left>=lv_right){
            insert_node(ln,l,mid,insert_level,num);
        }
        else{
            insert_node(rn,mid+1,r,insert_level,num);
        }
        update(n,ln,rn);
        return 1;
    }
    void remove_node(int n,int l,int r,int lx,int rx){
        if(l==lx&&r==rx){
            setAtBit(region[n].des_state,region[n].level,1);
            region[n].processing_num=-1;
            return;
        }
        int ln=n<<1;    //左子节点编号
        int rn=n<<1|1;  //右子节点编号
        int mid=(l+r)>>1;   //分隔中点
        if(rx<=mid)remove_node(ln,l,mid,lx,rx);
        else if(lx>mid)remove_node(rn,mid+1,r,lx,rx);
        update(n,ln,rn);
    }
    void update(int n,int ln,int rn){
        int lv=region[n].level;
        if(valueAtBit(region[ln].des_state,lv+1)&&valueAtBit(region[rn].des_state,lv+1)){
            //合并
            region[ln].des_state=region[rn].des_state=region[n].des_state=0;
            setAtBit(region[n].des_state,lv,1);
        }
        else
            region[n].des_state=region[ln].des_state|region[rn].des_state;
    }
    void push_down(int n,int ln,int rn){
        int lv=region[n].level;
        if (!valueAtBit(region[n].des_state,lv))return;//该层已经被拆分
        setAtBit(region[n].des_state,lv,0);
        setAtBit(region[ln].des_state,lv+1,1);
        setAtBit(region[rn].des_state,lv+1,1);
    }
    void print(int l,int r,bool stage,int n){
        fout<<"内存区间: "<<l<<"~"<<r<<"  ";
        fout<<"内存大小: "<<r-l+1<<"  ";
        fout<<"当前状态: ";
        if(stage)fout<<"占用  "<<"进程编号: "<<region[n].processing_num<<endl;
        else fout<<"空闲"<<endl;
    }
    void print_node(int n,int l,int r){
        if(region[n].processing_num!=-1){
            print(l,r,1,n);
            return;
        }
        if(valueAtBit(region[n].des_state,region[n].level)){
            print(l,r,0,n);
            return;
        }
        int ln=n<<1;    //左子节点编号
        int rn=n<<1|1;  //右子节点编号
        int mid=(l+r)>>1;   //分隔中点
        print_node(ln,l,mid);
        print_node(rn,mid+1,r);
    }
};

int main()
{
    ios::sync_with_stdio(0);cin.tie(0);fout.tie(0);
    double st=GetTickCount();

    int n,storage;
    fin>>n>>storage;
    fout<<n<<' '<<storage<<endl;
    FriendSystem* fd=new FriendSystem(storage);
    for(int i=0;i<n;++i){
        int flag;
        fin>>flag;
        if(flag==1){
            int num,nsize;
            fin>>num>>nsize;
            fout<<"进程"<<num<<"分配内存  申请大小: "<<nsize<<"  ";
            int success=fd->add_processing(num,nsize);
            if(success)fout<<"实际分配大小: "<<success<<endl<<"申请成功!";
            else fout<<endl<<"申请失败!";
        }
        else{
            int num;
            fin>>num;
            fout<<"进程"<<num<<"释放内存"<<endl;
            bool success=fd->remove_processing(num);
            if(success)fout<<"释放成功!";
            else fout<<"释放失败!";
        }
        fout<<endl;
        fd->show_processing();
        fout<<endl;
    }
    delete fd;

    double ed=GetTickCount();
    cout<<ed-st<<"ms"<<endl;

    fin.close();fout.close();

    return 0;
}
