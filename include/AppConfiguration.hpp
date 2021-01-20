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

#ifndef _TERMGRAM_INCLUDE_APP_CONFIGURATION_HPP_
#define _TERMGRAM_INCLUDE_APP_CONFIGURATION_HPP_

#include <cstdint>

#include <string>

#include <td/telegram/td_api.h>

/**
 * \brief Application configuration
 */
struct AppConfiguration {
    /* Use TDlib in test mode */
    bool test_mode = true;

    /** TDlib App API ID */
    td::td_api::int32 api_id;

    /** TDlib App API Hash */
    td::td_api::string api_hash;

    /** App version */
    std::string version;
};

#endif  // _TERMGRAM_INCLUDE_APP_CONFIGURATION_HPP_
