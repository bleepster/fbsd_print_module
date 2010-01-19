#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/module.h>

struct print_data {
  char params[256];
};

int main(int argc, char **argv)
{
   int syscall_num;
   struct print_data pd;
   struct module_stat stat;

   memset(&pd, '\0', sizeof(struct print_data));
   if (argc < 2)
     snprintf(pd.params, 256, "this is something"); 
   else
     snprintf(pd.params, 256, "%s", argv[1]); 

   stat.version = sizeof(stat);
   if (modstat(modfind("print_module"), &stat) < 0) {
     perror("error!\n");
     return (1);
   }

   syscall_num = stat.data.intval;

   printf("pd: %p\n", &pd);
   if (syscall(syscall_num, &pd) < 0) {
     perror("error!\n");
     return (1);
   }

   return (0);
}
