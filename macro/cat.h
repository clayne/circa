#define C2_(A, B) A##_##B
#define C2(A, B) C2_(A, B)

#define C3_(A, B, C) A##_##B##_##C
#define C3(A, B, C) C3_(A, B, C)

#define C4_(A, B, C, D) A##_##B##_##C##_##D
#define C4(A, B, C, D) C4_(A, B, C, D)
