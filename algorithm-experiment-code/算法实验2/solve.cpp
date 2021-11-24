#include<iostream>
#include<cstring>

using namespace std;

#define pii pair<int,int>

const int maxn = 1e2 + 10; 
int n; 
int a[maxn]; //存储矩阵信息
int dp[maxn][maxn]; //当前区间最优解
pii ls[maxn][maxn];	//最优左区间
pii rs[maxn][maxn];	//最优右区间
int L[maxn],R[maxn];	//左括号个数和右括号个数

void dfs(pii a){
	int l=a.first,r=a.second;
	if(l!=r&&!(1==l&&n==r)){
		++L[l],++R[r];
	}
	if(ls[l][r]!=a) dfs(ls[l][r]);
	if(rs[l][r]!=a) dfs(rs[l][r]);
}

void solve(){
	for(int i = 0; i <= n; ++ i)
		cin >> a[i], L[i]=0, R[i]=0; 
	memset(dp, 0x3f, sizeof dp); 
	for(int i = 0; i <= n;  ++ i)
		dp[i][i] = 0; 
	for(int len = 1; len < n ;  ++ len){	//枚举区间长度
		for(int i = 1, j = 1 + len; j <= n;  ++ i,  ++ j){	//枚举端点
			for(int k = i; k < j;  ++ k){	//枚举中间节点
				if(dp[i][j] > dp[i][k] + dp[k + 1][j] + a[i - 1] * a[k] * a[j]){
					dp[i][j]=dp[i][k] + dp[k + 1][j] + a[i - 1] * a[k] * a[j];
					//存储方案
					ls[i][j]=make_pair(i,k);
					rs[i][j]=make_pair(k+1,j);
				}
			}
		}
	}
	cout << dp[1][n] << " "; 	//输出答案
	dfs(make_pair(1,n));	//搜索方案
	for (int i=1;i<=n;++i){	//输出方案
		for (int j=0;j<L[i];++j) cout<<'(';
		cout<<'A'<<i;
		for (int j=0;j<R[i];++j) cout<<')';
	}
	cout<<'\n';

}
signed main(){
	int kase = 0; 
	while(cin >> n){
		cout << "Case " << ++kase << '\n'; 
		solve(); 
	}
}
