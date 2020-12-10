typedef enum {
  CIRCA_ERR_NONE,
  CIRCA_ERR_READ,
  CIRCA_ERR_WRITE,
  CIRCA_ERR_LENGTH
} circa_err;

static const char *circa_err_explain[CIRCA_ERR_LENGTH] = {
  [CIRCA_ERR_NONE] = "No error.",
  [CIRCA_ERR_READ] = "Failed to read a file.",
  [CIRCA_ERR_WRITE] = "Failed to write to a file."
};

