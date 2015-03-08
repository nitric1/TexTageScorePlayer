#pragma once

#pragma execution_character_set("UTF-8")

// All inclusion listing is dictionary order; Ordering is case insensitive.
// Standard C/C++ library & boost library inclusion

#include <cassert>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/range/any_range.hpp>
#include <boost/range/iterator_range_core.hpp>

// External library inclusion

#include <SDL/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <cppformat/format.h>
