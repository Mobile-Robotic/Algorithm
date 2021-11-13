#include <stdio.h>

#define N 10
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char HoleNode=0;
char IP[N];
char SS[N];

char g[N][N];

char SortDir;

// SortNode
char SNode[2][3]={
	{1,0,2},
	{8,9}
};

// BeforeNodeValue
char BN_value;
char TargetNode, TargetColor;

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

void draw(char a[]){
	for(int i=0;i<N;i++){
		if(i==3 || i==8) printf("  ");
		printf("%4d",a[i]);
		if(i==2 || i==4 || i==7 || i==9) printf("\n");
	}
	printf("\n");
}
char BFS_is_partsame(char a[], char mode){
	if(a[HoleNode] && a[HoleNode]!=M) return 0;
	char cnt=0;
	if(mode==1){
		if(a[HoleNode]==0){
			copy(IP,a);
			return 1;
		}
	}
	else if(mode==2){
		for(int i=0;i<2;i++) if(SNode[!SortDir][i]!=HoleNode && !a[SNode[!SortDir][i]]) return 0;
		if(!a[TargetNode]){
			for(int i=0;i<N;i++){
				if(g[TargetNode][i] && a[i]==TargetColor){
					copy(IP,a);
					return 1;
				}
			}
		}
	}
	else if(mode==3){
		if(a[TargetNode]==SS[TargetNode]){
			copy(IP,a);
			return 1;
		}
	}
	else if(mode==4){
		for(int i=0;i<N;i++) if(a[i] && a[i]!=M && a[i]!=SS[i]) return 0;
		copy(IP,a);
		return 1;
	}
	return 0;
}
char BFS(char mode){
	char a[N], b[N], *p;
	char point[2]={0,};

	char forst, foren;

	front=rear=0;
	enqueue(point);
	while(1){
		p=dequeue();
		if(p==NULL) return 0;
		Link(front-1,0);
		copy(a,temp_IP);
		if(BFS_is_partsame(a,mode)) return 1;
		if(a[HoleNode] && a[HoleNode]!=M && mode!=1){
			forst=HoleNode;
			foren=HoleNode+1;
		}
		else{
			forst=0;
			foren=N;
		}
		for(int i=forst;i<foren;i++){
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
void conn(char st, char en){
	g[st][en]=g[en][st]=1;
}
void solve(){
	for(int i=0;i<2;i++) conn(i,i+1);
	conn(3,4);
	for(int i=5;i<7;i++) conn(i,i+1);
	conn(8,9);

	conn(0,3);
	conn(1,3);
	conn(1,4);
	conn(2,4);

	conn(3,5);
	conn(3,6);
	conn(4,6);
	conn(4,7);

	conn(5,8);
	conn(6,8);
	conn(6,9);
	conn(7,9);

	if(IP[HoleNode] && IP[HoleNode]==SS[HoleNode]) IP[HoleNode]=SS[HoleNode]=M;
	if(IP[HoleNode] && IP[HoleNode]!=M) BFS_run(1);

	char cnt=0, flg;
	for(int i=0;i<3;i++) if(SNode[0][i]==HoleNode || IP[SNode[0][i]]==M) cnt++;
	if(cnt>=2) SortDir=1;
	else SortDir=0;

	for(int i=0;i<3;i++){
		TargetNode=SNode[SortDir][i];
		TargetColor=SS[TargetNode];
		flg=0;
		for(int j=0;j<N;j++) if(IP[j]!=M && IP[j]==TargetColor) flg=1;
		if(flg && TargetNode!=HoleNode && IP[TargetNode]!=M){
			BFS_run(2);
			if(BFS_run(3)){
				if(TargetNode==1) BN_value=IP[1];
				IP[TargetNode]=SS[TargetNode]=M;
			}
		}
		if(SortDir && i==1) break;
	}
	if(!BFS_run(4)){
		IP[1]=SS[1]=BN_value;
		BFS_run(4);
	}
}
//===========================================================
char gps[2];
char move_dir[N], move_cnt; 

char get_dir(int from, int to){
	if(from-to==3) return 0;
	if(from-to==2) return 1;
	if(from-to==-1) return 2;
	if(from-to==-3) return 3;
	if(from-to==-2) return 4;
	if(from-to==1) return 5;
	return 99;
}
char findway(char fIP[], char from, char to){
	if(from==to) return 1;
	char a[N], *p, point[2]={0,};
	
	copy(a,fIP);
	a[to]=0;
	point[0]=from;
	point[1]=0;
	
	front=rear=0;
	enqueue(point);
	while(1){
		p=dequeue();
		if(p==NULL) return 0;
		if(p[0]==to) break;
		for(int j=0;j<N;j++){
			if(g[p[0]][j] && !a[j]){
				a[j]=1;
				parent[rear]=front-1;
				point[0]=j;
				point[1]=get_dir(p[0],j);
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
char set_dir(int set){	// set¿¡ ³ÖÀº ¹æÇâÀ¸·Î ¸ÂÃçÁÜ 
	if(gps[1]==set) return 0;
	int w=set-gps[1];
	if(w>3) w-=6;
	if(w<-3) w+=6;
	printf("%d, %d -> %d\n",gps[1],set,w);
	////turn((w<0 ? -1:1),200,11,90*fabs(w),0,0);
	//gps[1]=set;
	//return 1;
}
char get_node(char from, char dir){
	if(dir==0) return from-3;
	if(dir==1) return from-2;
	if(dir==2) return from+1;
	if(dir==3) return from+3;
	if(dir==4) return from+2;
	if(dir==5) return from-1;
	return 99;
}

int main(){
	for(int i=0;i<2;i++) conn(i,i+1);
	conn(3,4);
	for(int i=5;i<7;i++) conn(i,i+1);
	conn(8,9);
	
	conn(0,3);
	conn(1,3);
	conn(1,4);
	conn(2,4);

	conn(3,5);
	conn(3,6);
	conn(4,6);
	conn(4,7);

	conn(5,8);
	conn(6,8);
	conn(6,9);
	conn(7,9);

	char IP1[N]={
		3,3,3,
		2,0,
		0,M,0,
		1,1,
	};
	char SS1[N]={
		2,1,1,
		1,1,
		2,2,3,
		3,3,
	};
	copy(IP,IP1);
	copy(SS,SS1);

	/*char IP1[N]={
		0,1,2,
		 2,3,
		0,0,M,
		 3,1,
	};
	char SS1[N]={
		2,3,1,
		 1,3,
		3,2,1,
		 2,1,
	};
	copy(IP,IP1);
	copy(SS,SS1);*/

	solve();

	copy(IP,IP1);
	for(int i=0;i<step_cnt;i++){
		Path_cnt=0;
		Path_S(i);

		/*//debug
		dc(0,0,stack[i][0]);
		dc(0,4,stack[i+Path_cnt][1]);
		Hold();*/

		move_cnt=0;
		if(stack[i][0]!=stack[i+Path_cnt][1]){
			findway(IP,gps[0],stack[i][0]);
			findway(IP,stack[i][0],stack[i+Path_cnt][1]);

			//printf("gps %d, %d\n",gps[0],gps[1]);
			printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
			for(int i=0;i<move_cnt;i++) printf("%d ",move_dir[i]);
			printf("\n");
			swap(&IP[stack[i][0]],&IP[stack[i+Path_cnt][1]]);
			draw(IP);

			for(int i=0;i<move_cnt-1;i++){
				gps[0]=get_node(gps[0],move_dir[i]);
				gps[1]=move_dir[i+1];
			}
		}
		i+=Path_cnt;
	}
	
	/*findway(IP,0,3);
	findway(IP,3,5);
	for(int i=0;i<move_cnt;i++){
		printf("%2d",move_dir[i]);
	}

	printf("\n\n");
	for(int i=0;i<6;i++){
		gps[1]=i;
		for(int j=0;j<6;j++){
			set_dir(j);
		}
	}*/
}
