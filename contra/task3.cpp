#include <iostream>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <string>

using handle_t = FILE*;
handle_t kNullHandle = NULL;

void raw_write(handle_t handle, const std::string& content) {
    fprintf(handle,"%s",content.c_str());
}

void raw_close(handle_t handle) {
    fclose(handle);
}

// YOUR CODE OF IOWrapper
class IOWrapper {
private:
    handle_t resource;
public:
    IOWrapper() = delete;
    IOWrapper(IOWrapper &src) = delete;
    IOWrapper& operator=(IOWrapper &src) = delete;

    IOWrapper(handle_t resource): resource(resource) {   }
    
    IOWrapper(IOWrapper &&src): resource(src.resource) {
        src.resource = kNullHandle;
    }
    
    IOWrapper& operator=(IOWrapper &&src) {
        IOWrapper tmp(std::move(src));
        
        resource = kNullHandle;
        std::swap(this->resource, tmp.resource);
        
        return *this;
    }

    void Write(const std::string& content) {
        raw_write(resource, content);
    }

    ~IOWrapper() {
        if (resource != kNullHandle) {
            raw_close(resource);
        }
    }
};


int main() {
    {
        IOWrapper wrapper(fopen("./data.txt","w"));
        wrapper.Write("Hello");
    }
    {
        IOWrapper wrapper(fopen("./data.txt","a"));
        wrapper.Write(" ");
    }
    {
        IOWrapper wrapper(fopen("./data.txt","a"));
        wrapper.Write("World!");
    }
    return 0;
}
  

