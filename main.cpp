#include <Windows.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>
#include <vector>

#include "filesearch_sdk.h"

namespace py = pybind11;

bool IsElevated() {
  bool is_elevated = false;
  HANDLE token_handle = nullptr;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token_handle)) {
    TOKEN_ELEVATION elevation;
    DWORD buffer_size = sizeof(TOKEN_ELEVATION);
    if (GetTokenInformation(token_handle, TokenElevation, &elevation,
                            sizeof(elevation), &buffer_size)) {
      is_elevated = (elevation.TokenIsElevated != 0);
    }
    CloseHandle(token_handle);
  }
  return is_elevated;
}

class FileSearch {
 public:
  FileSearch() {
    if (!IsElevated()) {
      throw std::exception("Please run the program as administrator.");
    }

    SE_CREATE_CALLBACKS create_callbacks{0};
    search_handle_ = SearchEngineCreate(&create_callbacks);
    if (!search_handle_) {
      throw std::exception("Create search handle failed.");
    }
    SearchEngineUseDatabse(search_handle_, false);
    SearchEngineStart(search_handle_);
  }
  ~FileSearch() {
    SearchEngineStop(search_handle_);
    SearchEngineDelete(search_handle_);
  }

  static void __stdcall Onfind(LPCWSTR filepath, PVOID context) {
    static_cast<FileSearch*>(context)->file_list_.emplace_back(filepath);
  }

  bool Search(const std::wstring& pattern) {
    file_list_.clear();
    SE_SEARCH_CALLBACKS callback = {0};
    callback.SearchOnFindOne = Onfind;
    callback.Context = this;
    return SearchEngineSearch(search_handle_, pattern.c_str(), &callback,
                              FALSE);
  }

  bool Wait(unsigned long milisec) {
    return SearchEngineWaitScanFinish(search_handle_, milisec) == WAIT_OBJECT_0;
  }

  std::vector<std::wstring> GetResult() { return file_list_; }

 private:
  SE_HANDLE search_handle_ = nullptr;
  std::vector<std::wstring> file_list_;
};

PYBIND11_MODULE(filesearch, m) {
  m.doc() = "Locate files and folders by name instantly.";

  py::class_<FileSearch>(m, "filesearch")
      .def(py::init<>())
      .def("search", &FileSearch::Search)
      .def("wait", &FileSearch::Wait)
      .def("result", &FileSearch::GetResult);
}