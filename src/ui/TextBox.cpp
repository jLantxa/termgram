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

#include <ncurses.h>

#include <cmath>

#include <iostream>
#include <string>

#include "ui/keyboard.hpp"
#include "ui/TextBox.hpp"

TextBox::TextBox(Rect rect, std::wstring title, unsigned int max_length)
:   m_rect(rect), m_max_length(max_length)
{
    m_outter_window = newwin(
        m_rect.Height(),
        m_rect.Width(),
        m_rect.Top(),
        m_rect.Left());
    m_text_box = newwin(
        m_rect.Height() - 2,
        m_rect.Width() - 2,
        m_rect.Top() + 1,
        m_rect.Left() + 1);
    SetTitle(title);
    m_max_length = std::min(m_max_length, (m_rect.Width()) - 2);
}

TextBox::~TextBox() {
    werase(m_text_box);
    werase(m_outter_window);
}

void TextBox::Draw() {
    wclear(m_text_box);
    DrawBox();
    mvwaddwstr(m_outter_window, 0, 1, m_title.c_str());
    mvwaddwstr(m_text_box, 0, 0, m_text.c_str());
    wmove(m_text_box, 0, m_col_cursor);
    wrefresh(m_outter_window);
    wrefresh(m_text_box);
}

void TextBox::Clear() {
    m_char_cursor = 0;
    m_col_cursor = 0;
    m_text.clear();
    wclear(m_text_box);
    Draw();
}

void TextBox::DrawBox() {
    box(m_outter_window, 0, 0);
}

void TextBox::SetTitle(std::wstring title) {
    m_title = title;
    Draw();
}

void TextBox::CursorLeft() {
    if (m_char_cursor > 0) {
        --m_char_cursor;
        m_col_cursor -= wcwidth(m_text[m_char_cursor]);
    }
}

void TextBox::CursorRight() {
    if (m_char_cursor < m_text.size()) {
        ++m_char_cursor;
        m_col_cursor += wcwidth(m_text[m_char_cursor-1]);
    }
}

void TextBox::SetMaxLength(unsigned int max_length) {
    m_max_length = max_length;
}

void TextBox::AddListener(Listener* listener) {
    auto found_listener = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (found_listener == m_listeners.end()) {
        m_listeners.push_back(listener);
    }
}

void TextBox::RemoveListener(Listener* listener) {
    auto found_listener = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (found_listener != m_listeners.end()) {
        m_listeners.erase(found_listener);
    }
}

std::wstring TextBox::GetText() const {
    return m_text;
}

void TextBox::Input(wint_t ch) {
    switch (ch) {
        case KB_ENTER:
            for (Listener* listener : m_listeners) {
                listener->OnEnterPressed(this);
            }
            break;

        case KEY_BACKSPACE:
            if (!m_text.empty() && m_char_cursor > 0 && m_char_cursor <= m_text.size()) {
                CursorLeft();
                m_text.erase(m_char_cursor, 1);
                Draw();
            }
            break;

        case KEY_DC:
            if (!m_text.empty() && m_char_cursor < m_text.size()) {
                m_text.erase(m_char_cursor, 1);
                Draw();
            }
            break;

        case KEY_LEFT:
            CursorLeft();
            Draw();
            break;

        case KEY_RIGHT:
            CursorRight();
            Draw();
            break;

        case KEY_UP:
        case KEY_DOWN:
            break;

        default: {
            const bool can_input_char = (m_max_length == 0) || (m_text.size() < m_max_length);
            if (can_input_char) {
                m_text.insert(m_char_cursor, 1, ch);
                CursorRight();
                Draw();
            }
            break;
        }
    }
}
