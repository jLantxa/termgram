/*
 * tgram
 * Copyright (C) 2021  Javier Lancha Vázquez
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _TERMGRAM_INCLUDE_UI_TERMINAL_SCREEN_HPP_
#define _TERMGRAM_INCLUDE_UI_TERMINAL_SCREEN_HPP_

#include <ncurses.h>

#include <cmath>

#include <string>

class Rect {
public:
    explicit Rect(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom)
    :   m_left(left),
        m_top(top),
        m_right(std::max(right, left)),
        m_bottom(std::max(bottom, top)) { }

    inline unsigned int Left() {
        return m_left;
    }

    inline unsigned int Top() {
        return m_top;
    }

    inline unsigned int Right() {
        return m_right;
    }

    inline unsigned int Bottom() {
        return m_bottom;
    }

    inline unsigned int Width() {
        return (m_right - m_left);
    }

    inline unsigned int Height() {
        return (m_bottom - m_top);
    }

    void Set(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom) {
        m_left = left;
        m_top = top;
        m_right = right;
        m_bottom = bottom;
    }

protected:
    unsigned int m_left;
    unsigned int m_top;
    unsigned int m_right;
    unsigned int m_bottom;
};

#endif  // _TERMGRAM_INCLUDE_UI_TERMINAL_SCREEN_HPP_
