#include "engine/core/uuid.hpp"

#include "catch/catch.hpp"

TEST_CASE("UUIDs are constructed", "[uuid]") {
    using engine::core::Uuid;

    SECTION("Null UUIDs") {
        Uuid default_uuid;
        Uuid null_uuid_pair(0, 0);
        Uuid null_uuid_string("00000000-0000-0000-0000-000000000000");
        Uuid invalid_uuid_string("some invalid uuid");

        REQUIRE(default_uuid == null_uuid_pair);
        REQUIRE(default_uuid == null_uuid_string);
        REQUIRE(default_uuid == invalid_uuid_string);
    }

    SECTION("Valid UUIDs") {
        Uuid uuid_pair(0x7d4e481479364f0e, 0x932a39d831a31c2a);
        Uuid uuid_string("7d4e4814-7936-4f0e-932a-39d831a31c2a");
        REQUIRE(uuid_pair == uuid_string);

        Uuid default_uuid;
        REQUIRE(default_uuid != uuid_pair);
        REQUIRE(default_uuid != uuid_string);
    }

    SECTION("Invalid UUID strings") {
        Uuid invalid_uuid1("7d4e48147936-4f0e-932a-39d831a31c2a");
        Uuid invalid_uuid2("7d4e4814-7936-4f0e-932a39d831a31c2a");
        Uuid invalid_uuid3("7d4e481479364f0e932a39d831a31c2a");
        Uuid invalid_uuid4("7d4e4814-7936-4f0e-932a-39d831a31c2aa");
        Uuid invalid_uuid5("7W4e4814-7936-4f0e-932a-39d831a31c2a");
        Uuid invalid_uuid6("Wd4e4814-7936-4f0e-932a-39d831a31c2a");

        Uuid default_uuid;
        REQUIRE(default_uuid == invalid_uuid1);
        REQUIRE(default_uuid == invalid_uuid2);
        REQUIRE(default_uuid == invalid_uuid3);
        REQUIRE(default_uuid == invalid_uuid4);
        REQUIRE(default_uuid == invalid_uuid5);
        REQUIRE(default_uuid == invalid_uuid6);
    }
}