#pragma once
#include <GL/glew.h>
#include <gsl/span>
#include <iostream>
#include <common.h>

struct VertexBuffer {

    VertexBuffer(): name_reference{generate()} {}

    template<typename T>
    void data(gsl::span<T> data, GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW) const {
        bind(target);
        glBufferData(target, data.size_bytes(), data.data(), usage);
    }

    uint generate() const {
        uint new_name;
        glGenBuffers(1, &new_name);
        return new_name;
    }

    void bind(GLenum target = GL_ARRAY_BUFFER) const {
        glBindBuffer(target, name_reference);
    }

    ~VertexBuffer() {
        if (name_reference.use_count() == 1) {
            glDeleteBuffers(1, name_reference);
        }
    }
    
    uint get_id() const {
        return *name_reference;
    }

    private:
    RefCnt<uint const> name_reference;
};
