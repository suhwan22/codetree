// 1 ~ R 행 1-index
// 위쪽은 모두 입구
// K 명의 정령이 각자 골렘을 가지고 출발, 골렘 크기 위아래오른쪽왼쪽중앙 5개의 칸을 차지함 중앙 빼고는 출입구
// 내릴때는 정해준 출구 탑승은 네 곳 중 아무곳 가능
// 탐색 i 번째 골렘은 숲의 가장 북쪽(r == 1) 에서부터 Ci 가 중앙에 오도록
// 즉 (1, ci) 가 중앙에 오도록 시작 처음 위쪽은 맵 밖이네? 근데 idx 0 있으니 segfalut는 안날듯
// 초기 출구는 di 로 정해짐

// 탐색 순서
// 1. 아래로 한칸 r++, 갈 공간이 있는 경우만 가능
// 2. 아래로 못 가면, 왼쪽으로 돌아서(회전임에 주의, 출구가 반시계로 돔) 내려감
// 3. 왼쪽으로도 못가면 오른쪽으로 돌아서(회전에 주의, 출구가 시계로 돈다서 내려감
//
// 골렘이 내려갈 수 있는 가장 밑으로 간 경우엔 좌우로만 움직일 수 있고, 골렘의 출구가 다른 골렘의 출구와 만난 경우에 골렘을 옮기기 가능
// 끝나는 건 정령 기준으로 모두가 움직임이 끝나야함, 골렘이 다 움직였으면 출구로 다른 골렘 옮겨 타서 더 내려갈수 있으면 가야함
// 최대한 밑으로 가야함
// 최종적으로 움직임이 다 멈춤 정령들의 최종 위치의 r 값을 다 더하는게 목적
// 더이상 못 움직이는 상태인데 골렘이 숲 밖에 있으면 걔는 정답에 포함하지 않고 숲을 리셋하고 다음 골렘부터 시작
// 이게 들어와 있지않은 상태가 있네? 흠 중앙이 -1 행인 경우가 있는데 어케할까 initial 해주는 부분도 넣어야 할듯?

//
// 주의 codetree 에 제출할 경우 틀린다!!!!
// dp 로 점수를 계산했는데 테케 33 에서 틀림
// 하지만 이유를 알지 못함...
//


#include <iostream>
#include <map>

using namespace std;

int robot[1001] = {};
int dir[1001] = {};
int forest[100][100] = {};
int score[1001] = {};
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

int r, c;

bool isValidPos(int x, int y, int i);
void rotLeft(int x, int y, int i);
void rotRight(int x, int y, int i);
void goDown(int x, int y, int i);
int calPos(int x, int y, int i);
void printForest();
void resetForest();

int main()
{
  ios::sync_with_stdio(0);
  cin.tie(0);

  int k;
  // 5 <= r,c <= 70 행,열
  // 1 <= k <= 1000 출발할 골렘의 수

  cin >> r >> c >> k;
  for (int i = 1; i <= k; i++)
  {
    // Ci 과 Di 인데
    // 2 <= Ci <= c - 1
    // 양 끝에서는 시작 안하네
    // d = 0 1 2 3 == 북동남서

    cin >> robot[i] >> dir[i];
  }

  int x, y;
  int ret = 0;
  for (int i = 1; i <= k; i++)
  {
    x = -1;
    y = robot[i];
    while (1)
    {
      if (isValidPos(x + 1, y, i))
      {
        goDown(x, y, i);
        x = x + 1;
    printForest();
      }
      else if (isValidPos(x, y - 1, i) && isValidPos(x + 1, y - 1, i))
      {
        rotLeft(x, y, i);
        goDown(x, y - 1, i);
        x = x + 1;
        y = y - 1;
    printForest();
      }
      else if (isValidPos(x, y + 1, i) && isValidPos(x + 1, y + 1, i))
      {
        rotRight(x, y, i);
        goDown(x, y + 1, i);
        x = x + 1;
        y = y + 1;
    printForest();
      }
      else
        break;
    }
    if (x >= 2)
      ret += calPos(x, y, i);
    else
      resetForest();
  }
  cout << ret;

  return 0;
}

bool isValidPos(int x, int y, int i)
{
  // 지금 있는 위치가 괜찮은 가?
  cout << "[isValidPos] " << "x: " << x << ",y: " << y << ",i: " << i << "\n";
  // m, d, u, l, r
  int xPos[5] = {x, x + 1, x - 1, x, x};
  int yPos[5] = {y, y, y, y - 1, y + 1};
  if (x == 0)
  {
    // x == 0 이면 이제 막 출발
    if (forest[xPos[1]][yPos[1]] != 0)
      return false;
  }
  else if (x == 1)
  {
    for (int j = 0 ; j < 5; j++)
    {
      if (j == 2)
        continue;
      if (xPos[j] < 1 || yPos[j] < 1 || xPos[j] > r || yPos[j] > c)
      {
        cout << "r: " << r << ", c: " << c << ", xPos: " << xPos[j] << ", yPos: " << yPos[j] << "\n";
        cout << "out of bound\n";
        return false;
      }
      if (forest[xPos[j]][yPos[j]] != 0 && forest[xPos[j]][yPos[j]] != i)
        return false;
    }
  }
  else
  {
    for (int j = 0; j < 5; j++)
    {
      if (xPos[j] < 1 || yPos[j] < 1 || xPos[j] > r || yPos[j] > c)
      {
        cout << "r: " << r << ", c: " << c << ", xPos: " << xPos[j] << ", yPos: " << yPos[j] << "\n";
        cout << "out of bound\n";
        return false;
      }
      if (forest[xPos[j]][yPos[j]] != 0 && forest[xPos[j]][yPos[j]] != i)
      {
        cout << "invalid pos\n";
        return false;
      }
    }
    // 일반적인 경우
  }
  return true;;
}

void goDown(int x, int y, int i)
{
  // 한칸 내려가기
  // (x, y) -> (x + 1, y)
  // i 는 i 번째 로봇

  if (x == -1)
  {
    forest[1][y] = i;
  }
  else if (x == 0)
  {
    forest[x + 1][y - 1] = i;
    forest[x + 1][y + 1] = i;
    forest[x + 2][y] = i;
  }
  else
  {
    forest[x - 1][y] = 0;
    forest[x][y - 1] = 0;
    forest[x][y + 1] = 0;

    forest[x + 1][y - 1] = i;
    forest[x + 1][y + 1] = i;
    forest[x + 2][y] = i;
  }
}

void rotRight(int x, int y, int i)
{
  // 한칸 오른쪽으로 가기
  // (x, y) -> (x, y + 1)
  // dir 시계로 회전
  // i 는 i 번째 로봇

  if (x == -1)
  {
    dir[i] = dir[i] != 3 ? dir[i] + 1 : 0;
  }
  else if (x == 0)
  {
    dir[i] = dir[i] != 3 ? dir[i] + 1 : 0;
    forest[x + 1][y] = 0;

    forest[x + 1][y + 1] = i;
  }
  else if (x == 1)
  {
    dir[i] = dir[i] != 3 ? dir[i] + 1 : 0;
    forest[x][y - 1] = 0;
    forest[x + 1][y] = 0;

    forest[x][y + 2] = i;
    forest[x + 1][y + 1] = i;
  }
  else
  {
    dir[i] = dir[i] != 3 ? dir[i] + 1 : 0;
    forest[x - 1][y] = 0;
    forest[x][y - 1] = 0;
    forest[x + 1][y] = 0;

    forest[x - 1][y + 1] = i;
    forest[x][y + 2] = i;
    forest[x + 1][y + 1] = i;
  }
}

void rotLeft(int x, int y, int i)
{
  // 한칸 왼쪽으로 가기
  // (x, y) -> (x, y - 1)
  // dir 반시계로 회전
  // i 는 i 번째 로봇

  if (x == -1)
  {
    dir[i] = dir[i] != 0 ? dir[i] - 1 : 3;
  }
  else if (x == 0)
  {
    dir[i] = dir[i] != 0 ? dir[i] - 1 : 3;
    forest[x + 1][y] = 0;

    forest[x + 1][y - 1] = i;
  }
  else if (x == 1)
  {
    dir[i] = dir[i] != 0 ? dir[i] - 1 : 3;
    forest[x][y + 1] = 0;
    forest[x + 1][y] = 0;

    forest[x][y - 2] = i;
    forest[x + 1][y - 1] = i;
  }
  else
  {
    dir[i] = dir[i] != 0 ? dir[i] - 1 : 3;
    forest[x - 1][y] = 0;
    forest[x][y + 1] = 0;
    forest[x + 1][y] = 0;

    forest[x - 1][y - 1] = i;
    forest[x][y - 2] = i;
    forest[x + 1][y - 1] = i;
  }
}

int calPos(int x, int y, int i)
{
  // 골렘 탐사 dfs? bfs? -> dp
  // (x, y) 정령 위치, i 번째 골렘
  // dir[i] 확인해서 근처에 골렘 있는지 보고 이동, 그 골렘의 dir 봐서 또 이동 반복 모든 지점
  // cur을 dir 로 받고 거기서 nx, ny 갈수있는지 보고 그 골렘의 dir 을 다시 cur로 받는 식?
  // 아니면 미리 i 번째 골렘이 몇점인지 계산 해놓고 새로운 친구가 i 번째 골렘 만나면 그친구 점수로 바로 결정

  pair<int ,int> cur;
  if (dir[i] == 0)
  {
    cur.first = x - 1;
    cur.second = y;
  }
  else if (dir[i] == 1)
  {
    cur.first = x;
    cur.second = y + 1;
  }
  else if (dir[i] == 2)
  {
    cur.first = x + 1;
    cur.second = y;
  }
  else if (dir[i] == 3)
  {
    cur.first = x;
    cur.second = y - 1;
  }
  int nx, ny;
  int pos = x + 1;
  for (int idx = 0; idx < 4; idx++)
  {
    nx = cur.first + dx[idx];
    ny = cur.second + dy[idx];
    if (nx < 1 || ny < 1 || nx > r || ny > c) continue;
    if (forest[nx][ny] <= 0 || forest[nx][ny] == i) continue;
    if (pos < score[forest[nx][ny]])
      pos = score[forest[nx][ny]];
  }
  score[i] = pos;
  cout << "score[" << i << "]" << pos << "\n";
  return pos;
}

void printForest()
{
  cout << "-----------------------\n";
  for (int i = 1; i <= r; i++)
  {
    for (int j = 1; j <= c; j++)
    {
      cout << forest[i][j] << " ";
    }
    cout << "\n";
  }
  cout << "-----------------------\n";
}

void resetForest()
{
  for (int i = 0; i <= r; i++)
  {
    for (int j = 0; j <= c; j++)
      forest[i][j] = 0;
  }
}