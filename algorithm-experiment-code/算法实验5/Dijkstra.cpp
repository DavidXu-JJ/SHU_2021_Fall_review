#include<bits/stdc++.h>
using namespace std;
#define forn(i, n) for (int i = 1; i <= n; ++i)
#define forr(i, n) for (int i = n; i >= 1; --i)
#define IOFast() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)

const int maxn = 100; 
const int inf = 0x3f3f3f3f; 
int n; 
bool vis[maxn]; //标记一个点的dis是否是最短距离
int dis[maxn]; //记录源点到该点的最短距离
int mp[maxn][maxn]; 	//记录两点之间的路径长度
vector<int> G[maxn];	//记录有效路径
int s, t; 

struct node{	//节点类，表示当前状态到u的距离为w
	int u, w; 
	bool operator  < (const node & cmp) const{
		return w > cmp.w; 
	}
};
string ans;

void dfs(int u, string path){
	path = path + (char)('0' + u); 
	if(u == t){
		ans=path;
		return; 
	}
	vector<int> v;
	forn(i, n){
		if(dis[i] + mp[i][u] == dis[u])
			v.push_back(i);	//合法的解加入vector
	}
	int mn=inf;
	for(auto i:v){
		if(i==t){	//如果能到达终点，直接前往终点
			mn=i;
			break;
		}
		mn=min(mn,i);	//挑选编号最小的节点
	}
	dfs(mn,path);
}

void solve(){
	memset(vis, 0, sizeof vis); 		//初始化vis
	memset(dis, 0x3f, sizeof dis); 	//初始化dis
	forn(j, n)forn(i, n){	//反向建图
		cin >> mp[i][j], mp[i][j] = (mp[i][j] == -1?inf:mp[i][j]); 
		if(mp[i][j]!=inf){
			G[i].push_back(j);
			G[j].push_back(i);
		}
	}
	cin >> s >> t; 
	priority_queue < node > q; 
	dis[t] = 0; 
	q.push({t, 0}); 
	while(!q.empty()){		//堆优化Dijkstra
		node now = q.top(); 
		q.pop(); 
		if(vis[now.u]) continue; 
		vis[now.u] = 1; 
		for(int i = 0; i < G[now.u].size();  ++ i){
			int v=G[now.u][i];
			int cost = mp[now.u][v]; 
			if(!vis[v]&&cost + dis[now.u] < dis[v]){
				dis[v] = cost + dis[now.u]; 
				q.push({v, dis[v]}); 
			}
		}
	}
	cout << "The least distance from " << s << " -> " << t << " is " << dis[s] << '\n'; 
	dfs(s, ""); 	//搜索答案
	cout << "the path is "; 
	for(int i = 0; i < ans.size();  ++ i){
		cout << ans[i] - '0'; 
		if(i != ans.size() - 1){
			cout << "->"; 
		}else cout << '\n'; 
	}
}

signed main(){
	IOFast(); 
	int kase = 0; 
	while(cin >> n){
		cout << "Case " << ++kase << '\n'; 
		solve(); 
	}
}
