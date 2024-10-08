# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (ProtobufTest 
"main.cpp" 
)

include_directories ( include protobuf)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WinServer PROPERTY CXX_STANDARD 20)
endif()
