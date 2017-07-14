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

#ifndef SWARM_HTTP_INMEMORYBODYRESPONSE_HXX
#define SWARM_HTTP_INMEMORYBODYRESPONSE_HXX

#include "BodyResponse.hxx"
#include <memory>
#include <sstream>

namespace swarm {
    namespace http {
        
        /// \brief Class InMemoryBodyResponse
        class InMemoryBodyResponse : public MutableBodyResponse {
        private:
            std::stringstream ss_;
            
        public:
            
            ~InMemoryBodyResponse() {
            }
            
            virtual size_t append(char * content, size_t size) override {
                
                ss_.write(content, size);
                
                return size;
            }
            
            /// \brief Convert body to string
            virtual std::string str() override {
                return ss_.str();
            }
        };
        
        class InMemoryBodyResponseBuilder : public BodyResponseBuilder {
        public:
            virtual std::shared_ptr<MutableBodyResponse> build() override {
                return std::make_shared<InMemoryBodyResponse>();
            }
        };
    }
}

#endif // SWARM_HTTP_INMEMORYBODYRESPONSE_HXX
