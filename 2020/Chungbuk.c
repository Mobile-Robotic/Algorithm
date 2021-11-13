#include <stdio.h>

#define N 12
#define MAX 560
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N]={0,2,0,3,1,0,2,1,2,3,M,3}, SS[N]={3,0,2,0,0,1,3,2,3,1,M,2};
char HoleNode=1; // 홀 노드(홀 노드의 SS에는 빈값이여야 합니다)

char oIP[N], oSS[N]; // 원본 IP, SS
char sSS[N]; // Corner로 정답을 밀어넣은 SS
char tIP[3], tSS[3]; // 마지막 라인의 3개의 이전 노드값을 저장: (0, 4, 8) (3, 7, 11)
char TargetNode, TargetColor, TargetStep; // BFS_IsPartSame의 Mode가 1일떄 Target 퍽의 데이터들
char BFS_EXIT; // BFS 종료 여부(중복조건으로 while형태의 BFS를 호출할 떄 사용됩니다)

char BCN_value[2][2], BCN_cnt; // 메모리 최적화를 위해 M or HoleNode의 좌우의 퍽을 복구를 위한 값: index(0: node, 1: color)
char FBS, FBSP; // 마지막 BFS 실패시 휴리스틱을 제외하고 다시 돌릴지 여부를 결정하는 변수

char SortDir; // BFS 검색 우선 순위 방향

// BFS 3X3 Map Node List
char SNode[2][3][3]={
	{
		{0,4,8},
		{1,5,9},
		{2,6,10}
	},
	{
		{3,7,11},
		{2,6,10},
		{1,5,9}
	},
};

char CornerNode[4]={0,3,8,11};
char CornerBuff[4][5]={
	{1,2,4,5,9},
	{2,1,7,6,10},
	{9,10,4,5,1},
	{10,9,7,6,2}
};

char MidNode[2]={4,7};
char MidBuff[2][6]={
	{5,6,1,9,2,10},
	{6,5,2,10,1,9}
};

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
	if(mode==0) return value/4;
	if(mode==1) return value%4;
	return 0;
}
void Pose_init(char d){
	if(d==0) pose[0]--;
	if(d==1) pose[1]++;
	if(d==2) pose[0]++;
	if(d==3) pose[1]--;
}
char Find_Node(char x, char y){
	return x*4+y;
}
char safe(char x, char y){
	return 0<=x && x<=2 && 0<=y && y<=3;
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
float heuristic(char a[]){
	float n_cnt=0, cnt=0, rv_cnt=0;
	
	for(int i=0,j=0;i<N;i++){
		for(int D=0;D<4;D++){
			pose[0]=Change(i,0);
			pose[1]=Change(i,1);
			Pose_init(D);
			j=Find_Node(pose[0],pose[1]);
			if(safe(pose[0],pose[1])){
				n_cnt++;
				if(a[j]==SS[j]) cnt++;
			}
		}
		rv_cnt+=cnt/n_cnt; // (정답 예정 개수 / 길목 개수)
		if(a[i]==SS[i]) rv_cnt++; // 정답 일치 개수
		n_cnt=cnt=0;
	}
	return rv_cnt;
}
void shift_ss_to_corner(){
	copy(sSS,oSS);
	for(int i=0;i<4;i++){
		if(sSS[CornerNode[i]]==0){
			for(int j=0;j<5;j++){
				if(j==0 && sSS[CornerBuff[i][j]]==M){
					j++;
					continue;
				}
				if(j==2 && sSS[CornerBuff[i][j]]==M){
					continue;
				}
				if(sSS[CornerBuff[i][j]] && sSS[CornerBuff[i][j]]!=M){
					swap(&sSS[CornerNode[i]],&sSS[CornerBuff[i][j]]);
					break;
				}
			}
		}
	}
	for(int i=0;i<2;i++){
		if(sSS[MidNode[i]]==0 && MidNode[i]!=HoleNode){
			for(int j=0;j<6;j++){
				if(sSS[MidBuff[i][j]] && sSS[MidBuff[i][j]]!=M){
					swap(&sSS[MidNode[i]],&sSS[MidBuff[i][j]]);
					break;
				}
			}
		}
	}
	copy(SS,sSS);
}
char BFS_priority(){
	char p_cnt[2]={0,};
	
	// 홀 노드가 아니면서 빈공간인거 체크
	for(int i=0;i<2;i++) for(int j=0;j<3;j++) if(SNode[i][0][j]!=HoleNode && SS[SNode[i][0][j]]) p_cnt[i]++;
	
	// 장애물과 홀이 C3, C4, C7, C8에 있는경우
	if(p_cnt[0]==p_cnt[1]){
		// 아래의 시퀀스들은 처음 맞추는게 메모리 소모가 크다는 가정하에 작성됨
		if(IP[1]==M) return 1;
		if(IP[2]==M) return 0;
		if(IP[9]==M) return 1;
		if(IP[10]==M) return 0;
		if(IP[4]==M) return 1;
		if(IP[7]==M) return 0;
	}
	else if(p_cnt[0]<p_cnt[1]) return 1;
	return 0;
}
void change_to_3x3(char mode, char dir){
	// 3X3 맵 막기
	if(mode==0){
		for(int i=0;i<3;i++){
			tIP[i]=IP[SNode[dir][0][i]];
			tSS[i]=SS[SNode[dir][0][i]];
			
			IP[SNode[dir][0][i]]=M;
			SS[SNode[dir][0][i]]=M;
		}
	}
	// 3X3 맵 풀기
	else{
		for(int i=0;i<3;i++){	
			IP[SNode[dir][0][i]]=tIP[i];
			SS[SNode[dir][0][i]]=tSS[i];
		}
	}
}
char is_there_target_puck(){
	// 가장 마지막 노드들은 순서대로 맞춰짐으로 이미 맞춰진 퍽을 대상에 들어가면 안됨
	// 맨 왼쪽 줄
	for(int i=TargetStep-1;i<3;i++) if(IP[SNode[SortDir][0][i]]==TargetColor) return 1;
	for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(IP[SNode[SortDir][i][j]]==TargetColor) return 1;
	return 0;
}
char BFS_is_partsame(char a[], char mode){
	// 홀에 퍽을 놓자 마자 정답과 일차하는 case를 배재하기 위한 로직입니다
	// 홀 노드가 M일때는 해당 구역이 막혀있다는것을 의미함으로 조건을 무시하고 넘어갑니다
	if(a[HoleNode] && a[HoleNode]!=M) return 0;
	char isTargetColor=0;
	char n_cnt=0;
	
	// TargetNode에 퍽이 들어오면 종료이외 해당 퍽들은 공간이 없음으로 막지 않습니다.
	if(mode==1){
		// 장애물 옆에 있는 퍽이 정답이라면 홀딩시키기, 중요한건 셔플된 정답이라는 점이다 함부로 OrigSS를 막아서는 안된다 가짜 정답이기 때문
		for(int i=0;i<3;i++){
			if(SNode[SortDir][1][i]==HoleNode || a[SNode[SortDir][1][i]]==M){
				if(
					SS[SNode[SortDir][0][i]] && // 정답에 값이 존재하고
					SS[SNode[SortDir][0][i]]!=M && // M이 아니며
					SS[SNode[SortDir][0][i]]==a[SNode[SortDir][0][i]] // 정답과 일치한다면
				){
					BCN_value[BCN_cnt][0]=SNode[SortDir][0][i];
					BCN_value[BCN_cnt][1]=a[SNode[SortDir][0][i]];
					BCN_cnt++;
					
					a[SNode[SortDir][0][i]]=M;
					SS[SNode[SortDir][0][i]]=M;
					
					copy(IP,a);
					return 1;
				}
			}
		}
		for(int i=0;i<TargetStep;i++) if(a[SNode[SortDir][0][i]]==SS[SNode[SortDir][0][i]]) n_cnt++;
		if(n_cnt>=TargetStep){
			BFS_EXIT=1;
			copy(IP,a);
			return 1;
		}
	}
	// 반대쪽에서 필요한 퍽을 빼와야합니다. 반대쪽 구역에 빈공간이 2개 남을때 탈출(단, 홀구간은 빈공간으로 볼 수 없음)
	else if(mode==2){
		for(int i=0;i<3;i++) if(tIP[i]==0 && SNode[!SortDir][0][i]!=HoleNode) n_cnt++;
		for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(a[SNode[!SortDir][i][j]]==0 && SNode[!SortDir][i][j]!=HoleNode) n_cnt++;
		if(n_cnt>=2){
			copy(IP,a);
			return 1;
		}
	}
	// 반대쪽구역에서 현재 구역에 퍽을 가져와야합니다. 현재 구역에서 빈공간이 2개가 남고 필요한 퍽이 해당 열에 있을떄 탈출
	else if(mode==3){
		for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(a[SNode[!SortDir][i][j]]==TargetColor) isTargetColor=1;
		for(int i=0;i<3;i++) if(tIP[i]==0 && SNode[SortDir][0][i]!=HoleNode) n_cnt++;
		for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(a[SNode[SortDir][i][j]]==0 && SNode[SortDir][i][j]!=HoleNode) n_cnt++;
		if(n_cnt>=2 && isTargetColor){
			copy(IP,a);
			return 1;
		}
	}
	// 홀이 비어지는 순간 탈출
	else if(mode==4){
		if(a[HoleNode]==0){
			copy(IP,a);
			return 1;
		}
	}
	// 최종정답을 맞출때 구석에 있는 퍽의 경우 M처리하면서 메모리 최적화하고 최종정답이면 BFS 완전 종료 시키는 로직
	else if(mode==5){
		// 모든정답이 일치한다면
		if(same(a,SS)){
			BFS_EXIT=1;
			copy(IP,a);
			return 1;
		}
		// 구석에 정답이 맞다면
		for(int i=0;i<4;i++){
			if(
				SS[CornerNode[i]] && // 정답에 값이 존재하고
				SS[CornerNode[i]]!=M && // M이 아니며
				SS[CornerNode[i]] == a[CornerNode[i]] // 정답과 일치한다면
			){
				SS[CornerNode[i]]=M;
				a[CornerNode[i]]=M;
				
				copy(IP,a);
				return 1;
			}
		}
	}
	// Target으로 맞추려고 하는 퍽이 0 ~ 1 step거리 안으로 들어오면 탈출
	else if(mode==6){
		// Worst Case가 800개 까지 나올수 있음으로 끈어서 돌립니다.
		if(TargetStep==1){
			if(
				a[SNode[SortDir][0][0]]==TargetColor ||
				a[SNode[SortDir][0][1]]==TargetColor ||
				a[SNode[SortDir][1][0]]==TargetColor ||
				a[SNode[SortDir][1][1]]==TargetColor
			){
				copy(IP,a);
				return 1;
			}
		}
		else if(TargetStep==2){
			if(
				a[SNode[SortDir][0][0]]==TargetColor ||
				a[SNode[SortDir][0][1]]==TargetColor ||
				a[SNode[SortDir][0][2]]==TargetColor ||
				a[SNode[SortDir][1][0]]==TargetColor ||
				a[SNode[SortDir][1][1]]==TargetColor ||
				a[SNode[SortDir][1][2]]==TargetColor 
			){
				copy(IP,a);
				return 1;
			}
		}
		else if(TargetStep==3){
			if(
				a[SNode[SortDir][0][1]]==TargetColor ||
				a[SNode[SortDir][0][2]]==TargetColor ||
				a[SNode[SortDir][1][1]]==TargetColor ||
				a[SNode[SortDir][1][2]]==TargetColor 
			){
				copy(IP,a);
				return 1;
			}
		}
	}
	// 처음에 퍽이 대칭이라서 홀노드를 제외한 빈공간을 반드시 2개 만들어주기 위해 반대쪽으로 퍽을 밀으면 탈출
	// 즉 SortDir의 반대반향의 0번째 줄이 모두 퍽으로 가득차면 탈출함
	else if(mode==7){
		for(int i=0;i<3;i++) if(a[SNode[!SortDir][0][i]]) n_cnt++;
		if(n_cnt>=3){
			copy(IP,a);
			return 1;
		}
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
		if(a[HoleNode] && a[HoleNode]!=M && mode!=4){
			forst=HoleNode;
			foren=HoleNode+1;
		}
		else{
			forst=0;
			foren=N;
		}
		for(int i=forst,j=0;i<foren;i++){
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
							if(
								(FBS==1 && mode==5 && heuristic(b)-heuristic(a)>-3) ||
								(FBS==0 || mode!=5)
							){
								parent[rear]=front-1;
								point[0]=i;
								point[1]=j;
								enqueue(point);
								if(BFS_is_partsame(b,mode)) return 1;
							}
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
	return state;
}
void solve(){
	copy(oSS,SS);
	shift_ss_to_corner();
	
	// 홀위에 퍽이 있다면 무조건 치우기
	if(IP[HoleNode]) BFS_run(4);
	
	// BFS 3x3으로 돌릴 우선순위
	SortDir=BFS_priority();
	
	// 0번이 선택되고 0번이 막히고 1번이 3개로 꽉차면 탈출
	change_to_3x3(0,SortDir);
	BFS_run(7);
	change_to_3x3(1,SortDir);
	
	// 원하는 맵을 막을때는 Dir을 반대로
	// 원하는 맵을 탐색할때는 Dir을 그대로
	change_to_3x3(0,!SortDir);
	for(int i=0;i<3;i++){
		TargetStep=i+1;
		TargetNode=SNode[SortDir][0][i];
		TargetColor=SS[TargetNode];
		
		if(TargetNode!=HoleNode && IP[TargetNode]!=SS[TargetNode]){
			if(is_there_target_puck()==0){
				// 반대쪽 맵에서 퍽 뺴올 수 있도록 공간 만들어주기
				BFS_run(2);
				// 반대쪽에서 현재 맵으로 퍽 빼오기
				change_to_3x3(1,!SortDir);
				change_to_3x3(0,SortDir);

				BFS_run(3);

				change_to_3x3(1,SortDir);
				change_to_3x3(0,!SortDir);
			}
			// 가져와야하는 퍽이 M이 아닌경우
			if(TargetColor!=M) BFS_run(6);
			// Target위치에 맞게 퍽 배치
			BFS_EXIT=0;
			while(BFS_EXIT==0) BFS_run(1);
		}
	}
	change_to_3x3(1,!SortDir);

	// 맞추는 맵 반전
	SortDir=!SortDir;
	// 반대쪽 BFS
	change_to_3x3(0,!SortDir);

	// 메모리 최적화를 위해서 따로 돌립니다
	for(int i=0;i<3;i++){
		TargetStep=i+1;
		TargetNode=SNode[SortDir][0][i];
		TargetColor=SS[TargetNode];
		
		// 가져와야하는 퍽이 M이 아닌경우
		if(TargetColor!=M) BFS_run(6);
		BFS_EXIT=0;
		while(BFS_EXIT==0) BFS_run(1);
	}
	change_to_3x3(1,!SortDir);
	
	// 가짜 정답에서 M옆에 있는 퍽중 정답인 경우 M처리한거 복구하는 로직
	for(int i=0;i<BCN_cnt;i++) IP[BCN_value[i][0]]=BCN_value[i][1];
	
	FBS=1;
	FBSP=step_cnt;
	copy(oIP,IP);
	copy(IP,oIP);
	copy(SS,oSS);

	// 최종 BFS
	BFS_EXIT=0;
	while(BFS_EXIT==0){
		if(BFS_run(5)==0){
			FBS=0;
			break;
		}
	}
	// 최종 BFS 실패시 휴리스틱 사용하지 않고 재탐색
	if(FBS==0){
		step_cnt=FBSP;
		copy(IP,oIP);
		copy(SS,oSS);
		BFS_EXIT=0;
		while(BFS_EXIT==0) BFS_run(5);
	}
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
		i+=Path_cnt;
	}
	printf("%d\n\n",step_cnt);
	
	if(same(oSS,move_arr)) printf("Success!!");
}
