# 라이브러리 찾기
find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)

# C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# echo_server 정의
add_executable(echo_server 
    "main.cpp"
    "packet/packet.pb.cc"
    "network/EchoServer.cpp"
)

# PUBLIC : echo_server를 사용하는 프로젝트에도 포함된다.
target_include_directories(echo_server 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        network_core
        ${Protobuf_INCLUDE_DIRS}
)

# PUBLIC : echo_server에서 링크한 라이브러리들이 echo_server를 사용하는 프로젝트에게도 링크된다.
target_link_libraries(echo_server 
    PRIVATE 
        ${Boost_LIBRARIES}
        network_core
        protobuf::libprotobuf
)
