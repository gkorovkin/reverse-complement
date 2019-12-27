#include <unordered_map>
#include <iostream>
#include <vector>
#include <deque>
#include <string_view>

constexpr std::uint32_t column_width = 60;

std::vector<std::pair<char, char>> codetable_src = {
	{ 'A', 'T' },
	{ 'C', 'G' },
	{ 'G', 'C' },
	{ 'T', 'A' },
	{ 'U', 'A' },
	{ 'M', 'K' },
	{ 'R', 'Y' },
	{ 'Y', 'R' },
	{ 'K', 'M' },
	{ 'V', 'B' },
	{ 'H', 'D' },
	{ 'D', 'H' },
	{ 'B', 'V' },
	{ 'N', 'N' },
	{ 'W', 'W' },
	{ 'S', 'S' }
};

struct marker_t
{
	marker_t(const std::string& descr) : description(descr) {}

	std::string description;
	std::string marker;
};

std::ostream& operator<<(std::ostream& strm, const marker_t& m)
{
	strm << m.description << '\n';

	std::uint32_t whole_chunks = m.marker.size() / column_width;
	std::uint32_t leftovers = m.marker.size() % column_width;

	for (std::int32_t idx = 0; idx < whole_chunks; ++idx)
	{
		std::string_view str_view(m.marker.c_str() + column_width * idx, column_width);
		std::cout << str_view << '\n';
	}

	if (0 != leftovers)
	{
		std::string_view str_view(m.marker.c_str() + column_width * whole_chunks, leftovers);
		std::cout << str_view << '\n';
	}

	return strm;
}

int main()
{
	//no conversions for iostreams
	char const* oldLocale = setlocale(LC_ALL,"C");

	// comment to use the default setting
	bool oldSyncSetting = std::ios_base::sync_with_stdio(false);

	std::unordered_map<char, char> codetable;

	for (const auto &itm : codetable_src)
	{
		codetable[ itm.first ] = itm.second;
		codetable[ static_cast<char>(std::tolower(itm.first)) ] = itm.second;
	}

	std::deque<marker_t> markers;
	auto current_itr = markers.begin();

	for(std::string line; getline(std::cin, line);)
	{
		if (line.empty())
			continue;

		if (line[0] == '>')
			current_itr = markers.emplace(markers.end(), marker_t {line} );
		else
			current_itr->marker.append({line.c_str(), line.size()});
	}

	for (auto& m : markers)
	{
		auto begin_itr = m.marker.begin();
		auto end_itr = m.marker.end();
		--end_itr;

		while( (end_itr - begin_itr) > 0)
		{
			//std::cout << static_cast<char>(codetable[*begin_itr]) << " < " << *begin_itr << std::endl;
			if (codetable.end() == codetable.find(*begin_itr))
			{
				std::cerr << "Cannot find " << *begin_itr << std::endl;
				return -1;
			}

			*begin_itr = static_cast<char>(codetable[*begin_itr]);
			*end_itr = static_cast<char>(codetable[*end_itr]);

			std::swap(*begin_itr, *end_itr);

			++begin_itr;
			--end_itr;
		}
		std::cout << m;
	}
}
