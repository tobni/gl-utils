#pragma once
#include <GL/glew.h>
#include <gsl/span>
#include <memory>

class VertexBuffer {

    std::shared_ptr<uint const> id;

    uint generate() const {
        uint new_id;
        glGenBuffers(1, &new_id);
        return new_id;
    }
    
    public:

    VertexBuffer(): id{std::make_shared(generate())} { }

    ~VertexBuffer() {
        if (id.use_count() == 1) {
            glDeleteBuffers(1, id.get());
        }
    }

    template<GLenum usage = GL_STATIC_DRAW, typename T>
    void data(gsl::span<T> data) const {
        glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    }

    void bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id.get());
    }

    uint get_id() const {
        return *id;
    }

};
