#include <stdio.h>

#define N 12
#define MAX 560
#define M 99

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N]={0,2,0,3,1,0,2,1,2,3,M,3}, SS[N]={3,0,2,0,0,1,3,2,3,1,M,2};
char HoleNode=1; // Ȧ ���(Ȧ ����� SS���� ���̿��� �մϴ�)

char oIP[N], oSS[N]; // ���� IP, SS
char sSS[N]; // Corner�� ������ �о���� SS
char tIP[3], tSS[3]; // ������ ������ 3���� ���� ��尪�� ����: (0, 4, 8) (3, 7, 11)
char TargetNode, TargetColor, TargetStep; // BFS_IsPartSame�� Mode�� 1�ϋ� Target ���� �����͵�
char BFS_EXIT; // BFS ���� ����(�ߺ��������� while������ BFS�� ȣ���� �� ���˴ϴ�)

char BCN_value[2][2], BCN_cnt; // �޸� ����ȭ�� ���� M or HoleNode�� �¿��� ���� ������ ���� ��: index(0: node, 1: color)
char FBS, FBSP; // ������ BFS ���н� �޸���ƽ�� �����ϰ� �ٽ� ������ ���θ� �����ϴ� ����

char SortDir; // BFS �˻� �켱 ���� ����

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
		rv_cnt+=cnt/n_cnt; // (���� ���� ���� / ��� ����)
		if(a[i]==SS[i]) rv_cnt++; // ���� ��ġ ����
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
	
	// Ȧ ��尡 �ƴϸ鼭 ������ΰ� üũ
	for(int i=0;i<2;i++) for(int j=0;j<3;j++) if(SNode[i][0][j]!=HoleNode && SS[SNode[i][0][j]]) p_cnt[i]++;
	
	// ��ֹ��� Ȧ�� C3, C4, C7, C8�� �ִ°��
	if(p_cnt[0]==p_cnt[1]){
		// �Ʒ��� ���������� ó�� ���ߴ°� �޸� �Ҹ� ũ�ٴ� �����Ͽ� �ۼ���
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
	// 3X3 �� ����
	if(mode==0){
		for(int i=0;i<3;i++){
			tIP[i]=IP[SNode[dir][0][i]];
			tSS[i]=SS[SNode[dir][0][i]];
			
			IP[SNode[dir][0][i]]=M;
			SS[SNode[dir][0][i]]=M;
		}
	}
	// 3X3 �� Ǯ��
	else{
		for(int i=0;i<3;i++){	
			IP[SNode[dir][0][i]]=tIP[i];
			SS[SNode[dir][0][i]]=tSS[i];
		}
	}
}
char is_there_target_puck(){
	// ���� ������ ������ ������� ���������� �̹� ������ ���� ��� ���� �ȵ�
	// �� ���� ��
	for(int i=TargetStep-1;i<3;i++) if(IP[SNode[SortDir][0][i]]==TargetColor) return 1;
	for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(IP[SNode[SortDir][i][j]]==TargetColor) return 1;
	return 0;
}
char BFS_is_partsame(char a[], char mode){
	// Ȧ�� ���� ���� ���� ����� �����ϴ� case�� �����ϱ� ���� �����Դϴ�
	// Ȧ ��尡 M�϶��� �ش� ������ �����ִٴ°��� �ǹ������� ������ �����ϰ� �Ѿ�ϴ�
	if(a[HoleNode] && a[HoleNode]!=M) return 0;
	char isTargetColor=0;
	char n_cnt=0;
	
	// TargetNode�� ���� ������ �����̿� �ش� �ܵ��� ������ �������� ���� �ʽ��ϴ�.
	if(mode==1){
		// ��ֹ� ���� �ִ� ���� �����̶�� Ȧ����Ű��, �߿��Ѱ� ���õ� �����̶�� ���̴� �Ժη� OrigSS�� ���Ƽ��� �ȵȴ� ��¥ �����̱� ����
		for(int i=0;i<3;i++){
			if(SNode[SortDir][1][i]==HoleNode || a[SNode[SortDir][1][i]]==M){
				if(
					SS[SNode[SortDir][0][i]] && // ���信 ���� �����ϰ�
					SS[SNode[SortDir][0][i]]!=M && // M�� �ƴϸ�
					SS[SNode[SortDir][0][i]]==a[SNode[SortDir][0][i]] // ����� ��ġ�Ѵٸ�
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
	// �ݴ��ʿ��� �ʿ��� ���� ���;��մϴ�. �ݴ��� ������ ������� 2�� ������ Ż��(��, Ȧ������ ��������� �� �� ����)
	else if(mode==2){
		for(int i=0;i<3;i++) if(tIP[i]==0 && SNode[!SortDir][0][i]!=HoleNode) n_cnt++;
		for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(a[SNode[!SortDir][i][j]]==0 && SNode[!SortDir][i][j]!=HoleNode) n_cnt++;
		if(n_cnt>=2){
			copy(IP,a);
			return 1;
		}
	}
	// �ݴ��ʱ������� ���� ������ ���� �����;��մϴ�. ���� �������� ������� 2���� ���� �ʿ��� ���� �ش� ���� ������ Ż��
	else if(mode==3){
		for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(a[SNode[!SortDir][i][j]]==TargetColor) isTargetColor=1;
		for(int i=0;i<3;i++) if(tIP[i]==0 && SNode[SortDir][0][i]!=HoleNode) n_cnt++;
		for(int i=1;i<3;i++) for(int j=0;j<3;j++) if(a[SNode[SortDir][i][j]]==0 && SNode[SortDir][i][j]!=HoleNode) n_cnt++;
		if(n_cnt>=2 && isTargetColor){
			copy(IP,a);
			return 1;
		}
	}
	// Ȧ�� ������� ���� Ż��
	else if(mode==4){
		if(a[HoleNode]==0){
			copy(IP,a);
			return 1;
		}
	}
	// ���������� ���⶧ ������ �ִ� ���� ��� Mó���ϸ鼭 �޸� ����ȭ�ϰ� ���������̸� BFS ���� ���� ��Ű�� ����
	else if(mode==5){
		// ��������� ��ġ�Ѵٸ�
		if(same(a,SS)){
			BFS_EXIT=1;
			copy(IP,a);
			return 1;
		}
		// ������ ������ �´ٸ�
		for(int i=0;i<4;i++){
			if(
				SS[CornerNode[i]] && // ���信 ���� �����ϰ�
				SS[CornerNode[i]]!=M && // M�� �ƴϸ�
				SS[CornerNode[i]] == a[CornerNode[i]] // ����� ��ġ�Ѵٸ�
			){
				SS[CornerNode[i]]=M;
				a[CornerNode[i]]=M;
				
				copy(IP,a);
				return 1;
			}
		}
	}
	// Target���� ���߷��� �ϴ� ���� 0 ~ 1 step�Ÿ� ������ ������ Ż��
	else if(mode==6){
		// Worst Case�� 800�� ���� ���ü� �������� ��� �����ϴ�.
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
	// ó���� ���� ��Ī�̶� Ȧ��带 ������ ������� �ݵ�� 2�� ������ֱ� ���� �ݴ������� ���� ������ Ż��
	// �� SortDir�� �ݴ������ 0��° ���� ��� ������ �������� Ż����
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
	
	// Ȧ���� ���� �ִٸ� ������ ġ���
	if(IP[HoleNode]) BFS_run(4);
	
	// BFS 3x3���� ���� �켱����
	SortDir=BFS_priority();
	
	// 0���� ���õǰ� 0���� ������ 1���� 3���� ������ Ż��
	change_to_3x3(0,SortDir);
	BFS_run(7);
	change_to_3x3(1,SortDir);
	
	// ���ϴ� ���� �������� Dir�� �ݴ��
	// ���ϴ� ���� Ž���Ҷ��� Dir�� �״��
	change_to_3x3(0,!SortDir);
	for(int i=0;i<3;i++){
		TargetStep=i+1;
		TargetNode=SNode[SortDir][0][i];
		TargetColor=SS[TargetNode];
		
		if(TargetNode!=HoleNode && IP[TargetNode]!=SS[TargetNode]){
			if(is_there_target_puck()==0){
				// �ݴ��� �ʿ��� �� ���� �� �ֵ��� ���� ������ֱ�
				BFS_run(2);
				// �ݴ��ʿ��� ���� ������ �� ������
				change_to_3x3(1,!SortDir);
				change_to_3x3(0,SortDir);

				BFS_run(3);

				change_to_3x3(1,SortDir);
				change_to_3x3(0,!SortDir);
			}
			// �����;��ϴ� ���� M�� �ƴѰ��
			if(TargetColor!=M) BFS_run(6);
			// Target��ġ�� �°� �� ��ġ
			BFS_EXIT=0;
			while(BFS_EXIT==0) BFS_run(1);
		}
	}
	change_to_3x3(1,!SortDir);

	// ���ߴ� �� ����
	SortDir=!SortDir;
	// �ݴ��� BFS
	change_to_3x3(0,!SortDir);

	// �޸� ����ȭ�� ���ؼ� ���� �����ϴ�
	for(int i=0;i<3;i++){
		TargetStep=i+1;
		TargetNode=SNode[SortDir][0][i];
		TargetColor=SS[TargetNode];
		
		// �����;��ϴ� ���� M�� �ƴѰ��
		if(TargetColor!=M) BFS_run(6);
		BFS_EXIT=0;
		while(BFS_EXIT==0) BFS_run(1);
	}
	change_to_3x3(1,!SortDir);
	
	// ��¥ ���信�� M���� �ִ� ���� ������ ��� Mó���Ѱ� �����ϴ� ����
	for(int i=0;i<BCN_cnt;i++) IP[BCN_value[i][0]]=BCN_value[i][1];
	
	FBS=1;
	FBSP=step_cnt;
	copy(oIP,IP);
	copy(IP,oIP);
	copy(SS,oSS);

	// ���� BFS
	BFS_EXIT=0;
	while(BFS_EXIT==0){
		if(BFS_run(5)==0){
			FBS=0;
			break;
		}
	}
	// ���� BFS ���н� �޸���ƽ ������� �ʰ� ��Ž��
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
