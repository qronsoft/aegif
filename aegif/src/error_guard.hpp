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
            AEGLOG_ERROR("A_Err occured");                                                                   \
            return err;                                                                                      \
        }                                                                                                    \
    } while (0);
