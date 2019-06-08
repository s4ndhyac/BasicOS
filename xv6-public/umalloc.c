#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
// #include "thread_mutex.h"
// #include "thread_util.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;

union header {
  struct
  {
    union header *ptr;
    uint size;
  } s;
  Align x;
  //struct thread_mutex ml;
};

// // mutex
// void mutex_lock(struct thread_mutex *m)
// {
//   // The xchg is atomic.
//   while (xchg(&m->locked, 1) != 0)
//     sleep(1);
//   __sync_synchronize();
// }

// void mutex_unlock(struct thread_mutex *m)
// {
//   __sync_synchronize();
//   asm volatile("movl $0, %0"
//                : "+m"(m->locked)
//                :);
// }

typedef union header Header;

static Header base; //= {.ml.locked = 0}
static Header *freep;

void free(void *ap)
{
  Header *bp, *p;

  bp = (Header *)ap - 1;
  //mutex_lock(&base.ml);
  for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if (bp + bp->s.size == p->s.ptr)
  {
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  }
  else
    bp->s.ptr = p->s.ptr;
  if (p + p->s.size == bp)
  {
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  }
  else
    p->s.ptr = bp;
  freep = p;
  //mutex_unlock(&base.ml);
}

static Header *
morecore(uint nu)
{
  char *p;
  Header *hp;

  if (nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if (p == (char *)-1)
    return 0;
  hp = (Header *)p;
  hp->s.size = nu;
  free((void *)(hp + 1));
  return freep;
}

void *
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
  //mutex_lock(&base.ml);
  if ((prevp = freep) == 0)
  {
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr)
  {
    if (p->s.size >= nunits)
    {
      if (p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else
      {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      //mutex_unlock(&base.ml);
      return (void *)(p + 1);
    }
    if (p == freep)
      if ((p = morecore(nunits)) == 0)
      {
        //mutex_unlock(&base.ml);
        return 0;
      }
  }
  //mutex_unlock(&base.ml);
}
