#include "except.h"

#include <exception>
#include <iostream>

namespace renderer {
void React() {
    auto eptr = std::current_exception();
    try {
        std::rethrow_exception(eptr);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка " << e.what() << "\n";
    } catch (...) {
        std::cerr << "Неизвестное исключение \n";
    }
}
}  // namespace renderer
