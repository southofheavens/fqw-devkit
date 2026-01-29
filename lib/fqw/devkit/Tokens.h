#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <string>
#include <chrono>

#include <Poco/Net/HTTPServerRequest.h>

namespace FQW::Devkit::Tokens
{

/**
 * @brief Проверяет является ли access токен валидным
 * @note Токен является валидным в том случае, если у него корректная подпись и он не просрочен
 * @param token Токен
 * @return true, если токен валидный, в противном случае false
 */
bool isAccessTokenValid(const std::string& token) noexcept;

/**
 * @brief Полезная нагрузка access токена
 */
struct Payload 
{
    uint64_t             sub;
    std::string          role;
    std::chrono::seconds exp;
};

/**
 * @brief Извлекает полезную нагрузку из access токена
 * @param token Токен
 * @throw std::runtime_error если получен невалиданый токен
 * @return Полезная нагрузка
 */
Payload extractPayload(const std::string& token);

// Извлекает Bearer access-токен из запроса
// Функция ответственна за обработку ошибок при извлечении токена
// 
// Если в запросе отсутствует заголовок Authorization 
// или тип токена не Bearer 
// или токен невалидный (неверная подпись || просрочен),
// то будет выброшено исключение FQWException
std::string extractTokenFromRequest(Poco::Net::HTTPServerRequest & request);

} // namespace FQW::Devkit::Tokens

#endif // __TOKENS_H__

