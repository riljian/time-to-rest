#include <libnotify/notify.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define DURATION 3

static GMainLoop *loop;

NotifyNotification *
notification_create ();

static void
close_callback  (GObject *);

static void
shutup_callback (NotifyNotification *, const char *);

static void
done_callback   (NotifyNotification *, const char *);

int
main (int argc, char **argv)
{
    if (!notify_init ("Time to Rest"))
    {
        exit (1);
    }

    loop = g_main_loop_new (NULL, FALSE);

    notification_create ();

    g_main_loop_run (loop);

    return 0;
}

NotifyNotification *
notification_create ()
{
    NotifyNotification *n;

    n = notify_notification_new ("Time to Rest",
                                 "Drink some water.",
                                 "avatar-default-symbolic");
    notify_notification_set_hint (n,
                                  "transient",
                                  g_variant_new_boolean (TRUE));
    notify_notification_set_urgency (n, NOTIFY_URGENCY_CRITICAL);
    notify_notification_set_timeout (n, NOTIFY_EXPIRES_DEFAULT);
    notify_notification_add_action (n,
                                    "done",
                                    "Done",
                                    (NotifyActionCallback)
                                    done_callback,
                                    NULL,
                                    NULL);
    notify_notification_add_action (n,
                                    "shutup",
                                    "Shut Up",
                                    (NotifyActionCallback)
                                    shutup_callback,
                                    NULL,
                                    NULL);
    g_signal_connect (G_OBJECT (n),
                      "closed",
                      G_CALLBACK (close_callback),
                      NULL);
    notify_notification_set_category (n, "health");

    if (!notify_notification_show (n, NULL))
    {
        fprintf (stderr, "failed to send notification\n");
        return NULL;
    }

    return n;
}

static void
close_callback (GObject *obj)
{
    g_object_unref (obj);

    sleep (DURATION);

    if (!notification_create ()) {
        g_main_loop_quit (loop);
    }
}

static void
done_callback (NotifyNotification *n,
               const char         *action)
{
    g_assert (action != NULL);
    g_assert (strcmp (action, "done") == 0);

    g_object_unref (G_OBJECT (n));

    sleep (DURATION);

    if (!notification_create ()) {
        g_main_loop_quit (loop);
    }
}

static void
shutup_callback (NotifyNotification *n,
                 const char         *action)
{
    g_assert (action != NULL);
    g_assert (strcmp (action, "shutup") == 0);

    g_object_unref (G_OBJECT (n));

    g_main_loop_quit (loop);
}
