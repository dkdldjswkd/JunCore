# Boost 라이브러리 찾기
find_package(Boost REQUIRED)

# shared 라이브러리 정의
add_library(shared 
 "Packets/ByteBuffer.cpp"
 "Networking/Socket.h"
 "Networking/SocketMgr.h"
 "Networking/NetworkThread.h"
 "Networking/AsyncAcceptor.h")

# shared.lib에 해당 디렉터리들을 포함한다.
# PUBLIC : shared.lib를 사용하는 프로젝트에도 포함된다. (ex. worldserver/main.cpp 에서 #include<Networking/...> 의 접근이 가능해진다.)
# PRIVATE : shared만 해당 디렉터리들을 포함한다.
target_include_directories(shared 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
		${Boost_INCLUDE_DIRS}
)

# shared.lib에 해당 라이브러리들을 링크한다.
# PUBLIC : shared.lib에서 링크한 라이브러리들이 shared.lib를 사용하는 프로젝트에게도 링크된다.
target_link_libraries(shared 
	PUBLIC 
		${Boost_LIBRARIES}
		common
)