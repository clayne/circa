#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #include "circa_str.h"
  #define circa_static
#endif

circa_static
circa_err str_alloc(str *const restrict s) {
  circa_nullck(s);
  s->data = malloc(2);
  circa_oomck(s->data);
  s->data[0] = '\0';
  s->cap = 2;
  s->len = 0;
  return CE_NONE;
}

circa_static
circa_err str_alloc_cap(str *const restrict s, register const size_t cap) {
  circa_nullck(s);
  circa_oneck(cap);
  s->data = malloc(cap);
  circa_oomck(s->data);
  s->cap = cap;
  s->len = 0;
  return CE_NONE;
}

circa_static
circa_err str_require(str *const restrict s, register const size_t cap) {
  circa_nullck(s);
  if (cap > s->cap) {
    char *const restrict new_data = realloc(s->data, cap);
    circa_oomck(new_data);
    s->data = new_data;
    s->cap = cap;
  }
  return CE_NONE;
}

circa_static
circa_err str_amort(str *const restrict s) {
  circa_nullck(s);
  if (s->len + 1 == s->cap) {
    register const size_t new_cap = s->cap + (s->cap >> 1);
    char *const restrict new_data = realloc(s->data, new_cap);
    circa_oomck(new_data);
    s->data = new_data;
    s->cap += (s->cap >> 1);
  }
  return CE_NONE;
}

circa_static
circa_err str_free(str *const restrict s) {
  circa_nullck(s);
  free(s->data);
  return CE_NONE;
}

circa_static
circa_err str_set_v(str *const restrict s, register const size_t i, register const char c) {
  circa_nullck(s);
  circa_zerock(c);
  s->data[i] = c;
  return CE_NONE;
}

circa_static
circa_err str_set_r(str *const restrict s, register const size_t i, char *const restrict c) {
  circa_nullck(s);
  circa_nullck(c);
  circa_zerock(*c);
  s->data[i] = *c;
  return CE_NONE;
}

circa_static
char str_get_v(str *const restrict s, register const size_t i) {
  // TODO null & bounds checking + logging
  return s->data[i];
}

circa_static
circa_err str_get_r(str *const restrict s, register const size_t i, char *const restrict c) {
  circa_nullck(s);
  circa_nullck(c);
  *c = s->data[i];
  return CE_NONE;
}

circa_static
circa_err str_snoc_v(str *const restrict s, register const char c) {
  circa_nullck(s);
  circa_zerock(c);
  register const circa_err e = str_amort(s);
  circa_if_oomck (e) circa_throw(e, "Out of memory.");
  s->data[s->len++] = c;
  s->data[s->len] = '\0';
  return CE_NONE;
}

circa_static
circa_err str_snoc_r(str *const restrict s, char *const restrict c) {
  circa_nullck(s);
  circa_nullck(c);
  circa_zerock(*c);
  register const circa_err e = str_amort(s);
  circa_if_oomck (e) circa_throw(e, "Out of memory.");
  s->data[s->len++] = *c;
  s->data[s->len] = '\0';
  return CE_NONE;
}

circa_static
char str_pop_v(str *const restrict s) {
  circa_nullck(s);
  // TODO: oomck
  char c = s->data[--s->len];
  s->data[s->len] = '\0';
  return c;
}

circa_static
circa_err str_pop_r(str *const restrict s, char *const restrict c) {
  circa_nullck(s);
  circa_nullck(c);
  // TODO: oomck
  *c = s->data[--s->len];
  s->data[s->len] = '\0';
  return CE_NONE;
}

circa_static
circa_err str_cpy(str *const restrict dst, str *const restrict src) {
  circa_nullck(dst);
  circa_nullck(src);
  register const size_t req = src->len + 1;
  if (dst->cap < req) {
    // NOTE: this is more performant than the naive "str_require" here in that
    // unnecessary copying of the old string contents in the case of size-up
    // is avoided.
    char *const restrict new_data = malloc(req);
    circa_oomck(new_data);
    dst->data = new_data;
  }
  memcpy(dst->data, src->data, req);
  dst->len = src->len;
  return CE_NONE;
}

circa_static
circa_err str_cpy_ur(str *dst, str *src) {
  circa_nullck(dst);
  circa_nullck(src);
  // TODO
  return CE_NONE;
}

circa_static
circa_err str_cpy_cstr(str *const restrict dst, char *restrict src) {
  circa_nullck(dst);
  circa_nullck(src);
  register const size_t old_len = dst->len;
  dst->len = 0;
  while (*src) {
    register const circa_err e = str_snoc_r(dst, src++);
    circa_if_oomck (e) circa_throw(e, "Out of memory.");
  }
  return CE_NONE;
}

circa_static
circa_err str_cpy_cstr_ur(str *dst, char *src) {
  circa_nullck(dst);
  circa_nullck(src);
  // TODO
  return CE_NONE;
}

circa_static
circa_err str_cat(str *const restrict dst, str *const restrict src) {
  circa_nullck(dst);
  circa_nullck(src);
  char *restrict c = src->data;
  while (*c) {
    register const circa_err e = str_snoc_r(dst, c++);
    circa_if_oomck (e) return e;
  }
  return CE_NONE;
}

circa_static
circa_err str_cat_ur(str *const dst, str *const src) {
  circa_nullck(dst);
  circa_nullck(src);
  char *const stop = src->data + src->len;
  char *c = src->data;
  while (c != stop) {
    register const circa_err e = str_snoc_r(dst, c++);
    circa_if_oomck (e) return e;
  }
  return CE_NONE;
}

circa_static
circa_err str_cat_cstr(str *const restrict dst, char *restrict src) {
  circa_nullck(dst);
  circa_nullck(src);
  while (*src) {
    register const circa_err e = str_snoc_r(dst, src++);
    circa_if_oomck (e) return e;
  }
  return CE_NONE;
}

circa_static
circa_err str_cat_cstr_ur(str *const dst, char *src) {
  circa_nullck(dst);
  circa_nullck(src);
  register const size_t len = strlen(src);
  char *const stop = src + len;
  while (src != stop) {
    register const circa_err e = str_snoc_r(dst, src++);
    circa_if_oomck (e) return e;
  }
  return CE_NONE;
}

circa_static
circa_err str_cat_cstr_ur_len(str *dst, char *src, size_t len) {
  circa_nullck(dst);
  circa_nullck(src);
  char *const stop = src + len;
  while (src != stop) {
    circa_err e = str_snoc_r(dst, src++);
    circa_if_oomck (e) circa_throw(e, "Out of memory.");
  }
  return CE_NONE;
}

circa_static
circa_err str_vfmt(str *const restrict s, const char *const restrict fmt, register va_list ap) {
  circa_nullck(s);
  circa_nullck(fmt);
  va_list ap2;
  va_copy(ap2, ap);
  register const size_t len = (size_t) vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  register const circa_err e = str_require(s, len + 1);
  circa_if_oomck (e) circa_throw(e, "Out of memory.");
  if_unlikely ((size_t) vsnprintf(s->data, len + 1, fmt, ap) != len) circa_throw(CE_FMT, "Formatting error.");
  // NOTE: vsnprintf null terminates, so there is no need to do that manually
  s->len = len;
  return CE_NONE;
}

circa_static
circa_err str_fmt(str *const restrict s, const char *const restrict fmt, ...) {
  circa_nullck(s);
  circa_nullck(fmt);
  va_list ap;
  va_start(ap, fmt);
  register const circa_err e = str_vfmt(s, fmt, ap);
  if_unlikely (e) circa_throw(e, "Formatting error."); // TODO: make "circa_if_fmtck"?
  va_end(ap);
  return CE_NONE;
}

circa_static
circa_err str_cat_vfmt(str *const restrict s, const char *const restrict fmt, register va_list ap) {
  circa_nullck(s);
  circa_nullck(fmt);
  va_list ap2;
  va_copy(ap2, ap);
  register const size_t len = (size_t) vsnprintf(NULL, 0, fmt, ap2);
  va_end(ap2);
  register const circa_err e = str_require(s, s->len + len + 1);
  circa_if_oomck (e) circa_throw(e, "Out of memory.");
  if_unlikely ((size_t) vsnprintf(s->data + s->len, len + 1, fmt, ap) != len) circa_throw(CE_FMT, "Formatting error.");
  // NOTE: vsnprintf null terminates, so there is no need to do that manually
  s->len += len;
  return CE_NONE;
}

circa_static
circa_err str_cat_fmt(str *const restrict s, const char *const restrict fmt, ...) {
  circa_nullck(s);
  circa_nullck(fmt);
  va_list ap;
  va_start(ap, fmt);
  register const circa_err e = str_cat_vfmt(s, fmt, ap);
  if_unlikely (e) circa_throw(e, "Formatting error."); // TODO: make "circa_if_fmtck"?
  va_end(ap);
  return CE_NONE;
}

circa_static
circa_err str_read(str *const restrict s, FILE *const restrict fp) {
  circa_nullck(s);
  circa_nullck(fp);
  if_unlikely (fseek(fp, 0, SEEK_END)) circa_throw(CE_READ, "Failed to read from file.");
  register const size_t len = (size_t) ftell(fp);
  if_unlikely (fseek(fp, 0, SEEK_SET)) circa_throw(CE_READ, "Failed to read from file.");
  register const circa_err e = str_require(s, len + 1);
  circa_if_oomck (e) circa_throw(e, "Out of memory.");
  if_unlikely (fread(s->data, 1, len, fp) != len) circa_throw(CE_READ, "Failed to read from file.");
  s->data[len] = '\0';
  s->len = len;
  return CE_NONE;
}

circa_static
circa_err str_read_name(str *const restrict s, char *const restrict path) {
  circa_nullck(s);
  circa_nullck(path);
  FILE *const restrict fp = fopen(path, "r");
  if_unlikely (!fp) return CE_READ;
  register const circa_err e = str_read(s, fp);
  fclose(fp);
  return e;
}

circa_static
circa_err str_cat_read(str *const restrict s, FILE *const restrict fp) {
  circa_nullck(s);
  circa_nullck(fp);
  if_unlikely (fseek(fp, 0, SEEK_END)) circa_throw(CE_READ, "Failed to read from file.");
  register const size_t len = (size_t) ftell(fp);
  if_unlikely (fseek(fp, 0, SEEK_SET)) circa_throw(CE_READ, "Failed to read from file.");
  register const circa_err e = str_require(s, s->len + len + 1);
  circa_if_oomck (e) circa_throw(e, "Out of memory.");
  if_unlikely (fread(s->data + s->len, 1, len, fp) != len) circa_throw(CE_READ, "Failed to read from file.");
  register const size_t new_len = s->len + len;
  s->data[new_len] = '\0';
  s->len = new_len;
  return CE_NONE;
}

circa_static
circa_err str_cat_read_name(str *const restrict s, char *const restrict path) {
  circa_nullck(s);
  circa_nullck(path);
  FILE *const restrict fp = fopen(path, "r");
  if_unlikely (!fp) circa_throw(CE_READ, "Failed to read from file.");
  register const circa_err e = str_cat_read(s, fp);
  fclose(fp);
  return e;
}

circa_static
circa_err str_write(str *const restrict s, FILE *const restrict fp) {
  circa_nullck(s);
  circa_nullck(fp);
  return (fputs(s->data, fp) == EOF) ? CE_WRITE : CE_NONE;
}

circa_static
circa_err str_write_name(str *const restrict s, char *const restrict path) {
  circa_nullck(s);
  circa_nullck(path);
  FILE *const restrict fp = fopen(path, "w+");
  if_unlikely (!fp) circa_throw(CE_WRITE, "Failed to write to file.");
  register const circa_err e = str_write(s, fp);
  fclose(fp);
  return e;
}

circa_static
circa_err str_write_cat_name(str *const restrict s, char *const restrict path) {
  circa_nullck(s);
  circa_nullck(path);
  FILE *const restrict fp = fopen(path, "a");
  if_unlikely (!fp) circa_throw(CE_WRITE, "Failed to write to file.");
  register const circa_err e = str_write(s, fp);
  fclose(fp);
  return e;
}

circa_static
cmp str_cmp(str *a, str *b) {
  if (a->data == b->data) return EQ;
  for (size_t i = 0; i < size_t_min(a->len, b->len); i++)
    if (a->data[i] < b->data[i])
      return LT;
    else if (a->data[i] > b->data[i])
      return GT;
  return size_t_cmp(&a->len, &b->len);
}

circa_static
size_t str_hash_seeded(str *s, size_t seed) {
  for (size_t i = 0; i < s->len; i++)
    seed = (seed ^ (size_t) s->data[i]) * fnv_prime_size;
  return seed;
}

circa_static
size_t str_hash(str *s) {
  return str_hash_seeded(s, fnv_offset_size);
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
