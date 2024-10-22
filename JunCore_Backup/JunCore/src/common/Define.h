#include <cinttypes>
#include <chrono>

// signed
typedef int64_t		int64;
typedef int32_t		int32;
typedef int16_t		int16;
typedef int8_t		int8;

// unsigned
typedef uint64_t	uint64;
typedef uint32_t	uint32;
typedef uint16_t	uint16;
typedef uint8_t		uint8;

// Milliseconds shorthand typedef.
typedef std::chrono::milliseconds Milliseconds;

// Seconds shorthand typedef.
typedef std::chrono::seconds Seconds;

// Minutes shorthand typedef.
typedef std::chrono::minutes Minutes;

// Hours shorthand typedef.
typedef std::chrono::hours Hours;

// time_point shorthand typedefs
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::system_clock::time_point SystemTimePoint;