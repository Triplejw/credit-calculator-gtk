#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    GtkWidget *window, *grid, *label1, *label2, *dropdown[8], *button;
    GtkListStore *store;
    GtkTreeIter iter;

    gtk_init(&argc, &argv);

    // Create the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Credit Calculator");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Create the grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create the labels
    label1 = gtk_label_new("Karunya University of Technology and Sciences");
    label2 = gtk_label_new("Department: Electronics and Communication Engineering 2020-2021");

    // Add the labels to the grid
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);

    // Create the dropdown lists
    for (int i = 0; i < 8; i++) {
        store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
        dropdown[i] = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
        g_object_unref(store);

        // Populate the dropdown list from the data file
        char filename[] = "data";
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error opening file %s\n", filename);
            return 1;
        }
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            char course_code[10];
            char course_name[100];
            int credits;
            sscanf(line, "%s | %[^|] | %d", course_code, course_name, &credits);    
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, course_name, 1, credits, -1);
        }
        fclose(file);

        // Set up the dropdown list
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(dropdown[i]), renderer, TRUE);
        gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(dropdown[i]), renderer, "text", 0, NULL);
        gtk_combo_box_set_active(GTK_COMBO_BOX(dropdown[i]), 0);

        // Add the dropdown list to the grid
        char label_text[50];
        sprintf(label_text, "Semester %d:", i+1);
        GtkWidget *label = gtk_label_new(label_text);
        gtk_grid_attach(GTK_GRID(grid), label, 0, i+2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), dropdown[i], 1, i+2, 1, 1);
    }

// Callback function for the "Calculate Credits" button
void on_calculate_button_clicked(GtkWidget *widget, gpointer data) {
    int total_credits = 0;
    for (int i = 0; i < 8; i++) {
        GtkTreeIter iter;
        GtkTreeModel *model;
        int credits;
        if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(dropdown[i]), &iter)) {
            model = gtk_combo_box_get_model(GTK_COMBO_BOX(dropdown[i]));
            gtk_tree_model_get(model, &iter, 1, &credits, -1);
            total_credits += credits;
        }
    }

    char message[100];
    GtkWidget *dialog;
    if (total_credits < 24) {
        sprintf(message, "Need %d credits to complete the target", 24-total_credits);
        dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "%s", message);
    }
    else {
        dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Congratulations! You have met the credit requirements.");
    }
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

    // Create the calculate button
    button = gtk_button_new_with_label("Calculate Credits");

    // Add the button to the grid
    gtk_grid_attach(GTK_GRID(grid), button, 0, 10, 1, 1);

    // Show the window and its contents
    gtk_widget_show_all(window);

    // Set up signal handling
    g_signal_connect(button, "clicked", G_CALLBACK(on_calculate_button_clicked), NULL);

    // Set up signal handling
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Run the GTK+ main loop
    gtk_main();

    return 0;
}