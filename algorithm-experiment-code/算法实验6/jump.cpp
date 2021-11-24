#include<bits/stdc++.h>
using namespace std;

#define IOFast() ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
char x_1, x_2; 
int posx_1, posy_1, posx_2, posy_2; 
int dx[] = {1, 1, -1, -1, 2, 2, -2, -2}; 	//常量，存储马可以跳跃的八个转移方向
int dy[] = {2, -2, 2, -2, 1, -1, 1, -1}; 
bool vis[10][10];	//记录坐标i，j是否被访问过

struct node{
	int x, y, step; 
}; 
int bfs(int x, int y){
	memset(vis, 0, sizeof vis);
	queue < node > q; 	//存储当前仍可更新的节点的队列
	q.push({x, y, 0}); 
	vis[x][y]=1;	//初始化
	while(!q.empty()){
		node now = q.front(); 
		q.pop(); 
		if(posx_2 == now.x&&posy_2 == now.y){	//第一次到达终点
			return now.step; 
		}
		for(int i = 0; i < 8; ++ i){	//遍历八个转移方向
			node nx; 
			nx.x = now.x + dx[i]; 
			nx.y = now.y + dy[i]; 
			if(nx.x >= 1&&nx.x <= 8&&nx.y >= 1&&nx.y <= 8&&!vis[nx.x][nx.y])
				q.push({nx.x, nx.y, now.step + 1}),vis[nx.x][nx.y]=1;
		}
	}
	return  -1; 	//不可达
}
signed main(){
	IOFast(); 
	while(cin >> x_1 >> posy_1 >> x_2 >> posy_2){
		posx_1 = x_1 - 'a' + 1; 
		posx_2 = x_2 - 'a' + 1; 
		int res = bfs(posx_1, posy_1); 
		cout << x_1 << posy_1 << "==>" << x_2 << posy_2 << ": " << res << " move" << "s "[res == 1] << '\n'; 
	}
}
