#include "version.hpp"

resource 'PiPL' (16000) {
{
        /* [1] */
        Kind {
            AEGP
        },
        /* [2] */
        Name {
            "QRONSOFT_AEGIF"
        },
        /* [3] */
        Category {
            "Qronsoft"
        },
        /* [4] */
        Version {
            AEGIF_MAJOR_VERSION * 524288 +
            AEGIF_MINOR_VERSION * 32768 +
            AEGIF_BUG_VERSION   * 2048 +
            AEGIF_STAGE_VERSION * 512 +
            AEGIF_BUILD_VERSION
        },
        /* [5] */
        CodeWin64X86 {"EntryPointFunc"},
}
};
