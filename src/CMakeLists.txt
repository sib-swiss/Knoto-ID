include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ ${Boost_INCLUDE_DIR} ${PROJECT_BINARY_DIR} )
set(LIBRARIES ${Boost_LIBRARIES} )

set(CMAKE_VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION})

configure_file (  "${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in"  "${PROJECT_BINARY_DIR}/Version.h"  )


add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/KnotoidNames.cpp
  COMMAND ${CMAKE_COMMAND}  -DDEST=${PROJECT_BINARY_DIR} -DSOURCE=${PROJECT_SOURCE_DIR}/examples  -P  ${PROJECT_SOURCE_DIR}/src/configure_knotoid_names.cmake
  DEPENDS ${PROJECT_SOURCE_DIR}/examples/knot_names.txt
          ${PROJECT_SOURCE_DIR}/examples/knotoid_names_planar.txt
          ${PROJECT_SOURCE_DIR}/examples/knotoid_names_planar_arrow.txt
          ${PROJECT_SOURCE_DIR}/examples/knotoid_names_sphere.txt
          ${PROJECT_SOURCE_DIR}/examples/knotoid_names_sphere_arrow.txt
  COMMENT "Configuring KnotoidNames.cpp")


if( "${CMAKE_VERSION}" VERSION_LESS "2.8.8")
   add_executable(polynomial_invariant main.cpp Polygon.cpp PlanarDiagram.cpp PlanarGraph.cpp PolynomialInvariant.cpp Polynomial.cpp Random.cpp ${PROJECT_BINARY_DIR}/KnotoidNames.cpp)
   add_executable(knotted_core main_knotted_core.cpp Polygon.cpp PlanarDiagram.cpp PlanarGraph.cpp PolynomialInvariant.cpp Polynomial.cpp Random.cpp ${PROJECT_BINARY_DIR}/KnotoidNames.cpp)
   add_executable(convert_diagram main_convert.cpp Polygon.cpp PlanarDiagram.cpp PlanarGraph.cpp Random.cpp ${PROJECT_BINARY_DIR}/KnotoidNames.cpp)
else("${CMAKE_VERSION}" VERSION_LESS "2.8.8")
   add_library(polygon_objects OBJECT Polygon.cpp)
   add_library(planardiagram_objects OBJECT PlanarDiagram.cpp)
   add_library(planargraph_objects OBJECT PlanarGraph.cpp)
   add_library(polynomialinvariant_objects OBJECT PolynomialInvariant.cpp)
   add_library(polynomial_objects OBJECT Polynomial.cpp)
   add_library(random_objects OBJECT Random.cpp)
   add_library(knotoidnames_objects OBJECT ${PROJECT_BINARY_DIR}/KnotoidNames.cpp)
   add_executable(polynomial_invariant main.cpp $<TARGET_OBJECTS:polygon_objects> $<TARGET_OBJECTS:planardiagram_objects> $<TARGET_OBJECTS:planargraph_objects>  $<TARGET_OBJECTS:polynomialinvariant_objects>  $<TARGET_OBJECTS:polynomial_objects>  $<TARGET_OBJECTS:random_objects> $<TARGET_OBJECTS:knotoidnames_objects>)
   add_executable(knotted_core main_knotted_core.cpp $<TARGET_OBJECTS:polygon_objects> $<TARGET_OBJECTS:planardiagram_objects> $<TARGET_OBJECTS:planargraph_objects>  $<TARGET_OBJECTS:polynomialinvariant_objects>  $<TARGET_OBJECTS:polynomial_objects>  $<TARGET_OBJECTS:random_objects> $<TARGET_OBJECTS:knotoidnames_objects> )
   add_executable(convert_diagram main_convert.cpp $<TARGET_OBJECTS:polygon_objects> $<TARGET_OBJECTS:planardiagram_objects> $<TARGET_OBJECTS:planargraph_objects>  $<TARGET_OBJECTS:random_objects> $<TARGET_OBJECTS:knotoidnames_objects> )
endif("${CMAKE_VERSION}" VERSION_LESS "2.8.8" )

target_link_libraries(polynomial_invariant ${LIBRARIES})
install (TARGETS polynomial_invariant DESTINATION bin COMPONENT applications)


target_link_libraries(knotted_core ${LIBRARIES})
install (TARGETS knotted_core DESTINATION bin COMPONENT applications)

target_link_libraries(convert_diagram ${LIBRARIES})
install (TARGETS convert_diagram DESTINATION bin COMPONENT applications)

