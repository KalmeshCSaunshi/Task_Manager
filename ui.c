#include <gtk/gtk.h>
#include "backend.h"

// GLOBAL UI
GtkWidget *entry_title;
GtkWidget *entry_desc;
GtkWidget *task_box;
GtkWidget *stack;
GtkWidget *entry_task_id;
GtkWidget *entry_member_id;
GtkWidget *entry_update_id;
GtkWidget *combo_status;
GtkWidget *entry_delete_id;

// ================= REFRESH =================
void refresh_tasks() {

    // clear old widgets
    GList *children = gtk_container_get_children(GTK_CONTAINER(task_box));
    for (GList *iter = children; iter != NULL; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // HEADER ROW
    GtkWidget *h1 = gtk_label_new("ID");
    GtkWidget *h2 = gtk_label_new("Title");
    GtkWidget *h3 = gtk_label_new("Assigned");
    GtkWidget *h4 = gtk_label_new("Status");


    gtk_grid_attach(GTK_GRID(task_box), h1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(task_box), h2, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(task_box), h3, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(task_box), h4, 3, 0, 1, 1);

    // DATA ROWS
    for (int i = 0; i < count; i++) {

        char id[10];
        sprintf(id, "%d", tasks[i].id);

        char assigned[20];
        if(tasks[i].assignedTo == -1)
            sprintf(assigned, "None");
        else
            sprintf(assigned, "%d", tasks[i].assignedTo);

        
        char status[20];

        if(tasks[i].status == TODO) strcpy(status, "TODO");
        else if(tasks[i].status == IN_PROGRESS) strcpy(status, "IN_PROGRESS");
        else strcpy(status, "DONE");


        GtkWidget *l1 = gtk_label_new(id);
        GtkWidget *l2 = gtk_label_new(tasks[i].title);
        GtkWidget *l3 = gtk_label_new(assigned);
        GtkWidget *l4 = gtk_label_new(status);

        gtk_grid_attach(GTK_GRID(task_box), l1, 0, i+1, 1, 1);
        gtk_grid_attach(GTK_GRID(task_box), l2, 1, i+1, 1, 1);
        gtk_grid_attach(GTK_GRID(task_box), l3, 2, i+1, 1, 1);
        gtk_grid_attach(GTK_GRID(task_box), l4, 3, i+1, 1, 1);

        

    }

    gtk_widget_show_all(task_box);
}

// ================= HANDLERS =================
void on_add_clicked(GtkWidget *widget, gpointer data) {
    const char *title = gtk_entry_get_text(GTK_ENTRY(entry_title));
    const char *desc  = gtk_entry_get_text(GTK_ENTRY(entry_desc));

    if(strlen(title) == 0) return;

    createTask((char*)title, (char*)desc);

    gtk_entry_set_text(GTK_ENTRY(entry_title), "");
    gtk_entry_set_text(GTK_ENTRY(entry_desc), "");
}

void on_update_clicked(GtkWidget *w, gpointer d) {

    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_update_id));
    if(strlen(id_text) == 0) return;

    int taskId = atoi(id_text);

    int status = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_status));

    int result = updateTaskStatus(taskId, status);

    if(result == 0) {
        printf("Task not found\n");
    }

    gtk_entry_set_text(GTK_ENTRY(entry_update_id), "");
}

void on_delete_clicked(GtkWidget *w, gpointer d) {

    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_delete_id));

    if(strlen(id_text) == 0) return;

    int taskId = atoi(id_text);

    int result = deleteTask(taskId);

    if(result == 0) {
        printf("Task not found\n");
    }
    refresh_tasks(); 

    gtk_entry_set_text(GTK_ENTRY(entry_delete_id), "");
}

// NAVIGATION
void show_create(GtkWidget *w, gpointer d) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "create");
}

void show_view(GtkWidget *w, gpointer d) {
    printf("view button clicked\n");
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "view");
    refresh_tasks();
}

void show_assign(GtkWidget *w, gpointer d) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "assign");
}

void show_update(GtkWidget *w, gpointer d) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "update");
}

void show_delete(GtkWidget *w, gpointer d) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "delete");
}

// ================= ASSIGN_TASK =================
void on_assign_clicked(GtkWidget *w, gpointer d) {
    const char *task_text = gtk_entry_get_text(GTK_ENTRY(entry_task_id));
    const char *member_text = gtk_entry_get_text(GTK_ENTRY(entry_member_id));

    if(strlen(task_text) == 0 || strlen(member_text) == 0) return;

    int taskId = atoi(task_text);
    int memberId = atoi(member_text);

    int result=assignTask(taskId, memberId);
    if(result == 0) {
    printf("Task not found\n");
    }

    gtk_entry_set_text(GTK_ENTRY(entry_task_id), "");
    gtk_entry_set_text(GTK_ENTRY(entry_member_id), "");
}


// ================= MAIN =================
GtkWidget *stack;


int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Task Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 500);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 🔥 MAIN HORIZONTAL LAYOUT (IMPORTANT)
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

   // ================= SIDEBAR =================
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_size_request(sidebar, 200, -1);

    GtkWidget *btn_create = gtk_button_new_with_label("Create Task");
    GtkWidget *btn_assign = gtk_button_new_with_label("Assign Task");
    GtkWidget *btn_view   = gtk_button_new_with_label("View Tasks");
    GtkWidget *btn_update = gtk_button_new_with_label("Update Status");
    GtkWidget *btn_delete = gtk_button_new_with_label("Delete Task");

    gtk_box_pack_start(GTK_BOX(sidebar), btn_create, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(sidebar), btn_assign, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(sidebar), btn_view, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(sidebar), btn_update, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(sidebar), btn_delete, FALSE, FALSE, 10);

    // 🔥 ADD THESE (IMPORTANT)
    g_signal_connect(btn_create, "clicked", G_CALLBACK(show_create), NULL);
    g_signal_connect(btn_assign, "clicked", G_CALLBACK(show_assign), NULL);
    g_signal_connect(btn_view,   "clicked", G_CALLBACK(show_view), NULL);

    // ================= STACK =================
    stack = gtk_stack_new();
    

    // ===== CREATE PAGE =====
    GtkWidget *create_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    entry_title = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_title), "Enter Title");

    entry_desc = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_desc), "Enter Description");

    GtkWidget *add_btn = gtk_button_new_with_label("Add Task");

    gtk_box_pack_start(GTK_BOX(create_page), entry_title, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(create_page), entry_desc, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(create_page), add_btn, FALSE, FALSE, 10);

    g_signal_connect(add_btn, "clicked", G_CALLBACK(on_add_clicked), NULL);
  

    //  ===== ASSIGN PAGE (ADD HERE) =====
    GtkWidget *assign_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    entry_task_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_task_id), "Enter Task ID");

    entry_member_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_member_id), "Enter Member ID");

    GtkWidget *assign_btn = gtk_button_new_with_label("Assign Task");

    gtk_box_pack_start(GTK_BOX(assign_page), entry_task_id, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(assign_page), entry_member_id, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(assign_page), assign_btn, FALSE, FALSE, 10);

    g_signal_connect(assign_btn, "clicked", G_CALLBACK(on_assign_clicked), NULL);

    // ================= UPDATE_STATUS PAGE =================

    GtkWidget *update_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    entry_update_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_update_id), "Enter Task ID");

    combo_status = gtk_combo_box_text_new();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_status), "TODO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_status), "IN_PROGRESS");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_status), "DONE");

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_status), 0);

    GtkWidget *update_btn = gtk_button_new_with_label("Update Status");

    gtk_box_pack_start(GTK_BOX(update_page), entry_update_id, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(update_page), combo_status, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(update_page), update_btn, FALSE, FALSE, 10);

    g_signal_connect(update_btn, "clicked", G_CALLBACK(on_update_clicked), NULL);

    // ===== DELETE PAGE =====
    GtkWidget *delete_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    entry_delete_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_delete_id), "Enter Task ID");

    GtkWidget *delete_btn = gtk_button_new_with_label("Delete Task");

    gtk_box_pack_start(GTK_BOX(delete_page), entry_delete_id, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(delete_page), delete_btn, FALSE, FALSE, 10);

    g_signal_connect(delete_btn, "clicked", G_CALLBACK(on_delete_clicked), NULL);

    // ===== VIEW PAGE =====
    GtkWidget *view_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    task_box = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(task_box), 5);
    gtk_grid_set_column_spacing(GTK_GRID(task_box), 20);
    gtk_box_pack_start(GTK_BOX(view_page), task_box, TRUE, TRUE, 10);

    // ADD TO STACK
    gtk_stack_add_named(GTK_STACK(stack), create_page, "create");
    gtk_stack_add_named(GTK_STACK(stack), assign_page, "assign");
    gtk_stack_add_named(GTK_STACK(stack), view_page, "view");
    gtk_stack_add_named(GTK_STACK(stack), update_page, "update");
    gtk_stack_add_named(GTK_STACK(stack), delete_page, "delete");

    // ================= NAVIGATION =================
    g_signal_connect(btn_create, "clicked", G_CALLBACK(show_create), NULL);
    g_signal_connect(btn_view,   "clicked", G_CALLBACK(show_view), NULL);
    g_signal_connect(btn_update, "clicked", G_CALLBACK(show_update), NULL);
    g_signal_connect(btn_delete, "clicked", G_CALLBACK(show_delete), NULL);

    // DEFAULT PAGE
    // gtk_widget_show_all(window);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "create");

    // ================= FINAL LAYOUT =================
    gtk_box_pack_start(GTK_BOX(main_box), sidebar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), stack, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), main_box);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}