#include "INIFile.h"

//#include "../log/Log.h"

INISection* INIFile::AddSection(std::string key)
{
	INISection* section = new INISection(key);
	sections.push_back(section);
	return section;
}

void INIFile::Save(std::string path)
{
	std::fstream file;
	file.open(path.c_str(), std::fstream::out | std::fstream::trunc);

	for (auto section : sections)
	{
		file << "[" << section->key << "]" << std::endl;

		/*
		* TEMPORARY FIX
		*/

		/*
		if (section->tmpSaveFix)
		{
		*/
			/*
			for (auto line : section->rawLines)
			{
				file << line << std::endl;
			}
			*/
		/*
		}
		else {
		*/
			for (auto value : section->values)
			{
				if (value.first.find("##LINE##") != std::string::npos)
				{
					file << value.second << std::endl;
					continue;
				}

				file << value.first << " = " << value.second << std::endl;
			}
		//}

		file << std::endl;
	}

	file.close();
}

void INIFile::Destroy()
{
	for (auto sec : sections)
	{
		delete sec;
	}
	sections.clear();
}

bool INIFile::Read(std::string path)
{
	std::vector<std::string> lines;
	std::string secname = "";
	auto PushSection = [&lines, &secname, this]() {
		if (lines.size() == 0)
		{
			return;
		}

		auto section = AddSection(secname);

		for (auto line : lines)
		{
			if (line == lines[0]) continue;

			char firstLineChar = line[0];
			if (firstLineChar == '#') continue;

			std::string key = line.substr(0, line.find("="));
			key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

			//
			//section->values.push_back(line);
			//

			if (line.find("=") == std::string::npos)
			{
				//Log::file << "INIFile: AddLine " << line << std::endl;
				section->AddLine(line);
				continue;
			}

			std::string value = line.substr(line.find("=") + 1);

			//removes first char if its a space
			char firstChar = value[0];
			if (firstChar == ' ') value.erase(0, 1);
			
			//Log::file << "INIFile: AddString " << key << " | " << value << std::endl;
			section->AddString(key, value);
		}

		secname = "";
		lines.clear();
	};

	PushSection();

	std::ifstream infile(path);

	if (!infile.good()) return false;

	std::string line;
	while (std::getline(infile, line))
	{
		if (line.size() == 0)
		{
			PushSection();
			continue;
		}

		char szSection[100];
		if (sscanf(line.c_str(), "[%99[^]]]", szSection) == 1)
		//if (sscanf_s(line.c_str(), "[%99[^]]]", szSection, sizeof(szSection)) == 1)
		{
			PushSection();

			secname = szSection;
		}

		lines.push_back(line);
	}

	PushSection();

	return true;
}

std::vector<INISection*> INIFile::GetSections(std::string name)
{
	std::vector<INISection*> result;

	for (auto session : sections)
	{
		if (ToLower(session->key).compare(ToLower(name)) == 0)
		{
			result.push_back(session);
		}
	}

	return result;
}