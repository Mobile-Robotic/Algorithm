#include <stdio.h>

#define X 3
#define Y 6
#define N X*Y
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N]={
	M,0,M,0,M,M,
	1,2,3,1,2,3,
	M,M,0,M,0,M
};
char SS[N]={
	M,1,M,3,M,M,
	3,0,0,0,0,2,
	M,M,2,M,1,M
};
/*
	0	1	2	3	4	5
	6	7	8	9	10	11
	12	13	14	15	16	17
*/
char g[N][N];

void conn(char st, char en){
	g[st][en]=g[en][st]=1;
}

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
char Change(char value, char mode){
	if(mode==0) return value/Y;
	if(mode==1) return value%Y;
	return 0;
}
void Pose_init(char d){
	if(d==0) pose[0]--;
	if(d==1) pose[1]++;
	if(d==2) pose[0]++;
	if(d==3) pose[1]--;
}
char Find_Node(char x, char y){
	return x*Y+y;
}
char safe(char x, char y){
	return 0<=x && x<X && 0<=y && y<Y;
}
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

// Debug
void draw(char a[]){
	for(int i=0;i<N;i++){
		printf("%4d",a[i]);
		if(Change(i,1)==5) printf("\n");
	}
	printf("\n");
}

char BFS_is_partsame(char a[], char mode){
	if(mode==1){
		for(int i=0;i<N;i++){
			if(SS[i] && SS[i]!=M && SS[i]==a[i]){
				SS[i]=a[i]=M;
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
	for(int i=0;i<4;i++){
		conn(i+1,i+7);
		conn(i+7,i+13);
	}
	for(int i=0;i<5;i++) conn(i+6,i+7);
	for(int i=0;i<6;i++) BFS_run(1);
}
int main(){
	char move_arr[N], first_SS[N];
	copy(move_arr,IP);
	copy(first_SS,SS);
	solve();
	for(int i=0;i<step_cnt;i++){
		Path_cnt=0;
		Path_S(i);
		printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
		swap(&move_arr[stack[i][0]],&move_arr[stack[i+Path_cnt][1]]);
		draw(move_arr);
		i+=Path_cnt;
	}
	printf("step_cnt=%d\n",step_cnt);
	if(same(move_arr,first_SS)) printf("Success!!");
	else printf("fail");
}
