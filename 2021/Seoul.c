#include <stdio.h>

int sheet[4]={6,8,12,}, sheet_cnt=3;
int sheet_val[4]={2,1,3,};

int skip[4], skip_flg, skip_node;
int stack[12], step_cnt;

int tri[4][3], tri_cnt;

void swap(int *a,int *b){
	int tmp=*a;
	*a=*b;
	*b=tmp;
}
char Change(char value,char mode){
	if(mode==0) return value/5;
	if(mode==1) return value%5;
	return 0;
}
int sign(int a[],int b[],int c[]) // α(알파), β(베타), γ(감마) 값을 구하는 로직
{
	return (a[0]-c[0])*(b[1]-c[1])-(b[0]-c[0])*(a[1]-c[1]);
}
int check_tri(int p,int pp[]) // 삼각형 내부에 점이 있는지 확인(선분 제외)
{
	int a[2],b[3][2];

	a[0]=Change(p,0);
	a[1]=Change(p,1);

	for(int i=0;i<3;i++){
		b[i][0]=Change(pp[i],0);
		b[i][1]=Change(pp[i],1);
	}

	int d1,d2,d3;
	int f1,f2;

	d1=sign(a,b[0],b[1]);
	d2=sign(a,b[1],b[2]);
	d3=sign(a,b[2],b[0]);

	f1=(d1<=0)||(d2<=0)||(d3<=0);
	f2=(d1>=0)||(d2>=0)||(d3>=0);

	return !(f1&&f2);
}
int current_sheet(int i){
	int cnt=0;
	for(int j=0;j<tri_cnt;j++) if(check_tri(sheet[i],tri[j])) cnt++;
	return cnt;
}
void current_skip(){
	for(int i=0;i<sheet_cnt;i++){
		if(current_sheet(i)>=sheet_val[i]) skip[i]=1;
		else skip[i]=0;
	}
}
int check(int p[]){
	int cnt;
	
	for(int i=0;i<sheet_cnt;i++){
		cnt=check_tri(sheet[i],p);
		// 필요한 삼각형보다 더 많다면
		if(current_sheet(i)+cnt>sheet_val[i]) return 0;
		// 한개의 시트지도 포함되지 않았다면
		if(!skip[i] && cnt<1) return 0;
	}

	// 좌표 3개가 겹친다면
	for(int i=0;i<tri_cnt;i++){
		cnt=0;
		for(int j=0;j<3;j++) for(int k=0;k<3;k++) if(tri[i][j]==p[k]) cnt++;
		if(cnt>=3) return 0;
	}
	return 1;
}

// start_node는 node의 index
int make_tri(int start_node){
	int node[16]={0,1,2,3,4,9,14,19,24,23,22,21,20,15,10,5};
	int addr[3], p[3];

	addr[0]=addr[1]=addr[2]=start_node;
	p[0]=node[addr[0]];
	for(int i=0;i<5;i++){
		addr[1]--;
		if(addr[1]<0) addr[1]=15;
		p[1]=node[addr[1]];
		for(int j=0;j<10;j++){
			addr[2]++;
			if(addr[2]>15) addr[2]=0;
			p[2]=node[addr[2]];
			if(check(p)){
				tri[tri_cnt][0]=p[0];
				tri[tri_cnt][1]=p[1];
				tri[tri_cnt++][2]=p[2];
				return 1;
			}
		}
		addr[2]=start_node;
	}

	addr[0]=addr[1]=addr[2]=start_node;
	p[0]=node[addr[0]];
	for(int i=0;i<10;i++){
		addr[1]--;
		if(addr[1]<0) addr[1]=15;
		p[1]=node[addr[1]];
		for(int j=0;j<5;j++){
			addr[2]++;
			if(addr[2]>15) addr[2]=0;
			p[2]=node[addr[2]];
			if(check(p)){
				tri[tri_cnt][0]=p[0];
				tri[tri_cnt][1]=p[1];
				tri[tri_cnt++][2]=p[2];
				return 1;
			}
		}
		addr[2]=start_node;
	}
	return 0;
}
int make(){
	int start[12]={0,1,15, 3,4,5, 7,8,9, 11,12,13};
	int flg, cnt;
	for(int i=0;i<12;i++){
		for(int j=0;j<4;j++){
			if(skip_flg){
				skip[skip_node]=1;
				if(make_tri(start[i])) return 1;
			}
			else{
				make_tri(start[i]);
				current_skip();
				flg=0;
				cnt=3;
				for(int i=0;i<sheet_cnt;i++){
					if(!skip[i]) flg=1;
					if(sheet_val[i]==3) cnt=4;
				}
				if(tri_cnt>cnt) return 0;
				if(!flg) return 1;
			}
		}
	}
	return 0;
}
void solve(){
	int flg;

	if(!make()){
		for(int i=0;i<sheet_cnt;i++){
			current_skip();
			if(!skip[i]){
				skip_flg=1;
				skip_node=i;
				make();
			}
		}
		skip_flg=0;
		current_skip();
		make();
	}

	for(int i=0;i<tri_cnt;i++) for(int j=0;j<2;j++) for(int k=0;k<2;k++) if(tri[i][k]>tri[i][k+1]) swap(&tri[i][k],&tri[i][k+1]);
	for(int i=0;i<tri_cnt-1;i++){
		for(int j=0;j<tri_cnt-1;j++){
			if(
				(tri[j][0]>tri[j+1][0]) ||
				(tri[j][0]==tri[j+1][0] && tri[j][1]>tri[j+1][1]) ||
				(tri[j][0]==tri[j+1][0] && tri[j][1]==tri[j+1][1] && tri[j][2]>tri[j+1][2])
			){
				swap(&tri[j][0],&tri[j+1][0]);
				swap(&tri[j][1],&tri[j+1][1]);
				swap(&tri[j][2],&tri[j+1][2]);
			}
		}
	}

	for(int i=0;i<tri_cnt;i++){
		for(int j=0;j<3;j++){
			flg=0;
			for(int k=0;k<step_cnt;k++) if(tri[i][j]==stack[k]) flg=1;
			if(!flg) stack[step_cnt++]=tri[i][j];
		}
	}
	for(int i=0;i<step_cnt-1;i++) for(int j=0;j<step_cnt-1;j++) if(stack[j]>stack[j+1]) swap(&stack[j],&stack[j+1]);
}

int main(){
	solve();

	for(int i=0;i<tri_cnt;i++) printf("%d, %d, %d\n",tri[i][0],tri[i][1],tri[i][2]);
	printf("\n");

	current_skip();
	int flg=0, cnt=3;
	for(int i=0;i<sheet_cnt;i++){
		if(!skip[i]) flg=1;
		if(sheet_val[i]==3) cnt=4;
	}
	if(tri_cnt>cnt) flg=1;

	if(!flg){
		printf("Success!\n");
	}
}
