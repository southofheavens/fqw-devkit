#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <exception>
#include <string>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace RGT::Devkit
{

/**
 * В библиотеке Poco у методов и функций отсутствует квалификатор noexcept, поэтому очень тяжело
 * отследить самому выбрасывает ли функция исключения или нет. Для перестраховки перехватывается
 * два типа исключений: RGTException - исключение выбрасывается для предусмотренных ошибок (например, 
 * от пользователя ожидается логин и пароль в теле json, а что-то из этого отсутствует) и ... для 
 * непредусмотренных исключений, которые могут вылететь из "недр" других функций. Конструктор RGTException 
 * принимает std::string errorMessage - сообщение об ошибке и Poco::Net::HTTPResponse::HTTPStatus - код
 * http-ответа, эти данные будут отправлены клиенту. Если исключение перехватит блок catch (...),
 * то клиент получит код 500 - HTTP_INTERNAL_SERVER_ERROR и сообщение "Internal server error."
 */
class RGTException : public std::exception 
{
public:
    RGTException(const std::string & errorMessage, Poco::Net::HTTPResponse::HTTPStatus httpStatus) 
        : errorMessage_{errorMessage}, httpStatus_{httpStatus} {}

    const char * what() const noexcept final 
    { return errorMessage_.c_str(); }

    Poco::Net::HTTPResponse::HTTPStatus status() const noexcept
    { return httpStatus_; }

private:
    std::string                         errorMessage_;
    Poco::Net::HTTPResponse::HTTPStatus httpStatus_;
};



/**
 * Отправляет клиенту ответ со статусом status и сообщением message 
 */
void sendJsonResponse(Poco::Net::HTTPServerResponse & res,
    const std::string & status, const std::string & message);

} // namespace RGT::Devkit

#endif // __GENERAL_H__
