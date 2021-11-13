#include <stdio.h>

#define X 3
#define Y 3
#define N X*Y
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N]={
	3,0,1,
	2,0,3,
	1,2,2
};
char SS[N]={
	2,0,2,
	1,0,1,
	3,0,3
};
char CornerNode[4]={0,2,6,8};

char enqueue(char arr[]){
	if(rear<MAX){
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
	if(stack[i][1]==stack[i+1][0]){
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

void draw(char a[]){
	for(int i=0;i<N;i++){
		printf("%3d",a[i]);
		if(Change(i,1)==2) printf("\n");
	}
	printf("\n");
}
char BFS_is_partsame(char a[], char mode){
	if(mode==1){
		for(int i=0;i<4;i++){
			if(
				SS[CornerNode[i]] &&
				SS[CornerNode[i]]!=M &&
				a[CornerNode[i]]==SS[CornerNode[i]]
			){
				a[CornerNode[i]]=SS[CornerNode[i]]=M;
				copy(IP,a);
				return 1;
			}
		}
	}
	else if(mode==2){
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
		for(int i=0,j=0;i<N;i++){
			if(a[i] && a[i]!=M){
				for(int D=0;D<4;D++){
					pose[0]=Change(i,0);
					pose[1]=Change(i,1);
					Pose_init(D);
					j=Find_Node(pose[0],pose[1]);
					if(safe(pose[0],pose[1]) && a[j]==0){
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
	for(int i=0;i<4;i++) BFS_run(1);
	BFS_run(2);
}
int main(){
	char move_arr[N];
	char oSS[N];
	copy(oSS,SS);
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
	printf("step_cnt=%d\n",step_cnt);

	if(same(move_arr,oSS)) printf("Success!!");
	else printf("fail");
}
