#include "Material.hpp"
#include "IL2CPPResolver/API/String.hpp"
namespace zr {
Shader *Material::getShader() { return CallMethod<Shader *>("get_shader"); }
void Material::setShader(Shader *shader) {
  CallMethod<void>("set_shader", shader);
}
void Material::setInt(const std::string &name, int value) {
  CallMethod<void>("SetInt", IL2CPP::String::New(name), value);
}
} // namespace zr