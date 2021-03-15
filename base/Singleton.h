#ifndef _SYS_SINGLETON_H_
#define _SYS_SINGLETON_H_

namespace base {

template<typename T>
class Singleton {
protected:
    inline Singleton() = default;

public:
    static inline T &Get() {
        static T obj;
        return obj;
    }
    static inline T *GetPtr() {
        return &Get();
    }
};

}

#endif // _SYS_SINGLETON_H_
