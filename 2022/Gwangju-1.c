#include <stdio.h>

#define N 11

enum node {start, a, b, c, d, e, f, g, h, i, j};
char *node_str[N] = { "start","a","b","c","d","e","f","g","h","i","j"};

int gr[N][N], visit[N];

int start_a(int flg) {
	return 1;
}
int a_start(int flg) {
	return 1;
}
int start_b(int flg) {
	return 1;
}
int b_start(int flg) {
	return 1;
}

int a_c(int flg) {
	return 0;
}
int c_a(int flg) {
	return 0;
}
int b_d(int flg) {
	return 1;
}
int d_b(int flg) {
	return 1;
}

int c_e(int flg) {
	return 1;
}
int e_c(int flg) {
	return 1;
}
int d_g(int flg) {
	return 1;
}
int g_d(int flg) {
	return 1;
}

int e_f(int flg) {
	return 1;
}
int f_e(int flg) {
	return 1;
}
int g_f(int flg) {
	return 1;
}
int f_g(int flg) {
	return 1;
}

int e_h(int flg) {
	return 1;
}
int h_e(int flg) {
	return 1;
}
int g_j(int flg) {
	return 0;
}
int j_g(int flg) {
	return 0;
}

int h_i(int flg) {
	return 1;
}
int i_h(int flg) {
	return 1;
}
int j_i(int flg) {
	return 1;
}
int i_j(int flg) {
	return 1;
}

int (*fp[N][N])(int);

void con(int st, int en, int *p1, int *p2) {
	gr[st][en] = gr[en][st] = 1;
	fp[st][en] = p1;
	fp[en][st] = p2;
}
void init() {
	con(start, a, start_a, a_start);
	con(start, b, start_b, b_start);

	con(a, c, a_c, c_a);
	con(b, d, b_d, d_b);

	con(c, e, c_e, e_c);
	con(d, g, d_g, g_d);
	con(e, f, e_f, f_e);
	con(g, f, g_f, f_g);

	con(e, h, e_h, h_e);
	con(g, j, g_j, j_g);
	con(h, i, h_i, i_h);
	con(j, i, j_i, i_j);
}

int area_move(int en){
	static int st = start;
	if (st == en) return 1;
	visit[st] = 1;

	int gate, flg = !visit[en];
	gate = fp[st][en](flg);

	printf("%s_%s(%d)\n",node_str[st], node_str[en], flg);
	
	if(gate) st = en; // 게이트 통과 가능할경우에만 이동
	return gate;
}
void dfs(int st) {
	if (visit[st]) return;

	for (int i = 0; i < N; i++) {
		if (gr[st][i] && !visit[i]) {
			if (area_move(i)) dfs(i);
			area_move(st);
		}
	}
}

int main(){
	init();
	dfs(start);
}