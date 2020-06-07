#ifndef __ISLESSPOINTERPREDICATE_H__
#define __ISLESSPOINTERPREDICATE_H__

template <class T>
class IsLessPointerPredicate
{
public:
    IsLessPointerPredicate<T>() { }
    ~IsLessPointerPredicate<T>() { }

    inline bool operator()(const T * ptr1, const T * ptr2) const
    {
        if (ptr1 && ptr2)
            return (*ptr1) < (*ptr2);
        else
            return false;
    }
};

#endif

