#ifndef EXIT_FAILURE
  #error "circa/str.h depends upon <stdlib.h>. PLease include it beforehand."
#endif

#ifndef CIRCA_ERR
  #error "circa/str.h depends upon circa/err.h. Please include it beforehand."
#endif

typedef struct str {
  char *data;
  size_t cap;
  size_t len;
} str;

static
circa_err str_alloc(str *s) {
  circa_nullck(s);
  s->data = malloc(2);
  circa_oomck(s->data);
  s->cap = 2;
  s->len = 0;
  return CE_NONE;
}

static
circa_err str_prealloc(str *s, size_t cap) {
  circa_nullck(s);
  circa_oneck(cap);
  s->data = malloc(cap);
  circa_oomck(s->data);
  s->cap = cap;
  s->len = 0;
  return CE_NONE;
}

static
circa_err str_amort(str *s) {
  circa_nullck(s);
  if (s->len + 1 == s->cap) {
    size_t new_cap = s->cap + (s->cap >> 1);
    s->data = realloc(s->data, new_cap);
    circa_oomck(s->data);
    s->cap += (s->cap >> 1);
  }
  return CE_NONE;
}

static
circa_err str_free(str *s) {
  circa_nullck(s);
  free(s->data);
  return CE_NONE;
}

static
circa_err str_set_v(str *s, size_t i, char c) {
  circa_nullck(s);
  circa_zerock(c);
  s->data[i] = c;
  return CE_NONE;
}

static
circa_err str_set_r(str *s, size_t i, char *c) {
  circa_nullck(s);
  circa_nullck(c);
  circa_zerock(*c);
  s->data[i] = *c;
  return CE_NONE;
}

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
