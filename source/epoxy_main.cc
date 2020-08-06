// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

#include <iostream>

#include "code_gen.h"
#include "command_line.h"
#include "driver.h"
#include "file.h"
#include "sema.h"
#include "version.h"

namespace epoxy {

constexpr const char* kHelpString = R"~(
Epoxy FFI Bindings Code Generator
=================================

Usage:
------

    epoxy  --output <output file path>
           --idl    <Epoxy IDL file path>
           [--template-file <Template File Path>]
           [--template-data-dump]
           [--help]
           [--version]

Options:
--------

  --output            The path to the file containing the Code generated for the
                      given options. Optional if if --template-data-dump is
                      specified.

  --idl               The path the Epoxy IDL file.

  --template-file     The path to a custom code generation template. To
                      introspect the data used to render the template, use the
                      --template-data-dump option. The Inja template rendering
                      system is used to render the template data.

  --template-data-dump
                      Instead of rendering the code generation template, dump
                      the template data. This is useful when writing or
                      customizing a custom code generation template.

  --help              Dump these help instructions.

  --version           Get the Epoxy version.
)~";

static void DumpHelpString(std::ostream& stream) {
  stream << kHelpString << std::endl;
}

struct FileInfo {
  std::string file_name;
  std::string file_contents;
};

static std::optional<FileInfo> GetTemplateData(const CommandLine& args) {
  auto template_file_flag = args.GetString("template-file");

  if (!template_file_flag.has_value()) {
    std::cerr << "No flag specified for the code generation template. Use the "
                 "template-file flag."
              << std::endl;
    return std::nullopt;
  }

  auto template_file_data = ReadFileAsString(template_file_flag.value());
  if (!template_file_data.has_value()) {
    std::cerr << "Could not read " << template_file_flag.value()
              << " to obtain code generation template data." << std::endl;
    return std::nullopt;
  }
  return FileInfo{template_file_flag.value(), template_file_data.value()};
}

static std::optional<FileInfo> GetIDLData(const CommandLine& args) {
  auto idl_flag = args.GetString("idl");

  if (!idl_flag.has_value()) {
    std::cerr << "-idl flag not specified." << std::endl;
    return std::nullopt;
  }

  auto idl_data = ReadFileAsString(idl_flag.value());
  if (!idl_data.has_value()) {
    std::cerr << "Could not read IDL data from file at path "
              << idl_flag.value() << std::endl;
    return std::nullopt;
  }

  return FileInfo{idl_flag.value(), idl_data.value()};
}

bool Main(const CommandLine& args) {
  if (auto help = args.GetOption("help"); help.has_value() && help.value()) {
    DumpHelpString(std::cout);
    return true;
  }

  if (auto version = args.GetOption("version");
      version.has_value() && version.value()) {
    std::cout << EPOXY_VERSION_MAJOR << "." << EPOXY_VERSION_MINOR << "."
              << EPOXY_VERSION_PATCH << std::endl;
    return true;
  }

  const auto template_data = GetTemplateData(args);

  if (!template_data.has_value()) {
    std::cerr << "Could not figure out which template to render." << std::endl;
    return false;
  }

  auto idl_data = GetIDLData(args);
  if (!idl_data.has_value()) {
    std::cerr << "Could not figure out the IDL to parse." << std::endl;
    return false;
  }

  Driver driver(idl_data.value().file_name);
  const auto parse_result = driver.Parse(idl_data.value().file_contents);
  if (parse_result != Driver::ParserResult::kSuccess) {
    std::cerr << "Errors when attempting to parse IDL: " << std::endl;
    driver.PrettyPrintErrors(std::cerr, idl_data.value().file_contents);
    return false;
  }

  Sema sema;
  const auto sema_result = sema.Perform(driver.GetNamespaces());
  if (sema_result != Sema::Result::kSuccess) {
    std::cerr << "Errors in interface definition: ";
    sema.PrettyPrintErrors(std::cerr);
    return false;
  }

  CodeGen code_gen(template_data.value().file_contents);

  auto dump_template_data_flag = args.GetOption("template-data-dump");
  if (dump_template_data_flag.has_value() && dump_template_data_flag.value()) {
    std::cout << code_gen.GenerateTemplateDataJSON(sema.GetNamespaces())
              << std::endl;
    return true;
  }

  auto code_gen_result = code_gen.Render(sema.GetNamespaces());
  if (code_gen_result.error.has_value()) {
    std::cerr << "Errors during code generation: " << std::endl
              << code_gen_result.error.value() << std::endl;
    return false;
  }

  if (!code_gen_result.result.has_value()) {
    std::cerr << "Code generation failed." << std::endl;
    return false;
  }

  auto out_file_flag = args.GetString("output");
  if (!out_file_flag.has_value()) {
    std::cerr << "Output file path not specified. Specify the save via the "
                 "--output flag."
              << std::endl;
    return false;
  }

  if (!OverwriteFileWithStringData(out_file_flag.value(),
                                   code_gen_result.result.value())) {
    std::cerr << "Error while writing the output to file at path: "
              << out_file_flag.value() << std::endl;
    return false;
  }

  return true;
}

}  // namespace epoxy

int main(int argc, const char* argv[]) {
  return epoxy::Main({argc, argv}) ? EXIT_SUCCESS : EXIT_FAILURE;
}
