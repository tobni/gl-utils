set(SOURCES
	"test-gl-utils.cc"
)

add_executable(test-main test-main.cc ${SOURCES})

target_link_libraries(test-main
	PRIVATE
		gl-utils::gl-utils
		Catch2::Catch2
)

add_test(NAME test COMMAND test-main)