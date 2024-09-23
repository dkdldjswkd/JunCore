# common 라이브러리 정의
add_library(common
 "Common.h"
 "Common.cpp"
 "Define.h"
 "Utilities/MessageBuffer.h")

# common에 해당 디렉터리들을 포함한다.
# PUBLIC : common을(를) 사용하는 프로젝트에도 포함된다.
target_include_directories(common 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
)