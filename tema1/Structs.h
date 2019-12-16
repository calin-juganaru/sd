// Copyright [2018] <Călin Jugănaru>

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "./Deque.h"
#include "./Stack.h"


//-------------------------------------------------------------
struct queue_type
{
    bool cmd_type;
    int x, y, nr_boxes, add_time;

    queue_type() = default;
    ~queue_type() = default;

    queue_type(bool cmd_type, int x, int y,
               int nr_boxes, int add_time):
        cmd_type(cmd_type),
        add_time(add_time),
        nr_boxes(nr_boxes),
        x(x), y(y) {}

    queue_type& operator=(const queue_type& other) = default;
};
//-------------------------------------------------------------

//-------------------------------------------------------------
struct stack_type
{
    int robot_id, nr_boxes, x, y;
    bool cmd_type, priority;
    int add_time, exec_time;

    stack_type() = default;
    ~stack_type() = default;

    stack_type(int robot_id, bool cmd_type, int x,
               int y, int nr_boxes, int add_time,
               int exec_time = 0, bool priority = 0):
        robot_id(robot_id),
        cmd_type(cmd_type),
        add_time(add_time),
        exec_time(exec_time),
        nr_boxes(nr_boxes),
        priority(priority),
        x(x), y(y) {}

    stack_type& operator=(const stack_type& other) = default;
};
//-------------------------------------------------------------

using deque_t = Deque<queue_type>;
using stack_t = Stack<stack_type>;

//  primul tip definit este pentru coada de comenzi,
//      iar al doilea, pentru last_add/exec_cmd

#endif  // STRUCTS_H_
