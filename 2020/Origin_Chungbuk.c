// sleep�Լ� ����ҋ� �ʿ�
#define _CRT_OBSOLETE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define N      12
#define MAX      580
#define M      99

#define Mode   1

#if Mode == 1
// ���� (Hole: 1)
char HoleNode = 1;
char SS[N] =
{
    3, 0, 2, 0,
    0, 1, 3, 2,
    3, 1, M, 2,
};

char IP[N] =
{
    0, 2, 0, 3,
    1, 0, 2, 1,
    2, 3, M, 3
};
#elif Mode == 2
// Hole: 7 (Ȧ�� 7�϶� empty ��ȯ)
char HoleNode = 4;
char SS[N] =
{
    3, M, 2, 2,
    0, 1, 2, 0,
    0, 3, 3, 1,
};

char IP[N] =
{
    1, M, 1, 0,
    3, 3, 3, 0,
    2, 2, 2, 0,
};
#elif Mode == 3
char HoleNode = 7;
char SS[N] =
{
    3, 2, 3, 1,
    2, 2, 0, 0,
    1, 3, M, 0,
};

char IP[N] =
{
    0, 1, 2, 2,
    1, 3, 0, 2,
    3, 0, M, 3,
};
#elif Mode == 4
// All���� Fail���� Return�� Case (Hole: 2)
char HoleNode = 2;
char SS[N] =
{
    0, M, 0, 0,
    2, 3, 3, 2,
    2, 1, 1, 1,
};

char IP[N] =
{
    0, M, 0, 0,
    3, 3, 2, 2,
    2, 1, 1, 1,
};
#elif Mode == 5
// Hole: 2
char HoleNode = 2;
char SS[N] =
{
    3, 2, 0, 0,
    0, 1, 3, 2,
    3, 1, M, 2,
};

char IP[N] =
{
    3, 2, 0, 0,
    1, 3, 3, 2,
    0, 1, M, 2,
};
#elif Mode == 6
char HoleNode = 1;
char SS[N] =
{
    3, 0, 2, 0,
    M, 1, 3, 2,
    3, 1, 0, 2,
};

char IP[N] =
{
    0, 0, 2, 3,
    M, 1, 2, 1,
    2, 3, 0, 3
};
#elif Mode == 7
// ������ ���� ���⶧ 560���� ������ ����Ǽ� �Դϴ�
char HoleNode = 2;
char SS[N] =
{
    0, M, 0, 0,
    3, 3, 3, 1,
    2, 2, 1, 1,
};

char IP[N] =
{
    0, M, 0, 0,
    3, 3, 3, 2,
    2, 1, 1, 1,
};
#elif Mode == 8
char HoleNode = 2;
char SS[N] =
{
    0, M, 0, 0,
    1, 3, 3, 2,
    3, 2, 2, 1,
};

char IP[N] =
{
    0, M, 0, 0,
    3, 3, 3, 2,
    2, 2, 1, 1,
};
#elif Mode == 9
// ��Ī�� ��� �޸𸮰� ������ ����Ǽ� �Դϴ�
char HoleNode = 2;
char SS[N] =
{
    0, M, 0, 0,
    3, 3, 3, 2,
    2, 1, 1, 2,
};

char IP[N] =
{
    0, M, 0, 0,
    3, 3, 3, 2,
    2, 2, 1, 1,
};
#elif Mode == 10
char HoleNode = 2;
char SS[N] =
{
    0, M, 0, 2,
    3, 0, 3, 2,
    2, 1, 1, 1,
};

char IP[N] =
{
    0, M, 0, 0,
    3, 3, 2, 2,
    2, 1, 1, 1,
};
#elif Mode == 11
char HoleNode = 2;
char SS[N] =
{
    0, M, 0, 0,
    3, 3, 3, 2,
    2, 1, 2, 1,
};

char IP[N] =
{
    0, M, 0, 0,
    3, 3, 3, 2,
    2, 2, 1, 1,
};
#endif

int rear, front, cnt, Parent[MAX], sp, Path_cnt;
char Queue[MAX][2], temp_IP[N], stack[60][2], pose[2];

#pragma region 2020-Chungbuk-2 �Լ�����

void Shift_SS_To_Corner();
void Change_To_3X3(int mode, int dir);
char Is_There_Target_Puck(int SortDir, int color);
float heuristic(char a[]);
char BFS_Search_Priority();
char BFS_IsPartSame(char a[], char mode);
char BFS_Run(char mode);
void BFS_Mission();

char Change(char value, char mode);
char Find_Node(char x, char y);
char safe(char x, char y);
void Pose_init(int Dir1);
void Path_S(int i);
void push(char x, char y);
void Link(int a, int mode);
void copy(char a[], char b[]);
void swap(char arr[], char i, char j);
void EnQueue(char arr[]);
char* DeQueue();
char same(char a[], char b[]);
char S_same(char a[]);
char BFS();

void VisualStudio_BFS_Debug(int printfMode, int mode);
void draw(char* arr);
void show_move(char* arr);

#pragma endregion

#pragma region 2020-Chungbuk-2 ��������

/// <summary>
/// ���� ���� �������� ���� �����ϴ� ����(�˰��� �������)
/// </summary>
char move_arr[N];

/// <summary>
/// Ȧ ���(Ȧ ����� SS���� ���̿��� �մϴ�)
/// </summary>
// char HoleNode = 4;

/// <summary>
/// BFS ���� ����(�ߺ��������� while������ BFS�� ȣ���� �� ���˴ϴ�)
/// </summary>
char IsBFSExit = 0;

/// <summary>
/// ���� StackPoint
/// </summary>
char BeforeSP = 0;

/// <summary>
/// ���� IP, SS
/// </summary>
char OrigIP[N];
char OrigSS[N];

/// <summary>
/// Corner�� ������ �о���� SS
/// </summary>
char SortSS[N];

/// <summary>
/// ������ ������ 3���� ���� ��尪�� ����: (0, 4, 8) (3, 7, 11)
/// </summary>
char TempStorageIP[3];
char TempStorageSS[3];

/// <summary>
/// BFS 3X3 Map Node List
/// </summary>
char SortNode[2][3][3] =
{
    // ����
    {
        { 0, 4, 8},
    { 1, 5, 9},
    { 2, 6,10},
    },
    // ������
   {
       { 3, 7,11},
    { 2, 6,10},
    { 1, 5, 9},
   },
};

/// <summary>
/// BFS_IsPartSame�� Mode�� 1�ϋ� Traget ���� �����͵�
/// </summary>
char TragetNode;
char TragetColor;
char TragetStep;

/// <summary>
/// BFS �˻� �켱 ���� ����
/// </summary>
char SortDir;

/// <summary>
/// �޸� ����ȭ�� ���� M or HoleNode�� �¿��� ���� ������ ���� ��: index(0: node, 1: color)
/// </summary>
char BeforeCornerNodeCnt;
char BeforeCornerNodeValue[2][2];

/// <summary>
/// ������ BFS ���н� �޸���ƽ�� �����ϰ� �ٽ� ������ ���θ� �����ϴ� ����
/// </summary>
char IsFinal_BFS_Success;
char IsFinal_BeforeSP;

#pragma endregion

#pragma region 2020-Chungbuk-2 Logic

void Shift_SS_To_Corner()
{
    int i, j;

    copy(SortSS, OrigSS);

    int CornerNode[4] = { 0, 3, 8, 11 };
    int CornerBuff[4][5] =
    {
        { 1,  2,  4,  5,  9}, // 0
        { 2,  1,  7,  6, 10}, // 3
        { 9, 10,  4,  5,  1}, // 8
        {10,  9,  7,  6,  2}, // 11
    };
    for (i = 0; i < 4; i++)
    {
        if (SortSS[CornerNode[i]] == 0)
        {
            for (j = 0; j < 5; j++)
            {
                if (j == 0 && SortSS[CornerBuff[i][j]] == M)
                {
                    j++;
                    continue;
                }
                if (j == 2 && SortSS[CornerBuff[i][j]] == M)
                {
                    continue;
                }
                if (SortSS[CornerBuff[i][j]] != 0 &&
                    SortSS[CornerBuff[i][j]] != M)
                {
                    swap(SortSS, CornerNode[i], CornerBuff[i][j]);
                    break;
                }
            }
        }
    }

    int MidNode[2] = { 4, 7 };
    int MidBuff[2][6] =
    {
        { 5,  6,  1,  9,  2, 10}, // 4
        { 6,  5,  2,  10, 1,  9}, // 7
    };
    for (i = 0; i < 2; i++)
    {
        if (SortSS[MidNode[i]] == 0 && MidNode[i] != HoleNode)
        {
            for (j = 0; j < 6; j++)
            {
                if (SortSS[MidBuff[i][j]] != 0 &&
                    SortSS[MidBuff[i][j]] != M)
                {
                    swap(SortSS, MidNode[i], MidBuff[i][j]);
                    break;
                }
            }
        }
    }

    copy(SS, SortSS);
}

void Change_To_3X3(int mode, int dir)
{
    int i;

    // 3X3 �� ����
    if (mode == 0)
    {
        for (i = 0; i < 3; i++)
        {
            TempStorageIP[i] = IP[SortNode[dir][0][i]];
            TempStorageSS[i] = SS[SortNode[dir][0][i]];

            IP[SortNode[dir][0][i]] = M;
            SS[SortNode[dir][0][i]] = M;
        }
    }
    // 3X3 �� Ǯ��
    else if (mode == 1)
    {
        for (i = 0; i < 3; i++)
        {
            IP[SortNode[dir][0][i]] = TempStorageIP[i];
            SS[SortNode[dir][0][i]] = TempStorageSS[i];
        }
    }
}

char Is_There_Target_Puck(int SortDir, int color)
{
    // ���� ������ ������ ������� ���������� �̹� ������ ���� ��� ���� �ȵ�
    // �� ���� ��
    for (int i = TragetStep - 1; i < 3; i++)
        if (IP[SortNode[SortDir][0][i]] == color)
            return 1;

    for (int i = 1; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (IP[SortNode[SortDir][i][j]] == color)
                return 1;

    return 0;
}

float heuristic(char a[])
{
    int i, j, D;
    char my[2];
    float cnt = 0, n_cnt = 0, rv_cnt = 0;

    for (i = 0; i < N; i++)
    {
        my[0] = Change(i, 0);
        my[1] = Change(i, 1);
        for (D = 0; D < 4; D++)
        {
            pose[0] = my[0];
            pose[1] = my[1];
            Pose_init(D);
            j = Find_Node(pose[0], pose[1]);
            if (safe(pose[0], pose[1]))
            {
                n_cnt++;
                if (a[j] == SS[j])
                    cnt++;
            }
        }
        // (���� ���� ���� / ��� ����)
        rv_cnt += cnt / n_cnt;
        cnt = n_cnt = 0;
    }
    for (int i = 0; i < N; i++)
    {
        // ���� ��ġ ����
        if (a[i] == SS[i])
            rv_cnt++;
    }
    return rv_cnt;
}

char BFS_Search_Priority()
{
    int i, j, puckCnt[2] = { 0, };

    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++)
            // Ȧ ��尡 �ƴϸ鼭 ������ΰ� üũ
            if (SortNode[i][0][j] != HoleNode && SS[SortNode[i][0][j]] != M)
                puckCnt[i]++;

    // ��ֹ��� Ȧ�� C3, C4, C7, C8�� �ִ°��
    if (puckCnt[0] == puckCnt[1])
    {
        // �Ʒ��� ���������� ó�� ���ߴ°� �޸� �Ҹ� ũ�ٴ� �����Ͽ� �ۼ���
        if (IP[1] == M)return 1;
        if (IP[2] == M)return 0;
        if (IP[9] == M)return 1;
        if (IP[10] == M)return 0;
        if (IP[4] == M)return 1;
        if (IP[7] == M)return 0;
    }
    else
    {
        if (puckCnt[0] < puckCnt[1])
            return 0;
        return 1;
    }
}

char BFS_IsPartSame(char a[], char mode)
{
    int i, j;
    int isTragetColor = 0;
    int NodeCnt = 0;
    int CornerNode[4] = { 0, 3, 8, 11 };

    // Ȧ�� ���� ���� ���� ����� �����ϴ� case�� �����ϱ� ���� �����Դϴ�
    // Ȧ ��尡 M�϶��� �ش� ������ �����ִٴ°��� �ǹ������� ������ �����ϰ� �Ѿ�ϴ�
    if (
        a[HoleNode] == M ||
        a[HoleNode] != M && a[HoleNode] == 0)
    {
        // TragetNode�� ���� ������ �����̿� �ش� �ܵ��� ������ �������� ���� �ʽ��ϴ�.
        if (mode == 1)
        {
            // ���ع� ���� �ִ� ���� �����̶�� Ȧ����Ű��, �߿��Ѱ� ���õ� �����̶�� ���̴� �Ժη� OrigSS�� ���Ƽ��� �ȵȴ� ��¥ �����̱� ����
            for (i = 0; i < 3; i++)
                if (a[SortNode[SortDir][1][i]] == M || SortNode[SortDir][1][i] == HoleNode)
                    if (
                        // ���信 ���� �����ϰ�
                        SS[SortNode[SortDir][0][i]] &&
                        // M�� �ƴϸ�
                        SS[SortNode[SortDir][0][i]] != M &&
                        // ����� ��ġ�Ѵٸ�
                        SS[SortNode[SortDir][0][i]] == a[SortNode[SortDir][0][i]])
                    {
                        BeforeCornerNodeValue[BeforeCornerNodeCnt][0] = SortNode[SortDir][0][i];
                        BeforeCornerNodeValue[BeforeCornerNodeCnt][1] = a[SortNode[SortDir][0][i]];
                        BeforeCornerNodeCnt++;

                        a[SortNode[SortDir][0][i]] = M;
                        SS[SortNode[SortDir][0][i]] = M;

                        copy(IP, a);
                        return 1;
                    }

            for (i = 0; i < TragetStep; i++)
                if (a[SortNode[SortDir][0][i]] == SS[SortNode[SortDir][0][i]])
                    NodeCnt++;
            if (NodeCnt >= TragetStep)
            {
                IsBFSExit = 1;
                copy(IP, a);
                return 1;
            }
        }
        // �ݴ��ʿ��� �ʿ��� ���� ���;��մϴ�. �ݴ��� ������ ������� 2�� ������ Ż��(��, Ȧ������ ��������� �� �� ����)
        else if (mode == 2)
        {
            for (i = 0; i < 3; i++)
                if (TempStorageIP[i] == 0 && SortNode[!SortDir][0][i] != HoleNode)
                    NodeCnt++;

            for (i = 1; i < 3; i++)
                for (j = 0; j < 3; j++)
                    if (a[SortNode[!SortDir][i][j]] == 0 && SortNode[!SortDir][i][j] != HoleNode)
                        NodeCnt++;

            if (NodeCnt >= 2)
            {
                copy(IP, a);
                return 1;
            }
        }
        // �ݴ��ʱ������� ���� ������ ���� �����;��մϴ�. ���� �������� ������� 2���� ���� �ʿ��� ���� �ش� ���� ������ Ż��
        else if (mode == 3)
        {
            for (i = 1; i < 3; i++)
                for (j = 0; j < 3; j++)
                    if (a[SortNode[!SortDir][i][j]] == TragetColor)
                        isTragetColor = 1;

            for (i = 0; i < 3; i++)
                if (TempStorageIP[i] == 0 && SortNode[SortDir][0][i] != HoleNode)
                    NodeCnt++;

            for (i = 1; i < 3; i++)
                for (j = 0; j < 3; j++)
                    if (a[SortNode[SortDir][i][j]] == 0 && SortNode[SortDir][i][j] != HoleNode)
                        NodeCnt++;

            if (NodeCnt >= 2 && isTragetColor == 1)
            {
                copy(IP, a);
                return 1;
            }
        }
        // Ȧ�� ������ ���� Ż��
        else if (mode == 4)
        {
            if (a[HoleNode] == 0)
            {
                copy(IP, a);
                return 1;
            }
        }
        // ���������� ���⶧ ������ �ִ� ���� ��� Mó���ϸ鼭 �޸� ����ȭ�ϰ� ���������̸� BFS ���� ���� ��Ű�� ����
        else if (mode == 5)
        {
            // ��������� ��ġ�Ѵٸ�
            if (same(a, SS))
            {
                IsBFSExit = 1;
                copy(IP, a);
                return 1;
            }

            // ������ ������ �´ٸ�
            for (i = 0; i < 4; i++)
            {
                if (
                    // ���信 ���� �����ϰ�
                    SS[CornerNode[i]] &&
                    // M�� �ƴϸ�
                    SS[CornerNode[i]] != M &&
                    // ����� ��ġ�Ѵٸ�
                    SS[CornerNode[i]] == a[CornerNode[i]])
                {
                    a[CornerNode[i]] = M;
                    SS[CornerNode[i]] = M;
                    copy(IP, a);
                    return 1;
                }
            }
        }
        // Traget���� ���߷��� �ϴ� ���� 0 ~ 1 step�Ÿ� ������ ������ Ż��
        else if (mode == 6)
        {
            // Worst Case�� 800�� ���� ���ü� �������� ��� �����ϴ�.
            if (TragetStep == 1)
            {
                if (a[SortNode[SortDir][0][0]] == TragetColor ||
                    a[SortNode[SortDir][0][1]] == TragetColor ||
                    a[SortNode[SortDir][1][0]] == TragetColor ||
                    a[SortNode[SortDir][1][1]] == TragetColor)
                {
                    copy(IP, a);
                    return 1;
                }
            }
            else if (TragetStep == 2)
            {
                if (a[SortNode[SortDir][0][0]] == TragetColor ||
                    a[SortNode[SortDir][0][1]] == TragetColor ||
                    a[SortNode[SortDir][0][2]] == TragetColor ||
                    a[SortNode[SortDir][1][0]] == TragetColor ||
                    a[SortNode[SortDir][1][1]] == TragetColor ||
                    a[SortNode[SortDir][1][2]] == TragetColor)
                {
                    copy(IP, a);
                    return 1;
                }
            }
            else if (TragetStep == 3)
            {
                if (a[SortNode[SortDir][0][1]] == TragetColor ||
                    a[SortNode[SortDir][0][2]] == TragetColor ||
                    a[SortNode[SortDir][1][1]] == TragetColor ||
                    a[SortNode[SortDir][1][2]] == TragetColor)
                {
                    copy(IP, a);
                    return 1;
                }
            }
        }
        // ó���� ���� ��Ī�̶� Ȧ��带 ������ ������� �ݵ�� 2�� ������ֱ� ���� �ݴ������� ���� ������ Ż��
        // �� SortDir�� �ݴ������ 0��° ���� ��� ������ �������� Ż����
        else if (mode == 7)
        {
            for (i = 0; i < 3; i++)
                if (a[SortNode[!SortDir][0][i]] != 0)
                    NodeCnt++;
            if (NodeCnt >= 3)
            {
                copy(IP, a);
                return 1;
            }
        }
    }
    return 0;
}

char BFS_Run(char mode)
{
    int i, debugView = 0;

    if (debugView == 1)
        VisualStudio_BFS_Debug(0, mode);

    int state = BFS(mode);

    if (debugView == 1)
        VisualStudio_BFS_Debug(1, mode);

    if (state == 1)
    {
        printf("Success: %3d\n", cnt);
        Link(rear - 1, 1);

# if 0
        printf("* sp : %2d\n", sp - BeforeSP);
        for (i = BeforeSP; i < sp; i++)
        {
            Path_cnt = 0;
            Path_S(i);
            printf("from %2d  to %2d\n", stack[i][0], stack[i + Path_cnt][1]);
            i += Path_cnt;
        }
#endif
        BeforeSP = sp;
    }
    if (state == 2)
    {
        printf("Queue EMPTY: %3d\n", cnt);
        // getch();
    }
    if (state == 3)
    {
        printf("Memory Overflow: %3d\n", cnt);
        getch();
    }
    if (debugView == 1)
        printf("\n\n\n\n\n\n");
    return state;
}

void BFS_Mission()
{
    int i;

    IsBFSExit = 0;
    BeforeCornerNodeCnt = 0;
    sp = 0;
    BeforeSP = 0;

    copy(OrigSS, SS);
    Shift_SS_To_Corner();

    printf("* OrigIP\n"); draw(IP);
    printf("* OrigSS\n"); draw(OrigSS);
    printf("* SortSS\n"); draw(SortSS);
    printf("* Hole: %d\n\n", HoleNode);

    // Ȧ���� ���� �ִٸ� ������ ġ���
    if (IP[HoleNode] != 0)
        BFS_Run(4);

    SortDir = BFS_Search_Priority();

    printf("* SortDir: %d\n\n", SortDir);

    // 0���� ���õǰ� 0���� ������ 1���� 3���� ������ Ż��
    Change_To_3X3(0, SortDir);
    BFS_Run(7);
    Change_To_3X3(1, SortDir);

    // ���ϴ� ���� �������� Dir�� �ݴ��
    // ���ϴ� ���� Ž���Ҷ��� Dir�� �״��
    Change_To_3X3(0, !SortDir);
    for (i = 0; i < 3; i++)
    {
        TragetStep = i + 1;
        TragetNode = SortNode[SortDir][0][i];
        TragetColor = SS[SortNode[SortDir][0][i]];
        if (SortNode[SortDir][0][i] !=
            HoleNode &&
            IP[SortNode[SortDir][0][i]] !=
            SS[SortNode[SortDir][0][i]])
        {
            if (Is_There_Target_Puck(SortDir, TragetColor) == 0)
            {
                // �ݴ��� �ʿ��� �� ���� �� �ֵ��� ���� ������ֱ�
                BFS_Run(2);
                // �ݴ��ʿ��� ���� ������ �� ������
                Change_To_3X3(1, !SortDir);
                Change_To_3X3(0, SortDir);

                BFS_Run(3);

                Change_To_3X3(1, SortDir);
                Change_To_3X3(0, !SortDir);
            }

            // �����;��ϴ� ���� M�� �ƴѰ��
            if (TragetColor != M)
                BFS_Run(6);

            // Traget��ġ�� �°� �� ��ġ
            IsBFSExit = 0;
            while (IsBFSExit == 0)
                BFS_Run(1);
        }
    }
    Change_To_3X3(1, !SortDir);

    // ���ߴ� �� ����
    SortDir = !SortDir;
    // �ݴ��� BFS
    Change_To_3X3(0, !SortDir);

    // �޸� ����ȭ�� ���ؼ� ���� �����ϴ�
    for (i = 0; i < 3; i++)
    {
        TragetStep = i + 1;
        TragetNode = SortNode[SortDir][0][i];
        TragetColor = SS[SortNode[SortDir][0][i]];

        // �����;��ϴ� ���� M�� �ƴѰ��
        if (TragetColor != M)
            BFS_Run(6);

        IsBFSExit = 0;
        while (IsBFSExit == 0)
            BFS_Run(1);
    }
    Change_To_3X3(1, !SortDir);

    // ��¥ ���信�� M���� �ִ� ���� ������ ��� Mó���Ѱ� �����ϴ� ����
    for (i = 0; i < BeforeCornerNodeCnt; i++)
        IP[BeforeCornerNodeValue[i][0]] = BeforeCornerNodeValue[i][1];

    IsFinal_BFS_Success = 1;
    IsFinal_BeforeSP = sp;
    copy(OrigIP, IP);
    copy(IP, OrigIP);
    copy(SS, OrigSS);

    // ���� BFS
    IsBFSExit = 0;
    while (IsBFSExit == 0)
    {
        if (BFS_Run(5) != 1)
        {
            IsFinal_BFS_Success = 0;
            break;
        }
    }

    // ���� BFS ���н� �޸���ƽ ������� �ʰ� ��Ž��
    if (IsFinal_BFS_Success == 0)
    {
        printf("heuristic off BFS Ž��\n");
        copy(IP, OrigIP);
        copy(SS, OrigSS);
        sp = IsFinal_BeforeSP;

        IsBFSExit = 0;
        while (IsBFSExit == 0)
            BFS_Run(5);
    }
}

#pragma endregion

#pragma region Standard / BFS-location-queue2

char Change(char value, char mode)
{
    if (mode == 0)return (value / 4) + 1;// X
    if (mode == 1)return (value % 4) + 1;// Y
    if (mode == 2)return (value + 2) % 4;// W
    return 0;
}

char Find_Node(char x, char y)
{
    return ((x - 1) * 4) + y - 1;
}

char safe(char x, char y)
{
    if (x > 0 && x < 4 && y > 0 && y < 5)return 1;
    return 0;
}

void Pose_init(int Dir1)
{
    if (Dir1 == 0)pose[0] -= 1;
    if (Dir1 == 1)pose[1] += 1;
    if (Dir1 == 2)pose[0] += 1;
    if (Dir1 == 3)pose[1] -= 1;
}

void Path_S(int i)
{
    if (stack[i][1] == stack[i + 1][0])
    {
        Path_cnt++;
        Path_S(i + 1);
    }
}

void push(char x, char y)
{
    stack[sp][0] = x;
    stack[sp][1] = y;
    sp++;
}

void Link(int a, int mode)
{
    if (a > 0)Link(Parent[a], mode);
    else
    {
        copy(temp_IP, IP);
        return;
    }
    if (mode)push(Queue[a][0], Queue[a][1]);
    swap(temp_IP, Queue[a][0], Queue[a][1]);
}

void copy(char a[], char b[])
{
    for (int i = 0; i < N; i++)
        a[i] = b[i];
}

void swap(char arr[], char i, char j)
{
    char temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void EnQueue(char arr[])
{
    if (rear < MAX)
    {
        for (int i = 0; i < 2; i++)
            Queue[rear][i] = arr[i];
        rear++;
    }
}

char* DeQueue()
{
    if (front < rear)
        return Queue[front++];
    else
        return NULL;
}

char same(char a[], char b[])
{
    for (int i = 0; i < N; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

char S_same(char a[])
{
    for (int i = 0; i < rear; i++)
    {
        Link(i, 0);
        if (same(temp_IP, a))
            return 1;
    }
    return 0;
}

char BFS(char mode)
{
    char i, j, D;
    char a[N] = { 0, }, b[N] = { 0, }, * p;
    char point[2] = { 0, };
    char my[2];

    int forst = 0;
    int foren = 0;

    cnt = front = rear = 0;
    EnQueue(point);
    while (1)
    {
        // printf("depth : %d\r", cnt);
        p = DeQueue();
        if (p == NULL)return 2;
        if (cnt >= MAX)return 3;
        Link(front - 1, 0);
        copy(a, temp_IP);
        if (BFS_IsPartSame(a, mode))return 1;

        // ó�� �����ϴ� BFS Ž���� �����ϰ� HoleNode�� ���� �ִٸ�
        if (a[HoleNode] != 0 && a[HoleNode] != M && mode != 4)
        {
            // HoleNode�� Ž���Ͽ� ġ�쵵����
            forst = HoleNode;
            foren = HoleNode + 1;
        }
        // ���� BFS Ž��
        else
        {
            forst = 0;
            foren = N;
        }

        for (i = forst; i < foren; i++)
        {
            if (a[i] && a[i] != M)
            {
                my[0] = Change(i, 0);
                my[1] = Change(i, 1);
                for (D = 0; D < 4; D++)
                {
                    pose[0] = my[0];
                    pose[1] = my[1];
                    Pose_init(D);
                    j = Find_Node(pose[0], pose[1]);
                    if (a[j] == 0 && safe(pose[0], pose[1]))
                    {
                        copy(b, a);
                        swap(b, i, j);
                        if (S_same(b) == 0)
                        {
                            if (
                                // ��ü�� �������� ó�� BFS Ž��
                                (IsFinal_BFS_Success == 1 && mode == 5 && heuristic(b) - heuristic(a) > -3)
                                ||
                                // �κ������� BFS Ž�� or ��ü�� �������� BFS ������ Ž��
                                (IsFinal_BFS_Success == 0 || mode != 5)
                                )
                            {
                                cnt++;
                                Parent[rear] = front - 1;
                                point[0] = i;
                                point[1] = j;
                                EnQueue(point);
                                if (BFS_IsPartSame(b, mode))return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

#pragma endregion

#pragma region BFS Debug Function

void VisualStudio_BFS_Debug(int printfMode, int mode)
{
    if (printfMode == 0)
    {
        printf("+------------------+\n");
        printf("|  BFS Management  |\n");
        printf("+------------------+\n");
        printf("| HoleNode   | %3d |\n", HoleNode);
        printf("| mode       | %3d |\n", mode);
        printf("| sp         | %3d |\n", sp);
        printf("| TragetStep | %3d |\n", TragetStep);
        printf("+------------------+\n");
        printf("|        IP        |\n");
        printf("+------------------+\n");
        for (int i = 0; i < 3; i++)
        {
            for (int j = (i * 4); j < (i * 4) + 4; j++)
            {
                if (j == (i * 4))
                    printf("|   %2d ", IP[j]);
                else if (j == ((i * 4) + 4) - 1)
                    printf("%2d    |", IP[j]);
                else
                    printf("%2d ", IP[j]);
            }
            printf("\n");
        }
        printf("+------------------+\n");
        printf("|        SS        |\n");
        printf("+------------------+\n");
        for (int i = 0; i < 3; i++)
        {
            for (int j = (i * 4); j < (i * 4) + 4; j++)
            {
                if (j == (i * 4))
                    printf("|   %2d ", SS[j]);
                else if (j == ((i * 4) + 4) - 1)
                    printf("%2d    |", SS[j]);
                else
                    printf("%2d ", SS[j]);
            }
            printf("\n");
        }
        printf("+------------------+\n");
    }
    else if (printfMode == 1)
    {
        printf("|   BFS After IP   |\n");
        printf("+------------------+\n");
        for (int i = 0; i < 3; i++)
        {
            for (int j = (i * 4); j < (i * 4) + 4; j++)
            {
                if (j == (i * 4))
                    printf("|   %2d ", IP[j]);
                else if (j == ((i * 4) + 4) - 1)
                    printf("%2d    |", IP[j]);
                else
                    printf("%2d ", IP[j]);
            }
            printf("\n");
        }
        printf("+------------------+\n");
    }
}

void draw(char* arr)
{
    // * Y ��
    // X
    // ��

    // ����(X)
    for (int i = 0; i < 3; i++)
    {
        // ����(Y)
        for (int j = (i * 4); j < (i * 4) + 4; j++)
        {
            printf("%2d ", arr[j]);
        }
        printf("\n");
    }
    printf("\n");
}

void show_move(char* arr)
{
    int i;

    printf("press any key to see moves\n");
    getch();
    for (i = 0; i < sp; i++)
    {
        swap(arr, stack[i][0], stack[i][1]);
        draw(arr);
        _sleep(500); // 0.5��
    }
    printf("Success!!\n");
}

#pragma endregion

void main()
{
    copy(move_arr, IP);

    BFS_Mission();

    printf("\n");
    printf("* MoveStack\n");
    printf("* sp: %d\n", sp);
    for (int i = 0; i < sp; i++)
    {
        Path_cnt = 0;
        Path_S(i);
        printf("from %2d  to %2d\n", stack[i][0], stack[i + Path_cnt][1]);
        i += Path_cnt;
    }

    show_move(move_arr);
}