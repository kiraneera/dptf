if (IN_SOURCE_BUILD MATCHES YES)                                                                                                                                                                                                                                        
        include_directories(../)
        include_directories(../../../../Common)
        include_directories(../../ThirdParty)
		include_directories(../../SharedLib/BasicTypesLib)
		include_directories(../../SharedLib/XmlLib)
		include_directories(../../SharedLib/DptfTypesLib)
		include_directories(../../SharedLib/DptfObjectsLib)
        file(GLOB_RECURSE participant_controls_lib_SOURCES "*.cpp")
else ()
        include_directories(../../../Sources)
        include_directories(../../../../Common)
        include_directories(../../../Sources/ThirdParty)
		include_directories(../../../Sources/SharedLib/BasicTypesLib)
		include_directories(../../../Sources/SharedLib/XmlLib)
		include_directories(../../../Sources/SharedLib/DptfTypesLib)
		include_directories(../../../Sources/SharedLib/DptfObjectsLib)
        file(GLOB_RECURSE participant_controls_lib_SOURCES "../../../Sources/SharedLib/ParticipantControlsLib/*.cpp")
endif()

add_library(${PARTICIPANT_CONTROLS_LIB} STATIC ${participant_controls_lib_SOURCES})

set_target_properties(${PARTICIPANT_CONTROLS_LIB} PROPERTIES PREFIX "")
