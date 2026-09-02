cmake_minimum_required(VERSION 3.20)

project(online_judge VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.3.3
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(Crow nlohmann_json)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(online_judge
    ${SOURCES}
)

add_executable(blocking_queue_test
    tests/BlockingQueueTest.cpp
)

target_include_directories(blocking_queue_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(worker_pool_test
    tests/WorkerPoolTest.cpp
    src/worker/WorkerPool.cpp
)

add_executable(execution_repository_test
    tests/ExecutionRepositoryTest.cpp
    src/repository/ExecutionRepository.cpp
)

target_include_directories(execution_repository_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(execution_repository_test PRIVATE
    nlohmann_json::nlohmann_json
)

target_include_directories(worker_pool_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(judge_test
    tests/JudgeTest.cpp
    src/judge/Judge.cpp
    src/repository/SubmissionRepository.cpp
    src/repository/ExecutionRepository.cpp
    src/utils/SubmissionStatus.cpp
)

target_include_directories(judge_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(judge_test PRIVATE
    nlohmann_json::nlohmann_json
)

target_include_directories(online_judge PRIVATE
    include
)

target_link_libraries(online_judge PRIVATE
    Crow::Crow
    nlohmann_json::nlohmann_json
)