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


#define CHAR_BUF_LEN 4096

#define MIN_FOR_COMPARE 1.e-12

#define OM_POLY_DEG 6
#define  K_POLY_DEG 10

#endif // COMMON_DEFINES_H

