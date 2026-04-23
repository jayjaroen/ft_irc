#include "parser.hpp"

size_t	Command::lexer(const std::string &input, const std::string &icomm, std::vector<std::vector<std::string> > &output)
{
	size_t	last = icomm.length();
	std::vector<std::string>	content;

	while (input[last])
	{
		if (isspace(static_cast<unsigned char>(input[last])))
			last++;
		else if (input[last] == ':')
		{
			last++;
			while (input[last] && isspace(static_cast<unsigned char>(input[last])))
				last++;
			content.push_back(input.substr(last));
			if (content.empty())
				return FAILURE;
			last = input.length();
		}
		else
		{
			while (input[last] && !isspace(static_cast<unsigned char>(input[last])))
			{
				size_t	init = last;
				while (input[last] && !isspace(static_cast<unsigned char>(input[last])) && input[last] != ',')
					last++;
				content.push_back(input.substr(init, last - init));
				if (content.empty())
					return FAILURE;
				if (input[last] && input[last] == ',')
					last++;
			}
		}
		if (!(content.empty()))
		{
			output.push_back(content);
			content.clear();
		}
	}
	return SUCCESS;
}
