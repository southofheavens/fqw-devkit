#ifndef __TOKENS_H__
#define __TOKENS_H__

#include <string>
#include <chrono>

namespace Devkit::Tokens
{

/**
 * @brief Проверяет является ли access токен валидным
 * @details Токен является валидным в том случае, если у него корректная подпись и он не просрочен
 * @param token Токен
 * @return true, если токен валидный, в противном случае false
 */
bool isAccessTokenValid(const std::string& token) noexcept;

/**
 * @brief Проверяет является ли refresh токен валидным
 * @details Токен является валидным в том случае, если он найден в БД
 * @param token Токен
 * @return true, если токен валидный, в противном случае false
 */
bool isRefreshTokenValid(const std::string& token /*, подключение_к_бд  */) noexcept;



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

} // namespace Devkit::Tokens

#endif // __TOKENS_H__
