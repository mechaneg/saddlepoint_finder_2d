#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

//file /src/common_defines.h

#define FIX_UNUSED(x) (void)((x))

#define FREE(x)    \
  do               \
  {                \
    if ((x) != 0)  \
     delete (x);   \
  }while(0)        \

#define FREE_ARR(x)  \
  do                 \
  {                  \
    if ((x))         \
      delete[] (x);  \
  }while (0)         \

inline int sign (double x)
{
  return (x > 0.) - (x < 0.);
}

#define CHAR_BUF_LEN 4096

#define MIN_FOR_COMPARE 1.e-12

#define MAX_OM_POLY_DEG 6
#define MAX_K_POLY_DEG 10

#define IM_OMEGA_MIN 1.e-10
#define ROOT_SELECT_EPS 1.e-14

#endif // COMMON_DEFINES_H

