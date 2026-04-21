#ifndef BACKEND_H
#define BACKEND_H

// ================= ENUM =================
enum Status {
    TODO,
    IN_PROGRESS,
    DONE
};

// ================= STRUCT =================
struct Task {
    int id;
    char title[200];
    char description[1000];
    enum Status status;
    int assignedTo;
};

// ================= GLOBAL DATA =================
extern struct Task tasks[100];
extern int count;

// ================= FUNCTIONS =================
void createTask(char *title, char *desc);
int assignTask(int taskId, int memberId);
int updateTaskStatus(int taskId, int status);
int deleteTask(int taskId);
#endif