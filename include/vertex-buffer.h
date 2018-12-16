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

    size_t data_size; 
    
    public:

    VertexBuffer(): id{std::make_shared<uint>(generate())} { }

    ~VertexBuffer() {
        if (id.use_count() == 1) {
            glDeleteBuffers(1, id.get());
        }
    }

    template<GLenum usage = GL_STATIC_DRAW, typename T>
    void data(gsl::span<T> data){
        glNamedBufferData(*id, data.size_bytes(), data.data(), usage);
        data_size = sizeof(T);
    }

    void bind(uint binding_index) const {
        glBindVertexBuffer(binding_index, *id, 0L, data_size);
    }

    uint get_id() const {
        return *id;
    }

    size_t get_data_size() const {
        return data_size;
    }

};
