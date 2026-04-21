#include <stdio.h>
#include <string.h>
#include "backend.h"

// ================= GLOBAL STORAGE =================
struct Task tasks[100];
int count = 0;

// ================= CREATE TASK =================
void createTask(char *title, char *desc) {

    if(count >= 100) return;   // safety

    tasks[count].id = count + 1;

    strcpy(tasks[count].title, title);
    strcpy(tasks[count].description, desc);

    tasks[count].status = TODO;
    tasks[count].assignedTo = -1;

    count++;
}

// ================= ASSIGN TASK =================
int assignTask(int taskId, int memberId) {

    for(int i = 0; i < count; i++) {
        if(tasks[i].id == taskId) {
            tasks[i].assignedTo = memberId;
            return 1;
        }
    }
    return 0;
}

// ================= UPDATE TASK STATUS =================
int updateTaskStatus(int taskId, int status) {

    for(int i = 0; i < count; i++) {
        if(tasks[i].id == taskId) {
            tasks[i].status = status;
            return 1;
        }
    }
    return 0;
}

// ================= DELETE TASK =================
int deleteTask(int taskId) {

    for(int i = 0; i < count; i++) {
        if(tasks[i].id == taskId) {

            // shift left
            for(int j = i; j < count - 1; j++) {
                tasks[j] = tasks[j + 1];
            }

            count--;

            // reassign IDs (important)
            for(int k = 0; k < count; k++) {
                tasks[k].id = k + 1;
            }

            return 1;
        }
    }

    return 0;
}

