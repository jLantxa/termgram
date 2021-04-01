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

#include <string>

class TerminalScreen {
public:
    virtual ~TerminalScreen() = default;

    /**
     * \brief Pass terminal inputs.
     */
    virtual void Input(int ch) = 0;

    /**
     * \brief Draw screen to terminal
     */
    virtual void Draw() = 0;

    /**
     * \brief Callback for terminal resize
     */
    virtual void OnTerminalResize() = 0;
};

#endif  // _TERMGRAM_INCLUDE_UI_TERMINAL_SCREEN_HPP_
