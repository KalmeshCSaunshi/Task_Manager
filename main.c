#include<stdio.h>
#include <string.h>
enum Status{
    TODO,
    IN_PROGRESS,
    DONE
};

struct Task
            {
                int id;
                char title[200];
                char description[1000];
                enum Status status;
                int assignedTo;
            };

 struct member
 {
    int id;
    char name[100];
 };
 

 void saveTasks(struct Task tasks[], int count) {
    FILE *fp = fopen("tasks.txt", "w");

    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    for(int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%d|%d\n",
            tasks[i].id,
            tasks[i].title,
            tasks[i].description,
            tasks[i].status,
            tasks[i].assignedTo
        );
    }

    fclose(fp);
}


void loadTasks(struct Task tasks[], int *count) {
    FILE *fp = fopen("tasks.txt", "r");

    if(fp == NULL) {
        return; // file may not exist first time
    }

    *count = 0;

    while(fscanf(fp, "%d|%[^|]|%[^|]|%d|%d\n",
        &tasks[*count].id,
        tasks[*count].title,
        tasks[*count].description,
        (int*)&tasks[*count].status,
        &tasks[*count].assignedTo
    ) == 5) {
        (*count)++;
    }

    fclose(fp);
}


void saveTeam(struct member team[], int membercount) {
    FILE *fp = fopen("team.txt", "w");

    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    for(int i = 0; i < membercount; i++) {
        fprintf(fp, "%d|%s\n",
            team[i].id,
            team[i].name
        );
    }

    fclose(fp);
}


void loadTeam(struct member team[], int *membercount) {
    FILE *fp = fopen("team.txt", "r");

    if(fp == NULL) {
        return;
    }

    *membercount = 0;

    while(fscanf(fp, "%d|%[^\n]\n",
        &team[*membercount].id,
        team[*membercount].name
    ) == 2) {
        (*membercount)++;
    }

    fclose(fp);
}

int main(){
    struct Task tasks[100];
    int count=0;
    struct member team[50];
    int membercount=0;
    loadTasks(tasks, &count);
    loadTeam(team, &membercount);
    while (1)
    {
        printf("====TASK MANAGER====\n");
        printf("1.CREATE TASK\n");
        printf("2.VIEW TASK\n");
        printf("3.UPDATE TASK\n");
        printf("4.ASSIGN TASK\n");
        printf("5.ADD TEAM MEMBER\n");
        printf("6.VIEW TEAM\n");
        printf("7.EXIT\n");

        int choice;
        printf("ENTER YOUR CHOICE: ");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:   //CREATE TASK

            printf("CREATE TASK SELECTED\n");
            getchar();
            printf("ENTER TITLE:\n");
            fgets(tasks[count].title, 200, stdin);
            tasks[count].title[strcspn(tasks[count].title, "\n")] = '\0';

            printf("ENTER DESCRIPTION:\n");
            fgets(tasks[count].description, 1000, stdin);
            tasks[count].description[strcspn(tasks[count].description, "\n")] = '\0';

            tasks[count].id = count + 1;
            tasks[count].status = TODO;
            tasks[count].assignedTo = -1;

            count++;
            printf("TASK CREATED SUCCESSFULLY\n");
            saveTasks(tasks, count);
            
            break;


        case 2:     //VIEW TASAK
            
            printf("VIEW TASK SELECTED\n");

            if(count == 0) {
                printf("NO TASK AVAILABLE\n");
                break;
            }
            printf("\n===== ALL TASKS =====\n");
            for (int i = 0; i < count; i++) {

                printf("-------------------------\n");

                printf("TASK ID: %d\n", tasks[i].id);
                printf("TASK TITLE: %s\n", tasks[i].title);
                printf("TASK DESCRIPTION: %s\n", tasks[i].description);

                if(tasks[i].status == TODO)
                       printf("STATUS: TODO\n");
                else if(tasks[i].status == IN_PROGRESS)
                    printf("STATUS: IN_PROGRESS\n");
                else
                    printf("STATUS: DONE\n");
                int assigned=0;
                for(int j=0; j<membercount; j++) {
                    if(team[j].id == tasks[i].assignedTo){
                        printf("TASK ASSIGNED TO: %s\n", team[j].name);
                        assigned=1;
                        break;
                    
                        
                    }
                }  
                if (assigned==0)
                    printf("TASK ASSINGED TO:NONE\n");                     
                    
                printf("\n");
            }
            printf("Total Tasks: %d\n\n", count);

        break;




        case 3:    //UPDATE TASK
            printf("UPDATE TASK STATUS  SELECTED\n");
            int updateid;
            int found=0;
            
            printf("ENTER TASK ID:");
            scanf("%d",&updateid);
            for(int i=0;i<count;i++){
                if(tasks[i].id==updateid){
                    found=1;
                    printf("SELECT OPTIONS BELOW\n");
                    printf("1.TODO\n2.IN_PROGRESS\n3.DONE");
                    int newStatus;
                    scanf("%d",&newStatus);
                    if(newStatus == 1) tasks[i].status = TODO;
                    else if(newStatus == 2) tasks[i].status = IN_PROGRESS;
                    else if(newStatus == 3) tasks[i].status = DONE;
                    printf("TASK STATUS UPDATED SUCCESSFULLY\n");
                    break;
                }
            }
            if(found==0)
                printf("TASK NOT FOUND\n");
            saveTasks(tasks, count);
            saveTeam(team, membercount);
            break;


        
        case 4:    //ASSIGN TASK   
            printf("ASSIGN TASK SELECTED\n");
            

            int assignid;
            int taskfound=0;
            int memberfound=0;
            printf("ENTER TASK ID");
            scanf("%d",&assignid);
            getchar();
            for(int i=0;i<count;i++){
                if(tasks[i].id == assignid){
                    taskfound=1;
                    printf("ENTER MEMBER ID:\n");
                    int member_id;
                    scanf("%d",&member_id);
                    getchar();
                    for (int j=0;j<membercount;j++){
                        if (member_id==team[j].id){
                            tasks[i].assignedTo=member_id;
                            memberfound=1;
                            printf("TASK ASSIGNED SUCCESSFULLY\n");
                            break;
                        }
                    } 
                    break;
                }
            }
            if (taskfound==0)
                printf("TASK ID NOT FOUND\n");
            if(taskfound==1 && memberfound==0)
                printf("MEMBER NOT FOUND\n");
                
            saveTasks(tasks, count);
            saveTeam(team, membercount);
            break;


        case 5:  // ADD TEAM MEMBER
            printf("ADD TEAM MEMBER SELECTED\n");
            if(membercount >= 50) {
                printf("TEAM LIMIT REACHED\n");
                break;
            }
            printf("ENTER TEAM MEMBER NAME");
            getchar();
            fgets(team[membercount].name, 100, stdin);
            team[membercount].name[strcspn(team[membercount].name, "\n")] = '\0';


            team[membercount].id = membercount + 1;
            membercount++;
            printf("TEAM MEMBER ADDED SUCCESSFULLY\n");
            saveTasks(tasks, count);
            saveTeam(team, membercount);
            break;



        case 6:  //VIEW TASK MEMBER
            printf("VIEW TEAM MEMBER SELECTED\n");
            if(membercount == 0) {
                printf("NO TEAM MEMBERS AVAILABLE\n");
                break;
            }
            for (int i=0;i<membercount;i++){
                printf("MEMBER ID:%d\n",team[i].id);
                printf("MEMBER NAME:%s\n",team[i].name);
            }
            break;



        case 7:  //EXIT PROGRAM
            
            printf("EXIT PROGRAM\n");
            return 0;
            
            
        
        default:printf("INVALID CHOICE\n");
            break;
        }

    }
    
    
    
    
    
}
