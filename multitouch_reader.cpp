#include <libinput.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

static int open_restricted(const char *path, int flags, void *user_data)
{
        int fd = open(path, flags);
        return fd < 0 ? -errno : fd;
}
 
static void close_restricted(int fd, void *user_data)
{
        close(fd);
}
 
 const static struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted,
};
 
int main() {
    // Open the libinput context
    struct libinput *li;
    struct udev *udev;
    udev = udev_new();
    if (!udev) {
        perror("Failed to create udev context");
        return 1;
    }

    li = libinput_udev_create_context(&interface, NULL, udev);
    if (!li) {
        perror("Failed to create libinput context");
        udev_unref(udev);
        return 1;
    }

    libinput_udev_assign_seat(li, "seat0");
    if (!li) {
        perror("Failed to create libinput context");
        udev_unref(udev);
        return 1;
    }

    // Main event loop
    while (1)
    {
        struct libinput_event *ev;
        libinput_dispatch(li);

        while ((ev = libinput_get_event(li)))
        {
            libinput_event_type evType = libinput_event_get_type(ev);

            if (evType == LIBINPUT_EVENT_TOUCH_DOWN)
            {
                // Touch down event
                struct libinput_event_touch *tev = libinput_event_get_touch_event(ev);
                if (tev)
                {
                    int touch_id = libinput_event_touch_get_slot(tev);
                    double x = libinput_event_touch_get_x_transformed(tev, 1.0);
                    double y = libinput_event_touch_get_y_transformed(tev, 1.0);
                    printf("Touch Down - ID: %d, X: %f, Y: %f\n", touch_id, x, y);
                }
            }
            else if (evType == LIBINPUT_EVENT_TOUCH_UP)
            {
                // Touch up event
                struct libinput_event_touch *tev = libinput_event_get_touch_event(ev);
                if (tev)
                {
                    int touch_id = libinput_event_touch_get_slot(tev);
                    printf("Touch Up - ID: %d\n", touch_id);
                }
            }
            else if (evType == LIBINPUT_EVENT_TOUCH_MOTION)
            {
                // Touch motion event
                struct libinput_event_touch *tev = libinput_event_get_touch_event(ev);
                if (tev)
                {
                    int touch_id = libinput_event_touch_get_slot(tev);
                    double x = libinput_event_touch_get_x_transformed(tev, 1.0);
                    double y = libinput_event_touch_get_y_transformed(tev, 1.0);
                    printf("Touch Motion - ID: %d, X: %f, Y: %f\n", touch_id, x, y);
                }
            }
            else if (evType == LIBINPUT_EVENT_TOUCH_CANCEL)
            {
                // Touch cancel event
                struct libinput_event_touch *tev = libinput_event_get_touch_event(ev);
                if (tev)
                {
                    int touch_id = libinput_event_touch_get_slot(tev);
                    printf("Touch Cancel - ID: %d\n", touch_id);
                }
            }

            libinput_event_destroy(ev);
        }
    }

    // Clean up and close the libinput context
    libinput_unref(li);
    
    return 0;
}
