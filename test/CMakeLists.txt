set(SOURCES
	"test-gl-utils.cpp"
)

add_executable(test-main test-main.cpp ${SOURCES})

target_link_libraries(test-main
	PRIVATE
		gl-utils::gl-utils
		Catch2::Catch2
)

add_test(NAME test COMMAND test-main)