#include "code_gen_cxx.h"

#include <inja.hpp>

namespace epoxy {

constexpr const char* kCXXCodeGenTemplate = R"~(
{% for ns in namespaces %}

#pragma once

namespace {{ ns.name }} {

{% for struct in ns.structs %}
typedef struct _{{ struct.name }} {
{% for var in struct.variables %}
  {{var.type}}{% if var.is_pointer %}* {% endif %} {{var.identifier}};
{% endfor %}
} {{ struct.name }};

{{ struct.name }}* {{struct.name}}Create();

void {{struct.name}}Destroy({{struct.name}}* object);
{% endfor %}


{% for func in ns.functions %}
{{func.return_type}}{% if func.pointer_return %}*{% endif %} {{func.name}}(
{% for arg in func.arguments %}
  {{arg.type}}{% if arg.is_pointer %}*{% endif %} {{ arg.identifier }},
{% endfor %}
);
{% endfor %}

} // namespace {{ ns.name }}

{% endfor %}

)~";

CodeGenCXX::CodeGenCXX() = default;

CodeGenCXX::~CodeGenCXX() = default;

// |CodeGen|
CodeGen::RenderResult CodeGenCXX::Render(
    std::vector<Namespace> namespaces) const {
  nlohmann::json template_data;

  for (const auto& ns : namespaces) {
    template_data["namespaces"].push_back(ns.GetJSONObject());
  }

  return {inja::render(kCXXCodeGenTemplate, template_data), std::nullopt};
}

}  // namespace epoxy
