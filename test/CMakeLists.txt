set(SOURCES
	"test-input-manager.cc"
	"test-vertex-array.cc"
	"test-vertex-buffer.cc"
	"test-window.cc"
	"test-matrix.cc"
	"test-vec.cc"
)

add_executable(test-main test-main.cc ${SOURCES})

target_link_libraries(test-main
	PRIVATE
		gl-utils::gl-utils
		Catch2::Catch2
)

add_test(NAME test COMMAND test-main)