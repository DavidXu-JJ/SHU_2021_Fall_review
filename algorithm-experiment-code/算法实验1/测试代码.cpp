#include <iostream>
#include <iomanip>
using namespace std;
#define IOFast() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
const int maxn=(1<<10)+10;
int kind[20];
int mp[maxn][maxn];
int kase=0;
int tot=0;
void init(){
    kind[0]=0,kind[1]=1,kind[10]=2,kind[11]=3;
}
void color(int x,int y,int dep,int specialx,int specialy){
    if(dep==0) return;
    int centerx=x+(1<<(dep-1)),centery=y+(1<<(dep-1));
    int status=kind[10*(specialx>=centerx)+(specialy>=centery)];
    switch(status){
        case 0:{
            mp[centerx][centery]=++tot;
            mp[centerx-1][centery]=tot;
            mp[centerx][centery-1]=tot;
            color(x,y,dep-1,specialx,specialy);
            color(centerx,y,dep-1,centerx,centery-1);
            color(x,centery,dep-1,centerx-1,centery);
            color(centerx,centery,dep-1,centerx,centery);
            break;
        }
        case 1:{
            mp[centerx-1][centery-1]=++tot;
            mp[centerx][centery]=tot;
            mp[centerx][centery-1]=tot;
            color(x,y,dep-1,centerx-1,centery-1);
            color(centerx,y,dep-1,centerx,centery-1);
            color(x,centery,dep-1,specialx,specialy);
            color(centerx,centery,dep-1,centerx,centery);
            break;
        }
        case 2:{
            mp[centerx-1][centery-1]=++tot;
            mp[centerx][centery]=tot;
            mp[centerx-1][centery]=tot;
            color(x,y,dep-1,centerx-1,centery-1);
            color(centerx,y,dep-1,specialx,specialy);
            color(x,centery,dep-1,centerx-1,centery);
            color(centerx,centery,dep-1,centerx,centery);
            break;
        }
        case 3:{
            mp[centerx-1][centery-1]=++tot;
            mp[centerx][centery-1]=tot;
            mp[centerx-1][centery]=tot;
            color(x,y,dep-1,centerx-1,centery-1);
            color(centerx,y,dep-1,centerx,centery-1);
            color(x,centery,dep-1,centerx-1,centery);
            color(centerx,centery,dep-1,specialx,specialy);
            break;
        }
    }
}
void show(int n){
    int N=1<<n;
    for(int i=1;i<=N;++i)
        for(int j=1;j<=N;++j)
        if(~mp[i][j]) cout<<setw(2)<<mp[i][j]<<" \n"[j==N];
        else cout<<setw(2)<<"#"<<" \n"[j==N];
}
void solve(int n,int x,int y){
    tot=0;
    mp[x][y]=-1;
    color(1,1,n,x,y);
    show(n);
}  
signed main(){
    int n,x,y;
    init();
    while(cin>>n>>x>>y){
        cout<<"Case "<<++kase<<": n="<<n<<'\n';
        solve(n,x,y);
    }
}
