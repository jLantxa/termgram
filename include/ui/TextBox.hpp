/*
 * termgram
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

#ifndef _TERMGRAM_INCLUDE_UI_TEXT_BOX_HPP_
#define _TERMGRAM_INCLUDE_UI_TEXT_BOX_HPP_

#include <ncurses.h>

#include <list>
#include <string>

class TextBox {
public:
    TextBox(std::wstring title, unsigned int m_max_length);
    virtual ~TextBox();

    struct Listener {
        virtual void OnEnterPressed(TextBox* text_box) = 0;
    };

    void Input(wint_t ch);
    void Draw();
    void OnTerminalResize();

    std::wstring GetText() const;

    void SetTitle(std::wstring title);
    void SetMaxLength(unsigned int max_length);
    void AddListener(Listener* listener);
    void RemoveListener(Listener* listener);
    void Clear();

private:
    unsigned int m_screen_rows;
    unsigned int m_screen_cols;
    unsigned int m_max_length;

    /** Number of editable rows */
    static constexpr int EDIT_HEIGHT = 1;

    unsigned int m_col_cursor = 0;
    unsigned int m_char_cursor = 0;
    std::wstring m_text;
    std::wstring m_title;
    WINDOW* m_text_box;

    std::list<Listener*> m_listeners;

    void UpdateScreenBounds();

    void CursorLeft();
    void CursorRight();

    void DrawBox();
};

#endif  // _TERMGRAM_INCLUDE_UI_TEXT_BOX_HPP_
