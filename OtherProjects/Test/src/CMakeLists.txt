# Protobuf 라이브러리 자동 링크
find_package(Protobuf REQUIRED)

# 실행 파일 생성
add_executable(src 
    "main.cpp"
    "addressbook.pb.cc"
)

# 포함 디렉터리 설정
target_include_directories(src 
    PRIVATE 
        ${Protobuf_INCLUDE_DIRS}
)

# Lib 링크
target_link_libraries(src 
    PRIVATE 
        protobuf::libprotobuf
)
