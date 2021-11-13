#include <stdio.h>

char DEBUG=1;
#define X 2
#define Y 4
#define N X*Y
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[70][2], pose[2];

char IP[N]={
	0,0,1,1,
	1,1,2,3,
};
char SS[N]={
	1,0,1,0,
	3,1,1,2,
};
char CornerNode[4]={0,3,4,7};
char BFS_EXIT;

char fIP[N], fSS[N];

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

char BFS_is_partsame(char a[], char mode){
	char cnt=0;
	if(mode==1){
		for(int i=0;i<4;i++){
			if(SS[CornerNode[i]] && SS[CornerNode[i]]!=M && a[CornerNode[i]]==SS[CornerNode[i]]){
				a[CornerNode[i]]=SS[CornerNode[i]]=M;
				copy(IP,a);
				return 1;
			}
		}
		for(int i=0;i<4;i++) if(SS[CornerNode[i]]==M) cnt++;
		if(cnt>=2){
			BFS_EXIT=1;
			copy(IP,a);
			return 1;
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
	//printf("mode=%d, state=%d, rear=%d\n",mode,state,rear);
	return state;
}

char gps[2], Getting_point;

char move_dir[N], move_cnt;
char findway(char from, char to){
	if(from==to) return 1;
	char a[N], *p, point[2]={0,};
	copy(a,IP);
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

char stack2[70][2], step_cnt2;
char clear_node[N][2], clear_cnt;

void push2(char x, char y){
	stack2[step_cnt2][0]=x;
	stack2[step_cnt2++][1]=y;
}
char overlap(char num, char from){
	for(int i=0;i<clear_cnt;i++) if(clear_node[i][num]==from) return i;
	return M;
}
void clear_puck(char D, char from, char to){
	if(Change(from,1)==Change(to,1)){
		gps[0]=from;
		return;
	}
	pose[0]=Change(from,0);
	pose[1]=Change(from,1);
	Pose_init(D);
	char j=Find_Node(pose[0],pose[1]);
	if(safe(pose[0],pose[1]) && IP[j]){
		push2(j,from);
		swap(&IP[j],&IP[from]);

		if(DEBUG) printf("clear_puck: from %d to %d\n",j,from);

		clear_node[clear_cnt][0]=from;
		clear_node[clear_cnt++][1]=j;

		clear_puck(Change(gps[0],1)<2 ? 1:3,j,to);
	}
}
void run_clear(char to){
	char D=Change(gps[0],1)<2 ? 1:3;
	for(int i=0;i<N;i++){
		if(!IP[i] && i!=gps[0] && Change(i,0)==Change(gps[0],0)){
			if(Change(i,0)==0) D=2;
			else D=0;
		}
	}
	clear_puck(D,gps[0],to);
}
char back(char i){
	move_cnt=0;
	if(findway(gps[0],clear_node[i][0]) && findway(clear_node[i][0],clear_node[i][1])){
		push2(clear_node[i][0],clear_node[i][1]);
		swap(&IP[clear_node[i][0]],&IP[clear_node[i][1]]);
		gps[0]=clear_node[i][0];

		if(DEBUG) printf("back: from %d to %d\n",clear_node[i][0],clear_node[i][1]);

		for(int j=i;j<clear_cnt-1;j++){
			clear_node[j][0]=clear_node[j+1][0];
			clear_node[j][1]=clear_node[j+1][1];
		}
		clear_cnt--;
		return 1;
	}
	return 0;
}
void all_back(){
	for(int i=clear_cnt-1;i>=0;i--) back(i);
}
void target_back(char i){
	if(!back(i)){
		run_clear(clear_node[i][0]);
		back(i);
	}
}
void solve(){
	BFS_EXIT=0;
	while(!BFS_EXIT) BFS_run(1);
	BFS_run(2);
	
	char rv;
	copy(IP,fIP);
	copy(SS,fSS);
	gps[0]=stack[0][0];
	for(int i=0;i<step_cnt;i++){
		Path_cnt=0;
		Path_S(i);

		all_back();

		move_cnt=0;
		if(!findway(gps[0],stack[i][0])) run_clear(stack[i][0]);

		rv=overlap(1,stack[i][0]);
		if(rv!=M) target_back(rv);
		rv=overlap(0,stack[i+Path_cnt][1]);
		if(rv!=M) target_back(rv);

		push2(stack[i][0],stack[i+Path_cnt][1]);
		swap(&IP[stack[i][0]],&IP[stack[i+Path_cnt][1]]);

		if(DEBUG) {printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]); draw(IP);}

		gps[0]=stack[i][0];
		i+=Path_cnt;
	}
	
	if(DEBUG) for(int i=0;i<clear_cnt;i++) printf("%d, %d\n",clear_node[i][0],clear_node[i][1]);

	all_back();
	
	if(DEBUG) for(int i=0;i<clear_cnt;i++) printf("%d, %d\n",clear_node[i][0],clear_node[i][1]);

	printf("%d\n",step_cnt2);
	step_cnt=step_cnt2;
	for(int i=0;i<step_cnt;i++){
		stack[i][0]=stack2[i][0];
		stack[i][1]=stack2[i][1];
	}

	for(int j=0;j<2;j++){
		step_cnt2=0;
		for(int i=0;i<step_cnt;i++){
			Path_cnt=0;
			Path_S(i);
			if(stack[i][0]!=stack[i+Path_cnt][1]) push2(stack[i][0],stack[i+Path_cnt][1]);
			i+=Path_cnt;
		}

		step_cnt=step_cnt2;
		for(int i=0;i<step_cnt;i++){
			stack[i][0]=stack2[i][0];
			stack[i][1]=stack2[i][1];
		}
	}
}

int main(){
	char cnt=0;
	copy(fIP,IP);
	copy(fSS,SS);
	solve();
	printf("\n\n\n");
	copy(IP,fIP);
	copy(SS,fSS);
	for(int i=0;i<step_cnt;i++){
		Path_cnt=0;
		Path_S(i);
		printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
		swap(&IP[stack[i][0]],&IP[stack[i+Path_cnt][1]]);
		draw(IP);

		// 막혀서 로봇이 못 지나갈 경우
		move_cnt=0;
		if(i+Path_cnt+1<step_cnt && !findway(stack[i][0],stack[i+Path_cnt+1][0])){
			printf("%d,%d fail\n",stack[i][0],stack[i+Path_cnt+1][0]);
			cnt++;
		}
		i+=Path_cnt;
	}
	printf("step_cnt=%d\n",step_cnt);
	if(same(IP,SS)) printf("Success!!\n");
	else printf("fail\n");
	printf("%d\n",cnt);
}
