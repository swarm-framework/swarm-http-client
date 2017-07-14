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

#ifndef SWARM_HTTP_BODYREQUEST_HXX
#define SWARM_HTTP_BODYREQUEST_HXX

#include <optional>
#include <sstream>

namespace swarm {
    namespace http {

        /// \brief Class BodyRequest
        struct BodyRequest {
            
            /// \return content length
            virtual std::optional<size_t> size() const = 0;
            
            /// \brief Append data
            /// \param ptr External pointer
            /// \param size Max size
            virtual size_t append(void *ptr, size_t size) = 0;
            
            /// \brief Destructor
            virtual ~BodyRequest() {}
        };
        
        /// \brief String body request
        class StringBodyRequest : public BodyRequest {
            
        private:
            
            /// \brief Body as string
            std::string str_;
            
            /// \brief Offset to read
            size_t offset_ = 0;
            
            /// \brief C point to chars
            const char * c_str;
            
        public:
            
            /// \brief Constructor with string
            /// \param str STring containing the body
            StringBodyRequest(const std::string & str);
            
            /// \return content length
            virtual std::optional<size_t> size() const override;
            
            /// \brief Append data
            /// \param ptr External pointer
            /// \param size Max size
            virtual size_t append(void *ptr, size_t size) override;
        };
    }
}

#endif // SWARM_HTTP_BODYREQUEST_HXX
