#include <stdio.h>
#include <stdlib.h>

#define N 18 // NODE 수
#define MAX 100 // QUEUE의 크기
#define M 99 // 장애물

// QUEUE
int rear, front, parent[MAX];
char queue[MAX][N];

// MAP
char A[3] = { 2,3,1 };
char house[6][3] = {
	{-1,-1,-1 },
	{-1,-1,-1 },
	{-1,-1,-1 },
	{ 0, 3, 0 },
	{ 1, 1, 3 },
	{ 0, 0, 0 }
};
char dirx[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
char diry[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };

char IP[N] = { 
	-1, -1, -1,
	-1, -1, -1,
	-1, -1, -1,
	 0,  0,  3, 
	 1,  0,  3,
	 1,  0,  0 
};
int step_cnt;

// GRAPH
char g[N][N];

// QUEUE 함수
char* dequeue() {
	if (front <= rear) return queue[front++];
	else return NULL;
}
void enqueue(char arr[]) {
	if (rear < MAX)
	{
		for (int i = 0; i < N + 1; i++) queue[rear][i] = arr[i];
		rear++;
	}
}
// BFS 함수
void copy(char* a, char* b) {
	for (int i = 0; i < N + 1; i++) b[i] = a[i];
}
void swap(char* a, char* b) {
	char temp = *a;
	*a = *b;
	*b = temp;
}
char same(char* a, char* b) {
	for (int i = 0; i < N; i++) if (a[i] != b[i] && b[i]) return 0;
	return 1;
}
char search_same(char* a) {
	for (int i = 0; i < rear; i++) if (same(queue[i], a))return 1;
	return 0;
}
char answer(char* a) {
	char r0 = 0, r1 = 0, r2 = 0, b0 = 0, b1 = 0, b2 = 0, ye0 = 0, ye1 = 0, ye2 = 0;
	for (int i = 0; i < N; i++) {
		if (a[i] != -1) {
			if (i % 3 == 0) {
				if (a[i] == 1) r0++;
				if (a[i] == 2) b0++;
				if (a[i] == 3) ye0++;
			}
			if (i % 3 == 1) {
				if (a[i] == 1) r1++;
				if (a[i] == 2) b1++;
				if (a[i] == 3) ye1++;
			}
			if (i % 3 == 2) {
				if (a[i] == 1) r2++;
				if (a[i] == 2) b2++;
				if (a[i] == 3) ye2++;
			}
		}
	}
	if (r0 > 1 || b0 > 1 || ye0 > 1 || (r0 && b0 && ye0 && A[0] != 1)) return 0;
	if (r1 > 1 || b1 > 1 || ye1 > 1 || (r1 && b1 && ye1 && A[1] != 1)) return 0;
	if (r2 > 1 || b2 > 1 || ye2 > 1 || (r2 && b2 && ye2 && A[2] != 1)) return 0;
	return 1;
}
void LinkSave(int a)
{
	int i, from, to;
	if (a > 0)LinkSave(parent[a]);
	else return;
	for (i = 0; i < N; i++) if (queue[a][i] && queue[a][i] != queue[parent[a]][i])to = i;
	for (i = 0; i < N; i++) if (queue[parent[a]][i] && queue[a][i] != queue[parent[a]][i])from = i;
	step_cnt++;

	printf("from %d  to %d", from, to);
	if (abs(from - to) == 2 || abs(from - to) == 4) printf(" cross");
	printf("\n");
}
void bfs() {
	char* a, b[N + 1];
	enqueue(IP);
	while (1) {
		a = dequeue();
		for (int i = 0; i < N + 1; i++) {
			printf("%d ", a[i]);
		}
		printf("\n\n");
		if (a == NULL) return;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (g[i][j] && a[i] && !a[j]) {
					copy(a, b);
					swap(&b[i], &b[j]);
					if (!search_same(b)) {
						parent[rear] = front - 1;
						enqueue(b);
						if (answer(b)) return;
					}
				}
			}
		}
	}
}
void conn()
{
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			if (house[i][j]!=-1) {
				for (int k = 0; k < 8; k++) {
					int nx = i + dirx[k];
					int ny = j + diry[k];
					if (house[nx][ny] != -1 && 0 <= nx && nx < 6 && 0 <= ny && ny < 3) {
						g[i * 3 + j][nx * 3 + ny] = 1;
						g[nx * 3 + ny][i * 3 + j] = 1;
					}
				}
			}
		}
	}
}

int main() {
	conn();
	bfs();
	printf("front   : %d\n", front);
	printf("rear    : %d\n", rear);
	printf("\n");
	for (int i = rear - 1; i > 0; i = parent[i])
	{
		for (int j = 0; j < N; j++) printf("%d ", queue[i][j]);
		printf("\t%d\n", i);
	}
	for (int i = 0; i < N; i++) printf("%d ", queue[0][i]);
	printf("\n\n");
	LinkSave(rear - 1);
	printf("\nstep = %d\n", step_cnt);
}
