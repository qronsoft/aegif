﻿#pragma once

#define AEGIF_MAJOR_VERSION 0 /* 0-7 */
#define AEGIF_MINOR_VERSION 0 /* 0-15 */
#define AEGIF_BUG_VERSION 1   /* 0-15 */
#define AEGIF_STAGE_VERSION 3 /* develop: 0, alpha: 1, beta: 2, rc: 3 */
#define AEGIF_BUILD_VERSION 0 /* 0-511 */

#define _AEGIF_VERSION_STR2(x) #x
#define _AEGIF_VERSION_STR(x) _AEGIF_VERSION_STR2(x)
#define AEGIF_SEM_VERSION                 \
    _AEGIF_VERSION_STR(AEGIF_MAJOR_VERSION) \
    "." _AEGIF_VERSION_STR(AEGIF_MINOR_VERSION) "." _AEGIF_VERSION_STR(AEGIF_BUG_VERSION)
