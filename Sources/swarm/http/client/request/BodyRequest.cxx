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

#include "BodyRequest.hxx"

#include <cstring>

namespace swarm {
    namespace http {
        
            StringBodyRequest::StringBodyRequest(const std::string & str) : str_(str) {
                
            }
            
            // content length
            std::optional<size_t> StringBodyRequest::size() const {
                return str_.str().length();
            }
            
            // Append data
            size_t StringBodyRequest::append(void *ptr) {
                auto size = str_.str().length();
                const char * cStr = str_.str().c_str();
                memcpy (ptr, cStr, size );
                return size;
            }
    }
}
