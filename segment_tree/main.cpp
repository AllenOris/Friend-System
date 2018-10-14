#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
#define endl '\n'
ifstream fin("data.txt");
ofstream fout("result3.txt");

struct node{
    bool occupied;
    bool has_occupied_descendent;
    int processing_num; //���̱��
    node(){
        occupied=has_occupied_descendent=0;
        processing_num=-1;
    }
};

class FriendSystem{
protected:
    int storage;
    map <int, pair<int,int> > pross2node;   //����ӳ������
    node *region;
public:
    FriendSystem(int storage):storage(storage){
        region=new node[4*storage];
        build_node(1,0,storage-1);//�����߶�����������
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
            return 0;//�ڴ��в����ڸý���
        int lx=it->second.first;
        int rx=it->second.second;
        remove_node(1,0,storage-1,lx,rx);
        pross2node.erase(it);
        return 1;
    }
    void show_processing(){
        fout<<"��ǰ�ڴ�״̬Ϊ:"<<endl;
        int mark=0;
        print_node(1,0,storage-1);
    }
    ~FriendSystem(){
        delete[]region;
        region=NULL;
    }
private:
    void build_node(int n,int l,int r){;
        region[n].occupied=0;  //��ʼ��
        if(l==r){
            region[n].has_occupied_descendent=0;
            return;
        }
        int ln=n<<1;    //���ӽڵ���
        int rn=n<<1|1;  //���ӽڵ���
        int mid=(l+r)>>1;   //�ָ��е�
        build_node(ln,l,mid);
        build_node(rn,mid+1,r);
        update(n,ln,rn);//����״̬����
    }
    bool insert_node(int n,int l,int r,int need_size,int num){
        if(r-l+1<need_size||region[n].occupied)//������С�������С���Ѿ���ռ���������������ع���һ��
            return 0;
        if(r-l+1==need_size){//��ѯ������Ҫ��Ŀ�
            if(region[n].has_occupied_descendent)//�ýڵ��ѱ����
                return 0;
            else{
                region[n].occupied=1;
                region[n].processing_num=num;
                pross2node[num].first=l;
                pross2node[num].second=r;
                return 1;
            }
        }
        int mid=(l+r)>>1;   //�ָ��е�
        int ln=n<<1,ll=l,lr=mid;    //���ӽڵ���
        int rn=n<<1|1,rl=mid+1,rr=r;  //���ӽڵ���

        //���ȷ��䱻�𿪵Ľڵ�
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
        int ln=n<<1;    //���ӽڵ���
        int rn=n<<1|1;  //���ӽڵ���
        int mid=(l+r)>>1;   //�ָ��е�
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
        fout<<"�ڴ�����: "<<l<<"~"<<r<<"  ";
        fout<<"�ڴ��С: "<<r-l+1<<"  ";
        fout<<"��ǰ״̬: ";
        if(stage)fout<<"ռ�� "<<"���̱��: "<<region[n].processing_num<<endl;
        else fout<<"����"<<endl;
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
        int ln=n<<1;    //���ӽڵ���
        int rn=n<<1|1;  //���ӽڵ���
        int mid=(l+r)>>1;   //�ָ��е�
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
            fout<<"����"<<num<<"�����ڴ�  �����С: "<<nsize<<"  ";
            int success=fd->add_processing(num,nsize);
            if(success)fout<<"ʵ�ʷ����С: "<<success<<endl<<"����ɹ�!";
            else fout<<endl<<"����ʧ��!";
        }
        else{
            int num;
            fin>>num;
            fout<<"����"<<num<<"�ͷ��ڴ�"<<endl;
            bool success=fd->remove_processing(num);
            if(success)fout<<"�ͷųɹ�!";
            else fout<<"�ͷ�ʧ��!";
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
