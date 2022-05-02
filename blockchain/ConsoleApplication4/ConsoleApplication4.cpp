// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <iostream>
#include<filesystem>

using namespace utility; // Common utilities like string conversions
using namespace web; // Common features like URIs.
using namespace web::http; // Common HTTP functionality
using namespace web::http::client; // HTTP client features
using namespace concurrency::streams; // Asynchronous streams
using namespace std;

void getgenesis()
{
	// Create http_client to send the request.
	http_client client(U("http://ergast.com/api/f1/2004/1"));

	// Build request URI and start the request.
	uri_builder builder(U("/results.json"));
	client.request(methods::GET, builder.to_string())


		// Handle response headers arriving.
		.then([](http_response response) -> pplx::task<json::value>
	{
		// if the status is OK extract the body of the response into a JSON value
		// works only when the content type is application\json
		
			if (response.status_code() == status_codes::OK)
					{
						printf("Received response status code:%u\n", response.status_code());
						response.headers().set_content_type(L"application/json");

						return response.extract_json();
					}

			else {
				printf("Invalid Response\n");
				// return an empty JSON value
				return pplx::task_from_result(json::value());
			}
		



	})
		.then([](pplx::task<json::value> previousTask)
	{
		// get the JSON value from the task and display content from it
		try
		{
			//get json value from response
			const json::value val = previousTask.get();
			string_t s = val.serialize();
			wcout << (L"Extracted json data from webservice") << endl;
			//Display extracted json from webservice
			wcout << s;
			std::filesystem::path filePath(L"response.json"); // Assuming wide chars here. Could be U instead of L depending on your setup
			std::wofstream outputFile(filePath);
			outputFile << s.c_str();
			outputFile.close();
		}
		catch (const std::exception& e)
		{
			printf("Error Exception:%s\n", e.what());
		}
	})
		.wait();

}
void gethealth()
{
	// Create http_client to send the request.
	http_client client(U("https://algoexplorer.io/api-dev/v2"));

	// Build request URI and start the request.
	uri_builder builder(U("/health"));
	client.request(methods::GET, builder.to_string())


		// Handle response headers arriving.
		.then([](http_response response) -> pplx::task<json::value>
	{
		// if the status is OK extract the body of the response into a JSON value
		// works only when the content type is application\json
		if (response.status_code() == status_codes::OK)
		{
			printf("Received response status code:%u\n", response.status_code());
			response.headers().set_content_type(L"application/json");

			return response.extract_json();
		}
		else {
			printf("Invalid Response\n");
			// return an empty JSON value
			return pplx::task_from_result(json::value());
		}



	})
		.then([](pplx::task<json::value> previousTask)
	{
		// get the JSON value from the task and display content from it
		try
		{
			//get json value from response
			const json::value val = previousTask.get();

			wcout << (L"Extracted json data from webservice") << endl;
			//Display extracted json from webservice
			wcout << val.serialize();



		}
		catch (const std::exception& e)
		{
			printf("Error Exception:%s\n", e.what());
		}
	})
		.wait();

}
int main(int argc, char* argv[])
{
	getgenesis();
	gethealth();
    return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu



// Tips for Getting Started:
// 1. Use the Solution Explorer window to add/manage files
// 2. Use the Team Explorer window to connect to source control
// 3. Use the Output window to see build output and other messages
// 4. Use the Error List window to view errors
// 5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
// 6. In the future, to open this project again, go to File > Open > Project and select the .sln file