#define rint register int
#define MAX 362880 //9!
#define MAX_HASH 1000000 
int dict[5][2] = { 0, 0, 1, 0, -1, 0, 0, -1, 0, 1 };
 
struct State
{
    int value;
    int ePos;
    int row;
    int col;
    int dict;
    int prev;
    int step;
};
State que[MAX];
 
struct Hash
{
    int value;
    int index;
    Hash()
    {
        value = 0;
        index = -1;
    }
};
Hash hash[MAX_HASH][6];
int front;
int tail;
 
bool CheckHash(int value)
{
    rint hashIndex = value % MAX_HASH;
    for (rint i = 0; i < 6; i++)
    {
        if (hash[hashIndex][i].value == value) return true;
        if (hash[hashIndex][i].index == -1)
        {
            hash[hashIndex][i].value = value;
            hash[hashIndex][i].index = tail;
            return false;
        }
    }
}
 
void BFS()
{
    front = tail = 0;
    State start, next;
    start.value = 123456780;
    start.row = -1;
    start.col = -1;
    start.dict = -1;
    start.step = 0;
    start.ePos = 0;
    CheckHash(start.value);
    que[tail++] = start;
 
    while (front < tail)
    {
        start = que[front++];
 
        rint i, j;
        for (i = 1; i <= 4; i++)
        {
            rint eRow = start.ePos / 3;
            rint eCol = start.ePos % 3;
            rint nRow = eRow + dict[i][0];
            rint nCol = eCol + dict[i][1];
            if (nRow >= 0 && nRow < 3 && nCol >= 0 && nCol < 3)
            {
                rint nPos = 3 * nRow + nCol;
                rint eMul = 1;
                rint nMul = 1;
                for (j = 0; j < start.ePos; j++) eMul *= 10;
                for (j = 0; j < nPos; j++) nMul *= 10;
 
                rint key = (start.value / nMul) % 10;
                rint newValue = start.value + key * eMul - key * nMul;
 
                if (!CheckHash(newValue))
                {
                    next = start;
                    next.dict = i;
                    next.ePos = nPos;
                    next.row = 3 - eRow;
                    next.col = 3 - eCol;
                    next.prev = front-1;
                    next.step = start.step + 1;
                    next.value = newValue;
                    que[tail++] = next;
                }
            }
        }
    }
}
 
bool isInit = false;
int solvePuzzle(int puzzle[4][4], int *row, int *column, int *directions)
{
    if (!isInit)
    {
        BFS();
        isInit = true;
    }
 
    rint i, j;
    rint value = 0;
    for (i = 1; i < 4; i++)
    {
        for (j = 1; j < 4; j++)
        {
            value = value * 10 + puzzle[i][j];
        }
    }
    rint hashIndex = value % MAX_HASH;
    rint index = 0;
    for (i = 0; i < 6; i++)
    {
        if (hash[hashIndex][i].index == -1) return -1;
        if (hash[hashIndex][i].value == value)
        {
            index = hash[hashIndex][i].index;
            break;
        }
    }
 
    rint step = 0;
    while (que[index].step != 0)
    {
        step++;
        row[step] = que[index].row;
        column[step] = que[index].col;
        directions[step] = que[index].dict;
        index = que[index].prev;
    }
    return step;
}
