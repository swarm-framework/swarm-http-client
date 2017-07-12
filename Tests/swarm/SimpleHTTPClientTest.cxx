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

#include <string>
#include <swarm/http/client/HTTPClient.hxx>
#include <swarm/http/client/HTTPClientBuilder.hxx>
#include <swarm/http/client/request/BodyRequest.hxx>
#include <swarm/http/client/HTTPResult.hxx>

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
    auto result = httpClient->perform();
    REQUIRE(result->status() == HTTPResponseStatus::OK);
}

TEST_CASE("Client Test headers", "[client]") {

    HTTPClientBuilder httpClientBuilder{};

    REQUIRE_THROWS(httpClientBuilder.build());
    httpClientBuilder.host("http://localhost")
        .path("/swarm/client/headers.php")
        .header("header1", "value1")
        .header("header2", "value2");

    auto httpClient = httpClientBuilder.build();
    auto result = httpClient->perform();
    REQUIRE(result->status() == HTTPResponseStatus::OK);
}

TEST_CASE("Client Test method", "[client]") {

    HTTPClientBuilder httpClientBuilder{};

    REQUIRE_THROWS(httpClientBuilder.build());
    httpClientBuilder.host("http://localhost").path("/swarm/client/method.php");

    auto httpClient = httpClientBuilder.method(HTTPMethod::GET).build();
    auto result1 = httpClient->perform();
    REQUIRE(result1->status() == HTTPResponseStatus::OK);
    REQUIRE("GET" == result1->body<std::string>());

    httpClient = httpClientBuilder.method(HTTPMethod::POST).build();
    auto result2 = httpClient->perform();
    REQUIRE(result2->status() == HTTPResponseStatus::OK);
    REQUIRE("POST" == result2->body<std::string>());

    httpClient = httpClientBuilder.method(HTTPMethod::PUT).build();
    auto result3 = httpClient->perform();
    REQUIRE(result3->status() == HTTPResponseStatus::OK);
    REQUIRE("PUT" == result3->body<std::string>());

    httpClient = httpClientBuilder.method(HTTPMethod::DELETE).build();
    auto result4 = httpClient->perform();
    REQUIRE(result4->status() == HTTPResponseStatus::OK);
    REQUIRE("DELETE" == result4->body<std::string>());

    //httpClient = httpClientBuilder.method(HTTPMethod::HEAD).build();
    //auto result5 = httpClient->perform();
    //REQUIRE(result5->status() == HTTPResponseStatus::OK);
    //REQUIRE("HEAD" == result5->body<std::string>());

    httpClient = httpClientBuilder.method(HTTPMethod::OPTIONS).build();
    auto result6 = httpClient->perform();
    REQUIRE(result6->status() == HTTPResponseStatus::OK);
    REQUIRE("OPTIONS" == result6->body<std::string>());

    httpClient = httpClientBuilder.method(HTTPMethod::PATCH).build();
    auto result7 = httpClient->perform();
    REQUIRE(result7->status() == HTTPResponseStatus::OK);
    REQUIRE("PATCH" == result7->body<std::string>());
}

TEST_CASE("Client Test POST data", "[client]") {

    HTTPClientBuilder httpClientBuilder{};

    REQUIRE_THROWS(httpClientBuilder.build());
    httpClientBuilder.host("http://localhost")
        .path("/swarm/client/post.php")
        .method(HTTPMethod::POST)
        .body(std::make_shared<StringBodyRequest>("Post data as string"));

    auto httpClient = httpClientBuilder.build();
    auto result = httpClient->perform();
    REQUIRE(result->status() == HTTPResponseStatus::OK);
    REQUIRE("Post data as string" == result->body<std::string>());
}
