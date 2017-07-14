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

#include "HTTPResult.hxx"

namespace swarm {
    namespace http {

        // Constructor
        HTTPResult::HTTPResult(const HTTPResponseStatus &status,
                               const std::map<std::shared_ptr<const HTTPHeader>, std::string> &headers,
                               std::shared_ptr<BodyResponse> body)
            : status_(status), headers_(headers), body_(body) {}

        // Response status
        const HTTPResponseStatus HTTPResult::status() const { return status_; }

        // Response headers
        const std::map<std::shared_ptr<const HTTPHeader>, std::string> & HTTPResult::headers() const { return headers_; }
    }
}
