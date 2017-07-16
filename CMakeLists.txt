# Set minimum version of CMake.
cmake_minimum_required(VERSION 3.8)

# Set variables:
#   * PROJECT_NAME
#   * PROJECT_VERSION
project(swarm-http-client VERSION 0.1.0)

# Include config
include(Config/config.cmake)

# Find Curl
find_package(CURL REQUIRED)

# Include sub projects
find_dependencies(cxx-log)
find_dependencies(swarm-commons)
find_dependencies(swarm-mapping)
find_dependencies(swarm-http-api)

# Create targets
add_library(swarm-http-client
    Sources/swarm/http/client/HTTPClient.cxx Sources/swarm/http/client/HTTPClient.hxx
    Sources/swarm/http/client/HTTPClientBuilder.cxx Sources/swarm/http/client/HTTPClientBuilder.hxx
    
    Sources/swarm/http/client/response/BodyResponse.cxx Sources/swarm/http/client/response/BodyResponse.hxx
    Sources/swarm/http/client/response/InMemoryBodyResponse.cxx Sources/swarm/http/client/response/InMemoryBodyResponse.hxx
    
    Sources/swarm/http/client/request/BodyRequest.cxx Sources/swarm/http/client/request/BodyRequest.hxx
    Sources/swarm/http/client/HTTPResult.cxx Sources/swarm/http/client/HTTPResult.hxx Sources/swarm/http/client/HTTPResult.txx
)

# Target includes
target_include_directories(
    swarm-http-client 
        
    PRIVATE
        ${cxx-log_INCLUDE_DIR}
        ${swarm-commons_INCLUDE_DIR}
        ${swarm-http-api_INCLUDE_DIR}
)

# Target links
target_link_libraries(
    swarm-http-client 
    cxx-log
    swarm-commons
    swarm-mapping
    swarm-http-api
    ${CURL_LIBRARIES}
)

# Include build
include(Config/build.cmake)

# Include test
add_subdirectory(Tests)
