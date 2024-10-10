add_executable (src 
"main.cpp"
"addressbook.pb.h"
"addressbook.pb.cc"
)

# 포함 디렉터리 설정
set(LIB_INCLUDE_DIR "../include")
include_directories ( ${LIB_INCLUDE_DIR} )

# LIB 디렉터리 설정 (lib를 찾을 path 설정)
set(LIB_DIR "../lib/Debug")
link_directories( ${LIB_DIR} )

# LIB 링크
set( LINK_LIBS libprotobufd )
link_libraries ( ${LINK_LIBS} )