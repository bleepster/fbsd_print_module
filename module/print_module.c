#include <sys/types.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysproto.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>

struct print_data {
  char params[256];
};

struct print_something_args {
  struct print_data *args;
};

static int
print_something(struct thread *td, void *params)
{
  int error = 1;
  char str[256];
  struct print_something_args *uap = NULL;

  uap = (struct print_something_args *)params;
  
  if (uap->args) {
    bzero(str, sizeof(str));

    error = copyinstr(uap->args->params, str, sizeof(uap->args->params), NULL);
    if(!error)
      printf (">> %s\n", str);
  }

  return (error);
}

static struct sysent print_something_systent = {
  1,                  /* number of arguments */
  print_something     /* function name       */
};

static int offset = NO_SYSCALL;

static int
load(struct module *module, int cmd, void *arg)
{
  int err = 0;

  switch (cmd) {
  case MOD_LOAD:
    printf("loading print module\n");
    break;

  case MOD_UNLOAD:
    printf("unloading print module\n");
    break;

  default:
    err = EOPNOTSUPP;
    break;
  }

  return (err);
}

SYSCALL_MODULE(print_module, &offset, &print_something_systent, load, NULL);
