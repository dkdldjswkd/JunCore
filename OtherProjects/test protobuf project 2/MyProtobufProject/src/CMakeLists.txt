# 바이너리 파일의 이름 설정
add_executable(src 
main.cpp 
../generated/addressbook.pb.cc
)

# Protobuf 라이브러리 링크
TARGET_LINK_LIBRARIES(src
        PUBLIC
            ${PROTOBUF_LIBRARIES}
)

# Protobuf 파일 생성 명령어 (Optional)
# 아래 명령어는 cmake로 직접 .proto 파일을 컴파일하도록 설정합니다.
# 주의: Windows 환경에서 protoc와 protobuf 라이브러리 경로를 올바르게 설정해야 합니다.
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../proto/addressbook.proto)
# add_executable(MyProtobufApp main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
