// Copyright [2018] <Călin Jugănaru>

#ifndef TASKS_H_
#define TASKS_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <string>
#include <utility>

#include "./graf.h"
#include "./hash.h"

#define newl fout << '\n'
#define tab  fout << ' '

using std::ofstream;
using std::cin;

// -----------------------------------------------

void task_1(graf<size_t>& harta,
            hash_table<size_t, string>& oras)
{
    ofstream fout("task1.out");

    auto grad_max = 0u,
            index = 0u,
        index_max = 0u;

    //  aflu nodul cu grad interior maxim
    for (auto& nod in harta.vecini)
    {
        auto grad = nod.size();

        if (grad > grad_max)
        {
            grad_max = grad;
            index_max = index;
        }

        ++index;
    }

    fout << oras[index_max], tab
         << grad_max, newl;

    fout.close();
}

// ---------------------------------------------------------

vector<deque<size_t>> task_2(size_t& N, graf<size_t>& harta,
                             graf<size_t>& transpusa)
{
    ofstream fout("task2.out");

    auto vizitat = vector<bool>(N);
    auto topo = deque<size_t>();
    auto comp_tare_conexe = vector<deque<size_t>>();

    auto nr_comp_conexe = 0;

    for (auto nod = 0u; nod < N; ++nod)
    {
        if (!vizitat[nod])
        {
            dfs(harta, nod, vizitat, topo);
            ++nr_comp_conexe;
        }
    }

    vizitat.assign(N, false);
    for (auto& nod in topo)
        if (!vizitat[nod])
        {
            comp_tare_conexe.push_back(deque<size_t>());
            dfs(transpusa, nod, vizitat,
                comp_tare_conexe.back());
        }

    if (nr_comp_conexe > 1 || comp_tare_conexe.size() > 1)
        fout << "HARTA INVALIDA\n";
    else
        fout << "HARTA VALIDA\n";

    fout.close();

    return comp_tare_conexe;
}

// ---------------------------------------------------------

void task_3(size_t& N, vector<deque<size_t>>& ctc,
            hash_table<string, size_t>& index,
            hash_table<size_t, string>& oras)
{
    ofstream fout("task3.out");

    size_t K; cin >> K;
    string nume;

    auto lista = vector<size_t>();
    lista.reserve(K);

    while (K--)
    {
        cin >> nume;
        lista.push_back(index[nume]);
    }

    auto nr_ctc = ctc.size();
    //  componentele tare conexe
    //      de la task-ul 2

    if (nr_ctc == 1)
    {
        fout << "Exista drum intre oricare doua orase\n";
        return;
    }

    auto comp = vector<size_t>(N);

    for (auto i = 0u; i < nr_ctc; ++i)
        for (auto& j in ctc[i])
            comp[j] = i;

    for (auto& i in lista)
    {
        fout << "Orase inaccesibile pentru "
             << oras[i] << ":";

        auto fara_drum = vector<bool>(N);

        for (auto j = 0u; j < comp[i]; ++j)
            for (auto& k in ctc[j])
                fara_drum[k] = true;

        for (auto j = comp[i] + 1; j < nr_ctc; ++j)
            for (auto& k in ctc[j])
                fara_drum[k] = true;

        for (auto j = 0u; j < N; ++j)
            if (fara_drum[j])
                tab << oras[j];

        newl;
    }

    fout.close();
}

// -------------------------------------------------------

void task_4(size_t& N, graf<size_t>& harta,
            hash_table<string, size_t>& index,
            hash_table<size_t, string>& oras,
            vector<bool>& benzinarie)
{
    ofstream fout("task4.out");

    size_t L, X, C, i, minim;
    string nume; bool da;
    cin >> L;

    while (L--)
    {
        cin >> nume >> X >> C;
        i = index[nume];
        da = false;

        minim = 0x3f3f3f3f;

        auto dist = vector<size_t>(N, 0);

        bfs(harta, i, dist);

        //  aflu care este cel mai apropiat
        //      oraș cu benzinărie
        for (auto j = 0u; j < N; ++j)
            if (dist[j] && dist[j] < minim &&
                dist[j]*C <= X && benzinarie[j])
            {
                da = true;
                minim = dist[j];
            }

        if (da)
            fout << "DA " <<  X - minim * C, newl;
        else
            fout << "NU\n";
    }

    fout.close();
}

// ----------------------------------------------------

void afisare_traseu(ofstream& fout, size_t& x,
               size_t& y, vector<size_t>& tata,
               hash_table<size_t, string>& oras)
{
    if (x != y)
        afisare_traseu(fout, x, tata[y], tata, oras);
    fout << oras[y], tab;
}

// ----------------------------------------------------

void task_5(size_t& N, graf<size_t>& harta,
            hash_table<string, size_t>& index,
            hash_table<size_t, string>& oras)
{
    ofstream fout("task5.out");

    size_t P, calatori, maxim = 0;
    string sursa, dest, ruta_max;

    cin >> P;

    auto traseu = hash_table<string, size_t>(P);

    while (P--)
    {
        cin >> sursa >> dest >> calatori;

        auto x = to_string(index[sursa]);
        auto y = to_string(index[dest]);

        auto z = x + " " + y;

        //  caut ruta cea mai aglomerată
        //      și cea mai mică lexicografic,
        //      adăugând noi călători, dacă
        //      aceasta are mai multe autobuze

        if (!traseu.find(z))
        {
            traseu.insert({z, calatori});
            if (calatori > maxim)
            {
                maxim = calatori;
                ruta_max = move(z);
            }
            else if (calatori == maxim)
            {
                auto poz = ruta_max.find(" ");
                auto i = ruta_max.substr(0, poz + 1);
                auto j = ruta_max.substr(poz + 1);

                //  compar orașele sursă și destinație
                //      ale rutei maximale obținute
                //      pană acum și ale celei curente,
                //      extrăgând-ule din string-uri

                if (x > i || (x == i && y > j))
                    ruta_max = move(z);

                //  move() este pentru a muta string-urile,
                //      în loc să fac multe copii, alocări
                //      și dealocări inutile
            }
        }
        else
        {
            traseu[z] += calatori;
            auto aux = traseu[z];
            if (aux > maxim)
            {
                maxim = aux;
                ruta_max = move(z);
            }
        }
    }

    auto poz = ruta_max.find(" ");
    size_t x = stoul(ruta_max.substr(0, poz + 1));
    size_t y = stoul(ruta_max.substr(poz + 1));

    auto tata = vector<size_t>(N, N);

    //  aflu traseul cel mai scurt și mai
    //      mic din punct de vedere lexicografic,
    //      printr-o singură parcurgere BFS
    min_dist(harta, x, tata);

    fout << maxim << ' ';
    afisare_traseu(fout, x, y, tata, oras);

    fout.close();
}

// ------------------------------------------------

#endif  // TASKS_H_
