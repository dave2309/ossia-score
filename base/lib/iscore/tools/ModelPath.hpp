#pragma once
#include <QDataStream>
#include <iscore/document/DocumentInterface.hpp>

template<typename T, typename U>
struct in_relationship
{
    static const constexpr bool value = std::is_base_of<T, U>::value || std::is_base_of<U, T>::value ;
};

// These forward declarations are required
// because Path<T>(Object&) calls iscore::IDocument::path()
// which in turns calls another constructor of Path<T>.
template<typename T>
class Path;

namespace iscore
{
namespace IDocument
{
#if defined(__clang__)
/*
template<typename T, typename U = void>
  Path<T> path(const T& obj)
  {
      static_assert(false, "Bad instantiation");
  }
  */
#endif
template<typename T, std::enable_if_t<
             std::is_base_of<
                 IdentifiedObjectAbstract,
                 T
                >::value
             >* = nullptr
         >
Path<T> path(const T& obj);

template<typename T, std::enable_if_t<
             !std::is_base_of<
                 IdentifiedObjectAbstract,
                 T
                >::value
             >* = nullptr
         >
Path<T> path(const T& obj);
}
}

/**
 * @brief The Path class is a typesafe wrapper around ObjectPath.
 */
template<typename Object>
class Path
{
        friend bool operator==(const Path& lhs, const Path& rhs)
        {
            return lhs.m_impl == rhs.m_impl;
        }

        friend uint qHash(const Path& obj, uint seed)
        {
          return qHash(obj.m_impl, seed);
        }

        template<typename U>
        friend class Path;
        friend class ObjectPath;

    public:
        // Use this if it is not possible to get a path
        // (for instance because the object does not exist yet)
        struct UnsafeDynamicCreation{ explicit UnsafeDynamicCreation() = default; };
        Path(const ObjectPath& obj, UnsafeDynamicCreation): m_impl(obj) { }
        Path(ObjectPath&& obj, UnsafeDynamicCreation): m_impl(std::move(obj)) { }

        Path(const Object& obj): Path(iscore::IDocument::path(obj)) { }


        template<typename U>
        auto extend(const QString& name, const Id<U>& id) const &
        {
            Path<U> p{this->m_impl};
            p.m_impl.vec().push_back({name, id});
            return p;
        }

        template<typename U>
        auto extend(const QString& name, const Id<U>& id) &&
        {
            Path<U> p{std::move(this->m_impl)};
            p.m_impl.vec().push_back({name, id});
            return p;
        }


        template<typename U>
        auto splitLast() const &
        {
            auto vec = m_impl.vec();
            auto last = vec.takeLast();
            return std::make_pair(Path<U>{ObjectPath{std::move(vec)}}, std::move(last));
        }

        template<typename U>
        auto splitLast() &&
        {
            auto last = m_impl.vec().takeLast();
            return std::make_pair(Path<U>{std::move(m_impl)}, std::move(last));
        }

        // TODO do the same for ids
        // TODO make it work only for upcasts
        template<typename U, std::enable_if_t<in_relationship<U, Object>::value>* = nullptr>
        Path(const Path<U>& other): m_impl(other.m_impl) { }
        template<typename U, std::enable_if_t<in_relationship<U, Object>::value>* = nullptr>
        Path(Path<U>&& other): m_impl(std::move(other.m_impl)) { }

        template<typename U, std::enable_if_t<in_relationship<U, Object>::value>* = nullptr>
        Path& operator=(const Path<U>& other) { m_impl = other.m_impl; return *this; }
        template<typename U, std::enable_if_t<in_relationship<U, Object>::value>* = nullptr>
        Path& operator=(Path<U>&& other) { m_impl = std::move(other.m_impl); return *this; }

        Path() = default;
        Path(const Path&) = default;
        Path(Path&&) = default;
        Path& operator=(const Path&) = default;
        Path& operator=(Path&&) = default;

        Object& find() const
        { return m_impl.find<Object>(); }
        Object* try_find() const
        { return m_impl.try_find<Object>(); }

        const auto& unsafePath() const
        { return m_impl; }
        auto& unsafePath()
        { return m_impl; }

        auto&& moveUnsafePath()
        { return std::move(m_impl); }

        bool valid() const
        { return m_impl.vec().size() > 0; }

    private:
        Path(const ObjectPath& path): m_impl{path} { }
        Path(ObjectPath&& path): m_impl{std::move(path)} { }
        ObjectPath m_impl;
};

template<typename T>
Path<T> make_path(const T& t)
{
    return t;
}
