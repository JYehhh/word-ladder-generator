#include "word_ladder.h"
#include <__iterator/counted_iterator.h>

#include <iostream>
#include <ostream>
#include <queue>

// Please note: it's not good practice to test your code via a main function that does
//  printing. Instead, you should be using your test folder. This file should only really
//  be used for more "primitive" debugging as we know that working solely with test
//  frameworks might be overwhelming for some.

auto main() -> int {
	auto const english_lexicon = std::unordered_set<std::string>{
		"bend",
		"bent",
		"abjs",
		"bble",
		"able"
	};

    auto const ladders = ::word_ladder::generate("bend", "able", english_lexicon);

    for (auto ladder : ladders) {
        std::cout << "This current ladder contains: "; 
        for (auto word : ladder) {
            std::cout << word << ", ";
        }
        std::cout << std::endl;
    }
	// auto const lexicon = std::unordered_set<std::string>{
	// 	"bend",
	// 	"bent",
	// 	"abjs",
	// 	"bble",
	// 	"able"
	// };

	// const auto expected = std::vector<std::vector<std::string>>{
		
	// };

	// auto const ladders = word_ladder::generate("bend", "able", lexicon);


	// std::queue<int> test;

	// test.push(1);
	// test.push(1);
	// test.push(1);
	// test.push(1);
	// test.push(1);
	// std::cout << test.size() << std::endl;
	// test.pop();
	// test.pop();
	// test.pop();
	// std::cout << test.size() << std::endl;
}
