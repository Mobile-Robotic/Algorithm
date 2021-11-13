#include <stdio.h>

// �����̵� ���� && ��� �˰���

// ũ�Ⱑ ū ���� ��������
#define N 20

#define MAX 580
#define M 99

// X, Y�� ������ �����
// Change, Find_Node, safe �Լ��� ����
char X, Y;

int front, rear, parent[MAX], step_cnt, Path_cnt;
char queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

char IP[N], SS[N];

char IP1[N]={
	1,0,2,
	M,2,0,
	2,0,1
};
char SS1[N]={
	2,2,0,
	M,0,1,
	1,0,2
};

char IP2[N]={
	1,0,0,0,0,
	1,0,0,0,0,
	2,0,0,0,0,
	2,3,0,0,0,
};
char sheet=2; // 0 1 2

char HoleNode=7;
char CornerNode[4]={0,2,6,8};
char BFS_EXIT;

char SS_Node[3][4]={
	{3,8,4,9},
	{8,13,9,14},
	{13,18,14,19},
};
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
	if(mode<3 && a[HoleNode] && a[HoleNode]!=M) return 0;
	char cnt=0;
	if(mode==1){
		if(!a[HoleNode]){
			copy(IP,a);
			return 1;
		}
	}
	else if(mode==2){
		for(int i=0;i<4;i++){
			if(SS[CornerNode[i]] && SS[CornerNode[i]]!=M && SS[CornerNode[i]]==a[CornerNode[i]]){
				SS[CornerNode[i]]=a[CornerNode[i]]=M;
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
	else if(mode==3){
		if(a[TargetNode]==TargetColor){
			copy(IP,a);
			return 1;
		}
	}
	else if(mode==4){
		for(int i=0;i<Y;i++){
			if(a[TargetNode+i]==TargetColor){
				TargetNode+=i;
				copy(IP,a);
				return 1;
			}
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
		if(mode!=1 && a[HoleNode] && a[HoleNode]!=M){
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
			for(int n=0;n<4;n++) if(SS_Node[sheet][n]==i) i++;
			if(i<N && a[i] && a[i]!=M){
				if(a[i]==1){
					for(int D=0;D<4;D++){
						pose[0]=Change(i,0);
						pose[1]=Change(i,1);
						while(1){
							Pose_init(D);
							j=Find_Node(pose[0],pose[1]);
							if(!safe(pose[0],pose[1])) break;
							if(safe(pose[0],pose[1]) && a[j] && a[j]!=1 && a[j]!=M){
								Pose_init(D);
								j=Find_Node(pose[0],pose[1]);
								if(!safe(pose[0],pose[1])) break;
								if(safe(pose[0],pose[1]) && !a[j]){
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
				else if(a[i]==2){
					for(int D=0;D<4;D++){
						pose[0]=Change(i,0);
						pose[1]=Change(i,1);
						Pose_init(D);
						j=Find_Node(pose[0],pose[1]);
						if(safe(pose[0],pose[1]) && a[j]==0){
							Pose_init(D);
							j=Find_Node(pose[0],pose[1]);
							if(safe(pose[0],pose[1])){
								for(int d=0,k;d<2;d++){
									pose[0]=Change(j,0);
									pose[1]=Change(j,1);
									if(D%2) Pose_init(d*2);
									else Pose_init(d*2+1);
									k=Find_Node(pose[0],pose[1]);
									if(safe(pose[0],pose[1]) && a[k]==0){
										copy(b,a);
										swap(&b[i],&b[k]);
										if(s_same(b)==0){
											parent[rear]=front-1;
											point[0]=i;
											point[1]=k;
											if(!enqueue(point)) return 0;
											if(BFS_is_partsame(b,mode)) return 1;
										}
									}
								}
							}
						}
					}
				}
				else{
					for(int D=0;D<2;D++){
						pose[0]=Change(i,0);
						pose[1]=Change(i,1);
						Pose_init(D*2);
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
	}
	return 0;
}
char BFS_run(char mode){
	char state;
	if(mode<3) state=BFS(mode);
	else state=BFS2(mode);
	if(state==1) Link(rear-1,1);
	printf("mode=%d, state=%d, rear=%d\n",mode,state,rear);
	return state;
}
void solve(char mode){
	if(mode==0){
		X=3, Y=3;

		if(IP[HoleNode] && IP[HoleNode]!=M) BFS_run(1);
		BFS_EXIT=0;
		while(!BFS_EXIT) BFS_run(2);
	}
	else{
		X=4, Y=5;

		char cnt=0;
		char BN_value[2];
		for(int i=0;i<4;i++) if(IP[i*Y]==1) cnt++;

		if(cnt==1){
			TargetNode=SS_Node[sheet][3];
			TargetColor=2;
			BFS_run(3);

			TargetNode=SS_Node[sheet][1];
			TargetColor=2;
			BFS_run(3);
		}

		for(int i=0;i<cnt;i++){
			TargetNode=SS_Node[sheet][i]-3;
			// TargetNode �� ġ���
			TargetColor=0;
			BFS_run(3);
			// �������� ��Ʈ���� ���� �࿡ ��ġ
			TargetColor=1;
			BFS_run(4);

			// �Ű�� ������ M ó��
			BN_value[0]=TargetNode;
			BN_value[1]=IP[TargetNode];
			IP[TargetNode]=M;

			// �Ķ����� ����
			TargetNode=SS_Node[sheet][i];
			TargetColor=2;
			BFS_run(3);

			// M ó�� ����
			IP[BN_value[0]]=BN_value[1];

			// �������� ����
			TargetNode++;
			TargetColor=1;
			BFS_run(3);
		}
	}
}
int main(){
	char fIP[N], fSS[N];
	for(int i=0;i<2;i++){
		if(i==0){
			copy(IP,IP1);
			copy(SS,SS1);
		}
		else copy(IP,IP2);

		copy(fIP,IP);
		copy(fSS,SS);
		step_cnt=0;
		solve(i);
		if(i==0){
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
		else{
			for(int i=0;i<step_cnt;i++){
				printf("from %d to %d\n",stack[i][0],stack[i][1]);
				swap(&fIP[stack[i][0]],&fIP[stack[i][1]]);
				draw(fIP);
			}
			printf("step_cnt=%d\n",step_cnt);
		}
	}
}
