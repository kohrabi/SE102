#include <Windows.h>

#include "Utils.h"

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	size_t newsize = st.length() + 1;
	vector<wchar_t> buffer(newsize);
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, buffer.data(), newsize, st.c_str(), _TRUNCATE);
	return wstring(buffer.data());
}

///*
//	Convert char* string to wchar_t* string.
//*/
//LPCWSTR ToLPCWSTR(string st)
//{
//	const char *str = st.c_str();
//
//	size_t newsize = strlen(str) + 1;
//	wchar_t * wcstring = new wchar_t[newsize];
//	size_t convertedChars = 0;
//	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);
//
//	wstring *w = new wstring(wcstring);
//
//	// delete wcstring   // << can I ? 
//	return w->c_str();
//}

wstring string_to_wstring(const string& str)
{
	return wstring(str.begin(), str.end());
}

//string wstring_to_string(const wstring& str)
//{
//	return string(str.begin(), str.end());
//}
