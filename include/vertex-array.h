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
        if (id.use_count() == 1) {
            glDeleteVertexArrays(1, id.get());
        }
    }

    void enable_attrib(uint attrib_index) {
        glEnableVertexArrayAttrib(*id, attrib_index);
    }

    void disable_attrib(uint attrib_index) {
        glDisableVertexArrayAttrib(*id, attrib_index);
    }

    void format_attrib(uint binding_index, int size, GLenum type, bool normalized, uint relative_offset) {
        glVertexArrayAttribFormat(*id, binding_index, size, type, normalized, relative_offset);
    }

    void bind_attrib(uint attrib_index, uint binding_index) {
        glVertexArrayAttribBinding(*id, attrib_index, binding_index);
    }

    void bind() {
        glBindVertexArray(*id);
    }

    uint get_id() const {
        return *id;
    }
};