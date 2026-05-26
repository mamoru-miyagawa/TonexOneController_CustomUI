#include "vars.h"

/* Hand-maintained backing storage for EEZ flow variables declared in vars.h.
   EEZ Studio declares get_/set_var_* but expects the integrator to provide
   the C implementation. */

static int32_t s_bpm_gauge = 0;

int32_t get_var_bpm_gauge(void)
{
    return s_bpm_gauge;
}

void set_var_bpm_gauge(int32_t value)
{
    s_bpm_gauge = value;
}
