#include<bits/stdc++.h>
using namespace std;
#define forn(i, n) for (int i = 1; i <= n; ++i)
#define forr(i, n) for (int i = n; i >= 1; --i)
#define IOFast() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)

const int maxn = 30; 
int n; 
int w[maxn];	//记录物品重量
int c1, c2; 	//1、2号船的装载重量
string ans = ""; 	//记录方案
int answ; 	//记录当前1号船的最大装载重量
void dfs(int pos, int weight1,int weight2, string plan){
	if(pos == n + 1){
		if(answ <= weight1){	//如果有更优的方案就更新
			ans = plan; 
			answ = weight1;
		}
		return; 
	}
	if(c2 >= weight2 + w[pos])	//剪枝
		dfs(pos + 1, weight1, weight2 + w[pos], plan + "0"); 
	if(c1 >= weight1 + w[pos])	//剪枝
		dfs(pos + 1, weight1 + w[pos],weight2, plan + "1"); 
}
signed main(){
	IOFast(); 
	int kase = 0; 
	while(cin >> n){
		ans = ""; 
		cout << "Case " << ++kase << '\n'; 
		int sum=0;
		for(int i = 1; i <= n; ++ i){
			cin >> w[i]; 	
			sum += w[i];
		}
		cin >> c1 >> c2;
		if(c1 + c2 < sum){	//剪枝
			cout<<"No\n";
			continue;
		}
		dfs(1, 0, 0, ""); 	//搜索
		if(ans == "")	//判断无解或有解
			cout << "No\n"; 
		else cout << answ << " " << ans << '\n'; 
	}
}
