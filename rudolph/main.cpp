// N x N, (r, c) 로 표현 (1, 1) 최상단
// M번 진행
// 루돌프 한 번 움직이고, 나머지 산타들 움직인다.
// 기절해있거나 격자 밖으로 나가 탈락한 산타는 움직일 수 없다
// 
// 루돌프 이동
// 가장 인접한 산타를 향해 한 칸 이동, 산타가 여러명이면 r이 큰 것, r도 같으면 c가 큰 것 우선
// 대각상하좌우 8 방향으로 1칸 가능
//
// 산타 이동
// 1번 ~ P번 순서대로 움직인다
// 기절 or 탈락 산타는 움직일수 없다
// 산타는 루돌프에게 거리가 가장 가까워지는 방향으로 1칸 이동
// 산타는 서로 겹칠 수 없고, 게임판 밖으로 이동 불가
// 움직일 수 있는 칸이 없으면 움직이지 않는다
// 움직일 수는 있어도 어디를 가도 루돌프에게 가까워지지 않는다면 움직이지 않는다
// 상하좌우 4 방향만 가능, 여러개면 상우하좌 순으로 선택
//
// 충돌
// 산타와 루돌프가 같은 칸에 오면 충돌 발생
// 루돌프가 움직여 충돌하면, 산타는 C 만큼 점수를 얻음, 이때 산타는 루돌프가 이동해온 방향으로 C칸 만큼 밀려난다
// 산타가 루돌프에 박은 경우, 산타는 D 만큼 점수를 얻음. 산타는 자신이 이동한 방향 반대 쪽으로 D 만큼 튕겨남
// -> 충돌해서 튕겨 나간다고 보면 될듯
// 중간에 부딪히는건 무시하고 딱 그 칸만큼 이동
// 밀려난 곳이 게임판 밖이면 탈락
// 다른 산타랑 만나면 상호작용 발생
//
// 상호작용
// 원래 있던 산타쪽으로 튕겨 나올때 발생
// 원래 있던 산타를 1칸 그 방향으로 밀어냄, 그 옆에도 있으면 그 옆의 산타도 1칸 밀어버림
// 이렇게 해서 게임판 밖으로 밀려나도 탈락
//
// 기절
// 산타는 루돌프랑 충돌하면 기절함
// k번째 턴에 기절했으면, k + 1 턴에 아무것도 못하고, k + 2 부터 활동 가능
// 기절해도 상호작용으로 밀려날 수는 있다.
// 루돌프는 기절한 산타에게도 돌진할 수 있다.
//
// 게임 종료
// M 번의 턴으로 종료
// P 명의 산타가 모두 탈락하는 순간 M 번째 턴이 오지 않아도 종료
// 매 턴이 끝나면 탈락하지 않은 산타들에게 1점 부여됨
// 게임이 끝났을 때 각 산타가 얻은 최종 점수를 출력해야함


#include <iostream>
#include <cstdlib>
#include <queue>
#include <cmath>

#define INF 2147483647

using namespace std;

class Santa
{
  public:
    pair<int, int> pos;
    int en;
    int score;
    int stunned;
};

int board[51][51] = {};
Santa santa[31] = {};
int cntSanta;
int X, Y;
// 탐색용 dx, dy 상우하좌 대각 순서

int N, M, P, C, D;
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

void moveRudolph();
void moveSanta();
void rudolphCrash(int idx);
void santaCrash(int idx);
void printBoard();
void addAllSanta();
void wakeSanta();

int main()
{
  cin >> N >> M >> P >> C >> D;
  // 3 <= N: 게임판 크기 <= 50
  // 1 <= M: 게임 턴 수 <= 1000
  // 1 <= P: 산타 수 <= 30
  // 1 <= C: 루돌프 파워 <= N  -> 루돌프가 산타를 쳤을때 산타가 튕겨 날아가는 거리
  // 1 <= D: 산타 파워 <= N   -> 산타가 루돌프를 쳤을때 산타가 튕겨 날아가는 거리
  cntSanta = P;

  cin >> X >> Y;
  // 루돌프 위치 (1 <= X, Y <= N)

  int n, r, c;
  // n: 산타 번호, 산타의 시작 좌표 (r, c) 1 <= r, c <= N
  for (int i = 1; i <= P; i++)
  {
    cin >> n >> r >> c;
    santa[n].pos.first = r;
    santa[n].pos.second = c;
    santa[n].en = true;
    santa[n].stunned = 0;
    santa[n].score = 0;
    board[r][c] = n;
  }
  printBoard();

  while (M)
  {
    wakeSanta();
    moveRudolph();
    printBoard();
    moveSanta();
    printBoard();
    if (cntSanta == 0)
      break;
    addAllSanta();
    for (int i = 1; i <= P; i++)
      cout << santa[i].score << " ";
    cout << "\n";
    M--;
  }
  for (int i = 1; i <= P; i++)
    cout << santa[i].score << " ";
  // printScore();
}

void moveRudolph()
{
  int min = -1;
  int idx;
  int len;
  for (int i = 1; i <= P; i++)
  {
    if (!santa[i].en) continue;
    len = pow(santa[i].pos.first - X, 2) + pow(santa[i].pos.second - Y, 2);
    if (min == -1 || min > len)
    {
      idx = i;
      min = len;
    }
    else if (min == len && santa[idx].pos < santa[i].pos)
      idx = i;
  }

  int curX = X;
  int curY = Y;
  pair<int, int> cur = santa[idx].pos;

  // move
  cout << "[moveRudolph] move to santa[" << idx << "]\n";
  if (cur.first == X)
    Y += Y < cur.second ? 1 : -1;
  else if (cur.second == Y)
    X += X < cur.first ? 1 : -1;
  else
  {
    X += X < cur.first ? 1 : -1;
    Y += Y < cur.second ? 1 : -1;
  }

  if (board[X][Y] > 0)
  {
    X = curX;
    Y = curY;
    rudolphCrash(idx);
  }

  // 예외 부분
  if (X < 1 || Y < 1 || X > N || Y > N || board[X][Y] > 0)
  {
    cout << "[moveRudolph] something wrong, X: " << X << ", Y: " << Y 
      << " board[" << X << "][" << Y << "] = " << board[X][Y] << "\n";
    exit(1);
  }
}

void moveSanta()
{
  for (int i = 1; i <= P; i++)
  {
    if (santa[i].en == false || santa[i].stunned) continue;
    // move
    pair<int, int> cur = santa[i].pos;
    cout << "[moveSanta]\nidx: " << i << " x: " << cur.first << ", y: " << cur.second << "\n";
    int nx, ny;
    pair<int, int> next;
    bool flag = false;
    int min = pow(X - cur.first, 2) + pow(Y - cur.second, 2);
    int len;
    for (int j = 0; j < 4; j++)
    {
      nx = cur.first + dx[j];
      ny = cur.second + dy[j];
      if (nx < 1 || ny < 1 || nx > N || ny > N) continue;
      len = pow(X - nx, 2) + pow(Y - ny, 2);
      if (board[nx][ny] == 0 && min > len)
      {
        min = len;
        next = {nx, ny};
        flag = true;
      }
    }
    if (!flag)
      continue;
    nx = next.first;
    ny = next.second;
    if (nx == X && ny == Y)
      santaCrash(i);
    else
    {
      board[santa[i].pos.first][santa[i].pos.second] = 0;
      board[nx][ny] = i;
      santa[i].pos = {nx, ny};
    }
  }
}

void rudolphCrash(int idx)
{
  // (X, Y) -> (x, y) 으로 총돌 산타 C 만큼 그 방향으로 날아감 대각으로도 가겠지?
  pair<int, int> cur = santa[idx].pos;
  int dirX = cur.first - X;
  int dirY = cur.second - Y;

  // 예외 dirX, dirY 는 둘다 크기가 1 이어야함
  if (abs(dirX) > 1 || abs(dirY) > 1)
  {
    cout << "[rudolphCrash] something wrong\n";
    exit(1);
  }
  cout << "[rudolphCrash] crash santa[" << idx << "]\n";
  int nx, ny;
  nx = cur.first + C * dirX;
  ny = cur.second + C * dirY;
  santa[idx].stunned = M;
  santa[idx].score += C;
  board[cur.first][cur.second] = 0;
  X = cur.first;
  Y = cur.second;
  int temp;
  while (!(nx < 1 || ny < 1 || nx > N || ny > N) && board[nx][ny] != 0)
  {
    temp = board[nx][ny]; 
    board[nx][ny] = idx;
    santa[idx].pos = {nx, ny};
    idx = temp;
    nx += dirX;
    ny += dirY;
  }
  if (nx < 1 || ny < 1 || nx > N || ny > N)
  {
    santa[idx].en = false;
    cntSanta--;
    return;
  }
  santa[idx].pos = {nx, ny};
  board[nx][ny] = idx;
}

void santaCrash(int idx)
{
  // (x, y) -> (X, y) 으로 총돌 산타 D 만큼 그 방향으로 날아감
  pair<int, int> cur = santa[idx].pos;
  int dirX = cur.first - X;
  int dirY = cur.second - Y;

  // 예외 dirX, dirY 는 둘다 크기가 1 or 0 이어야함
  if (abs(dirX) > 1 || abs(dirY) > 1)
  {
    cout << "[santaCrash] something wrong\n";
    exit(1);
  }

  int nx, ny;
  nx = X + D * dirX;
  ny = Y + D * dirY;
  santa[idx].stunned = M;
  santa[idx].score += D;
  board[cur.first][cur.second] = 0;
  int temp;
  while (!(nx < 1 || ny < 1 || nx > N || ny > N) && board[nx][ny] != 0)
  {
    temp = board[nx][ny]; 
    board[nx][ny] = idx;
    santa[idx].pos = {nx, ny};
    idx = temp;
    nx += dirX;
    ny += dirY;
  }
  if (nx < 1 || ny < 1 || nx > N || ny > N)
  {
    santa[idx].en = false;
    cntSanta--;
    return;
  }
  santa[idx].pos = {nx, ny};
  board[nx][ny] = idx;
}

void addAllSanta()
{
  for (int i = 1; i <= P; i++)
  {
    if (santa[i].en)
    {
      santa[i].score++;
    }
  }
}

void wakeSanta()
{
  for (int i = 1; i <= P; i++)
  {
    if (santa[i].stunned - M >= 2)
      santa[i].stunned = 0;
  }
}

void printBoard()
{
  cout << "------------board-------------\n";
  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
    {
      if (i == X && j == Y)
        cout << "R" << " ";
      else
        cout << board[i][j] << " ";
    }
    cout << "\n";
  }
  cout << "------------board-------------\n";
}