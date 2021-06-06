#pragma once
#include <map>

static std::map<std::string, int> mapGetParam = {
    {"I915_PARAM_IRQ_ACTIVE           ", 1 },
    {"I915_PARAM_ALLOW_BATCHBUFFER    ", 2 },
    {"I915_PARAM_LAST_DISPATCH        ", 3 },
    {"I915_PARAM_CHIPSET_ID           ", 4 },
    {"I915_PARAM_HAS_GEM              ", 5 },
    {"I915_PARAM_NUM_FENCES_AVAIL     ", 6 },
    {"I915_PARAM_HAS_OVERLAY          ", 7 },
    {"I915_PARAM_HAS_PAGEFLIPPING     ", 8 },
    {"I915_PARAM_HAS_EXECBUF2         ", 9 },
    {"I915_PARAM_HAS_BSD              ", 10},
    {"I915_PARAM_HAS_BLT              ", 11},
    {"I915_PARAM_HAS_RELAXED_FENCING  ", 12},
    {"I915_PARAM_HAS_COHERENT_RINGS   ", 13},
    {"I915_PARAM_HAS_EXEC_CONSTANTS   ", 14},
    {"I915_PARAM_HAS_RELAXED_DELTA    ", 15},
    {"I915_PARAM_HAS_GEN7_SOL_RESET   ", 16},
    {"I915_PARAM_HAS_LLC              ", 17},
    {"I915_PARAM_HAS_ALIASING_PPGTT   ", 18},
    {"I915_PARAM_HAS_WAIT_TIMEOUT     ", 19},
    {"I915_PARAM_HAS_SEMAPHORES       ", 20},
    {"I915_PARAM_HAS_PRIME_VMAP_FLUSH ", 21},
    {"I915_PARAM_HAS_VEBOX            ", 22},
    {"I915_PARAM_HAS_SECURE_BATCHES   ", 23},
    {"I915_PARAM_HAS_PINNED_BATCHES   ", 24},
    {"I915_PARAM_HAS_EXEC_NO_RELOC    ", 25},
    {"I915_PARAM_HAS_EXEC_HANDLE_LUT  ", 26},
    {"I915_PARAM_HAS_WT               ", 27},
    {"I915_PARAM_CMD_PARSER_VERSION   ", 28},
    {"I915_PARAM_HAS_COHERENT_PHYS_GTT", 29},
    {"I915_PARAM_MMAP_VERSION         ", 30},
    {"I915_PARAM_HAS_BSD2             ", 31},
    {"I915_PARAM_REVISION             ", 32},
    {"I915_PARAM_SUBSLICE_TOTAL       ", 33},
    {"I915_PARAM_EU_TOTAL             ", 34},
    {"I915_PARAM_HAS_GPU_RESET        ", 35},
    {"I915_PARAM_HAS_RESOURCE_STREAMER", 36},
    {"I915_PARAM_HAS_EXEC_SOFTPIN     ", 37},
    {"I915_PARAM_HAS_POOLED_EU        ", 38},
    {"I915_PARAM_MIN_EU_IN_POOL       ", 39},
    {"I915_PARAM_MMAP_GTT_VERSION     ", 40},
};

static std::map<std::string, int> mapContextParam = {
    {"I915_CONTEXT_PARAM_BAN_PERIOD        ", 0x1}, 
    {"I915_CONTEXT_PARAM_NO_ZEROMAP        ", 0x2}, 
    {"I915_CONTEXT_PARAM_GTT_SIZE          ", 0x3}, 
    {"I915_CONTEXT_PARAM_NO_ERROR_CAPTURE  ", 0x4}, 
    {"I915_CONTEXT_PARAM_BANNABLE          ", 0x5}, 
    {"I915_CONTEXT_PARAM_PRIORITY          ", 0x6}, 
    {"I915_CONTEXT_PARAM_SSEU              ", 0x7}, 
    {"I915_CONTEXT_PARAM_RECOVERABLE       ", 0x8}, 
};

static std::map<const char*, int32_t> engine_class_map = 
{
    {"RENDER",   0}, 
    {"BLT",      1}, 
    {"VCS",      2}, 
    {"VECS",     3}, 
    {"INVALID", -1}, 
};
