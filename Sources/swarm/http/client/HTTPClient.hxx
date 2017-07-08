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

#include <cxxlog/Logger.hxx>

#include <string>
#include <map>

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

            /// \brief URL path
            std::string path_;

            /// \brief Headers
            std::map<std::string, std::string> headers_;
            
            /// \brief Query parameters
            std::map<std::string, std::string> queryParams_;
            
            /// \brief Delete default constructor
            HTTPClient() = delete;
            
        private:
            
            /// \brief Constructor with host, path, headers and queryParams
            /// \param host Server host
            /// \param path Query path
            /// \param headers All query headers
            /// \param queryParams All query params
            HTTPClient(const std::string & host, 
                       const std::string & path, 
                       const std::map<std::string, std::string> & headers, 
                       const std::map<std::string, std::string> & queryParams);
            
        public:
            
            HTTPResponse perform();
        };
        
    }
}

#endif // SWARM_HTTP_HTTPCLIENT_HXX
