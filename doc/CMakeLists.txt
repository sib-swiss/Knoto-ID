
find_package(LATEX COMPONENTS PDFLATEX BIBTEX)

if(PDFLATEX_COMPILER)

SET( LATEX_FIGURES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/figures/")
SET( LATEX_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")

SET( texsourcemain "${CMAKE_CURRENT_SOURCE_DIR}/user_guide.tex" )
SET( bibsource  "${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib" )
SET( texsources "${CMAKE_CURRENT_SOURCE_DIR}/tutorial.tex" "${CMAKE_CURRENT_SOURCE_DIR}/reference.tex"  "${CMAKE_CURRENT_SOURCE_DIR}/theory.tex"  "${CMAKE_CURRENT_SOURCE_DIR}/bibliography.bib" )
SET( variablessource_in "${CMAKE_CURRENT_SOURCE_DIR}/variables_cmake.tex.in")
SET( variablessource "${CMAKE_CURRENT_BINARY_DIR}/variables_cmake.tex" )
SET( texoutput "${CMAKE_CURRENT_BINARY_DIR}/user_guide.pdf" )
get_filename_component(texmain ${texsourcemain} NAME_WE)

#figures
file(GLOB figures "${LATEX_FIGURES_DIR}/*pdf")

configure_file(${variablessource_in} ${variablessource} @ONLY)



ADD_CUSTOM_COMMAND( OUTPUT ${texoutput}
   COMMAND ${PDFLATEX_COMPILER}  -interaction=batchmode ${texsourcemain} && cp ${bibsource} "${CMAKE_CURRENT_BINARY_DIR}/" &&${BIBTEX_COMPILER} ${texmain} &&  ${PDFLATEX_COMPILER}  -interaction=batchmode ${texsourcemain} &&  ${PDFLATEX_COMPILER}  -interaction=batchmode ${texsourcemain}
   MAIN_DEPENDENCY ${texsourcemain}
   DEPENDS ${texsourcemain} ${texsources} ${variablessource} ${figures}
   COMMENT "Generating User Manual")


ADD_CUSTOM_TARGET( doc DEPENDS ${texoutput})
 



INSTALL( FILES ${texoutput} DESTINATION doc  OPTIONAL COMPONENT documentation)

else(PDFLATEX_COMPILER)
message(WARNING " WARNING pdflatex missing, target doc will not be available.")

endif(PDFLATEX_COMPILER)
 
  
