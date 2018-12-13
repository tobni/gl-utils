#pragma once
#include <GL/glew.h>

#include <gsl/span>
#include <string>
#include <iostream>
#include <algorithm>
#include <optional>

#include "../include/common.h"

class VertexArrayObject {

    struct VertexBufferObject {

        template<typename T>
        VertexBufferObject(gsl::span<T> data, GLint components)
            : id{generate_and_bind()}, components{components}
        {
            glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), GL_STATIC_DRAW);
        }

        GLuint generate_and_bind() const {
            GLuint new_id;
            glGenBuffers(1, &new_id);
            glBindBuffer(GL_ARRAY_BUFFER, new_id);
            return new_id;
        }

        ~VertexBufferObject() {
            std::cout << "Removing buffer " << id << std::endl; 
            glDeleteBuffers(1, &id);
        }
        
        GLuint get_id() const {
            return id;
        }
        private:
        GLuint const id;
        GLint components;
    };

    GLuint id;
    GLuint index_buffer_id;

    RefMap<std::string const, VertexBufferObject> vertex_buffer_objects;

    GLuint generate_id() const;     
    public:

    VertexArrayObject();
    VertexArrayObject(VertexArrayObject&& other);
    ~VertexArrayObject();

    VertexArrayObject(VertexArrayObject const&) = delete;

    template<typename T>
    void add_buffer(gsl::span<T> data, GLint components, std::string const& name) {
        glBindVertexArray(id);
        vertex_buffer_objects.try_emplace(name, data, components);
        //glVertexAttribPointer
    }

    void add_index_buffer(gsl::span<GLuint> data);

    void remove_buffer(std::string const& name);

    void remove_index_buffer();

    GLuint get_id() const;

    std::optional<GLuint> get_buffer_id(std::string const& name) const;

    friend std::ostream& operator<<(std::ostream& os, const VertexArrayObject& vao);
};