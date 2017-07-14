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

#include "response/BodyResponse.hxx"
#include <map>
#include <memory>
#include <string>
#include <swarm/http/message/header/HTTPHeader.hxx>
#include <swarm/http/message/response/HTTPResponseStatus.hxx>

namespace swarm {
    namespace http {

        /// \brief Define an HTTP client result
        class HTTPResult {

          private:
            /// \brief Response status
            HTTPResponseStatus status_;

            /// \brief Headers response
            std::map<std::shared_ptr<const HTTPHeader>, std::string> headers_;
            ww
            /// \brief Response body
            std::shared_ptr<BodyResponse> body_;

          public:
              
              /// \brief Constructor  with status, headers and body
              /// \param status Response status
              /// \param headers Responses headers
              /// \param body Response body
            HTTPResult(const HTTPResponseStatus &status,
                       const std::map<std::shared_ptr<const HTTPHeader>, std::string> &headers,
                       std::shared_ptr<BodyResponse> body);

            /// \return Response status
            const HTTPResponseStatus status() const;
            
            /// \return Response headers
            const std::map<std::shared_ptr<const HTTPHeader>, std::string> & headers() const;

            /// \return Retrieve body as T
            template <class T>
            std::shared_ptr<T> body();
        };
    }
}

#include "HTTPResult.txx"

#endif // SWARM_HTTP_HTTPRESULT_HXX
