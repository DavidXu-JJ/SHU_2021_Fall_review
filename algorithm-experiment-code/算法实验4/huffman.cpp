#include<bits/stdc++.h>
using namespace std;

struct node{
	node* left = NULL;
	node* right = NULL; 
	int id=0, weight=0; 
	node(){}
	node(int id_,int weight_):id(id_), weight(weight_){}
	bool operator  <  (const node & cmp) const{	//小于号重载，用于维护优先队列
		if(weight!=cmp.weight){
			return weight > cmp.weight; 
		}else return id < cmp.id; 
	}
}trie[200]; 	//哈夫曼节点的定义

const int maxn = 50; 
int n; 
int tot; 
int w[maxn];	//每个节点的权值
string ans[maxn]; 	//答案
void dfs(int u, string str){
	if(u >= 1 && u <= n){
		ans[u] = str; 
		return; 
	}
	if(trie[u].left->id!=0)
		dfs(trie[u].left -> id, str + "0"); 
	if(trie[u].right->id!=0)
		dfs(trie[u].right -> id, str + "1"); 
}
void solve(){
	cin >> n; 
	tot = n; 
	priority_queue <node> q;  
	for(int i=1;i<=n;++i){
		cin >> w[i];
		trie[i]=node(i,w[i]);
		q.push(trie[i]); 
	}
	while(q.size() > 1){	//q.size()==1时退出
		node right = q.top(); 
		q.pop(); 
		node left  = q.top(); 
		q.pop(); 
		//选出优先级最大的两个节点
		trie[ ++ tot].right = &trie[right.id]; 	//新建点，进行合并操作
		trie[tot].left = &trie[left.id]; 
		trie[tot].id = tot; 
		trie[tot].weight = trie[left.id].weight + trie[right.id].weight; 
		q.push(trie[tot]);	//新的哈夫曼树加入优先队列
	}
	int rt = q.top().id; 
	dfs(rt, ""); 	//搜索生成哈夫曼编码
	for(int i=1;i<=n;++i)		//输出答案
		cout << w[i] << " " << ans[i] << '\n'; 
}
signed main(){
	int t, kase = 0; 
	cin >> t; 
	while(t -- ){
		cout << "Case " << ++kase << '\n'; 
		solve(); 
	}
}
