# #############################################################################
# DownloadAndExtract.cmake - Download packages from ares-external
# #############################################################################
#
# Provides functions to download and extract pre-built packages from the
# ares-external GitHub repository.
#
# #############################################################################

# Generate package archive filename
# Format: <package>-<version>-<compiler><version>-release-<os>-<os_version>.tar.gz
function(generate_package_archive_filename PACKAGE_NAME VERSION OUTPUT_VAR)
    set(ARCHIVE_NAME "${PACKAGE_NAME}-${VERSION}-${MLBDEV2_COMPILER}${MLBDEV2_COMPILER_VERSION}-release-${MLBDEV2_OS_NAME}-${MLBDEV2_OS_VERSION}.tar.gz")
    set(${OUTPUT_VAR} "${ARCHIVE_NAME}" PARENT_SCOPE)
endfunction()

# Download and extract a package from ares-external
#
# Usage:
#   get_from_ares_external(
#       PACKAGE_NAME "boost"
#       PACKAGE_VERSION "1.83.0"
#       HASH "sha256hash..."
#       OUTPUT_DIR <variable_name>
#   )
function(get_from_ares_external PACKAGE_NAME PACKAGE_VERSION HASH OUTPUT_DIR)
    set(PACKAGE_NAME_VERSION "${PACKAGE_NAME}-${PACKAGE_VERSION}")
    generate_package_archive_filename("${PACKAGE_NAME}" "${PACKAGE_VERSION}" ARCHIVE_FILENAME)
    
    set(ARCHIVE_DIR "${EXTERNAL_DOWNLOADS_DIR}/${PACKAGE_NAME}")
    set(ARCHIVE_PATH "${ARCHIVE_DIR}/${ARCHIVE_FILENAME}")
    set(EXTRACTED_DIR "${ARCHIVE_DIR}/${PACKAGE_NAME_VERSION}")
    
    # Return the extracted directory path
    set(${OUTPUT_DIR} "${EXTRACTED_DIR}" PARENT_SCOPE)
    
    message(VERBOSE "get_from_ares_external: ${PACKAGE_NAME} ${PACKAGE_VERSION}")
    message(VERBOSE "  Archive: ${ARCHIVE_FILENAME}")
    message(VERBOSE "  Extract to: ${EXTRACTED_DIR}")
    
    # Check if already extracted
    if(EXISTS "${EXTRACTED_DIR}")
        message(STATUS "${PACKAGE_NAME_VERSION} already available at ${EXTRACTED_DIR}")
        return()
    endif()
    
    # Create download directory
    file(MAKE_DIRECTORY "${ARCHIVE_DIR}")
    
    # Check if archive exists and has non-zero size
    set(NEED_DOWNLOAD TRUE)
    if(EXISTS "${ARCHIVE_PATH}")
        file(SIZE "${ARCHIVE_PATH}" ARCHIVE_SIZE)
        if(ARCHIVE_SIZE GREATER 0)
            set(NEED_DOWNLOAD FALSE)
        else()
            file(REMOVE "${ARCHIVE_PATH}")
        endif()
    endif()
    
    if(NEED_DOWNLOAD)
        # Try GitHub CLI first (works for private repos)
        find_program(GH_CLI gh)
        if(GH_CLI)
            message(STATUS "Downloading ${ARCHIVE_FILENAME} via GitHub CLI...")
            execute_process(
                COMMAND ${GH_CLI} release download "${PACKAGE_NAME_VERSION}"
                    -R dare-global/ares-external
                    -D "${ARCHIVE_DIR}"
                    --pattern "${ARCHIVE_FILENAME}"
                RESULT_VARIABLE GH_RESULT
                ERROR_VARIABLE GH_ERROR
                OUTPUT_QUIET
            )
            if(NOT GH_RESULT EQUAL 0)
                message(WARNING "GitHub CLI download failed: ${GH_ERROR}")
            endif()
        endif()
        
        # Check if gh download succeeded
        if(EXISTS "${ARCHIVE_PATH}")
            file(SIZE "${ARCHIVE_PATH}" ARCHIVE_SIZE)
            if(ARCHIVE_SIZE EQUAL 0)
                file(REMOVE "${ARCHIVE_PATH}")
            endif()
        endif()
        
        # Fall back to HTTPS download if gh failed
        if(NOT EXISTS "${ARCHIVE_PATH}")
            set(DOWNLOAD_URL "${ARES_EXTERNAL_REPO_URL}/releases/download/${PACKAGE_NAME_VERSION}/${ARCHIVE_FILENAME}")
            message(STATUS "Downloading ${ARCHIVE_FILENAME} via HTTPS...")
            message(VERBOSE "  URL: ${DOWNLOAD_URL}")
            
            file(DOWNLOAD
                "${DOWNLOAD_URL}"
                "${ARCHIVE_PATH}"
                SHOW_PROGRESS
                EXPECTED_HASH SHA256=${HASH}
                STATUS DOWNLOAD_STATUS
            )
            
            list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
            list(GET DOWNLOAD_STATUS 1 STATUS_MESSAGE)
            
            if(NOT STATUS_CODE EQUAL 0)
                message(FATAL_ERROR "Failed to download ${ARCHIVE_FILENAME}\n"
                    "  URL: ${DOWNLOAD_URL}\n"
                    "  Error: ${STATUS_MESSAGE}\n"
                    "  Ensure you have access to ares-external repo")
            endif()
        endif()
    endif()
    
    # Extract archive
    message(STATUS "Extracting ${ARCHIVE_FILENAME}...")
    file(ARCHIVE_EXTRACT INPUT "${ARCHIVE_PATH}" DESTINATION "${ARCHIVE_DIR}")
    
    if(NOT EXISTS "${EXTRACTED_DIR}")
        message(FATAL_ERROR "Extraction failed: ${EXTRACTED_DIR} not found after extraction")
    endif()
    
    message(STATUS "${PACKAGE_NAME_VERSION} installed to ${EXTRACTED_DIR}")
endfunction()
