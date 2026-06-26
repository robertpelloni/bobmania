#ifndef GLOBAL_H
#define GLOBAL_H

// This is a stub global.h for compilation checks in this restricted environment
// The full StepMania global.h contains many dependencies not checked in here.

#include <string>

// Mock RString for compilation checks
typedef std::string RString;

// Mock Checkpoints
namespace Checkpoints {
    inline void SetCheckpoint(const char *file, int line, const char* message) {}
}
/** @brief Set a checkpoint with no message. */
#define CHECKPOINT (Checkpoints::SetCheckpoint(__FILE__, __LINE__, ""))
/** @brief Set a checkpoint with a specified message. */
#define CHECKPOINT_M(m) (Checkpoints::SetCheckpoint(__FILE__, __LINE__, m))

#endif



// Gym Singletons
class HeartRateManager;
extern HeartRateManager* HEARTRATEMAN;
