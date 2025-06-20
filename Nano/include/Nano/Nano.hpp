#pragma once

#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <exception>

#include <any>
#include <new>
#include <queue>
#include <mutex>
#include <chrono>
#include <future>
#include <thread>
#include <random>
#include <ranges>
#include <variant>
#include <utility>
#include <numeric>
#include <concepts>
#include <algorithm>
#include <coroutine>
#include <filesystem>
#include <functional>
#include <type_traits>
#include <shared_mutex>
#include <condition_variable>

#include <format>
#include <string>
#include <string_view>

#include <iostream>
#include <iomanip>
#include <sstream>

#include <set>
#include <map>
#include <tuple>
#include <array>
#include <optional>
//#include <flat_map>
#include <unordered_set>
#include <unordered_map>

// Compiler
#if defined(_MSC_VER)
    #define NANO_COMPILER_MSVC
#elif defined(__clang__)
    #define NANO_COMPILER_CLANG
#elif defined(__GNUC__)
    #define NANO_COMPILER_GCC
#endif

// Platform
#if (defined(_WIN32) || defined(_WIN64)) && (!defined(NANO_PLATFORM_WINDOWS) && !defined(NANO_PLATFORM_DESKTOP))
    #define NANO_PLATFORM_WINDOWS
    #define NANO_PLATFORM_DESKTOP
#elif defined(__ANDROID__) && (!defined(NANO_PLATFORM_ANDROID) && !defined(NANO_PLATFORM_MOBILE))
    #define NANO_PLATFORM_ANDROID
    #define NANO_PLATFORM_MOBILE
#elif defined(__APPLE__) && ((!defined(NANO_PLATFORM_MACOS) || defined(NANO_PLATFORM_IOS)) && (!defined(NANO_PLATFORM_DESKTOP) || !defined(NANO_PLATFORM_MOBILE)))
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define NANO_PLATFORM_MACOS
        #define NANO_PLATFORM_DESKTOP
    #elif TARGET_OS_IPHONE
        #define NANO_PLATFORM_IOS
        #define NANO_PLATFORM_MOBILE
    #endif
#elif defined(__linux__) && (!defined(NANO_PLATFORM_LINUX) && !defined(NANO_PLATFORM_DESKTOP))
    #define NANO_PLATFORM_LINUX
    #define NANO_PLATFORM_DESKTOP
#endif

#if defined(__ANDROID__) || defined(__linux__) || defined(__APPLE__)
    #define NANO_PLATFORM_UNIX
#endif

#if defined(__APPLE__)
    #define NANO_PLATFORM_APPLE
#endif

// Cpp standard
#define NANO_CPPSTD_UNKOWN 0
#define NANO_CPPSTD_11 11
#define NANO_CPPSTD_14 14
#define NANO_CPPSTD_17 17
#define NANO_CPPSTD_20 20
#define NANO_CPPSTD_23 23
#define NANO_CPPSTD_26 26
#define NANO_CPPSTD_LATEST NANO_CPPSTD_26

#if defined(NANO_COMPILER_MSVC)
    #if _MSVC_LANG >= 202602L
        #define NANO_CPPSTD NANO_CPPSTD_26
    #elif _MSVC_LANG >= 202302L
        #define NANO_CPPSTD NANO_CPPSTD_23
    #elif _MSVC_LANG >= 202002L
        #define NANO_CPPSTD NANO_CPPSTD_20
    #elif _MSVC_LANG >= 201703L
        #define NANO_CPPSTD NANO_CPPSTD_17
    #elif _MSVC_LANG >= 201402L
        #define NANO_CPPSTD NANO_CPPSTD_14
    #elif _MSVC_LANG >= 201103L
        #define NANO_CPPSTD NANO_CPPSTD_11
    #else
        #define NANO_CPPSTD NANO_CPPSTD_UNKOWN
    #endif
#else
    #if __cplusplus == 202602L
        #define NANO_CPPSTD NANO_CPPSTD_26
    #elif __cplusplus >= 202302L
        #define NANO_CPPSTD NANO_CPPSTD_23
    #elif __cplusplus >= 202002L
        #define NANO_CPPSTD NANO_CPPSTD_20
    #elif __cplusplus >= 201703L
        #define NANO_CPPSTD NANO_CPPSTD_17
    #elif __cplusplus >= 201402L
        #define NANO_CPPSTD NANO_CPPSTD_14
    #elif __cplusplus >= 201103L
        #define NANO_CPPSTD NANO_CPPSTD_11
    #else
        #define NANO_CPPSTD NANO_CPPSTD_UNKOWN
    #endif
#endif

// Debug-break
#if defined(NANO_PLATFORM_WINDOWS)
    #include <intrin.h>
    #define NANO_DEBUG_BREAK() __debugbreak()
#elif defined(NANO_PLATFORM_LINUX) || defined(NANO_PLATFORM_APPLE)
    #include <csignal>
    #define NANO_DEBUG_BREAK() std::raise(SIGTRAP)
#else
    #include <cstdlib>
    #define NANO_DEBUG_BREAK() std::abort()
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif

// Debug
#if defined(NANO_DEBUG)
    #define NANO_DEBUG_EXPECTED

    #if !defined(NANO_ASSERT)
        #define NANO_ASSERT(x, msg) do { if (!(x)) { std::cerr << "Nano: Assert failed" << msg << std::endl; NANO_DEBUG_BREAK(); } } while (false)
    #endif

    #define NANO_DEBUG_CONSTEXPR 
#else
    #if !defined(NANO_ASSERT)
        #define NANO_ASSERT(x, msg)
    #endif

    #define NANO_DEBUG_CONSTEXPR constexpr
#endif

// Implementations
#if defined(NANO_IMPLEMENTATION)
    #define NANO_IMPL_RANDOM
    #define NANO_IMPL_TIME
    #define NANO_IMPL_MEMORY
    #define NANO_IMPL_THREADING
    #define NANO_IMPL_TESTS
    #define NANO_IMPL_BENCHMARKS
#endif

// Function start-up macros
#define NANO_RUN_FUNCTION_HELPER2(id, functionName, ...)	    \
namespace													    \
{															    \
	inline void _Run##id()									    \
	{														    \
		functionName(__VA_ARGS__);							    \
	}														    \
															    \
	static const char _RunDummy##id = (_Run##id(), 0);		    \
}
#define NANO_RUN_FUNCTION_HELPER(id, functionName, ...)      NANO_RUN_FUNCTION_HELPER2(id, functionName, __VA_ARGS__)

#define NANO_RUN_FUNCTION_NN_HELPER2(id, functionName, ...)  \
inline static void _Run##id()								    \
{															    \
    functionName(__VA_ARGS__);								    \
}															    \
															    \
inline static const char _RunDummy##id = (_Run##id(), 0)
#define NANO_RUN_FUNCTION_NN_HELPER(id, functionName, ...)   NANO_RUN_FUNCTION_NN_HELPER2(id, functionName, __VA_ARGS__)

#define NANO_RUN_FUNCTION(functionName, ...)				    NANO_RUN_FUNCTION_HELPER(__COUNTER__, functionName, __VA_ARGS__)
#define NANO_RUN_FUNCTION_NN(functionName, ...)			    NANO_RUN_FUNCTION_NN_HELPER(__COUNTER__, functionName, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- CompileInformation HPP
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::CompileInformation
{

    namespace Structs
    {

        ////////////////////////////////////////////////////////////////////////////////////
        // Structs
        ////////////////////////////////////////////////////////////////////////////////////
        enum class Compiler : uint8_t
        {
            Unknown = 0,

            MSVC,
            GCC,
            Clang
        };

        enum class Platform : uint8_t
        {
            Unknown = 0,

            Windows,
            Linux,
            MacOS,

            Android,
            iOS,
        };

        enum class CppStd : uint8_t
        {
            Unknown = NANO_CPPSTD_UNKOWN,

            Cpp11 = NANO_CPPSTD_11,
            Cpp14 = NANO_CPPSTD_14,
            Cpp17 = NANO_CPPSTD_17,
            Cpp20 = NANO_CPPSTD_20,
            Cpp23 = NANO_CPPSTD_23,
            Cpp26 = NANO_CPPSTD_26,
        };

    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Values
    ////////////////////////////////////////////////////////////////////////////////////
    #if defined(NANO_COMPILER_MSVC)
        inline constexpr const Structs::Compiler Compiler = Structs::Compiler::MSVC;
    #elif defined(NANO_COMPILER_GCC)
        inline constexpr const Structs::Compiler Compiler = Structs::Compiler::GCC;
    #elif defined(NANO_COMPILER_CLANG)
        inline constexpr const Structs::Compiler Compiler = Structs::Compiler::Clang;
    #else
        inline constexpr const Structs::Compiler Compiler = Structs::Compiler::Unknown;
    #endif

    #if defined(NANO_PLATFORM_WINDOWS)
        inline constexpr const Structs::Platform Platform = Structs::Platform::Windows;
    #elif defined(NANO_PLATFORM_LINUX) 
        inline constexpr const Structs::Platform Platform = Structs::Platform::Linux;
    #elif defined(NANO_PLATFORM_MACOS)
        inline constexpr const Structs::Platform Platform = Structs::Platform::MacOS;
    #elif defined(NANO_PLATFORM_ANDROID)
        inline constexpr const Structs::Platform Platform = Structs::Platform::Android;
    #elif defined(NANO_PLATFORM_IOS)
        inline constexpr const Structs::Platform Platform = Structs::Platform::iOS;
    #endif

    inline constexpr const Structs::CppStd CppStd = static_cast<Structs::CppStd>(NANO_CPPSTD);

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Logging HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Colours
    ////////////////////////////////////////////////////////////////////////////////////
    namespace Log::Colour
    {
        // Reset
        inline constexpr std::string_view Reset = "\033[0m";

        // Text Attributes
        inline constexpr std::string_view Bold = "\033[1m";
        inline constexpr std::string_view Dim = "\033[2m";
        inline constexpr std::string_view Italic = "\033[3m";
        inline constexpr std::string_view Underline = "\033[4m";
        inline constexpr std::string_view BlinkSlow = "\033[5m";
        inline constexpr std::string_view BlinkFast = "\033[6m";
        inline constexpr std::string_view Invert = "\033[7m";
        inline constexpr std::string_view Hidden = "\033[8m";
        inline constexpr std::string_view StrikeThrough = "\033[9m";

        // Foreground Colours
        inline constexpr std::string_view BlackFG = "\033[30m";
        inline constexpr std::string_view RedFG = "\033[31m";
        inline constexpr std::string_view GreenFG = "\033[32m";
        inline constexpr std::string_view YellowFG = "\033[33m";
        inline constexpr std::string_view BlueFG = "\033[34m";
        inline constexpr std::string_view MagentaFG = "\033[35m";
        inline constexpr std::string_view CyanFG = "\033[36m";
        inline constexpr std::string_view WhiteFG = "\033[37m";

        // Bright Foreground Colours
        inline constexpr std::string_view BrightBlackFG = "\033[90m";
        inline constexpr std::string_view BrightRedFG = "\033[91m";
        inline constexpr std::string_view BrightGreenFG = "\033[92m";
        inline constexpr std::string_view BrightYellowFG = "\033[93m";
        inline constexpr std::string_view BrightBlueFG = "\033[94m";
        inline constexpr std::string_view BrightMagentaFG = "\033[95m";
        inline constexpr std::string_view BrightCyanFG = "\033[96m";
        inline constexpr std::string_view BrightWhiteFG = "\033[97m";

        // Background Colours
        inline constexpr std::string_view BlackBG = "\033[40m";
        inline constexpr std::string_view RedBG = "\033[41m";
        inline constexpr std::string_view GreenBG = "\033[42m";
        inline constexpr std::string_view YellowBG = "\033[43m";
        inline constexpr std::string_view BlueBG = "\033[44m";
        inline constexpr std::string_view MagentaBG = "\033[45m";
        inline constexpr std::string_view CyanBG = "\033[46m";
        inline constexpr std::string_view WhiteBG = "\033[47m";

        // Bright Background Colours
        inline constexpr std::string_view BrightBlackBG = "\033[100m";
        inline constexpr std::string_view BrightRedBG = "\033[101m";
        inline constexpr std::string_view BrightGreenBG = "\033[102m";
        inline constexpr std::string_view BrightYellowBG = "\033[103m";
        inline constexpr std::string_view BrightBlueBG = "\033[104m";
        inline constexpr std::string_view BrightMagentaBG = "\033[105m";
        inline constexpr std::string_view BrightCyanBG = "\033[106m";
        inline constexpr std::string_view BrightWhiteBG = "\033[107m";
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Loggging
    ////////////////////////////////////////////////////////////////////////////////////
    namespace Log
    {

        ////////////////////////////////////////////////////////////////////////////////////
        // Utils
        ////////////////////////////////////////////////////////////////////////////////////
        inline void SetColour(std::string_view colour)
        {
            std::cout << colour;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Print
        ////////////////////////////////////////////////////////////////////////////////////
        template<typename... TArgs>
        inline void Print(std::string_view msg)
        {
            std::cout << msg << Colour::Reset;
        }

        template<typename ...TArgs>
        inline void PrintF(std::format_string<TArgs...> fmt, TArgs&&... args)
        {
            std::cout << std::format(fmt, std::forward<TArgs>(args)...) << Colour::Reset;
        }

        template<typename... TArgs>
        inline void PrintLn(std::format_string<TArgs...> fmt, TArgs&&... args)
        {
            PrintF(fmt, std::forward<TArgs>(args)...);
            std::cout << '\n';
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Levels
        ////////////////////////////////////////////////////////////////////////////////////
        enum class Level : uint8_t { Trace, Info, Warn, Error, Fatal };

        template<Level level>
        [[nodiscard]] consteval std::string_view LevelTag() noexcept(true)
        {
            if constexpr (level == Level::Trace)
                return "T";
            else if constexpr (level == Level::Info)
                return "I";
            else if constexpr (level == Level::Warn)
                return "W";
            else if constexpr (level == Level::Error)
                return "E";
            else if constexpr (level == Level::Fatal)
                return "F";
        }

        template<Level level, typename ...TArgs>
        void PrintLvl(std::format_string<TArgs...> fmt, TArgs&&... args)
        {
            std::string time;
            {
                auto now = std::chrono::system_clock::now();
                std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

                std::ostringstream oss;
                oss << std::put_time(std::localtime(&nowTime), "%H:%M:%S");
                time = oss.str();
            }

            std::string message = std::format("[{0}] [{1}]: {2}", time, LevelTag<level>(), std::format(fmt, std::forward<TArgs>(args)...));

            // Set colour
            if constexpr (level == Level::Trace)
                std::cout << Colour::Reset;
            else if constexpr (level == Level::Info)
                std::cout << Colour::GreenFG;
            else if constexpr (level == Level::Warn)
                std::cout << Colour::BrightYellowFG;
            else if constexpr (level == Level::Error)
                std::cout << Colour::BrightRedFG;
            else if constexpr (level == Level::Fatal)
                std::cout << Colour::RedBG;

            // Simulate PrintLn
            Print(message);
            std::cout << '\n';
        }

    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Text HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Text
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Utility
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename ...Args>
    [[nodiscard]] std::string Format(std::format_string<Args...> fmt, Args&& ...args)
    {
        return std::format(fmt, std::forward<Args>(args)...);
    }

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Traits HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Traits
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Traits
    ////////////////////////////////////////////////////////////////////////////////////
    struct NoCopy
    {
    public:
        constexpr NoCopy() noexcept(true) = default;
        constexpr NoCopy(const NoCopy&) noexcept(true) = delete;
        constexpr NoCopy& operator = (const NoCopy&) noexcept(true) = delete;
    };

    struct NoMove
    {
    public:
        constexpr NoMove() noexcept(true) = default;
        constexpr NoMove(NoMove&&) noexcept(true) = delete;
        constexpr NoMove& operator = (NoMove&&) noexcept(true) = delete;
    };

    struct NoConstruct
    {
    public:
        constexpr NoConstruct() noexcept(true) = delete;
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Hash HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Hash
    ////////////////////////////////////////////////////////////////////////////////////
    class Hash final : public Traits::NoConstruct
    {
    public:
        // String hashing
        [[nodiscard]] static constexpr uint64_t fnv1(const std::string_view str) noexcept(true)
        {
            uint64_t hash = 14695981039346656037ULL;
            for (char c : str)
            {
                hash *= 1099511628211ULL;
                hash ^= static_cast<uint64_t>(c);
            }

            return hash;
        }

        [[nodiscard]] static constexpr uint64_t fnv1a(const std::string_view str) noexcept(true)
        {
            uint64_t hash = 14695981039346656037ULL;
            for (char c : str)
            {
                hash ^= static_cast<uint64_t>(c);
                hash *= 1099511628211ULL;
            }

            return hash;
        }

        [[nodiscard]] static constexpr uint64_t djb2(const std::string_view str) noexcept(true)
        {
            uint64_t hash = 5381;
            for (char c : str)
                hash = ((hash << 5) + hash) + static_cast<uint64_t>(c); // hash * 33 + c

            return hash;
        }

        [[nodiscard]] static constexpr uint64_t sdbm(const std::string_view str) noexcept(true)
        {
            uint64_t hash = 0;
            for (char c : str)
                hash = static_cast<uint64_t>(c) + (hash << 6) + (hash << 16) - hash;

            return hash;
        }

        [[nodiscard]] static constexpr uint32_t crc32(const std::string_view str) noexcept(true)
        {
            uint32_t crc = 0xFFFFFFFF;
            for (char c : str)
            {
                crc ^= static_cast<uint8_t>(c);
                for (int i = 0; i < 8; ++i)
                    crc = (crc >> 1) ^ (0xEDB88320 & ((crc & 1)));
            }

            return ~crc;
        }

        [[nodiscard]] static constexpr uint64_t murmur64(const std::string_view str, uint64_t seed = 0xc70f6907UL) noexcept(true)
        {
            uint64_t hash = seed ^ (str.size() * 0xc6a4a7935bd1e995);
            for (char c : str)
            {
                uint64_t k = static_cast<uint64_t>(c);
                k *= 0xc6a4a7935bd1e995;
                k ^= k >> 47;
                k *= 0xc6a4a7935bd1e995;
                hash ^= k;
                hash *= 0xc6a4a7935bd1e995;
            }

            hash ^= hash >> 47;
            hash *= 0xc6a4a7935bd1e995;
            hash ^= hash >> 47;

            return hash;
        }

        // Integer hashing
        [[nodiscard]] static constexpr uint64_t UInt64(uint64_t x) noexcept(true)
        {
            x ^= x >> 33;
            x *= 0xff51afd7ed558ccdULL;
            x ^= x >> 33;
            x *= 0xc4ceb9fe1a85ec53ULL;
            x ^= x >> 33;

            return x;
        }

    public:
        // Helper functions
        [[nodiscard]] static constexpr uint64_t Combine(const uint64_t hash1, const uint64_t hash2) noexcept(true)
        {
            uint64_t combined = hash1 + 0x9e3779b97f4a7c15;
            combined = (combined ^ (hash2 >> 30)) * 0xbf58476d1ce4e5b9;
            combined = (combined ^ (combined >> 27)) * 0x94d049bb133111eb;

            return combined ^ (combined >> 31);
        }
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Memory HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Internal::Memory
{

    namespace RefUtils // Note: Mainly used for supporting WeakRefs
    {

        ////////////////////////////////////////////////////////////////////////////////////
        // Ref Utilities
        ////////////////////////////////////////////////////////////////////////////////////
        void AddToLiveReferences(void* instance);
        void RemoveFromLiveReferences(void* instance);
        bool IsLive(void* instance);

    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Tracking type erased objects
    ////////////////////////////////////////////////////////////////////////////////////
    struct Tracked
    {
    public:
        using DestructorFn = void (*)(void* object);
    public:
        void* Object;
        DestructorFn Destructor;
    };

    template<bool Track> struct TrackedObjects  { std::vector<Tracked> Objects = { }; };
    template<> struct TrackedObjects<false>     { };

}

namespace Nano::Memory
{

    ////////////////////////////////////////////////////////////////////////////////////
    // DeferredConstruct<T>
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T
#if defined(NANO_EXPERIMENTAL)
        , bool Destroyable = false
#endif
    >
    class DeferredConstruct final //: public Traits::NoCopy, public Traits::NoMove
    {
    public:
        using ValueType = T;
    public:
        // Constructor & Destructor
        DeferredConstruct() noexcept(true) = default;
        inline ~DeferredConstruct() noexcept(std::is_nothrow_destructible_v<T>)
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
            #if defined(NANO_EXPERIMENTAL)
                if constexpr (Destroyable)
                {
                    if (!std::ranges::all_of(m_Storage, [](std::byte b) { return b == std::byte{ 0 }; }))
                    {
                        std::launder(reinterpret_cast<T*>(m_Storage))->~T();
                    }
                }
                else
            #endif
                {
                    std::launder(reinterpret_cast<T*>(m_Storage))->~T();
                }
            }
        }

        // Operators
        inline operator T& ()               noexcept(true) { return Get(); }
        inline operator const T& () const   noexcept(true) { return Get(); }
        inline operator T* ()               noexcept(true) { return std::launder(reinterpret_cast<T*>(m_Storage)); }
        inline operator const T* () const   noexcept(true) { return std::launder(reinterpret_cast<const T*>(m_Storage)); }

        inline T* operator -> () { return std::launder(reinterpret_cast<T*>(m_Storage)); }
        inline const T* operator -> () const { return std::launder(reinterpret_cast<const T*>(m_Storage)); }

        // Getters
        [[nodiscard]] inline T& Get()               noexcept(true) { return *std::launder(reinterpret_cast<T*>(m_Storage)); }
        [[nodiscard]] inline const T& Get() const   noexcept(true) { return *std::launder(reinterpret_cast<const T*>(m_Storage)); }

        // Methods
        template<typename ...Args>
        inline void Construct(Args&& ...args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
        {
            new (m_Storage) T(std::forward<Args>(args)...);
        }

    #if defined(NANO_EXPERIMENTAL)
        inline void Destroy() noexcept(std::is_nothrow_destructible_v<T>) requires(Destroyable)
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
                std::launder(reinterpret_cast<T*>(m_Storage))->~T();

            std::memset(m_Storage, 0, sizeof(m_Storage));
        }

        // Getter
        [[nodiscard]] inline bool IsConstructed() noexcept(true) requires(Destroyable)
        {
            return !(std::ranges::all_of(m_Storage, [](std::byte b) { return b == std::byte{ 0 }; }));
        }
    #endif

    private:
        // std::aligned_storage is deprecated as of C++23
        #if defined(NANO_COMPILER_GCC)
            #pragma GCC diagnostic push
            #pragma GCC diagnostic ignored "-Wpadded"
                alignas(T) std::byte m_Storage[sizeof(T)] = {};
            #pragma GCC diagnostic pop
        #elif !defined(NANO_PLATFORM_APPLE)
            #pragma warning(push)
            #pragma warning(disable: 4324)
                alignas(T) std::byte m_Storage[sizeof(T)] = {};
            #pragma warning(pop)
        #else
                alignas(T) std::byte m_Storage[sizeof(T)] = {};
        #endif
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // SparseSet<T>
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T, typename ID = uint64_t> requires(std::is_integral_v<ID>)
    class SparseSet final : public Traits::NoCopy
    {
    public:
        static constexpr ID InvalidID = std::numeric_limits<ID>::max();
    public:
        // Constructor & Destructor
        SparseSet() = default;
        ~SparseSet() noexcept(std::is_nothrow_destructible_v<T>) = default;

        // Methods
        void Add(ID id, const T& value) requires(std::is_copy_constructible_v<T>)
        {
            NANO_ASSERT(!Has(id), "Duplicate ID in SparseSet");

            if (id >= m_Sparse.size())
                m_Sparse.resize(id + 1, InvalidID);

            m_Sparse[id] = static_cast<ID>(m_IDs.size());
            m_IDs.push_back(id);
            m_Values.push_back(value);
        }

        void Add(ID id, T&& value) requires(std::is_move_constructible_v<T>)
        {
            NANO_ASSERT(!Has(id), "Duplicate ID in SparseSet");

            if (id >= m_Sparse.size())
                m_Sparse.resize(id + 1, InvalidID);

            m_Sparse[id] = static_cast<ID>(m_IDs.size());
            m_IDs.push_back(id);
            m_Values.emplace_back(std::move(value));
        }

        template<typename ...Args>
        void Add(ID id, Args&& ...args) requires (std::is_constructible_v<T, Args...>)
        {
            NANO_ASSERT(!Has(id), "Duplicate ID in SparseSet");

            if (id >= m_Sparse.size())
                m_Sparse.resize(id + 1, InvalidID);

            m_Sparse[id] = static_cast<ID>(m_IDs.size());
            m_IDs.push_back(id);
            m_Values.emplace_back(std::forward<Args>(args)...);
        }

        void Remove(ID id) noexcept(true)
        {
            using std::swap;

            NANO_ASSERT(Has(id), "ID not found in SparseSet");

            ID idx = m_Sparse[id];
            ID last = static_cast<ID>(m_IDs.size() - 1);

            // Swap-remove
            swap(m_IDs[idx], m_IDs[last]);
            swap(m_Values[idx], m_Values[last]);

            // Update sparse index of swapped-in ID
            m_Sparse[m_IDs[idx]] = idx;

            // Remove last
            m_IDs.pop_back();
            m_Values.pop_back();
            m_Sparse[id] = InvalidID;
        }

        // Getters
        [[nodiscard]] inline bool Has(ID id) const noexcept(true) { return id < m_Sparse.size() && m_Sparse[id] != InvalidID && m_Sparse[id] < m_IDs.size() && m_IDs[m_Sparse[id]] == id; }

        [[nodiscard]] inline T& Get(ID id)                noexcept(true) { NANO_ASSERT(Has(id), "No value present by this ID."); return m_Values[m_Sparse[id]]; }
        [[nodiscard]] inline const T& Get(ID id) const    noexcept(true) { NANO_ASSERT(Has(id), "No value present by this ID."); return m_Values[m_Sparse[id]]; }

        [[nodiscard]] inline size_t Size()                        const   noexcept(true) { return m_IDs.size(); }
        [[nodiscard]] inline std::vector<ID>& GetIDs()                    noexcept(true) { return m_IDs; }
        [[nodiscard]] inline const std::vector<ID>& GetIDs()      const   noexcept(true) { return m_IDs; }
        [[nodiscard]] inline std::vector<T>& GetValues()                  noexcept(true) { return m_Values; }
        [[nodiscard]] inline const std::vector<T>& GetValues()    const   noexcept(true) { return m_Values; }

        // Iterators
        inline auto begin()       noexcept(true) { return m_Values.begin(); }
        inline auto end()         noexcept(true) { return m_Values.end(); }
        inline auto begin() const noexcept(true) { return m_Values.begin(); }
        inline auto end()   const noexcept(true) { return m_Values.end(); }

    private:
        std::vector<ID> m_Sparse = { };
        std::vector<ID> m_IDs = { };
        std::vector<T> m_Values = { };
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // StaticString<N>
    ////////////////////////////////////////////////////////////////////////////////////
    template<size_t N>
    struct StaticString final
    {
    public:
        // Constructor & Destructor
        constexpr StaticString(std::string_view sv) noexcept(true)
        {
            std::copy(sv.begin(), sv.end(), m_Content.data());
        }
        constexpr ~StaticString() noexcept(true) = default;

        // Operators
        constexpr operator std::string_view() const noexcept(true) { return { m_Content.data(), N }; }

    private:
        std::array<char, N + 1> m_Content = {};
    };

#if defined(NANO_EXPERIMENTAL)
    ////////////////////////////////////////////////////////////////////////////////////
    // FixedStack
    ////////////////////////////////////////////////////////////////////////////////////
    template<size_t Size = 1024, bool TrackDestructors = true>
    class FixedStack final // Note: The stack is current on the stack, maybe should be on the heap.
    {
    public:
        // Constructor & Destructor
        FixedStack()
            : m_Stack(new std::byte[Size]) {
        }
        ~FixedStack()
        {
            if constexpr (TrackDestructors)
            {
                for (auto des = m_Tracked.Objects.rbegin(); des != m_Tracked.Objects.rend(); des++)
                    des->Destructor(des->Object);
            }

            delete[] m_Stack;
        }

        // Methods
        template<typename T, typename ...TArgs>
        [[nodiscard]] T* Allocate(TArgs&& ...args)
        {
            void* memory = Allocate(sizeof(T), alignof(T));
            T* obj = new (memory) T(std::forward<TArgs>(args)...);

            if constexpr (TrackDestructors && !std::is_trivially_destructible_v<T>)
                m_Tracked.Objects.emplace_back(obj, [](void* p) { static_cast<T*>(p)->~T(); });

            return obj;
        }

        template<typename T>
        void Destroy(T* object) requires(TrackDestructors)
        {
            size_t fullSize = sizeof(T) + GetPadding(reinterpret_cast<size_t>(object), alignof(T));
            NANO_ASSERT((reinterpret_cast<uint64_t>(static_cast<void*>(m_Stack - fullSize)) == (m_Used - fullSize)), "Trying to destroy an object not on the back of the stack.");

            auto& back = m_Tracked.Objects.back();
            NANO_ASSERT((reinterpret_cast<uint64_t>(static_cast<void*>(m_Stack - fullSize)) == back.Object), "Trying to destroy an object not on the back of the stack.");

            back.Destuctor(back.Object);
            m_Tracked.Objects.pop_back();
            m_Used -= fullSize;
        }

        // Extra
        void ReserveTracked(size_t objectCount) requires(TrackDestructors)
        {
            m_Tracked.Objects.reserve(objectCount);
        }

    private:
        // Private methods
        [[nodiscard]] void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t))
        {
            NANO_ASSERT(!(m_Used + size + alignment > Size), "Size + alignment exceeds the Stack's max size.");

            size_t padding = GetPadding(reinterpret_cast<size_t>(m_Stack + m_Used), alignment);

            NANO_ASSERT(!(m_Used + padding + size > Size), "Size + padding exceeds the Stack's max size.");

            void* ptr = m_Stack + m_Used + padding;
            m_Used += padding + size;
            return ptr;
        }

        [[nodiscard]] size_t GetPadding(size_t current, size_t alignment)
        {
            size_t aligned = (current + alignment - 1) & ~(alignment - 1);
            return aligned - current;
        }

    private:
        std::byte* m_Stack;
        size_t m_Used = 0;

        [[no_unique_address]]
        Internal::Memory::TrackedObjects<TrackDestructors> m_Tracked = {};
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // ArenaAllocator
    ////////////////////////////////////////////////////////////////////////////////////
    template<size_t Size = 1 << 20, bool TrackDestructors = true>
    class ArenaAllocator final // Note: Allocates in blocks
    {
    private:
        struct Block // Note: The capacity is the index + 1 times the templated Size
        {
        public:
            std::byte* Data;
            size_t Used;
        };
    public:
        // Constructor & Destructor
        ArenaAllocator()
        {
            AllocateBlock();
        }
        ~ArenaAllocator()
        {
            ResetBlocks();
        }

        // Methods
        template<typename T, typename ...TArgs>
        [[nodiscard]] T* Allocate(TArgs&& ...args)
        {
            void* memory = Allocate(sizeof(T), alignof(T));
            T* obj = new (memory) T(std::forward<TArgs>(args)...);

            if constexpr (TrackDestructors && !std::is_trivially_destructible_v<T>)

                m_Tracked.Objects.emplace_back(obj, [](void* p) { static_cast<T*>(p)->~T(); });
            return obj;
        }

        // Extra
        void ReserveTracked(size_t objectCount) requires(TrackDestructors)
        {
            m_Tracked.Objects.reserve(objectCount);
        }

        void ReserveBlocks(size_t blockCount)
        {
            m_Blocks.reserve(blockCount);
        }

    private:
        // Private methods
        void AllocateBlock()
        {
            size_t capacity = Size * (m_Blocks.size() + 1);
            m_Blocks.emplace_back(new std::byte[capacity], 0ull);
        }

        [[nodiscard]] void* Allocate(size_t size, size_t alignment = alignof(std::max_align_t))
        {
            if (m_Blocks.empty() || m_Blocks.back().Used + size + alignment > (m_Blocks.size() * Size))
                AllocateBlock();

            auto& b = m_Blocks.back();

            size_t padding = GetPadding(reinterpret_cast<size_t>(b.Data + b.Used), alignment);

            if (b.Used + padding + size > (m_Blocks.size() * Size))
            {
                AllocateBlock();
                return Allocate(size, alignment); // Retry on new block
            }

            void* ptr = b.Data + b.Used + padding;
            b.Used += padding + size;
            return ptr;
        }

        [[nodiscard]] size_t GetPadding(size_t current, size_t alignment)
        {
            size_t aligned = (current + alignment - 1) & ~(alignment - 1);
            return aligned - current;
        }

        void ResetBlocks()
        {
            if constexpr (TrackDestructors)
            {
                for (auto des = m_Tracked.Objects.rbegin(); des != m_Tracked.Objects.rend(); des++)
                    des->Destructor(des->Object);
            }

            for (auto block = m_Blocks.rbegin(); block != m_Blocks.rend(); block++)
                delete[] block->Data;
        }

    private:
        std::vector<Block> m_Blocks = { };

        [[no_unique_address]]
        Internal::Memory::TrackedObjects<TrackDestructors> m_Tracked = {};
    };
#endif

    ////////////////////////////////////////////////////////////////////////////////////
    // RefCounted
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    class Ref;

    class RefCounted
    {
    public:
        // Constructor & Destructor
        RefCounted() = default;
        virtual ~RefCounted() = default;

    protected:
        // Methods
        inline void IncRefCount() const { ++m_RefCount; }
        inline void DecRefCount() const { --m_RefCount; }

        // Getters
        [[nodiscard]] inline uint32_t GetRefCount() const { return m_RefCount.load(); }

    private:
        mutable std::atomic<uint32_t> m_RefCount = 0;

        template<typename T>
        friend class Ref;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Ref<T>
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    class Ref
    {
    public:
        // Constructors & Destructor
        Ref() requires(std::derived_from<T, RefCounted>) = default;
        inline Ref(std::nullptr_t n) requires(std::derived_from<T, RefCounted>)
            : m_Instance(nullptr) {}

        inline Ref(T* instance) requires(std::derived_from<T, RefCounted>)
            : m_Instance(instance)
        {
            IncRef();
        }

        inline Ref(const Ref<T>& other) requires(std::derived_from<T, RefCounted>)
            : m_Instance(other.m_Instance)
        {
            IncRef();
        }

        template<typename T2>
        inline Ref(const Ref<T2>& other) requires(std::derived_from<T, RefCounted>)
            : m_Instance(std::launder(reinterpret_cast<T*>(other.m_Instance)))
        {
            IncRef();
        }

        template<typename T2>
        inline Ref(Ref<T2>&& other) requires(std::derived_from<T, RefCounted>)
            : m_Instance(std::launder(reinterpret_cast<T*>(other.m_Instance)))
        {
            other.m_Instance = nullptr;
        }

        inline ~Ref()
        {
            DecRef();
        }

        // Operators
        Ref& operator = (std::nullptr_t n)
        {
            DecRef();
            m_Instance = nullptr;
            return *this;
        }

        Ref& operator = (const Ref<T>& other)
        {
            other.IncRef();
            DecRef();

            m_Instance = other.m_Instance;

            return *this;
        }

        template<typename T2>
        Ref& operator = (const Ref<T2>& other)
        {
            other.IncRef();
            DecRef();

            m_Instance = other.m_Instance;

            return *this;
        }

        template<typename T2>
        Ref& operator = (Ref<T2>&& other)
        {
            DecRef();

            m_Instance = other.m_Instance;

            other.m_Instance = nullptr;

            return *this;
        }

        inline bool operator == (const Ref<T>& other) const { return m_Instance == other.m_Instance; }
        inline bool operator != (const Ref<T>& other) const { return !(*this == other); }

        inline operator bool() { return m_Instance != nullptr; }
        inline operator bool() const { return m_Instance != nullptr; }

        inline T* operator -> () { return m_Instance; }
        inline const T* operator -> () const { return m_Instance; }

        inline T& operator * () { return *m_Instance; }
        inline const T& operator * () const { return *m_Instance; }

        // Getters
        [[nodiscard]] inline T* Raw() { return m_Instance; }
        [[nodiscard]] inline const T* Raw() const { return m_Instance; }

        // Methods
        void Reset(T* instance = nullptr)
        {
            DecRef();
            m_Instance = instance;
        }

        template<typename T2>
        [[nodiscard]] inline Ref<T2> As() const
        {
            return Ref<T2>(*this);
        }

        template<typename... TArgs>
        [[nodiscard]] inline static Ref<T> Create(TArgs&&... args)
        {
            return Ref<T>(new T(std::forward<TArgs>(args)...));
        }

        [[nodiscard]] bool EqualsObject(const Ref<T>& other)
        {
            if (!m_Instance || !other.m_Instance)
                return false;

            return *m_Instance == *other.m_Instance;
        }

    private:
        // Private methods
        void IncRef() const
        {
            if (m_Instance)
            {
                m_Instance->IncRefCount();

                Internal::Memory::RefUtils::AddToLiveReferences(static_cast<void*>(m_Instance));
            }
        }

        void DecRef() const
        {
            if (m_Instance)
            {
                m_Instance->DecRefCount();
                if (m_Instance->GetRefCount() == 0)
                {
                    delete m_Instance;

                    Internal::Memory::RefUtils::RemoveFromLiveReferences(static_cast<void*>(m_Instance));

                    m_Instance = nullptr;
                }
            }
        }

    private:
        mutable T* m_Instance = nullptr;

        template<typename T2>
        friend class Ref;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // WeakRef
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    class WeakRef
    {
    public:
        // Constructors & Destructor
        WeakRef() = default;
        WeakRef(T* ptr) requires(std::derived_from<T, RefCounted>)
            : m_Instance(ptr) {}
        WeakRef(Ref<T> ref) requires(std::derived_from<T, RefCounted>)
            : m_Instance(ref.Raw()) {}
        ~WeakRef() = default;

        // Operators
        inline operator bool() { return IsValid(); }
        inline operator bool() const { return IsValid(); }

        // Methods
        inline bool IsValid() const { return m_Instance ? Internal::Memory::RefUtils::IsLive(m_Instance) : false; }

        Ref<T> GetRef() const
        {
            return Ref<T>(m_Instance);
        }

        template<typename T2>
        WeakRef<T2> As() const
        {
            return WeakRef<T2>(std::launder(reinterpret_cast<T2*>(m_Instance)));
        }

    private:
        T* m_Instance = nullptr;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // StaticVector<T, N> // TODO: Add more functions
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T, size_t MaxElements>
    class StaticVector : private std::array<T, MaxElements>
    {
    public:
        using Base = std::array<T, MaxElements>;
    public:
        using typename Base::value_type;
        using typename Base::size_type;
        using typename Base::difference_type;
        using typename Base::reference;
        using typename Base::const_reference;
        using typename Base::pointer;
        using typename Base::const_pointer;
        using typename Base::iterator;
        using typename Base::const_iterator;
    public:
        // Constructors & Destructor
        inline constexpr StaticVector()
            : Base(), m_Size(0) {}
        inline NANO_DEBUG_CONSTEXPR StaticVector(size_t size)
            : Base(), m_Size(size)
        {
            NANO_ASSERT((size <= MaxElements), "[StaticVector] Size exceeds max elements.");
        }
        inline NANO_DEBUG_CONSTEXPR StaticVector(std::initializer_list<T> list)
            : m_Size(0)
        {
            for (auto i : list)
                push_back(i);
        }

        // Operators
        using Base::at;
        [[nodiscard]] inline NANO_DEBUG_CONSTEXPR reference operator [] (size_type pos)
        {
            NANO_ASSERT((pos < m_Size), "[StaticVector] Size exceeds max elements.");
            return Base::operator[](pos);
        }

        [[nodiscard]] inline NANO_DEBUG_CONSTEXPR const_reference operator[] (size_type pos) const
        {
            NANO_ASSERT((pos < m_Size), "[StaticVector] Size exceeds max elements.");
            return Base::operator[](pos);
        }

        // Methods
        using Base::front;
        [[nodiscard]] inline constexpr reference back() noexcept { auto tmp = end(); --tmp; return *tmp; }
        [[nodiscard]] inline constexpr const_reference back() const noexcept { auto tmp = cend(); --tmp; return *tmp; }

        // Iterators
        using Base::data;
        using Base::begin;
        using Base::cbegin;
        inline constexpr iterator end() noexcept { return iterator(begin()) + m_Size; }
        inline constexpr const_iterator end() const noexcept { return cend(); }
        inline constexpr const_iterator cend() const noexcept { return const_iterator(cbegin()) + m_Size; }

        // Helper methods
        [[nodiscard]] inline constexpr bool empty() const noexcept { return m_Size == 0; }
        [[nodiscard]] inline constexpr size_t size() const noexcept { return m_Size; }
        [[nodiscard]] inline constexpr size_t max_size() const noexcept { return MaxElements; }

        // Filling methods
        inline constexpr void fill(const T& value) noexcept
        {
            Base::fill(value);
            m_Size = MaxElements;
        }

        inline constexpr void swap(StaticVector& other) noexcept
        {
            Base::swap(*this);
            std::swap(m_Size, other.m_Size);
        }

        inline NANO_DEBUG_CONSTEXPR void push_back(const T& value) noexcept
        {
            NANO_ASSERT((m_Size < MaxElements), "[StaticVector] Size exceeds max elements.");
            *(data() + m_Size) = value;
            m_Size++;
        }

        inline NANO_DEBUG_CONSTEXPR void push_back(T&& value) noexcept
        {
            NANO_ASSERT((m_Size < MaxElements), "[StaticVector] Size exceeds max elements.");
            *(data() + m_Size) = std::move(value);
            m_Size++;
        }

        inline NANO_DEBUG_CONSTEXPR void pop_back() noexcept
        {
            NANO_ASSERT((m_Size > 0), "[StaticVector] Vector is empty.");
            m_Size--;
        }

        inline NANO_DEBUG_CONSTEXPR void resize(size_type newSize) noexcept
        {
            NANO_ASSERT((newSize <= MaxElements), "[StaticVector] Size exceeds max elements.");

            if (m_Size > newSize)
            {
                for (size_type i = newSize; i < m_Size; i++)
                    *(data() + i) = T{};
            }
            else
            {
                for (size_type i = m_Size; i < newSize; i++)
                    *(data() + i) = T{};
            }

            m_Size = newSize;
        }

        [[nodiscard]] inline constexpr reference emplace_back() noexcept
        {
            resize(m_Size + 1);
            return back();
        }

        // TODO: Emplace back with args

    private:
        size_type m_Size = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Helper methods
    ////////////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] inline NANO_DEBUG_CONSTEXPR size_t AlignOffset(size_t offset, size_t alignment = 4) // Note: Requires alignment to be a power of 2
    {
        NANO_ASSERT((alignment % 2 == 0), "Alignment must be a power of two");
        return (offset + (alignment - 1)) & ~(alignment - 1);
    }

    template<typename T>
    [[nodiscard]] inline NANO_DEBUG_CONSTEXPR T* AlignPointer(T* ptr, size_t alignment = alignof(T)) // Note: Requires alignment to be a power of 2
    {
        return std::launder(reinterpret_cast<T*>((reinterpret_cast<uintptr_t>(ptr) + (alignment - 1)) & ~(alignment - 1)));
    }

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Memory CPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
#if defined(NANO_IMPL_MEMORY)
namespace Nano::Internal::Memory
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Ref Objects
    ////////////////////////////////////////////////////////////////////////////////////
    namespace
    {
        static std::unordered_set<void*> s_LiveReferences = { };
        static std::shared_mutex s_LiveReferenceMutex = {};
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Ref Utilities
    ////////////////////////////////////////////////////////////////////////////////////
    void AddToLiveReferences(void* instance)
    {
        std::unique_lock<std::shared_mutex> lock(s_LiveReferenceMutex);
        s_LiveReferences.insert(instance);
    }

    void RemoveFromLiveReferences(void* instance)
    {
        std::unique_lock<std::shared_mutex> lock(s_LiveReferenceMutex);
        NANO_ASSERT((s_LiveReferences.find(instance) != s_LiveReferences.end()), "Trying to remove instance which doesn't exists");
        s_LiveReferences.erase(instance);
    }

    bool IsLive(void* instance)
    {
        std::shared_lock<std::shared_mutex> lock(s_LiveReferenceMutex);
        return s_LiveReferences.contains(instance);
    }

}
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Types HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Types
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Selector
    ////////////////////////////////////////////////////////////////////////////////////
    template<auto E, typename T> requires(std::is_enum_v<decltype(E)>)
    struct EnumToType
    {
    public:
        inline static constexpr bool IsEnumToType = true;
    public:
        inline static constexpr auto Value = E;
        using Type = T;
    };

}


namespace Nano::Internal::Types
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Internal naming
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    struct ConstexprName final : public Nano::Traits::NoConstruct
    {
    public:
        inline constexpr static const std::string_view Invalid = "1nvalid";
    public:
        [[nodiscard]] consteval static std::string_view TypeNameImpl() noexcept(true)
        {
            constexpr std::string_view fn = FunctionSignatureImpl();

            #if defined(NANO_COMPILER_MSVC) || defined(NANO_COMPILER_CLANG)
            constexpr std::string_view startSeq = "ConstexprName<";
            constexpr std::string_view endSeq = ">::FunctionSignatureImpl";

            size_t start = fn.find(startSeq);
            if (start == std::string_view::npos)
                return Invalid;
            start += startSeq.size();

            size_t end = fn.rfind(endSeq);
            if (end == std::string_view::npos)
                return Invalid;

            std::string_view typeName = fn.substr(start, end - start);
            if constexpr (std::is_class_v<T>)
            {
                // Check for nested define
                size_t colonPos = typeName.rfind("::");
                if (colonPos != std::string_view::npos)
                {
                    typeName = typeName.substr(colonPos + std::string_view("::").size()); // Move past the space
                }
                else
                {
                    size_t spacePos = typeName.rfind(' ');
                    if (spacePos != std::string_view::npos)
                        typeName = typeName.substr(spacePos + std::string_view(" ").size()); // Move past the space
                }
            }

            return typeName;

            #elif defined(NANO_COMPILER_GCC)

            constexpr std::string_view startSeq = "with T = ";
            constexpr std::string_view endSeq = ";";

            size_t start = fn.find(startSeq);
            if (start == std::string_view::npos)
                return Invalid;
            start += startSeq.size();

            size_t end = fn.rfind(endSeq);
            if (end == std::string_view::npos)
                return Invalid;

            std::string_view typeName = fn.substr(start, end - start);
            if constexpr (std::is_class_v<T>)
            {
                // Check for nested define
                size_t colonPos = typeName.rfind("::");
                if (colonPos != std::string_view::npos)
                {
                    typeName = typeName.substr(colonPos + std::string_view("::").size()); // Move past the space
                }
            }

            return typeName;

            #else
                #error Nano Types: Unsupported compiler...
            #endif
        }

        [[nodiscard]] consteval static std::string_view FunctionSignatureImpl() noexcept(true)
        {
            #if defined(NANO_COMPILER_MSVC)
                return { __FUNCSIG__, sizeof(__FUNCSIG__) };
            #elif defined(NANO_COMPILER_GCC) || defined(NANO_COMPILER_CLANG)
                return { __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) };
            #else
                #error Nano Types: Unsupported compiler...
            #endif
        }

    public:
        constexpr static const std::string_view TypeName = TypeNameImpl();
        constexpr static const std::string_view FunctionSignature = FunctionSignatureImpl();
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // TypeWrapper
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    struct TypeWrapper { using Type = T; };

    ////////////////////////////////////////////////////////////////////////////////////
    // Tuple
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename Tuple>
    inline constexpr size_t TupleTypeCount = std::tuple_size_v<Tuple>;

    template<size_t Index, typename Tuple>
    using TupleIndexedType = std::tuple_element_t<Index, Tuple>;

    template<typename T, typename Tuple, size_t Index = 0>
    [[nodiscard]] consteval size_t TupleTypeToIndex() noexcept(true)
    {
        if constexpr (Index < TupleTypeCount<Tuple> && std::is_same_v<T, TupleIndexedType<Index, Tuple>>) 
        {
            return Index;
        }
        else if constexpr (Index + 1 < TupleTypeCount<Tuple>) 
        {
            return TupleTypeToIndex<T, Tuple, Index + 1>();
        }
        else 
        {
            static_assert(Index + 1 <= TupleTypeCount<Tuple>, "Type not found in Tuple");
            return 0; // Note: Unreachable, just to satisfy return-type rules
        }
    }

    template<typename T, typename Tuple>
    struct TupleContains;

    template<typename T, typename... Types>
    struct TupleContains<T, std::tuple<Types...>> : std::bool_constant<(std::is_same_v<T, Types> || ...)> {};

    template<typename Tuple, typename TFunc, std::size_t... I>
    void ForEachTypeInTuple(TFunc&& func, std::index_sequence<I...>)
    {
        (func.template operator()<std::tuple_element_t<I, Tuple>>(), ...);
    }

    template<size_t Index, typename ...RemoveTypes, typename ...Types, typename ...NewTypes>
    [[nodiscard]] constexpr auto TupleRemoveTypes(const std::tuple<Types...>& values, const std::tuple<NewTypes...>& newTuple)
    {
        if constexpr (Index < TupleTypeCount<std::tuple<Types...>>)
        {
            if constexpr (TupleContains<TupleIndexedType<Index, std::tuple<Types...>>, std::tuple<RemoveTypes...>>::value)
            {
                return TupleRemoveTypes<Index + 1, RemoveTypes...>(values, newTuple);
            }
            else
            {
                return TupleRemoveTypes<Index + 1, RemoveTypes...>(values, std::tuple_cat(newTuple, std::tuple<TupleIndexedType<Index, std::tuple<Types...>>>{ std::get<Index>(values) }));
            }
        }
        else
        {
            return newTuple;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Selector
    ////////////////////////////////////////////////////////////////////////////////////
    template<size_t Index, typename TEnum, TEnum EValue, typename ...EnumToTypes>
    [[nodiscard]] consteval auto Selector() noexcept(true)
    {
        using TypesTuple = std::tuple<EnumToTypes...>;

        if constexpr (Index < TupleTypeCount<TypesTuple>)
        {
            using T = TupleIndexedType<Index, TypesTuple>;

            constexpr TEnum compValue = T::Value;
            using RetType = T::Type;

            if constexpr (compValue == EValue)
            {
                return TypeWrapper<RetType>();
            }
            else
            {
                return Selector<Index + 1, TEnum, EValue, EnumToTypes...>();
            }
        }
        else
        {
            static_assert(false, "Value not found in EnumToTypes...");
        }
    }

    template<auto EValue, typename ...EnumToTypes> requires(std::is_enum_v<decltype(EValue)>&& requires { ((EnumToTypes::IsEnumToType) && ...); })
    using SelectorType = decltype(Selector<0, decltype(EValue), EValue, EnumToTypes...>());

}

namespace Nano::Types
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Name
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    [[nodiscard]] consteval std::string_view Name() noexcept(true)
    {
        return Nano::Internal::Types::ConstexprName<T>::TypeName;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Tuple
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename Tuple>
    inline constexpr size_t TupleTypeCount = Nano::Internal::Types::TupleTypeCount<std::decay_t<Tuple>>;

    template<size_t Index, typename Tuple>
    using TupleIndexedType = Nano::Internal::Types::TupleIndexedType<Index, std::decay_t<Tuple>>;

    template<typename T, typename Tuple>
    inline constexpr size_t TupleTypeIndex = Nano::Internal::Types::TupleTypeToIndex<T, std::decay_t<Tuple>, 0>();

    template<typename T, typename Tuple>
    inline constexpr bool TupleContains = Nano::Internal::Types::TupleContains<T, std::decay_t<Tuple>>::value;

    template<typename Tuple, typename TFunc>
    constexpr void ForEachTypeInTuple(TFunc&& func)
    {
        Nano::Internal::Types::ForEachTypeInTuple<Tuple>(std::forward<TFunc>(func), std::make_index_sequence<TupleTypeCount<std::decay_t<Tuple>>>{});
    }

    template<typename ...RemoveTypes, typename ...Types>
    constexpr auto TupleRemoveTypes(const std::tuple<Types...>& values)
    {
        return Nano::Internal::Types::TupleRemoveTypes<0, RemoveTypes...>(values, std::tuple{});
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Selector
    ////////////////////////////////////////////////////////////////////////////////////
    template<auto EValue, typename ...EnumToTypes> requires(std::is_enum_v<decltype(EValue)>&& requires { ((EnumToTypes::IsEnumToType) && ...); })
    using SelectorType = Internal::Types::SelectorType<EValue, EnumToTypes...>::Type; 

}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Enum HPP --- // TODO: Make use of StaticString to avoid bloating exe
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Enum
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Customizations
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    struct Range : public Traits::NoConstruct
    {
    public:
        inline static constexpr int32_t Min = ((std::is_signed_v<std::underlying_type_t<TEnum>>) ? -128 : 0);
        inline static constexpr int32_t Max = ((std::is_signed_v<std::underlying_type_t<TEnum>>) ? 128 : 255);

        static_assert((Max - Min <= std::numeric_limits<uint16_t>::max()), "[Max - Min] must not exceed uint16 max value.");
    };

    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    struct Bitwise : public Traits::NoConstruct
    {
    public:
        inline static constexpr bool Enabled = false;
    };

}

namespace Nano::Internal::Enum
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Internal naming
    ////////////////////////////////////////////////////////////////////////////////////
    #if defined(NANO_COMPILER_MSVC)
    template<typename TEnum, TEnum EValue> requires(std::is_enum_v<TEnum>)
    class ConstexprName final : public Nano::Traits::NoConstruct
    {
    public:
        inline constexpr static const std::string_view ClassToken = "Nano::Internal::Enum::ConstexprName<";
        inline constexpr static const std::string_view Invalid = "1nvalid";

    public:
        template<TEnum> requires(std::is_enum_v<TEnum>)
        [[nodiscard]] consteval static std::string_view FullNameImpl() noexcept(true)
        {
            constexpr std::string_view FunctionToken = ">::FullNameImpl<";

            // Function signature
            constexpr std::string_view function = __FUNCSIG__;

            // Class token
            constexpr auto startClassToken = function.find(ClassToken);
            constexpr auto endClassToken = startClassToken + ClassToken.size();
            if constexpr (startClassToken == std::string_view::npos)
                return Invalid;

            // Function token
            constexpr auto startFunctionToken = function.find(FunctionToken, endClassToken);
            constexpr auto endFunctionToken = startFunctionToken + FunctionToken.size();
            if constexpr (startFunctionToken == std::string_view::npos)
                return Invalid;

            // Close marker
            constexpr auto closeMarker = function.find('>', endFunctionToken);
            if constexpr (closeMarker == std::string_view::npos)
                return Invalid;

            // Full name
            constexpr std::string_view fullName = function.substr(endFunctionToken, closeMarker - endFunctionToken);

            // Brackets (if brackets, then not a valid enum name, ex. (enum Dummy)0xa instead of Dummy::First)
            if constexpr (fullName.find('(') != std::string_view::npos)
                return Invalid;

            return fullName;
        }

        [[nodiscard]] consteval static std::string_view ElementNameImpl() noexcept(true)
        {
            constexpr std::string_view fullName = FullNameImpl<EValue>();

            // Check for invalid name
            if constexpr (!fullName.compare(Invalid))
                return Invalid;

            // '::' marker
            constexpr auto startColon = fullName.rfind("::");
            constexpr auto endColon = startColon + std::string_view("::").size();
            if constexpr (startColon == std::string_view::npos) // If not an enum class but a regular enum
                return fullName;

            // Element name
            constexpr std::string_view elementName = fullName.substr(endColon, fullName.size() - endColon);

            return elementName;
        }

        template<TEnum>
        [[nodiscard]] consteval static std::string_view FunctionSignatureImpl() noexcept(true)
        {
            return { __FUNCSIG__, sizeof(__FUNCSIG__) };
        }

    public:
        constexpr static const std::string_view FullName = FullNameImpl<EValue>();
        constexpr static const std::string_view ElementName = ElementNameImpl();

        constexpr static const std::string_view FunctionSignature = FunctionSignatureImpl<EValue>();
    };

    #elif defined(NANO_COMPILER_GCC) || defined(NANO_COMPILER_CLANG)
    template<typename TEnum, TEnum EValue> requires(std::is_enum_v<TEnum>)
    class ConstexprName final : public Nano::Traits::NoConstruct
    {
    public:
        inline constexpr static const std::string_view Invalid = "1nvalid";
    public:
        [[nodiscard]] consteval static std::string_view FullNameImpl() noexcept(true)
        {
            #if defined(NANO_COMPILER_CLANG)
            constexpr auto end = std::string_view(__PRETTY_FUNCTION__).find_last_of(']');
            if constexpr (end == std::string_view::npos)
                return Invalid;

            // '=' marker
            constexpr auto start = std::string_view(__PRETTY_FUNCTION__).rfind("EValue = ") + 7;
            if constexpr (start == std::string_view::npos)
                return Invalid;

            #else

            // Ending marker
            constexpr auto end = std::string_view(__PRETTY_FUNCTION__).find_last_of(';');
            if constexpr (end == std::string_view::npos)
                return Invalid;

            // '=' marker
            constexpr auto start = std::string_view(__PRETTY_FUNCTION__).find_last_of('=', end);
            if constexpr (start == std::string_view::npos)
                return Invalid;

#           endif

            // 0 <= start < end
            if constexpr (end - start <= 2)
                return Invalid;

            return std::string_view(__PRETTY_FUNCTION__).substr(start + 2, end - start - 2);
        }

        [[nodiscard]] consteval static std::string_view ElementNameImpl() noexcept(true)
        {
            constexpr std::string_view fullName = FullNameImpl();

            // Check for invalid name
            if constexpr (!fullName.compare(Invalid))
                return Invalid;

            // '::' marker
            constexpr auto startColon = fullName.rfind("::");
            constexpr auto endColon = startColon + std::string_view("::").size();
            if constexpr (startColon == std::string_view::npos) // If not an enum class but a regular enum
                return fullName;

            // Element name
            constexpr std::string_view elementName = fullName.substr(endColon, fullName.size() - endColon);

            return elementName;
        }

        [[nodiscard]] consteval static std::string_view FunctionSignatureImpl() noexcept(true)
        {
            return { __PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) };
        }

    public:
        constexpr static const std::string_view FullName = FullNameImpl();
        constexpr static const std::string_view ElementName = ElementNameImpl();

        constexpr static const std::string_view FunctionSignature = FunctionSignatureImpl();
    };

    #else
        #error Nano Enum: Unsupported compiler...
    #endif

    ////////////////////////////////////////////////////////////////////////////////////
    // Helper functions
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum, TEnum EValue> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval std::string_view Name() noexcept(true)
    {
        if constexpr (ConstexprName<TEnum, EValue>::ElementName == ConstexprName<TEnum, EValue>::Invalid)
            return {};

        return ConstexprName<TEnum, EValue>::ElementName;
    }

    template<typename TEnum, TEnum EValue> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval bool IsValid() noexcept(true)
    {
        constexpr TEnum value = static_cast<TEnum>(EValue);
        return !Name<TEnum, value>().empty();
    }

    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval TEnum UAlue(size_t v) noexcept(true)
    {
        return static_cast<TEnum>(Nano::Enum::Range<TEnum>::Min + v);
    }

    template<size_t N>
    [[nodiscard]] consteval size_t CountValues(const bool(&valid)[N]) noexcept(true)
    {
        size_t count = 0;
        for (size_t n = 0; n < N; n++)
        {
            if (valid[n])
                ++count;
        }

        return count;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Values
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum, size_t... I> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval auto ValuesImpl(std::index_sequence<I...>) noexcept(true)
    {
        constexpr bool valid[sizeof...(I)] = { IsValid<TEnum, UAlue<TEnum>(I)>()... };
        constexpr auto validCount = CountValues(valid);
        static_assert(validCount > 0, "No support for empty enums.");

        std::array<TEnum, validCount> values = {};
        for (size_t offset = 0, n = 0; n < validCount; offset++)
        {
            if (valid[offset])
            {
                values[n] = UAlue<TEnum>(offset);
                ++n;
            }
        }

        return values;
    }

    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval auto ValuesImpl() noexcept(true)
    {
        constexpr auto enumSize = Nano::Enum::Range<TEnum>::Max - Nano::Enum::Range<TEnum>::Min + 1;
        return ValuesImpl<TEnum>(std::make_index_sequence<enumSize>({}));
    }

    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    inline constexpr auto Values = ValuesImpl<TEnum>();

    ////////////////////////////////////////////////////////////////////////////////////
    // Entries
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum, size_t... I> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval auto EntriesImpl(std::index_sequence<I...>) noexcept(true)
    {
        return std::array<std::pair<TEnum, std::string_view>, sizeof...(I)>{
            { { Values<TEnum>[I], Name<TEnum, Values<TEnum>[I]>() }... }
        };
    }

    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    inline constexpr auto Entries = EntriesImpl<TEnum>(std::make_index_sequence<Values<TEnum>.size()>());

    ////////////////////////////////////////////////////////////////////////////////////
    // Utilities
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval size_t Count() noexcept(true)
    {
        constexpr auto values = Entries<TEnum>;
        return values.size();
    }

    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] consteval size_t Index(TEnum value) noexcept(true)
    {
        constexpr auto values = Entries<TEnum>;

        size_t index = 0;
        for (const auto& [v, n] : values)
        {
            if (v == value)
                return index;

            index++;
        }

        return index;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Bitwise
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum>
    concept BitwiseEnum = std::is_enum_v<TEnum> && Nano::Enum::Bitwise<TEnum>::Enabled;

    ////////////////////////////////////////////////////////////////////////////////////
    // Fusing
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] constexpr uintmax_t FuseOne(uintmax_t hash, TEnum value) noexcept(true)
    {
        constexpr auto typeHash = Hash::fnv1a(Nano::Types::Name<TEnum>());
        uintmax_t integerValue = static_cast<uintmax_t>(value);

        return (hash * 31) ^ (typeHash + integerValue);
    }

    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] constexpr uintmax_t Fuse(TEnum value) noexcept(true)
    {
        return FuseOne(0, value);
    }

    template<typename TEnum, typename... TEnums> requires(std::is_enum_v<TEnum> && (std::is_enum_v<TEnums> && ...))
    [[nodiscard]] constexpr uintmax_t Fuse(TEnum head, TEnums... tail) noexcept(true)
    {
        return FuseOne(Fuse(tail...), head);
    }

}

namespace Nano::Enum
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Name
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEnum> requires(std::is_enum_v<TEnum>)
    [[nodiscard]] constexpr std::string_view Name(TEnum value) noexcept(true)
    {
        constexpr const auto entries = Nano::Internal::Enum::Entries<TEnum>;

        for (const auto& [val, name] : entries)
        {
            if (val == value)
                return name;
        }

        return "";
    }

    // TODO: ...
    template<typename TEnum> requires(std::is_enum_v<TEnum> /*&& std::is_class_v<TEnum>*/)
    [[deprecated]] [[nodiscard]] constexpr std::string_view FullName(TEnum value) noexcept(true)
    {
        //return Nano::Types::Name<TEnum>() + "::" + Nano::Enum::Name(value);
        return "TODO";
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Fuse
    ////////////////////////////////////////////////////////////////////////////////////
    enum class Fused : uintmax_t;

    template<typename... TEnums>
    [[nodiscard]] constexpr Fused Fuse(TEnums... values) noexcept(true) requires((std::is_enum_v<TEnums> && ...))
    {
        static_assert(sizeof...(TEnums) >= 2, "Fuse requires at least 2 values.");
        return static_cast<Fused>(Internal::Enum::Fuse<std::decay_t<TEnums>...>(values...));
    }

}

template<Nano::Internal::Enum::BitwiseEnum TEnum>
constexpr auto operator & (TEnum lhs, TEnum rhs) noexcept(true)
{
    using U = std::underlying_type_t<TEnum>;
    return static_cast<TEnum>((static_cast<U>(lhs) & static_cast<U>(rhs)));
}

template<Nano::Internal::Enum::BitwiseEnum TEnum>
constexpr auto operator | (TEnum lhs, TEnum rhs) noexcept(true)
{
    using U = std::underlying_type_t<TEnum>;
    return static_cast<TEnum>((static_cast<U>(lhs) | static_cast<U>(rhs)));
}

template<Nano::Internal::Enum::BitwiseEnum TEnum>
constexpr auto operator ^ (TEnum lhs, TEnum rhs) noexcept(true)
{
    using U = std::underlying_type_t<TEnum>;
    return static_cast<TEnum>((static_cast<U>(lhs) ^ static_cast<U>(rhs)));
}

template<Nano::Internal::Enum::BitwiseEnum TEnum>
constexpr TEnum operator ~ (TEnum value) noexcept(true)
{
    using U = std::underlying_type_t<TEnum>;
    return TEnum(~static_cast<U>(value));
}

template<Nano::Internal::Enum::BitwiseEnum TEnum>
constexpr TEnum& operator &= (TEnum& lhs, TEnum rhs) noexcept(true)
{
    return lhs = (lhs & rhs);
}

template<Nano::Internal::Enum::BitwiseEnum TEnum>
constexpr TEnum& operator |= (TEnum& lhs, TEnum rhs) noexcept(true)
{
    return lhs = (lhs | rhs);
}

template<Nano::Internal::Enum::BitwiseEnum TEnum>
constexpr TEnum& operator ^= (TEnum& lhs, TEnum rhs) noexcept(true)
{
    return lhs = (lhs ^ rhs);
}

#define NANO_DEFINE_BITWISE(TEnum)                                                  \
    constexpr auto operator & (TEnum lhs, TEnum rhs) noexcept(true)                 \
    {                                                                               \
        using U = std::underlying_type_t<TEnum>;                                    \
        return static_cast<TEnum>((static_cast<U>(lhs) & static_cast<U>(rhs)));     \
    }                                                                               \
                                                                                    \
    constexpr auto operator | (TEnum lhs, TEnum rhs) noexcept(true)                 \
    {                                                                               \
        using U = std::underlying_type_t<TEnum>;                                    \
        return static_cast<TEnum>((static_cast<U>(lhs) | static_cast<U>(rhs)));     \
    }                                                                               \
                                                                                    \
    constexpr auto operator ^ (TEnum lhs, TEnum rhs) noexcept(true)                 \
    {                                                                               \
        using U = std::underlying_type_t<TEnum>;                                    \
        return static_cast<TEnum>((static_cast<U>(lhs) ^ static_cast<U>(rhs)));     \
    }                                                                               \
                                                                                    \
    constexpr TEnum operator ~ (TEnum value) noexcept(true)                         \
    {                                                                               \
        using U = std::underlying_type_t<TEnum>;                                    \
        return TEnum(~static_cast<U>(value));                                       \
    }                                                                               \
                                                                                    \
    constexpr TEnum& operator &= (TEnum& lhs, TEnum rhs) noexcept(true)             \
    {                                                                               \
        return lhs = (lhs & rhs);                                                   \
    }                                                                               \
                                                                                    \
    constexpr TEnum& operator |= (TEnum& lhs, TEnum rhs) noexcept(true)             \
    {                                                                               \
        return lhs = (lhs | rhs);                                                   \
    }                                                                               \
                                                                                    \
    constexpr TEnum& operator ^= (TEnum& lhs, TEnum rhs) noexcept(true)             \
    {                                                                               \
        return lhs = (lhs ^ rhs);                                                   \
    }

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Scope HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Utils
{

    ////////////////////////////////////////////////////////////////////////////////////
    // ScopeExit
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TFunc> requires(std::invocable<TFunc>)
    class ScopeExit final : public Traits::NoCopy, public Traits::NoMove
    {
    public:
        // Constructor & Destructor
        inline explicit ScopeExit(TFunc&& func) noexcept(true)
            : m_Func(std::move(func)) {}
        inline ~ScopeExit() noexcept(std::is_nothrow_invocable_v<TFunc>)
        {
            if (m_Active)
                m_Func();
        }

        // Method
        inline void Release() noexcept(true) { m_Active = false; }

    private:
        TFunc m_Func;
        bool m_Active = true;
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Functional HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Functional
{

    ////////////////////////////////////////////////////////////////////////////////////
    // FunctionConversion
    ////////////////////////////////////////////////////////////////////////////////////
    class Conversion final : public Traits::NoConstruct
    {
    public:
        // Converts a member function into a static-like function
        template<typename ObjType, typename ReturnType, typename... Args>
        [[nodiscard]] constexpr static auto ToStatic(ReturnType(ObjType::* memFn)(Args...)) noexcept(true)
        {
            return [memFn](ObjType* obj, Args... args) -> ReturnType { return (obj->*memFn)(args...); };
        }

        /*
        // Calls the constructor with the native type
        template<typename Type, typename ArgType>
        static Type UseNativeArgTypeInType(std::any arg);

        // Calls the function with the native type
        template<typename Func, typename ArgType>
        static auto UseNativeArgTypeInFunc(Func&& func, std::any arg) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;

        // Calls the function with the native type in a made ^static member function^
        template<typename Func, typename ClassType, typename ArgType>
        static auto UseNativeArgTypeInSMemFunc(Func&& func, ClassType* instance, std::any arg) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;

        // Calls the constructor with all the native types
        template<typename Type, typename ...Args>
        static Type UseNativeArgTypesInType(const std::vector<std::any>& args);

        // Calls the function with all the native types
        template<typename Func, typename ...Args>
        static auto UseNativeArgTypesInFunc(Func&& func, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;

        // Calls the function with all the native types in a made ^static member function^
        template<typename Func, typename ClassType, typename ...Args>
        static auto UseNativeArgTypesInSMemFunc(Func&& func, ClassType* instance, const std::vector<std::any>& args) -> typename Pulse::Types::FunctionTraits<Func>::ReturnType;
        */
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Time HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Time
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Formats
    ////////////////////////////////////////////////////////////////////////////////////
    namespace Period
    {
        struct Years            { using Ratio = std::chrono::years::period; };
        struct Months           { using Ratio = std::chrono::months::period; };
        struct Weeks            { using Ratio = std::chrono::weeks::period; };
        struct Days             { using Ratio = std::chrono::days::period; };
        struct Hours            { using Ratio = std::chrono::hours::period; };
        struct Minutes          { using Ratio = std::chrono::minutes::period; };
        struct Seconds          { using Ratio = std::chrono::seconds::period; };
        struct Milliseconds     { using Ratio = std::chrono::milliseconds::period; };
        struct Microseconds     { using Ratio = std::chrono::microseconds::period; };
        struct Nanoseconds      { using Ratio = std::chrono::nanoseconds::period; };

        template<typename TPeriod>
        concept IsPeriod = requires { typename TPeriod::Ratio; };
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Casting
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename FromPeriod, typename ToPeriod, typename Representation = uint64_t>
    [[nodiscard]] inline Representation Cast(Representation from) noexcept(true) requires(Period::IsPeriod<FromPeriod> && Period::IsPeriod<ToPeriod>)
    {
        std::chrono::duration_cast<std::chrono::duration<Representation, typename ToPeriod::Ratio>>(std::chrono::duration<Representation, typename FromPeriod::Ratio>(from)).count();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TimePeriod = Period::Milliseconds, typename Representation = uint64_t>
    [[nodiscard]] inline Representation SinceEpoch() noexcept(true) requires(Period::IsPeriod<TimePeriod>)
    {
        return std::chrono::duration_cast<std::chrono::duration<Representation, typename TimePeriod::Ratio>>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    template<typename TimePeriod = Period::Milliseconds, typename Representation = uint64_t>
    [[nodiscard]] inline Representation SteadyTime() noexcept(true) requires(Period::IsPeriod<TimePeriod>) // Note: This will just increase normally even when user changes system time.
    {
        return std::chrono::duration_cast<std::chrono::duration<Representation, typename TimePeriod::Ratio>>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    template<typename TimePeriod = Period::Milliseconds, typename Representation = uint64_t>
    [[nodiscard]] inline Representation HighResTime() noexcept(true) requires(Period::IsPeriod<TimePeriod>) // Note: This is accurate to a microsecond (below that is often rounded up/down, due to internal timer interval)
    {
        if constexpr (std::chrono::high_resolution_clock::is_steady)
        {
            return std::chrono::duration_cast<std::chrono::duration<Representation, typename TimePeriod::Ratio>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }
        else
        {
            return std::chrono::duration_cast<std::chrono::duration<Representation, typename TimePeriod::Ratio>>(std::chrono::steady_clock::now().time_since_epoch()).count();
        }
    }

    template<typename TimePeriod = Period::Milliseconds, typename Representation = double>
    inline void Sleep(Representation time) // Note: Makes the current thread sleep
    {
        std::this_thread::sleep_for(std::chrono::duration<Representation, typename TimePeriod::Ratio>(time));
    }

    template<typename TimePeriod = Period::Milliseconds, typename Representation = double>
    inline void BusyWait(Representation time) noexcept(true) // Note: Uses a lot of CPU (but is really accurate)
    {
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::duration<Representation, typename TimePeriod::Ratio>(time));
    }

    [[nodiscard]] std::string String(std::string_view format = "%H:%M:%S");

    ////////////////////////////////////////////////////////////////////////////////////
    // Timer
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TimePeriod = Period::Milliseconds, typename Representation = double> requires(Period::IsPeriod<TimePeriod>)
    class Timer final // Note: Uses high resolution clock
    {
    public:
        // Constructor & Destructor
        inline Timer(bool startOnConstruct = true) noexcept(true)
        {
            if (startOnConstruct)
                Start();
        }
        ~Timer() noexcept(true) = default;

        // Methods
        inline void Start() noexcept(true)
        {
            m_Start = HighResTime<TimePeriod, Representation>();
        }

        [[nodiscard]] Representation Restart() noexcept(true) // Note: Don't discard value, instead call Start() if the active time is not used
        {
            Representation end = HighResTime<TimePeriod, Representation>();
            Representation time = end - m_Start;
            m_Start = end;
            return time;
        }

        [[nodiscard]] inline Representation TimeActive() const noexcept(true)
        {
            return HighResTime<TimePeriod, Representation>() - m_Start;
        }

        // Operators
        inline operator Representation() const noexcept(true) { return TimeActive(); }

        // Getters
        [[nodiscard]] inline Representation GetStart() const noexcept(true) { return m_Start; }

    private:
        Representation m_Start = static_cast<Representation>(0);
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Time CPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
#if defined(NANO_IMPL_TIME)
namespace Nano::Time
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] std::string String(std::string_view format)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&nowTime), format.data());

        return oss.str();
    }

}
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Random HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Random
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Random
    ////////////////////////////////////////////////////////////////////////////////////
    class Random final : Traits::NoConstruct
    {
    public:
        // Seeding
        inline static void Seed(uint64_t seed = Time::SinceEpoch<Time::Period::Milliseconds, uint64_t>())
        {
            s_RandomEngine.seed(static_cast<uint32_t>(seed));
        }

        // Floating point
        [[nodiscard]] inline static float Float(float min = 0.0f, float max = 1.0f)
        {
            return min + s_FloatDist(s_RandomEngine) * (max - min);
        }

        [[nodiscard]] inline static double Double(double min = 0.0, double max = 1.0)
        {
            return min + s_DoubleDist(s_RandomEngine) * (max - min);
        }

        // Integer 
        [[nodiscard]] inline static int32_t Int(int32_t min = std::numeric_limits<int32_t>::min(), int32_t max = std::numeric_limits<int32_t>::max())
        {
            s_IntDist.param(typename decltype(s_IntDist)::param_type{ min, max });
            return s_IntDist(s_RandomEngine);
        }

        [[nodiscard]] inline static uint32_t UInt(uint32_t min = std::numeric_limits<uint32_t>::min(), uint32_t max = std::numeric_limits<uint32_t>::max())
        {
            s_UIntDist.param(typename decltype(s_UIntDist)::param_type{ min, max });
            return s_UIntDist(s_RandomEngine);
        }

        // Boolean
        [[nodiscard]] inline static bool Bool(float trueProbability = 0.5f)
        {
            return s_FloatDist(s_RandomEngine) < trueProbability;
        }

        // Utils
        [[nodiscard]] inline static bool Chance(float percentage)
        {
            return Float(0.0f, 100.0f) < percentage;
        }

    private:
        static thread_local std::mt19937 s_RandomEngine;
        static thread_local std::uniform_int_distribution<int> s_IntDist;
        static thread_local std::uniform_int_distribution<unsigned int> s_UIntDist;
        static thread_local std::uniform_real_distribution<float> s_FloatDist;
        static thread_local std::uniform_real_distribution<double> s_DoubleDist;
    };

    inline thread_local std::mt19937								Random::s_RandomEngine(static_cast<uint32_t>(Time::SinceEpoch<Time::Period::Milliseconds, uint64_t>()));
    inline thread_local std::uniform_int_distribution<int>		    Random::s_IntDist;
    inline thread_local std::uniform_int_distribution<unsigned int>	Random::s_UIntDist;
    inline thread_local std::uniform_real_distribution<float>		Random::s_FloatDist(0.0f, 1.0f);
    inline thread_local std::uniform_real_distribution<double>		Random::s_DoubleDist(0.0, 1.0);

    ////////////////////////////////////////////////////////////////////////////////////
    // FastRandom
    ////////////////////////////////////////////////////////////////////////////////////
    class FastRandom final : public Traits::NoConstruct
    {
    public:
        // Seeding
        inline static void Seed(uint64_t seed = Time::SinceEpoch<Time::Period::Milliseconds, uint64_t>())
        {
            s_State = static_cast<uint32_t>(seed);
        }

        // Floating point
        [[nodiscard]] inline static float Float(float min = 0.0f, float max = 1.0f)
        {
            return min + (Next() / static_cast<float>(std::numeric_limits<uint32_t>::max())) * (max - min);
        }

        [[nodiscard]] inline static double Double(double min = 0.0, double max = 1.0)
        {
            return min + (Next() / static_cast<double>(std::numeric_limits<uint32_t>::max())) * (max - min);
        }

        // Integer
        [[nodiscard]] inline static int32_t Int(int32_t min = std::numeric_limits<int32_t>::min(), int32_t max = std::numeric_limits<int32_t>::max())
        {
            return min + static_cast<int32_t>(Next() % (static_cast<uint64_t>(max) - static_cast<uint64_t>(min) + 1));
        }

        [[nodiscard]] inline static uint32_t UInt(uint32_t min = std::numeric_limits<uint32_t>::min(), uint32_t max = std::numeric_limits<uint32_t>::max())
        {
            return min + (Next() % (static_cast<uint64_t>(max) - static_cast<uint64_t>(min) + 1));
        }

        // Boolean
        [[nodiscard]] inline static bool Bool(float trueProbability = 0.5f)
        {
            return Float(0.0f, 1.0f) < trueProbability;
        }

        // Utils
        [[nodiscard]] inline static bool Chance(float percentage)
        {
            return Float(0.0f, 100.0f) < percentage;
        }

    private:
        // Private methods
        [[nodiscard]] inline static uint64_t Next() noexcept(true) { return NextXORShift64(); } // Chosen FastRandom method

        [[nodiscard]] static uint64_t NextXORShift64()    noexcept(true); // Very fast, low quality
        [[nodiscard]] static uint64_t NextSplitMix64()    noexcept(true); // Great for seeding
        [[nodiscard]] static uint64_t NextPCG32()         noexcept(true); // Great balance, highly recommended

    private:
        static thread_local uint64_t s_State;
    };

    inline thread_local uint64_t FastRandom::s_State = static_cast<uint32_t>(Time::SinceEpoch<Time::Period::Milliseconds, uint64_t>());

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Random CPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
#if defined(NANO_IMPL_RANDOM)
namespace Nano::Random
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Private methods
    ////////////////////////////////////////////////////////////////////////////////////
    uint64_t FastRandom::NextXORShift64() noexcept(true)
    {
        s_State ^= s_State << 13;
        s_State ^= s_State >> 7;
        s_State ^= s_State << 17;
        return s_State;
    }

    uint64_t FastRandom::NextSplitMix64() noexcept(true)
    {
        s_State += 0x9E3779B97F4A7C15ULL;
        uint64_t z = s_State;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return (z ^ (z >> 31));
    }

    uint64_t FastRandom::NextPCG32() noexcept(true)
    {
        uint64_t oldstate = s_State;
        s_State = oldstate * 6364136223846793005ULL + 1442695040888963407ULL;
        uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
        uint32_t rot = static_cast<uint32_t>(oldstate >> 59u);
        return (xorshifted >> rot) | (static_cast<uint64_t>(xorshifted) << ((rot) & 31));
    }

}
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Maths HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Maths
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Utility
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T>
    [[nodiscard]] NANO_DEBUG_CONSTEXPR uint8_t CountNumbers(T value) noexcept(true) requires(std::is_arithmetic_v<T>)
    {
        std::array<char, 64> buffer = { };
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);

        NANO_ASSERT(ec == std::errc(), "Failed to convert number to string.");

        std::string_view view(buffer.data(), ptr - buffer.data());

        if constexpr (std::is_floating_point_v<T>) 
        {
            auto dot_pos = view.find('.');
            return static_cast<uint8_t>(dot_pos == std::string_view::npos ? view.size() : dot_pos);
        }

        return static_cast<uint8_t>(view.size()); // For integers, it's all before the dot.
    }

    template<typename T>
    [[nodiscard]] std::string ToString(T value) requires(std::is_arithmetic_v<T>)
    {
        std::array<char, 64> buffer = { };
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value);

        NANO_ASSERT(ec == std::errc(), "Failed to convert number to string.");

        std::string_view view(buffer.data(), ptr - buffer.data());
        return std::string(view);
    }

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- ErrorHandling HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Errors
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Expected<T, E>
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename T, typename E>
    class Expected final : public Traits::NoCopy, public Traits::NoMove
    {
    public:
        using ValueType = T;
        using ErrorType = E;
    private:
        // Utilitites
        template<typename T2>               struct _GetExpectedErrorType;
        template<typename T2, typename E2>  struct _GetExpectedErrorType<Expected<T2, E2>> { using Type = E2; };
    public:
        // Constructors & Destructor // TODO: Add Args&& constructors?
        inline constexpr Expected(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
            : m_Value(value), m_HasValue(true) {}
        inline constexpr Expected(T&& value) noexcept(true)
            : m_Value(std::move(value)), m_HasValue(true) {}
        inline constexpr Expected(const E& error) noexcept(std::is_nothrow_copy_constructible_v<E>)
            : m_Error(error), m_HasValue(false) {}
        inline constexpr Expected(E&& error) noexcept(true)
            : m_Error(std::move(error)), m_HasValue(false) {}
        inline constexpr ~Expected()
        {
            if (m_HasValue)
            {
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    m_Value.~T();
                }
            }
            else
            {
                if constexpr (!std::is_trivially_destructible_v<E>)
                {
                    m_Error.~E();
                }
            }
        }
        
        // Operators
        inline NANO_DEBUG_CONSTEXPR operator T& ()                 noexcept(true)  { CheckConstructed(); return m_Value; }
        inline NANO_DEBUG_CONSTEXPR operator const T& ()   const   noexcept(true)  { CheckConstructed(); return m_Value; }
        inline NANO_DEBUG_CONSTEXPR operator T* ()                 noexcept(true)  { CheckConstructed(); return m_Value; }
        inline NANO_DEBUG_CONSTEXPR operator const T* ()   const   noexcept(true)  { CheckConstructed(); return m_Value; }

        inline NANO_DEBUG_CONSTEXPR T* operator -> ()              noexcept(true)  { CheckConstructed(); return m_Value; }
        inline NANO_DEBUG_CONSTEXPR const T* operator -> () const                  { CheckConstructed(); return m_Value; }

        inline constexpr explicit operator bool() const noexcept(true) { return m_HasValue; }
        
        // Value
        [[nodiscard]] inline NANO_DEBUG_CONSTEXPR T& Get()               noexcept(true) { CheckConstructed(); return m_Value; }
        [[nodiscard]] inline NANO_DEBUG_CONSTEXPR const T& Get() const   noexcept(true) { CheckConstructed(); return m_Value; }

        // Error
        [[nodiscard]] inline NANO_DEBUG_CONSTEXPR E& Error()             noexcept(true)  { ErrorPresent(); return m_Value; }
        [[nodiscard]] inline NANO_DEBUG_CONSTEXPR const E& Error() const noexcept(true)  { ErrorPresent(); return m_Value; }

        // Getters
        [[nodiscard]] inline constexpr bool HasValue() const noexcept(true) { return m_HasValue; }
        [[nodiscard]] inline constexpr bool HasError() const noexcept(true) { return !m_HasValue; }

        // Methods
        template<typename TFunc, typename ...TArgs>
        constexpr auto AndThen(TFunc&& f, TArgs&& ...args) & noexcept(std::is_nothrow_invocable_v<TFunc, T&, TArgs...>) requires (std::invocable<TFunc, T&, TArgs...>)
        {
            using Result = std::invoke_result_t<TFunc, T&, TArgs...>;
            static_assert(std::is_same_v<typename _GetExpectedErrorType<Result>::Type, E>, "Mismatched error types");

            if (m_HasValue)
            {
                return std::invoke(std::forward<TFunc>(f), m_Value, std::forward<TArgs>(args)...);
            }

            return Result(m_Error);
        }

        template<typename TFunc, typename ...TArgs>
        constexpr auto AndThen(TFunc&& f, TArgs&& ...args) const& noexcept(std::is_nothrow_invocable_v<TFunc, const T&, TArgs...>) requires (std::invocable<TFunc, const T&, TArgs...>)
        {
            using Result = std::invoke_result_t<TFunc, const T&, TArgs...>;
            static_assert(std::is_same_v<typename _GetExpectedErrorType<Result>::Type, E>, "Mismatched error types");

            if (m_HasValue)
            {
                return std::invoke(std::forward<TFunc>(f), m_Value, std::forward<TArgs>(args)...);
            }

            return Result(m_Error);
        }

        template<typename TFunc, typename ...TArgs>
        constexpr auto AndThen(TFunc&& f, TArgs&& ...args) && noexcept(std::is_nothrow_invocable_v<TFunc, const T&, TArgs...>) requires (std::invocable<TFunc, const T&, TArgs...>)
        {
            using Result = std::invoke_result_t<TFunc, const T&, TArgs...>;
            static_assert(std::is_same_v<typename _GetExpectedErrorType<Result>::Type, E>, "Mismatched error types");

            if (m_HasValue)
            {
                return std::invoke(std::forward<TFunc>(f), m_Value, std::forward<TArgs>(args)...);
            }

            return Result(m_Error);
        }

    private:
        // Debug method
        inline NANO_DEBUG_CONSTEXPR void CheckConstructed() const noexcept(true)
        {
            #if defined(NANO_DEBUG_EXPECTED)
            NANO_ASSERT(m_HasValue, "Object not constructed yet.");
            #endif
        }

        inline NANO_DEBUG_CONSTEXPR void ErrorPresent() const noexcept(true)
        {
            #if defined(NANO_DEBUG_EXPECTED)
            NANO_ASSERT(!m_HasValue, "No error present.");
            #endif
        }

    private:
        union
        {
            T m_Value;
            E m_Error;
        };
        bool m_HasValue;
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Threading HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Threading
{

    ////////////////////////////////////////////////////////////////////////////////////
    // AsyncMode
    ////////////////////////////////////////////////////////////////////////////////////
    enum class AsyncMode : uint8_t { Immediate = 0, Deferred };

}

namespace Nano::Internal::Threading
{

    ////////////////////////////////////////////////////////////////////////////////////
    // AsyncMode
    ////////////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] inline constexpr std::launch AsyncModeToLauch(Nano::Threading::AsyncMode mode) noexcept(true)
    {
        switch (mode)
        {
        case Nano::Threading::AsyncMode::Immediate:  return std::launch::async;
        case Nano::Threading::AsyncMode::Deferred:   return std::launch::deferred;
        }

        return std::launch::async;
    }
}

namespace Nano::Threading // TODO: Add concurrency in the future
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Utilities
    ////////////////////////////////////////////////////////////////////////////////////
    [[nodiscard]] inline uint64_t ID()                noexcept(true) { return std::hash<std::thread::id>()(std::this_thread::get_id()); }
    [[nodiscard]] inline uint32_t HardwareThreads()   noexcept(true) { return std::thread::hardware_concurrency(); }

    ////////////////////////////////////////////////////////////////////////////////////
    // Async
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TFunc, typename ...TArgs>
    inline auto Async(AsyncMode mode, TFunc&& func, TArgs&& ...args) -> std::future<std::invoke_result_t<TFunc, TArgs...>> requires (std::invocable<TFunc, TArgs...>)
    {
        return std::async(Nano::Internal::Threading::AsyncModeToLauch(mode), std::forward<TFunc>(func), std::forward<TArgs>(args)...);
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // ThreadPool
    ////////////////////////////////////////////////////////////////////////////////////
    class ThreadPool final : public Traits::NoCopy
    {
    public:
        // Constructor & Destructor
        ThreadPool(uint32_t threadCount = HardwareThreads());
        ~ThreadPool();

        // Methods
        template<typename TFunc, typename ...Args>
        auto Enqueue(TFunc&& func, Args&& ...args) -> std::future<std::invoke_result_t<TFunc, Args...>> requires (std::invocable<TFunc, Args...>)
        {
            std::packaged_task<std::invoke_result_t<TFunc, Args...>()> task(std::forward<TFunc>(func), std::forward<Args>(args)...);
            auto future = task.get_future();

            std::move_only_function<void()> wrapper = [task = std::move(task)]() mutable { std::move(task)(); };
            {
                std::scoped_lock<std::mutex> lock(m_QueueMutex);
                m_Tasks.push(std::move(wrapper));
                m_TaskAvailable.notify_one();
            }

            return future;
        }

    private:
        std::vector<std::thread> m_Threads;

        std::queue<std::move_only_function<void()>> m_Tasks = {};

        std::mutex m_QueueMutex = {};
        std::condition_variable m_TaskAvailable = {};
        bool m_Stop = false;
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Threading CPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
#if defined(NANO_IMPL_THREADING)
namespace Nano::Threading
{
    
    ////////////////////////////////////////////////////////////////////////////////////
    // Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    ThreadPool::ThreadPool(uint32_t threadCount)
    {
        m_Threads.reserve(static_cast<size_t>(threadCount));
        
        for (size_t i = 0; i < threadCount; i++)
        {
            m_Threads.emplace_back([this] () 
            {
                while (true) 
                {
                    std::move_only_function<void()> task;
                    
                    {
                        std::unique_lock<std::mutex> lock(m_QueueMutex);

                        m_TaskAvailable.wait(lock, [this] { return !m_Tasks.empty() || m_Stop; });

                        // Exit the thread if asked to stop and no tasks available
                        if (m_Stop && m_Tasks.empty())
                            return;

                        // Get the next task from the queue
                        task = std::move(m_Tasks.front());
                        m_Tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::scoped_lock<std::mutex> lock(m_QueueMutex);
            m_Stop = true;
        }

        // Notify all threads
        m_TaskAvailable.notify_all();

        // Wait for all tasks
        for (auto& thread : m_Threads)
            thread.join();
    }

}
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Events HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Events
{

    ////////////////////////////////////////////////////////////////////////////////////
    // EventDispatcher
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEventVariant> requires requires { typename std::variant_size<TEventVariant>::type; }
    class EventDispatcher final
    {
    public:
        using SubscribeFn = std::function<void(TEventVariant& e)>;
    public:
        // Constructor & Destructor
        EventDispatcher() { m_Subscribers.reserve(5); }
        ~EventDispatcher() = default;

        // Methods
        void Dispatch(TEventVariant& e)
        {
            for (const auto& subscriber : m_Subscribers)
                subscriber(e);
        }

        template<typename TFunc>
        void Subscribe(TFunc&& func) requires(std::invocable<TFunc, TEventVariant&>)
        {
            m_Subscribers.emplace_back(std::forward<TFunc>(func));
        }

    private:
        std::vector<SubscribeFn> m_Subscribers = {};
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // EventHandler
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename TEventVariant> requires requires { typename std::variant_size<TEventVariant>::type; }
    class EventHandler final
    {
    public:
        // Constructor & Destructor
        EventHandler(TEventVariant& e)
            : m_Event(e) {}
        ~EventHandler() = default;

        // Methods
        template<typename TEvent, typename TFunc>
        void Handle(TFunc&& func)
        {
            if (std::holds_alternative<TEvent>(m_Event))
                func(std::get<TEvent>(m_Event));
        }

    private:
        TEventVariant& m_Event;
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- ECS HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Internal::ECS
{
    
    ////////////////////////////////////////////////////////////////////////////////////
    // Storage
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename ID, typename ...Components> requires(std::is_integral_v<ID>)
    class Storage final
    {
    public:
        using TypesTuple = std::tuple<Components...>;
        using StorageTuple = std::tuple<Nano::Memory::SparseSet<Components, ID>...>;
    public:
        // Constructor & Destructor
        Storage() = default;
        ~Storage() = default;

        // Methods
        template<typename TComponent>
        void AddComponent(ID id, const TComponent& component) requires(Nano::Types::TupleContains<TComponent, TypesTuple> && std::is_copy_constructible_v<TComponent>)
        {
            std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components).Add(id, component);
        }

        template<typename TComponent>
        void AddComponent(ID id, TComponent&& component) requires(Nano::Types::TupleContains<TComponent, TypesTuple>&& std::is_move_constructible_v<TComponent>)
        {
            std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components).Add(id, std::move(component));
        }

        template<typename TComponent, typename ...TArgs>
        void AddComponent(ID id, TArgs&& ...args) requires(Nano::Types::TupleContains<TComponent, TypesTuple>&& std::is_constructible_v<TComponent, TArgs...>)
        {
            std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components).Add(id, std::forward<TArgs>(args)...);
        }

        template<typename TComponent>
        void RemoveComponent(ID id) requires(Nano::Types::TupleContains<TComponent, TypesTuple>)
        {
            std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components).Remove(id);
        }

        // Getters
        template<typename TComponent>
        [[nodiscard]] bool HasComponent(ID id) const requires(Nano::Types::TupleContains<TComponent, TypesTuple>)
        {
            return std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components).Has(id);
        }

        template<typename TComponent>
        [[nodiscard]] TComponent& GetComponent(ID id) requires(Nano::Types::TupleContains<TComponent, TypesTuple>)
        {
            return std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components).Get(id);
        }

        template<typename TComponent>
        [[nodiscard]] const TComponent& GetComponent(ID id) const requires(Nano::Types::TupleContains<TComponent, TypesTuple>)
        {
            return std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components).Get(id);
        }

        // Internal
        template<typename TComponent>
        inline Nano::Memory::SparseSet<TComponent, ID>& GetSparseSet() requires(Nano::Types::TupleContains<TComponent, TypesTuple>) { return std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components); }
        template<typename TComponent>
        inline const Nano::Memory::SparseSet<TComponent, ID>& GetSparseSet() const requires(Nano::Types::TupleContains<TComponent, TypesTuple>) { return std::get<Nano::Memory::SparseSet<TComponent, ID>>(m_Components); }

    private:
        StorageTuple m_Components;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // ComponentView
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename ID, typename ...Components>
    class ComponentView final : public Nano::Traits::NoCopy
    {
    public:
        using SparseSetsTuple = std::tuple<std::add_lvalue_reference_t<Nano::Memory::SparseSet<Components, ID>>...>;
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        // TypeIterator
        ////////////////////////////////////////////////////////////////////////////////////
        struct TypeIterator
        {
        public:
            // Constructor & Destructor
            inline TypeIterator(const ID* beginID, const ID* endID, const SparseSetsTuple& sets)
                : m_Current(beginID), m_End(endID), m_Sets(sets)
            {
                Satisfy();
            }
            ~TypeIterator() = default;

            // Methods
            void Satisfy()
            {
                if constexpr (sizeof...(Components) == 1)
                    return;
                else
                {
                    while (m_Current != m_End) 
                    {
                        ID id = *m_Current;

                        if ((std::get<Nano::Memory::SparseSet<Components, ID>&>(m_Sets).Has(id) && ...))
                            break;

                        ++m_Current;
                    }
                }
            }

            // Operators
            inline bool operator == (const TypeIterator& other) const { return m_Current == other.m_Current; }
            inline bool operator != (const TypeIterator& other) const { return !(*this == other); }
            
            auto operator * () const 
            { 
                ID id = *m_Current;
                return std::tuple<ID, std::add_lvalue_reference_t<Components>...>{ id, std::get<Nano::Memory::SparseSet<Components, ID>&>(m_Sets).Get(id)... };
            }
            TypeIterator& operator ++ ()
            {
                ++m_Current;
                Satisfy();
                return *this;
            }

        private:
            const ID* m_Current;
            const ID* m_End;
            const SparseSetsTuple& m_Sets;
        };

        ////////////////////////////////////////////////////////////////////////////////////
        // IndexIterator
        ////////////////////////////////////////////////////////////////////////////////////
        struct IndexIterator
        {
        public:
            using PtrTuple = std::tuple<std::add_pointer_t<Nano::Memory::SparseSet<Components, ID>>...>;
        public:
            // Constructor & Destructor
            IndexIterator(const ID* beginID, const ID* endID, const SparseSetsTuple& sets)
                : m_Current(beginID), m_End(endID)
            {
                m_Ptrs = std::apply([](auto&... ss) { return PtrTuple{ &ss... }; }, sets);
                Satisfy();
            }
            ~IndexIterator() = default;

            // Methods
            void Satisfy()
            {
                if constexpr (sizeof...(Components) == 1)
                    return;
                else
                {
                    while (m_Current != m_End)
                    {
                        ID id = *m_Current;
                        if (SatisfyImpl(id, std::index_sequence_for<Components...>{}))
                            break;
                        ++m_Current;
                    }
                }
            }

            // Operators
            IndexIterator& operator ++ ()
            {
                ++m_Current;
                Satisfy();
                return *this;
            }

            inline auto operator * () const { return DerefImpl(std::index_sequence_for<Components...>{}); }
            inline bool operator == (const IndexIterator& other) const { return m_Current == other.m_Current; }
            inline bool operator != (const IndexIterator& other) const { return !(*this == other); }

        private:
            // Private methods
            template<size_t... Is>
            [[nodiscard]] bool SatisfyImpl(ID id, std::index_sequence<Is...>) const
            {
                return (std::get<Is>(m_Ptrs)->Has(id) && ...);
            }

            template<size_t... Is>
            [[nodiscard]] auto DerefImpl(std::index_sequence<Is...>) const
            {
                ID id = *m_Current;
                return std::tuple<ID, Components&...>{ id, std::get<Is>(m_Ptrs)->Get(id)... };
            }

        private:
            const ID* m_Current;
            const ID* m_End;
            PtrTuple m_Ptrs; 
        };

        // Iterator
        using Iterator = IndexIterator;

    public:
        // Constructor & Destructor
        inline ComponentView(std::span<const ID> id, SparseSetsTuple&& sets)
            : m_IDs(id), m_Sets(std::move(sets)) {}
        ~ComponentView() = default;

        // Methods
        template<typename TFunc>
        void ForEach(TFunc&& func)
        {
            if constexpr (std::invocable<TFunc, const ID&, std::add_lvalue_reference_t<Components>...> || 
                std::invocable<TFunc, const ID&, std::add_lvalue_reference_t<const Components>...> ||
                std::invocable<TFunc, ID&, std::add_lvalue_reference_t<Components>...> ||
                std::invocable<TFunc, ID&, std::add_lvalue_reference_t<const Components>...>)
            {
                for (Iterator it = begin(); it != end(); ++it)
                {
                    std::apply(func, *it);
                }
            }
            else if constexpr (std::invocable<TFunc, std::add_lvalue_reference_t<Components>...> || 
                std::invocable<TFunc, std::add_lvalue_reference_t<const Components>...>)
            {
                for (Iterator it = begin(); it != end(); ++it)
                {
                    std::apply(func, ::Nano::Types::TupleRemoveTypes<ID>(*it));
                }
            }
            else
            {
                static_assert(false, "Function doesn't match allowed structures.");
            }
        }

        // Iterators
        inline Iterator begin() const { return Iterator(m_IDs.data(), m_IDs.data() + m_IDs.size(), m_Sets); }
        inline Iterator end() const { return Iterator(m_IDs.data() + m_IDs.size(), m_IDs.data() + m_IDs.size(), m_Sets); }

    private:
        std::span<const ID> m_IDs;
        SparseSetsTuple m_Sets;
    };

}

namespace Nano::ECS
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Registry
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename ID, typename ...Components> requires(std::is_integral_v<ID>)
    class Registry // Note: We have to predefine the components to avoid the V-table
    {
    public:
        using TypesTuple = std::tuple<Components...>;
    public:
        // Constructor & Destructor
        Registry() = default;
        ~Registry() = default;

        // Methods
        template<typename TComponent>
        void AddComponent(ID id, const TComponent& component) requires(Nano::Types::TupleContains<TComponent, TypesTuple> && std::is_copy_constructible_v<TComponent>) { m_Storage.template AddComponent<TComponent>(id, component); }
        template<typename TComponent>
        void AddComponent(ID id, TComponent&& component) requires(Nano::Types::TupleContains<TComponent, TypesTuple> && std::is_move_constructible_v<TComponent>) { m_Storage.template AddComponent<TComponent>(id, std::move(component)); }
        template<typename TComponent, typename ...TArgs>
        void AddComponent(ID id, TArgs&& ...args) requires(Nano::Types::TupleContains<TComponent, TypesTuple> && std::is_constructible_v<TComponent, TArgs...>) { m_Storage.template AddComponent<TComponent>(id, std::forward<TArgs>(args)...); }

        template<typename TComponent>
        void RemoveComponent(ID id) requires(Nano::Types::TupleContains<TComponent, TypesTuple>) { m_Storage.template RemoveComponent<TComponent>(id); }

        // Getters
        template<typename TComponent>
        [[nodiscard]] bool HasComponent(ID id) const requires(Nano::Types::TupleContains<TComponent, TypesTuple>) { return m_Storage.template HasComponent<TComponent>(id); }

        template<typename TComponent>
        [[nodiscard]] TComponent& GetComponent(ID id) requires(Nano::Types::TupleContains<TComponent, TypesTuple>) { return m_Storage.template GetComponent<TComponent>(id); }
        template<typename TComponent>
        [[nodiscard]] const TComponent& GetComponent(ID id) const requires(Nano::Types::TupleContains<TComponent, TypesTuple>) { return m_Storage.template GetComponent<TComponent>(id); }

        // Views
        template<typename TComponent>
        [[nodiscard]] auto View() requires(Nano::Types::TupleContains<TComponent, TypesTuple>)
        {
            /*
            auto& set = m_Storage.template GetSparseSet<TComponent>();
            NANO_ASSERT((set.GetIDs().size() == set.GetValues().size()), "Amount of entities doesn't equal amount of components.");
            return std::ranges::views::zip(set.GetIDs(), set.GetValues());
            */
            
            std::span<const ID> ids = m_Storage.template GetSparseSet<TComponent>().GetIDs();
            auto sets = std::forward_as_tuple(m_Storage.template GetSparseSet<TComponent>());
            return Internal::ECS::ComponentView<ID, TComponent>{ ids, std::move(sets) };
        }

        template<typename ...TComponents>
        [[nodiscard]] auto View() requires((sizeof...(TComponents) > 1) && (Nano::Types::TupleContains<TComponents, TypesTuple> && ...))
        {
            size_t smallestSize = std::numeric_limits<size_t>::max();
            std::variant<std::add_pointer_t<Memory::SparseSet<TComponents, ID>>...> smallest;

            Nano::Types::ForEachTypeInTuple<std::tuple<TComponents...>>([&]<typename TComponent>()
            {
                auto& set = m_Storage.template GetSparseSet<TComponent>();
                if (set.Size() < smallestSize)
                {
                    smallestSize = set.Size();
                    smallest = &set;
                }
            });

            std::span<const ID> ids = std::visit([](auto&& obj) -> std::span<const ID> { return obj->GetIDs(); }, smallest);
            auto sets = std::forward_as_tuple(m_Storage.template GetSparseSet<TComponents>()...);
            return Internal::ECS::ComponentView<ID, TComponents...>{ ids, std::move(sets) };
        }

        // Operators

        // Iterators

    private:
        Internal::ECS::Storage<ID, Components...> m_Storage;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Specialization for Components Tuple
    ////////////////////////////////////////////////////////////////////////////////////
    template<typename ID, typename... Components>
    class Registry<ID, std::tuple<Components...>> final : public Registry<ID, Components...>
    {
    public:
        using Registry<ID, Components...>::Registry;
    };

}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Tests HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Internal
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Test
    ////////////////////////////////////////////////////////////////////////////////////
    struct Test final
    {
    public:
        using Function = void (*)();
    public:
        std::string_view Name = {};
        Function TestFunction = nullptr;

    public:
        constexpr Test() = default;
        inline constexpr Test(std::string_view name, Function function)
            : Name(name), TestFunction(function) {}
        constexpr ~Test() = default;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // ExpressionDetails
    ////////////////////////////////////////////////////////////////////////////////////
    struct ExpressionDetails final
    {
    public:
        const char* File = nullptr;
        int Line = 0;

    public:
        constexpr ExpressionDetails() = default;
        inline constexpr ExpressionDetails(const char* file, int line)
            : File(file), Line(line) {}
        constexpr ~ExpressionDetails() = default;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // TestRegistry
    ////////////////////////////////////////////////////////////////////////////////////
    class TestRegistry final : public Nano::Traits::NoCopy, public Nano::Traits::NoMove
    {
    public:
        // Constructor & Destructor
        TestRegistry() = default;
        ~TestRegistry() = default;

        // Methods
        void Run();
        
        inline void AddTest(std::string_view name, Test::Function function) { m_Tests.emplace_back(name, function); }

        // Internal methods
        [[nodiscard]] bool Bool(const char* file, int line, const char* expressionStr, bool expression) noexcept(true); // Has to be true

        template<typename T, typename T2>
        [[nodiscard]] bool Equals(const char* file, int line, const char* leftStr, const char* rightStr, bool expression, T left, T2 right) noexcept(true) // Has to be true
        {
            if (!expression)
            {
                m_ExpressionDetails = ExpressionDetails(file, line);

                if constexpr (std::formattable<T, char> && std::formattable<T2, char>)
                {
                    m_ExpressionFailString = Nano::Text::Format("{0}({1}, {2}); Left = {3}, Right = {4}", "NANO_TEST_EQUALS", leftStr, rightStr, left, right);
                }
                else
                {
                    m_ExpressionFailString = Nano::Text::Format("{0}({1}, {2});", "NANO_TEST_EQUALS", leftStr, rightStr);
                }
            }

            return expression;
        }

        // Static methods
        static TestRegistry& Get();

    private:
        Test* m_ActiveTest = nullptr;

        ExpressionDetails m_ExpressionDetails = {};
        std::string m_ExpressionFailString = {};

        size_t m_CurrentTest = 1;
        std::vector<Test> m_Tests = { };
    };

}

// Test case
#define NANO_TEST_CASE_HELPER2(identifier, name)                                                                                \
    void _TestFn##identifier();                                                                                                 \
    inline void _TestAdd##identifier() { ::Nano::Internal::TestRegistry::Get().AddTest(name, &_TestFn##identifier); }           \
    NANO_RUN_FUNCTION(_TestAdd##identifier)                                                                                     \
    void _TestFn##identifier()

#define NANO_TEST_CASE_HELPER(identifier, name)                      NANO_TEST_CASE_HELPER2(identifier, name)
#define NANO_TEST_CASE(name)                                         NANO_TEST_CASE_HELPER(__COUNTER__, name)

// Test bool/require
#define NANO_TEST_BOOL_HELPER2(identifier, expr, line, file)                                                                    \
    bool _BoolResult##identifier =    ::Nano::Internal::TestRegistry::Get().Bool(file, line, #expr, expr);                      \
    if (!_BoolResult##identifier)     return                                                                                  

#define NANO_TEST_BOOL_HELPER(identifier, expr, line, file)          NANO_TEST_BOOL_HELPER2(identifier, expr, line, file)
#define NANO_TEST_BOOL(expr)                                         NANO_TEST_BOOL_HELPER(__COUNTER__, expr, __LINE__, __FILE__)
#define NANO_TEST_REQUIRE(expr)                                      NANO_TEST_BOOL(expr)
#define NANO_TEST_EXPECT(expr)                                       NANO_TEST_BOOL(expr)

// Test equals
#define NANO_TEST_EQUALS_HELPER2(identifier, one, two, line, file)                                                              \
    auto _EqualsLeft##identifier =      one;                                                                                    \
    auto _EqualsRight##identifier =     two;                                                                                    \
    bool _EqualsResult##identifier =    ::Nano::Internal::TestRegistry::Get().Equals(file, line, #one, #two, (_EqualsLeft##identifier == _EqualsRight##identifier), _EqualsLeft##identifier, _EqualsRight##identifier);  \
    if (!_EqualsResult##identifier)     return

#define NANO_TEST_EQUALS_HELPER(identifier, one, two, line, file)    NANO_TEST_EQUALS_HELPER2(identifier, one, two, line, file)
#define NANO_TEST_EQUALS(left, right)                                NANO_TEST_EQUALS_HELPER(__COUNTER__, left, right, __LINE__, __FILE__)

// Test main
#define NANO_TEST_MAIN()                                        \
    int main(int, char**)                                       \
    {                                                           \
        ::Nano::Internal::TestRegistry::Get().Run();            \
        return 0;                                               \
    }

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Tests CPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
#if defined(NANO_IMPL_TESTS)
namespace Nano::Internal
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void TestRegistry::Run()
    {
        // Strings
        constexpr std::string_view start = "[TESTS] - Results .............. ";
        constexpr std::string_view part1 = "result: ... ";
        constexpr std::string_view end = "message:";

        Nano::Log::PrintLn("{0}{1}{2}", start, part1, end);

        for (auto& test : m_Tests)
        {
            m_ActiveTest = &test;
            m_ExpressionFailString.clear();
            
            test.TestFunction();

            // Output
            std::string fileStr = {};

            if (!m_ExpressionFailString.empty())
            {
                std::filesystem::path file = std::filesystem::path(m_ExpressionDetails.File).filename();
                std::filesystem::path filename = file.filename();
                fileStr = filename.string();
            }
            
            std::string first = Nano::Text::Format("    ({0}) - ", test.Name);
            NANO_ASSERT((first.size() <= start.size()), "Name of Test is too long");
            std::string second = std::string(start.size() - first.size(), ' ') + (!m_ExpressionFailString.empty() ? 
                Nano::Text::Format("{0}failed{1}", Nano::Log::Colour::BrightRedFG, Nano::Log::Colour::Reset) : 
                Nano::Text::Format("{0}passed{1}", Nano::Log::Colour::BrightGreenFG, Nano::Log::Colour::Reset)
            );
            
            size_t sizeBegin = start.size() + part1.size() - first.size();
            size_t size = sizeBegin - (second.size() - (!m_ExpressionFailString.empty() ?
                Nano::Log::Colour::BrightRedFG.size() :
                Nano::Log::Colour::BrightGreenFG.size()
            ) - Nano::Log::Colour::Reset.size());
            std::string third = std::string(size, ' ') + (!m_ExpressionFailString.empty() ? 
                Nano::Text::Format("{0} in {1}:{2}", m_ExpressionFailString, fileStr, m_ExpressionDetails.Line) :
                ""
            );

            Nano::Log::PrintLn("{0}{1}{2}", first, second, third);
            m_CurrentTest++;
        }

        Nano::Log::PrintLn("");
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Internal methods
    ////////////////////////////////////////////////////////////////////////////////////
    bool TestRegistry::Bool(const char* file, int line, const char* expressionStr, bool expression) noexcept(true)
    {
        if (!expression)
        {
            m_ExpressionDetails = ExpressionDetails(file, line);
            m_ExpressionFailString = Nano::Text::Format("{0}({1});", "NANO_TEST_BOOL", expressionStr);
        }

        return expression;
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Static methods
    ////////////////////////////////////////////////////////////////////////////////////
    TestRegistry& TestRegistry::Get()
    {
        static TestRegistry s_Tests;
        return s_Tests;
    }

}
#endif

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Benchmark HPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
namespace Nano::Internal
{

    ////////////////////////////////////////////////////////////////////////////////////
    // BenchmarkResults
    ////////////////////////////////////////////////////////////////////////////////////
    struct BenchmarkResults final
    {
    public:
        std::string_view Name = {};

        // Times are in nanoseconds
        uint64_t Mean = 0;
        uint64_t Median = 0;
        uint64_t Iterations = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // Benchmark
    ////////////////////////////////////////////////////////////////////////////////////
    struct Benchmark final
    {
    public:
        using Function = void (*)();
    public:
        std::string_view Name = {};
        Function BenchmarkFunction = nullptr;

    public:
        constexpr Benchmark() = default;
        inline constexpr Benchmark(std::string_view name, Function function)
            : Name(name), BenchmarkFunction(function) {}
        constexpr ~Benchmark() = default;
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // BenchmarkPart
    ////////////////////////////////////////////////////////////////////////////////////
    struct BenchmarkPart final
    {
    public:
        enum class Type : uint8_t { None = 0, Timeout, Iterations };
        using Function = std::function<void()>;
    public:
        Type PartType = Type::None;
        std::string_view Name = {};
        Function PartFunction = nullptr;

        uint64_t Count = 0;

    public:
        // Constructors & Destructor
        BenchmarkPart() = default;
        inline BenchmarkPart(Type type, std::string_view name, uint64_t count)
            : PartType(type), Name(name), Count(count) {}
        ~BenchmarkPart();

        // Operators
        inline constexpr operator bool() const { return true; }
        inline BenchmarkPart& operator = (Function function)
        {
            PartFunction = function;
            return *this;
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////
    // BenchmarkRegistry
    ////////////////////////////////////////////////////////////////////////////////////
    class BenchmarkRegistry final
    {
    public:
        // Constructor & Destructor
        BenchmarkRegistry() = default;
        ~BenchmarkRegistry() = default;

        // Methods
        void Run();

        inline void AddBenchmark(std::string_view name, Benchmark::Function function) { m_Benchmarks.emplace_back(name, function); }
        inline void AddResult(std::string_view name, uint64_t mean, uint64_t median, uint64_t iterations) { m_Results[m_ActiveBenchmark].emplace_back(name, mean, median, iterations); }

        // Static methods
        static BenchmarkRegistry& Get();

    private:
        size_t m_ActiveBenchmark = 0;

        std::vector<Benchmark> m_Benchmarks = { };
        std::vector<std::vector<BenchmarkResults>> m_Results = { };
    };

}

// Bench init
#define NANO_BENCHMARK_INIT_HELPER2(identifier, name)                                                                               \
    void _BenchFn##identifier();                                                                                                    \
    inline void _BenchAdd##identifier() { ::Nano::Internal::BenchmarkRegistry::Get().AddBenchmark(name, &_BenchFn##identifier); }   \
    NANO_RUN_FUNCTION(_BenchAdd##identifier)                                                                                        \
    void _BenchFn##identifier()

#define NANO_BENCHMARK_INIT_HELPER(identifier, name)        NANO_BENCHMARK_INIT_HELPER2(identifier, name)
#define NANO_BENCHMARK_INIT(name)                           NANO_BENCHMARK_INIT_HELPER(__COUNTER__, name)

// Bench iterations
#define NANO_BENCHMARK_ITERATIONS_HELPER2(identifier, name, iterations)                                                             \
    if (::Nano::Internal::BenchmarkPart part{::Nano::Internal::BenchmarkPart::Type::Iterations, name, iterations})                  \
        part = [&]() 

#define NANO_BENCHMARK_ITERATIONS_HELPER(identifier, name, iterations)   NANO_BENCHMARK_ITERATIONS_HELPER2(identifier, name, iterations)
#define NANO_BENCHMARK_ITERATIONS(name, iterations)                      NANO_BENCHMARK_ITERATIONS_HELPER(__COUNTER__, name, iterations)

// Bench timeout
#define NANO_BENCHMARK_TIMEOUT_HELPER2(identifier, name, milliseconds)                                                              \
    if (::Nano::Internal::BenchmarkPart part{::Nano::Internal::BenchmarkPart::Type::Timeout, name, milliseconds})                   \
        part = [&]() 

#define NANO_BENCHMARK_TIMEOUT_HELPER(identifier, name, milliseconds)    NANO_BENCHMARK_TIMEOUT_HELPER2(identifier, name, milliseconds)
#define NANO_BENCHMARK_TIMEOUT(name, milliseconds)                       NANO_BENCHMARK_TIMEOUT_HELPER(__COUNTER__, name, milliseconds)

#define NANO_BENCHMARK_AUTO(name)                                        NANO_BENCHMARK_TIMEOUT(name, 2000)

// Bench main
#define NANO_BENCHMARK_MAIN()                                   \
    int main(int, char**)                                       \
    {                                                           \
        ::Nano::Internal::BenchmarkRegistry::Get().Run();       \
        return 0;                                               \
    }

#define NANO_TEST_BENCHMARK_MAIN()                              \
    int main(int, char**)                                       \
    {                                                           \
        ::Nano::Internal::TestRegistry::Get().Run();            \
        ::Nano::Internal::BenchmarkRegistry::Get().Run();       \
        return 0;                                               \
    }

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// --- Benchmark CPP ---
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
#if defined(NANO_IMPL_BENCHMARKS)
namespace Nano::Internal
{

    ////////////////////////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////////////////////////
    BenchmarkPart::~BenchmarkPart()
    {
        std::vector<uint64_t> times;
        
        // Timing
        switch (PartType)
        {
        case Type::Timeout:
        {
            auto start = Nano::Time::HighResTime<Nano::Time::Period::Nanoseconds>();
            auto timeoutStart = Nano::Time::HighResTime<Nano::Time::Period::Milliseconds>();
            while (Nano::Time::HighResTime<Nano::Time::Period::Milliseconds>() - timeoutStart < Count)
            {
                start = Nano::Time::HighResTime<Nano::Time::Period::Nanoseconds>();

                PartFunction();

                times.push_back(Nano::Time::HighResTime<Nano::Time::Period::Nanoseconds>() - start);
            }
            break;
        }
        case Type::Iterations:
        {
            times.resize(Count);

            auto start = Nano::Time::HighResTime<Nano::Time::Period::Nanoseconds>();
            for (size_t i = 0; i < Count; i++)
            {
                start = Nano::Time::HighResTime<Nano::Time::Period::Nanoseconds>();

                PartFunction();

                times[i] = Nano::Time::HighResTime<Nano::Time::Period::Nanoseconds>() - start;
            }
            break;
        }

        default:
            NANO_ASSERT(false, "Invalid BenchmarkPart::Type.");
            break;
        }

        // Sorting
        {
            std::ranges::sort(times); // Sorts in ascending order
        }
        // Output
        {
            uint64_t median = times[static_cast<size_t>(std::floorf(static_cast<float>(times.size()) / 2.0f))];
            uint64_t mean = std::accumulate(times.begin(), times.end(), 0ull) / times.size();
            uint64_t iterations = times.size();

            BenchmarkRegistry::Get().AddResult(Name, mean, median, iterations);
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////////
    // Methods
    ////////////////////////////////////////////////////////////////////////////////////
    void BenchmarkRegistry::Run()
    {
        // Strings
        constexpr std::string_view start = "[BENCHMARK] - Results .......... ";
        constexpr std::string_view part1 = "mean: ..... ";
        constexpr std::string_view part2 = "median: ... ";
        constexpr std::string_view end = "iterations:";

        Nano::Log::PrintLn("{0}{1}{2}{3}", start, part1, part2, end);

        m_Results.resize(m_Benchmarks.size());
        for (; m_ActiveBenchmark < m_Benchmarks.size(); m_ActiveBenchmark++)
        {
            m_Benchmarks[m_ActiveBenchmark].BenchmarkFunction();

            for (const auto& result : m_Results[m_ActiveBenchmark])
            {
                // Time to string
                auto toStr = [](uint64_t time) -> std::string
                {
                    constexpr std::string_view floatPointFormat = "00.00";

                    uint8_t numCount = Nano::Maths::CountNumbers(time);
                    std::string numStr = Nano::Maths::ToString(time);

                    if (numCount > 9)
                    {
                        numStr.insert(static_cast<size_t>(numCount) - 9, ".");
                        numStr = numStr.substr(0, floatPointFormat.size()) + "s";
                    }
                    else if (numCount > 6)
                    {
                        numStr.insert(static_cast<size_t>(numCount) - 6, ".");
                        numStr = numStr.substr(0, floatPointFormat.size()) + "ms";
                    }
                    else if (numCount > 3)
                    {
                        numStr.insert(static_cast<size_t>(numCount) - 3, ".");
                        numStr = numStr.substr(0, floatPointFormat.size()) + "us"; // µs
                    }
                    else
                    {
                        numStr = numStr + "ns";
                    }

                    return numStr;
                };

                // Add ' between every 3 numbers
                auto addDot = [](std::string iterations) -> std::string
                {
                    std::string newTime;
                    newTime.reserve(iterations.size() + (iterations.size() / 3));

                    int count = 0;
                    for (auto it = iterations.rbegin(); it != iterations.rend(); it++) 
                    {
                        newTime.push_back(*it);
                        if (++count % 3 == 0 && std::next(it) != iterations.rend())
                            newTime.push_back('\'');
                    }

                    std::reverse(newTime.begin(), newTime.end());
                    return newTime;
                };

                // Values
                std::string meanStr = toStr(result.Mean);
                std::string medianStr = toStr(result.Median);
                std::string iterationsStr = addDot(Nano::Maths::ToString(result.Iterations));

                // Output
                std::string first = Nano::Text::Format("    ({0}:{1}) - ", m_Benchmarks[m_ActiveBenchmark].Name, result.Name);
                NANO_ASSERT((first.size() <= start.size()), "Name of Benchmark is too long");
                std::string second = std::string(start.size() - first.size(), ' ') + meanStr;
                std::string third = std::string(start.size() + part1.size() - first.size() - second.size(), ' ') + medianStr;
                std::string fourth = std::string(start.size() + part1.size() + part2.size() - first.size() - second.size() - third.size(), ' ') + iterationsStr;

                Nano::Log::PrintLn("{0}{1}{2}{3}", first, second, third, fourth);
            }

            Nano::Log::PrintLn("");
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Static methods
    ////////////////////////////////////////////////////////////////////////////////////
    BenchmarkRegistry& BenchmarkRegistry::Get()
    {
        static BenchmarkRegistry s_Benchmarks;
        return s_Benchmarks;
    }

}
#endif