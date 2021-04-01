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

#ifndef _TERMGRAM_INCLUDE_UI_TEXT_BOX_HPP_
#define _TERMGRAM_INCLUDE_UI_TEXT_BOX_HPP_

#include <ncurses.h>

#include <list>
#include <string>

#include "Terminal.hpp"

/**
 * \brief A textbox widget for text input. It supports wide characters by default.
 */
class TextBox {
public:
    TextBox(Rect rect, std::wstring title, unsigned int m_max_length);
    virtual ~TextBox();

    /**
     * \brief An event listener for the TextBox class
     **/
    struct Listener {
        /**
         * \brief Callback for pressing the ENTER key.
         * \param text_box A pointer to the TextBox that generated the event.
         */
        virtual void OnEnterPressed(TextBox* text_box) = 0;
    };

    /**
     * \brief Input one character into the TextBox. It can be a textual character
     * or an action character such as ENTER, BACKSPACE, DELETE, CTRL, etc...
     */
    void Input(wint_t ch);

    /**
     * \brief Draw the TextBox widget into the screen.
     */
    void Draw();

    /**
     * \brief Return the std::wstring representation of the contents of the TextBox.
     * \return TextBox content as a std::wstring
     */
    std::wstring GetText() const;

    /**
     * \brief Sets the title of the TextBox.
     * \param title A title represented as a std::wstring
     */
    void SetTitle(std::wstring title);

    /**
     * \brief Set the maximum number of character the TextBox can hold.
     * \param max_length Maximum number of characters. If set to 0,
     * there will be no limit.
     */
    void SetMaxLength(unsigned int max_length);

    /**
     * \brief Add a Listener for TextBox events. Listeners are stored in a list. If the listener
     * already exists in the list it will not be added again.
     * \param listener Pointer to a Listener
     */
    void AddListener(Listener* listener);

    /**
     * \brief Remove a Listener from the list of Listeners. Listeners are only removed if they exist
     * inside the list.
     * \param listener Pointer to a Listener.
     */
    void RemoveListener(Listener* listener);

    /**
     * \brief Clear the TextBox. It deletes the text, but the title is kept as is.
     **/
    void Clear();

private:
    Rect m_rect;
    unsigned int m_max_length;

    /** Number of editable rows */
    static constexpr int EDIT_HEIGHT = 1;

    unsigned int m_col_cursor = 0;
    unsigned int m_char_cursor = 0;
    std::wstring m_text;
    std::wstring m_title;
    WINDOW* m_text_box;
    WINDOW* m_outter_window;

    std::list<Listener*> m_listeners;

    /**
     * \brief Move one character to the left.
     */
    void CursorLeft();

    /**
     * \brief Move one character to the right.
     */
    void CursorRight();

    /**
     * \brief Draw the enclosing windo box
     */
    void DrawBox();
};

#endif  // _TERMGRAM_INCLUDE_UI_TEXT_BOX_HPP_
