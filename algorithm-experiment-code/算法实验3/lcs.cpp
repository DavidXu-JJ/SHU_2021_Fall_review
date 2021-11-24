#include<bits/stdc++.h>
using namespace std;

const int maxn = 60; 
char x[maxn]; 	//串1
char y[maxn]; 	//串2
int dp[maxn][maxn]; 	//记录LCS(x,y)
int a, b; 
int cnt; 
vector < string  >  ans; //记录答案
void dfs(int i, int j, string now, int num){
	if(!i||!j){
		if(num != now.size())	return;
		ans.push_back(now); //答案正确，加入ans
		return; 
	}
	if(x[i] == y[j]){	//符合转移条件
		now.push_back(x[i]); 
		dfs(i - 1, j - 1, now, num); 
	}else{
		if(dp[i - 1][j] == dp[i][j])
			dfs(i - 1, j, now, num); 
		if(dp[i][j] == dp[i][j - 1])
			dfs(i, j - 1, now, num); 
	}
}
void solve(){
	ans.clear(); 
	ans.resize(1); 
	cin >> a >> b; 
	for(int i = 1; i <= a;  ++ i){
		cin >> x[i]; 
	}
	for(int i = 1; i <= b;  ++ i){
		cin >> y[i]; 
	}
	memset(dp, 0, sizeof dp); 	
	for(int i = 1; i <= a;  ++ i){
		for(int j = 1; j <= b;  ++ j){
			if(x[i] == y[j])	//转移方程
				dp[i][j] = dp[i - 1][j - 1] + 1; 
			else dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
		}
	}
	dfs(a, b, "", dp[a][b]); 	//搜索答案
	for(int i = 0; i < ans.size();  ++ i)
		reverse(ans[i].begin(), ans[i].end()); 
	cout << "最长公共子序列的长度:" << dp[a][b] << '\n'; 
	cout << "LCS<X,Y>:" << '\n'; 
	for(int i = 0; i < ans.size();  ++ i){
		for(int j = 0; j < ans[i].size();  ++ j){
			cout << ans[i][j] << " \n"[j == ans[i].size() - 1];
		}
	}
	cout<<'\n';
	for(int i=1;i<=a;++i){		//输出dp方程
		for(int j=1;j<=b;++j){
			cout<<dp[i][j]<<" ";
		}
		cout<<'\n';
	}

}
signed main(){
	int t, kase = 0; 
	cin >> t; 
	while(t -- ){
		cout << "Case " << ++kase << '\n'; 
		solve(); 		
	}

}
