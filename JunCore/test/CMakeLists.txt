# Boost 라이브러리 찾기
find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)

# C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(test 
    "main.cpp"
    "packet.pb.cc"
)

# Boost와 Protobuf의 헤더 파일 경로 추가
target_include_directories(test 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
)

# Boost와 Protobuf의 라이브러리 링크
target_link_libraries(test 
    PRIVATE 
    ${Boost_LIBRARIES} 
    protobuf::libprotobuf
)