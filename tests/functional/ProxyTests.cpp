#include <iostream>

// Include before boost::log headers
#include "restc-cpp/logging.h"

#include "restc-cpp/restc-cpp.h"
#include "restc-cpp/RequestBuilder.h"

#include "restc-cpp/test_helper.h"
#include "lest/lest.hpp"

using namespace std;
using namespace restc_cpp;


static const string defunct_proxy_address = GetDockerUrl("http://localhost:0");
static const string proxy_address = GetDockerUrl("http://localhost:3003");

const lest::test specification[] = {

STARTCASE(TestFailToConnect)
{
    Request::Properties properties;
    properties.proxy.type = Request::Proxy::Type::HTTP;
    properties.proxy.address = defunct_proxy_address;

    // Create the client with our configuration
    auto rest_client = RestClient::Create(properties);


    EXPECT_THROWS(rest_client->ProcessWithPromise([&](Context& ctx) {
        auto reply = RequestBuilder(ctx)
            .Get("http://api.example.com/normal/posts/1")
            .Execute();

    }).get());
} ENDCASE

STARTCASE(TestWithProxy)
{
    Request::Properties properties;
    properties.proxy.type = Request::Proxy::Type::HTTP;
    properties.proxy.address = proxy_address;

    // Create the client with our configuration
    auto rest_client = RestClient::Create(properties);

    rest_client->ProcessWithPromise([&](Context& ctx) {
        auto reply = RequestBuilder(ctx)
            .Get("http://api.example.com/normal/posts/1")
            .Execute();

            cout << "Got: " << reply->GetBodyAsString() << endl;
    }).get();
} ENDCASE

}; //lest


int main( int argc, char * argv[] )
{
    RESTC_CPP_TEST_LOGGING_SETUP("debug");

    return lest::run( specification, argc, argv );
}
