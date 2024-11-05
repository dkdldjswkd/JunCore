# 개발환경 대응 cmake

# 라이브러리 찾기
find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)

# C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# EchoClient 정의
add_executable(EchoClient 
    "main.cpp"
    "packet/packet.pb.cc"
    "network/EchoClient.cpp"
)

if(MSVC) # Windows
    link_directories(${LIB_DIR})
    set(LINK_LIBS libprotobufd)
    link_libraries(${LINK_LIBS})
endif()

# PUBLIC : EchoClient를 사용하는 프로젝트에도 포함된다.
target_include_directories(EchoClient 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${LIB_INCLUDE_DIR}
        ${Protobuf_INCLUDE_DIRS}
        ${Abseil_INCLUDE_DIRS}
        common
        NetworkCore
)

# PUBLIC : EchoClient에서 링크한 라이브러리들이 EchoClient를 사용하는 프로젝트에게도 링크된다.
target_link_libraries(EchoClient 
    PRIVATE 
        ${Boost_LIBRARIES}
        common
        NetworkCore
        protobuf::libprotobuf
        absl::base
)