#include <stdio.h>
#include <math.h>

#define X 3
#define Y 6
#define N X*Y
#define MAX 580
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char TargetNode, TargetColor, TargetCnt;

char SortDir;
char Holeflg[N];

// BeforeNode
char BN_value[6][2], BN_cnt;

// SortNode
char SNode[2][2]={
	{1,13},
	{4,16},
};

/*
	0	1	2	3	4	5
	6	7	8	9	10	11
	12	13	14	15	16	17
*/

#define CASE 1

#if CASE==1
char IP[N]={
	0,0,0,0,0,0,
	3,1,2,3,2,1,
	0,0,0,0,0,0,
};
char SS[N]={
	1,2,3,0,3,0,
	0,M,0,M,0,M,
	0,1,3,1,0,2,
};
#elif CASE==2
char IP[N]={
	0,0,0,0,0,0,
	3,3,2,2,1,1,
	0,0,0,0,0,0,
};
char SS[N]={
	1,1,3,0,3,0,
	0,M,0,M,0,M,
	0,2,2,1,0,3,
};
#elif CASE==3
char IP[N]={
	0,0,0,0,0,0,
	3,3,2,2,1,1,
	0,0,0,0,0,0,
};
char SS[N]={
	1,1,0,2,3,0,
	0,M,M,0,M,0,
	0,1,0,2,3,3,
};
#elif CASE==4
char IP[N]={
	0,0,0,0,0,0,
	1,2,1,3,2,3,
	0,0,0,0,0,0,
};
char SS[N]={
	3,1,0,1,2,0,
	0,M,0,M,M,0,
	0,2,0,2,3,3,
};
#elif CASE==5
char IP[N]={
	0,0,0,0,0,0,
	3,3,3,2,1,1,
	0,0,0,0,0,0,
};
char SS[N]={
	3,3,0,0,1,0,
	0,M,0,M,0,M,
	0,1,1,1,2,3,
};
#endif

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
		if(Change(i,1)==Y-1) printf("\n");
	}
	printf("\n");
}
char BFS_is_partsame(char a[], char mode){
	for(int i=0;i<N;i++) if(mode!=1 && ((Holeflg[i] && 6<=i && i<12) || !(6<=i && i<12)) && !SS[i] && a[i] && a[i]!=M) return 0;
	char cnt=0;
	if(mode==1){
		for(int i=TargetNode;i<TargetNode+3;i++) if(SS[i] && SS[i]!=M && SS[i]==a[i]) cnt++;
		if(cnt>=TargetCnt){
			copy(IP,a);
			return 1;
		}
	}
	else if(mode==2){
		// 가운데 노드에 퍽이 없거나 장애물일때만 탈출
		if(a[TargetNode]==TargetColor && (!a[Find_Node(1,Change(TargetNode,1))] || a[Find_Node(1,Change(TargetNode,1))]==M)){
			copy(IP,a);
			return 1;
		}
	}
	else if(mode==3){
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

		forst=0;
		foren=N;
		for(int i=0;i<N;i++){
			if(
				mode!=1 && 
				((Holeflg[i] && 6<=i && i<12) || !(6<=i && i<12)) &&
				!SS[i] && a[i] && a[i]!=M
				){
				forst=i;
				foren=i+1;
				break;
			}
		}
		for(int i=forst,j=0;i<foren;i++){
			if(a[i] && a[i]!=M){
				if(Change(i,0)==1 && (a[Find_Node(0,Change(i,1))] || a[Find_Node(2,Change(i,1))])) continue;
				for(int D=0;D<4;D++){
					if(Change(i,0)==1 && D%2) continue;
					pose[0]=Change(i,0);
					pose[1]=Change(i,1);
					Pose_init(D);
					j=Find_Node(pose[0],pose[1]);
					if(safe(pose[0],pose[1]) && a[j]==0 && SS[j]!=M){
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
	for(int i=6;i<12;i++){
		if(!IP[i]) Holeflg[i]=1;
		else Holeflg[i]=0;
	}
	char state=BFS(mode);
	if(state==1) Link(rear-1,1);
	printf("mode=%d, state=%d, rear=%d\n",mode,state,rear);
	return state;
}
void solve(){
	char cnt, flg;

	// 0 1 2 노드와 6 7 8 퍽의 색이 동일 할 때 바로 처리
	cnt=0;
	for(int i=0;i<3;i++){
		if(IP[i+6]==SS[i]) cnt++;
		if(cnt<i+1) break;
	}
	if(cnt>2){
		TargetCnt=cnt;
		TargetNode=0;
		BFS_run(1);
		for(int i=0;i<N;i++) if(SS[i] && SS[i]!=M && SS[i]==IP[i]) IP[Find_Node(1,Change(i,1))]=SS[Find_Node(1,Change(i,1))]=IP[i]=SS[i]=M;
		if(BFS_run(3)) return;
	}

	// 15 16 17 노드와 9 10 11 퍽의 색이 동일 할 때 바로 처리
	cnt=0;
	for(int i=0;i<3;i++){
		if(IP[11-i]==SS[17-i]) cnt++;
		if(cnt<i+1) break;
	}
	if(cnt>2){
		TargetCnt=cnt;
		TargetNode=15;
		BFS_run(1);
		for(int i=0;i<N;i++) if(SS[i] && SS[i]!=M && SS[i]==IP[i]) IP[Find_Node(1,Change(i,1))]=SS[Find_Node(1,Change(i,1))]=IP[i]=SS[i]=M;
		if(BFS_run(3)) return;
	}

	// 코너 2곳 처리
	for(int i=0;i<2;i++){
		TargetNode=i*17;
		TargetColor=SS[TargetNode];
		if(IP[TargetNode]!=M && TargetColor && TargetColor!=M){
			BN_cnt=0;
			flg=0;
			for(int j=6;j<12;j++){
				if(!flg && IP[j]==TargetColor) {flg=1; continue;}
				if(Change(TargetNode,1)!=Change(j,1)){
					BN_value[BN_cnt][0]=j;
					BN_value[BN_cnt++][1]=IP[j];
					IP[j]=M;
				}
			}
			BFS_run(2);
			for(int j=0;j<BN_cnt;j++) IP[BN_value[j][0]]=BN_value[j][1];
		}
		// 코너 완전히 막기
		for(int j=0;j<3;j++) IP[Find_Node(j,Change(TargetNode,1))]=M;
	}

	// 1,13,4,16 노드 처리
	char color[3]={0,};
	for(int i=0;i<N;i++) if(IP[i]!=M && IP[i]) color[IP[i]-1]++;
	for(int i=0;i<2;i++){
		// 맞추고자 하는 Node가 0 or M or 정답이 존재하면서 맞춰야 할 색이 부족하다면
		// 반대쪽부터 맞춤
		if(!SS[SNode[0][i]] || SS[SNode[0][i]]==M || (SS[SNode[0][i]] && SS[SNode[0][i]]!=M && color[SS[SNode[0][i]]-1]<1)){
			SortDir=1;
			break;
		}
	}

	for(int i=0;i<2;i++){
		TargetNode=SNode[SortDir][i];
		TargetColor=SS[TargetNode];
		flg=0;
		for(int j=0;j<N;j++) if(IP[j]!=M && IP[j]==TargetColor) flg=1;
		if(flg && TargetColor && TargetColor!=M){
			BFS_run(2);
			IP[TargetNode]=SS[TargetNode]=M;
		}
	}

	// 최종 BFS
	BFS_run(3);
}
int main(){
	char fIP[N], fSS[N];
	copy(fIP,IP);
	copy(fSS,SS);
	solve();
	for(int i=0;i<step_cnt;i++){
		Path_cnt=0;
		Path_S(i);
		printf("from %d to %d\n",stack[i][0],stack[i+Path_cnt][1]);
		swap(&fIP[stack[i][0]],&fIP[stack[i+Path_cnt][1]]);
		draw(fIP);
		i+=Path_cnt;
	}
	printf("step_cnt=%d\n",step_cnt);
	char cnt=0;
	for(int i=0;i<N;i++) if(fSS[i] && fSS[i]!=M && fSS[i]==fIP[i]) cnt++;
	if(cnt==6) printf("Success!!\n");
	else printf("Fail\n");
}