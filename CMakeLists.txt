# Top-level module CMakeLists for zmk-config-klor.
#
# This repo is both a ZMK user config (manifest at config/west.yml, shields at
# config/boards/shields/) AND a Zephyr module providing the ST7789V display
# driver amendment, event types, and behaviors used by the klor_dongle_screen
# shield. The dual role is enabled by:
#   1. zephyr/module.yml at the repo root (read by ZMK's build-user-config
#      workflow, which then sets ZMK_EXTRA_MODULES=<repo-root>).
#   2. ZMK_CONFIG=<repo>/config (default) adds config/ as a BOARD_ROOT for
#      shield discovery.
#
# Everything below is module-side: it only fires when the klor_dongle_screen
# shield is selected, i.e. the dongle build. Halves builds load this file too
# but the if-guard skips the bodies.

zephyr_include_directories(include)
zephyr_include_directories(${APPLICATION_SOURCE_DIR}/include)
zephyr_library()

if(CONFIG_SHIELD_KLOR_DONGLE_SCREEN)

        # Pull in the ST7789V driver amendment (drivers/display/CMakeLists.txt
        # uses zephyr_library_amend() to override Zephyr's stock driver).
        add_subdirectory(${ZEPHYR_CURRENT_MODULE_DIR}/drivers/display)

        # Caps-word state change event + behavior override. Only compiled if
        # the caps_word behavior is actually present in the build (it is, on
        # the dongle, because the shared keymap uses &caps_word).
        if(CONFIG_DT_HAS_ZMK_BEHAVIOR_CAPS_WORD_ENABLED)
                zephyr_library_sources(src/events/caps_word_state_changed.c)
                set_source_files_properties(
                        ${APPLICATION_SOURCE_DIR}/src/behaviors/behavior_caps_word.c
                        TARGET_DIRECTORY app
                        PROPERTIES HEADER_FILE_ONLY ON)
                target_sources(app PRIVATE src/behaviors/behavior_caps_word.c)
        endif()

        # Split-central status change event + observer (used by the layouts
        # to know when each half connects/disconnects from the dongle).
        zephyr_library_sources(src/events/split_central_status_changed.c)
        zephyr_library_sources(src/split/bluetooth/central_status_changed_observer.c)

endif()
