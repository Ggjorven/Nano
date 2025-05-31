#define NANO_IMPLEMENTATION
#define NANO_EXPERIMENTAL
#include "Nano.hpp"

////////////////////////////////////////////////////////////////////////////////////
// Pre-defines
////////////////////////////////////////////////////////////////////////////////////
// Type
struct S;
class C;

// Enum
enum class E { A = 0, B, C };
enum class E2 { N0 = 0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14, N15, N16, N17, N18, N19, N20, N21, N22, N23, N24, N25, N26, N27, N28, N29, N30, N31, N32, N33, N34, N35, N36, N37, N38, N39, N40, N41, N42, N43, N44, N45, N46, N47, N48, N49, N50, N51, N52, N53, N54, N55, N56, N57, N58, N59, N60, N61, N62, N63, N64, N65, N66, N67, N68, N69, N70, N71, N72, N73, N74, N75, N76, N77, N78, N79, N80, N81, N82, N83, N84, N85, N86, N87, N88, N89, N90, N91, N92, N93, N94, N95, N96, N97, N98, N99, N100, N101, N102, N103, N104, N105, N106, N107, N108, N109, N110, N111, N112, N113, N114, N115, N116, N117, N118, N119, N120, N121, N122, N123, N124, N125, N126, N127, N128, N129, N130, N131, N132, N133, N134, N135, N136, N137, N138, N139, N140, N141, N142, N143, N144, N145, N146, N147, N148, N149, N150, N151, N152, N153, N154, N155, N156, N157, N158, N159, N160, N161, N162, N163, N164, N165, N166, N167, N168, N169, N170, N171, N172, N173, N174, N175, N176, N177, N178, N179, N180, N181, N182, N183, N184, N185, N186, N187, N188, N189, N190, N191, N192, N193, N194, N195, N196, N197, N198, N199, N200, N201, N202, N203, N204, N205, N206, N207, N208, N209, N210, N211, N212, N213, N214, N215, N216, N217, N218, N219, N220, N221, N222, N223, N224, N225, N226, N227, N228, N229, N230, N231, N232, N233, N234, N235, N236, N237, N238, N239, N240, N241, N242, N243, N244, N245, N246, N247, N248, N249, N250, N251, N252, N253, N254, N255 };

// Deferred
struct Deferred
{
public:
    int X;
public:
    Deferred(int x)
        : X(x) {}
    ~Deferred() = default;
};

// Test
struct Test
{
public:
    int A()
    {
        return 69;
    }
};

// NonTrivial
struct NonTrivial
{
public:
    NonTrivial(int val)
        : Value(val) {
    }
    ~NonTrivial()
    {
        int a = 10;
        a++;
    }

public:
    int Value;
};

////////////////////////////////////////////////////////////////////////////////////
// Tests
////////////////////////////////////////////////////////////////////////////////////
// Text
NANO_TEST_CASE("Text")
{
    NANO_TEST_EQUALS("12-23", Nano::Text::Format("{0}-{1}", 12, 23));
    NANO_TEST_EQUALS("1.0000", Nano::Text::Format("{0:.4f}", 1.0f));
    NANO_TEST_EQUALS("1.23456789", Nano::Text::Format("{0:.8f}", 1.23456789));
}

// Hash
NANO_TEST_CASE("Hashing")
{
    constexpr uint64_t hash = Nano::Hash::fnv1a("coolstring");

    NANO_TEST_EQUALS(hash, 9289698454369670365ull);
    NANO_TEST_EQUALS(hash, Nano::Hash::fnv1a("coolstring"));

    // TODO: More hashes
}

// Type
NANO_TEST_CASE("Types")
{
    struct EmbeddedType;

    constexpr std::string_view eType = Nano::Types::Name<EmbeddedType>();
    NANO_TEST_EQUALS(eType, "EmbeddedType");

    constexpr std::string_view sType = Nano::Types::Name<S>();
    NANO_TEST_EQUALS(sType, "S");

    constexpr std::string_view cType = Nano::Types::Name<C>();
    NANO_TEST_EQUALS(cType, "C");

    constexpr std::string_view intType = Nano::Types::Name<int>();
    NANO_TEST_EQUALS(intType, "int");

    constexpr std::string_view uintType = Nano::Types::Name<unsigned int>();
    NANO_TEST_EQUALS(uintType, "unsigned int");

    {
        using NumberTuple = std::tuple<int, unsigned int, float, double>;
        NANO_TEST_EQUALS(Nano::Types::TupleTypeCount<NumberTuple>, 4);
        NANO_TEST_EQUALS((Nano::Types::TupleTypeIndex<unsigned int, NumberTuple>), 1);
        NANO_TEST_BOOL((Nano::Types::TupleContains<unsigned int, NumberTuple>));
        NANO_TEST_BOOL(!(Nano::Types::TupleContains<std::string, NumberTuple>));

        size_t i = 0;
        Nano::Types::ForEachTypeInTuple<NumberTuple>([&]<typename T>() constexpr
        {
            i++;
        });
        NANO_TEST_EQUALS(i, 4);
    }
    {
        constexpr std::tuple<int, unsigned int, float, double> first = { 1, 2ul, 3.0f, 4.0 };
        constexpr auto second = Nano::Types::TupleRemoveTypes<unsigned int, float>(first);

        NANO_TEST_EQUALS(Nano::Types::TupleTypeCount<decltype(second)>, 2);
        NANO_TEST_BOOL((Nano::Types::TupleContains<int, decltype(second)>));
        NANO_TEST_BOOL((Nano::Types::TupleContains<double, decltype(second)>));
        NANO_TEST_BOOL(!(Nano::Types::TupleContains<unsigned int, decltype(second)>));
        NANO_TEST_BOOL(!(Nano::Types::TupleContains<float, decltype(second)>));
    }
}

// Functional
NANO_TEST_CASE("Functional")
{
    Test test;
    auto aFunc = Nano::Functional::Conversion::ToStatic(&Test::A);

    NANO_TEST_EQUALS(test.A(), aFunc(&test));
}

// Enum
NANO_TEST_CASE("Enum")
{
    enum class EmbeddedE { One, Two };

    constexpr std::string_view constexprEnum = Nano::Enum::Name(E::A);
    NANO_TEST_EQUALS(constexprEnum, "A");

    volatile E runtime = E::A;
    std::string_view runtimeEnum = Nano::Enum::Name(runtime);
    NANO_TEST_EQUALS(runtimeEnum, "A");

    constexpr std::string_view constexprEmbedded = Nano::Enum::Name(EmbeddedE::Two);
    NANO_TEST_EQUALS(constexprEmbedded, "Two");

    volatile EmbeddedE runtimeEmbedded = EmbeddedE::One;
    std::string_view runtimeEmbeddedStr = Nano::Enum::Name(runtimeEmbedded);
    NANO_TEST_EQUALS(runtimeEmbeddedStr, "One");

    // TODO: ...
    //std::string_view full = Nano::Enum::FullName(runtimeEmbedded);
    //Nano::Log::PrintLn("{0}", full);

    // TODO: Fusing
}

// DeferredConstruct
NANO_TEST_CASE("DeferredConstruct")
{
    Nano::Memory::DeferredConstruct<Deferred> defer;
    defer.Construct(10);

    NANO_TEST_EQUALS(defer->X, 10);
}

// FixedStack
NANO_TEST_CASE("FixedStack") // TODO: ...
{
    using str = std::string;
    NANO_TEST_EQUALS(str("TODO"), str("DONE"));
}

// Arena
NANO_TEST_CASE("Arena") // TODO: ...
{
    using str = std::string;
    NANO_TEST_EQUALS(str("TODO"), str("DONE"));
}

// ScopeExit
NANO_TEST_CASE("ScopeExit")
{
    int i = 0;

    {
        Nano::Utils::ScopeExit exit([&]() { i = 1; });
        NANO_TEST_EQUALS(i, 0);
    }

    NANO_TEST_EQUALS(i, 1);
}

// Expected
NANO_TEST_CASE("Expected") // TODO: ...
{
    using str = std::string;
    NANO_TEST_EQUALS(str("TODO"), str("DONE"));
}

// Threading
NANO_TEST_CASE("Threading")
{
    Nano::Threading::ThreadPool pool(Nano::Threading::HardwareThreads());

    std::array<std::future<size_t>, 10> threadFutures = { };
    for (size_t i = 0; i < threadFutures.size(); i++)
        threadFutures[i] = pool.Enqueue([i]() { return i; });

    for (size_t i = 0; i < threadFutures.size(); i++)
    {
        NANO_TEST_EQUALS(i, threadFutures[i].get());
    }
}

// Event
NANO_TEST_CASE("Events")
{
    using MyEvent = std::variant<int, unsigned int>;
    Nano::Events::EventDispatcher<MyEvent> dispatcher;

    dispatcher.Subscribe([](MyEvent& e)
    {
        Nano::Events::EventHandler<MyEvent> handler(e);
        handler.Handle<int>([](int& i) { NANO_TEST_EQUALS(i++, 14); });
    });
    dispatcher.Subscribe([](MyEvent& e)
    {
        Nano::Events::EventHandler<MyEvent> handler(e);
        handler.Handle<int>([](int& i) { NANO_TEST_EQUALS(i, 15); });
        handler.Handle<unsigned int>([](unsigned int& i) { NANO_TEST_EQUALS(i, 16u); });
    });

    MyEvent e(14);
    dispatcher.Dispatch(e);
    MyEvent e2(16u);
    dispatcher.Dispatch(e2);
}

// ECS
NANO_TEST_CASE("ECS")
{
    using Components = std::tuple<int, unsigned int, float, double>;
    Nano::ECS::Registry<uint64_t, Components> registry;

    {
        uint64_t id0 = 0;
        uint64_t id1 = 1;
        uint64_t id2 = 70;
        registry.AddComponent<int>(id0, 69);
        registry.AddComponent<int>(id1, 9);
        registry.AddComponent<int>(id2, 732890);
        registry.AddComponent<unsigned int>(id1, 10);
        NANO_TEST_BOOL(registry.HasComponent<int>(id0));
        NANO_TEST_BOOL(!registry.HasComponent<unsigned int>(id0));
        NANO_TEST_BOOL(registry.HasComponent<unsigned int>(id1));
        NANO_TEST_BOOL(!registry.HasComponent<float>(id0));

        auto& val = registry.GetComponent<int>(id0);
        NANO_TEST_EQUALS(val, 69);

        {
            {
                auto view = registry.View<int>();
                size_t index = 0;
                std::array<int, 3> values = { 69, 9, 732890 };
                for (auto [entity, i] : view)
                {
                    NANO_TEST_EQUALS(i, values[index++]);
                }

                index = 0;
                view.ForEach([&](const uint64_t&, int& iVal)
                {
                    NANO_TEST_EQUALS(iVal, values[index++]);
                });

                index = 0;
                view.ForEach([&](int& iVal)
                {
                    NANO_TEST_EQUALS(iVal, values[index++]);
                });
            }
            {
                auto view = registry.View<int, unsigned int>();
                size_t index = 0;
                std::array<std::pair<int, unsigned int>, 1> values = { std::make_pair<int, unsigned int>(9, 10u) };
                for (auto [entity, i, ui] : view)
                {
                    NANO_TEST_EQUALS(i, values[index].first);
                    NANO_TEST_EQUALS(ui, values[index++].second);
                }

                index = 0;
                view.ForEach([&](const uint64_t&, int& iVal, unsigned int& uiVal)
                {
                    NANO_TEST_EQUALS(iVal, values[index].first);
                    NANO_TEST_EQUALS(uiVal, values[index++].second);
                });

                index = 0;
                view.ForEach([&](int& iVal, unsigned int& uiVal)
                {
                    NANO_TEST_EQUALS(iVal, values[index].first);
                    NANO_TEST_EQUALS(uiVal, values[index++].second);
                });
            }
        }

        registry.RemoveComponent<int>(id0);
        NANO_TEST_BOOL(!registry.HasComponent<int>(id0));
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Benchmarks
////////////////////////////////////////////////////////////////////////////////////
// Enum
NANO_BENCHMARK_INIT("Enum")
{
    volatile E2 e1 = E2::N1;
    volatile E2 e128 = E2::N128;
    volatile E2 e255 = E2::N255;

    NANO_BENCHMARK_AUTO("1-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Enum::Name(e1);
    };

    NANO_BENCHMARK_AUTO("128-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Enum::Name(e128);
    };

    NANO_BENCHMARK_AUTO("255-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Enum::Name(e255);
    };

    // TODO: Fusing
}

// FixedStack
NANO_BENCHMARK_INIT("FixedStack")
{
    NANO_BENCHMARK_AUTO("Init-1KiB")
    {
        Nano::Memory::FixedStack<1024, true> fixedStack = {};
    };

    NANO_BENCHMARK_AUTO("1KiB-T")
    {
        Nano::Memory::FixedStack<1024, true> fixedStack1KiB_T = {};
        for (size_t i = 0; i < ((1024 / sizeof(NonTrivial)) - 1); i++)
        {
            (void)fixedStack1KiB_T.Allocate<NonTrivial>(69);
        }
    };

    NANO_BENCHMARK_AUTO("1KiB-UT")
    {
        Nano::Memory::FixedStack<1024, false> fixedStack1KiB_UT = {};
        for (size_t i = 0; i < ((1024 / sizeof(NonTrivial)) - 1); i++)
        {
            (void)fixedStack1KiB_UT.Allocate<int>(69);
        }
    };
}

// ArenaAllocator
NANO_BENCHMARK_INIT("Arena")
{
    NANO_BENCHMARK_AUTO("Init-1MiB")
    {
        Nano::Memory::ArenaAllocator<1 << 20, true> arenaAllocator = {};
    };

    NANO_BENCHMARK_AUTO("1MiB-T")
    {
        Nano::Memory::ArenaAllocator<1 << 20, true> arenaAllocator1MiB_T = {};
        for (size_t i = 0; i < ((1ull << 20ull) / sizeof(NonTrivial)) - 1; i++)
        {
            (void)arenaAllocator1MiB_T.Allocate<NonTrivial>(69);
        }
    };

    NANO_BENCHMARK_AUTO("1MiB-UT")
    {
        Nano::Memory::ArenaAllocator<1 << 20, false> arenaAllocator1MiB_UT = {};
        for (size_t i = 0; i < ((1ull << 20ull) / sizeof(int)) - 1; i++)
        {
            (void)arenaAllocator1MiB_UT.Allocate<int>(69);
        }
    };
}

// Random
NANO_BENCHMARK_INIT("Random")
{
    NANO_BENCHMARK_AUTO("Int-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::Random::Int();
    };

    NANO_BENCHMARK_AUTO("UInt-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::Random::UInt();
    };

    NANO_BENCHMARK_AUTO("Float-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::Random::Float();
    };

    NANO_BENCHMARK_AUTO("Double-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::Random::Double();
    };
}

// FastRandom
NANO_BENCHMARK_INIT("FastRandom")
{
    NANO_BENCHMARK_AUTO("Int-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::FastRandom::Int();
    };

    NANO_BENCHMARK_AUTO("UInt-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::FastRandom::UInt();
    };

    NANO_BENCHMARK_AUTO("Float-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::FastRandom::Float();
    };

    NANO_BENCHMARK_AUTO("Double-10'000")
    {
        for (size_t i = 0; i < 10'000; i++)
            (void)Nano::Random::FastRandom::Double();
    };
}

// ECS
NANO_BENCHMARK_INIT("ECS")
{
    NANO_BENCHMARK_AUTO("View")
    {
        // TODO: ...
    };
}


////////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////////
NANO_TEST_BENCHMARK_MAIN()