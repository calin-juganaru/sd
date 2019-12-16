// Copyright [2018] <Călin Jugănaru>

#include "./functions.h"

int main()
{
    // ---------- INITIALIZE -----------
    ifstream fin("robots.in");
    ofstream fout("robots.out");

    int N, LIN, COL;

    fin >> N >> LIN >> COL;

    auto cmd_queue = new deque_t[N];
    auto map = new int*[LIN];
    auto boxes = new int[N];

    memset(boxes, 0, N * sizeof(int));

    for (auto i = 0; i < LIN; ++i)
    {
        map[i] = new int[COL];
        for (auto j = 0; j < COL; ++j)
            fin >> map[i][j];
    }
    // --------------------------------

    solution(fin, fout, map, cmd_queue, boxes);

    // -------- CLEAN - UP ---------
    for (auto i = 0; i < LIN; ++i)
        delete[] map[i];
    delete[] map;
    delete[] cmd_queue;
    delete[] boxes;

    fin.close();
    fout.close();
    // -----------------------------
}
