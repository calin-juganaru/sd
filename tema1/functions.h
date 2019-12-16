// Copyright [2018] <Călin Jugănaru>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <string>
#include <fstream>
#include <algorithm>

#include "./Structs.h"

#define newl fout << '\n'
#define tab  fout << ' '
#define in :

using std::min;
using std::string;
using std::ifstream;
using std::ofstream;

//--------------------------------------------------------
void add_get_box(ifstream& fin, deque_t cmd_queue[],
                 stack_t& last_add_cmd, const int& time)
{
    int robot_id, x, y, nr_boxes;
    bool priority;

    fin >> robot_id >> x >> y >> nr_boxes >> priority;

    auto aux = queue_type(false, x, y, nr_boxes, time);

    //  comanda va fi adăugată în coada robotului
    //      la început sau sfârșit, după prioritate
    if (priority)
        cmd_queue[robot_id].push_back(aux);
    else
        cmd_queue[robot_id].push_front(aux);

    //  al doilea parametru, 0, semnifică
    //      tipul comenzii primite de robot,
    //      în cazul acesta, ADD_GET_BOX

    last_add_cmd.push(stack_type(robot_id, 0, x, y,
                      nr_boxes, time, 0, priority));

    //  penultimul argument, 0, reprezintă
    //      momentul în care a fost executată
    //      această comandă, dar, la inițializare,
    //      va fii, prin convenție, 0
}
//--------------------------------------------------------

//--------------------------------------------------------
void add_drop_box(ifstream& fin, deque_t cmd_queue[],
                  stack_t& last_add_cmd, const int& time)
{
    int robot_id, x, y, nr_boxes;
    bool priority;

    fin >> robot_id >> x >> y >> nr_boxes >> priority;

    auto aux = queue_type(true, x, y, nr_boxes, time);

    //  la fel ca la funcția add_get_box()
    if (priority)
        cmd_queue[robot_id].push_back(aux);
    else
        cmd_queue[robot_id].push_front(aux);

    //  aici 1, pentru DROP
    last_add_cmd.push(stack_type(robot_id, 1, x, y,
                      nr_boxes, time, 0, priority));
}
//--------------------------------------------------------

//---------------------------------------------------------------------
void execute(ifstream& fin, ofstream& fout, deque_t cmd_queue[],
    int* map[], stack_t& last_exec_cmd, int boxes[], const int& time)
{
    int robot_id, nr_boxes;

    fin >> robot_id;

    if (cmd_queue[robot_id].empty())
    {
        fout << "EXECUTE: No command to execute\n";
        return;
    }

    auto aux = cmd_queue[robot_id].front();
    cmd_queue[robot_id].pop_front();

    //  dacă această comandă este de tipul DROP
    if (aux.cmd_type)
    {
        //  numărul de cutii pe care îl poate lăsa robotul
        //      este minumul dintre numărul precizat
        //      în comandă și numărul de cutii
        //      care se (mai) află în 'stocul' robotului

        nr_boxes = min(boxes[robot_id], aux.nr_boxes);
        map[aux.x][aux.y] += nr_boxes;
        boxes[robot_id] -= nr_boxes;

        //  acest număr trebuie adăugat în celulă
        //      și scăzut din 'stocul' robotului
    }
    else    //  sau GET, unde
    {
        //  numărul de cutii pe care le poate lua robotul
        //      este minumul dintre numărul precizat
        //      în comandă și numărul de cutii
        //      care se (mai) află în acea celulă

        nr_boxes = min(map[aux.x][aux.y], aux.nr_boxes);
        map[aux.x][aux.y] -= nr_boxes;
        boxes[robot_id] += nr_boxes;

        //  iar acest număr trebuie adăugat
        //      în 'stocul' robotului
        //      și scăzut din celulă
    }

    last_exec_cmd.push(stack_type(robot_id, aux.cmd_type, aux.x,
                        aux.y, nr_boxes, aux.add_time, time));
}
//---------------------------------------------------------------------

//-----------------------------------------------------------------------
void print_commands(ifstream& fin, ofstream& fout, deque_t cmd_queue[])
{
    int robot_id;

    fin >> robot_id;
    fout << "PRINT_COMMANDS: ";

    if (cmd_queue[robot_id].empty())
    {
        fout << "No command found\n";
        return;
    }

    fout << robot_id << ": ";

    auto stop = cmd_queue[robot_id].size();

    //  cu ajutorul clasei de iteratori
    //      am putut aplica un 'range-based
    //      for loop' pe container-ul meu

    for (auto&& it in cmd_queue[robot_id])
    {
        if (it.cmd_type)
            fout << "DROP ";
        else
            fout << "GET ";

        fout << it.x, tab << it.y,
         tab << it.nr_boxes;

        if (--stop)
            fout << "; ";
    }
    newl;
}
//-----------------------------------------------------------------------

//------------------------------------------------------------------
void last_executed_command(ofstream& fout, stack_t& last_exec_cmd)
{
    fout << "LAST_EXECUTED_COMMAND: ";

    fout << last_exec_cmd.top().robot_id << ": ";

    if (last_exec_cmd.top().cmd_type)
        fout << "DROP ";
    else
        fout << "GET ";

    fout << last_exec_cmd.top().x, tab
         << last_exec_cmd.top().y, tab
         << last_exec_cmd.top().nr_boxes, newl;
}
//------------------------------------------------------------------

//------------------------------------------------------------------
void undo(deque_t cmd_queue[], int* map[], stack_t& last_add_cmd,
            stack_t& last_exec_cmd, int boxes[], const int& time)
{
    auto last_exec = last_exec_cmd.top();
    auto last_add = last_add_cmd.top();

    //  pentru a aplica operația de UNDO pe ultima comandă
    //      trebuie să știu daca aceasta a fost una de tip
    //      ADD sau EXECUTE și să tratez cazurile separat

    if (last_exec_cmd.empty()  ||
        last_add.add_time > last_exec.exec_time)
    {
        //  verific dacă ultima comandă de tip ADD
        //      a fost adăugată la finalul cozii
        if (last_add.priority)
            cmd_queue[last_add.robot_id].pop_back();
        else    //  sau la început
            cmd_queue[last_add.robot_id].pop_front();

        last_add_cmd.pop();
    }

    else    //  EXECUTE
    {
        auto aux = queue_type(last_exec.cmd_type,
                        last_exec.x, last_exec.y,
                        last_exec.nr_boxes, time);

        //  aici trebuie să 'refac' acea comandă,
        //      să o pun la loc în coada robotului,
        //      să mut cutiile înapoi unde erau
        //      și să o scot din stiva celor executate

        cmd_queue[last_exec.robot_id].push_front(aux);

        //  dacă ultima comandă executată a fost de tipul DROP
        if (last_exec.cmd_type)
        {
            boxes[last_exec.robot_id] += last_exec.nr_boxes;
            map[last_exec.x][last_exec.y] -= last_exec.nr_boxes;
        }
        else    //  sau GET
        {
            boxes[last_exec.robot_id] -= last_exec.nr_boxes;
            map[last_exec.x][last_exec.y] += last_exec.nr_boxes;
        }

        last_exec_cmd.pop();
    }
}
//------------------------------------------------------------------

//-------------------------------------------------------------
void solution(ifstream& fin, ofstream& fout, int* map[],
                deque_t cmd_queue[], int boxes[])
{
    string cmd;
    auto TIME = 0;

    //  folosesc două stive, pentru
    //      ultima comandă adaugată
    //      și ultima comandă executată,
    //      astfel încăt să pot face UNDO

    stack_t last_add_cmd;
    stack_t last_exec_cmd;

    while (fin >> cmd)
    {
        ++TIME;
        //  TIME este un timp abslout: la fiecare comandă
        //      citită va fi incrementat, și îl voi folosi
        //      pentru determinarea timpului de adăgare
        //      și de execuție al unei comenzi

        if (cmd == "ADD_GET_BOX")
        {
            add_get_box(fin, cmd_queue, last_add_cmd, TIME);
        }

        else if (cmd == "ADD_DROP_BOX")
        {
            add_drop_box(fin, cmd_queue, last_add_cmd, TIME);
        }

        else if (cmd == "EXECUTE")
        {
            execute(fin, fout, cmd_queue, map,
                    last_exec_cmd, boxes, TIME);
        }

        else if (cmd == "PRINT_COMMANDS")
            print_commands(fin, fout, cmd_queue);

        else if (cmd == "LAST_EXECUTED_COMMAND")
        {
            last_executed_command(fout, last_exec_cmd);
        }

        else if (cmd == "UNDO")
        {
            if (last_exec_cmd.empty() && last_add_cmd.empty())
                fout << "UNDO: No History\n";
            else
                undo(cmd_queue, map, last_add_cmd,
                     last_exec_cmd, boxes, TIME);
        }

        else if (cmd == "HOW_MUCH_TIME")
        {
            fout << "HOW_MUCH_TIME: ";

            if (last_exec_cmd.empty())
                fout << "No command was executed\n";
            else
                fout << last_exec_cmd.top().exec_time  -
                        last_exec_cmd.top().add_time, newl;

            //  timpul cât a stat o comandă în coadă până să
            //      fie executată, reprezintă diferența dintre
            //      momentul în care a fost executată și
            //      momentul în care a fost adaugată în coadă
        }

        else if (cmd == "HOW_MANY_BOXES")
        {
            int robot_id;
            fin >> robot_id;
            fout << "HOW_MANY_BOXES: "
                 << boxes[robot_id], newl;
        }
    }
}
//-------------------------------------------------------------

#endif  // FUNCTIONS_H_
