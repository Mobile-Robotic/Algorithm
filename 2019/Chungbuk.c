#include <stdio.h>
#include <math.h>

#define X 4
#define Y 7
#define N X*Y
#define MAX 580
#define M 99	// 장애물

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char gps[2];
char move_dir[N], move_cnt;

#define CASE 8

#if CASE==1
char IP[N]= {
	M,0,0,3,0,0,0,
	0,0,0,0,0,1,0,
	0,3,2,0,0,0,M,
	0,0,0,2,0,1,0
};
char SS[N]= {
	M,0,0,0,0,0,0,
	0,0,0,0,0,3,2,
	0,0,3,0,0,1,M,
	0,2,1,0,0,0,0
};
#elif CASE==2
char IP[N]= {
	0,0,0,0,1,0,0,
	2,0,0,2,0,3,M,
	0,M,1,0,0,0,0,
	0,0,0,0,0,0,3
};
char SS[N]= {
	0,0,0,0,0,0,0,
	0,0,0,0,0,3,M,
	0,M,3,0,0,1,2,
	0,2,1,0,0,0,0
};
#elif CASE==3
char IP[N] = {
	2,1,0,0,0,0,0,
	0,0,0,0,2,3,M,
	0,1,0,0,0,0,0,
	0,M,0,0,0,0,3
};
char SS[N] = {
	0,0,0,0,3,1,0,
	0,0,0,0,0,2,M,
	2,3,0,0,0,0,0,
	1,M,0,0,0,0,0
};
#elif CASE==4
char IP[N]= {
	0,0,0,3,2,0,0,
	M,0,3,0,0,0,0,
	0,1,0,0,M,0,0,
	0,0,0,1,0,2,0,
};
char SS[N]= {
	0,0,3,0,0,0,0,
	M,2,1,0,0,0,0,
	0,0,0,1,M,0,0,
	0,0,0,2,3,0,0
};
#elif CASE==5
char IP[N]= {
	0,0,0,3,M,1,0,
	3,0,0,0,0,0,0,
	0,0,2,0,0,0,0,
	0,M,0,2,0,1,0
};
char SS[N]= {
	1,3,0,3,M,0,0,
	0,2,0,1,2,0,0,
	0,0,0,0,0,0,0,
	0,M,0,0,0,0,0
};
#elif CASE==6
char IP[N]= {
	3,0,0,0,0,0,0,
	0,0,1,0,M,2,0,
	0,2,0,3,0,0,0,
	0,0,M,0,0,1,0
};
char SS[N]= {
	0,0,0,0,2,1,0,
	0,0,0,0,M,3,0,
	0,0,1,3,0,0,0,
	0,0,M,2,0,0,0
};
#elif CASE==7
char IP[N] = {
	1,0,0,0,0,0,0,
	0,0,0,0,0,M,0,
	0,0,3,0,0,2,0,
	1,M,0,2,0,3,0
};
char SS[N] = {
	0,0,0,0,2,1,0,
	0,0,0,0,3,M,0,
	0,0,1,3,0,0,0,
	0,M,0,2,0,0,0
};
#elif CASE==8
char IP[N] = {
	0,0,0,0,0,0,0,
	0,3,0,1,M,0,0,
	0,0,M,0,0,0,1,
	2,0,0,2,3,0,0
};
char SS[N] = {
	0,0,0,0,2,1,0,
	0,0,0,0,M,3,0,
	0,1,M,0,0,0,0,
	0,3,2,0,0,0,0
};
#elif CASE==9
char IP[N] = {
	0,0,0,3,0,0,1,
	0,0,0,0,M,2,0,
	0,3,M,0,0,0,0,
	0,1,2,0,0,0,0
};
char SS[N] = {
	0,0,0,0,1,2,0,
	0,0,0,0,M,3,0,
	0,0,M,2,0,0,0,
	0,0,1,3,0,0,0
};
#endif

// temporary
char tIP[12], tSS[12], MidSS[4];
char TargetColor, TargetNode, TargetStep;

/*
	0	1	2	3	4	5	6
	7	8	9	10	11	12	13
	14	15	16	17	18	19	20
	21	22	23	24	25	26	27
*/

// SortNode
char SNode[N]={
	0,3,6,21,24,27,
	
	7,14,10,17,13,20,

	8,9,12,11,
	15,16,19,18,

	1,2,5,4,
	22,23,26,25
};

char enqueue(char arr[]) {
	if(rear<MAX) {
		queue[rear][0]=arr[0];
		queue[rear][1]=arr[1];
		rear++;
		return 1;
	}
	return 0;
}
char *dequeue(){
	if(front<rear) return queue[front++];
	return NULL;
}
void push(char x, char y) {
	stack[step_cnt][0]=x;
	stack[step_cnt][1]=y;
	step_cnt++;
}
void swap(char *a, char *b) {
	char tmp=*a;
	*a=*b;
	*b=tmp;
}
void copy(char a[], char b[]) {
	for(int i=0; i<N; i++) a[i]=b[i];
}
char Change(char value, char mode) {
	if(mode==0) return value/Y;
	if(mode==1) return value%Y;
	return 0;
}
void Pose_init(char d) {
	if(d==0) pose[0]--;
	if(d==1) pose[1]++;
	if(d==2) pose[0]++;
	if(d==3) pose[1]--;
}
char Find_Node(char x, char y) {
	return x*Y+y;
}
char safe(char x, char y) {
	return 0<=x && x<X && 0<=y && y<Y;
}
void Link(int a, char mode) {
	if(a>0) Link(parent[a],mode);
	else {
		copy(temp_IP,IP);
		return;
	}
	if(mode) push(queue[a][0],queue[a][1]);
	swap(&temp_IP[queue[a][0]],&temp_IP[queue[a][1]]);
}
void Path_S(int i) {
	if(i<step_cnt && stack[i][1]==stack[i+1][0]) {
		Path_cnt++;
		Path_S(i+1);
	}
}
char same(char a[], char b[]) {
	for(int i=0; i<N; i++) if(a[i]!=b[i] && b[i]) return 0;
	return 1;
}
char s_same(char a[]) {
	for(int i=0; i<rear; i++) {
		Link(i,0);
		if(same(temp_IP,a)) return 1;
	}
	return 0;
}

// Debug
void draw(char a[]){
	for (int i = 0; i < N; i++) {
		printf("%4d", a[i]);
		if (Change(i, 1) == 6) printf("\n");
	}
	printf("\n");
}

void change_to_4x4(char mode, char dir) {
	char cnt=0;
	if(mode==0) {
		for(int i=0; i<N; i++) {
			if((!dir && Change(i,1)>3) || (dir && Change(i,1)<3)) {
				tIP[cnt]=IP[i];
				tSS[cnt++]=SS[i];
				IP[i]=SS[i]=M;
			}
		}
	}
	else {
		for(int i=0; i<N; i++) {
			if((!dir && Change(i,1)>3) || (dir && Change(i,1)<3)) {
				IP[i]=tIP[cnt];
				SS[i]=tSS[cnt++];
			}
		}
	}
}
char BFS_is_partsame(char a[], char mode) {
	// TargetStep Row안에 퍽이 들어온다면
	if(mode==1){
		for(int i=0;i<N;i++){
			if(a[i]==TargetColor && abs(Change(TargetNode,1)-Change(i,1))<TargetStep){
				copy(IP,a);
				return 1;
			}
		}
	}
	// 목표 노드가 정답과 일치한다면
	// M 처리하고 탈출
	else if(mode==2){
		if(a[TargetNode]==SS[TargetNode]){
			a[TargetNode]=SS[TargetNode]=M;
			copy(IP,a);
			return 1;
		}
	}
	return 0;
}
char BFS(char mode) {
	char a[N], b[N], *p;
	char point[2]= {0,};

	front=rear=0;
	enqueue(point);
	while(1) {
		p=dequeue();
		if(p==NULL) return 0;
		Link(front-1,0);
		copy(a,temp_IP);

		if(BFS_is_partsame(a,mode)) return 1;

		for(int i=0, j=0; i<N; i++) {
			if(a[i] && a[i]!=M){
				for(int D=0; D<4; D++) {
					pose[0]=Change(i,0);
					pose[1]=Change(i,1);
					Pose_init(D);
					j=Find_Node(pose[0],pose[1]);
					if(safe(pose[0],pose[1]) && a[j]==0) {
						copy(b,a);
						swap(&b[i],&b[j]);
						if(s_same(b)==0) {
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
	}
	return 0;
}
char BFS_run(char mode) {
	char state=BFS(mode);
	printf("mode=%d, state=%d, rear=%d\n",mode,state,rear);
	if(state==1) Link(rear-1,1);
	return state;
}
void solve() {
	char cnt;
	for(int i=0;i<3;i++){
		TargetColor=i+1;

		cnt=0;
		for(int j=0;j<N;j++) if(Change(j,1)>3 && IP[j]==TargetColor) cnt++;
		if(cnt>1) TargetNode=0;
		else if(cnt<1) TargetNode=6;
		if(cnt!=1){
			for (int j=0;j<4;j++) {
				TargetStep=5-j;
				BFS_run(1);
			}
		}

		cnt=0;
		for(int j=0;j<N;j++) if(Change(j,1)<3 && IP[j]==TargetColor) cnt++;
		if(cnt>1) TargetNode=6;
		else if(cnt<1) TargetNode=0;
		if(cnt!=1){
			for (int j=0;j<4;j++) {
				TargetStep=5-j;
				BFS_run(1);
			}
		}
	}
	char dir=0;
	for(int i=0;i<4;i++) if(SS[i*7+3] && SS[i*7+3]!=M && SS[i*7+2] && SS[i*7+2]!=M) dir=1;
	for(int i=0;i<4;i++) if(SS[i*7+3] && SS[i*7+3]!=M) MidSS[i]=SS[i*7+3], SS[i*7+3]=0;
	for(int i=0;i<2;i++){
		change_to_4x4(0,abs(i-dir));
		for(int j=0;j<N;j++) {
			TargetNode=SNode[j];
			if(SS[TargetNode] && SS[TargetNode]!=M){
				TargetColor=SS[TargetNode];
				BFS_run(2);
			}
		}
		change_to_4x4(1,abs(i-dir));
		for(int i=0;i<4;i++) if(MidSS[i]) SS[i*7+3]=MidSS[i];
	}
}
char findway(char fIP[], char from, char to){
	if(from==to) return 1;
	char a[N], *p, point[2]={0,};
	copy(a,fIP);
	a[to]=0;
	front=rear=0;
	point[0]=from;
	point[1]=0;
	enqueue(point);
	while(1){
		p=dequeue();
		if(p==NULL) return 0;
		if(p[0]==to) break;
		for(int D=0,j=0;D<4;D++){
			pose[0]=Change(p[0],0);
			pose[1]=Change(p[0],1);
			Pose_init(D);
			j=Find_Node(pose[0],pose[1]);
			if(safe(pose[0],pose[1]) && !a[j]){
				a[j]=1;
				parent[rear]=front-1;
				point[0]=j;
				point[1]=D;
				enqueue(point);
			}
		}
	}
	char path[N], cnt=0;
	for(int i=front-1;i>0;i=parent[i]) path[cnt++]=queue[i][1];
	for(int i=0;i<cnt/2;i++) swap(&path[i],&path[cnt-i-1]);
	for(int i=0;i<cnt;i++) move_dir[move_cnt++]=path[i];
	return 1;
}

int main() {
	char move_arr[N], first_SS[N];
	copy(move_arr,IP);
	copy(first_SS,SS);

	solve();

	char flg=0;
	for(int i=0; i<step_cnt; i++) {
		Path_cnt=0;
		Path_S(i);

		swap(&move_arr[stack[i][0]],&move_arr[stack[i+Path_cnt][1]]);
		
		printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
		for(int i=0;i<N;i++){
			printf("%4d",move_arr[i]);
			if(Change(i,1)==6) printf("\n");
		}
		printf("\n");

		// 막혀서 로봇이 못 지나갈 경우 flg=1 
		if(i+Path_cnt+1<step_cnt && !findway(move_arr,stack[i][0],stack[i+Path_cnt+1][0])){
			printf("%d,%d fail\n",stack[i][0],stack[i+Path_cnt+1][0]);
			flg=1;
		}

		i+=Path_cnt;
	}
	printf("step_cnt: %d\n",step_cnt);
	if(same(move_arr,first_SS)) printf("Success!!");
	else printf("fail");
	printf(" flg=%d\n",flg);
}