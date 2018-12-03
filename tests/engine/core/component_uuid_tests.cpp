#include <catch/catch.hpp>

#include <engine/core/component_uuid.hpp>

TEST_CASE("Component UUIDs are constructed", "[component_uuid]") {
    using engine::core::Component_uuid;

    SECTION("Null UUIDs") {
        Component_uuid default_uuid;
        Component_uuid null_uuid_pair(0, 0);
        Component_uuid null_uuid_string("00000000-0000-0000-0000-000000000000");
        Component_uuid invalid_uuid_string("some invalid uuid");

        REQUIRE(default_uuid == null_uuid_pair);
        REQUIRE(default_uuid == null_uuid_string);
        REQUIRE(default_uuid == invalid_uuid_string);
    }

    SECTION("Valid UUIDs") {
        Component_uuid uuid_pair(0x7d4e481479364f0e, 0x932a39d831a31c2a);
        Component_uuid uuid_string("7d4e4814-7936-4f0e-932a-39d831a31c2a");
        REQUIRE(uuid_pair == uuid_string);

        Component_uuid default_uuid;
        REQUIRE(default_uuid != uuid_pair);
        REQUIRE(default_uuid != uuid_string);
    }

    SECTION("Invalid UUID strings") {
        Component_uuid invalid_uuid1("7d4e48147936-4f0e-932a-39d831a31c2a");
        Component_uuid invalid_uuid2("7d4e4814-7936-4f0e-932a39d831a31c2a");
        Component_uuid invalid_uuid3("7d4e481479364f0e932a39d831a31c2a");
        Component_uuid invalid_uuid4("7d4e4814-7936-4f0e-932a-39d831a31c2aa");
        Component_uuid invalid_uuid5("7W4e4814-7936-4f0e-932a-39d831a31c2a");
        Component_uuid invalid_uuid6("Wd4e4814-7936-4f0e-932a-39d831a31c2a");

        Component_uuid default_uuid;
        REQUIRE(default_uuid == invalid_uuid1);
        REQUIRE(default_uuid == invalid_uuid2);
        REQUIRE(default_uuid == invalid_uuid3);
        REQUIRE(default_uuid == invalid_uuid4);
        REQUIRE(default_uuid == invalid_uuid5);
        REQUIRE(default_uuid == invalid_uuid6);
    }
}