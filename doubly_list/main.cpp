/*
author:1711366
date:2018-10-09
���ϵͳ,˫������
ע�������:
1.����㼶�����⣺���ȷ���С���䣬�����ٲ��
2.���ϲ������⣺�������ѭ�ݴ����ڵ�ԭ�򣬲���ֱ��ȡ����
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
    bool state;//״̬��1����ռ�ã�0�������
    int processing_num;//���̱��
    int nsize;//�ڴ���С
    int l,r;//��Ͻ�����䣨�����䣩
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
    int add_processing(int processing_num, int distribution_size){ //��ӽ��̣�����ʵ�ʷ���Ĵ�С����Ϊ0�����ʧ��
        if(distribution_size>storage||distribution_size<=0)return 0;
        int temp_size=storage;
        while(distribution_size<=temp_size>>1)temp_size>>=1;//�õ�Ӧ�÷����ʵ���ڴ��С

        int need_size=temp_size;
        int min_size=storage+1;
        node *p=NULL;
        for(node*it=head;it!=tail;it=it->next){
            if(it->state||it->nsize<need_size)continue;//��̫С���Ѿ���ռ�ã�����������
            if(it->nsize<min_size){ //�ҵ���С�Ŀ�
                p=it;
                min_size=it->nsize;
            }
        }
        if(p)split(p,processing_num,need_size);

        if(min_size==storage+1)return 0;    //û���κη�������������
        else return need_size;
    }
    bool remove_processing(int processing_num){ //�Ƴ�����
        for(node*it=head;it!=tail;it=it->next){
            if(it->processing_num==processing_num){
                set_empty(it);
                return true;
            }
        }
        return false;
    }
    void show_processing(){//�����ǰ�ڴ�״̬
        fout<<"��ǰ�ڴ�״̬Ϊ:"<<endl;
        for(node*it=head;it!=tail;it=it->next){
            fout<<"�ڴ�����: "<<it->l<<"~"<<it->r<<"  ";
            fout<<"�ڴ��С: "<<it->nsize<<"  ";
            fout<<"��ǰ״̬: ";
            if(it->state){
                fout<<"ռ��"<<"  ";
                fout<<"���̱��: "<<it->processing_num;
            }
            else{
                fout<<"����";
            }
            fout<<endl;
        }
    }
private:
    void split(node* node_now,int processing_num,int need_size){//�ݹ��ֿ飬ֱ�����������ڴ���С
        if(node_now->nsize==need_size){//�����ǰ�ڵ����������С�������ݹ�
            node_now->state=true;
            node_now->processing_num=processing_num;
            return;
        }

        int new_size=node_now->nsize>>1;

        node_now->nsize=new_size;
        node_now->r=(node_now->l+node_now->r)>>1;
        node* left_node=node_now;    //��֣����԰ѵ�ǰ�ڵ�ֱ�ӱ�Ϊ��ֺ�����ڵ㣬������ǰ�ڵ�֮ǰ��ָ���ϵ�Ͳ���ı�
        node* right_node=new node(new_size,left_node->r+1,left_node->r+new_size);

        right_node->next=node_now->next;//�����½ڵ�ָ���ϵ
        left_node->next=right_node;
        right_node->pre=left_node;  //˫������ĸ���
        right_node->next->pre=right_node;

        split(left_node,processing_num,need_size);
    }
    void merge_node(node*left_node,node*right_node){
        node *new_node=left_node;

        new_node->next=right_node->next;  //���½ڵ��ϵ
        new_node->next->pre=new_node;   //˫���������

        new_node->nsize<<=1;
        new_node->r=right_node->r;

        set_empty(new_node);
    }
    void set_empty(node*node_now){
        node_now->state=0;//���ռ��
        node_now->processing_num=-1;

        int now_size=node_now->nsize;
        int find_l=(node_now->l%(now_size<<1))==0?node_now->r+1:node_now->l-now_size;//�����������
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
            fout<<"����"<<num<<"�����ڴ�  �����С: "<<nsize<<"  ";
            int success=fd.add_processing(num,nsize);
            if(success)fout<<"ʵ�ʷ����С: "<<success<<endl<<"����ɹ�!";
            else fout<<endl<<"����ʧ��!";
        }
        else{
            int num;
            fin>>num;
            fout<<"����"<<num<<"�ͷ��ڴ�"<<endl;
            bool success=fd.remove_processing(num);
            if(success)fout<<"�ͷųɹ�!";
            else fout<<"�ͷ�ʧ��!";
        }
        fout<<endl;
        fd.show_processing();
        fout<<endl;
    }
    double ed=GetTickCount();
    cout<<ed-st<<"ms"<<endl;
    return 0;
}
