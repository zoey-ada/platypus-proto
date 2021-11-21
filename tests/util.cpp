#include <catch2/catch.hpp>

#include <util.hpp>
#include <settings.hpp>

SCENARIO("Can convert JSON to proto", "[proto]") {
	GIVEN("JSON str") {
		std::string json("{'full_screen':true,'height':false}");

		WHEN("attempting to convert to proto") {
			auto s = platypus::toProto<platypus::Settings>(json);

			THEN("proto obj is correctly translated") {
				REQUIRE(s.full_screen() == true);
				REQUIRE(s.height() == false);
			}
		}
	}
}

// TODO: This is an aweful and brittle test. A better comparison method is needed
SCENARIO("Can convert proto to JSON", "[proto]") {
	GIVEN("proto obj") {
		platypus::Settings proto;
		proto.set_full_screen(false);
		proto.set_height(true);

		WHEN("attempting to convert to JSON") {
			auto s = platypus::toJson(proto);

			THEN("JSON str is correctly translated") {
				REQUIRE(s.compare("{\n \"height\": true\n}\n") == 0);
			}
		}
	}
}
