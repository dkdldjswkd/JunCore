# Boost 라이브러리 찾기
find_package(Boost REQUIRED)

# game 라이브러리 정의
add_library(game 
 "Server/WorldSession.h"
 "Server/WorldSession.cpp"
 "Server/WorldSocket.h"
 "Server/WorldSocket.cpp"
 "Server/WorldSocketMgr.h"
 "Server/WorldSocketMgr.cpp"
 "Server/WorldPacket.h")

# game에 해당 디렉터리들을 포함한다.
# PUBLIC : game을(를) 사용하는 프로젝트에도 포함된다.
target_include_directories(game 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
)

# game에 해당 라이브러리들을 링크한다.
# PUBLIC : game에서 링크한 라이브러리들이 game을(를) 사용하는 프로젝트에게도 링크된다.
target_link_libraries(game 
    PUBLIC 
        ${Boost_LIBRARIES}
        common
        shared
)
