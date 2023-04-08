#pragma once

namespace App::Str
{
inline std::wstring Widen(const std::string& aStr)
{
    if (aStr.empty())
        return L"";

    auto length = MultiByteToWideChar(CP_UTF8, 0, aStr.data(), static_cast<int32_t>(aStr.size()), nullptr, 0);

    if (length <= 0)
        return L"";

    std::wstring result(length, 0);
    length = MultiByteToWideChar(CP_UTF8, 0, aStr.data(), static_cast<int32_t>(aStr.size()), result.data(),
                                 static_cast<int32_t>(result.size()));

    if (length <= 0)
        return L"";

    return result;
}
}
