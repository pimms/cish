#include <gtest/gtest.h>

#include "ast/StringTable.h"

#include <vector>
#include <string>
#include <set>

using namespace cish::ast;

TEST(StringTableTest, uniqueStringsAreGivenUniqueIds)
{
    const std::vector<std::string> strings = {
        "abc",
        "1234",
        "9384198",
        "hei mamma",
        "",
        "i'm on tv moms",
        "phartyphukborlz",
    };

    std::set<StringId> ids;
    StringTable table;

    for (std::string s: strings) {
        StringId id = table.insert(s);
        ASSERT_FALSE(ids.count(id));
    }
}

TEST(StringTableTest, stringsAreRetrievableById)
{
    StringTable table;

    StringId key1 = table.insert("a");
    StringId key2 = table.insert("b");

    ASSERT_EQ("a", table.resolve(key1));
    ASSERT_EQ("b", table.resolve(key2));
}

TEST(StringTableTest, stringRetrievalFailureThrows)
{
    StringTable table;
    ASSERT_ANY_THROW(table.resolve(393));
}

TEST(StringTableTest, identicalStringsAreGivenTheSameId)
{
    StringTable table;

    StringId key1 = table.insert("one");
    StringId key2 = table.insert("one");
    StringId key3 = table.insert("two");

    ASSERT_EQ("one", table.resolve(key1));
    ASSERT_EQ("one", table.resolve(key2));
    ASSERT_EQ("two", table.resolve(key3));

    ASSERT_EQ(key1, key2);
    ASSERT_NE(key1, key3);
}