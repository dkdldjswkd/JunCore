# Boost 라이브러리 찾기
find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)

 # C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# EchoClient 정의
add_executable(EchoClient 
 "main.cpp"
 "packet/packet.pb.cc"
 "network/EchoClient.cpp"
)

# PUBLIC : EchoClient를 사용하는 프로젝트에도 포함된다.
target_include_directories(EchoClient 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        common
        NetworkCore
        ${Protobuf_INCLUDE_DIRS}
)

# PUBLIC : EchoClient에서 링크한 라이브러리들이 EchoClient를 사용하는 프로젝트에게도 링크된다.
target_link_libraries(EchoClient 
    PRIVATE 
        ${Boost_LIBRARIES}
        common
        NetworkCore
        protobuf::libprotobuf
)
