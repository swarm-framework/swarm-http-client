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
#define SWARM_HTTP_HTTPRESULT_HXX

#include <memory>
#include <string>
#include <swarm/http/message/response/HTTPResponseStatus.hxx>
#include "response/BodyResponse.hxx"

namespace swarm {
    namespace http {
        
        /// \brief Class HTTPResult
        class HTTPResult {
            
        private:
            
            HTTPResponseStatus status_;
            
            std::shared_ptr<BodyResponse> response_;
            
        public:
            
            HTTPResult(const HTTPResponseStatus & status, std::shared_ptr<BodyResponse> response);
            
            const HTTPResponseStatus status();
            
            template<class T>
            T body();
        };
    }
}

#include "HTTPResult.txx"

#endif // SWARM_HTTP_HTTPRESULT_HXX
