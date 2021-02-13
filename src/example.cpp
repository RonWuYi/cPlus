#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
#include <set>
#include <dirent.h>

const std::string ClearData_CCA("ClearData_CCA");
const std::string ClearData_IFCP("ClearData_IFCP");
const std::string DotPef(".pef");
const std::string DotTsc1(".tsc");
const std::string DotTsc2("Tsc");
const std::string DotTsc3("TSC");

const std::regex rgx_clear_data("lab: ([0-9A-F]{8}): (C[WP]{1}UK) : (AES|TDES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcp("lab: ([0-9A-F]{8}): (C[WP]{1}UK|OTT).?: (VAR=[10]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");

const std::regex rgx_clear_data_tdes("lab.?:.?([0-9A-F]{8}).?:.?(C[WP]{1}UK|OTT).?:.?(TDES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_aes("lab.?:.?([0-9A-F]{8}).?:.?(C[WP]{1}UK|OTT).?:.?(AES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv0("lab.?:.?([0-9A-F]{8}).?:.?(C[WP]{1}UK|OTT).?:.?(VAR=[0]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv1("lab.?:.?([0-9A-F]{8}).?:.?(C[WP]{1}UK|OTT).?:.?(VAR=[1]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");

const std::regex rgx_clear_data_tdes_cwuk("lab.?:.?([0-9A-F]{8}).?:.?(CWUK).?:.?(TDES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_aes_cwuk("lab.?:.?([0-9A-F]{8}).?:.?(CWUK).?:.?(AES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv0_cwuk("lab.?:.?([0-9A-F]{8}).?:.?(CWUK).?:.?(VAR=[0]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv1_cwuk("lab.?:.?([0-9A-F]{8}).?:.?(CWUK).?:.?(VAR=[1]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");

const std::regex rgx_clear_data_tdes_cpuk("lab.?:.?([0-9A-F]{8}).?:.?(CPUK).?:.?(TDES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_aes_cpuk("lab.?:.?([0-9A-F]{8}).?:.?(CPUK).?:.?(AES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv0_cpuk("lab.?:.?([0-9A-F]{8}).?:.?(CPUK).?:.?(VAR=[0]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv1_cpuk("lab.?:.?([0-9A-F]{8}).?:.?(CPUK).?:.?(VAR=[1]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");

const std::regex rgx_clear_data_tdes_ott("lab.?:.?([0-9A-F]{8}).?:.?(OTT).?:.?(TDES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_aes_ott("lab.?:.?([0-9A-F]{8}).?:.?(OTT).?:.?(AES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv0_ott("lab.?:.?([0-9A-F]{8}).?:.?(OTT).?:.?(VAR=[0]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv1_ott("lab.?:.?([0-9A-F]{8}).?:.?(OTT).?:.?(VAR=[1]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");

const std::regex rgx_clear_data_tdes_ta("lab.?:.?([0-9A-F]{8}).?:.?(TAUK).?:.?(TDES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_aes_ta("lab.?:.?([0-9A-F]{8}).?:.?(TAUK).?:.?(AES).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv0_ta("lab.?:.?([0-9A-F]{8}).?:.?(TAUK).?:.?(VAR=[0]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");
const std::regex rgx_clear_data_ifcpv1_ta("lab.?:.?([0-9A-F]{8}).?:.?(TAUK).?:.?(VAR=[1]).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32}).*([0-9A-F]{32})");

const std::regex cssn_num_re("lab.?:.?([0-9a-fA-F]{8}).*");
const std::regex chip_re(".*(TDES|AES).*");
const std::regex key_re(".*(CWUK|CPUK|OTT|TAUK).*");
const std::regex var_re(".*(VAR=[01]).*");

const std::regex pd_re("<PRIVATEDATA>([0-9A-F]*)<\\/PRIVATEDATA>.*");
const std::regex spe_re(".*<PERDATA>([1-9A-F]*)<\\/PERDATA>.*");

//const std::string mdefaultuk = "00000000000000000000000000000000";
//const uc_byte mdefaultuk[SECURE_CHIPSET_SIMULATOR_CSUK_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//																  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//#include ""

#define MW_UCHAR unsigned char
#define MW_CHAR char

#define MW_UINT unsigned int
#define MW_INT int

using namespace std;

typedef std::vector<std::string> VecString;

struct KeyData {
	MW_UCHAR* m_pucData;
	MW_UINT m_uiDataLength;
};

struct FileInfo
{
	unsigned int m_key_type;
	unsigned int m_chip_arg_type;
	unsigned int m_ifcp_key_type;
	unsigned int m_var_type;
};

// struct tdesAesKey
// {
// 	VecString cssnList;
// 	VecString tdesCwuk;
// 	VecString aesCwuk;

// 	VecString tdesCpuk;
// 	VecString aesCpuk;

// 	VecString tdesOttuk;
// 	VecString aesOttuk;

// 	VecString tdesTauk;
// 	VecString aesTauk;

// 	VecString privateData;
// 	VecString speData;
// };

struct tdesAesKey
{
	VecString cssnList;
	VecString cwuk;
	// VecString tdesCwuk;
	// VecString aesCwuk;

	// VecString tdesCpuk;
	// VecString aesCpuk;
	VecString cpuk;

	// VecString tdesOttuk;
	// VecString aesOttuk;

	VecString ottuk;
	// VecString tdesTauk;
	// VecString aesTauk;
	VecString tauk;
	VecString privateData;
	VecString speData;
};

struct tdesKey
{
	VecString cssnList;
	VecString tdesCwuk;

	VecString tdesCpuk;

	VecString tdesOttuk;

	VecString tdesTauk;

	VecString privateData;

	VecString speData;
};

struct AesKey
{
	VecString cssnList;
	VecString aesCwuk;

	VecString aesCpuk;

	VecString aesOttuk;

	VecString aesTauk;

	VecString privateData;

	VecString speData;
};

struct IfcpAesKey
{
	VecString cssnList;
	VecString ifcpCwuk;

	VecString ifcpCpuk;

	VecString ifcpOttuk;

	VecString ifcpTauk;
};

enum class file_type_class {
	tdes_aes,
	ifcp,
	pef,
	tsc,
	ignore
};

enum class KeyChipVar
{
	KeyType,
	ChipType,
	VarType,
	ignoreType
};

int checkChipType(const char* file, std::regex re, int sizea, int sizeb, KeyChipVar flag)
{
	std::ifstream infile(file);
	std::string line;
	std::smatch m;

	std::vector<std::string> cssn;

	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			if (std::regex_search(line, m, re))
			{
				for (auto x = m.begin(); x != m.end(); ++x)
				{
					if (x->length() == sizea || x->length() == sizeb)
					{
						cssn.push_back(x->str());
					}
				}
			}
		}
	}
	std::set<std::string> s(cssn.begin(), cssn.end());
	cssn.assign(s.begin(), s.end());
	//CWUK|CPUK|OTT|TAUK
	std::string CWUK = "CWUK";
	std::string CPUK = "CPUK";
	std::string OTT = "OTT";
	std::string TAUK = "TAUK";
	
	if (std::find(cssn.begin(), cssn.end(), CPUK)!=cssn.end() 
		&& std::find(cssn.begin(), cssn.end(), OTT) != cssn.end() 
		&& std::find(cssn.begin(), cssn.end(), TAUK) != cssn.end())
	{
		return 15;
	}
	return cssn.size();
}

file_type_class check_file_type(const char* d_name)
{
	std::size_t found_tdes_aes = static_cast<std::string>(d_name).find(ClearData_CCA);
	std::size_t found_ifcp = static_cast<std::string>(d_name).find(ClearData_IFCP);
	std::size_t found_pef = static_cast<std::string>(d_name).find(DotPef);
	std::size_t found_tsc1 = static_cast<std::string>(d_name).find(DotTsc1);
	std::size_t found_tsc2 = static_cast<std::string>(d_name).find(DotTsc2);
	std::size_t found_tsc3 = static_cast<std::string>(d_name).find(DotTsc3);

	if (found_tdes_aes != std::string::npos)
	{
		return file_type_class::tdes_aes;
	}

	else if (found_ifcp != std::string::npos)
	{
		return file_type_class::ifcp;
	}

	else if (found_pef != std::string::npos)
	{
		return file_type_class::pef;
	}
	else if (found_tsc1 != std::string::npos)
	{
		return file_type_class::tsc;
	}
	else if (found_tsc2 != std::string::npos)
	{
		return file_type_class::tsc;
	}
	else if (found_tsc3 != std::string::npos)
	{
		return file_type_class::tsc;
	}
	else
	{
		return file_type_class::ignore;
	}
}

std::vector<std::string> readFileFolder(const std::string targetFolder)
{
	DIR* dir;
	struct dirent* ent;
	std::vector<std::string> picQueue;

	if ((dir = opendir(targetFolder.c_str())) != nullptr)
	{
		while ((ent = readdir(dir)) != nullptr)
		{

			if (check_file_type(ent->d_name) != file_type_class::ignore)
			{
				std::string s(ent->d_name);
				picQueue.push_back(targetFolder + "\\" + s);
			}
		}
		closedir(dir);
		return picQueue;
	}
	else
	{
		return picQueue;
	}
}

VecString getCssnList(const char* file)
{
	std::ifstream infile(file);
	std::string line;
	std::smatch m;
	std::vector<std::string> key_data;

	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			if (std::regex_search(line, m, cssn_num_re))
			{
				key_data.push_back(m[1]);
			}
		}
	}
	sort(key_data.begin(), key_data.end());
	key_data.erase(unique(key_data.begin(), key_data.end()), key_data.end());

	return key_data;
}

int main(int argc, char** argv)
{
    std::cout << "Have " << argc << " arguments:" << '\n';
    for (int i = 0; i < argc; ++i)
    {
        std::cout << argv[i] << '\n';
    }
	std::vector<std::string> fileList;

	std::string folderPath = "C:\\Work\\project\\gitlab\\ffmpeg411sdl220932bit\\fileFolder";
	fileList = readFileFolder(folderPath);

	if (fileList.size() > 0)
	{
		std::cout << fileList.size() << '\n';
	}

	FileInfo fileinfo = FileInfo{};
	/*tdesAesKey tdesaeskey = tdesAesKey{};*/

	tdesAesKey tdesUK = tdesAesKey{};
	tdesAesKey aesUK = tdesAesKey{};

	// AesKey aesUK = AesKey{};
	IfcpAesKey ifcpaesV0UK = IfcpAesKey{};
	IfcpAesKey ifcpaesV1UK = IfcpAesKey{};

	KeyData* keydata = new KeyData{};

	VecString privateData;
	VecString speData;

	for (size_t i = 0; i < fileList.size(); ++i)
	{
		/*std::cunt*/
		std::cout << "start read " << fileList[i] << '\n';
		switch (check_file_type(fileList[i].c_str()))
		{
		case file_type_class::tdes_aes:
			/*FileInfo* fileinfo = new FileInfo();*/
			tdesUK.cssnList = getCssnList(fileList[i].c_str());

			// 0 - cwuk, 1 - cpuk, 2 - ottuk, 3 - tauk
			fileinfo.m_key_type = checkChipType(fileList[i].c_str(),
												key_re,
												3,
												4,
												KeyChipVar::KeyType);
			// 0 - tdes, 1 - aes, 2 - tdes&aes 
			fileinfo.m_chip_arg_type = checkChipType(fileList[i].c_str(),
													 chip_re,
													 3,
													 4,
													 KeyChipVar::ChipType);

			for (size_t i = 0; i < tdesUK.cssnList.size(); ++i)
			{
				/* code */
				std::cout << "cssn " << i << " is " << tdesUK.cssnList[i] << '\n';
			}
			
			break;
		default:
			break;
		}
	}
	return 0;
}