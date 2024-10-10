#pragma once

#include <cstddef>

namespace ntw {

/**
 * @brief The size of the buffer used for network operations.
 *
 * This constant defines the size of the buffer in bytes used when reading
 * from or writing to TCP sockets. A buffer size of 4096 bytes is chosen
 * as it provides a balance between memory usage and the ability to handle
 * reasonably sized messages efficiently.
 *
 * @note Adjusting this value may impact the performance and memory footprint
 *       of the networking components. Ensure that the buffer size aligns with
 *       the expected message sizes and system capabilities.
 */
constexpr std::size_t BUFF_SIZE = 4096;

} // namespace ntw
