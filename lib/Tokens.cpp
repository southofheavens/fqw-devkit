#include <Tokens.h>

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <Poco/JSON/Object.h>

#include <General.h>

namespace FQW::Devkit::Tokens
{

namespace
{

const std::string key = "secret_key";

std::vector<std::string> split(const std::string & str)
{
    std::vector<std::string> result;
    std::string currentString;

    for (const char & c : str)
    {
        if (c != ' ') {
            currentString += c;
        } 
        else 
        {
            if (not currentString.empty()) 
            {
                result.push_back(currentString);
                currentString.clear();
            }
        }
    }

    if (not currentString.empty()) [[likely]] {
        result.push_back(std::move(currentString));
    }

    return result;
}

} // namespace

bool isAccessTokenValid(const std::string& token) noexcept
{
    try
    {
        Poco::JWT::Signer signer(key);
        
        Poco::JWT::Token decoded = signer.verify(token);

        Poco::Timestamp expires = decoded.getExpiration();
        Poco::Timestamp now;
        
        return expires > now;
    }
    catch (const std::exception& e) {
        return false;
    }
}

Payload extractPayload(const std::string& token)
{
    try
    {
        const Poco::JWT::Token decoded(token);

        Payload p;

        // sub
        p.sub = std::stoull(decoded.getSubject());

        // role
        const Poco::JSON::Object& payload = decoded.payload();
        if (payload.has("role")) {
            p.role = payload.getValue<std::string>("role");
        }
        else {
            throw std::runtime_error("Missing 'role' field");
        }

        // exp
        Poco::Timestamp exp = decoded.getExpiration();
        p.exp = std::chrono::seconds(exp.epochTime());

        return p;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Invalid token received");
    }
}

std::string extractTokenFromRequest(Poco::Net::HTTPServerRequest & request)
{
    if (not request.has("Authorization")) {
        throw FQW::Devkit::FQWException("Unauthorized", Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);        
    }

    std::vector<std::string> tokenInfo = split(request.get("Authorization"));

    if (tokenInfo.size() != 2) {
        throw FQW::Devkit::FQWException("Authorization header must have 2 words: 'Bearer' and the token itself", 
            Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    }
    if (tokenInfo[0] != "Bearer") {
        throw FQW::Devkit::FQWException("Expected 'Bearer' authorization token", 
            Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
    }
    if (not FQW::Devkit::Tokens::isAccessTokenValid(tokenInfo[1])) {
        throw FQW::Devkit::FQWException("Invalid access token", 
            Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
    }

    return tokenInfo[1];
}

} // namespace FQW::Devkit::Tokens

