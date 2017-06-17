#pragma once

#include <cuda.h>
#include <cublas_v2.h>

namespace amunmt {
namespace GPU {
namespace mblas {

class CudaStreamHandler {
    CudaStreamHandler()
    {
      HANDLE_ERROR( cudaStreamCreate(&stream_));
      // cudaStreamCreateWithFlags(stream_.get(), cudaStreamNonBlocking);
    }

    CudaStreamHandler(const CudaStreamHandler&) = delete;

  protected:
    static thread_local CudaStreamHandler instance_;
    cudaStream_t stream_;

  public:
    static const cudaStream_t& GetStream() {
      return (instance_.stream_);
    }

    virtual ~CudaStreamHandler() {
      HANDLE_ERROR(cudaStreamDestroy(stream_));
    }
};


class CublasHandler {
  public:
    static cublasHandle_t GetHandle() {
      if(handle_ == nullptr) {
        assert(handle_ == nullptr);
        handle_ = new cublasHandle_t;
        cublasCreate(handle_);
        cublasSetStream(*handle_, CudaStreamHandler::GetStream());
      }
      return *handle_;
    }

  private:
    ~CublasHandler() {
      cublasDestroy(*handle_);
      if (handle_) {
        delete handle_;
      }
    }

    static thread_local cublasHandle_t* handle_;
};

} // namespace mblas
} // namespace GPU
}
