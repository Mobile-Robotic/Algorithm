#include <stdio.h>

#define X 3
#define Y 4
#define N 12
#define MAX 560
#define M 99

char fIP[N]={
    0,0,0,M,
    0,0,0,0,
    M,0,0,0,
};
char gps[2];
char move_dir[N], move_cnt; 

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N];
char SS[N];

void enqueue(char arr[]){
	if(rear<MAX){
		queue[rear][0]=arr[0];
		queue[rear][1]=arr[1];
		rear++;
	}
}
char *dequeue(){
	if(front<rear) return queue[front++];
	return NULL;
}
void push(char x, char y){
	stack[step_cnt][0]=x;
	stack[step_cnt][1]=y;
	step_cnt++;
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
void Path_S(int i){
	if(stack[i][1]==stack[i+1][0]){
		Path_cnt++;
		Path_S(i+1);
	}
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
char same(char a[], char b[]){
	for(int i=0;i<N;i++) if(a[i]!=b[i]) return 0;
	return 1;
}
char s_same(char a[]){
	for(int i=0;i<rear;i++){
		Link(i,0);
		if(same(temp_IP,a)) return 1;
	}
	return 0;
}
char findway(char fIP[], char from, char to){
	if(from==to) return 1;
	char a[N], *p, point[2]={0,};
	
	copy(a,fIP);
	point[0]=from;
	point[1]=0;
	
	front=rear=0;
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

int main(){
	findway(fIP,0,11);
	for(int i=0;i<move_cnt;i++){
		printf("%2d",move_dir[i]);
	}
}
