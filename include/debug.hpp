/*
 * Copyright (C) 2020 - 2021  Javier Lancha Vázquez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _JLTX_INCLUDE_DEBUG_HPP_
#define _JLTX_INCLUDE_DEBUG_HPP_

#include <cstdio>

namespace jltx {
namespace debug {

#ifdef DEBUG_LEVEL
    static constexpr int defined_level = DEBUG_LEVEL;
#else
    static constexpr int defined_level = 0;
#endif

template <typename... Args>
static void log(FILE* file, const char* tag, const char* levelTag, const char* fmt, Args... args) {
    fprintf(file, "[%s] %s: ", levelTag, tag);
    fprintf(file, fmt, args...);
    fprintf(file, "\n");
}

template <typename... Args>
static void log(FILE* file, const char* tag, const char* levelTag, const char* str) {
    log(file, tag, levelTag, "%s", str);
}

/** \brief Custom logger to configurable output file */
class Logger final {
private:
    FILE* file;

public:
    explicit Logger(FILE* log_file) : file(log_file) { }

#define LOG_FUNCTION(name, level, level_tag) \
    template <typename... Args> \
    inline void name(const char* tag, Args... args) const { \
        if constexpr (defined_level >= level) { \
            log(file, tag, level_tag, args...); \
        } \
    }

    LOG_FUNCTION(e, -2, "ERROR");
    LOG_FUNCTION(w, -1, "WARNING");
    LOG_FUNCTION(i,  0, "INFO");
    LOG_FUNCTION(d,  1, "DEBUG");
    LOG_FUNCTION(v,  2, "VERBOSE");
};

/** Standard Logger to stdout */
const Logger Log(stdout);

}   // namespace debug
}   // namespace jltx

#endif  // _JLTX_INCLUDE_DEBUG_HPP_
