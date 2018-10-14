#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
#define endl '\n'
ifstream fin("data.txt");
ofstream fout("result3.txt");

struct node{
    bool occupied;
    bool has_occupied_descendent;
    int processing_num; //进程编号
    node(){
        occupied=has_occupied_descendent=0;
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
        build_node(1,0,storage-1);//建立线段树管理区间
        pross2node.clear();
    }
    int add_processing(int processing_num,int distribution_size){
        if(distribution_size>storage||distribution_size<=0)return 0;
        int need_size=storage;
        while(distribution_size<=need_size>>1)need_size>>=1;
        bool success=insert_node(1,0,storage-1,need_size,processing_num);
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
        int mark=0;
        print_node(1,0,storage-1);
    }
    ~FriendSystem(){
        delete[]region;
        region=NULL;
    }
private:
    void build_node(int n,int l,int r){;
        region[n].occupied=0;  //初始化
        if(l==r){
            region[n].has_occupied_descendent=0;
            return;
        }
        int ln=n<<1;    //左子节点编号
        int rn=n<<1|1;  //右子节点编号
        int mid=(l+r)>>1;   //分隔中点
        build_node(ln,l,mid);
        build_node(rn,mid+1,r);
        update(n,ln,rn);//区间状态更新
    }
    bool insert_node(int n,int l,int r,int need_size,int num){
        if(r-l+1<need_size||region[n].occupied)//若区间小于所需大小或已经被占用则不满足条件，回归上一层
            return 0;
        if(r-l+1==need_size){//查询到满足要求的块
            if(region[n].has_occupied_descendent)//该节点已被拆分
                return 0;
            else{
                region[n].occupied=1;
                region[n].processing_num=num;
                pross2node[num].first=l;
                pross2node[num].second=r;
                return 1;
            }
        }
        int mid=(l+r)>>1;   //分隔中点
        int ln=n<<1,ll=l,lr=mid;    //左子节点编号
        int rn=n<<1|1,rl=mid+1,rr=r;  //右子节点编号

        //优先分配被拆开的节点
        if(region[rn].has_occupied_descendent&&!region[ln].has_occupied_descendent){
            swap(ln,rn);swap(ll,rl);swap(lr,rr);
        }
        if(insert_node(ln,ll,lr,need_size,num)){
            update(n,ln,rn);
            return 1;
        }
        else if(insert_node(rn,rl,rr,need_size,num)){
            update(n,ln,rn);
            return 1;
        }
        return 0;
    }
    void remove_node(int n,int l,int r,int lx,int rx){
        if(l==lx&&r==rx){
            region[n].has_occupied_descendent=region[n].occupied=0;
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
        region[n].has_occupied_descendent=region[ln].has_occupied_descendent
                                         |region[rn].has_occupied_descendent
                                         |region[ln].occupied
                                         |region[rn].occupied;
    }
    void print(int l,int r,bool stage,int n){
        fout<<"内存区间: "<<l<<"~"<<r<<"  ";
        fout<<"内存大小: "<<r-l+1<<"  ";
        fout<<"当前状态: ";
        if(stage)fout<<"占用 "<<"进程编号: "<<region[n].processing_num<<endl;
        else fout<<"空闲"<<endl;
    }
    void print_node(int n,int l,int r){
        if(region[n].occupied){
            print(l,r,1,n);
            return;
        }
        if(!region[n].has_occupied_descendent){
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

    return 0;
}
