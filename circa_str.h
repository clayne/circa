#ifdef CIRCA_STATIC
  #define circa_static static
#else
  #define circa_static
#endif

typedef struct str {
  char *data;
  size_t cap;
  size_t len;
} str;

circa_static circa_err str_alloc(str *const restrict);
circa_static circa_err str_alloc_cap(str *const restrict, register const size_t);
circa_static circa_err str_require(str *const restrict, register const size_t);
circa_static circa_err str_amort(str *const restrict);
circa_static circa_err str_clear(str *const restrict);
circa_static circa_err str_free(str *const restrict);

circa_static circa_err str_set_v(str *const restrict, register const size_t, register const char);
circa_static circa_err str_set_r(str *const restrict, register const size_t, char *const restrict);
circa_static char str_get_v(str *const restrict, register const size_t);
circa_static circa_err str_get_r(str *const restrict, register const size_t, char *const restrict);

circa_static circa_err str_snoc_v(str *const restrict, register const char);
circa_static circa_err str_snoc_r(str *const restrict, char *const restrict);
circa_static char str_pop_v(str *const restrict);
circa_static circa_err str_pop_r(str *const restrict, char *const restrict);

circa_static circa_err str_cpy(str *const restrict, str *const restrict);
circa_static circa_err str_cpy_ur(str *, str *);
circa_static circa_err str_cpy_cstr(str *const restrict, char *restrict);
circa_static circa_err str_cpy_cstr_ur(str *, char *);

circa_static circa_err str_cat(str *const restrict, str *const restrict);
circa_static circa_err str_cat_ur(str *const, str *const);
circa_static circa_err str_cat_cstr(str *const restrict, char *restrict);
circa_static circa_err str_cat_cstr_ur(str *const, char *);
circa_static circa_err str_cat_cstr_ur_len(str *, char *, size_t);

circa_static circa_err str_vfmt(str *const restrict, const char *const restrict, register va_list);
circa_static circa_err str_fmt(str *const restrict, const char *const restrict, ...);
circa_static circa_err str_cat_vfmt(str *const restrict, const char *const restrict, register va_list);
circa_static circa_err str_cat_fmt(str *const restrict, const char *const restrict, ...);

circa_static circa_err str_read(str *const restrict, FILE *const restrict);
circa_static circa_err str_read_name(str *const restrict, char *const restrict);
circa_static circa_err str_cat_read(str *const restrict, FILE *const restrict);
circa_static circa_err str_cat_read_name(str *const restrict, char *const restrict);
circa_static circa_err str_write(str *const restrict, FILE *const restrict);
circa_static circa_err str_write_name(str *const restrict s, char *const restrict);
circa_static circa_err str_write_cat_name(str *const restrict s, char *const restrict);

circa_static cmp str_cmp(str *, str *);
circa_static size_t str_hash_seeded(str *, size_t);
circa_static size_t str_hash(str *);

#undef circa_static

#ifdef CIRCA_STATIC
  #include "circa_str.c"
#endif

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
