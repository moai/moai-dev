#ifndef HandleFactory_h
#define HandleFactory_h

#include "Common.h"
#include <zl-util/pch.h>

namespace TestHandleFactory {
    class Accessor;
}

class BaseHandleFactory {
protected:
    class BaseHandle {
    protected:
        BaseHandle();

        bool operator ==(const BaseHandle& rhs) const;
        void operator ++();

        void* _Get() const;
        void _Free() const;

    private:
        friend class BaseHandleFactory;
        friend class TestHandleFactory::Accessor;

        // Handle constructor
        BaseHandle(
            BaseHandleFactory* pFactory,
            u32 uIndex,
            u32 uKey
        );

        // Iterator constructor
        BaseHandle(BaseHandleFactory* pFactory, u32 uIndex);

        // mod_uIndex should contain the index of the entry you want to begin
        // the search at.
        void _FindValidEntry(u32& mod_uIndex, u32& out_uKey) const;

        bool _IsValid() const;

    private:
        BaseHandleFactory* m_pFactory;
        u32 m_uIndex;
        u32 m_uKey;
    };

    explicit BaseHandleFactory();
    ~BaseHandleFactory();

    BaseHandle Alloc(const void* pPointer);
    void Free(const BaseHandle& hHandle);

    void* Get(u32 uIndex, u32 uKey) const;

    u32 GetNumAllocated() const;

    BaseHandle Begin();
    BaseHandle End();

private:
    friend class TestHandleFactory::Accessor;

    static const u32 kuNullIndex = ~0u;
    static const u32 kuNullKey = ~0u;

    union HandleData {
        void* m_pPointer;
        u32 m_uNext;
    };

    struct Entry {
        bool IsFree() const {
            return (m_uIsFree != 0);
        }

        void SetFree(bool bFree) {
            m_uIsFree = (bFree ? 1 : 0);
        }

        HandleData m_data;
        u32 m_uKey : 31;

    private:
        u32 m_uIsFree : 1;
    };

private:
    vector<Entry> m_aHandles;
    u32 m_uNumAllocated;
    u32 m_uFreeListHead;
};

inline void* BaseHandleFactory::BaseHandle::_Get() const {
	return m_pFactory != NULL ? m_pFactory->Get(m_uIndex, m_uKey) : NULL;
}

inline u32 BaseHandleFactory::GetNumAllocated() const {
    return m_uNumAllocated;
}


// This class can be used to create handles that return TType* when
// dereferenced. This uses a key system instead of ref counting to ensure
// that expired handles don't return valid data.
template<typename TType> class HandleFactory : private BaseHandleFactory {
public:
    class Handle : protected BaseHandle {
    public:
        Handle() {
        }

        bool operator ==(const Handle& rhs) const {
            return BaseHandle::operator ==(rhs);
        }

        bool operator !=(const Handle& rhs) const {
            return !(*this == rhs);
        }

        // Returns NULL if the handle is uninitialized or has been freed.
        TType* operator *() const {
            return static_cast<TType*>(_Get());
        }

    protected:
        friend class HandleFactory;
        friend class TestHandleFactory::Accessor;

        explicit Handle(const BaseHandle& hHandle) :
            BaseHandle(hHandle)
        {
        }
    };

    class ConstIterator : public Handle {
    public:
        void operator ++() {
            BaseHandle::operator ++();
        }

        const TType* operator *() const {
            TType* const pResult = static_cast<TType*>(BaseHandle::_Get());

            // Iterators aren't allowed to be dereferenced if invalid.
            assert(pResult != NULL);
            return pResult;
        }

    protected:
        friend class HandleFactory;

        explicit ConstIterator(const BaseHandle& hHandle) :
            Handle(hHandle)
        {
        }
    };

    class Iterator : public ConstIterator {
    public:
        TType* operator *() const {
            TType* const pResult = static_cast<TType*>(BaseHandle::_Get());

            // Iterators aren't allowed to be dereferenced if invalid.
            assert(pResult != NULL);
            return pResult;
        }

    private:
        friend class HandleFactory;

        explicit Iterator(const BaseHandle& hHandle) :
            ConstIterator(hHandle)
        {
        }
    };

    explicit HandleFactory() :
        BaseHandleFactory()
    {
    }

    Handle Alloc(TType* pPointer) {
        return Handle(BaseHandleFactory::Alloc(pPointer));
    }

    // Even though Free could be part of the Handle interface, I require the
    // handle owner to go through the factory to maximize a system's ability to
    // manage its handles.
    void Free(const Handle& hHandle) {
        BaseHandleFactory::Free(hHandle);
    }

    void FreeAndAdvance(Iterator& mod_itHandle) {
        mod_itHandle._Free();
        ++(mod_itHandle);
    }

    Iterator Begin() {
        return Iterator(BaseHandleFactory::Begin());
    }

    ConstIterator Begin() const {
        return
            ConstIterator(
                const_cast<HandleFactory*>(this)->BaseHandleFactory::Begin()
            )
        ;
    }

    Iterator End() {
        return Iterator(BaseHandleFactory::End());
    }

    ConstIterator End() const {
        return
            ConstIterator(
                const_cast<HandleFactory*>(this)->BaseHandleFactory::End()
            )
        ;
    }

    using BaseHandleFactory::GetNumAllocated;

private:
    friend class TestHandleFactory::Accessor;
};

#endif//HandleFactory_h
