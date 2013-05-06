#include "HandleFactory.h"

// BaseHandleFactory::BaseHandle ///////////////////////////////////////////////
BaseHandleFactory::BaseHandle::BaseHandle() :
    m_pFactory(NULL),
    m_uIndex(kuNullIndex),
    m_uKey(kuNullKey)
{
}

bool BaseHandleFactory::BaseHandle::operator ==(const BaseHandle& rhs) const {
    // I currently disallow comparing handles from separate factories.
    assert(
        (m_pFactory == NULL) ||
        (rhs.m_pFactory == NULL) ||
        (m_pFactory == rhs.m_pFactory)
    );

    return ((m_uIndex == rhs.m_uIndex) && (m_uKey == rhs.m_uKey));
}

void BaseHandleFactory::BaseHandle::operator ++() {
    assert(m_pFactory != NULL);
    assert(m_uIndex < m_pFactory->m_aHandles.size());

    ++m_uIndex;
    _FindValidEntry(m_uIndex, m_uKey);
}

void BaseHandleFactory::BaseHandle::_Free() const {
    assert(m_pFactory != NULL);
    m_pFactory->Free(*this);
}

BaseHandleFactory::BaseHandle::BaseHandle(
    BaseHandleFactory* pFactory,
    u32 uIndex,
    u32 uKey
) :
    m_pFactory(pFactory),
    m_uIndex(uIndex),
    m_uKey(uKey)
{
}

BaseHandleFactory::BaseHandle::BaseHandle(
    BaseHandleFactory* pFactory,
    u32 uIndex
) :
    m_pFactory(pFactory)
{
    m_uIndex = uIndex;
    _FindValidEntry(m_uIndex, m_uKey);
}

void BaseHandleFactory::BaseHandle::_FindValidEntry(
    u32& mod_uIndex,
    u32& out_uKey
) const {
    assert(m_pFactory != NULL);
    const vector<Entry>& aHandles = m_pFactory->m_aHandles;

    const u32 uNumHandles = aHandles.size();
    u32 uHandle = mod_uIndex;
    while(uHandle < uNumHandles) {
        if(!(aHandles[uHandle].IsFree())) {
            mod_uIndex = uHandle;
            out_uKey = aHandles[uHandle].m_uKey;
            return;
        }

        ++uHandle;
    }

    mod_uIndex = uNumHandles;
    out_uKey = kuNullKey;
}

bool BaseHandleFactory::BaseHandle::_IsValid() const {
    return (
        (m_pFactory != NULL) &&
        (m_pFactory->Get(m_uIndex, m_uKey) != NULL)
    );
}


// BaseHandleFactory ///////////////////////////////////////////////////////////
BaseHandleFactory::BaseHandleFactory() :
    m_uNumAllocated(0),
    m_uFreeListHead(kuNullIndex)
{
}

BaseHandleFactory::~BaseHandleFactory() {
}

BaseHandleFactory::BaseHandle BaseHandleFactory::Alloc(const void* pPointer) {
    assert(pPointer != NULL);

    ++m_uNumAllocated;

    // If the free list is empty, add a new entry.
    if(m_uFreeListHead == kuNullIndex) {
        const u32 uKey = 0;
        const u32 uIndex = m_aHandles.size();
        
        Entry rEntry;
        rEntry.m_data.m_pPointer = const_cast<void*>(pPointer);
        rEntry.m_uKey = uKey;
        rEntry.SetFree(false);
        m_aHandles.push_back(rEntry);

        return BaseHandle(this, uIndex, uKey);
    }
    else {
        const u32 uIndex = m_uFreeListHead;
        Entry& rEntry = m_aHandles[uIndex];

        assert(rEntry.IsFree());
        rEntry.SetFree(false);

        m_uFreeListHead = rEntry.m_data.m_uNext;
        rEntry.m_data.m_pPointer = const_cast<void*>(pPointer);

        return BaseHandle(this, uIndex, rEntry.m_uKey);
    }
}

void BaseHandleFactory::Free(const BaseHandle& hHandle) {
    if(hHandle.m_pFactory == NULL) {
        return;
    }

    // Verify we're not trying to Free another factory's handles.
    assert(hHandle.m_pFactory == this);

    Entry& rEntry = m_aHandles[hHandle.m_uIndex];

    // If the handle is valid, invalidate the entry.
    if(rEntry.m_uKey == hHandle.m_uKey) {
        ++(rEntry.m_uKey);
        rEntry.SetFree(true);
        rEntry.m_data.m_uNext = m_uFreeListHead;
        m_uFreeListHead = hHandle.m_uIndex;
        assert(m_uNumAllocated > 0);
        --m_uNumAllocated;
    }
    // Else do nothing.
}

void* BaseHandleFactory::Get(u32 uIndex, u32 uKey) const
{
	if (uIndex < m_aHandles.size())
	{
		// If the handle has a valid key, return the data.
		const Entry& entry = m_aHandles[uIndex];
		if (entry.m_uKey == uKey)
		{
			assert(!(entry.IsFree()));
			return entry.m_data.m_pPointer;
		}
	}

    // Else it's expired, so return NULL.
	return (NULL);
}

BaseHandleFactory::BaseHandle BaseHandleFactory::Begin() {
    return BaseHandle(this, 0);
}

BaseHandleFactory::BaseHandle BaseHandleFactory::End() {
    return BaseHandle(this, m_aHandles.size());
}
