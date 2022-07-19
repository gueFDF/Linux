#include "threadpool.hpp"
void task1(void *num)
{
    int i=10;
    while (i--)
    {
        cout << *(int *)num<<endl;
    }
}
int task2(int num)
{
    while (1)
    {
        cout << num;
    }
}
int main()
{
    Task task;
    task.function = task1;
    int num = 1;
    task.arg = &num;
    ThreadPool p(6);
    p.addTask(task);
    sleep(1);
}