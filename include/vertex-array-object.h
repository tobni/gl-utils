#pragma once
#include <GL/glew.h>

#include <map>
#include <gsl/span>
#include <tuple>
#include <string>
#include <iostream>

class VertexArrayObject {

    struct VertexBufferObject {

        template<typename T>
        void init_buffer(gsl::span<T> data, GLint components, VertexArrayObject* vertex_array_object) {
            glBindVertexArray(vertex_array_object->id);
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), GL_STATIC_DRAW);
        }

        ~VertexBufferObject() {
            glDeleteBuffers(1, &id);
        }

        private:
        GLuint id;
    };


    GLuint id;
    GLuint index_buffer_object;
    std::map<std::string, VertexBufferObject> vertex_buffer_objects;
    
    public:

    VertexArrayObject() {
        glGenVertexArrays(1, &id);
    }
    
    template<typename T>
    void add_buffer(gsl::span<T> data, GLint components, std::string name) {
        vertex_buffer_objects[name] = VertexBufferObject();
        vertex_buffer_objects[name].init_buffer(data, components, this);
    }

    void remove_buffer(std::string const& name) {
        vertex_buffer_objects.erase(name);
    }

    void print_buffers() {
        for(auto const& kv : vertex_buffer_objects) {
            std::cout << kv.first << " ";
        }
        std::cout << std::endl;
    }
};