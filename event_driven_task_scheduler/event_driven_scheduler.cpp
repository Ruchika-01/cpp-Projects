#include <iostream>
#include <vector>
#include <queue>

using namespace std;

enum State { READY, RUNNING, DONE };

struct Task {
    int id;
    int duration;
    State state;
    vector<int> dependents;
};

struct Event {
    int time;
    int taskId;

    bool operator>(const Event& other) const {
        return time > other.time;
    }
};

int main() {
    vector<Task> tasks(3);

    tasks[0] = {0, 3, READY, {1, 2}};
    tasks[1] = {1, 2, READY, {}};
    tasks[2] = {2, 4, READY, {}};

    priority_queue<Event, vector<Event>, greater<Event>> eventQueue;

    cout << "OS-like Event Driven Scheduler\n";
    cout << "-------------------------------\n";

    int currentTime = 0;

    // Start first task
    tasks[0].state = RUNNING;
    cout << "Time " << currentTime << " : Task 0 RUNNING\n";
    eventQueue.push({currentTime + tasks[0].duration, 0});

    while (!eventQueue.empty()) {
        Event e = eventQueue.top();
        eventQueue.pop();
        currentTime = e.time;

        Task &t = tasks[e.taskId];
        t.state = DONE;
        cout << "Time " << currentTime << " : Task " << t.id << " DONE\n";

        for (int dep : t.dependents) {
            if (tasks[dep].state == READY) {
                tasks[dep].state = RUNNING;
                cout << "Time " << currentTime << " : Task " << dep << " RUNNING\n";
                eventQueue.push({currentTime + tasks[dep].duration, dep});
            }
        }
    }

    cout << "-------------------------------\n";
    cout << "Simulation Complete\n";
    return 0;
}


  