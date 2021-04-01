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
#include <unistd.h>

#include <csignal>
#include <cstdio>

#include <functional>
#include <string>

#include <debug.hpp>

#include "AppConfiguration.hpp"
#include "TGramClient.hpp"

static const char* LOG_PATH = "log.txt";
static FILE* LOG_FILE = fopen(LOG_PATH, "w");
jltx::debug::Logger Log(LOG_FILE);

static bool ValidateAppConfig(const AppConfiguration& app_config) {
    const bool api_id_error = (app_config.api_id == -1);
    const bool api_hash_error = (app_config.api_hash.compare("") == 0);

    if (api_id_error || api_hash_error) {
        return false;
    }

    return true;
}

static void ArgumentError(char* arg0) {
    fprintf(stderr, "Usage: %s [-t] <-i api_id> <-h api_hash>\n", arg0);
}

int main(int argc, char *argv[]) {
    bool test_mode = false;
    int32_t api_id = -1;
    std::string api_hash;

    int opt;
    while ((opt = getopt(argc, argv, "ti:h:")) != -1) {
        switch (opt) {
        case 't':
            test_mode = true;
            break;
        case 'i':
            api_id = atoi(optarg);
            break;
        case 'h':
            api_hash = std::string {optarg};
            break;
        default: /* '?' */
            ArgumentError(argv[0]);
            return -1;
        }
    }

    const AppConfiguration app_config = {
        .test_mode = test_mode,
        .api_id = api_id,
        .api_hash = api_hash,
    };

    if (!ValidateAppConfig(app_config)) {
        ArgumentError(argv[0]);
        return -1;
    }

    // TODO: Implement screens
    // setlocale(LC_ALL, "");
    // initscr();
    // raw();
    // cbreak();
    // noecho();
    // keypad(stdscr, true);

    TGramClient client(app_config);
    client.Run();

    fclose(LOG_FILE);
    // endwin();
    return 0;
}
