#ifndef CHECK_SIZE_H
#define CHECK_SIZE_H

template<typename T, size_t ExpectedSize, size_t ActualSize=sizeof(T)>
void check_size()
{
    static_assert(ExpectedSize == ActualSize, "Wrong size! Check output to see which type. Look for #pragma pack to solve this problem or consult the accompanying text");
}

#endif
