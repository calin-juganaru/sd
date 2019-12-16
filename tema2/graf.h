// Copyright [2018] <Călin Jugănaru>

#ifndef GRAF_H_
#define GRAF_H_

#include <vector>
#include <deque>
#include <queue>
#include <algorithm>

#define in :

using std::move;
using std::sort;
using std::queue;
using std::deque;
using std::vector;

template <typename T>
class graf
{
    size_t _nr_noduri;

 public:
    vector<vector<T>> vecini;

    explicit graf(size_t n):
        _nr_noduri(n)
        {
            vecini.resize(n);
        }

    void insert(size_t sursa, T dest)
    {
        vecini[sursa].push_back(dest);
    }

    size_t nr_noduri()
    {
        return _nr_noduri;
    }
};

// --------------------------------------------------

//  această funcție poate fi folosită pentru ambele
//      aplicări ale DFS-ului de la task-ul 2

template <typename T>
void dfs(graf<T>& g, size_t nod,
         vector<bool>& vizitat, deque<T>& lista)
{
    vizitat[nod] = true;

    for (auto& vecin in g.vecini[nod])
        if (!vizitat[vecin])
            dfs(g, vecin, vizitat, lista);

    lista.push_front(nod);
}

// -----------------------------------------------------

template <typename T>
void bfs(graf<T>& g, size_t nod, vector<size_t>& dist)
{
    auto N = g.nr_noduri();
    queue<size_t> q;

    auto viz = vector<bool>(N);

    q.push(nod);

    while (!q.empty())
    {
        nod = q.front();
        q.pop();

        if (!viz[nod])
        {
            viz[nod] = 1;
            for (auto& vecin in g.vecini[nod])
                if (!viz[vecin])
                {
                    q.push(vecin);
                    if (!dist[vecin])
                        dist[vecin] = dist[nod] + 1;
                }
        }
    }
}

// ---------------------------------------------------

template <typename T>
void min_dist(graf<T>& g, size_t start,
              vector<size_t>& tata)
{
    auto N = g.nr_noduri();
    queue<size_t> q;

    auto viz = vector<bool>(N);

    q.push(start);

    while (!q.empty())
    {
        auto nod = q.front(); q.pop();

        //  sortez lista de vecini
        //      pentru a obține traseul
        //      cel mai mic lexicografic

        sort(g.vecini[nod].begin(),
             g.vecini[nod].end());

        if (!viz[nod])
        {
            viz[nod] = 1;
            for (auto& vecin in g.vecini[nod])
                if (!viz[vecin])
                {
                    q.push(vecin);
                    if (tata[vecin] == N)
                        tata[vecin] = nod;

                    //  dacă am aflat deja drumul
                    //      minim până la destinație,
                    //      ne putem opri din parcurs

                    if (vecin == start)
                        return;
                }
        }
    }
}

#endif  // GRAF_H_
