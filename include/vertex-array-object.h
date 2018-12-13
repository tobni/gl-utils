#pragma once
#include <GL/glew.h>

#include <map>
#include <gsl/span>
#include <tuple>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

class VertexArrayObject {

    struct VertexBufferObject {

        template<typename T>
        void init_buffer(gsl::span<T> data, GLint components, VertexArrayObject*const vertex_array_object) {
            glBindVertexArray(vertex_array_object->id);
            glGenBuffers(1, &id);
            glBindBuffer(GL_ARRAY_BUFFER, id);
            glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), GL_STATIC_DRAW);
        }

        ~VertexBufferObject() {
            glDeleteBuffers(1, &id);
        }

        private:
        GLuint id = 0;
    };

    GLuint id = 0;
    GLuint index_buffer_id = 0;

    template<typename T>
    struct Less{

    bool operator()(T const& lhs, T const& rhs) const 
    {
        return lhs<rhs;
    }
    };

    std::map<std::reference_wrapper<std::string const>, VertexBufferObject, Less<std::string>> vertex_buffer_objects;
    
    public:

    VertexArrayObject() {
        glGenVertexArrays(1, &id);
    }

    VertexArrayObject(VertexArrayObject const&) = delete;
    VertexArrayObject(VertexArrayObject&& other)
        : id{other.id}, index_buffer_id{other.index_buffer_id}, vertex_buffer_objects{std::move(other.vertex_buffer_objects)}
    {
        other.id = 0;
        other.index_buffer_id = 0;
    }
    
    ~VertexArrayObject() {
        remove_index_buffer();
        glDeleteVertexArrays(1, &id);
    }

    template<typename T>
    void add_buffer(gsl::span<T> data, GLint components, std::string const& name) {
        vertex_buffer_objects.emplace(std::cref(name), VertexBufferObject());
        vertex_buffer_objects[std::cref(name)].init_buffer(data, components, this);
    }

    void add_index_buffer(gsl::span<GLuint> data) {
        glBindVertexArray(id);
        glGenBuffers(1, &index_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size_bytes(), data.data(), GL_STATIC_DRAW);
    }

    void remove_buffer(std::string const& name) {
        vertex_buffer_objects.erase(std::cref(name));
    }

    void remove_index_buffer() {
        glDeleteBuffers(1, &index_buffer_id);
    }

    GLuint const get_id() {
        return id;
    }

    friend std::ostream& operator<<(std::ostream& os, const VertexArrayObject& vao) {
        os << "id=" << vao.id << " buffers= [";
        char const* separator = "";
        for (auto const& kv : vao.vertex_buffer_objects) {
            auto const& name = kv.first; 
            os << separator << name.get();
            separator = ", ";
        }
        os << "]";
        return os;
    }
};