#include <stdio.h>

#define N 16
#define MAX 580
#define M 99

char X, Y;

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N], SS[N];

char IP1[N]={
	2,3,1,0,
	2,3,1,0,
};
char SS1[N]={
	1,3,2,0,
	1,3,2,0,
};
char IP2[N]={
	0,0,0,0,
	0,0,0,0,
	0,0,M,0,
	1,3,0,2,
};
char SS2[N]={
	0,1,0,0,
	3,0,2,0,
	0,0,M,0,
	0,0,0,0,
};

char CornerNode[4]={0,3,4,7};
char BFS_EXIT;
char BN_value[4][2], BN_cnt;

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
	for(int i=0;i<X*Y;i++){
		printf("%4d",a[i]);
		if(Change(i,1)==Y-1) printf("\n");
	}
	printf("\n");
}

char BFS_is_partsame(char a[], char mode){
	if(mode==1){
		for(int i=0;i<4;i++){
			if(SS[CornerNode[i]] && SS[CornerNode[i]]!=M && a[CornerNode[i]]==SS[CornerNode[i]]){
				BN_value[BN_cnt][0]=CornerNode[i];
				BN_value[BN_cnt++][1]=a[CornerNode[i]];
				a[CornerNode[i]]=SS[CornerNode[i]]=M;
				copy(IP,a);
				return 1;
			}
		}
		if(same(a,SS)){
			BFS_EXIT=1;
			copy(IP,a);
			return 1;
		}
	}
	else if(mode==2){
		
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
char BFS2(char mode){
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
	char state;
	if(mode==1) state=BFS(mode);
	else state=BFS2(mode);
	if(state==1) Link(rear-1,1);
	printf("mode=%d, state=%d, rear=%d\n",mode,state,rear);
	return state;
}
void solve(char mode){
	if(mode==0){
		BFS_EXIT=0;
		while(!BFS_EXIT) BFS_run(1);
		for(int i=0;i<BN_cnt;i++) IP[BN_value[i][0]]=BN_value[i][1];
		copy(IP1,IP);
	}
	else{

	}
}
int main(){
	char fIP[N], fSS[N];
	for(int i=0;i<2;i++){
		if(i==0){
			X=2, Y=4;
			copy(IP,IP1);
			copy(SS,SS1);
		}
		else{
			X=4, Y=4;
			copy(IP,IP2);
			copy(SS,SS2);
		}

		copy(fIP,IP);
		copy(fSS,SS);
		step_cnt=0;
		solve(i);
		for(int i=0;i<step_cnt;i++){
			Path_cnt=0;
			Path_S(i);
			printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
			swap(&fIP[stack[i][0]],&fIP[stack[i+Path_cnt][1]]);
			draw(fIP);
			i+=Path_cnt;
		}
		printf("step_cnt=%d\n",step_cnt);
		if(same(fIP,fSS)) printf("Success!!");
		else printf("fail");
		printf("\n\n");
	}
}
