// RUN: %clam -O0 --lower-unsigned-icmp --crab-inter --crab-dom=int --crab-track=mem --crab-singleton-aliases --crab-heap-analysis=cs-sea-dsa --llvm-peel-loops=1 --crab-check=assert --crab-sanity-checks "%s" 2>&1 | OutputCheck %s
// CHECK: ^1  Number of total safe checks$

/** Same as test-arr-4.c but with --crab-track=mem **/

extern int nd ();
extern void __CRAB_assert(int);

int x = 5;
int y = 3;

void foo ()
{
  x++;
}

void bar ()
{
  y++;
}

int a[10];

int main ()
{
  int i;
  foo ();
  for (i=0;i<10;i++)
  {
    if (nd ())
      a[i] =y;
    else 
      a[i] =x;
  }
  bar ();
  int res = a[i-1];
  __CRAB_assert(res >= 0 && res <= 6);
  return res;
}
