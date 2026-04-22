//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "Main.h"
//#include "HttpClass.h"
#include "base64.h"

bool avaiableNow = true;
int DownProgress = 0, DownStatus = 0;
std::string LatestDownloadedString;


bool ProgressProc(double progress)
{
	wprintf(L"Current progress: %-.1f%%\r\n", progress);
	return true;
}

struct UrlComponents {
	std::string host;
	std::string path;
};

UrlComponents parse_url(const std::string& url) {
	std::regex url_regex(R"(^(http[s]?:\/\/)?([^\/\s]+)(\/.*)?$)");
	std::smatch url_match_result;

	UrlComponents components;

	if (std::regex_match(url, url_match_result, url_regex)) {
		components.host = url_match_result[1].str() + url_match_result[2].str();
		components.path = url_match_result[3].str();
		if (components.path.empty()) {
			components.path = "/";
		}
	}
	else {
		components.host = url;
		components.path = "/";
	}

	return components;
}


std::string SendHttpPostRequest(const char* url, const char* data)
{
	if (!url || url[0] == '\0' || !data)
		return "[ERROR] Bad hostname or request";

	UrlComponents uril = parse_url(url);
	try
	{
		httplib::Client client(uril.host);

		client.set_follow_location(true);
		client.set_read_timeout(20, 0);
		client.set_write_timeout(20, 0);
		client.set_connection_timeout(10, 0);

		auto res = client.Post(uril.path, data, "text/plain");

		if (res)
		{
			if (res->status == httplib::StatusCode::OK_200)
			{
				DownStatus = 1;
				return res->body;
			}
			else
			{
				auto err = res.error();
				DownStatus = -1;
				return "[ERROR] HTTP error: " + httplib::to_string(err) + " status " + std::to_string(res->status);
			}
		}
		else
		{
			DownStatus = -1;
			return "[ERROR] No response";
		}
	}
	catch (std::exception& ex)
	{
		DownStatus = -3;
		return ex.what() ? ex.what() : "";
	}
	catch (...)
	{

	}
	DownStatus = -2;
	return "[ERROR] Unhandled error";
}

bool SendHttpGetRequest(const char* host, const char* path)
{
	if (!host || !path || !*host || !*path) {
#ifdef _DEBUG
		OutputDebugStringA("[WebHelper] ERROR: Invalid host or path parameter\n");
#endif
		return false;
	}

	if (strlen(host) > 256 || strlen(path) > 1024) {
#ifdef _DEBUG
		char dbg[512];
		sprintf_s(dbg, "[WebHelper] ERROR: Suspicious string length: host=%zu, path=%zu\n",
			strlen(host), strlen(path));
		OutputDebugStringA(dbg);
#endif
		return false;
	}
	try
	{
		httplib::Client client(host);

		auto res = client.Get(path);

		client.set_follow_location(true);
		client.set_read_timeout(20, 0);
		client.set_write_timeout(20, 0);
		client.set_connection_timeout(10, 0);

		if (res)
		{
			if (res->status == httplib::StatusCode::OK_200)
			{
				DownStatus = 1;
				return res->body;
			}
			else
			{
				auto err = res.error();
				DownStatus = -1;
				return "[ERROR] HTTP error: " + httplib::to_string(err) + " status " + std::to_string(res->status);
			}
		}
		else
		{
			DownStatus = -1;
			return "[ERROR] No response";
		}
	}
	catch (std::exception& ex)
	{
		DownStatus = -3;
		return ex.what() ? ex.what() : "";
	}
	catch (...)
	{

	}

	DownStatus = -2;
	return "[ERROR] Unhandled error";
}


void DownloadNewMapToFile(const char* szUrl, const char* filepath)
{
	if (FileExist(filepath))
	{
		DownStatus = 2;
		return;
	}
	DownProgress = 0;
	DownStatus = -1;

	try
	{
		httplib::Client client(szUrl);

		auto progress_callback = [&](uint64_t current, uint64_t total) {
			if (total > 0) {
				DownProgress = (int)((static_cast<double>(current) / total) * 100.0);
			}
			return true;
			};

		auto res = client.Get("/", progress_callback);

		if (res && (res->status == httplib::MovedPermanently_301 || res->status == httplib::PermanentRedirect_308))
		{
			client.set_follow_location(true);
			res = client.Get("/", progress_callback);
		}

		if (res && res->status == httplib::StatusCode::OK_200)
		{
			std::ofstream ofs(filepath, std::ios::binary);
			if (ofs.is_open())
			{
				ofs.write(res->body.c_str(), res->body.size());
				ofs.close();
				DownStatus = 1;
				DownProgress = 100;
				return;
			}
			else
			{
				DownStatus = -1;
				DownProgress = 0;
			}
		}
		if (res)
		{
			auto err = res.error();
			LatestDownloadedString = "[ERROR] HTTP error: " + httplib::to_string(err).substr(0, 64);
			DownStatus = -1;
			DownProgress = 0;
		}
	}
	catch (...)
	{
	}
	DownStatus = -2;
	DownProgress = 0;
}

int __stdcall SendGetRequest(const char* url, const  char* path)
{
	if (!avaiableNow)
		return 0;
	LatestDownloadedString = "[ERROR] you need wait response!";
	avaiableNow = false;
	DownProgress = 0;
	DownStatus = 0;

	if (SetInfoObjDebugVal)
	{
		PrintText(("Send host:" + std::string(url) + ". Path:").c_str());
		PrintText(std::string(path).c_str());
	}

	std::string _url = url;
	std::string _path = path;

	std::thread([_url, _path]() {
		try
		{
			LatestDownloadedString = SendHttpGetRequest(_url, _path);
		}
		catch (...)
		{
			DownStatus = -1;
		}
		avaiableNow = true;
		}).detach();
	return 1;
}

int __stdcall SendPostRequest(const char* url, const  char* request)
{
	if (!avaiableNow)
		return 0;
	LatestDownloadedString = "[ERROR] You need wait response!";
	avaiableNow = false;
	DownProgress = 0;
	DownStatus = 0;
	std::string _url = url;
	std::string _request = request;

	std::thread([_url, _request]() {
		try
		{
			LatestDownloadedString = SendHttpPostRequest(_url, _request);
		}
		catch (...)
		{
			DownStatus = -1;
		}
		avaiableNow = true;
		}).detach();

	return 0;
}


int __stdcall SendPostRequestEx(const char* url, const char* path, const  char* request)
{
	if (!avaiableNow)
		return 0;
	LatestDownloadedString = "[ERROR] You need wait response!";
	avaiableNow = false;
	DownProgress = 0;
	DownStatus = 0;
	std::string _url = url;
	std::string _path = path;
	std::string _request = request;

	std::thread([_url, _path, _request]() {
		try
		{
			LatestDownloadedString = SendHttpPostRequest((_url + _path).c_str(), _request);
		}
		catch (...)
		{
			DownStatus = -1;
		}
		avaiableNow = true;
		}).detach();

	return 1;
}

int __stdcall SaveNewDotaVersionFromUrl(const  char* url, const  char* path)
{
	if (!avaiableNow)
		return 0;
	avaiableNow = false;
	DownProgress = 0;
	DownStatus = 0;

	std::string _url = url;
	std::string _path = path;

	std::thread([_url, _path]() {
		DownloadNewMapToFile(_url, _path);
		avaiableNow = true;
		}).detach();

	return 1;
}

int __stdcall GetDownloadStatus(int)
{
	return DownStatus;
}

int __stdcall GetDownloadProgress(int)
{
	return DownProgress;
}

const char* __stdcall GetLatestDownloadedString(int)
{
	if (LatestDownloadedString.size() > 1023)
		LatestDownloadedString = LatestDownloadedString.substr(0, 1023);
	if (SetInfoObjDebugVal)
	{
		const std::string& str = LatestDownloadedString;
		size_t length = str.length();
		size_t pos = 0;
		size_t chunkSize = 100;

		while (pos < length)
		{
			std::string chunk = str.substr(pos, chunkSize);
			if (pos)
				PrintText(("Recv data:" + chunk).c_str());
			else 
				PrintText(chunk.c_str());
			pos += chunkSize;
		}
	}
	return LatestDownloadedString.c_str();
}

//
//fs::file_time_type GetLastFileTime( const std::vector<std::string> & files )
//{
//	fs::file_time_type retval = fs::file_time_type( );
//	std::string file = files
//	for ( auto s : files )
//	{
//		auto curfiletime = fs::last_write_time( s );
//		if ( curfiletime > retval )
//		{
//			retval = curfiletime;
//		}
//	}
//
//}

bool IsOkayLogFile(std::string file)
{
	try
	{
		if (file.size() && fs::path(file).extension().string() == ".txt")
		{
			std::string filedata = ToLower(GetFileContent(file));
			if (filedata.length() > 0)
			{
				if (strstr(filedata.c_str(), ToLower("DotAAllstarsHelper").c_str()))
				{
					return true;
				}
				if (strstr(filedata.c_str(), ToLower("DotA Allstars").c_str()))
				{
					return true;
				}
			}
		}
	}
	catch (...)
	{

	}
	return false;
}


int __stdcall SendLatestError(const char* url)
{
	auto Errors = get_file_list("Errors", true);
	std::error_code err{};
	std::string sendfilename = "Errors\\lastcheck.txt";
	auto LatestError = FileExist(sendfilename.c_str()) ?
		fs::last_write_time(sendfilename, err) : fs::file_time_type();
	bool foundlatesterror = false;

	if (SetInfoObjDebugVal)
	{
		if (FileExist(sendfilename.c_str()))
		{
			PrintText("Found lastcheck.txt");
		}
		else
		{
			PrintText("lastcheck.txt not found");
		}
	}

	for (auto& s : Errors)
	{

		if (IsOkayLogFile(s))
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Process file:" + s);
			}
			auto curfiletime = fs::last_write_time(s,err);
			if (curfiletime > LatestError)
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("Found new file!");
				}
				LatestError = curfiletime;
				sendfilename = s;
				foundlatesterror = true;
			}
		}
	}

	if (foundlatesterror)
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Found error: " + sendfilename);
		}
		std::string filedata = GetFileContent(sendfilename);
		std::string encodedfiledata;
		if (!Base64::Encode(filedata, &encodedfiledata))
		{
			SendPostRequest(url, encodedfiledata.c_str());
			if (SetInfoObjDebugVal)
			{
				PrintText("Error success sent!");
			}
		}
		else
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Error send file! Encode Base64 problem.");
			}
		}
	}
	else
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Error not found!");
		}
	}

	std::ofstream newFile("Errors\\lastcheck.txt");
	newFile << " Just empty file ";
	newFile.flush();
	newFile.close();

	return 1;
}
