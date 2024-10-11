// (v, u, w)
// v - u w : 도시v 와 도시u 를 잇는 간선 생성
// tree 형식
// 자기 자신을 잇는 간선 있을 수 있고, 여러개 가능
// 방향성은 없음
// 가중치(w) 는 거리인 듯? 정확하게 주어지질 않네?..
// 
// 여행 상품 리스트가 있어야함
// (id, revenue, dest)
// id: 고유값, revenue: 매, dest: 목적지
//
// COMMAND
// 100 n m v_1 u_1 w_1 . . . v_m u_m w_m
// n만큼 도시가 있는거고 0~n-1번, 1 <= n <= 2,000
// m개의 간선이 있는것         , 1 <= m <= 10,000
// 맨 처음 한번만 주어짐        , 1 <= w <= 100
//
// 200 id revenue dest
// 여행 상품 추가
// id 는 겹치지 않음을 보장함 최대 30,000번, 1 <= id <= 30,000
// revenue 는 매출                    , 1 <= revenue <= 1,000
// dest 는 목적 도시                   , 0 <= dest <= n -1
//
// 300 id
// id 번째 여행 상품 제거
// 있을때만 지움 최대 30,000번
//
// 400
// 최적 여행 상품 판매
// 여행 상품중에서 최적의 상품을 판매
// revenue - cost 를 최대로 해야하고 같으면 id 가 작은거 먼저
// 도착하지 못하거나 음수인건 당연히 판매 가능 상품이 아니고
// 판매 가능한게 하나도 없으면 -1 출력
// 판매 가능한게 있으면 해당 id 출력하고 제거
// 최대 30,000번 들어올 수 있다.
//
// 500 s
// 상품의 출발지를 s 로 변경,            0 <= s <= n - 1
// 처음은 0 이다.
// 이를 통해서 당연히 cost 들이 변경됨을 유의하자
// 최대 15 번 들어올 수 있다.

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>

using namespace std;

typedef struct node
{
  vector<int> enable;
  // 목적지, 거리
} node;

typedef struct item
{
  int id;
  int revenue;
  int dest;
  int dis;
  bool en;
} item;

node city[2000] = {};
int dis[2000][2000] = {};
item items[30001] = {};

int D[2000] = {};

int min;
bool visited[2000];

int start = 0;
int n, m;
int cntItem = 0;

void deleteItem(int id);
int bestItem();
void calDis();
void dfs(int dest, int cityId, int dis, int revenue);
void setD();

class cmp
{
  public:
    bool operator() (int a, int b)
    {
      if (items[a].en == false)
        return false;
      if (items[a].revenue - D[items[a].dest] > items[b].revenue - D[items[b].dest])
        return true;
      return false;
    }
};

priority_queue<int, vector<int>, cmp> pq;

int main()
{
  ios::sync_with_stdio(0);
  cin.tie(0);

  int q;
  // q: 명령어 개수 1 <= 1 <= 100,000
  cin >> q;

  int input;
  int v, u, w, id, revenue, dest;
  while (q--)
  {
    cin >> input;
    if (input == 100)
    {
      // 100 n m v_1 u_1 w_1 . . . v_m u_m w_m
      // n만큼 도시가 있는거고 0~n-1번, 1 <= n <= 2,000
      // m개의 간선이 있는것         , 1 <= m <= 10,000
      // 맨 처음 한번만 주어짐        , 1 <= w <= 100
      cin >> n >> m;
      for (int i = 0; i < m; i++)
      {
        cin >> v >> u >> w;
        for (int x = 0; x < n; x++)
        {
          for (int y = 0; y < n; y++)
          {
            if (x == y)
              dis[x][y] = 0;
            dis[x][y] = 2147483647;
          }
        }
        if (v == u)
          continue;
        else
        {
          if (find(city[u].enable.begin(), city[u].enable.end(), v) == city[u].enable.end())
          {
            city[u].enable.push_back(v);
            dis[u][v] = w;
          }
          else
          {
            if (w < dis[u][v])
              dis[u][v] = w;
          }

          if (find(city[v].enable.begin(), city[v].enable.end(), u) == city[v].enable.end())
          {
            city[v].enable.push_back(u);
            dis[v][u] = w;
          }
          else
          {
            if (w < dis[v][u])
              dis[v][u] = w;
          }
        }
      }
      setD();
      for (int i = 0; i < n; i++)
      {
        cout << "city[" << i << "]\n";
        for (int j = 0; j < (int)city[i].enable.size(); j++)
          cout << "(" << city[i].enable[j] << ", " << dis[i][city[i].enable[j]] << ") ";
        cout << "\n";
      }
    }
    else if (input == 200)
    {
      // 200 id revenue dest
      // 여행 상품 추가
      // id 는 겹치지 않음을 보장함 최대 30,000번, 1 <= id <= 30,000
      // revenue 는 매출                    , 1 <= revenue <= 1,000
      // dest 는 목적 도시                   , 0 <= dest <= n -1
      cin >> id >> revenue >> dest;
      items[cntItem].id = id;
      items[cntItem].en = true;
      items[cntItem].revenue = revenue;
      items[cntItem].dest = dest;
      cntItem++;
      pq.push(id);
    }
    else if (input == 300)
    {
      // 300 id
      // id 번째 여행 상품 제거
      // 있을때만 지움 최대 30,000번
      cin >> id;
      deleteItem(id);
      for (int i = 0; i < cntItem; i++)
      {
        if (items[i].en)
        {
          cout << "id: " << items[i].id << ", revenue: " << items[i].revenue
               << ", dest: " << items[i].dest << ", dis: " << items[i].dis << "\n";
        }
      }
    }
    else if (input == 400)
    {
      // 400
      // 최적 여행 상품 판매
      // 여행 상품중에서 최적의 상품을 판매
      // revenue - cost 를 최대로 해야하고 같으면 id 가 작은거 먼저
      // 도착하지 못하거나 음수인건 당연히 판매 가능 상품이 아니고
      // 판매 가능한게 하나도 없으면 -1 출력
      // 판매 가능한게 있으면 해당 id 출력하고 제거
      // 최대 30,000번 들어올 수 있다.
      cout << bestItem() << "\n";
      for (int i = 0; i < cntItem; i++)
      {
        if (items[i].en)
        {
          cout << "id: " << items[i].id << ", revenue: " << items[i].revenue
               << ", dest: " << items[i].dest << ", dis: " << items[i].dis << "\n";
        }
      }
    }
    else if (input == 500)
    {
      // 500 s
      // 상품의 출발지를 s 로 변경,            0 <= s <= n - 1
      // 처음은 0 이다.
      // 이를 통해서 당연히 cost 들이 변경됨을 유의하자
      // 최대 15 번 들어올 수 있다.
      cin >> start;
      calDis();
    }
  }

  return 0;
}

void deleteItem(int id)
{
  for (int i = 0; i < cntItem; i++)
  {
    if (items[i].id == id && items[i].en)
    {
      items[i].en = false;
      break;
    }
  }
}

int bestItem()
{
  // 최적 여행 상품 판매
  // 여행 상품중에서 최적의 상품을 판매
  // revenue - cost 를 최대로 해야하고 같으면 id 가 작은거 먼저
  // 도착하지 못하거나 음수인건 당연히 판매 가능 상품이 아니고
  // 판매 가능한게 하나도 없으면 -1 출력
  // 판매 가능한게 있으면 해당 id 출력하고 제거
  // 최대 30,000번 들어올 수 있다.
  
  // int bestId = 200000;
  // int idx;
  // int max = 0;
  // int benefit;
  // for (int i = 0; i < cntItem; i++)
  // {
  //   if (items[i].en && items[i].dis >= 0)
  //   {
  //     benefit = items[i].revenue - items[i].dis;
  //     if (benefit >= 0)
  //     {
  //       if (benefit > max)
  //       {
  //         bestId = items[i].id;
  //         idx = i;
  //         max = benefit;
  //       }
  //       else if (benefit == max)
  //       {
  //         if (bestId > items[i].id)
  //         {
  //           bestId = items[i].id;
  //           idx = i;
  //         }
  //       }
  //     }
  //   }
  // }
  // cout << "[bestItem] max: " << max << "\n";
  // if (bestId == 200000)
  //   return -1;
  // items[idx].en = false;
  // return bestId;
  int idx = pq.top();
}


void calDis()
{
  for (int i = 0; i < cntItem; i++)
  {
    if (items[i].en)
    {
      ::min = -1;
      bzero(visited, sizeof(visited));
      dfs(items[i].dest, start, 0, items[i].revenue);
      items[i].dis = ::min;
    }
  }
  for (int i = 0; i < cntItem; i++)
  {
    if (items[i].en)
    {
      cout << "id: " << items[i].id << ", revenue: " << items[i].revenue
          << ", dest: " << items[i].dest << ", dis: " << items[i].dis << "\n";
    }
  }
}

void dfs(int dest, int cityId, int dist, int revenue)
{
  // cout << "[dfs] cityId: " << cityId << ", dest: " << dest << "\n";
  if (dest == cityId || revenue - dist < 0)
  {
    if (::min == -1 || ::min > dist)
      ::min = dist;
    return ;
  }

  for (int i = 0; i < (int)city[cityId].enable.size(); i++)
  {
    if (visited[city[cityId].enable[i]]) continue;
    visited[city[cityId].enable[i]] = true;
    dfs(dest, city[cityId].enable[i], dist + dis[cityId][city[cityId].enable[i]], revenue);
    visited[city[cityId].enable[i]] = false;
  }
}

void setD()
{
  for (int i = 0; i < n; i++)
    D[i] = 2147483647;
  D[start] = 0;
  bool v[2000] = {};
  int cur;
  int distance;

  for (int i = 0; i < n; i++)
  {
    distance = 2147483647;
    for (int j = 0; j < n; j++)
    {
      if (!v[j] && D[j] < distance)
      {
        distance = D[j];
        cur = j;
      }
    }
    v[cur] = true;
    for (int j = 0; j < n ;j++)
    {
      if (!v[j] && D[j] != 2147483647 && dis[cur][j] != 2147483647 && D[j] > D[cur] + dis[cur][j])
        D[j] = D[cur] + dis[cur][j];
    }
  }
}