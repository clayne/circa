/* Snow, a testing library - https://github.com/mortie/snow */

/*
 * Copyright (c) 2018 Martin Dørum
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SNOW_H
#define SNOW_H

#ifndef SNOW_ENABLED

#define describe(...)

#else

#ifndef __GNUC__
#error "Your compiler doesn't support GNU extensions."
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

#define SNOW_VERSION "1.4.0"

#ifndef SNOW_COLOR_SUCCESS
#define SNOW_COLOR_SUCCESS "\033[32m"
#endif

#ifndef SNOW_COLOR_MAYBE
#define SNOW_COLOR_MAYBE "\033[35m"
#endif

#ifndef SNOW_COLOR_FAIL
#define SNOW_COLOR_FAIL "\033[31m"
#endif

#ifndef SNOW_COLOR_DESC
#define SNOW_COLOR_DESC "\033[1m\033[33m"
#endif

#define _SNOW_COLOR_BOLD    "\033[1m"
#define _SNOW_COLOR_RESET   "\033[0m"

// Compatibility with MinGW
#ifdef __MINGW32__
#  pragma GCC diagnostic ignored "-Wint-conversion"
#  include <io.h>
#  ifdef __WIN64
#    define _SNOW_PRIuSIZE PRIu64
#  else
#    define _SNOW_PRIuSIZE PRIu32
#  endif
// cmd.exe is identified as a tty but doesn't understand escape sequences,
// git bash understand escape sequences but isn't identified as a TTY
// by _isatty(_fileno(stdout))
#  define _SNOW_ISATTY(file) 0
#else
#  define _SNOW_PRIuSIZE "zu"
#  define _SNOW_ISATTY(file) isatty(fileno(file))
// For stdio.h to define fileno, _POSIX_C_SOURCE or similar has to be defined
// before stdio.h is included. I want Snow to work without any other compiler
// flags than -DSNOW_ENABLED.
int fileno(FILE *stream);
#endif

// Undef the <assert.h> assert macro, because we're defining our own
#ifdef assert
#undef assert
#endif

extern FILE *_snow_log_file;

extern int _snow_exit_code;
extern int _snow_extra_newline;
extern int _snow_global_total;
extern int _snow_global_successes;
extern int _snow_num_defines;
extern double _snow_time_total;

struct _snow_labels {
	void **labels;
	size_t size;
	size_t count;
};
extern struct _snow_labels _snow_defer_labels;

struct _snow_describe {
	void (*func)();
	char *name;
};
extern struct _snow_describe _snow_describe;

struct _snow_describes {
	struct _snow_describe *describes;
	size_t size;
	size_t count;
};
extern struct _snow_describes _snow_describes;

typedef enum
{
	_snow_opt_version,
	_snow_opt_help,
	_snow_opt_color,
	_snow_opt_quiet,
	_snow_opt_maybes,
	_snow_opt_cr,
	_snow_opt_timer,
} _snow_opt_names;

struct _snow_option
{
	char *fullname;
	char shortname;
	int value;
	int overridden;
};
extern struct _snow_option _snow_opts[];

#define _snow_time2ms(t) ((t).tv_sec * 1000.0 + (t).tv_usec / 1000.0)

#define _snow_print(...) \
	do { \
		fprintf(_snow_log_file, __VA_ARGS__); \
		fflush(_snow_log_file); \
	} while (0)

#define _snow_print_timer(timediff) \
	do { \
		if (_snow_opts[_snow_opt_timer].value) { \
			if ((timediff) < 1000) \
				_snow_print(" (%.2fms)\n", (timediff)); \
			else \
				_snow_print(" (%.2fs)\n", (timediff) / 1000); \
		} else if (_snow_opts[_snow_opt_maybes].value) { \
			/* We need a space if we have maybes, to remove */ \
			/* the colon it adds to make noisy tests look good */ \
			_snow_print(" \n"); \
		} else { \
			_snow_print("\n"); \
		} \
	} while (0)

#define _snow_print_result_newline() \
	do { \
		if (_snow_opts[_snow_opt_maybes].value) {\
			if (_snow_opts[_snow_opt_cr].value) \
				_snow_print("\r"); \
			else \
				_snow_print("\n"); \
		} \
	} while (0)

#define _snow_fail(desc, spaces, name, file, ...) \
	do { \
		_snow_print_result_newline(); \
		_snow_extra_newline = 1; \
		_snow_exit_code = EXIT_FAILURE; \
		if (_snow_opts[_snow_opt_color].value) { \
			_snow_print( \
				_SNOW_COLOR_BOLD SNOW_COLOR_FAIL "%s✕ " \
				_SNOW_COLOR_RESET SNOW_COLOR_FAIL "Failed:  " \
				_SNOW_COLOR_RESET SNOW_COLOR_DESC "%s" \
				_SNOW_COLOR_RESET ":", \
				spaces, desc); \
		} else { \
			_snow_print("%s✕ Failed:  %s:", spaces, desc); \
		} \
		_snow_print("\n"); \
		_snow_print("%s    ", spaces); \
		_snow_print(__VA_ARGS__); \
		_snow_print("\n%s    in %s:%s\n", spaces, file, name); \
	} while (0)

#define _snow_array_append(arr, count, size, val) \
	do { \
		count += 1; \
		/* Realloc labels array if necessary */ \
		if (count >= size) { \
			if (size == 0) \
				size = 16; \
			else \
				size *= 2; \
			arr = realloc(arr, size * sizeof(*arr)); \
		} \
		/* Add pointer to label to labels array */ \
		arr[count - 1] = val; \
	} while (0)

#define fail(...) \
	do { \
		_snow_fail(_snow_desc, _snow_spaces, _snow_name, __FILE__, __VA_ARGS__); \
		goto _snow_done; \
	} while (0)

#define assert(x) \
	do { \
		if (!(x)) { \
			fail("Assertion failed: " #x); \
		} \
	} while (0)

#define _snow_decl_asserteq(suffix, type, fmt) \
	static int __attribute__((unused)) _snow_asserteq_##suffix( \
			const char *desc, const char *spaces, const char *name, const char *file, \
			const char *astr, const char *bstr, type a, type b) \
	{ \
		if (a != b) { \
			_snow_fail( \
				desc, spaces, name, file, \
				"Expected %s to equal %s, but got " fmt, \
				astr, bstr, a); \
			return -1; \
		} \
		return 0; \
	}
#define _snow_decl_assertneq(suffix, type) \
	static int __attribute__((unused)) _snow_assertneq_##suffix( \
			const char *desc, const char *spaces, const char *name, const char *file, \
			const char *astr, const char *bstr, type a, type b) \
	{ \
		if (a == b) { \
			_snow_fail( \
				desc, spaces, name, file, \
				"Expected %s to not equal %s", \
				astr, bstr); \
			return -1; \
		} \
		return 0; \
	}

_snow_decl_asserteq(int, intmax_t, "%" PRIdMAX)
_snow_decl_assertneq(int, intmax_t)
#define asserteq_int(a, b) \
	do { \
		if (_snow_asserteq_int(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)
#define assertneq_int(a, b) \
	do { \
		if (_snow_assertneq_int(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)

_snow_decl_asserteq(ptr, void *, "%p")
_snow_decl_assertneq(ptr, void *)
#define asserteq_ptr(a, b) \
	do { \
		if (_snow_asserteq_ptr(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)
#define assertneq_ptr(a, b) \
	do { \
		if (_snow_assertneq_ptr(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)

_snow_decl_asserteq(dbl, double, "%f");
_snow_decl_assertneq(dbl, double);
#define asserteq_dbl(a, b) \
	do { \
		if (_snow_asserteq_dbl(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)
#define assertneq_dbl(a, b) \
	do { \
		if (_snow_assertneq_dbl(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)

static int __attribute__((unused)) _snow_asserteq_str(
		const char *desc, const char *spaces, const char *name, const char *file,
		const char *astr, const char *bstr, const char *a, const char *b)
{
	if (strcmp(a, b) != 0)
	{
		_snow_fail(
			desc, spaces, name, file,
			"Expected %s to equal %s, but got \"%s\"",
			astr, bstr, a);
		return -1;
	}
	return 0;
}
static int __attribute__((unused)) _snow_assertneq_str(
		const char *desc, const char *spaces, const char *name, const char *file,
		const char *astr, const char *bstr, const char *a, const char *b)
{
	if (strcmp(a, b) == 0)
	{
		_snow_fail(
			desc, spaces, name, file,
			"Expected %s to not equal %s",
			astr, bstr);
		return -1;
	}
	return 0;
}
#define asserteq_str(a, b) \
	do { \
		if (_snow_asserteq_str(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)
#define assertneq_str(a, b) \
	do { \
		if (_snow_assertneq_str(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b)) < 0) \
			goto _snow_done; \
	} while (0)

static int __attribute__((unused)) _snow_asserteq_buf(
		const char *desc, const char *spaces, const char *name, const char *file,
		const char *astr, const char *bstr, const void *a, const void *b, size_t n)
{
	const char *_a = (const char *)a;
	const char *_b = (const char *)b;
	size_t i;
	for (i = 0; i < n; ++i)
	{
		if (_a[i] != _b[i])
		{
			_snow_fail(
				desc, spaces, name, file,
				"Expected %s to equal %s, but they differ at byte %" _SNOW_PRIuSIZE,
				astr, bstr, i);
			return -1;
		}
	}
	return 0;
}
static int __attribute__((unused)) _snow_assertneq_buf(
		const char *desc, const char *spaces, const char *name, const char *file,
		const char *astr, const char *bstr, const void *a, const void *b, size_t n)
{
	const char *_a = (const char *)a;
	const char *_b = (const char *)b;
	size_t i;
	for (i = 0; i < n; ++i)
	{
		if (_a[i] != _b[i])
			return 0;
	}
	_snow_fail(
		desc, spaces, name, file,
		"Expected %s to not equal %s",
		astr, bstr);
	return -1;
}
#define asserteq_buf(a, b, n) \
	do { \
		if (_snow_asserteq_buf(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b), (n)) < 0) \
			goto _snow_done; \
	} while (0)
#define assertneq_buf(a, b, n) \
	do { \
		if (_snow_assertneq_buf(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, (a), (b), (n)) < 0) \
			goto _snow_done; \
	} while (0)

#if(__STDC_VERSION__ >= 201112L)

#define asserteq(a, b) \
	do { \
		_Pragma("GCC diagnostic push") \
		_Pragma("GCC diagnostic ignored \"-Wpragmas\"") \
		_Pragma("GCC diagnostic ignored \"-Wint-conversion\"") \
		int r = _Generic((b), \
			char *: _snow_asserteq_str, \
			const char *: _snow_asserteq_str, \
			float: _snow_asserteq_dbl, \
			double: _snow_asserteq_dbl, \
			default: _Generic((b) - (b), \
				int: _snow_asserteq_int, \
				default: _Generic((b) - (b), \
					ptrdiff_t: _snow_asserteq_ptr, \
					default: _snow_asserteq_int)) \
		)(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, a, b); \
		_Pragma("GCC diagnostic pop") \
		if (r < 0) \
			goto _snow_done; \
	} while (0)
#define assertneq(a, b) \
	do { \
		_Pragma("GCC diagnostic push") \
		_Pragma("GCC diagnostic ignored \"-Wpragmas\"") \
		_Pragma("GCC diagnostic ignored \"-Wint-conversion\"") \
		int r = _Generic((b), \
			char *: _snow_assertneq_str, \
			const char *: _snow_assertneq_str, \
			float: _snow_assertneq_dbl, \
			double: _snow_assertneq_dbl, \
			default: _Generic((b) - (b), \
				int: _snow_assertneq_int, \
				default: _Generic((b) - (b), \
					ptrdiff_t: _snow_assertneq_ptr, \
					default: _snow_assertneq_int)) \
		)(_snow_desc, _snow_spaces, _snow_name, __FILE__, #a, #b, a, b); \
		_Pragma("GCC diagnostic pop") \
		if (r < 0) \
			goto _snow_done; \
	} while (0)

#else // __STDC_VERSION__ >= 201112L

#define asserteq(a, b) _Pragma("GCC error \"asserteq requires support for C11.\"")
#define assertneq(a, b) _Pragma("GCC error \"assertneq requires support for C11.\"")

#endif

#define _snow_print_success(timediff) \
	do { \
		_snow_print_result_newline(); \
		if (_snow_opts[_snow_opt_quiet].value) break; \
		_snow_extra_newline = 1; \
		if (_snow_opts[_snow_opt_color].value) { \
			_snow_print( \
				_SNOW_COLOR_BOLD SNOW_COLOR_SUCCESS "%s✓ " \
				_SNOW_COLOR_RESET SNOW_COLOR_SUCCESS "Success: " \
				_SNOW_COLOR_RESET SNOW_COLOR_DESC "%s" \
				_SNOW_COLOR_RESET, \
				_snow_spaces, _snow_desc); \
		} else { \
			_snow_print( \
				"%s✓ Success: %s", \
				_snow_spaces, _snow_desc); \
		} \
		_snow_print_timer(timediff); \
	} while (0)

#define _snow_print_maybe() \
	do { \
		if (_snow_opts[_snow_opt_quiet].value) break; \
		if (!_snow_opts[_snow_opt_maybes].value) break; \
		_snow_extra_newline = 1; \
		if (_snow_opts[_snow_opt_color].value) { \
			_snow_print( \
				_SNOW_COLOR_BOLD SNOW_COLOR_MAYBE "%s? " \
				_SNOW_COLOR_RESET SNOW_COLOR_MAYBE "Testing: " \
				_SNOW_COLOR_RESET SNOW_COLOR_DESC "%s: " \
				_SNOW_COLOR_RESET, \
				_snow_spaces, _snow_desc); \
		} else { \
			_snow_print( \
				"%s? Testing: %s: ", \
				_snow_spaces, _snow_desc); \
		} \
	} while (0)

#define _snow_print_run() \
	do { \
		if (_snow_opts[_snow_opt_quiet].value) break; \
		if (_snow_extra_newline) { \
			_snow_print("\n"); \
		} \
		if (_snow_opts[_snow_opt_color].value) { \
			_snow_print( \
				_SNOW_COLOR_BOLD "%sTesting %s:" _SNOW_COLOR_RESET "\n", \
				_snow_spaces, _snow_name); \
		} else { \
			_snow_print( \
				"%sTesting %s:\n", \
				_snow_spaces, _snow_name); \
		} \
	} while (0)

#define _snow_print_done() \
	do { \
		if (_snow_opts[_snow_opt_quiet].value) break; \
		_snow_extra_newline = 0; \
		if (_snow_opts[_snow_opt_color].value) { \
			_snow_print( \
				_SNOW_COLOR_BOLD "%s%s: Passed %i/%i tests." \
				_SNOW_COLOR_RESET, \
				_snow_spaces, _snow_name, _snow_successes, _snow_total); \
			_snow_print_timer(_snow_time_total); \
			_snow_print("\n"); \
		} else { \
			_snow_print( \
				"%s%s: Passed %i/%i tests.", \
				_snow_spaces, _snow_name, _snow_successes, _snow_total); \
			_snow_print_timer(_snow_time_total); \
			_snow_print("\n"); \
		} \
	} while (0)

#define defer(...) \
	do { \
		__label__ _snow_defer_label; \
		_snow_defer_label: \
		if (_snow_rundefer) { \
			__VA_ARGS__; \
			/* Go to the previous defer, or the end of the `it` block */ \
			if (_snow_defer_labels.count > 0) \
				goto *_snow_defer_labels.labels[--_snow_defer_labels.count]; \
			else \
				goto _snow_done; \
		} else { \
			_snow_array_append( \
				_snow_defer_labels.labels, \
				_snow_defer_labels.count, \
				_snow_defer_labels.size, \
				&&_snow_defer_label); \
		} \
	} while (0)

#define before_each(...) \
	do { \
		__label__ _snow_around_label; \
		_snow_around_label: \
		if (_snow_runaround) { \
			__VA_ARGS__ \
			_snow_runaround = 0; \
			goto *_snow_around_return; \
		} else { \
			_snow_before_labels.labels[_snow_before_labels.count - 1] = &&_snow_around_label; \
		} \
	} while (0)

#define after_each(...) \
	do { \
		__label__ _snow_around_label; \
		_snow_around_label: \
		if (_snow_runaround) { \
			__VA_ARGS__ \
			_snow_runaround = 0; \
			goto *_snow_around_return; \
		} else { \
			_snow_after_labels.labels[_snow_after_labels.count - 1] = &&_snow_around_label; \
		} \
	} while (0)

#define test(testdesc, ...) \
	do { \
		__label__ _snow_done, _snow_begin_done, _snow_after_done; \
		/* This is to make Clang shut up about "indirect goto in function */ \
		/* with no address-of-label expressions" when there's no defer(). */ \
		__attribute__((unused)) void *_snow_unused_label = &&_snow_done; \
		int __attribute__((unused)) _snow_rundefer = 0; \
		const char *_snow_desc = testdesc; \
		struct timeval _snow_timer_pre, _snow_timer_post; \
		double timediff = 0; \
		_snow_total += 1; \
		for (int i = _snow_before_labels.count - 1; i >= 0; --i) { \
			if (_snow_before_labels.labels[i] != NULL) { \
				_snow_runaround = 1; \
				_snow_around_return = &&_snow_begin_done; \
				goto *_snow_before_labels.labels[i]; \
			} \
		} \
		_snow_begin_done: \
		_snow_print_maybe(); \
		if (_snow_opts[_snow_opt_timer].value) \
			gettimeofday(&_snow_timer_pre, NULL); \
		__VA_ARGS__ \
		if (_snow_opts[_snow_opt_timer].value) { \
			gettimeofday(&_snow_timer_post, NULL); \
			double pre_ms = _snow_time2ms(_snow_timer_pre); \
			double post_ms = _snow_time2ms(_snow_timer_post); \
			timediff = post_ms - pre_ms; \
			_snow_time_total += timediff; \
		} \
		_snow_successes += 1; \
		_snow_print_success(timediff); \
		_snow_done: \
		__attribute__((unused)); \
		_snow_rundefer = 1; \
		if (_snow_defer_labels.count > 0) { \
			_snow_defer_labels.count -= 1; \
			goto *_snow_defer_labels.labels[_snow_defer_labels.count]; \
		} \
		for (int i = _snow_after_labels.count - 1; i >= 0; --i) { \
			if (_snow_after_labels.labels[i] != NULL) { \
				_snow_runaround = 1; \
				_snow_around_return = &&_snow_after_done; \
				goto *_snow_after_labels.labels[i]; \
			} \
		} \
		_snow_after_done: \
		; \
	} while (0)
#define it test

#define _snow_desc(...) \
	int _snow_successes = 0; \
	int _snow_total = 0; \
	double *_snow_parent_time_total = &_snow_time_total; \
	double _snow_time_total = 0; \
	_snow_array_append( \
		_snow_before_labels.labels, \
		_snow_before_labels.count, \
		_snow_before_labels.size, \
		NULL); \
	_snow_array_append( \
		_snow_after_labels.labels, \
		_snow_after_labels.count, \
		_snow_after_labels.size, \
		NULL); \
	_snow_print_run(); \
	__VA_ARGS__ \
	_snow_print_done(); \
	*_snow_parent_time_total += _snow_time_total

#define subdesc(testname, ...) \
	do { \
		char *_snow_name = #testname; \
		int *_snow_parent_total = &_snow_total; \
		int *_snow_parent_successes = &_snow_successes; \
		int _snow_parent_depth = _snow_depth; \
		int __attribute__((unused)) _snow_depth = _snow_parent_depth + 1; \
		/* Malloc because Clang doesn't like using a variable length
		 * stack allocated array here, because dynamic gotos */ \
		char *_snow_spaces = (char*)malloc(_snow_depth * 2 + 1); \
		int i; \
		for (i = 0; i < _snow_depth * 2; ++i) \
			_snow_spaces[i] = ' '; \
		_snow_spaces[_snow_depth * 2] = '\0'; \
		\
		_snow_desc(__VA_ARGS__); \
		\
		free(_snow_spaces); \
		_snow_before_labels.count--; \
		_snow_after_labels.count--; \
		*_snow_parent_successes += _snow_successes; \
		*_snow_parent_total += _snow_total; \
	} while (0)

#define describe(testname, ...) \
	static void test_##testname() { \
		_snow_num_defines += 1; \
		const char *_snow_name = #testname; \
		int __attribute__((unused)) _snow_depth = 0; \
		int  __attribute__((unused)) _snow_runaround = 0; \
		void __attribute__((unused)) *_snow_around_return = NULL; \
		struct _snow_labels _snow_before_labels = {NULL, 0, 0}; \
		struct _snow_labels _snow_after_labels = {NULL, 0, 0}; \
		const char *_snow_spaces = ""; \
		\
		_snow_desc(__VA_ARGS__); \
		\
		if (_snow_before_labels.labels != NULL) \
			free(_snow_before_labels.labels); \
		if (_snow_after_labels.labels != NULL) \
			free(_snow_after_labels.labels); \
		_snow_global_successes += _snow_successes; \
		_snow_global_total += _snow_total; \
	} \
	__attribute__((constructor (__COUNTER__ + 101))) \
	static void _snow_constructor_##testname() { \
		struct _snow_describe describe = { \
			.func = &test_##testname, \
			.name = #testname, \
		}; \
		_snow_array_append( \
			_snow_describes.describes, \
			_snow_describes.count, \
			_snow_describes.size, \
			describe); \
	}

#define _snow_parse_args(argc, argv, ...) \
	do { \
		int i; \
		int done = 0; \
		for (i = 1; i < argc; ++i) { \
			if (argv[i][0] != '-' || done) { \
				__VA_ARGS__ \
				continue; \
			} \
			/* Stop parsing arguments if we encounter -- */ \
			if (strcmp(argv[i], "--") == 0) { \
				done = 1; \
				continue; \
			} \
			int j; \
			int len = sizeof(_snow_opts)/sizeof(*_snow_opts); \
			/* Disable options with --no-* */ \
			if (strncmp(argv[i], "--no-", 5) == 0) { \
				for (j = 0; j < len; ++j) { \
					struct _snow_option *opt = &_snow_opts[j]; \
					if (strcmp(&argv[i][5], opt->fullname) == 0) { \
						opt->value = 0; \
						opt->overridden = 1; \
						break; \
					} \
				} \
			/* Enable long options with --* */ \
			} else if (strncmp(argv[i], "--", 2) == 0) { \
				for (j = 0; j < len; ++j) { \
					struct _snow_option *opt = &_snow_opts[j]; \
					if (strcmp(&argv[i][2], opt->fullname) == 0) { \
						opt->value = 1; \
						opt->overridden = 1; \
						break; \
					} \
				} \
			/* Enable short options with -* */ \
			} else if (strncmp(argv[i], "-", 1) == 0 && strlen(argv[i]) == 2) { \
				for (j = 0; j < len; ++j) { \
					struct _snow_option *opt = &_snow_opts[j]; \
					if (argv[i][1] == opt->shortname) { \
						opt->value = 1; \
						opt->overridden = 1; \
					} \
				} \
			} \
			/* --log */ \
			if (strcmp(argv[i], "--log") == 0) { \
				if (++i >= argc) break; \
				if (strcmp(argv[i], "-") == 0) \
					_snow_log_file = stdout; \
				else \
					_snow_log_file = fopen(argv[i], "w"); \
				if (_snow_log_file == NULL) { \
					_snow_log_file = stdout; \
					_snow_print( \
						"Could not open log file '%s': %s", \
						argv[i], strerror(errno)); \
					return -1; \
				} \
			} \
		} \
	} while (0)

#define _snow_usage(argv0) \
	do { \
		_snow_print("Usage: %s [options]            Run all tests.\n", argv0); \
		_snow_print("       %s [options] <test>...  Run specific tests.\n", argv0); \
		_snow_print("       %s -v|--version         Print version and exit.\n", argv0); \
		_snow_print("       %s -h|--help            Display this help text and exit.\n", argv0); \
		_snow_print( \
			"\n" \
			"Arguments:\n" \
			"    --color|-c:   Enable colors.\n" \
			"                  Default: on when output is a TTY.\n" \
			"    --no-color:   Force disable --color.\n" \
			"\n" \
			"    --quiet|-q:   Suppress most messages, only test failures and a summary\n" \
			"                  error count is shown.\n" \
			"                  Default: off.\n" \
			"    --no-quiet:   Force disable --quiet.\n" \
			"\n" \
			"    --log <file>: Log output to a file, rather than stdout.\n" \
			"\n" \
			"    --timer|-t:   Display the time taken for by each test after\n" \
			"                  it is completed.\n" \
			"                  Default: on.\n" \
			"    --no-timer:   Force disable --timer.\n" \
			"\n" \
			"    --maybes|-m:  Print out messages when begining a test as well\n" \
			"                  as when it is completed.\n" \
			"                  Default: on when the output is a TTY.\n" \
			"    --no-maybes:  Force disable --maybes.\n" \
			"\n" \
			"    --cr:         Print a carriage return (\\r) rather than a newline\n" \
			"                  after each --maybes message. This means that the fail or\n" \
			"                  success message will appear on the same line.\n" \
			"                  Default: on when the output is a TTY.\n" \
			"    --no-cr:      Force disable --cr.\n"); \
	} while (0)

#define snow_main() \
	int _snow_exit_code = EXIT_SUCCESS; \
	int _snow_extra_newline = 1; \
	int _snow_global_total = 0; \
	int _snow_global_successes = 0; \
	int _snow_num_defines = 0; \
	double _snow_time_total = 0; \
	int _snow_specific_tests_count = 0; \
	int _snow_specific_tests_size = 0; \
	void (**_snow_specific_tests)() = NULL; \
	FILE *_snow_log_file; \
	struct _snow_labels _snow_defer_labels = { NULL, 0, 0 }; \
	struct _snow_describes _snow_describes = { NULL, 0, 0 }; \
	struct _snow_option _snow_opts[] = { \
		[_snow_opt_version] = { "version", 'v',  0, 0 }, \
		[_snow_opt_help]    = { "help",    'h',  0, 0 }, \
		[_snow_opt_color]   = { "color",   'c',  1, 0 }, \
		[_snow_opt_quiet]   = { "quiet",   'q',  0, 0 }, \
		[_snow_opt_maybes]  = { "maybes",  'm',  1, 0 }, \
		[_snow_opt_cr]      = { "cr",      '\0', 1, 0 }, \
		[_snow_opt_timer]   = { "timer",   't',  1, 0 }, \
	}; \
	int main(int argc, char **argv) { \
		_snow_log_file = stdout; \
		/* Parse options, and add non-option arguments to _snow_specific_tests */ \
		_snow_parse_args(argc, argv, { \
			size_t j; \
			int found = 0; \
			for (j = 0; j < _snow_describes.count; ++j) {\
				if (strcmp(argv[i], _snow_describes.describes[j].name) == 0) { \
					found = 1; \
					_snow_array_append( \
						_snow_specific_tests, \
						_snow_specific_tests_count, \
						_snow_specific_tests_size, \
						_snow_describes.describes[j].func); \
					break; \
				} \
			} \
			if (!found) { \
				fprintf(stderr, "Unknown test: %s\n", argv[i]); \
				return EXIT_FAILURE; \
			} \
		}); \
		/* Default to no colors if NO_COLOR */ \
		if (getenv("NO_COLOR") != NULL) { \
			if (!_snow_opts[_snow_opt_color].overridden) \
				_snow_opts[_snow_opt_color].value = 0; \
		} \
		/* If not a tty, default to "boring" output */ \
		if (!_SNOW_ISATTY(_snow_log_file)) { \
			if (!_snow_opts[_snow_opt_color].overridden) \
				_snow_opts[_snow_opt_color].value = 0; \
			if (!_snow_opts[_snow_opt_maybes].overridden) \
				_snow_opts[_snow_opt_maybes].value = 0; \
			if (!_snow_opts[_snow_opt_cr].overridden) \
				_snow_opts[_snow_opt_cr].value = 0; \
		} \
		/* --version: Print version and exit */ \
		if (_snow_opts[_snow_opt_version].value) { \
			_snow_print("Snow %s\n", SNOW_VERSION); \
			return EXIT_SUCCESS; \
		} \
		/* --help: Print usage and exit */ \
		if (_snow_opts[_snow_opt_help].value) { \
			_snow_usage(argv[0]); \
			return EXIT_SUCCESS; \
		} \
		if (_snow_specific_tests != NULL) { \
			/* Run specific tests */\
			int j; \
			for (j = 0; j < _snow_specific_tests_count; ++j) { \
				_snow_specific_tests[j](); \
			} \
		} else { \
			/* Run tests */ \
			size_t j; \
			for (j = 0; j < _snow_describes.count; ++j) { \
				_snow_describes.describes[j].func(); \
			} \
		} \
		/* Cleanup, print result */ \
		free(_snow_defer_labels.labels); \
		free(_snow_describes.describes); \
		if (_snow_specific_tests != NULL) \
			free(_snow_specific_tests); \
		if (_snow_num_defines > 1 || _snow_opts[_snow_opt_quiet].value) { \
			if (_snow_opts[_snow_opt_color].value) { \
				_snow_print( \
					_SNOW_COLOR_BOLD "Total: Passed %i/%i tests." \
					_SNOW_COLOR_RESET, \
					_snow_global_successes, _snow_global_total); \
			} else { \
				_snow_print( \
					"Total: Passed %i/%i tests.", \
					_snow_global_successes, _snow_global_total); \
			} \
			_snow_print_timer(_snow_time_total); \
			if (!_snow_opts[_snow_opt_quiet].value) \
				_snow_print("\n"); \
		} \
		return _snow_exit_code; \
	}

#endif // SNOW_ENABLED

#endif // SNOW_H
