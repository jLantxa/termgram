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

#include <debug.hpp>
#include "ui/TextBox.hpp"

#include <iostream>

struct ClearListener : public TextBox::Listener {
    void OnEnterPressed(TextBox* tb) {
        tb->Clear();
    }
};

struct TitleListener : public TextBox::Listener {
    void OnEnterPressed(TextBox* tb) {
        tb->SetTitle(tb->GetText());
    }
};

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");

    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    TextBox text_box(Rect{0, 0, 32, 4}, L"Input text", 64);

    TitleListener title_listener;
    ClearListener clear_listener;
    text_box.AddListener(&title_listener);
    text_box.AddListener(&clear_listener);

    while (true) {
        wint_t wch;
        get_wch(&wch);
        text_box.Input(wch);
    }

    endwin();
    return 0;
}
