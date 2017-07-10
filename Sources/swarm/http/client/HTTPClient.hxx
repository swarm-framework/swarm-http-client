/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef SWARM_HTTP_HTTPCLIENT_HXX
#define SWARM_HTTP_HTTPCLIENT_HXX

#include "body/BodyResponse.hxx"
#include <cxxlog/Logger.hxx>

#include <map>
#include <string>

#include <swarm/http/message/request/HTTPMethod.hxx>
#include <swarm/http/message/response/HTTPResponse.hxx>

namespace swarm {
    namespace http {

        /// \brief Class HTTPClient
        class HTTPClient {

            friend class HTTPClientBuilder;

          private:
            // Set logger
            static const cxxlog::Logger LOGGER;

          private:
            /// \brief Serveur host
            std::string host_;

            /// \brief define HTTP method
            HTTPMethod method_;

            /// \brief URL path
            std::string path_;

            /// \brief Headers
            std::map<std::string, std::string> headers_;

            /// \brief Query parameters
            std::map<std::string, std::string> queryParams_;

            /// \brief Body response builder
            std::shared_ptr<BodyResponseBuilder> bodyResponseBuilder_;

            /// \brief Delete default constructor
            HTTPClient() = delete;

          private:
            /// \brief Constructor with host, path, headers and queryParams
            /// \param host Server host
            /// \param method HTTP method
            /// \param path Query path
            /// \param headers All query headers
            /// \param queryParams All query params
            /// \param body response builder
            HTTPClient(const std::string &host, const HTTPMethod &method, const std::string &path,
                       const std::map<std::string, std::string> &headers,
                       const std::map<std::string, std::string> &queryParams,
                       std::shared_ptr<BodyResponseBuilder> bodyResponseBuilder);

            /// \brief Callback for reading body response
            /// \param contents buffer pointer
            /// \param size First part size
            /// \param nmemb Second part size (multiplicator)
            /// \param userp User pointer
            static size_t bodyResponseCallback(void *contents, size_t size, size_t nmemb, void *userp);

          public:
            HTTPResponse perform();
        };
    }
}

#endif // SWARM_HTTP_HTTPCLIENT_HXX
