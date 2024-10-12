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
#include <queue>

using namespace std;

int n, m;
int dis[2000][2000] = {};
int D[2000] = {};
int start = 0;

class item
{
  public:
    int id;
    int revenue;
    int dest;
};

bool isEnable[30001] = {};
int cntItem = 0;

class comp
{
  public:
    bool operator() (item a, item b)
    {
      if (a.revenue - D[a.dest] < b.revenue - D[b.dest])
        return true;
      if (a.revenue - D[a.dest] == b.revenue - D[b.dest])
      {
        if (a.id > b.id)
          return true;
      }
      return false;
    }
};

priority_queue<item, vector<item>, comp> pq;

void setD();
void resetItems();

int main()
{
  ios::sync_with_stdio(0);
  cin.tie(0);

  int q;
  cin >> q;

  int v, u, w, id, revenue, dest;
  int input;
  while (q--)
  {
    cin >> input;
    if (input == 100)
    {
      cin >> n >> m;
      for (int i = 0; i < n; i++)
      {
        for (int j = 0; j < n; j++)
        {
          if (i == j)
            dis[i][j] = 0;
          else
            dis[i][j] = 2147483647;
        }
      }
      for (int i = 0; i < m; i++)
      {
        cin >> v >> u >> w;
        dis[v][u] = w < dis[v][u] ? w : dis[v][u];
        dis[u][v] = w < dis[u][v] ? w : dis[u][v];
      }
      setD();

    }
    else if (input == 200)
    {
      cin >> id >> revenue >> dest;
      item newItem;
      newItem.id = id;
      newItem.revenue = revenue;
      newItem.dest = dest;
      pq.push(newItem);
      isEnable[id] = true;
      cout << "[item] top id: " << pq.top().id << ", revenue: " << pq.top().revenue << ", dest: " << pq.top().dest << "\n";
    }
    else if (input == 300)
    {
      cin >> id;
      isEnable[id] = false;
    }
    else if (input == 400)
    {

      for (int i = 0; i < n; i++)
      {
        cout << "D[" << i << "] : " << D[i] << "\n";
      }
      while (!pq.empty() && !isEnable[pq.top().id])
        pq.pop();
      if (pq.empty() || pq.top().revenue < D[pq.top().dest])
        cout << -1 << "\n";
      else
      {
        cout << pq.top().id << "\n";
        pq.pop();
      }
    }
    else if (input == 500)
    {
      cin >> start;
      setD();
      resetItems();
    }
  }
  return 0;
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
    for (int j = 0; j < n; j++)
    {
      if (!v[j] && D[cur] != 2147483647 && dis[cur][j] != 2147483647 && D[j] > distance + dis[cur][j])
        D[j] = distance + dis[cur][j];
    }
  }      
  for (int i = 0; i < n; i++)
  {
    cout << "D[" << i << "] : " << D[i] << "\n";
  }
}

void resetItems()
{
  priority_queue<item, vector<item>, comp> npq;

  while (!pq.empty())
  {
    if (isEnable[pq.top().id])
      npq.push(pq.top());
    pq.pop();
  }
  pq = npq;
}