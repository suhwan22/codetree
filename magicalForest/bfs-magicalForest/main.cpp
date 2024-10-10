#include <iostream>
#include <map>
#include <queue>

using namespace std;

int forest[100][100] = {};
int robot[1001] = {};
int dir[1001] = {};
pair<int ,int> E[1001] = {};

int dx[5] = {-1, 0, 1, 0, 0};
int dy[5] = {0, 1, 0, -1, 0};

int r, c;

bool isValidPos(int x, int y, int n);
void setRobot(int x, int y, int n);
void Down(int x, int y, int n);
void Right(int x, int y, int n);
void Left(int x, int y, int n);
long long calScore(int x, int y);
void resetForest();
void printForest();

int main()
{
  ios::sync_with_stdio(0);
  cin.tie(0);

  int k;
  cin >> r >> c >> k;
  for (int i = 1; i <= k; i++)
  {
    // dir 0 1 2 3 북동남서
    cin >> robot[i] >> dir[i];
  }

  int ret = 0;
  int x, y;
  for (int i = 1; i <= k; i++)
  {
    x = 1;
    y = robot[i];
    setRobot(x, y, i);
    cout << "x: " << x << ", y: " << y<< "\n";
    while (1)
    {
      if (isValidPos(x + 1, y, i))
      {
        Down(x, y, i);
        x++;
      }
      else if (isValidPos(x, y - 1, i) && isValidPos(x + 1, y - 1, i))
      {
        Left(x, y, i);
        y--;
        Down(x, y, i);
        x++;
      }
      else if (isValidPos(x, y + 1, i) && isValidPos(x + 1, y + 1, i))
      {
        Right(x, y, i);
        y++;
        Down(x, y, i);
        x++;
      }
      else
        break;
      printForest();
    }
    if (x < 4)
      resetForest();
    else
      ret += calScore(x, y);
  }
  cout << ret;
  return 0;
}

bool isValidPos(int x, int y, int n)
{
  int nx, ny;
  for (int i = 0; i < 4; i++)
  {
    nx = x + dx[i];
    ny = y + dy[i];
    if (nx < 0 || ny < 1 || nx >= r + 3 || ny > c) return false;
    if (forest[nx][ny] != 0 && forest[nx][ny] != n) return false;
  }
  return true;
}

void setRobot(int x, int y, int n)
{
  int nx, ny;
  for (int i = 0; i < 5; i++)
  {
    nx = x + dx[i];
    ny = y + dy[i];
    forest[nx][ny] = n;
  }
  E[n] = {x + dx[dir[n]], y + dy[dir[n]]};
}

void Down(int x, int y, int n)
{
  int nx, ny;
  for (int i = 0; i < 4; i++)
  {
    if (i == 2)
      continue;
    nx = x + dx[i];
    ny = y + dy[i];
    forest[nx][ny] = 0;
  }
  x = x + 1;
  for (int i = 0; i < 4; i++)
  {
    if (i == 0)
      continue;
    nx = x + dx[i];
    ny = y + dy[i];
    forest[nx][ny] = n;
  }
  E[n].first += 1;
  cout << "[Down] x: " << x << ", y: " << y << ", exit(" << E[n].first << ", " << E[n].second << ")\n";
}

void Right(int x, int y, int n)
{
  int nx, ny;
  for (int i = 0; i < 4; i++)
  {
    if (i == 1)
      continue;
    nx = x + dx[i];
    ny = y + dy[i];
    forest[nx][ny] = 0;
  }
  y = y + 1;
  for (int i = 0; i < 4; i++)
  {
    if (i == 3)
      continue;
    nx = x + dx[i];
    ny = y + dy[i];
    forest[nx][ny] = n;
  }

  int d = dir[n];
  dir[n] = d != 3 ? d + 1 : 0;
  E[n].first = x + dx[dir[n]];
  E[n].second = y + dy[dir[n]];
  cout << "[Right] x: " << x << ", y: " << y << ", exit(" << E[n].first << ", " << E[n].second << ")\n";
}

void Left(int x, int y, int n)
{
  int nx, ny;
  for (int i = 0; i < 4; i++)
  {
    if (i == 3)
      continue;
    nx = x + dx[i];
    ny = y + dy[i];
    forest[nx][ny] = 0;
  }
  y = y - 1;
  for (int i = 0; i < 4; i++)
  {
    if (i == 1)
      continue;
    nx = x + dx[i];
    ny = y + dy[i];
    forest[nx][ny] = n;
  }

  int d = dir[n];
  dir[n] = d != 0 ? d - 1 : 3;
  E[n].first = x + dx[dir[n]];
  E[n].second = y + dy[dir[n]];
  cout << "[Left] x: " << x << ", y: " << y << ", exit(" << E[n].first << ", " << E[n].second << ")\n";
}

long long calScore(int x, int y)
{
  queue<pair<int, int>> q;
  pair<int, int> cur;
  bool v[100][100] = {};
  q.push({x, y});
  v[x][y] = true;
  int nx, ny;
  int score = 0;
  while (!q.empty())
  {
    cur = q.front();
    v[cur.first][cur.second] = true;
    if (x == 5 && y == 3)
    {
      cout << "cur(" << cur.first << ", " << cur.second << ")\n";
    }
    q.pop();
    for (int i = 0; i < 4; i++)
    {
      nx = cur.first + dx[i];
      ny = cur.second + dy[i];
        if (x == 5 && y == 3)
        {
          cout << "(nx, ny) = (" << nx << ", " << ny << ") before \n";
        }
      if (nx < 0 || ny < 1 || nx >= r + 3 || ny > c) continue;
        if (x == 5 && y == 3)
        {
          cout << "(nx, ny) = (" << nx << ", " << ny << ") out of bound\n";
        }
      if (v[nx][ny] || forest[nx][ny] == 0) continue;
        if (x == 5 && y == 3)
        {
          cout << "(nx, ny) = (" << nx << ", " << ny << ") vistied\n";
        }
      if (forest[cur.first][cur.second] != forest[nx][ny])
      {
        // 다른 로봇을 볼때
        if (x == 5 && y == 3)
        {
          cout << "(nx, ny) = (" << nx << ", " << ny << ")\n";
        }

        if (cur == E[forest[cur.first][cur.second]])
        {
          // 현재 출입구에 마침 있었으면 가능
          if (score < nx - 2)
            score = nx - 2;
          q.push({nx, ny});
        }
      }
      else
      {
        // 현재 있는 로봇의 안쪽을 보는 경우
        if (score < nx - 2)
          score = nx - 2;
        q.push({nx, ny});
      }
    }
  }
  cout << "[calScore] " <<  score << "\n";
  return score;
}

void resetForest()
{
  for (int i = 0; i < r + 3; i++)
  {
    for (int j = 0; j <= c; j++)
      forest[i][j] = 0;
  }
}

void printForest()
{
  cout << "-----------------------\n";

  for (int i = 0; i < r + 3; i++)
  {
    cout << "[" << i << "]";
    for (int j = 1; j <= c; j++)
    {
      cout << forest[i][j] << " ";
    }
    cout << "\n";
  }
  cout << "-----------------------\n";
}