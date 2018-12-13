#include "../include/vertex-array-object.h"

GLuint VertexArrayObject::generate_id() const {
    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}

VertexArrayObject::VertexArrayObject()
    : id{generate_id()}, index_buffer_id{0} 
{}

VertexArrayObject::VertexArrayObject(VertexArrayObject&& other)
    : id{other.id}, index_buffer_id{other.index_buffer_id}, vertex_buffer_objects{std::move(other.vertex_buffer_objects)}
{
    other.id = 0;
    other.index_buffer_id = 0;
}
    
VertexArrayObject::~VertexArrayObject() {
    remove_index_buffer();
    std::cout << "Removing " << id << std::endl; 
    glDeleteVertexArrays(1, &id);
}

void VertexArrayObject::add_index_buffer(gsl::span<GLuint> data) {
    glBindVertexArray(id);
    glGenBuffers(1, &index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size_bytes(), data.data(), GL_STATIC_DRAW);
}

void VertexArrayObject::remove_buffer(std::string const& name) {
    vertex_buffer_objects.erase(std::cref(name));
}

void VertexArrayObject::remove_index_buffer() {
    glDeleteBuffers(1, &index_buffer_id);
}


GLuint VertexArrayObject::get_id() const {
    return id;
}

std::optional<GLuint> VertexArrayObject::get_buffer_id(std::string const& name) const {
    if (vertex_buffer_objects.count(name)) {
        return vertex_buffer_objects.at(name).get_id();
    } else {
        return  { };
    }
}

std::ostream& operator<<(std::ostream& os, const VertexArrayObject& vao) {
    os << "id=" << vao.id << " buffers= [";
    char const* separator = "";
    for (auto const& kv : vao.vertex_buffer_objects) {
        auto const& name = kv.first; 
        auto const& buffer = kv.second; 
        os << separator << name.get();
        os << ':' << buffer.get_id();
        separator = ", ";
    }
    os << "]";
    return os;
}