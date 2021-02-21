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

#include <iostream>

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>

#include "debug.hpp"

#include "TermgramClient.hpp"

extern jltx::debug::Logger Log;
static __attribute_used__ const char* LOG_TAG = "Client";

using td::td_api::updateAuthorizationState;
using td::td_api::authorizationStateWaitTdlibParameters;
using td::td_api::authorizationStateWaitEncryptionKey;
using td::td_api::authorizationStateWaitPhoneNumber;
using td::td_api::authorizationStateWaitCode;
using td::td_api::authorizationStateWaitOtherDeviceConfirmation;
using td::td_api::authorizationStateWaitRegistration;
using td::td_api::authorizationStateWaitPassword;
using td::td_api::authorizationStateReady;
using td::td_api::authorizationStateLoggingOut;
using td::td_api::authorizationStateClosing;
using td::td_api::authorizationStateClosed;

using td::td_api::setLogVerbosityLevel;
using td::td_api::tdlibParameters;
using td::td_api::setTdlibParameters;
using td::td_api::setAuthenticationPhoneNumber;
using td::td_api::checkAuthenticationCode;
using td::td_api::destroy;
using td::td_api::checkDatabaseEncryptionKey;
using td::td_api::checkAuthenticationPassword;

TermgramClient::TermgramClient(const AppConfiguration& app_config)
:   m_app_config(app_config)
{
    td::ClientManager::execute(td::td_api::make_object<setLogVerbosityLevel>(1));
    Init();
}

void TermgramClient::Init() {
    m_client_manager = std::make_unique<td::ClientManager>();
    m_client_id = m_client_manager->create_client_id();
    m_query_id = 0;
    m_auth_query_id = 0;
    m_authorised = false;
    m_need_restart = false;
    SendQuery(td::td_api::make_object<td::td_api::getOption>("version"));
}

void TermgramClient::Restart() {
    Log.i(LOG_TAG, "%s()", __func__);
    m_client_manager.reset();
    Init();
}

void TermgramClient::Quit() {
    m_running = false;
}

void TermgramClient::Run() {
    m_running = true;

    Log.i(LOG_TAG, "Client starts");
    while (m_running) {
        if (m_need_restart) {
            Restart();
        }
        if (!m_authorised) {
            ProcessResponse(m_client_manager->receive(10));
        } else {
            // TODO: Authorised
            Quit();
        }
    }
}

void TermgramClient::SendQuery(object_ptr<td::td_api::Function> function, QueryHandler handler) {
    if (handler) {
        m_query_handlers.emplace(m_query_id, handler);
    }

    m_client_manager->send(m_client_id, m_query_id, std::move(function));
    m_query_id++;
}

void TermgramClient::ProcessResponse(td::ClientManager::Response response) {
    if (!response.object) {
        Log.v(LOG_TAG, "%s(): Received null response object", __func__);
        return;
    }

    const auto request_id = response.request_id;

    if (request_id == 0) {
        OnUpdate(std::move(response.object));
        return;
    }

    auto handler = m_query_handlers.find(request_id);
    if (handler != m_query_handlers.end()) {
        handler->second(std::move(response.object));
    }
}

void TermgramClient::OnUpdate(object_ptr<Object> update) {
    const int32_t update_id = update->get_id();
    switch (update_id) {
        case updateAuthorizationState::ID: {
            auto* updateAuthState = static_cast<updateAuthorizationState*>(update.get());
            const int32_t auth_state = updateAuthState->authorization_state_->get_id();
            OnAuthStateUpdate(auth_state);
            break;
        }

        default:
            Log.v(LOG_TAG, "Unhandled update ID %d", update_id);
            break;
    }
}

void TermgramClient::CheckAuthError(object_ptr<Object> object) {
    if (object->get_id() == td::td_api::error::ID) {
        OnAuthStateUpdate(m_auth_state);
    }
}

void TermgramClient::OnAuthStateUpdate(int32_t auth_state) {
    m_auth_state = auth_state;
    m_auth_query_id++;

    switch (auth_state) {
        case authorizationStateWaitTdlibParameters::ID:
            OnAuthorizationStateWaitTdlibParameters();
            break;

        case authorizationStateWaitEncryptionKey::ID:
            OnAuthorizationStateWaitEncryptionKey();
            break;

        case authorizationStateWaitPhoneNumber::ID:
            OnAuthorizationStateWaitPhoneNumber();
            break;

        case authorizationStateWaitCode::ID:
            OnAuthorizationStateWaitCode();
            break;

        case authorizationStateWaitOtherDeviceConfirmation::ID:
            Log.i(LOG_TAG, "%s(): authorizationStateWaitOtherDeviceConfirmation", __func__);
            break;

        case authorizationStateWaitRegistration::ID:
            Log.i(LOG_TAG, "%s(): authorizationStateWaitRegistration", __func__);
            break;

        case authorizationStateWaitPassword::ID: {
            Log.i(LOG_TAG, "%s(): authorizationStateWaitPassword", __func__);

            std::cout << "Enter authentication password: " << std::flush;
            std::string password;
            std::getline(std::cin, password);
            SendQuery(td::td_api::make_object<checkAuthenticationPassword>(password), AUTH_HANDLER);

            break;
        }

        case authorizationStateReady::ID:
            Log.i(LOG_TAG, "%s(): authorizationStateReady", __func__);
            m_authorised = true;
            break;

        case authorizationStateLoggingOut::ID:
            Log.i(LOG_TAG, "%s(): authorizationStateLoggingOut", __func__);
            m_authorised = false;
            break;

        case authorizationStateClosing::ID:
            Log.i(LOG_TAG, "%s(): authorizationStateClosing", __func__);
            m_authorised = false;
            break;

        case authorizationStateClosed::ID:
            Log.i(LOG_TAG, "%s(): authorizationStateClosed", __func__);
            m_authorised = false;
            m_need_restart = true;
            break;

        default:
            Log.v(LOG_TAG, "%s(): unhandled auth state update", __func__);
            break;
    }
}

void TermgramClient::OnAuthorizationStateWaitTdlibParameters() {
    Log.i(LOG_TAG, "%s()", __func__);

    auto parameters = td::td_api::make_object<tdlibParameters>();
    parameters->use_test_dc_ = m_app_config.test_mode;
    parameters->database_directory_ = "tdlib";
    parameters->use_message_database_ = true;
    parameters->use_secret_chats_ = false;
    parameters->api_id_ = m_app_config.api_id;
    parameters->api_hash_ = m_app_config.api_hash;
    parameters->system_language_code_ = "en";
    parameters->device_model_ = "Desktop";
    parameters->application_version_ = m_app_config.version;
    parameters->enable_storage_optimizer_ = true;

    SendQuery(td::td_api::make_object<setTdlibParameters>(std::move(parameters)), AUTH_HANDLER);
}

void TermgramClient::OnAuthorizationStateWaitEncryptionKey() {
    Log.i(LOG_TAG, "%s()", __func__);

    std::cout << "Enter encryption key or DESTROY: " << std::flush;
    std::string key;
    std::getline(std::cin, key);
    if (key == "DESTROY") {
        SendQuery(td::td_api::make_object<destroy>());
    } else {
        SendQuery(td::td_api::make_object<checkDatabaseEncryptionKey>(std::move(key)), AUTH_HANDLER);
    }
}

void TermgramClient::OnAuthorizationStateWaitPhoneNumber() {
    Log.i(LOG_TAG, "%s()", __func__);

    std::cout << "Enter phone number: " << std::flush;
    std::string phone_number;
    std::cin >> phone_number;
    SendQuery(td::td_api::make_object<setAuthenticationPhoneNumber>(phone_number, nullptr), AUTH_HANDLER);
}

void TermgramClient::OnAuthorizationStateWaitCode() {
    Log.i(LOG_TAG, "%s()", __func__);

    std::cout << "Enter authentication code: " << std::flush;
    std::string code;
    std::cin >> code;
    SendQuery(td::td_api::make_object<checkAuthenticationCode>(code), AUTH_HANDLER);
}

void TermgramClient::OnAuthorizationStateWaitPassword() {
    Log.i(LOG_TAG, "%s()", __func__);

    std::cout << "Enter authentication password: " << std::flush;
    std::string password;
    std::getline(std::cin, password);
    SendQuery(td::td_api::make_object<checkAuthenticationPassword>(password), AUTH_HANDLER);
}
