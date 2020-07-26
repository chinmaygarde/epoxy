// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include "code_gen.h"
#include "version.h"

#include <inja.hpp>
#include <sstream>

namespace epoxy {

CodeGen::CodeGen(std::string template_data)
    : template_data_(std::move(template_data)) {}

CodeGen::~CodeGen() = default;

static std::string GetEpoxyVersion() {
  std::stringstream stream;
  stream << EPOXY_VERSION_MAJOR << "." << EPOXY_VERSION_MINOR << "."
         << EPOXY_VERSION_PATCH;
  return stream.str();
}

static nlohmann::json CreateJSONTemplateData(
    const std::vector<Namespace>& namespaces) {
  nlohmann::json ns_data;
  ns_data["epoxy_version"] = GetEpoxyVersion();

  for (const auto& ns : namespaces) {
    ns_data["namespaces"].push_back(ns.GetJSONObject());
  }

  return ns_data;
}

static std::string TypeToDartFFIType(const std::string& type) {
  if (type == "void") {
    return "Void";
  }
  if (type == "int8_t") {
    return "Int8";
  }
  if (type == "int16_t") {
    return "Int16";
  }
  if (type == "int32_t") {
    return "Int32";
  }
  if (type == "int64_t") {
    return "Int64";
  }
  if (type == "uint8_t") {
    return "Uint8";
  }
  if (type == "uint16_t") {
    return "Uint16";
  }
  if (type == "uint32_t") {
    return "Uint32";
  }
  if (type == "uint64_t") {
    return "Uint64";
  }
  if (type == "double") {
    return "Double";
  }
  if (type == "float") {
    return "Float";
  }
  return "unknown";
}

static std::string TypeToDartType(const std::string& type) {
  if (type == "void") {
    return "void";
  }
  if (type == "int8_t") {
    return "int";
  }
  if (type == "int16_t") {
    return "int";
  }
  if (type == "int32_t") {
    return "int";
  }
  if (type == "int64_t") {
    return "int";
  }
  if (type == "uint8_t") {
    return "int";
  }
  if (type == "uint16_t") {
    return "int";
  }
  if (type == "uint32_t") {
    return "int";
  }
  if (type == "uint64_t") {
    return "int";
  }
  if (type == "double") {
    return "double";
  }
  if (type == "float") {
    return "double";
  }
  return "unknown";
}

CodeGen::RenderResult CodeGen::Render(
    const std::vector<Namespace>& namespaces) const {
  inja::Environment env;
  env.set_trim_blocks(true);
  env.set_lstrip_blocks(true);
  env.add_callback("dart_ffi_type", 1u, [](inja::Arguments& args) {
    return TypeToDartFFIType(args.at(0u)->get<std::string>());
  });
  env.add_callback("dart_type", 1u, [](inja::Arguments& args) {
    return TypeToDartType(args.at(0u)->get<std::string>());
  });
  try {
    auto render =
        env.render(template_data_.data(), CreateJSONTemplateData(namespaces));
    return {render, std::nullopt};
  } catch (std::exception e) {
    return {std::nullopt, e.what()};
  }
}

std::string CodeGen::GenerateTemplateDataJSON(
    const std::vector<Namespace>& namespaces) const {
  return CreateJSONTemplateData(namespaces).dump();
}

}  // namespace epoxy
