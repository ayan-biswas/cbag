#include <string>
#include <utility>

#include <catch2/catch.hpp>

#include <cbag/spirit/ast.h>
#include <cbag/util/name_convert.h>

SCENARIO("valid names", "[name_parse]") {
    std::pair<std::string, std::string> data =
        GENERATE(values<std::pair<std::string, std::string>>({
            {"foo", ""},
            {"bar0<3>", ""},
            {"bar_foo<0>", ""},
            {"bar_<3:5>", ""},
            {"ACG<5:1>", ""},
            {"ABC_a12<3:5:1>", "ABC_a12<3:5>"},
            {"bar<6:1:1>", "bar<6:1>"},
            {"<*1>baz", "baz"},
            {"<*1>(baz)", "baz"},
            {"<*3>baz", ""},
            {"<*6>baz<1>", ""},
            {"<*5>baz<2:6>", ""},
            {"<*5>(baz<2:6>)", "<*5>baz<2:6>"},
            {"<*4>baz<7:1>", ""},
            {"<*3>baz<2:6:1>", "<*3>baz<2:6>"},
            {"<*2>baz<7:1:2>", ""},
            {"foo,bar", ""},
            {"foo,bar<3>", ""},
            {"baz<3:1>,bar<1:5:1>", "baz<3:1>,bar<1:5>"},
            {"<*1>bar<3>,<*2>foo", "bar<3>,<*2>foo"},
            {"<*1>bar<3>,<*2>(foo)", "bar<3>,<*2>foo"},
            {"<*1>bar<3:10:2>,<*2>foo<1:3>", "bar<3:10:2>,<*2>foo<1:3>"},
            {"a,b,c", ""},
            {"<*2>(b,c)", ""},
            {"a,<*2>(b,c)", ""},
            {"<*2>(<*4>foo<3:5>,a)", ""},
            {"<*2>(<*4>foo<3:5>,a),<*2>(b,c)", ""},
        }));

    auto ns_info = cbag::spirit::get_ns_info(cbag::spirit::namespace_type::CDBA);

    THEN("parsing works") {
        cbag::spirit::ast::name name_obj;
        try {
            name_obj = cbag::util::parse_cdba_name(data.first);
        } catch (std::invalid_argument &ex) {
            FAIL("failed to parse " << data.first << ", error: " << std::string(ex.what()));
        }
        THEN("name converts back to same string") {
            std::string expected = (data.second == "") ? data.first : data.second;
            REQUIRE(name_obj.to_string(ns_info) == expected);
        }
    }
}

SCENARIO("invalid names", "[name_parse]") {
    std::string data = GENERATE(values<std::string>({
        "bar<1:2:0>",
        "<*0>foo_hi",
        "0baz",
        "haha,",
        "",
        "test<-1>",
        "tar<2:-3>",
        "taz<1:5:-1>",
        "<*3foo",
        "<*2>foo)",
        "<*2>(foo",
        "<*2>()",
    }));

    THEN("parsing fails") {
        CAPTURE(data);
        CHECK_THROWS_AS(cbag::util::parse_cdba_name(data), std::invalid_argument);
    }
}

SCENARIO("iterator_test", "[name_class]") {
    std::pair<std::string, std::vector<std::string>> data =
        GENERATE(values<std::pair<std::string, std::vector<std::string>>>({
            {"foo", {"foo"}},
            {"a,b", {"a", "b"}},
            {"foo<2>", {"foo<2>"}},
            {"foo<1:3>", {"foo<1>", "foo<2>", "foo<3>"}},
            {"foo<5:1:2>", {"foo<5>", "foo<3>", "foo<1>"}},
            {"foo<1:2>,a", {"foo<1>", "foo<2>", "a"}},
            {"<*3>a", {"a", "a", "a"}},
            {"<*2>a<2:1>", {"a<2>", "a<1>", "a<2>", "a<1>"}},
            {"<*2>(a,b)", {"a", "b", "a", "b"}},
            {"<*2>(a,b<1:0>)", {"a", "b<1>", "b<0>", "a", "b<1>", "b<0>"}},
            {"b,<*3>a", {"b", "a", "a", "a"}},
        }));

    auto ns_info = cbag::spirit::get_ns_info(cbag::spirit::namespace_type::CDBA);

    auto &test_name = data.first;
    auto &bit_list = data.second;

    THEN("iterator works") {
        cbag::spirit::ast::name name_obj;
        try {
            name_obj = cbag::util::parse_cdba_name(test_name);
        } catch (std::invalid_argument &ex) {
            FAIL("failed to parse " << test_name << ", error: " << std::string(ex.what()));
        }

        auto iter = name_obj.begin(&ns_info);
        auto stop = name_obj.end(&ns_info);
        std::size_t n = bit_list.size();
        for (std::size_t idx = 0; idx < n; ++idx, ++iter) {
            if (iter == stop) {
                CAPTURE(data);
                FAIL("Iterator has " << idx << " items, but expected " << n);
            }
            std::string &expect = bit_list[idx];
            REQUIRE(*iter == expect);
        }
        if (iter != stop) {
            CAPTURE(test_name);
            CAPTURE(bit_list);
            FAIL("Iterator has more than " << n << " items");
        }
    }
}
