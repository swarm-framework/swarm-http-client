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

#include "HTTPClient.hxx"

#include "body/InMemoryBodyResponse.hxx"

#include <swarm/exception/SwarmException.hxx>
#include <swarm/http/message/response/HTTPResponseBuilder.hxx>

#include <curl/curl.h>
#include <iostream>

namespace swarm {

    namespace http {

        // Init logger
        const cxxlog::Logger HTTPClient::LOGGER = LOGGER(HTTPClient);

        // Constructor with host, path, headers and queryParams
        HTTPClient::HTTPClient(const std::string &host, const HTTPMethod &method, const std::string &path,
                               const std::map<std::string, std::string> &headers,
                               const std::map<std::string, std::string> &queryParams,
                               std::shared_ptr<BodyResponseBuilder> bodyResponseBuilder)
            : host_(host), method_(method), path_(path), headers_(headers), queryParams_(queryParams),
              bodyResponseBuilder_(bodyResponseBuilder) {

            // Test or init body builder
            if (!bodyResponseBuilder_) {
                bodyResponseBuilder_ = std::shared_ptr<BodyResponseBuilder>{new InMemoryBodyResponseBuilder{}};
            }
        }

        // Callback for reading body response
        size_t HTTPClient::bodyResponseCallback(void *contents, size_t size, size_t nmemb, void *userp) {
            
            // Cast to body
            MutableBodyResponse & body = *static_cast<MutableBodyResponse*>(userp);
            
            // Append data
            return body.append(static_cast<char *>(contents), size * nmemb);
        }

        // Perform
        HTTPResponse HTTPClient::perform() {

            std::stringstream ss;

            // Add host
            ss << host_;

            // Test path
            ss << path_;

            // Test query param
            if (!queryParams_.empty()) {
                ss << '?';

                auto it = queryParams_.begin();
                auto itEnd = queryParams_.end();

                while (it != itEnd) {

                    auto entry = *it;

                    ss << entry.first << '=' << entry.second;

                    ++it;

                    if (it != itEnd) {
                        ss << '&';
                    }
                }
            }

            LOGGER.log(cxxlog::Level::INFO, ss.str());
            for (auto entry : headers_) {
                LOGGER.log(cxxlog::Level::INFO, "  - %1%:%2%", entry.first, entry.second);
            }

            // Init global
            curl_global_init(CURL_GLOBAL_ALL);

            // Init curl connection
            CURL *curl_handle = curl_easy_init();
            if (!curl_handle) {
                throw swarm::SwarmException{"Unable to init curl"};
            }

            // Set method
            curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, method_.name().c_str());

            // Create header chunk
            struct curl_slist *chunk = NULL;

            // Add all headers
            for (auto entry : headers_) {
                chunk = curl_slist_append(chunk, std::string{entry.first + ": " + entry.second}.c_str());
            }

            // Add headers
            curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk);

            // Set response body callback
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, bodyResponseCallback);
            
            // Create body reponse
            auto bodyResponse = bodyResponseBuilder_->build();
            
            // Pass body response
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)bodyResponse.get());

            // Set URL
            curl_easy_setopt(curl_handle, CURLOPT_URL, ss.str().c_str());

            auto res = curl_easy_perform(curl_handle);
            if (res != CURLE_OK) {
                throw SwarmException{"%1% for %2%", curl_easy_strerror(res), ss.str()};
            }

            // Read response
            long responseCode;
            curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &responseCode);

            // Read content type
            char *ct;
            curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_TYPE, &ct);

            std::cout << "Response : " << responseCode << " : " << ct << std::endl;

            auto response = HTTPResponseBuilder{}.build();

            // Cleanup global
            curl_global_cleanup();

            return response;
        }
    }
}
