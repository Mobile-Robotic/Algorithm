#include <stdio.h>

#define N 6 // map 크기

char map[N][N] = // 맵
{
	{0,1,0,0,0,0},
	{0,2,0,0,0,0},
	{0,1,0,2,2,1},
	{0,3,0,3,0,0},
	{1,0,0,1,0,0},
	{0,0,1,0,0,0}
};
int answer[N * N][2][6]; // 정답지 배열
int answer_cnt; // 정답지 개수

void copy(char a[N][N], char b[N][N]) // 2차원 배열 복사
{
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) b[i][j] = a[i][j];
}
int sign(int a1, int a2, int b1, int b2, int c1, int c2) // α(알파), β(베타), γ(감마) 값을 구하는 로직
{
    return (a1 - c1) * (b2 - c2) - (b1 - c1) * (a2 - c2);
}
int calc_p1(int p1, int p2, int a1, int a2, int b1, int b2, int c1, int c2) // 삼각형 내부에 점이 있는지 확인(선분 제외)
{
    int d1, d2, d3;
    int f1, f2;

    d1 = sign(p1, p2, a1, a2, b1, b2);
    d2 = sign(p1, p2, b1, b2, c1, c2);
    d3 = sign(p1, p2, c1, c2, a1, a2);

    f1 = (d1 <= 0) || (d2 <= 0) || (d3 <= 0);
    f2 = (d1 >= 0) || (d2 >= 0) || (d3 >= 0);

    return !(f1 && f2);
}
int calc_p2(int p1, int p2, int a1, int a2, int b1, int b2, int c1, int c2) // 삼각형 내부에 점이 있는지 확인(선분 포함) 복붙 = 제외
{
    int d1, d2, d3;
    int f1, f2;

    d1 = sign(p1, p2, a1, a2, b1, b2);
    d2 = sign(p1, p2, b1, b2, c1, c2);
    d3 = sign(p1, p2, c1, c2, a1, a2);

    f1 = (d1 < 0) || (d2 < 0) || (d3 < 0);
    f2 = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(f1 && f2);
}
void solve() // 삼각형 구하기
{
    char a[N][N];
    int a1, a2, b1, b2, c1, c2;
    char s_flg = 0;
    int b_t[N * N][6], b_cnt = 0;

    a1 = 0, a2 = 0;
    b1 = 0, b2 = 1;
    c1 = 0, c2 = 2;
    while (1)
    {
        if (!map[a1][a2] && !map[b1][b2] && !map[c1][c2]) {
            s_flg = 1;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (map[i][j] == 1) {
                        if (calc_p2(i, j, a1, a2, b1, b2, c1, c2) == 1) i = j = N - 1, s_flg = 0;
                    }
                    if (map[i][j] == 2) {
                        if (calc_p1(i, j, a1, a2, b1, b2, c1, c2) == 0) i = j = N - 1, s_flg = 0;
                    }
                    if (map[i][j] == 3) {
                        if (calc_p2(i, j, a1, a2, b1, b2, c1, c2) == 1) i = j = N - 1, s_flg = 0;
                    }
                }
            }
            if (s_flg) {
                b_t[b_cnt][0] = a1;
                b_t[b_cnt][1] = a2;
                b_t[b_cnt][2] = b1;
                b_t[b_cnt][3] = b2;
                b_t[b_cnt][4] = c1;
                b_t[b_cnt][5] = c2;
                b_cnt++;
            }
        }
        // 중복 제거 로직
        c2++;
        if (c2 == N) c2 = 0, c1++;
        if (c1 == N) c1 = 0, b2++;
        if (b2 == N - 1) b2 = 0, b1++;
        if (b1 == N) b1 = 0, a2++;
        if (a2 == N) a2 = 0, a1++;
        if (a1 == N -1 && a2 == N - 2) break;
        if (b1 <= a1) {
            if (b1 < a1 || b1 == 0) b1 = a2 != 5 ? a1 : a1 + 1;
            if (b2 <= a2) b2 = a2 != 5 ? a2 + 1 : 0;
        }
        if (c1 <= b1) {
            if (c1 < b1 || c1 == 0) c1 = b2 != 5 ? b1 : b1 + 1;
            if (c2 <= b2) c2 = b2 != 5 ? b2 + 1 : 0;
        }
    }
    for (int i = 0; i < b_cnt; i++) {
        copy(map, a); // 맵복사
        for (int j = 0; j < N; j++) { // 맵채우기
            for (int k = 0; k < N; k++) {
                if (calc_p2(j, k, b_t[i][0], b_t[i][1], b_t[i][2], b_t[i][3], b_t[i][4], b_t[i][5]) == 1 && !a[j][k]) {
                    a[j][k] = 2;
                }
            }
        }
        a1 = 0, a2 = 0;
        b1 = 0, b2 = 1;
        c1 = 0, c2 = 2;
        while (1)
        {
            if (!a[a1][a2] && !a[b1][b2] && !a[c1][c2]) {
                s_flg = 1;
                for (int j = 0; j < N; j++) {
                    for (int k = 0; k < N; k++) {
                        if (a[j][k] == 1) {
                            if (calc_p2(j, k, a1, a2, b1, b2, c1, c2) == 1) j = k = N - 1, s_flg = 0;
                        }
                        if (a[j][k] == 2) { // 노란 퍽을 찾으므로 서로 if조건을 swap
                            if (calc_p2(j, k, a1, a2, b1, b2, c1, c2) == 1) j = k = N - 1, s_flg = 0;
                        }
                        if (a[j][k] == 3) {
                            if (calc_p1(j, k, a1, a2, b1, b2, c1, c2) == 0) j = k = N - 1, s_flg = 0;
                        }
                    }
                }
                if (s_flg) { // 정답지 저장
                    answer[answer_cnt][0][0] = b_t[i][0];
                    answer[answer_cnt][0][1] = b_t[i][1];
                    answer[answer_cnt][0][2] = b_t[i][2];
                    answer[answer_cnt][0][3] = b_t[i][3];
                    answer[answer_cnt][0][4] = b_t[i][4];
                    answer[answer_cnt][0][5] = b_t[i][5];
                    answer[answer_cnt][1][0] = a1;
                    answer[answer_cnt][1][1] = a2;
                    answer[answer_cnt][1][2] = b1;
                    answer[answer_cnt][1][3] = b2;
                    answer[answer_cnt][1][4] = c1;
                    answer[answer_cnt][1][5] = c2;
                    answer_cnt++;
                }
            }
            c2++;
            if (c2 == N) c2 = 0, c1++;
            if (c1 == N) c1 = 0, b2++;
            if (b2 == N - 1) b2 = 0, b1++;
            if (b1 == N) b1 = 0, a2++;
            if (a2 == N) a2 = 0, a1++;
            if (a1 == N - 1 && a2 == N - 2) break;
            if (b1 <= a1) {
                if (b1 < a1 || b1 == 0) b1 = a2 != 5 ? a1 : a1 + 1;
                if (b2 <= a2) b2 = a2 != 5 ? a2 + 1 : 0;
            }
            if (c1 <= b1) {
                if (c1 < b1 || c1 == 0) c1 = b2 != 5 ? b1 : b1 + 1;
                if (c2 <= b2) c2 = b2 != 5 ? b2 + 1 : 0;
            }
        }
    }
}
int main() 
{
    char a_map[N][N] = { {0,}, };
    int success = N * N - 1;
    int s_cnt = 0;
    solve();
    for (int i = 0; i < answer_cnt; i++) {
        s_cnt = 0;
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (!map[j][k]) {
                    if (calc_p1(j, k, answer[i][0][0], answer[i][0][1], answer[i][0][2], answer[i][0][3], answer[i][0][4], answer[i][0][5]) == 1) s_cnt++;
                    if (calc_p1(j, k, answer[i][1][0], answer[i][1][1], answer[i][1][2], answer[i][1][3], answer[i][1][4], answer[i][1][5]) == 1) s_cnt++;
                }
            }
        }
        if (success > s_cnt) {
            success = i;
        }
    }
    a_map[answer[success][0][0]][answer[success][0][1]] = a_map[answer[success][0][2]][answer[success][0][3]] = a_map[answer[success][0][4]][answer[success][0][5]] = 2;
    a_map[answer[success][1][0]][answer[success][1][1]] = a_map[answer[success][1][2]][answer[success][1][3]] = a_map[answer[success][1][4]][answer[success][1][5]] = 3;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!map[i][j]) {
                if (calc_p1(i, j, answer[success][0][0], answer[success][0][1], answer[success][0][2], answer[success][0][3], answer[success][0][4], answer[success][0][5]) == 1 && !a_map[i][j]) {
                    a_map[i][j] = 2;
                }
                if (calc_p1(i, j, answer[success][1][0], answer[success][1][1], answer[success][1][2], answer[success][1][3], answer[success][1][4], answer[success][1][5]) == 1 && !a_map[i][j]) {
                    a_map[i][j] = 3;
                }
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", a_map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a_map[i][j]) {
                printf("color = %d\tx = %d\ty = %d\n", a_map[i][j], i, j);
            }
        }
    }
}