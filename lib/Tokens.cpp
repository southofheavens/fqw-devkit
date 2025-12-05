#include <Tokens.h>

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <Poco/JSON/Object.h>

namespace FQW::Devkit::Tokens
{

namespace
{

const std::string key_ = "secret_key";

bool verifyAccessToken(const std::string& token) noexcept
{
    try
    {
        Poco::JWT::Signer signer(key_);
        
        Poco::JWT::Token decoded = signer.verify(token);

        Poco::Timestamp expires = decoded.getExpiration();
        Poco::Timestamp now;
        
        return expires > now;
    }
    catch (const std::exception& e) {
        return false;
    }
}

} // namespace

bool isAccessTokenValid(const std::string& token) noexcept
{
    if (not verifyAccessToken(token)) {
        return false;
    }
}

bool isRefreshTokenValid(const std::string& hashed_token /*, подключение_к_бд  */) noexcept
{

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

} // namespace FQW::Devkit::Tokens
