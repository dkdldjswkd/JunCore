# Boost 라이브러리 찾기
find_package(Boost REQUIRED)

 # C++17 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# common 라이브러리 정의
add_library(common
 "Common.h"
 "Common.cpp"
 "Define.h"
 "Utilities/MessageBuffer.h"
 "ring_buffer.h"
 "ring_buffer.cpp"
)

# common에 해당 디렉터리들을 포함한다.
# PUBLIC : common을(를) 사용하는 프로젝트에도 포함된다.
target_include_directories(common 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
		${Boost_INCLUDE_DIRS}
)

# common.lib에 해당 라이브러리들을 링크한다.
# PUBLIC : common.lib에서 링크한 라이브러리들이 common.lib를 사용하는 프로젝트에게도 링크된다.
target_link_libraries(common 
	PUBLIC 
		${Boost_LIBRARIES}
        # ${LIB_DIR}/libboost_date_time-vc142-mt-gd-x64-1_76.lib # 개발 환경 대응
)