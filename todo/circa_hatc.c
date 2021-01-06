#ifndef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

circa_static
circa_err C3(hatc, T, alloc)(C2(hatc, T) *h) {
  circa_nullck(h);
  h->l = NULL;
  h->m = malloc(sizeof(*h->m));
  h->h = malloc(sizeof(*h->h) << 1);
  h->m_cap = 1;
  h->l_len = 0;
  h->m_len = 0;
  h->h_len = 0;
  h->len = 0;
}

circa_static
circa_err C3(hatc, T, free)(C2(hatc, T) *h) {
  circa_nullck(h);
  for (size_t i = 0; i < h->m_len; i++)
    free(h->m[i]);
  free(h->l);
  free(h->m);
  free(h->h);
}

#undef circa_static

/*
** Copyright 2020 David Garland
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/
