#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

  mainProcess();
  //   // Replace with your keyboard's device file
  //   const char *device = "/dev/input/event6"; // event2
  //   int fd = open(device, O_RDONLY);
  //   if (fd == -1) {
  //     perror("Error opening device");
  //     return 1;
  //   }

  //   struct input_event ev;

  //   int mousex = 0, mousey = 0;
  //   printf("Listening for keyboard events. Press Ctrl+C to quit.\n");
  //   while (1) {
  //     // Read an event
  //     if (read(fd, &ev, sizeof(struct input_event)) ==
  //         sizeof(struct input_event)) {
  //       if (ev.type == EV_REL && ev.code == ABS_X) { // Key press event
  //         printf("coordinate mouse X %d\n", ev.value);
  //         mousex = ev.value;
  //       }
  //       if (ev.type == EV_REL && ev.code == ABS_Y) { // Key press event
  //         printf("coordinate mouse Y: %d\n", ev.value);
  //         mousey = ev.value;
  //       }

  //       if (ev.type == EV_KEY && ev.value == 0) { // Key release event
  //         printf("Key %d released.\n", ev.code);
  //       }
  //     }
  //   }

  //   close(fd);
  //   return 0;
}