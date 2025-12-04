//
// Created by kali on 9/2/25.
//

#include "ParseColor.h"


int64_t parseInt(const std::string& str, uint8_t base = 10) {
    return strtoll(str.c_str(), nullptr, base);
}
optional<Color> parseColor(const std::string& str) {
    if (str.length() == 3) {
        int64_t iv = parseInt(str, 16);
        if (!(iv >= 0 && iv <= 0xfff)) {
            return {};
        } else {
            return {{
                static_cast<uint8_t>(((iv & 0xf00) >> 4) | ((iv & 0xf00) >> 8)),
                static_cast<uint8_t>((iv & 0xf0) | ((iv & 0xf0) >> 4)),
                static_cast<uint8_t>((iv & 0xf) | ((iv & 0xf) << 4)),
                255
            }};
        }
    } else if (str.length() == 4) {
        int64_t iv = parseInt(str, 16);
        if (!(iv >= 0 && iv <= 0xfff)) {
            return {};
        } else {
            return {{
                static_cast<uint8_t>(((iv & 0xf000) >> 8) | ((iv & 0xf000) >> 12)),
                static_cast<uint8_t>(((iv & 0xf00) >> 4) | ((iv & 0xf00) >> 8)),
                static_cast<uint8_t>((iv & 0xf0) | ((iv & 0xf0) >> 4)),
                static_cast<uint8_t>((iv & 0xf) | ((iv & 0xf) << 4)),
            }};
        }
    } else if (str.length() == 6) {
        int64_t iv = parseInt(str, 16);
        if (!(iv >= 0 && iv <= 0xffffff)) {
            return {};  // Covers NaN.
        } else {
            return {{
                static_cast<uint8_t>((iv & 0xff0000) >> 16),
                static_cast<uint8_t>((iv & 0xff00) >> 8),
                static_cast<uint8_t>(iv & 0xff),
                255
            }};
        }
    }else if (str.length() == 8) {
        int64_t iv = parseInt(str, 16);
        if (!(iv >= 0 && iv <= 0xffffff)) {
            return {};  // Covers NaN.
        } else {
            return {{
                static_cast<uint8_t>((iv & 0xff000000) >> 24),
                static_cast<uint8_t>((iv & 0xff0000) >> 16),
                static_cast<uint8_t>((iv & 0xff00) >> 8),
                static_cast<uint8_t>(iv & 0xff)
            }};
        }
    }

    return {};
}