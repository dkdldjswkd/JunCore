# Boost 라이브러리 찾기
find_package(Boost REQUIRED)

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (AsioServer "main.cpp" )

# AsioServer에 해당 디렉터리들을 포함한다.
# PUBLIC : AsioServer을(를) 사용하는 프로젝트에도 포함된다.
target_include_directories(AsioServer 
    PUBLIC 
        ${Boost_INCLUDE_DIRS}
)

# AsioServer에 해당 라이브러리들을 링크한다.
# PUBLIC : AsioServer에서 링크한 라이브러리들이 AsioServer을(를) 사용하는 프로젝트에게도 링크된다.
target_link_libraries(AsioServer 
    PRIVATE 
        ${Boost_LIBRARIES}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AsioServer PROPERTY CXX_STANDARD 20)
endif()

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
