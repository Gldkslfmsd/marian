#include "common/loader_factory.h"

#include "common/scorer.h"
#include "common/logging.h"

#ifdef HAS_CPU
#include "cpu/decoder/encoder_decoder_loader.h"
#endif

#ifdef CUDA
#include "gpu/decoder/encoder_decoder.h"
// #include "gpu/decoder/ape_penalty.h"

#ifdef KENLM
#include "gpu/decoder/language_model.h"
#endif
#endif

namespace amunmt {

LoaderPtr LoaderFactory::Create(
	const God &god,
    const std::string& name,
    const YAML::Node& config,
    const DeviceType deviceType) {
	Loader *loader;

#ifdef CUDA
  if (deviceType == GPUDevice) {
    loader = CreateGPU(god, name, config);
    if (loader) {
      return LoaderPtr(loader);
    } else {
      LOG(info, "No GPU scorer type. Switching to CPU");
    }
  }
#endif

#ifdef HAS_CPU
  if (deviceType == CPUDevice) {
    loader = CreateCPU(god, name, config);
    if (loader) {
      return LoaderPtr(loader);
    }
  }
#endif

	std::string type = config["type"].as<std::string>();
	amunmt_UTIL_THROW2("Unknown scorer in config file: " << type);
}

#ifdef CUDA
Loader *LoaderFactory::CreateGPU(
    const God &god,
    const std::string& name,
    const YAML::Node& config) {
  amunmt_UTIL_THROW_IF2(!config["type"],
				 "Missing scorer type in config file");

  std::string type = config["type"].as<std::string>();
  IF_MATCH_RETURN(god, type, "Nematus", GPU::EncoderDecoderLoader);
  IF_MATCH_RETURN(god, type, "nematus", GPU::EncoderDecoderLoader);
  IF_MATCH_RETURN(god, type, "NEMATUS", GPU::EncoderDecoderLoader);

  // IF_MATCH_RETURN(type, "Ape", GPU::ApePenaltyLoader);
  // IF_MATCH_RETURN(type, "ape", GPU::ApePenaltyLoader);
  // IF_MATCH_RETURN(type, "APE", GPU::ApePenaltyLoader);

#ifdef KENLM
  IF_MATCH_RETURN(god, type, "KenLM", GPU::KenLMLoader)
  IF_MATCH_RETURN(god, type, "kenlm", GPU::KenLMLoader)
  IF_MATCH_RETURN(god, type, "KENLM", GPU::KenLMLoader)
#endif

  return NULL;
}
#endif


#ifdef HAS_CPU
Loader *LoaderFactory::CreateCPU(
		const God &god,
		const std::string& name,
        const YAML::Node& config) {
  amunmt_UTIL_THROW_IF2(!config["type"],
         "Missing scorer type in config file");
  std::string type = config["type"].as<std::string>();

  IF_MATCH_RETURN(god, type, "Nematus", CPU::EncoderDecoderLoader);
  IF_MATCH_RETURN(god, type, "nematus", CPU::EncoderDecoderLoader);
  IF_MATCH_RETURN(god, type, "NEMATUS", CPU::EncoderDecoderLoader);

  return NULL;
}
#endif

}

