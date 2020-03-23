#include <napi.h>
#include <string>
#include "md5.hpp"

class MD5AsyncWorker : public Napi::AsyncWorker {
 public:
  MD5AsyncWorker(
    const std::string& path,
    const Napi::Function& cb
  ): AsyncWorker(cb), path_(path), result_("") {}
  
 protected:
  void Execute() {
    try {
      result_ = toyo::md5::calc_file(path_);
    } catch (const std::exception& err) {
      SetError(err.what());
    }
  }

  void OnOK() {
    Napi::Env env = Env();
    Callback().Call(env.Undefined(), { env.Null(), Napi::String::New(env, result_) });
  }

  void OnError(const Napi::Error& err) {
    Napi::Env env = Env();
    Callback().Call(env.Undefined(), { err.Value(), Napi::String::New(env, result_) });
  }
 private:
  std::string path_;
  std::string result_;
};

static Napi::Value calcFile(const Napi::CallbackInfo& info) {
  Napi::String arg = info[0].As<Napi::String>();
  MD5AsyncWorker* w = new MD5AsyncWorker(arg, info[1].As<Napi::Function>());
  w->Queue();
  return info.Env().Undefined();
}

static Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports["calcFile"] = Napi::Function::New(env, calcFile, "calcFile");
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)
