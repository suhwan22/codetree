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