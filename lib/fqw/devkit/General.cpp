#include <General.h>

#include <Poco/JSON/Object.h>

namespace FQW::Devkit
{

void sendJsonResponse(Poco::Net::HTTPServerResponse& res,
    const std::string& status, const std::string& message)
{
    Poco::JSON::Object json;
    json.set("status", status);
    json.set("message", message);

    std::ostream& out = res.send();
    json.stringify(out);
}

} // namespace FQW::Devkit
