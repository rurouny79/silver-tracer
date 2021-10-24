#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// #define DUMP(func, call) printf("%s: func = %p, called by = %p\n", __FUNCTION__, func, call)
#define DUMP(func, call) printf("%p\n", call_site)

static int call_level = 0;
static void *last_fn = NULL;

void __attribute__((no_instrument_function)) __cyg_profile_func_enter(void *this_func, void *call_site)
{
    // DUMP(this_func, call_site);
    int i;
    Dl_info di;

    if (last_fn != this_func) ++call_level;
    for (i = 0; i < call_level-1; i++) printf("   ");
    if (dladdr(this_func, &di)) {
        printf("%s\t\t(%s)\n", di.dli_sname ? di.dli_sname : "<unknown>", di.dli_fname);
    }
    last_fn = this_func;
}

void __attribute__((no_instrument_function)) __cyg_profile_func_exit(void *this_func, void *call_site)
{
    // DUMP(this_func, call_site);

    --call_level;
#if 0
    Dl_info di;
    if (dladdr(this_func, &di)) {
        printf("%s (%s)\n", di.dli_sname ? di.dli_sname : "<unknown>", di.dli_fname);
    }
#endif
}

int do_multi(int a, int b) {return a * b;}
int do_add(int a, int b) {return a+b;}
int do_calc(int a, int b) {do_multi(a, b);}

int main()
{
  int a = 4, b = 5;
  do_calc(a, b);
  do_add(a, b);
  return 0;
}
