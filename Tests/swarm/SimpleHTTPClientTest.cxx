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

#include <catch/catch.hxx>

#include <swarm/http/client/HTTPClient.hxx>
#include <swarm/http/client/HTTPClientBuilder.hxx>

using namespace swarm::http;

TEST_CASE("Simple Client", "[client]") {

    auto httpClient = HTTPClientBuilder{}
                          .host("http://localhost:9090")
                          .header("Accept", "application/json")
                          .method(HTTPMethod::GET)
                          .path("/auth/{name}/auth")
                          .queryParam("param1", "value1")
                          .queryParam("param2", "value2")
                          .pathParam("name", "MyName")
                          .build();

    REQUIRE_THROWS(httpClient->perform());
}

TEST_CASE("Error Client", "[client]") {

    HTTPClientBuilder httpClientBuilder{};

    REQUIRE_THROWS(httpClientBuilder.build());
    httpClientBuilder.host("http://localhost:9090");

    REQUIRE_NOTHROW(httpClientBuilder.build());
}

TEST_CASE("Client Test params", "[client]") {

    HTTPClientBuilder httpClientBuilder{};

    REQUIRE_THROWS(httpClientBuilder.build());
    httpClientBuilder.host("http://localhost").path("/swarm/client/params.php").queryParam("test", "param");

    auto httpClient = httpClientBuilder.build();
    httpClient->perform();
}

TEST_CASE("Client Test headers", "[client]") {

    HTTPClientBuilder httpClientBuilder{};

    REQUIRE_THROWS(httpClientBuilder.build());
    httpClientBuilder.host("http://localhost")
        .path("/swarm/client/headers.php")
        .header("header1", "value1")
        .header("header2", "value2");

    auto httpClient = httpClientBuilder.build();
    httpClient->perform();
}

TEST_CASE("Client Test method", "[client]") {

    HTTPClientBuilder httpClientBuilder{};

    REQUIRE_THROWS(httpClientBuilder.build());
    httpClientBuilder.host("http://localhost")
        .path("/swarm/client/method.php");

    auto httpClient = httpClientBuilder.method(HTTPMethod::GET).build();
    httpClient->perform();
    
    httpClient = httpClientBuilder.method(HTTPMethod::POST).build();
    httpClient->perform();
    
    httpClient = httpClientBuilder.method(HTTPMethod::PUT).build();
    httpClient->perform();
    
    httpClient = httpClientBuilder.method(HTTPMethod::DELETE).build();
    httpClient->perform();
    
    httpClient = httpClientBuilder.method(HTTPMethod::HEAD).build();
    httpClient->perform();
    
    httpClient = httpClientBuilder.method(HTTPMethod::OPTIONS).build();
    httpClient->perform();
    
    httpClient = httpClientBuilder.method(HTTPMethod::PATCH).build();
    httpClient->perform();
}
