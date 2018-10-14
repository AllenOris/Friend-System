/*
author:1711366
date:2018-10-09
伙伴系统,双向链表
注意的问题:
1.分配层级的问题：优先分配小区间，尽量少拆分
2.伙伴合并的问题：伙伴需遵循幂次相邻的原则，不可直接取相邻
*/
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
#define endl '\n'

ifstream fin("data.txt");
ofstream fout("result4.txt");

struct node{
    node* next;
    node* pre;
    bool state;//状态，1代表占用，0代表空闲
    int processing_num;//进程编号
    int nsize;//内存块大小
    int l,r;//管辖的区间（闭区间）
    node(int nsize,int l,int r):nsize(nsize),l(l),r(r){
        pre=next=NULL;processing_num=-1;state=false;
    }
    node(){}
};

class FriendSystem{
protected:
    int storage;
    node *head;
    node *tail;
public:
    FriendSystem(int storage=1):storage(storage){
        head=new node(storage,0,storage-1);
        tail=new node(-1,-1,-1);
        head->next=tail;
        tail->pre=head;
    }
    int add_processing(int processing_num, int distribution_size){ //添加进程，返回实际分配的大小，若为0则分配失败
        if(distribution_size>storage||distribution_size<=0)return 0;
        int temp_size=storage;
        while(distribution_size<=temp_size>>1)temp_size>>=1;//得到应该分配的实际内存大小

        int need_size=temp_size;
        int min_size=storage+1;
        node *p=NULL;
        for(node*it=head;it!=tail;it=it->next){
            if(it->state||it->nsize<need_size)continue;//块太小或已经被占用，则不满足条件
            if(it->nsize<min_size){ //找到最小的块
                p=it;
                min_size=it->nsize;
            }
        }
        if(p)split(p,processing_num,need_size);

        if(min_size==storage+1)return 0;    //没有任何符合条件的区间
        else return need_size;
    }
    bool remove_processing(int processing_num){ //移除进程
        for(node*it=head;it!=tail;it=it->next){
            if(it->processing_num==processing_num){
                set_empty(it);
                return true;
            }
        }
        return false;
    }
    void show_processing(){//输出当前内存状态
        fout<<"当前内存状态为:"<<endl;
        for(node*it=head;it!=tail;it=it->next){
            fout<<"内存区间: "<<it->l<<"~"<<it->r<<"  ";
            fout<<"内存大小: "<<it->nsize<<"  ";
            fout<<"当前状态: ";
            if(it->state){
                fout<<"占用"<<"  ";
                fout<<"进程编号: "<<it->processing_num;
            }
            else{
                fout<<"空闲";
            }
            fout<<endl;
        }
    }
private:
    void split(node* node_now,int processing_num,int need_size){//递归拆分块，直到满足所需内存块大小
        if(node_now->nsize==need_size){//如果当前节点满足所需大小，结束递归
            node_now->state=true;
            node_now->processing_num=processing_num;
            return;
        }

        int new_size=node_now->nsize>>1;

        node_now->nsize=new_size;
        node_now->r=(node_now->l+node_now->r)>>1;
        node* left_node=node_now;    //拆分，可以把当前节点直接变为拆分后的做节点，这样当前节点之前的指向关系就不会改变
        node* right_node=new node(new_size,left_node->r+1,left_node->r+new_size);

        right_node->next=node_now->next;//设置新节点指向关系
        left_node->next=right_node;
        right_node->pre=left_node;  //双向链表的更新
        right_node->next->pre=right_node;

        split(left_node,processing_num,need_size);
    }
    void merge_node(node*left_node,node*right_node){
        node *new_node=left_node;

        new_node->next=right_node->next;  //更新节点关系
        new_node->next->pre=new_node;   //双向链表更新

        new_node->nsize<<=1;
        new_node->r=right_node->r;

        set_empty(new_node);
    }
    void set_empty(node*node_now){
        node_now->state=0;//解除占用
        node_now->processing_num=-1;

        int now_size=node_now->nsize;
        int find_l=(node_now->l%(now_size<<1))==0?node_now->r+1:node_now->l-now_size;//计算伙伴的区间
        int find_r=find_l+now_size-1;

        node *left_node=node_now->pre;
        node *right_node=node_now->next;
        if(left_node&&!left_node->state&&left_node->l==find_l&&left_node->r==find_r){
            merge_node(left_node,node_now);
        }
        else if(!right_node->state&&right_node->l==find_l&&right_node->r==find_r){
            merge_node(node_now,right_node);
        }
    }
};

int main()
{
    //freopen("data.txt","r",stdin);
    ios::sync_with_stdio(0);cin.tie(0);fout.tie(0);
    double st=GetTickCount();
    int n,storage;
    fin>>n>>storage;
    cout<<n<<endl;
    fout<<n<<' '<<storage<<endl;
    FriendSystem fd(storage);
    for(int i=0;i<n;++i){
        int flag;
        fin>>flag;
        if(flag==1){
            int num,nsize;
            fin>>num>>nsize;
            fout<<"进程"<<num<<"分配内存  申请大小: "<<nsize<<"  ";
            int success=fd.add_processing(num,nsize);
            if(success)fout<<"实际分配大小: "<<success<<endl<<"申请成功!";
            else fout<<endl<<"申请失败!";
        }
        else{
            int num;
            fin>>num;
            fout<<"进程"<<num<<"释放内存"<<endl;
            bool success=fd.remove_processing(num);
            if(success)fout<<"释放成功!";
            else fout<<"释放失败!";
        }
        fout<<endl;
        fd.show_processing();
        fout<<endl;
    }
    double ed=GetTickCount();
    cout<<ed-st<<"ms"<<endl;
    return 0;
}
