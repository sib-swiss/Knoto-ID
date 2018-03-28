
#INSTALL( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ./ )

file(GLOB example_files "${CMAKE_CURRENT_SOURCE_DIR}/*[^~]" )

#remove CMakeLists.txt from list
list(REMOVE_ITEM example_files "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

#MESSAGE( "example_files= ${example_files} ")
INSTALL( FILES ${example_files} DESTINATION ./examples  COMPONENT documentation)
