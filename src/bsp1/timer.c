#include <assert.h>
#include <GLFW/glfw3.h>
#include "timer.h"

static
timer_func callback;

static
double base_interval = 0.5;  // base interval

static
double current_interval;  // current interval

static
double speedup_factor = 0.9;  // factor for speedup

extern
void timer_init(timer_func on_tick) {
    assert(on_tick);
    callback = on_tick;
    timer_reset();
}

extern
void timer_test(void) {
    if (glfwGetTime() >= current_interval) {
        callback();
        timer_reset();
    }
}

extern
void timer_reset(void) {
    glfwSetTime(0);
}

extern
double timer_get_interval(void) {
    return current_interval;
}

extern
void timer_set_interval(double itvl) {
    base_interval = itvl;
    current_interval = base_interval;
}

extern
void timer_speedup(void) {
    // speed up interval based on factor
    current_interval *= speedup_factor;

    // limit acceleration to prohibit a too fast speed
    if (current_interval < 0.1) {
        current_interval = 0.1;
    }
}

extern
void timer_reset_speed(void) {
    // reset invterval on base interval
    current_interval = base_interval;
}
