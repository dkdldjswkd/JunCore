cmake_minimum_required(VERSION 3.10)
project(MyProtobufProject)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 14)

# Protobuf 찾기
find_package(Protobuf REQUIRED)

# Include 디렉토리 추가
include_directories(${PROTOBUF_INCLUDE_DIR})

# 생성된 protobuf 파일 경로 추가
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated)

# Protobuf 경로를 출력
message(STATUS "Protobuf include directory: ${PROTOBUF_INCLUDE_DIR}")
message(STATUS "Protobuf libraries: ${PROTOBUF_LIBRARIES}")
message(STATUS "Protobuf protoc executable: ${PROTOBUF_PROTOC_EXECUTABLE}")

# 서브 디렉토리 추가
add_subdirectory(src)