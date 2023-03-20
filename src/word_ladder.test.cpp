#include "word_ladder.h"

#include <catch2/catch.hpp>

TEST_CASE("word_ladder::read_lexicon works as expected") {
	auto lexicon_set = word_ladder::read_lexicon("./english.txt");
	REQUIRE(lexicon_set.size() == 127142);
}

TEST_CASE("NO SOLUTIONS") {
	SECTION("Same Length: bend -> able") {
		auto const lexicon = std::unordered_set<std::string>{
			"bend",
			"bent",
			"abjs",
			"bble",
			"able"
		};

		auto const ladders = word_ladder::generate("bend", "able", lexicon);

		CHECK(ladders.size() == 0);
	}

	SECTION("Diff Length: hello -> bye") {
		auto const lexicon = std::unordered_set<std::string>{
			"hello",
			"helln",
			"bend",
			"bynd",
			"bye"
		};

		auto const ladders = word_ladder::generate("hello", "bye", lexicon);

		CHECK(ladders.size() == 0);
	}
}


TEST_CASE("ONE SOLUTION") {
	auto const lexicon = std::unordered_set<std::string>{
		"at",
		"it",
		"hello",
		"sello",
		"sallo",
		"sadlo",
		"sadgo",
		"sadge"
	};

	SECTION("Length 2: at -> it") {
		const auto expected = std::vector<std::vector<std::string>>{
			{"at", "it"}
		};

		auto const ladders = word_ladder::generate("at", "it", lexicon);

		CHECK(ladders == expected);
	}

	SECTION("Length 6: hello -> sadge") {
		const auto expected = std::vector<std::vector<std::string>>{
			{"hello", "sello", "sallo", "sadlo", "sadgo", "sadge"},
		};

		auto const ladders = word_ladder::generate("hello", "sadge", lexicon);

		CHECK(ladders == expected);
	}
}

TEST_CASE("TWO SOLUTIONS - no cycles: hello -> sadge") {
	auto const lexicon = std::unordered_set<std::string>{
		"hello",
		"sello",
		"sallo", "selle",
		"sadlo", "selge",
		"sadgo", "sedge",
		"sadge"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"hello", "sello", "sallo", "sadlo", "sadgo", "sadge"},
		{"hello", "sello", "selle", "selge", "sedge", "sadge"}
	};

	auto const ladders = word_ladder::generate("hello", "sadge", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("TWO PATHS - lengths 3 and 4 (code prioritises shortest): cat -> cod") {
	auto const lexicon = std::unordered_set<std::string>{
		"cat",
				"dat",
		"cad","dod",
		"cod"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"cat", "cad", "cod"}
	};

	auto const ladders = word_ladder::generate("cat", "cod", lexicon);

	CHECK(ladders == expected);
}


TEST_CASE("TWO SOLUTIONS - with cycles: hello -> ladge") {
	auto const lexicon = std::unordered_set<std::string>{
		"hello",
		"sello",
		"sallo", "selle",
		"sadlo", "selge",
		"sadgo", "sedge",
		"sadge",
		"ladge"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"hello", "sello", "sallo", "sadlo", "sadgo", "sadge", "ladge"},
		{"hello", "sello", "selle", "selge", "sedge", "sadge", "ladge"}
	};

	auto const ladders = word_ladder::generate("hello", "ladge", lexicon);

	CHECK(ladders == expected);
}	

TEST_CASE("MULTIPLE SOLUTIONS - (full lexicon): work -> play") {
	auto const lexicon = word_ladder::read_lexicon("./english.txt");

	const auto expected = std::vector<std::vector<std::string>>{
		{"work", "fork", "form", "foam", "flam", "flay", "play"},
		{"work", "pork", "perk", "peak", "pean", "plan", "play"},
		{"work", "pork", "perk", "peak", "peat", "plat", "play"},
		{"work", "pork", "perk", "pert", "peat", "plat", "play"},
		{"work", "pork", "porn", "pirn", "pian", "plan", "play"},
		{"work", "pork", "port", "pert", "peat", "plat", "play"},
		{"work", "word", "wood", "pood", "plod", "ploy", "play"},
		{"work", "worm", "form", "foam", "flam", "flay", "play"},
		{"work", "worn", "porn", "pirn", "pian", "plan", "play"},
		{"work", "wort", "bort", "boat", "blat", "plat", "play"},
		{"work", "wort", "port", "pert", "peat", "plat", "play"},
		{"work", "wort", "wert", "pert", "peat", "plat", "play"}
	};

	auto const ladders = word_ladder::generate("work", "play", lexicon);

	CHECK(ladders == expected);
}