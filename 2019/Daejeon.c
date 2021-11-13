#include <stdio.h>

#define X 5
#define Y 4
#define N X*Y
#define MAX 580
#define M 99	// 장애물

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char gps[2];
char move_dir[N], move_cnt;

#define CASE 1

#if CASE==1
char IP[N]={
	0,0,0,0,
	0,M,0,0,
	0,1,1,0,
	0,0,1,0,
	M,M,0,0,
};
char SS[N]={
	0,1,0,0,
	1,M,0,0,
	0,0,0,0,
	1,0,0,0,
	M,M,0,0,
};
#elif CASE==2
char IP[N]={
	0,0,0,0,
	0,M,0,0,
	0,1,1,0,
	0,0,1,0,
	M,M,0,0,
};
char SS[N]={
	1,1,1,0,
	0,M,0,0,
	0,0,0,0,
	0,0,0,0,
	M,M,0,0,
};
#elif CASE==3
char IP[N]={
	0,0,0,0,
	0,M,1,0,
	0,1,0,0,
	0,0,1,0,
	0,M,0,0,
};
char SS[N]={
	0,0,0,1,
	1,M,0,0,
	0,0,0,0,
	0,0,0,0,
	1,M,0,0,
};
#endif

char oIP[N], oSS[N], sSS[N];
char MidNode[6]={5,9,13,6,10,14};
char MidBuff[2][5]={
	{1,4,8,12,17},
	{2,7,11,15,18}
};

char TargetNode;
//char SNode[N]={1,2,17,18,4,7,8,11,12,15}

/*
	0	1	2	3
	4	5	6	7
	8	9	10	11
	12	13	14	15
	16	17	18	19
*/

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

char findway(char fIP[], char from, char to){
	if(from==to) return 1;
	int prev_front=front, prev_rear=rear;
	front=rear;
	char a[N], *p, point[2]={0,};
	copy(a,fIP);
	a[to]=0;
	point[0]=from;
	point[1]=0;
	enqueue(point);
	while(1){
		p=dequeue();
		if(p==NULL){
			front=prev_front, rear=prev_rear;
			return 0;
		}
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
	front=prev_front, rear=prev_rear;
	return 1;
}

// debug
void draw(char a[]){
	for(int i=0;i<N;i++){
		printf("%4d",a[i]);
		if(Change(i,1)==3) printf("\n");
	}
	printf("\n");
}
char check(char a[], char i, char D){
	/*if(D==0 && !a[Find_Node(pose[0]+2,pose[1])]) return Find_Node(pose[0]+2,pose[1]);
	if(D==1 && !a[Find_Node(pose[0],pose[1]-2)]) return Find_Node(pose[0],pose[1]-2);
	if(D==2 && !a[Find_Node(pose[0]-2,pose[1])]) return Find_Node(pose[0]-2,pose[1]);
	if(D==3 && !a[Find_Node(pose[0],pose[1]+2)]) return Find_Node(pose[0],pose[1]+2);*/
	char node=M;
	if(D==0 && safe(pose[0]+2,pose[1])) node=Find_Node(pose[0]+2,pose[1]);
	if(D==1 && safe(pose[0],pose[1]-2)) node=Find_Node(pose[0],pose[1]-2);
	if(D==2 && safe(pose[0]-2,pose[1])) node=Find_Node(pose[0]-2,pose[1]);
	if(D==3 && safe(pose[0],pose[1]+2)) node=Find_Node(pose[0],pose[1]+2);
	// && findway(a,i,node)
	if(node!=M && !a[node]) return 1;
	return 0;
}
void shift_ss_to_mid(){
	copy(sSS,oSS);
	for(int i=0;i<6;i++){
		if(!sSS[MidNode[i]]){
			for(int j=0;j<5;j++){
				if(sSS[MidBuff[i/3][j]] && sSS[MidBuff[i/3][j]]!=M){
					swap(&sSS[MidNode[i]],&sSS[MidBuff[i/3][j]]);
				}
			}
		}
	}
	copy(SS,sSS);
	draw(SS);
}
char BFS_is_partsame(char a[], char mode){
	if(mode==1){
		for(int i=0;i<N;i++){
			if(i!=12 && SS[i] && SS[i]!=M && a[i]==SS[i]){
				SS[i]=a[i]=M;
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
		
		for(int i=0, j=0;i<N;i++){
			if(a[i] && a[i]!=M){
				for(int D=0;D<4;D++){
					pose[0]=Change(i,0);
					pose[1]=Change(i,1);
					Pose_init(D);
					j=Find_Node(pose[0],pose[1]);
					if(safe(pose[0],pose[1]) && a[j]==0 && check(a,i,D)){
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
	/*copy(oSS,SS);
	shift_ss_to_mid();*/
	BFS_run(1);
	BFS_run(1);
	BFS_run(1);
	BFS_run(2);
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
		draw(move_arr);

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