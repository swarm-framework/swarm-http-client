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

#ifndef SWARM_HTTP_HTTPRESULT_HXX
#include "HTTPResult.hxx"
#endif // SWARM_HTTP_HTTPRESULT_HXX

#include <string>
#include <swarm/exception/SwarmException.hxx>

namespace swarm {
    namespace http {
                
        template<class T>
        struct BodyConverter {
            T convert(std::shared_ptr<BodyResponse> response) {
                throw SwarmException{"Unable to get data"};
            }
        };
                        
        template<>
        struct BodyConverter<std::string> {
            std::string convert(std::shared_ptr<BodyResponse> response) {
                return response->str();
            }
        };
        
        template<class T>
        T HTTPResult::body() {
            return BodyConverter<T>{}.convert(response_);
        }
    }
}
