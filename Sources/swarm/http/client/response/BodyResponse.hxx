/*
 * Copyright 2017 Damien Giron <contact@damiengiron.me>
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

#ifndef SWARM_HTTP_BODYRESPONSE_HXX
#define SWARM_HTTP_BODYRESPONSE_HXX

#include <memory>

namespace swarm {
    namespace http {

        /// \brief Define a body response
        class BodyResponse {
        public:
            
            /// \brief Virtual destructor
            virtual ~BodyResponse() {}
            
            /// \brief Convert body to string
            virtual std::string str() = 0;
        };
        
        /// \brief Mutable body response defining internal system used to set data
        class MutableBodyResponse : public BodyResponse {
        public:
            
            /// \brief Appen readed data to body
            /// \param content Data content to append
            /// \param size Size of data content to append
            virtual size_t append(char * content, size_t size) = 0;
        };
        
        /// \brief Define internal response builder
        struct BodyResponseBuilder {
            
            /// \brief Instanciate new response builder
            virtual std::shared_ptr<MutableBodyResponse> build() = 0;
            
            /// \brief Virtual destructor
            virtual ~BodyResponseBuilder() {}
        };
    }
}

#endif // SWARM_HTTP_BODYRESPONSE_HXX
