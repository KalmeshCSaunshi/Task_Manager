#include <gtk/gtk.h>

GtkWidget *stack;

void show_home(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "home");
}

void show_create(GtkWidget *widget, gpointer data) {
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "create");
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *main_box, *sidebar, *content;
    GtkWidget *btn_home, *btn_create;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Task Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // 🔵 Sidebar
    sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(sidebar, 150, -1);

    btn_home = gtk_button_new_with_label("Home");
    btn_create = gtk_button_new_with_label("Create Task");

    gtk_box_pack_start(GTK_BOX(sidebar), btn_home, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), btn_create, FALSE, FALSE, 5);

    // 🔵 Stack (pages)
    stack = gtk_stack_new();

    GtkWidget *home_page = gtk_label_new("Welcome to Task Manager");
    GtkWidget *create_page = gtk_label_new("Create Task Page");

    gtk_stack_add_named(GTK_STACK(stack), home_page, "home");
    gtk_stack_add_named(GTK_STACK(stack), create_page, "create");

    // 🔵 Button actions
    g_signal_connect(btn_home, "clicked", G_CALLBACK(show_home), NULL);
    g_signal_connect(btn_create, "clicked", G_CALLBACK(show_create), NULL);

    // Layout
    gtk_box_pack_start(GTK_BOX(main_box), sidebar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), stack, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(window), main_box);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}