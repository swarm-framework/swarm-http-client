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

#include "HTTPClientBuilder.hxx"

#include <swarm/exception/SwarmException.hxx>
#include <sstream>

namespace swarm {
    namespace http {
        
        // Init logger
        const cxxlog::Logger HTTPClientBuilder::LOGGER = LOGGER(HTTPClientBuilder);
        
        // Use to update method
        HTTPClientBuilder & HTTPClientBuilder::method(const HTTPMethod & method) {
            this->method_ = method;
            return *this;
        }
                    
        // Use to set host
        HTTPClientBuilder & HTTPClientBuilder::host(const std::string & host) {
            this->host_ = host;
            return *this;
        }
                                
        // Use to set path
        HTTPClientBuilder & HTTPClientBuilder::path(const std::string & path) {
            this->path_ = path;
            return *this;
        }
                                            
        // Add query param
        HTTPClientBuilder & HTTPClientBuilder::queryParam(const std::string & key, const std::string & value) {
            this->queryParams_[key] = value;
            return *this;
        }
                                                        
        // Add path param
        HTTPClientBuilder & HTTPClientBuilder::pathParam(const std::string & key, const std::string & value) {
            this->pathParams_[key] = value;
            return *this;
        }
        
        // Add header
        HTTPClientBuilder & HTTPClientBuilder::header(const std::string & key, const std::string & value) {
            this->headers_[key] = value;
            return *this; 
        }
        
        std::string decodeVariable(const std::map<std::string, std::string> & params, 
                                    std::string::const_iterator & current, 
                                    std::string::const_iterator end) {
            
            std::stringstream ss;
            
            ++current;
                
            while (current != end) {
                
                char c = *current;
            
                if (c == '}') {
                    ++current;
                    break;
                } else {
                    ss << c;
                }
                
                ++current;
            }
                
            auto param = params.find(ss.str());
            
            if (param == params.end()) {
                return "";
            } else {
                return param->second;
            }
        }
        
        // Transform path
        std::string HTTPClientBuilder::transform(const std::map<std::string, std::string> & params, const std::string & path) {
            
            std::stringstream ss;
            
            std::string::const_iterator it = path.begin();
            std::string::const_iterator end = path.end();
            
            while (it != end) {
                
                char c = *it;
                
                if (c == '{') {
                    ss << decodeVariable(params, it, end);
                } else {
                    ss << c;
                    ++it;
                }
            }
            
            return ss.str();
        }
        
        // Build client
        std::shared_ptr<HTTPClient> HTTPClientBuilder::build() {
            
            // Valid host
            if (host_.empty()) {
                throw SwarmException{"Unable to create WS without host"};
            }
            
            std::string path;
            if (path_.empty()) {
                path = '/';
            } else {
                //ss << path_;
                path = transform(pathParams_, path_);
            }
            
            return std::shared_ptr<HTTPClient>{
                new HTTPClient{host_, path, headers_, queryParams_}
            };
        }
        
    }
}
