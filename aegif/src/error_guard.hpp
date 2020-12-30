#pragma once

// AE
#include <A.h>

// aegif
#include "log.hpp"

#define GUARD_A_Err(expr)                                                                                    \
    do                                                                                                       \
    {                                                                                                        \
        if (A_Err err = (expr))                                                                              \
        {                                                                                                    \
            AEGLOG_ERROR("A_Err occured: {}", err);                                                          \
            return err;                                                                                      \
        }                                                                                                    \
    } while (0);

#define GUARD_ERROR(expr, retval)                                                                            \
    do                                                                                                       \
    {                                                                                                        \
        if (!(expr))                                                                                         \
        {                                                                                                    \
            AEGLOG_ERROR("error occured");                                                                   \
            return retval;                                                                                   \
        }                                                                                                    \
    } while (0);
