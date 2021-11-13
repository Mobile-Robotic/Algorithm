#include <stdio.h>

#define X 8
#define Y 4
#define N X*Y
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char fIP[N];

char sheet[3]={1,2,3};
char IP[N]={
	1,1,1,0,
	3,2,3,0,
	0,2,3,2,
	3,0,0,2,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
};
char SS[N];

char gate[4][2]={
	{0,0},
	{0,1},
	{1,2},
	{2,2}
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
	if(i<step_cnt-1 && stack[i][1]==stack[i+1][0]){
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
		if(Change(i,1)==Y-1) printf("\n");
	}
	printf("\n");
}
/*
	0	1	2	3
	4	5	6	7
	8	9	10	11
	12	13	14	15

	16	17	18	19
	20	21	22	23
	24	25	26	27
	28	29	30	31
*/

// Findway
/*
	gps[0] -> 로봇의 위치 (노드)(0~N)
	gps[1] -> 로봇이 바라보는 방향 (0,1,2,3)
*/
char gps[2];	
char move_dir[N], move_cnt;	// move_dir에 이동 방향이 저장됨 (0,1,2,3) 

char findway(char mode, char from, char to){
	if(from==to) return 1;
	char a[N], *p, point[2]={0,};
	int i;

	copy(a,IP);
	a[to]=0;
	point[0]=from;
	point[1]=0;

	front=rear=0;
	enqueue(point);
	while(1){
		p=dequeue();
		if(p==NULL) return 0;
		i=p[0];
		if(i==to) break;
		for(int D=0,j=0;D<4;D++){
			pose[0]=Change(i,0);
			pose[1]=Change(i,1);
			Pose_init(D);
			j=Find_Node(pose[0],pose[1]);
			if(safe(pose[0],pose[1]) && !a[j]){
				if(a[i]) a[j]=a[i];
				else a[j]=M;
				
				if(
					mode &&
					((Change(i,0)==3 && D==2) || (Change(i,0)==4 && D==0)) &&
					(a[i]!=sheet[gate[Change(i,1)][0]] && a[i]!=sheet[gate[Change(i,1)][1]])
				) {
					a[j]=0;
					continue;
				}

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

char run(char from, char to){
	if(from==to) return 1;
	move_cnt=0;
	if(findway(0,gps[0],from) && findway(1,from,to)){
		push(from,to);
		swap(&IP[from],&IP[to]);
		gps[0]=from;
		return 1;
	}
	return 0;
}
char sort(char target){
	if(SS[target] && !IP[target]) for(int i=15;i>=0;i--) if(IP[i]==SS[target] && run(i,target)) return 1;
	return 0;
}
void solve(){
	copy(fIP,IP);
	for(int i=0;i<4;i++) for(int j=0;j<4;j++) SS[28-i*4+j]=IP[i*4+j];

	// 초기 좌표
	gps[0]=19;
	gps[1]=3;

	// 12~15 노드 퍽
	// 16,19 노드로 치우기
	for(int i=12;i<16;i++){
		if(IP[i]){
			if(!run(i,16)){
				for(int j=15;j>=12;j--){
					if(IP[j]) {
						run(j,15);
						break;
					}
				}
				run(i,16);
			}
			for(int j=15;j>=12;j--) if(IP[j]) run(j,19);
			break;
		}
	}

	// 4~7 빈칸과 8~11의 빈칸 Y가 다르면
	// 8~11의 빈칸 위치를 바꿈
	for(int i=4;i<8;i++) for(int j=8;j<12;j++) if(!IP[i] && !IP[j] && Change(i,1)!=Change(j,1)) run(Find_Node(2,Change(i,1)),j);

	// 빈칸 위치에 따라 SortDir을 정함
	char SortDir;
	for(int i=8;i<12;i++) if(!IP[i]) SortDir=Change(i,1)>1;

	// 맨끝부터 맞추기
	char start[2][3]={
		{0,4,8},
		{28,24,20}
	};
	char pri[3][4]={
		{0,1,2,3},
		{3,2,1,0},
		{0,3,1,2},
	};
	char flg;
	for(int i=0;i<3;i++){
		flg=0;
		for(int j=0;j<4;j++){
			if(i==2){
				SortDir=2;
				sort(start[1][i]+pri[SortDir][j]);
			}
			else if(SS[start[1][i]+pri[SortDir][j]] && !IP[start[1][i]+pri[SortDir][j]]){
				if(!run(start[0][i]+pri[SortDir][j],start[1][i]+pri[SortDir][j])) flg=1;
			}
		}
		if(flg) for(int j=0;j<4;j++) run(start[0][i]+pri[SortDir][j],start[1][i]+pri[SortDir][j]);
	}

	// 마지막 맞추기
	for(int i=16;i<20;i++){
		if(SS[i]){
			run(16,i);
			break;
		}
	}
	for(int i=19;i>=16;i--){
		if(SS[i]){
			run(19,i);
			break;
		}
	}
}
int main(){
	solve();
	copy(IP,fIP);
	step_cnt=0;
	solve();

	copy(IP,fIP);
	for(int i=0;i<step_cnt;i++){
		Path_cnt=0;
		Path_S(i);
		swap(&IP[stack[i][0]],&IP[stack[i+Path_cnt][1]]);

		printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
		draw(IP);

		i+=Path_cnt;
	}

	if(same(IP,SS)) printf("Success!\n");
	else printf("fail\n");
}
