#ifndef MOUSE_H
#define MOUSE_H

void mouse_install(void);
void mouse_handle_irq(void);
int mouse_get_x(void);
int mouse_get_y(void);

#endif
