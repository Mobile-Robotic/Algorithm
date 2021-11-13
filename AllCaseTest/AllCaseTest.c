#include <stdio.h>

char case_arr[10];

void check_case(char mode, char max_idx);
void make_case(char mode, char min_idx, char max_idx);

void check_case(char mode, char max_idx){
	char r=0, b=0, y=0;
	for(int i=0;i<max_idx;i++){
		if(case_arr[i]==1) r++;
		else if(case_arr[i]==2) b++;
		else if(case_arr[i]==3) y++;
	}
	if(mode==0 && r==1 && b==1 && y==1){
		for(int i=0;i<max_idx;i++) printf("%d ",case_arr[i]);
		printf("\n");

		make_case(1,0,2);
	}
	if(mode==1 && r==1 && b==0 && y==0){
		for(int i=0;i<max_idx;i++) printf("%d ",case_arr[i]);
		printf("\n");
	}
}
void make_case(char mode, char min_idx, char max_idx){
	if(min_idx==max_idx){
		check_case(mode, max_idx);
		return;	
	}
	for(int i=0;i<4;i++){
		case_arr[min_idx]=i;
		make_case(mode, min_idx+1, max_idx);
	}
}

int main(){
	make_case(0,0,3);
}
