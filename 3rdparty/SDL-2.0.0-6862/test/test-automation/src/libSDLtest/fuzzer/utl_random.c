
/* 

 utl_random

 A "32-bit Multiply with carry" random number generator. 
 
*/


#include "utl_random.h"


/* Initialize random number generator with two integer variables */

void utl_randomInit(RND_CTX * rndContext, unsigned int xi, unsigned int ci)
{
  /*
   * Choose a value for 'a' from this list
   * 1791398085 1929682203 1683268614 1965537969 1675393560
   * 1967773755 1517746329 1447497129 1655692410 1606218150
   * 2051013963 1075433238 1557985959 1781943330 1893513180
   * 1631296680 2131995753 2083801278 1873196400 1554115554 
   */
  rndContext->a = 1655692410;
  rndContext->x = 30903;
  rndContext->c = 0;
  if (xi != 0)
    rndContext->x = xi;
  rndContext->c = ci;
  rndContext->ah = rndContext->a >> 16;
  rndContext->al = rndContext->a & 65535;
}

/* Initialize random number generator from time */

void utl_randomInitTime(RND_CTX * rndContext)
{
  int a,b;
  
  srand(time(NULL));
  a=rand();
  srand(clock());
  b=rand();
  utl_randomInit(rndContext, a, b);
}

/* Returns random numbers */

unsigned int utl_random(RND_CTX * rndContext)
{
  unsigned int xh, xl;

  xh = rndContext->x >> 16, xl = rndContext->x & 65535;
  rndContext->x = rndContext->x * rndContext->a + rndContext->c;
  rndContext->c =
    xh * rndContext->ah + ((xh * rndContext->al) >> 16) +
    ((xl * rndContext->ah) >> 16);
  if (xl * rndContext->al >= (~rndContext->c + 1))
    rndContext->c++;

  return (rndContext->x);
}
