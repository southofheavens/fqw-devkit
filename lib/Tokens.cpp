#include <Tokens.h>

#include <jwt-cpp/jwt.h>

namespace Devkit::Tokens
{

namespace
{

    const std::string key_ = "secret_key";

    bool verifyAccessToken(const std::string& token) noexcept
    {
        try
        {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{key_});
            
            verifier.verify(decoded);
            return true;
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
        auto decoded = jwt::decode(token);
        
        Payload p;
        p.sub = std::stoull(decoded.get_subject());
        p.role = decoded.get_payload_claim("role").as_string();
        p.exp = std::chrono::duration_cast<std::chrono::seconds>(decoded.get_expires_at().time_since_epoch());

        return p;
    }
    catch (const std::exception&) {
        throw std::runtime_error("Invalid token received");
    }
}


} // namespace Devkit::Tokens
