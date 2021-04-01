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

#ifndef _TERMGRAM_INCLUDE_TERMGRAM_CLIENT_HPP_
#define _TERMGRAM_INCLUDE_TERMGRAM_CLIENT_HPP_

#include <functional>
#include <memory>
#include <unordered_map>

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include "AppConfiguration.hpp"

using td::td_api::object_ptr;
using td::td_api::Object;

using QueryHandler = std::function<void(object_ptr<Object>)>;

class TGramClient final {
public:
    TGramClient(const AppConfiguration& app_config);
    ~TGramClient();

    /**
     * \brief Start execution of the client
     * Sets m_running to true and starts the application loop.
     */
    void Run();

private:
    /**
     * \brief Initialise the client.
     */
    void Init();

    /**
     * \brief Restart the client without quitting. This means resetting the
     * client manager and the query ids to 0.
     */
    void Restart();

    /**
     * \brief Handle terminal input
     * \param ch Input character
     */
    void HandleTerminalInput(int ch);

    /** \brief Stop running the main loop effectively quitting */
    inline void Quit();

    /** True while the client is running */
    bool m_running = false;

    /** True when the client is authorised */
    bool m_authorised = false;

    /** Need to restart the client manager */
    bool m_need_restart = false;

    /** Static app configuration */
    const AppConfiguration m_app_config;

    std::unique_ptr<td::ClientManager> m_client_manager;
    std::int32_t m_client_id;

    /** Query id. Auto-incremented after every request. */
    uint64_t m_query_id = 0;

    /** Authentication query id. Auto-incremented after every request. */
    uint64_t m_auth_query_id = 0;

    /** Current authentication status */
    int32_t m_auth_state;

    /**
     * \brief Query handlers. It maps a query request id with a handler function
     * to be executed when a response with that id is received.
     */
    std::unordered_map<uint64_t, QueryHandler> m_query_handlers;

    // TODO: Implement screens
    // TerminalScreen& m_current_screen;

    /**
     * \brief Send a query to the Telegram server.
     * \param function Query function. Container for the data to be sent.
     * \param handler Handler function to be executed when a response with the same id
     * is received.
     */
    void SendQuery(object_ptr<td::td_api::Function> function, QueryHandler handler = {});

    /**
     * \brief Process a response object from the server
     * \param response Response object. It can be null if no response was queued.
    */
    void ProcessResponse(td::ClientManager::Response response);

    /**
     * \brief Handler for update objects received from the server.
     * \param update Update object.
     **/
    void OnUpdate(object_ptr<Object> update);

    /**
     * \brief Handler for authentication state updates.
     * \param auth_state State of the authentication process.
     */
    void OnAuthStateUpdate(int32_t auth_state);
    inline void OnAuthorizationStateWaitTdlibParameters();
    inline void OnAuthorizationStateWaitEncryptionKey();
    inline void OnAuthorizationStateWaitPhoneNumber();
    inline void OnAuthorizationStateWaitCode();
    inline void OnAuthorizationStateWaitPassword();

    /**
     * \brief Check a authentication response object for error.
     */
    void CheckAuthError(object_ptr<Object> object);

    /** Handler for authentication queries */
    const QueryHandler AUTH_HANDLER = [this](object_ptr<Object> object) {
        CheckAuthError(std::move(object));
    };
};

#endif  // _TERMGRAM_INCLUDE_TERMGRAM_CLIENT_HPP_
