/* Read Only Memory
   Jacob Sorber
   https://www.youtube.com/watch?v=AYSISa95oJE

*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    char *p1 = malloc(sizeof(char) * 50);
    /*  irgendein Memorybereich, z.B. 0x55fc07db22a0 (2a0)
        dies ist keine pagegrenze
        Pagegrenze: 0x55fc07db22a0 & 0xfffffffff000 = 55FC07DB2000
    */
    printf("p1: %p\n", p1);

    long pagesize = sysconf(_SC_PAGE_SIZE);
    if (pagesize == -1)
    {
        perror("sysconf");
    }
    int buffersize = 2 * pagesize;

    char *p2, *p3;

    /* Memory startet bei pagesize*/
    /* x559fd734b000 erkennbar an den 000 am Ende */

    if (posix_memalign((void **)&p2, pagesize, buffersize))
    {
        perror("posix_memalign");
    }

    /* f addr is NULL, then the kernel chooses the (page-aligned) address at which to create the mapping */
    /* 0x7f7436bff000 erkennbar an den 000 am Ende */
    p3 = mmap(NULL, buffersize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (p3 == MAP_FAILED)
    {
        perror("mmap error");
    }

    printf("p2: %p\n", p2);
    printf("p3: %p\n", p3);

    printf("writing ...\n");
    memset(p2, 5, buffersize);
    memset(p3, 5, buffersize);
    printf("writing done!\n");

    /* pagesize kann nun beschrieben werden, die zweite Page (p2+pagesize) ist ReadOnly*/
    if (mprotect(p2 + pagesize, pagesize, PROT_READ) == -1)
    {
        perror("mprotect");
    }

    /* wenn man nun in den gesamten Buffer (=2*Pagesize) schreibt, funktioniert das in der
       ersten Page, in der zweiten nicht, Programm bricht dann ab (segmentation fault)
       */
    for (int i = 0; i < buffersize; i++)
    {
        printf("%d (%p): writing ...\n", i, p2 + i);
        p2[i] = 3;
    }

    return EXIT_SUCCESS;
}