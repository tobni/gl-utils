#pragma once
#include <GL/glew.h>

#include <gsl/span>
#include <string>
#include <iostream>
#include <algorithm>
#include <optional>

#include "../include/common.h"

class VertexArray {
    int id;
    uint index_buffer_id;

    GLuint generate_id() const;     
    public:

    VertexArray();
    VertexArray(VertexArray&& other);
    VertexArray(VertexArray const& other);
    ~VertexArray();

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

    friend std::ostream& operator<<(std::ostream& os, const VertexArray& vao);
};