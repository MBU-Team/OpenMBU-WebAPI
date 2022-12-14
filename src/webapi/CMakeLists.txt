cmake_minimum_required(VERSION 3.23)

project(OpenMBUWebAPI)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} SHARED "")
target_sources(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}../../../include/openmbu_webapi/account.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}../../../include/openmbu_webapi/status.hpp
    PRIVATE
        # Sources
        account.cpp
        util.cpp

        # Headers
        util.hpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC MBUAUTH_DLL)
target_compile_definitions(${PROJECT_NAME} PRIVATE MBUAUTH_BUILD)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../../../include)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}../../../libs/libcurl/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../../../libs/libcurl/lib)
target_compile_definitions(${PROJECT_NAME} PRIVATE CURL_STATICLIB)

if (CMAKE_CXX_SIZEOF_DATA_PTR EQUAL 8)
    target_link_libraries(${PROJECT_NAME} PUBLIC libcurl_a_x64.lib ws2_32.lib crypt32.lib Wldap32.Lib Normaliz.lib)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC libcurl_a_x86.lib ws2_32.lib crypt32.lib Wldap32.lib Normaliz.lib)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${ProjectOutDir}")
set_property(TARGET ${PROJECT_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${ProjectOutDir}")
if (CMAKE_CXX_SIZEOF_DATA_PTR EQUAL 8)
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_RELEASE ${PROJECT_NAME}64)
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_DEBUG ${PROJECT_NAME}64_DEBUG)
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_MINSIZEREL ${PROJECT_NAME}64_MINSIZEREL)
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_RELWITHDEBINFO ${PROJECT_NAME}64_RELWITHDEBINFO)
else()
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_RELEASE ${PROJECT_NAME})
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_DEBUG ${PROJECT_NAME}_DEBUG)
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_MINSIZEREL ${PROJECT_NAME}_MINSIZEREL)
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME_RELWITHDEBINFO ${PROJECT_NAME}_RELWITHDEBINFO)
endif()