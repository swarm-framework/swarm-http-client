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

#include "HTTPResult.hxx"
#include "response/InMemoryBodyResponse.hxx"
#include "request/BodyRequest.hxx"

#include <swarm/exception/SwarmException.hxx>
#include <swarm/http/message/response/HTTPResponseBuilder.hxx>
#include <swarm/http/message/header/HTTPHeaders.hxx>

#include <curl/curl.h>
#include <cstring>

#include <ostream>

namespace swarm {

    namespace http {


        /// \brief Internal class used to read headers from libcurl
        class HeaderReader {
            
            /// \brief Read states
            enum State {
                /// \brief Waitting request line
                REQUEST, 
                /// \brief Waiting other headers or commit '\r\n'
                HEADERS
            };
            
        private:
            
            /// \brief Current state
            State state_ = REQUEST;
            
            /// \brief current headers
            std::map<std::shared_ptr<const HTTPHeader>, std::string> headers_{};
            
            void appendHeader(const char * buffer, size_t size) {
                
                size_t index = 0;
                
                std::stringstream keySs{};
                std::stringstream valueSs{};
                
                // Read key
                for (; index < size; ++index) {
                    if (buffer[index] == ':') {
                        ++index;
                        break;
                    }
                    
                    // Add char to key
                    keySs << buffer[index];
                }
                
                if (index >= size) {
                    HTTPClient::LOGGER.log(cxxlog::Level::WARNING, " - Unable to read header %1%", keySs.str());
                    return;
                }
                
                // Remove white spaces
                for (; index < size; ++index) {
                    if (buffer[index] 
                        != ' ') {
                        break;
                    }
                }
                
                // Read value
                for (; index < size; ++index) {
                    
                    char c = buffer[index];
                    
                    if (c == '\n' || c == '\r') {
                        break;
                    }
                    
                    // Add char to value
                    valueSs << c;
                }
                
                HTTPClient::LOGGER.log(cxxlog::Level::INFO, [&](std::ostream & out) { out << " - " << keySs.str() << ": " << valueSs.str();});
                
                // Add header
                headers_[HTTPHeaders::get(keySs.str())] = valueSs.str();
            }
            
        public:
            
            /// \brief Append message
            /// \param buffer Buffer containing the message
            /// \param size Buffer size
            void append(const char * buffer, size_t size) {
                
                if (state_ == REQUEST) {
                    if (size > 4 && strncmp("HTTP", buffer, 4) == 0) {
                        state_ = HEADERS;
                        if (!headers_.empty()) {
                            headers_.clear();
                        }
                    }
                    // FIXME SINON LOG
                } else {
                    appendHeader(buffer, size);
                }
            }
            
            /// \brief Commit headers
            void commit() {
                // FIXME Log commit message
                state_ = REQUEST;
            }
            
            std::map<std::shared_ptr<const HTTPHeader>, std::string> get() {
                return headers_;
            }
        };
        
        // Init logger
        const cxxlog::Logger HTTPClient::LOGGER = LOGGER(HTTPClient);

        // Constructor with host, path, headers and queryParams
        HTTPClient::HTTPClient(const std::string &host, const HTTPMethod &method, const std::string &path,
                               const std::map<std::shared_ptr<const HTTPHeader>, std::string> &headers,
                               const std::map<std::string, std::string> &queryParams,
                               std::shared_ptr<BodyResponseBuilder> bodyResponseBuilder,
                               std::shared_ptr<BodyRequest> bodyRequest)
            : host_(host), method_(method), path_(path), headers_(headers), queryParams_(queryParams),
              bodyResponseBuilder_(bodyResponseBuilder), bodyRequest_(bodyRequest) {

            // Test or init body builder
            if (!bodyResponseBuilder_) {
                bodyResponseBuilder_ = std::shared_ptr<BodyResponseBuilder>{new InMemoryBodyResponseBuilder{}};
            }
        }
        
        // Callback for reading body request content
        size_t HTTPClient::bodyRequestCallback(void *ptr, size_t size, size_t nmemb, void *stream) {
                
            BodyRequest & body = *static_cast<BodyRequest*>(stream);
            
            // Append data
            return body.append(ptr, size * nmemb);
        }

        // Callback for reading headers
        size_t HTTPClient::headersResponseCallback(char *buffer, size_t size, size_t nitems, void *userdata) {
            
            // Cast to header reader
            HeaderReader & headerReader = *static_cast<HeaderReader*>(userdata);
            
            // Find real size
            size_t realSize = size * nitems;
            
            // Test commit
            if (realSize > 2) {
                headerReader.append(buffer, realSize);
            } else {
                headerReader.commit();
            }
            
            return realSize;
        }
        
        // Callback for reading body response
        size_t HTTPClient::bodyResponseCallback(void *contents, size_t size, size_t nmemb, void *userp) {

            // Cast to body
            MutableBodyResponse &body = *static_cast<MutableBodyResponse *>(userp);

            // Append data
            return body.append(static_cast<char *>(contents), size * nmemb);
        }

        // Perform
        std::shared_ptr<HTTPResult> HTTPClient::perform() {

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
                chunk = curl_slist_append(chunk, std::string{entry.first->key() + ": " + entry.second}.c_str());
            }
            
            // Add headers
            curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk);
            
            // Test body request
            if(bodyRequest_) {

                // Enable upload
                curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, 1L);

                // Set request body callback
                curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, bodyRequestCallback);
                
                // Set body request
                curl_easy_setopt(curl_handle, CURLOPT_READDATA, bodyRequest_.get());
                
                // Find and test size
                auto size = bodyRequest_->size();
                if (size.has_value()) {
                    // Set file size
                    curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t) size.value());
                }
            }
            
            // Create header reader
            HeaderReader headerReader{};
            
            // Set response headers callback
            curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, headersResponseCallback);
            
            // Pass body response
            curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, static_cast<void*>(&headerReader));

            
            // Create body reponse
            auto bodyResponse = bodyResponseBuilder_->build();

            // Set response body callback
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, bodyResponseCallback);
            
            // Pass body response
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, static_cast<void*>(bodyResponse.get()));
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

            // Cleanup global
            curl_global_cleanup();

            return std::shared_ptr<HTTPResult>{new HTTPResult{HTTPResponseStatus::fromCode(responseCode), headerReader.get(), bodyResponse}};
        }
    }
}
