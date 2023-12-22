#include <bits/stdc++.h>

int main()
{
  int l, w, h;
  unsigned long long a{0};
  while(scanf("%dx%dx%d", &l, &w, &h) != EOF)
  {
    //printf("l=%d w=%d h=%d\n", l, w, h);
    a += 2*l*w + 2*w*h + 2*h*l;
    a += std::min(std::min(l*w, l*h), w*h);
    //printf(">> a=%lu\n", a);
  }

  printf("Part one: %lu.\n", a);
}
