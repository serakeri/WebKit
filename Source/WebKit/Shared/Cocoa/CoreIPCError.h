/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if PLATFORM(COCOA)

#import <CoreFoundation/CoreFoundation.h>
#import <wtf/TZoneMalloc.h>
#import <wtf/text/WTFString.h>

OBJC_CLASS NSError;

namespace WebKit {

class CoreIPCError {
    WTF_MAKE_TZONE_ALLOCATED(CoreIPCError);
public:
    static bool hasValidUserInfo(const RetainPtr<CFDictionaryRef>&);

    CoreIPCError(CoreIPCError&&) = default;
    CoreIPCError& operator=(CoreIPCError&&) = default;

    CoreIPCError(NSError *);
    CoreIPCError(String&& domain, int64_t code, RetainPtr<CFDictionaryRef>&& userInfo, std::unique_ptr<CoreIPCError>&& underlyingError)
        : m_domain(WTFMove(domain))
        , m_code(WTFMove(code))
        , m_userInfo(WTFMove(userInfo))
        , m_underlyingError(WTFMove(underlyingError))
    {
    }

    RetainPtr<id> toID() const;

    String domain() const
    {
        return m_domain;
    }

    int64_t code() const
    {
        return m_code;
    }

    RetainPtr<CFDictionaryRef> userInfo() const
    {
        return m_userInfo;
    }

    const std::unique_ptr<CoreIPCError>& underlyingError() const
    {
        return m_underlyingError;
    }

private:
    bool isSafeToEncodeUserInfo(id value);

    String m_domain;
    int64_t m_code;
    RetainPtr<CFDictionaryRef> m_userInfo;
    std::unique_ptr<CoreIPCError> m_underlyingError;
};

}

#endif // PLATFORM(COCOA)
