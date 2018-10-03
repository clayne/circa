# Circa

## Dictionaries

### Accessors

#### dict

```C
struct dict_data *dict(Dict);
struct dict_data *dict_(Dict d, circa_msg fname, circa_msg line);
```

Access the core structure of a dictionary.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
printf("%zu\n", dict(d)->len); // 1
```

#### dict_set

```C
void dict_set(Dict, char*, <value>);
void dict_set_iso(<type>, Dict, char*, <value>);
Dict dict_set_(size_t siz, Dict d, char *a, void *v, circa_msg fname, circa_msg line);
```

Sets a dictionary's value at a given index.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
printf("%i\n", dict_get(d, "foo")); // 500
```

#### dict_get

```C
<value> dict_get(Dict, char*);
<value> dict_get_iso(<type>, Dict, char*);
void *dict_get_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line);
```

Gets a dictionary's value at a given index.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
printf("%i\n", dict_get(d, "foo")); // 500
```

#### dict_has

```C
bool dict_has(Dict, char*);
bool dict_has_iso(<type>, Dict, char*);
bool dict_has_(size_t siz, Dict d, char *a, circa_msg fname, circa_msg line);
```

Checks if a dictionary has a value a given index.

```C
Dict(int) d = dict_new(int, 10);
dict_set(d, "foo", 500);
if (dict_has(d, "foo"))
  puts("d has foo");
```
