// Copyright [2018] <Călin Jugănaru>

#include <vector>
#include <string>

#include "./tasks.h"

int main()
{
    size_t N, M, B, x, y;
    string nume, sursa, dest;

    cin >> N >> M;

    //  tabelele de dispersie pentru maparea
    //      nume oraș - indice și invers
    auto index = hash_table<string, size_t>(N);
    auto oras = hash_table<size_t, string>(N);

    for (auto i = 0u; i < N; ++i)
    {
        cin >> nume;
        oras.insert({i, nume});
        index.insert({nume, i});
    }

    // --------------------------------

    auto harta = graf<size_t>(N);
    auto transpusa = harta;

    for (auto i = 0u; i < M; ++i)
    {
        cin >> sursa >> dest;
        x = index[sursa];
        y = index[dest];
        harta.insert(x, y);
        transpusa.insert(y, x);
    }

    task_1(transpusa, oras);
    auto ctc = task_2(N, harta, transpusa);
    //  componentele tare conexe obținute
    //      la task-ul 2 sunt necesare la 3

    // ------------------------------------

    cin >> B;
    auto benzinarie = vector<bool>(N);
    for (auto i = 0u; i < B; ++i)
    {
        cin >> nume;
        benzinarie[index[nume]] = true;
    }

    task_3(N, ctc, index, oras);

    // -----------------------------------------

    task_4(N, harta, index, oras, benzinarie);

    // -----------------------------------------

    task_5(N, harta, index, oras);
}
