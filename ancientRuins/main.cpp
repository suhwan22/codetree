// 5 x 5 전체 맵
// 3 x 3 선택
// 시계 방향으로 90도씩 돌리기 가능
// 3개 이상 연결 되면 사라짐, 그 개수의 총합이 점수
// 사라진 곳에 새로운 블럭이 생기고 연쇄적으로 사라질 수 있고 그것도 점수
// 
// 회전 시켜 최대 점수를 얻어야 하고, 여러개라면 최소 회전, 그것도 여러개라면 중심의 좌표 c 가 가장 작은 거 c 도 같으면 r 이 적은거
// 새로운 유물이 생기는 거는 배열로 주어지고
// 앞에서 부터 하나씩 넣는데 넣는 규칙은 아래와 같다
// c 가 작은순 같다면 r 이 작은순
// 즉 왼쪽부터 오른쪽으로 채우는데 아래가 우선
//
// 한 번의 턴은 회전시키는 탐사 -> 연쇄 적으로 유물 사라지면서 점수 획득 -> 끝
// 한 번의 턴 마다 점수를 출력 해줘야하고
// 회전을 아무리 시켜도 점수를 얻을 수 없는 순간에 탐사는 종료 이때는 출력하지 않음

#include <iostream>
#include <map>
#include <queue>

using namespace std;

int board[5][5] = {};
int relics[300] = {};
int relicIndex = 0;
int m;
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

void rotBoard(int x, int y, int n);
void reBoard(int x, int y, int n);
int calScore(bool flag);
void setRelic();
void printBoard();

int main()
{
  ios::sync_with_stdio(0);
  cin.tie(0);

  int k;
  // 1 <= k: 탐사 횟수 <= 10
  // 10 <= m: 유물 개수 <= 300
  // 유물 번호는 1 ~ 7
  // 첫 번째 5x5 에서는 3개 연속된게 없음을 보장하고, 첫 번째 탐사에선 무조건 3개 조합이 생겨서 점수를 얻을 수 있음을 보장한다.

  cin >> k >> m;

  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
      cin >> board[i][j];
  }

  for (int i = 0; i < m; i++)
    cin >> relics[i];

  // (0, 0) (0, 1) (0, 2) (0, 3) (0, 4)
  // (1, 0) (1, 1) (1, 2) (1, 3) (1, 4)
  // (2, 0) (2, 1) (2, 2) (2, 3) (2, 4)
  // (3, 0) (3, 1) (3, 2) (3, 3) (3, 4)
  // (4, 0) (4, 1) (4, 2) (4, 3) (4, 4)

  int ans = 0;
  // printBoard();

  while (k--)
  {
    pair<int, int> pos;
    int max = 0;
    int rot = 0;
    int temp;
    ans = 0;
    for (int r = 0; r < 4; r++)
    {
      for (int j = 1; j < 4; j++)
      {
        for (int i = 1; i < 4; i++)
        {
          // cout << "(" << i << ", " << j << ") rot: " << r << "\n";
          rotBoard(i, j, r);
          temp = calScore(false);
          if (max < temp)
          {
            max = temp;
            rot = r;
            pos = {i, j};
          }
          reBoard(i, j, r);
        }
      }
    }
    if (max)
    {
      rotBoard(pos.first, pos.second, rot);
      // printBoard();
      // cout << "max: " << max << ", x: " << pos.first << ", y: " << pos.second << ", rot: " << rot << "\n";
      calScore(true);
      ans += max;
      while (max != 0)
      {
        setRelic();
        max = calScore(true);
        ans += max;
      }
      cout << ans << " ";
    }
  }


  return 0;
}

void rotBoard(int x, int y, int n)
{
  // 시계방향으로 n번 돌리기
  // 1 2 3
  // 4 5 6
  // 7 8 9

  // 7 4 1
  // 8 5 2
  // 9 6 3

  // (x-1, y-1)  (x-1,  y)  (x-1, y+1)
  // (  x, y-1)  (  x,  y)  (  x, y+1)
  // (x+1, y-1)  (x+1,  y)  (x+1, y+1)

  // (x+1, y-1) (  x, y-1)  (x-1, y-1)
  // (x+1,  y)  (  x,  y)  (x-1,  y)
  // (x+1, y+1) (  x, y+1) (x-1, y+1)

  // cout << "[rotBoard] x: " << x << ", y: " << y << "rot: " << n << "\n";
  int temp;
  for (int i = 0; i < n; i++)
  {
    temp = board[x-1][y-1];
    board[x-1][y-1] = board[x+1][y-1];
    board[x+1][y-1] = board[x+1][y+1];
    board[x+1][y+1] = board[x-1][y+1];
    board[x-1][y+1] = temp;
    temp = board[x-1][y];
    board[x-1][y] = board[x][y-1];
    board[x][y-1] = board[x+1][y];
    board[x+1][y] = board[x][y+1];
    board[x][y+1] = temp;
  }
}

void reBoard(int x, int y, int n)
{
  // 반시계로 n번 돌리기 돌리기
  // 1 2 3
  // 4 5 6
  // 7 8 9
  
  // 3 6 9
  // 2 5 8
  // 1 4 7

  // (x-1, y-1)  (x-1,  y)  (x-1, y+1)
  // (  x, y-1)  (  x,  y)  (  x, y+1)
  // (x+1, y-1)  (x+1,  y)  (x+1, y+1)

  // (x-1, y+1)  (  x, y+1)  (x+1, y+1)
  // (x-1,   y)  (  x,   y)  (x+1,   y)
  // (x-1, y-1)  (  x, y-1)  (x+1, y-1)

  // cout << "[reBoard] x: " << x << ", y: " << y << "rot: " << n << "\n";
  int temp;
  for (int i = 0; i < n; i++)
  {
    temp = board[x-1][y-1];
    board[x-1][y-1] = board[x-1][y+1];
    board[x-1][y+1] = board[x+1][y+1];
    board[x+1][y+1] = board[x+1][y-1];
    board[x+1][y-1] = temp;
    temp = board[x-1][y];
    board[x-1][y] = board[x][y+1];
    board[x][y+1] = board[x+1][y];
    board[x+1][y] = board[x][y-1];
    board[x][y-1] = temp;
  }
}

int calScore(bool flag)
{
  bool v[5][5] = {};
  pair<int, int> cur;
  vector<pair<int, int>> pos;
  vector<pair<int, int>> sel;
  int nx, ny;
  int cnt = 0;
  int ret = 0;
  for (int x = 0; x < 5; x++)
  {
    for (int y = 0; y < 5; y++)
    {
      if (v[x][y] == false)
      {
        cnt = 1;
        queue<pair<int, int>> q;
        q.push({x, y});
        pos.push_back({x, y});
        v[x][y] = true;
        while (!q.empty())
        {
          cur = q.front();
          q.pop();
          for (int i = 0; i < 4; i++)
          {
            nx = cur.first + dx[i];
            ny = cur.second + dy[i];
            if (nx < 0 || ny < 0 || nx >= 5 || ny >= 5)
              continue;
            if (v[nx][ny] || board[cur.first][cur.second] != board[nx][ny])
              continue;
            // cout << "[calScore] nx: " << nx << ", ny: " << ny << "\n";
            v[nx][ny] = true;
            cnt++;
            pos.push_back({nx, ny});
            q.push({nx, ny});
          }
        }
        if (cnt >= 3)
        {
          while (!pos.empty())
          {
            sel.push_back(pos.back());
            pos.pop_back();
          }
          ret += cnt;
        }
        else
        {
          while (!pos.empty())
            pos.pop_back();
        }
      }
    }
  }
  if (flag)
  {
    while (!sel.empty())
    {
      board[sel.back().first][sel.back().second] = 0;
      sel.pop_back();
    }
  }
  return ret;
}

void setRelic()
{
  // cout << "[setRelic] before\n";
  // printBoard();
  for (int j = 0; j < 5; j++)
  {
    for (int i = 4; i >= 0; i--)
    {
      if (board[i][j] == 0 && relicIndex < m)
        board[i][j] = relics[relicIndex++];
    }
  }
  // cout << "------------------\n";
  // printBoard();
  // cout << "[setRelic] after\n";
}

void printBoard()
{
  cout << "-------\n";
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      cout << board[i][j] << " ";
    }
    cout << "\n";
  }
  cout << "-------\n";
}