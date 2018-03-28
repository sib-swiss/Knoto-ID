#find all files in tests/bin (not ending in ~)
file(GLOB test_scripts "${CMAKE_CURRENT_SOURCE_DIR}/bin/*[^~]" )

FOREACH (test_script ${test_scripts})
	get_filename_component(test_script_name ${test_script} NAME)
	add_test(NAME ${test_script_name} COMMAND ${test_script}  ${CMAKE_CURRENT_SOURCE_DIR}) 
ENDFOREACH(test_script)

