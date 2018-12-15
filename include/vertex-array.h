#pragma once
#include <GL/glew.h>
#include <gsl/span>
#include <memory>
#include <iostream>

class VertexArray {

    std::shared_ptr<uint const> id;

    uint generate() const {
        uint new_id;
        glGenVertexArrays(1, &new_id);
        return new_id;
    }

    public:

    VertexArray(): id{std::make_shared<uint>(generate())} { }

    ~VertexArray() {
        std::cout << " " << id.use_count() << std::endl;
        if (id.use_count() == 1) {
            glDeleteVertexArrays(1, id.get());
        }
    }

    void attrib_pointer(uint index, int size, GLenum type, bool normalized, size_t stride, void const* offset);

    void bind() {
        glBindVertexArray(*id);
    }

    uint get_id() const {
        return *id;
    }
};