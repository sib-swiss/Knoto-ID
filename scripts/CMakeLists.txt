

INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/plot_projection_map.R DESTINATION scripts PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ  GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE  COMPONENT applications)
INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/plot_knotted_core.R DESTINATION scripts PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ  GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE COMPONENT applications)
INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/plot_diagram.R DESTINATION scripts PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ  GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE COMPONENT applications)
INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/pdb_to_xyz.R DESTINATION scripts PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ  GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE COMPONENT applications)
INSTALL( FILES ${CMAKE_CURRENT_SOURCE_DIR}/plot_3D_curve.R DESTINATION scripts PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ  GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE COMPONENT applications)


