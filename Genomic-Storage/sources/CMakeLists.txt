include_directories(. ${AZURESTORAGESAMPLES_INCLUDE_DIRS})

# THE ORDER OF FILES IS VERY /VERY/ IMPORTANT
if(UNIX)
  set(SOURCES
     genomic-storage.cpp 
     genomic-storage-main.cpp
    )
endif()

#buildsample(${AZURESTORAGESAMPLES_GENOMIC} ${SOURCES})
    add_executable(genomicstorage ${SOURCES})
    target_link_libraries(genomicstorage ${AZURESTORAGESAMPLES_LIBRARIES})

