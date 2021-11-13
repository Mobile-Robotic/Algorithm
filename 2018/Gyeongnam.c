#include <stdio.h>

#define N 10
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

/*
	0 1 2 3 4
	5 6 7 8 9
*/

#define CASE 1

#if CASE==1
char IP[N]={
	1,2,0,0,0,
	0,2,3,3,1
};
#elif CASE==2
char IP[N]={
	3,2,2,1,1,
	0,0,0,0,3
};
#elif CASE==3
char IP[N]={
	1,0,0,0,3,
	2,2,3,0,1
};
#elif CASE==4
char IP[N]={
	0,3,3,2,2,
	1,1,0,0,0
};
#elif CASE==5
char IP[N]={
	1,1,0,2,3,
	0,0,2,0,3,
};
#elif CASE==6
char IP[N]={
	0,0,2,3,3,
	0,0,2,1,1,
};
#endif
char SS[N];
char D=1; // 3 2 1
char g[N][N];

char SortDir;
char TargetColor;

char draw(char a[]){
	for(int i=0;i<N;i++){
		printf("%3d",a[i]);
		if(i==N/2-1) printf("\n");
	}
	printf("\n\n");
}

void conn(char st, char en){
	g[st][en]=g[en][st]=1;
}
void init_g(){
	for(int i=0;i<4;i++) conn(i,i+1), conn(i+5,i+6);
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
//char Change(char value, char mode){
//	if(mode==0) return value/4;
//	if(mode==1) return value%4;
//	return 0;
//}
//void Pose_init(char d){
//	if(d==0) pose[0]--;
//	if(d==1) pose[1]++;
//	if(d==2) pose[0]++;
//	if(d==3) pose[1]--;
//}
//char Find_Node(char x, char y){
//	return x*4+y;
//}
//char safe(char x, char y){
//	return 0<=x && x<=2 && 0<=y && y<=3;
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
char BFS_is_partsame(char a[], char mode){
	char node;
	// CornerNode 4곳 중 가로로 2곳이 퍽으로 채우고
	// 해당 퍽을 SS로 지정
	if(mode==1){
		for(int i=0;i<2;i++){
			if(a[i*5] && a[i*5+4] && a[i*5]!=a[i*5+4]){
				SS[0]=SS[5]=a[i*5];
				SS[4]=SS[9]=a[i*5+4];

				a[i*5]=SS[i*5]=a[i*5+4]=SS[i*5+4]=M;
			
				SortDir=i;
				if(D==1) TargetColor=SS[!SortDir*5];
				else TargetColor=SS[!SortDir*5+4];
				copy(IP,a);
				return 1;
			}
		}
	}
	// 통로와 가까운쪽에 반대편 구석에 필요한 퍽이 있으면
	else if(mode==2){
		if(D==3) node=!SortDir*5;
		else node=!SortDir*5+4;
		if(a[node]==TargetColor){
			copy(IP,a);
			return 1;
		}
	}
	// 통로와 먼쪽 구석이 정답과 일치한다면
	else if(mode==3){
		if(D==1) node=!SortDir*5;
		else node=!SortDir*5+4;
		if(a[node]==SS[node]){
			a[node]=SS[node]=M;
			copy(IP,a);
			return 1;
		}
	}
	// 모든 답이 일치한다면
	else if(mode==4){
		if(same(a,SS)){
			copy(IP,a);
			return 1;
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
	printf("mode=%d, state=%d, rear=%d\n",mode,state,rear);
	if(state==1) Link(rear-1,1);
	return state;
}
void solve(){
	init_g();
	if(D==3) conn(1,6);
	if(D==2) conn(2,7);
	if(D==1) conn(3,8);
	BFS_run(1);
	BFS_run(2);
	BFS_run(3);
	BFS_run(4);
	for(int i=0;i<N;i++) if(SS[i] && IP[i]==SS[i]) IP[i]=SS[i]=M;
	for(int i=0;i<N;i++){
		if(IP[i] && IP[i]!=M){
			if(i<5) SS[i]=SS[i+5]=IP[i];
			else SS[i]=SS[i-5]=IP[i];
			break;
		}
	}
	BFS_run(4);
}
int main(){
	char move_arr[N];
	copy(move_arr,IP);
	solve();
	for(int i=0;i<step_cnt;i++){
		Path_cnt=0;
		Path_S(i);
		printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
		swap(&move_arr[stack[i][0]],&move_arr[stack[i+Path_cnt][1]]);
		
		draw(move_arr);

		i+=Path_cnt;
	}
	printf("%d\n",step_cnt);
	for(int i=0;i<5;i++) if(move_arr[i]!=move_arr[i+5]) return -1;
	printf("Success!!\n");
	return 0;
}