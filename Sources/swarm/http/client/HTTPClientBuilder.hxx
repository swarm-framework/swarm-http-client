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

#ifndef SWARM_HTTP_HTTPCLIENTBUILDER_HXX
#define SWARM_HTTP_HTTPCLIENTBUILDER_HXX

#include "HTTPClient.hxx"
#include <swarm/http/message/request/HTTPMethod.hxx>
#include <cxxlog/Logger.hxx>

#include <string>
#include <map>

namespace swarm {
    namespace http {

        /// \brief Define an HTTP client builder
        class HTTPClientBuilder {

          private:
              
            // Set logger
            static const cxxlog::Logger LOGGER;

          private:
            /// \brief Method
            HTTPMethod method_ = HTTPMethod::GET;

            /// \brief Serveur host
            std::string host_;

            /// \brief URL path
            std::string path_;

            /// \brief Query parameters
            std::map<std::string, std::string> queryParams_;

            /// \brief Path parameters
            std::map<std::string, std::string> pathParams_;

            /// \brief Headers
            std::map<std::string, std::string> headers_;

            /// \brief Transform path
            /// \param params All params
            /// \param path Raw path
            std::string transform(const std::map<std::string, std::string> &params, const std::string &path);

          public:
            /// \brief Use to update method
            /// \param method New method
            /// \return Current builder
            HTTPClientBuilder &method(const HTTPMethod &method);

            /// \brief Use to set host
            /// \param host New host
            /// \return Current builder
            HTTPClientBuilder &host(const std::string &host);

            /// \brief Use to set path
            /// \param path New path
            /// \return Current builder
            HTTPClientBuilder &path(const std::string &path);

            /// \brief Add query param
            /// \param key Query key
            /// \param value Query value
            /// \return Current builder
            HTTPClientBuilder &queryParam(const std::string &key, const std::string &value);

            /// \brief Add path param
            /// \param key Path key
            /// \param value Path value
            /// \return Current builder
            HTTPClientBuilder &pathParam(const std::string &key, const std::string &value);

            /// \brief Add header
            /// \param key Header key
            /// \param value Header value
            /// \return Current builder
            HTTPClientBuilder &header(const std::string &key, const std::string &value);

            /// \brief Build client
            /// \return client
            std::shared_ptr<HTTPClient> build();
        };
    }
}

#endif // SWARM_HTTP_HTTPCLIENTBUILDER_HXX
