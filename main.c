#include <gtk/gtk.h>

static void on_yes_clicked(GtkButton *button, gpointer user_data) {
    GtkApplication *app = GTK_APPLICATION(user_data);
    g_application_quit(G_APPLICATION(app));
}

static void on_no_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = GTK_WIDGET(user_data);
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void on_close_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    gtk_builder_add_from_file(builder, "ui/dialog.ui", &error);
    if (error) {
        g_error("Failed to load dialog.ui: %s", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return;
    }

    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "confirm_dialog"));
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(main_window));

    GtkButton *yes = GTK_BUTTON(gtk_builder_get_object(builder, "yes_button"));
    GtkButton *no  = GTK_BUTTON(gtk_builder_get_object(builder, "no_button"));

    GtkApplication *app = gtk_window_get_application(GTK_WINDOW(main_window));

    g_signal_connect(yes, "clicked", G_CALLBACK(on_yes_clicked), app);
    g_signal_connect(no, "clicked", G_CALLBACK(on_no_clicked), dialog);

    gtk_window_present(GTK_WINDOW(dialog));
    g_object_unref(builder);
}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;
    if (!gtk_builder_add_from_file(builder, "ui/menu.ui", &error)) {
        g_error("Failed to load menu.ui: %s", error->message);
        g_clear_error(&error);
        g_object_unref(builder);
        return;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_window_set_application(GTK_WINDOW(window), app);

    GtkButton *close_button = GTK_BUTTON(gtk_builder_get_object(builder, "button"));
    g_signal_connect(close_button, "clicked", G_CALLBACK(on_close_clicked), window);

    gtk_window_present(GTK_WINDOW(window));
    g_object_unref(builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("org.example.ifc", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
