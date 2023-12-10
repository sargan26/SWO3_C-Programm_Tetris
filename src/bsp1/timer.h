#ifndef TIMER_H
#define TIMER_H

typedef void(*timer_func)(void);

void   timer_init(timer_func on_tick);
void   timer_test(void);
void   timer_reset(void);
double timer_get_interval(void);
void   timer_set_interval(double itvl);

#endif
