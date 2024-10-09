#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

typedef struct st
{
    int m_id;
    int p_id;
    int color;
    int max;
    int depth;
    vector<int> children;
} node;

void changeColor(int m_id, int color);
node arr[100001];
vector<int> roots;

void newNode(int m_id, int p_id, int color, int max, int depth);
bool isValidDepth(int p_id);
long long calValue();
long long dfs(int m_id, char& v);
int calOct(char c);
void printNode();
void printChildren(int m);

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;
    int input;
    int m_id, p_id, color, max;
    while (t--)
    {
        cin >> input;
        if (input == 100)
        {
            cin >> m_id >> p_id >> color >> max;
            if (p_id == -1)
            {
                newNode(m_id, p_id, color, max, 1);
                roots.push_back(m_id);
            }
            else
            {
                if (isValidDepth(p_id))
                {
                    newNode(m_id, p_id, color, max, arr[m_id].depth + 1);
                    arr[p_id].children.push_back(m_id);
                }
            }
            //printNode();
        }
        else if (input == 200)
        {
            cin >> m_id >> color;
            changeColor(m_id, color);
        }
        else if (input == 300)
        {
            cin >> m_id;
            cout << arr[m_id].color << "\n";
        }
        else if (input == 400)
            cout << calValue() << "\n";
    }

    return 0;
}

void newNode(int m_id, int p_id, int color, int max, int depth)
{
    arr[m_id].m_id = m_id;
    arr[m_id].p_id = p_id;
    arr[m_id].color = color;
    arr[m_id].max = max;
    arr[m_id].depth = depth;
}

bool isValidDepth(int p_id)
{
    int d = 2;
    while (p_id != -1)
    {
        if (arr[p_id].max < d++)
            return false;
        p_id = arr[p_id].p_id;
    }
    return true;
}

void changeColor(int m_id, int color)
{
    arr[m_id].color = color;
    for (int i = 0; i < arr[m_id].children.size(); i++)
        changeColor(arr[m_id].children[i], color);
}

long long calValue()
{
    long long ret = 0;
    for (int i = 0; i < roots.size(); i++)
    {
        char v = 0;
        ret += dfs(roots[i], v);
    }
    return ret;
}

long long dfs(int m_id, char& v)
{
    if (arr[m_id].children.size() == 0)
    {
        v |= (char)pow(2, arr[m_id].color - 1);
        //cout << "[value] m_id: " << m_id << " ret: 1\n";
        return 1;
    }
    long long ret = 0;
    char value = 0;
    for (int i = 0; i < arr[m_id].children.size(); i++)
    {
        long long temp = dfs(arr[m_id].children[i], value);
        ret += temp;
    }
    value |= (char)pow(2, arr[m_id].color - 1);
    //cout << "v: " << (int)value << "\n";
    ret += pow(calOct(value), 2);
    v |= value;
    //cout << "[value] m_id: " << m_id << " ret: " << ret << "\n";
    return ret;
}

int calOct(char c)
{
    int a = 0;
    while (1)
    {
        if (c % 2 == 1)
            a++;
        if (c / 2 == 0)
            break;
        c /= 2;
    }
    //cout << "[calOct]: " << a << "\n";
    return a;
}

void printNode()
{
    for (int i = 0; i < roots.size(); i++)
    {
        printChildren(roots[i]);
    }
}

void printChildren(int m)
{
    if (arr[m].children.empty())
    {
        cout << "m_id: " << arr[m].m_id << ", p_id: " << arr[m].p_id << ", color: " << arr[m].color
        << ", max: " << arr[m].max << "\n";
        return ;
    }
    for (int i = 0; i < arr[m].children.size(); i++)
    {
        printChildren(arr[m].children[i]);
    }
    cout << "m_id: " << arr[m].m_id << ", p_id: " << arr[m].p_id << ", color: " << arr[m].color
        << ", max: " << arr[m].max << "\n";
}
