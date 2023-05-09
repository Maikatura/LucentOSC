#include "Downloader.h"

#include <algorithm>
#include <fstream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


inline char asciitolower(char in) {
	if(in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

size_t write_data(char* ptr, size_t size, size_t nmemb, void* userdata) {
	std::vector<unsigned char>* buffer = (std::vector<unsigned char>*)userdata;
	size_t bytes = size * nmemb;
	buffer->insert(buffer->end(), ptr, ptr + bytes);
	return bytes;
}


std::string Downloader::Request(std::string word)
{
	//cpr::Response r = cpr::Get(cpr::Url{ "https://kick.com/" + word }, 
 //       cpr::Header{ {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"},
 //                    {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3"} });


 //   // basic init
 //   myhtml_t* myhtml = myhtml_create();
 //   myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

 //   // first tree init
 //   myhtml_tree_t* tree = myhtml_tree_create();
 //   myhtml_tree_init(tree, myhtml);

 //   // parse html
 //   myhtml_parse(tree, MyENCODING_UTF_8, r.text.c_str(), strlen(r.text.c_str()));

 //   // print result
 //   // or see serialization function with callback: myhtml_serialization_tree_callback
 //   mycore_string_raw_t str = { 0 };
 //   myhtml_serialization_tree_buffer(myhtml_tree_get_document(tree), &str);
 //   
 //   std::string returnString = std::string(str.data);

 //   // release resources
 //   mycore_string_raw_destroy(&str, false);
 //   myhtml_tree_destroy(tree);
 //   myhtml_destroy(myhtml);

	return "";
}

size_t Downloader::WriteCallback(char* buf, size_t size, size_t nmemb, std::string* data)
{
	if(data == nullptr) return 0;

	data->append(buf, size * nmemb);
	return size * nmemb;

}

