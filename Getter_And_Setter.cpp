
template<typename T>
class Gettable
{
public:
    virtual T& get() = 0;
};

template<typename T>
class Settable
{
public:
    virtual void set(T value) = 0;
};

template<typename T, typename Friend, typename Getter = void, typename Setter = void>
class GetAndSet : public Gettable<T>, public Settable<T>
{
    T m_value;
public:
    using get = Getter::operator();
    using set = Setter::operator();
    friend class Friend;
};

template<typename T, typename Friend>
class GetAndSet<T, Friend, void, void> : public Gettable<T>, public Settable<T>
{
    T m_value;
public:
    T& get() { return m_value; }
    void set(T value)
    {
        m_value = std::move(value);
    }
    friend class Friend;
};

template<typename T, typename Friend>
class Get : public Gettable<T>
{
    T m_value;
public:
    T& get() { return m_value; }
    friend class Friend;
};

template<typename T, typename Friend>
class Set : public Settable<T>
{
    T m_value;
public:
    void set(T value)
    {
        m_value = std::move(value);
    }
    friend class Friend;
};

struct MyClass
{
    template<typename Self>
    struct CustomSetter
    {
        auto& self = static_cast<Self&>(*this);
        void operator()(int i)
        {
            if (i <= 0)
                self.m_value = 0;
            else
                self.m_value = i;
        }
    };
    GetAndSet<int, MyClass, CustomSetter<GetAndSet<int, MyClass>>> member1;
    Set<int, MyClass> member2;
    Get<int, MyClass> member3;
};