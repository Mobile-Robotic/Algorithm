#include <stdio.h>

#define N 13
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

/*
	0	1	2
	  3   4
	5	6	7
	  8   9
	10	11	12
*/
char IP[N] = 
{ 
	0, 2, 0,
	 0, 2, 
	3, 0, 0, 
	 0, 3,
	1, 0, 0
};
char SS[N] = 
{
	3, 0, 1,
	 0, 0,
	0, 2, 0,
	 0, 0,
	2, 0, 3
};

char g[N][N] = 
{
	{0,1,0,0,0,1,0,0,0,0,0,0,0},
	{1,0,1,1,1,0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,1,0,0,0,0,0},
	{0,1,0,0,1,1,0,0,1,0,0,0,0},
	{0,1,0,1,0,0,0,1,0,1,0,0,0},
	{1,0,0,1,0,0,0,0,1,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,1,0,0,0,0,1,0,0,1},
	{0,0,0,1,0,1,0,0,0,0,0,1,0},
	{0,0,0,0,1,0,0,1,0,0,0,1,0},
	{0,0,0,0,0,1,0,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0,1,1,1,0,1},
	{0,0,0,0,0,0,0,1,0,0,0,1,0}
};

char enqueue(char arr[]){
	if(rear<MAX){
		queue[rear][0]=arr[0];
		queue[rear++][1]=arr[1];
		return 1;
	}
	return 0;
}
char *dequeue(){
	if(front<rear) return queue[front++];
	return NULL;
}
void push(char x, char y){
	stack[step_cnt][0]=x;
	stack[step_cnt++][1]=y;
}
void swap(char *a, char *b){
	char tmp=*a;
	*a=*b;
	*b=tmp;
}
void copy(char a[], char b[]){
	for(int i=0;i<N;i++) a[i]=b[i];
}
//char Change(char value, char mode){
//	if(mode==0) return value/Y;
//	if(mode==1) return value%Y;
//	return 0;
//}
//void Pose_init(char d){
//	if(d==0) pose[0]--;
//	if(d==1) pose[1]++;
//	if(d==2) pose[0]++;
//	if(d==3) pose[1]--;
//}
//char Find_Node(char x, char y){
//	return x*Y+y;
//}
//char safe(char x, char y){
//	return 0<=x && x<X && 0<=y && y<Y;
//}
void Link(int a, char mode){
	if(a>0) Link(parent[a],mode);
	else{
		copy(temp_IP,IP);
		return;
	}
	if(mode) push(queue[a][0],queue[a][1]);
	swap(&temp_IP[queue[a][0]],&temp_IP[queue[a][1]]);
}
void Path_S(int i){
	if(i<step_cnt && stack[i][1]==stack[i+1][0]){
		Path_cnt++;
		Path_S(i+1);
	}
}
char same(char a[], char b[]){
	for(int i=0;i<N;i++) if(a[i]!=b[i] && b[i]) return 0;
	return 1;
}
char s_same(char a[]){
	for(int i=0;i<rear;i++){
		Link(i,0);
		if(same(temp_IP,a)) return 1;
	}
	return 0;
}

/*
	0	1	2
	  3   4
	5	6	7
	  8   9
	10	11	12
*/

//Debug
void draw(char a[]){
	for(int i=0;i<N;i++){
		printf("%4d",a[i]);
		if(i==2 || i==7) printf("\n  ");
		if(i==4 || i==9) printf("\n");
	}
	printf("\n\n");
}

char BFS_is_partsame(char a[], char mode){
	if(mode==1){
		for(int i=0;i<N;i++){
			if(SS[i] && SS[i]!=M && a[i]==SS[i]){
				a[i]=SS[i]=M;
				copy(IP,a);
				return 1;
			}
		}
	}
	return 0;
}
char BFS(char mode){
	char a[N], b[N], *p;
	char point[2]={0,};

	front=rear=0;
	enqueue(point);
	while(1){
		p=dequeue();
		if(p==NULL) return 0;
		Link(front-1,0);
		copy(a,temp_IP);
		if(BFS_is_partsame(a,mode)) return 1;
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				if(g[i][j] && a[i] && a[i]!=M && !a[j]){
					copy(b,a);
					swap(&b[i],&b[j]);
					if(s_same(b)==0){
						parent[rear]=front-1;
						point[0]=i;
						point[1]=j;
						if(!enqueue(point)) return 0;
						if(BFS_is_partsame(b,mode)) return 1;
					}
				}
			}
		}
	}
	return 0;
}
char BFS_run(char mode){
	char state=BFS(mode);
	if(state==1) Link(rear-1,1);
	printf("mode=%d, state=%d, rear=%d\n",mode,state,rear);
	return state;
}
void solve(){
	for(int i=0;i<5;i++) BFS_run(1);
}
int main(){
	g[6][8] = g[8][6] = 1;
	char move_arr[N], first_SS[N];
	copy(move_arr,IP);
	copy(first_SS,SS);
	solve();
	for(int i=0;i<step_cnt;i++){
		/*Path_cnt=0;
		Path_S(i);*/
		printf("from %d to %d\n",stack[i][0],stack[i][1]);
		swap(&move_arr[stack[i][0]],&move_arr[stack[i][1]]);
		draw(move_arr);
		/*i+=Path_cnt;*/
	}
	printf("step_cnt=%d\n",step_cnt);
	if(same(move_arr,first_SS)) printf("Success!!");
	else printf("fail");
}
